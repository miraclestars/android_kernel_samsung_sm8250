// SPDX-License-Identifier: GPL-2.0+
/*
 * u_serial.c - utilities for USB gadget "serial port"/TTY support
 *
 * Copyright (C) 2003 Al Borchers (alborchers@steinerpoint.com)
 * Copyright (C) 2008 David Brownell
 * Copyright (C) 2008 by Nokia Corporation
 *
 * This code also borrows from usbserial.c, which is
 * Copyright (C) 1999 - 2002 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2000 Peter Berger (pberger@brimson.com)
 * Copyright (C) 2000 Al Borchers (alborchers@steinerpoint.com)
 */

/* #define VERBOSE_DEBUG */

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/module.h>
#include <linux/console.h>
#include <linux/kthread.h>
#include <linux/kfifo.h>

#include "u_serial.h"


/*
 * This component encapsulates the TTY layer glue needed to provide basic
 * "serial port" functionality through the USB gadget stack.  Each such
 * port is exposed through a /dev/ttyGS* node.
 *
 * After this module has been loaded, the individual TTY port can be requested
 * (gserial_alloc_line()) and it will stay available until they are removed
 * (gserial_free_line()). Each one may be connected to a USB function
 * (gserial_connect), or disconnected (with gserial_disconnect) when the USB
 * host issues a config change event. Data can only flow when the port is
 * connected to the host.
 *
 * A given TTY port can be made available in multiple configurations.
 * For example, each one might expose a ttyGS0 node which provides a
 * login application.  In one case that might use CDC ACM interface 0,
 * while another configuration might use interface 3 for that.  The
 * work to handle that (including descriptor management) is not part
 * of this component.
 *
 * Configurations may expose more than one TTY port.  For example, if
 * ttyGS0 provides login service, then ttyGS1 might provide dialer access
 * for a telephone or fax link.  And ttyGS2 might be something that just
 * needs a simple byte stream interface for some messaging protocol that
 * is managed in userspace ... OBEX, PTP, and MTP have been mentioned.
 *
 *
 * gserial is the lifecycle interface, used by USB functions
 * gs_port is the I/O nexus, used by the tty driver
 * tty_struct links to the tty/filesystem framework
 *
 * gserial <---> gs_port ... links will be null when the USB link is
 * inactive; managed by gserial_{connect,disconnect}().  each gserial
 * instance can wrap its own USB control protocol.
 *	gserial->ioport == usb_ep->driver_data ... gs_port
 *	gs_port->port_usb ... gserial
 *
 * gs_port <---> tty_struct ... links will be null when the TTY file
 * isn't opened; managed by gs_open()/gs_close()
 *	gserial->port_tty ... tty_struct
 *	tty_struct->driver_data ... gserial
 */

/* RX and TX queues can buffer QUEUE_SIZE packets before they hit the
 * next layer of buffering.  For TX that's a circular buffer; for RX
 * consider it a NOP.  A third layer is provided by the TTY code.
 */
#define QUEUE_SIZE		16
#define WRITE_BUF_SIZE		8192		/* TX only */
#define GS_CONSOLE_BUF_SIZE	8192

/* console info */
struct gscons_info {
	struct gs_port		*port;
	struct task_struct	*console_thread;
	struct kfifo		con_buf;
	/* protect the buf and busy flag */
	spinlock_t		con_lock;
	int			req_busy;
	struct usb_request	*console_req;
};

/*
 * The port structure holds info for each port, one for each minor number
 * (and thus for each /dev/ node).
 */
struct gs_port {
	struct tty_port		port;
	spinlock_t		port_lock;	/* guard port_* access */

	struct gserial		*port_usb;

	bool			openclose;	/* open/close in progress */
	u8			port_num;

	struct list_head	read_pool;
	int read_started;
	int read_allocated;
	struct list_head	read_queue;
	unsigned		n_read;
	struct tasklet_struct	push;

	struct list_head	write_pool;
	int write_started;
	int write_allocated;
	struct kfifo		port_write_buf;
	wait_queue_head_t	drain_wait;	/* wait while writes drain */
	bool                    write_busy;
	wait_queue_head_t	close_wait;

	/* REVISIT this state ... */
	struct usb_cdc_line_coding port_line_coding;	/* 8-N-1 etc */
};

static struct portmaster {
	struct mutex	lock;			/* protect open/close */
	struct gs_port	*port;
} ports[MAX_U_SERIAL_PORTS];

#define GS_CLOSE_TIMEOUT		15		/* seconds */



#ifdef VERBOSE_DEBUG
#ifndef pr_vdebug
#define pr_vdebug(fmt, arg...) \
	pr_debug(fmt, ##arg)
#endif /* pr_vdebug */
#else
#ifndef pr_vdebug
#define pr_vdebug(fmt, arg...) \
	({ if (0) pr_debug(fmt, ##arg); })
#endif /* pr_vdebug */
#endif

/*-------------------------------------------------------------------------*/

/* I/O glue between TTY (upper) and USB function (lower) driver layers */

/*
 * gs_alloc_req
 *
 * Allocate a usb_request and its buffer.  Returns a pointer to the
 * usb_request or NULL if there is an error.
 */
struct usb_request *
gs_alloc_req(struct usb_ep *ep, unsigned len, gfp_t kmalloc_flags)
{
	struct usb_request *req;

	req = usb_ep_alloc_request(ep, kmalloc_flags);

	if (req != NULL) {
		req->length = len;
		req->buf = kmalloc(len, kmalloc_flags);
		if (req->buf == NULL) {
			usb_ep_free_request(ep, req);
			return NULL;
		}
	}

	return req;
}
EXPORT_SYMBOL_GPL(gs_alloc_req);

/*
 * gs_free_req
 *
 * Free a usb_request and its buffer.
 */
void gs_free_req(struct usb_ep *ep, struct usb_request *req)
{
	kfree(req->buf);
	usb_ep_free_request(ep, req);
}
EXPORT_SYMBOL_GPL(gs_free_req);

/*
 * gs_send_packet
 *
 * If there is data to send, a packet is built in the given
 * buffer and the size is returned.  If there is no data to
 * send, 0 is returned.
 *
 * Called with port_lock held.
 */
static unsigned
gs_send_packet(struct gs_port *port, char *packet, unsigned size)
{
	unsigned len;

	len = kfifo_len(&port->port_write_buf);
	if (len < size)
		size = len;
	if (size != 0)
		size = kfifo_out(&port->port_write_buf, packet, size);
	return size;
}

/*
 * gs_start_tx
 *
 * This function finds available write requests, calls
 * gs_send_packet to fill these packets with data, and
 * continues until either there are no more write requests
 * available or no more data to send.  This function is
 * run whenever data arrives or write requests are available.
 *
 * Context: caller owns port_lock; port_usb is non-null.
 */
static int gs_start_tx(struct gs_port *port)
/*
__releases(&port->port_lock)
__acquires(&port->port_lock)
*/
{
	struct list_head	*pool = &port->write_pool;
	struct usb_ep		*in;
	int			status = 0;
	bool			do_tty_wake = false;

	if (!port->port_usb)
		return status;

	in = port->port_usb->in;

	while (!port->write_busy && !list_empty(pool)) {
		struct usb_request	*req;
		int			len;

		if (port->write_started >= QUEUE_SIZE)
			break;

		req = list_entry(pool->next, struct usb_request, list);
		len = gs_send_packet(port, req->buf, in->maxpacket);
		if (len == 0) {
			wake_up_interruptible(&port->drain_wait);
			break;
		}
		do_tty_wake = true;

		req->length = len;
		list_del(&req->list);
		req->zero = kfifo_is_empty(&port->port_write_buf);

		pr_vdebug("ttyGS%d: tx len=%d, 0x%02x 0x%02x 0x%02x ...\n",
			  port->port_num, len, *((u8 *)req->buf),
			  *((u8 *)req->buf+1), *((u8 *)req->buf+2));

		/* Drop lock while we call out of driver; completions
		 * could be issued while we do so.  Disconnection may
		 * happen too; maybe immediately before we queue this!
		 *
		 * NOTE that we may keep sending data for a while after
		 * the TTY closed (dev->ioport->port_tty is NULL).
		 */
		port->write_busy = true;
		spin_unlock(&port->port_lock);
		status = usb_ep_queue(in, req, GFP_ATOMIC);
		spin_lock(&port->port_lock);
		port->write_busy = false;

		if (status) {
			pr_debug("%s: %s %s err %d\n",
					__func__, "queue", in->name, status);
			list_add(&req->list, pool);
			break;
		}

		port->write_started++;

		/* abort immediately after disconnect */
		if (!port->port_usb)
			break;
	}

	if (do_tty_wake && port->port.tty)
		tty_wakeup(port->port.tty);
	return status;
}

/*
 * Context: caller owns port_lock, and port_usb is set
 */
static unsigned gs_start_rx(struct gs_port *port)
/*
__releases(&port->port_lock)
__acquires(&port->port_lock)
*/
{
	struct list_head	*pool = &port->read_pool;
	struct usb_ep		*out = port->port_usb->out;

	while (!list_empty(pool)) {
		struct usb_request	*req;
		int			status;
		struct tty_struct	*tty;

		/* no more rx if closed */
		tty = port->port.tty;
		if (!tty)
			break;

		if (port->read_started >= QUEUE_SIZE)
			break;

		req = list_entry(pool->next, struct usb_request, list);
		list_del(&req->list);
		req->length = out->maxpacket;

		/* drop lock while we call out; the controller driver
		 * may need to call us back (e.g. for disconnect)
		 */
		spin_unlock(&port->port_lock);
		status = usb_ep_queue(out, req, GFP_ATOMIC);
		spin_lock(&port->port_lock);

		if (status) {
			pr_debug("%s: %s %s err %d\n",
					__func__, "queue", out->name, status);
			list_add(&req->list, pool);
			break;
		}
		port->read_started++;

		/* abort immediately after disconnect */
		if (!port->port_usb)
			break;
	}
	return port->read_started;
}

/*
 * RX tasklet takes data out of the RX queue and hands it up to the TTY
 * layer until it refuses to take any more data (or is throttled back).
 * Then it issues reads for any further data.
 *
 * If the RX queue becomes full enough that no usb_request is queued,
 * the OUT endpoint may begin NAKing as soon as its FIFO fills up.
 * So QUEUE_SIZE packets plus however many the FIFO holds (usually two)
 * can be buffered before the TTY layer's buffers (currently 64 KB).
 */
static void gs_rx_push(unsigned long _port)
{
	struct gs_port		*port = (void *)_port;
	struct tty_struct	*tty;
	struct list_head	*queue = &port->read_queue;
	bool			disconnect = false;
	bool			do_push = false;

	/* hand any queued data to the tty */
	spin_lock_irq(&port->port_lock);
	tty = port->port.tty;
	while (!list_empty(queue)) {
		struct usb_request	*req;

		req = list_first_entry(queue, struct usb_request, list);

		/* leave data queued if tty was rx throttled */
		if (tty && tty_throttled(tty))
			break;

		switch (req->status) {
		case -ESHUTDOWN:
			disconnect = true;
			pr_vdebug("ttyGS%d: shutdown\n", port->port_num);
			break;

		default:
			/* presumably a transient fault */
			pr_warn("ttyGS%d: unexpected RX status %d\n",
				port->port_num, req->status);
			/* FALLTHROUGH */
		case 0:
			/* normal completion */
			break;
		}

		/* push data to (open) tty */
		if (req->actual && tty) {
			char		*packet = req->buf;
			unsigned	size = req->actual;
			unsigned	n;
			int		count;

			/* we may have pushed part of this packet already... */
			n = port->n_read;
			if (n) {
				packet += n;
				size -= n;
			}

			count = tty_insert_flip_string(&port->port, packet,
					size);
			if (count)
				do_push = true;
			if (count != size) {
				/* stop pushing; TTY layer can't handle more */
				port->n_read += count;
				pr_vdebug("ttyGS%d: rx block %d/%d\n",
					  port->port_num, count, req->actual);
				break;
			}
			port->n_read = 0;
		}

		list_move(&req->list, &port->read_pool);
		port->read_started--;
	}

	/* Push from tty to ldisc; this is handled by a workqueue,
	 * so we won't get callbacks and can hold port_lock
	 */
	if (do_push)
		tty_flip_buffer_push(&port->port);


	/* We want our data queue to become empty ASAP, keeping data
	 * in the tty and ldisc (not here).  If we couldn't push any
	 * this time around, there may be trouble unless there's an
	 * implicit tty_unthrottle() call on its way...
	 *
	 * REVISIT we should probably add a timer to keep the tasklet
	 * from starving ... but it's not clear that case ever happens.
	 */
	if (!list_empty(queue) && tty) {
		if (!tty_throttled(tty)) {
			if (do_push)
				tasklet_schedule(&port->push);
			else
				pr_warn("ttyGS%d: RX not scheduled?\n",
					port->port_num);
		}
	}

	/* If we're still connected, refill the USB RX queue. */
	if (!disconnect && port->port_usb)
		gs_start_rx(port);

	spin_unlock_irq(&port->port_lock);
}

static void gs_read_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct gs_port	*port = ep->driver_data;

	/* Queue all received data until the tty layer is ready for it. */
	spin_lock(&port->port_lock);
	list_add_tail(&req->list, &port->read_queue);
	tasklet_schedule(&port->push);
	spin_unlock(&port->port_lock);
}

static void gs_write_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct gs_port	*port = ep->driver_data;

	spin_lock(&port->port_lock);
	list_add(&req->list, &port->write_pool);
	port->write_started--;

	switch (req->status) {
	default:
		/* presumably a transient fault */
		pr_warn("%s: unexpected %s status %d\n",
			__func__, ep->name, req->status);
		/* FALL THROUGH */
	case 0:
		/* normal completion */
		gs_start_tx(port);
		break;

	case -ESHUTDOWN:
		/* disconnect */
		pr_vdebug("%s: %s shutdown\n", __func__, ep->name);
		break;
	}

	spin_unlock(&port->port_lock);
}

static void gs_free_requests(struct usb_ep *ep, struct list_head *head,
							 int *allocated)
{
	struct usb_request	*req;

	while (!list_empty(head)) {
		req = list_entry(head->next, struct usb_request, list);
		list_del(&req->list);
		gs_free_req(ep, req);
		if (allocated)
			(*allocated)--;
	}
}

static int gs_alloc_requests(struct usb_ep *ep, struct list_head *head,
		void (*fn)(struct usb_ep *, struct usb_request *),
		int *allocated)
{
	int			i;
	struct usb_request	*req;
	int n = allocated ? QUEUE_SIZE - *allocated : QUEUE_SIZE;

	/* Pre-allocate up to QUEUE_SIZE transfers, but if we can't
	 * do quite that many this time, don't fail ... we just won't
	 * be as speedy as we might otherwise be.
	 */
	for (i = 0; i < n; i++) {
		req = gs_alloc_req(ep, ep->maxpacket, GFP_ATOMIC);
		if (!req)
			return list_empty(head) ? -ENOMEM : 0;
		req->complete = fn;
		list_add_tail(&req->list, head);
		if (allocated)
			(*allocated)++;
	}
	return 0;
}

/**
 * gs_start_io - start USB I/O streams
 * @dev: encapsulates endpoints to use
 * Context: holding port_lock; port_tty and port_usb are non-null
 *
 * We only start I/O when something is connected to both sides of
 * this port.  If nothing is listening on the host side, we may
 * be pointlessly filling up our TX buffers and FIFO.
 */
static int gs_start_io(struct gs_port *port)
{
	struct list_head	*head = &port->read_pool;
	struct usb_ep		*ep = port->port_usb->out;
	int			status;
	unsigned		started;

	/* Allocate RX and TX I/O buffers.  We can't easily do this much
	 * earlier (with GFP_KERNEL) because the requests are coupled to
	 * endpoints, as are the packet sizes we'll be using.  Different
	 * configurations may use different endpoints with a given port;
	 * and high speed vs full speed changes packet sizes too.
	 */
	if (!ep->enabled || !port->port_usb->in->enabled) {
		pr_err("%s: ep is disabled.\n", __func__);
		return -ENODEV;
	}

	status = gs_alloc_requests(ep, head, gs_read_complete,
		&port->read_allocated);
	if (status)
		return status;

	status = gs_alloc_requests(port->port_usb->in, &port->write_pool,
			gs_write_complete, &port->write_allocated);
	if (status) {
		gs_free_requests(ep, head, &port->read_allocated);
		return status;
	}

	/* queue read requests */
	port->n_read = 0;
	started = gs_start_rx(port);

	/* unblock any pending writes into our circular buffer */
	if (started && port->port.tty) {
		tty_wakeup(port->port.tty);
	} else {
		gs_free_requests(ep, head, &port->read_allocated);
		gs_free_requests(port->port_usb->in, &port->write_pool,
			&port->write_allocated);
		status = -EIO;
	}

	return status;
}

/*-------------------------------------------------------------------------*/

/* TTY Driver */

/*
 * gs_open sets up the link between a gs_port and its associated TTY.
 * That link is broken *only* by TTY close(), and all driver methods
 * know that.
 */
static int gs_open(struct tty_struct *tty, struct file *file)
{
	int		port_num = tty->index;
	struct gs_port	*port;
	int		status;

	do {
		mutex_lock(&ports[port_num].lock);
		port = ports[port_num].port;
		if (!port)
			status = -ENODEV;
		else {
			spin_lock_irq(&port->port_lock);

			/* already open?  Great. */
			if (port->port.count) {
				status = 0;
				port->port.count++;

			/* currently opening/closing? wait ... */
			} else if (port->openclose) {
				status = -EBUSY;

			/* ... else we do the work */
			} else {
				status = -EAGAIN;
				port->openclose = true;
			}
			spin_unlock_irq(&port->port_lock);
		}
		mutex_unlock(&ports[port_num].lock);

		switch (status) {
		default:
			/* fully handled */
			return status;
		case -EAGAIN:
			/* must do the work */
			break;
		case -EBUSY:
			/* wait for EAGAIN task to finish */
			msleep(1);
			/* REVISIT could have a waitchannel here, if
			 * concurrent open performance is important
			 */
			break;
		}
	} while (status != -EAGAIN);

	/* Do the "real open" */
	spin_lock_irq(&port->port_lock);

	/* allocate circular buffer on first open */
	if (!kfifo_initialized(&port->port_write_buf)) {

		spin_unlock_irq(&port->port_lock);
		status = kfifo_alloc(&port->port_write_buf,
				     WRITE_BUF_SIZE, GFP_KERNEL);
		spin_lock_irq(&port->port_lock);

		if (status) {
			pr_debug("gs_open: ttyGS%d (%p,%p) no buffer\n",
				port->port_num, tty, file);
			port->openclose = false;
			goto exit_unlock_port;
		}
	}

	/* REVISIT if REMOVED (ports[].port NULL), abort the open
	 * to let rmmod work faster (but this way isn't wrong).
	 */

	/* REVISIT maybe wait for "carrier detect" */

	tty->driver_data = port;
	port->port.tty = tty;

	port->port.count = 1;
	port->openclose = false;

	/* if connected, start the I/O stream */
	if (port->port_usb) {
		struct gserial	*gser = port->port_usb;

		pr_debug("gs_open: start ttyGS%d\n", port->port_num);
		gs_start_io(port);

		if (gser->connect)
			gser->connect(gser);
	}

	pr_debug("gs_open: ttyGS%d (%p,%p)\n", port->port_num, tty, file);

	status = 0;

exit_unlock_port:
	spin_unlock_irq(&port->port_lock);
	return status;
}

static int gs_writes_finished(struct gs_port *p)
{
	int cond;

	/* return true on disconnect or empty buffer */
	spin_lock_irq(&p->port_lock);
	cond = (p->port_usb == NULL) || !kfifo_len(&p->port_write_buf);
	spin_unlock_irq(&p->port_lock);

	return cond;
}

static void gs_close(struct tty_struct *tty, struct file *file)
{
	struct gs_port *port = tty->driver_data;
	struct gserial	*gser;

	spin_lock_irq(&port->port_lock);

	if (port->port.count != 1) {
		if (port->port.count == 0)
			WARN_ON(1);
		else
			--port->port.count;
		goto exit;
	}

	pr_debug("gs_close: ttyGS%d (%p,%p) ...\n", port->port_num, tty, file);

	/* mark port as closing but in use; we can drop port lock
	 * and sleep if necessary
	 */
	port->openclose = true;
	port->port.count = 0;

	gser = port->port_usb;
	if (gser && gser->disconnect)
		gser->disconnect(gser);

	/* wait for circular write buffer to drain, disconnect, or at
	 * most GS_CLOSE_TIMEOUT seconds; then discard the rest
	 */
	if (kfifo_len(&port->port_write_buf) > 0 && gser) {
		spin_unlock_irq(&port->port_lock);
		wait_event_interruptible_timeout(port->drain_wait,
					gs_writes_finished(port),
					GS_CLOSE_TIMEOUT * HZ);
		spin_lock_irq(&port->port_lock);
		gser = port->port_usb;
	}

	/* Iff we're disconnected, there can be no I/O in flight so it's
	 * ok to free the circular buffer; else just scrub it.  And don't
	 * let the push tasklet fire again until we're re-opened.
	 */
	if (gser == NULL)
		kfifo_free(&port->port_write_buf);
	else
		kfifo_reset(&port->port_write_buf);

	port->port.tty = NULL;

	port->openclose = false;

	pr_debug("gs_close: ttyGS%d (%p,%p) done!\n",
			port->port_num, tty, file);

	wake_up(&port->close_wait);
exit:
	spin_unlock_irq(&port->port_lock);
}

static int gs_write(struct tty_struct *tty, const unsigned char *buf, int count)
{
	struct gs_port	*port = tty->driver_data;
	unsigned long	flags;

	pr_vdebug("gs_write: ttyGS%d (%p) writing %d bytes\n",
			port->port_num, tty, count);

	spin_lock_irqsave(&port->port_lock, flags);
	if (count)
		count = kfifo_in(&port->port_write_buf, buf, count);
	/* treat count == 0 as flush_chars() */
	if (port->port_usb)
		gs_start_tx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return count;
}

static int gs_put_char(struct tty_struct *tty, unsigned char ch)
{
	struct gs_port	*port = tty->driver_data;
	unsigned long	flags;
	int		status;

	pr_vdebug("gs_put_char: (%d,%p) char=0x%x, called from %ps\n",
		port->port_num, tty, ch, __builtin_return_address(0));

	spin_lock_irqsave(&port->port_lock, flags);
	status = kfifo_put(&port->port_write_buf, ch);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return status;
}

static void gs_flush_chars(struct tty_struct *tty)
{
	struct gs_port	*port = tty->driver_data;
	unsigned long	flags;

	pr_vdebug("gs_flush_chars: (%d,%p)\n", port->port_num, tty);

	spin_lock_irqsave(&port->port_lock, flags);
	if (port->port_usb)
		gs_start_tx(port);
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static int gs_write_room(struct tty_struct *tty)
{
	struct gs_port	*port = tty->driver_data;
	unsigned long	flags;
	int		room = 0;

	spin_lock_irqsave(&port->port_lock, flags);
	if (port->port_usb)
		room = kfifo_avail(&port->port_write_buf);
	spin_unlock_irqrestore(&port->port_lock, flags);

	pr_vdebug("gs_write_room: (%d,%p) room=%d\n",
		port->port_num, tty, room);

	return room;
}

static int gs_chars_in_buffer(struct tty_struct *tty)
{
	struct gs_port	*port = tty->driver_data;
	unsigned long	flags;
	int		chars = 0;

	spin_lock_irqsave(&port->port_lock, flags);
	chars = kfifo_len(&port->port_write_buf);
	spin_unlock_irqrestore(&port->port_lock, flags);

	pr_vdebug("gs_chars_in_buffer: (%d,%p) chars=%d\n",
		port->port_num, tty, chars);

	return chars;
}

/* undo side effects of setting TTY_THROTTLED */
static void gs_unthrottle(struct tty_struct *tty)
{
	struct gs_port		*port = tty->driver_data;
	unsigned long		flags;

	spin_lock_irqsave(&port->port_lock, flags);
	if (port->port_usb) {
		/* Kickstart read queue processing.  We don't do xon/xoff,
		 * rts/cts, or other handshaking with the host, but if the
		 * read queue backs up enough we'll be NAKing OUT packets.
		 */
		tasklet_schedule(&port->push);
		pr_vdebug("ttyGS%d: unthrottle\n", port->port_num);
	}
	spin_unlock_irqrestore(&port->port_lock, flags);
}

static int gs_break_ctl(struct tty_struct *tty, int duration)
{
	struct gs_port	*port = tty->driver_data;
	int		status = 0;
	struct gserial	*gser;

	pr_vdebug("gs_break_ctl: ttyGS%d, send break (%d) \n",
			port->port_num, duration);

	spin_lock_irq(&port->port_lock);
	gser = port->port_usb;
	if (gser && gser->send_break)
		status = gser->send_break(gser, duration);
	spin_unlock_irq(&port->port_lock);

	return status;
}

static const struct tty_operations gs_tty_ops = {
	.open =			gs_open,
	.close =		gs_close,
	.write =		gs_write,
	.put_char =		gs_put_char,
	.flush_chars =		gs_flush_chars,
	.write_room =		gs_write_room,
	.chars_in_buffer =	gs_chars_in_buffer,
	.unthrottle =		gs_unthrottle,
	.break_ctl =		gs_break_ctl,
};

/*-------------------------------------------------------------------------*/

static struct tty_driver *gs_tty_driver;

#ifdef CONFIG_U_SERIAL_CONSOLE

static struct gscons_info gscons_info;
static struct console gserial_cons;

static struct usb_request *gs_request_new(struct usb_ep *ep)
{
	struct usb_request *req = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (!req)
		return NULL;

	req->buf = kmalloc(ep->maxpacket, GFP_ATOMIC);
	if (!req->buf) {
		usb_ep_free_request(ep, req);
		return NULL;
	}

	return req;
}

static void gs_request_free(struct usb_request *req, struct usb_ep *ep)
{
	if (!req)
		return;

	kfree(req->buf);
	usb_ep_free_request(ep, req);
}

static void gs_complete_out(struct usb_ep *ep, struct usb_request *req)
{
	struct gscons_info *info = &gscons_info;

	switch (req->status) {
	default:
		pr_warn("%s: unexpected %s status %d\n",
			__func__, ep->name, req->status);
		/* fall through */
	case 0:
		/* normal completion */
		spin_lock(&info->con_lock);
		info->req_busy = 0;
		spin_unlock(&info->con_lock);

		wake_up_process(info->console_thread);
		break;
	case -ESHUTDOWN:
		/* disconnect */
		pr_vdebug("%s: %s shutdown\n", __func__, ep->name);
		break;
	}
}

static int gs_console_connect(int port_num)
{
	struct gscons_info *info = &gscons_info;
	struct gs_port *port;
	struct usb_ep *ep;

	if (port_num != gserial_cons.index) {
		pr_err("%s: port num [%d] is not support console\n",
		       __func__, port_num);
		return -ENXIO;
	}

	port = ports[port_num].port;
	ep = port->port_usb->in;
	if (!info->console_req) {
		info->console_req = gs_request_new(ep);
		if (!info->console_req)
			return -ENOMEM;
		info->console_req->complete = gs_complete_out;
	}

	info->port = port;
	spin_lock(&info->con_lock);
	info->req_busy = 0;
	spin_unlock(&info->con_lock);
	pr_vdebug("port[%d] console connect!\n", port_num);
	return 0;
}

static void gs_console_disconnect(struct usb_ep *ep)
{
	struct gscons_info *info = &gscons_info;
	struct usb_request *req = info->console_req;

	gs_request_free(req, ep);
	info->console_req = NULL;
}

static int gs_console_thread(void *data)
{
	struct gscons_info *info = &gscons_info;
	struct gs_port *port;
	struct usb_request *req;
	struct usb_ep *ep;
	int xfer, ret, count, size;

	do {
		port = info->port;
		set_current_state(TASK_INTERRUPTIBLE);
		if (!port || !port->port_usb
		    || !port->port_usb->in || !info->console_req)
			goto sched;

		req = info->console_req;
		ep = port->port_usb->in;

		spin_lock_irq(&info->con_lock);
		count = kfifo_len(&info->con_buf);
		size = ep->maxpacket;

		if (count > 0 && !info->req_busy) {
			set_current_state(TASK_RUNNING);
			if (count < size)
				size = count;

			xfer = kfifo_out(&info->con_buf, req->buf, size);
			req->length = xfer;

			spin_unlock(&info->con_lock);
			ret = usb_ep_queue(ep, req, GFP_ATOMIC);
			spin_lock(&info->con_lock);
			if (ret < 0)
				info->req_busy = 0;
			else
				info->req_busy = 1;

			spin_unlock_irq(&info->con_lock);
		} else {
			spin_unlock_irq(&info->con_lock);
sched:
			if (kthread_should_stop()) {
				set_current_state(TASK_RUNNING);
				break;
			}
			schedule();
		}
	} while (1);

	return 0;
}

static int gs_console_setup(struct console *co, char *options)
{
	struct gscons_info *info = &gscons_info;
	int status;

	info->port = NULL;
	info->console_req = NULL;
	info->req_busy = 0;
	spin_lock_init(&info->con_lock);

	status = kfifo_alloc(&info->con_buf, GS_CONSOLE_BUF_SIZE, GFP_KERNEL);
	if (status) {
		pr_err("%s: allocate console buffer failed\n", __func__);
		return status;
	}

	info->console_thread = kthread_create(gs_console_thread,
					      co, "gs_console");
	if (IS_ERR(info->console_thread)) {
		pr_err("%s: cannot create console thread\n", __func__);
		kfifo_free(&info->con_buf);
		return PTR_ERR(info->console_thread);
	}
	wake_up_process(info->console_thread);

	return 0;
}

static void gs_console_write(struct console *co,
			     const char *buf, unsigned count)
{
	struct gscons_info *info = &gscons_info;
	unsigned long flags;

	spin_lock_irqsave(&info->con_lock, flags);
	kfifo_in(&info->con_buf, buf, count);
	spin_unlock_irqrestore(&info->con_lock, flags);

	wake_up_process(info->console_thread);
}

static struct tty_driver *gs_console_device(struct console *co, int *index)
{
	struct tty_driver **p = (struct tty_driver **)co->data;

	if (!*p)
		return NULL;

	*index = co->index;
	return *p;
}

static struct console gserial_cons = {
	.name =		"ttyGS",
	.write =	gs_console_write,
	.device =	gs_console_device,
	.setup =	gs_console_setup,
	.flags =	CON_PRINTBUFFER,
	.index =	-1,
	.data =		&gs_tty_driver,
};

static void gserial_console_init(void)
{
	register_console(&gserial_cons);
}

static void gserial_console_exit(void)
{
	struct gscons_info *info = &gscons_info;

	unregister_console(&gserial_cons);
	if (!IS_ERR_OR_NULL(info->console_thread))
		kthread_stop(info->console_thread);
	kfifo_free(&info->con_buf);
}

#else

static int gs_console_connect(int port_num)
{
	return 0;
}

static void gs_console_disconnect(struct usb_ep *ep)
{
}

static void gserial_console_init(void)
{
}

static void gserial_console_exit(void)
{
}

#endif

static int
gs_port_alloc(unsigned port_num, struct usb_cdc_line_coding *coding)
{
	struct gs_port	*port;
	int		ret = 0;

	mutex_lock(&ports[port_num].lock);
	if (ports[port_num].port) {
		ret = -EBUSY;
		goto out;
	}

	port = kzalloc(sizeof(struct gs_port), GFP_KERNEL);
	if (port == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	tty_port_init(&port->port);
	spin_lock_init(&port->port_lock);
	init_waitqueue_head(&port->drain_wait);
	init_waitqueue_head(&port->close_wait);

	tasklet_init(&port->push, gs_rx_push, (unsigned long) port);

	INIT_LIST_HEAD(&port->read_pool);
	INIT_LIST_HEAD(&port->read_queue);
	INIT_LIST_HEAD(&port->write_pool);

	port->port_num = port_num;
	port->port_line_coding = *coding;

	ports[port_num].port = port;
out:
	mutex_unlock(&ports[port_num].lock);
	return ret;
}

static int gs_closed(struct gs_port *port)
{
	int cond;

	spin_lock_irq(&port->port_lock);
	cond = (port->port.count == 0) && !port->openclose;
	spin_unlock_irq(&port->port_lock);
	return cond;
}

static void gserial_free_port(struct gs_port *port)
{
	tasklet_kill(&port->push);
	/* wait for old opens to finish */
	wait_event(port->close_wait, gs_closed(port));
	WARN_ON(port->port_usb != NULL);
	tty_port_destroy(&port->port);
	kfree(port);
}

void gserial_free_line(unsigned char port_num)
{
	struct gs_port	*port;

	mutex_lock(&ports[port_num].lock);
	if (WARN_ON(!ports[port_num].port)) {
		mutex_unlock(&ports[port_num].lock);
		return;
	}
	port = ports[port_num].port;
	ports[port_num].port = NULL;
	mutex_unlock(&ports[port_num].lock);

	gserial_free_port(port);
	tty_unregister_device(gs_tty_driver, port_num);
	gserial_console_exit();
}
EXPORT_SYMBOL_GPL(gserial_free_line);

int gserial_alloc_line(unsigned char *line_num)
{
	struct usb_cdc_line_coding	coding;
	struct device			*tty_dev;
	int				ret;
	int				port_num;

	coding.dwDTERate = cpu_to_le32(9600);
	coding.bCharFormat = 8;
	coding.bParityType = USB_CDC_NO_PARITY;
	coding.bDataBits = USB_CDC_1_STOP_BITS;

	for (port_num = 0; port_num < MAX_U_SERIAL_PORTS; port_num++) {
		ret = gs_port_alloc(port_num, &coding);
		if (ret == -EBUSY)
			continue;
		if (ret)
			return ret;
		break;
	}
	if (ret)
		return ret;

	/* ... and sysfs class devices, so mdev/udev make /dev/ttyGS* */

	tty_dev = tty_port_register_device(&ports[port_num].port->port,
			gs_tty_driver, port_num, NULL);
	if (IS_ERR(tty_dev)) {
		struct gs_port	*port;
		pr_err("%s: failed to register tty for port %d, err %ld\n",
				__func__, port_num, PTR_ERR(tty_dev));

		ret = PTR_ERR(tty_dev);
		mutex_lock(&ports[port_num].lock);
		port = ports[port_num].port;
		ports[port_num].port = NULL;
		mutex_unlock(&ports[port_num].lock);
		gserial_free_port(port);
		goto err;
	}
	*line_num = port_num;
	gserial_console_init();
err:
	return ret;
}
EXPORT_SYMBOL_GPL(gserial_alloc_line);

/**
 * gserial_connect - notify TTY I/O glue that USB link is active
 * @gser: the function, set up with endpoints and descriptors
 * @port_num: which port is active
 * Context: any (usually from irq)
 *
 * This is called activate endpoints and let the TTY layer know that
 * the connection is active ... not unlike "carrier detect".  It won't
 * necessarily start I/O queues; unless the TTY is held open by any
 * task, there would be no point.  However, the endpoints will be
 * activated so the USB host can perform I/O, subject to basic USB
 * hardware flow control.
 *
 * Caller needs to have set up the endpoints and USB function in @dev
 * before calling this, as well as the appropriate (speed-specific)
 * endpoint descriptors, and also have allocate @port_num by calling
 * @gserial_alloc_line().
 *
 * Returns negative errno or zero.
 * On success, ep->driver_data will be overwritten.
 */
int gserial_connect(struct gserial *gser, u8 port_num)
{
	struct gs_port	*port;
	unsigned long	flags;
	int		status;

	if (port_num >= MAX_U_SERIAL_PORTS)
		return -ENXIO;

	port = ports[port_num].port;
	if (!port) {
		pr_err("serial line %d not allocated.\n", port_num);
		return -EINVAL;
	}
	if (port->port_usb) {
		pr_err("serial line %d is in use.\n", port_num);
		return -EBUSY;
	}

	/* activate the endpoints */
	status = usb_ep_enable(gser->in);
	if (status < 0)
		return status;
	gser->in->driver_data = port;

	status = usb_ep_enable(gser->out);
	if (status < 0)
		goto fail_out;
	gser->out->driver_data = port;

	/* then tell the tty glue that I/O can work */
	spin_lock_irqsave(&port->port_lock, flags);
	gser->ioport = port;
	port->port_usb = gser;

	/* REVISIT unclear how best to handle this state...
	 * we don't really couple it with the Linux TTY.
	 */
	gser->port_line_coding = port->port_line_coding;

	/* REVISIT if waiting on "carrier detect", signal. */

	/* if it's already open, start I/O ... and notify the serial
	 * protocol about open/close status (connect/disconnect).
	 */
	if (port->port.count) {
		pr_debug("gserial_connect: start ttyGS%d\n", port->port_num);
		gs_start_io(port);
		if (gser->connect)
			gser->connect(gser);
	} else {
		if (gser->disconnect)
			gser->disconnect(gser);
	}

	status = gs_console_connect(port_num);
	spin_unlock_irqrestore(&port->port_lock, flags);

	return status;

fail_out:
	usb_ep_disable(gser->in);
	return status;
}
EXPORT_SYMBOL_GPL(gserial_connect);
/**
 * gserial_disconnect - notify TTY I/O glue that USB link is inactive
 * @gser: the function, on which gserial_connect() was called
 * Context: any (usually from irq)
 *
 * This is called to deactivate endpoints and let the TTY layer know
 * that the connection went inactive ... not unlike "hangup".
 *
 * On return, the state is as if gserial_connect() had never been called;
 * there is no active USB I/O on these endpoints.
 */
void gserial_disconnect(struct gserial *gser)
{
	struct gs_port	*port = gser->ioport;
	unsigned long	flags;

	if (!port)
		return;

	/* tell the TTY glue not to do I/O here any more */
	spin_lock_irqsave(&port->port_lock, flags);

	/* REVISIT as above: how best to track this? */
	port->port_line_coding = gser->port_line_coding;

	port->port_usb = NULL;
	gser->ioport = NULL;
	if (port->port.count > 0 || port->openclose) {
		wake_up_interruptible(&port->drain_wait);
		if (port->port.tty)
			tty_hangup(port->port.tty);
	}
	spin_unlock_irqrestore(&port->port_lock, flags);

	/* disable endpoints, aborting down any active I/O */
	usb_ep_disable(gser->out);
	usb_ep_disable(gser->in);

	/* finally, free any unused/unusable I/O buffers */
	spin_lock_irqsave(&port->port_lock, flags);
	if (port->port.count == 0 && !port->openclose)
		kfifo_free(&port->port_write_buf);
	gs_free_requests(gser->out, &port->read_pool, NULL);
	gs_free_requests(gser->out, &port->read_queue, NULL);
	gs_free_requests(gser->in, &port->write_pool, NULL);

	port->read_allocated = port->read_started =
		port->write_allocated = port->write_started = 0;

	gs_console_disconnect(gser->in);
	spin_unlock_irqrestore(&port->port_lock, flags);
}
EXPORT_SYMBOL_GPL(gserial_disconnect);

static int userial_init(void)
{
	unsigned			i;
	int				status;

	gs_tty_driver = alloc_tty_driver(MAX_U_SERIAL_PORTS);
	if (!gs_tty_driver)
		return -ENOMEM;

	gs_tty_driver->driver_name = "g_serial";
	gs_tty_driver->name = "ttyGS";
	/* uses dynamically assigned dev_t values */

	gs_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	gs_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	gs_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
	gs_tty_driver->init_termios = tty_std_termios;

	/* 9600-8-N-1 ... matches defaults expected by "usbser.sys" on
	 * MS-Windows.  Otherwise, most of these flags shouldn't affect
	 * anything unless we were to actually hook up to a serial line.
	 */
	gs_tty_driver->init_termios.c_cflag =
			B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	gs_tty_driver->init_termios.c_ispeed = 9600;
	gs_tty_driver->init_termios.c_ospeed = 9600;

	tty_set_operations(gs_tty_driver, &gs_tty_ops);
	for (i = 0; i < MAX_U_SERIAL_PORTS; i++)
		mutex_init(&ports[i].lock);

	/* export the driver ... */
	status = tty_register_driver(gs_tty_driver);
	if (status) {
		pr_err("%s: cannot register, err %d\n",
				__func__, status);
		goto fail;
	}

	pr_debug("%s: registered %d ttyGS* device%s\n", __func__,
			MAX_U_SERIAL_PORTS,
			(MAX_U_SERIAL_PORTS == 1) ? "" : "s");

	return status;
fail:
	put_tty_driver(gs_tty_driver);
	gs_tty_driver = NULL;
	return status;
}
module_init(userial_init);

static void userial_cleanup(void)
{
	tty_unregister_driver(gs_tty_driver);
	put_tty_driver(gs_tty_driver);
	gs_tty_driver = NULL;
}
module_exit(userial_cleanup);

MODULE_LICENSE("GPL");
