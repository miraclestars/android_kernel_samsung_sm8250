/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SERIAL_BCM63XX_H
#define _LINUX_SERIAL_BCM63XX_H

/* UART Control Register */
#define UART_CTL_REG			0x0
#define UART_CTL_RXTMOUTCNT_SHIFT	0
#define UART_CTL_RXTMOUTCNT_MASK	(0x1f << UART_CTL_RXTMOUTCNT_SHIFT)
#define UART_CTL_RSTTXDN_SHIFT		5
#define UART_CTL_RSTTXDN_MASK		(1 << UART_CTL_RSTTXDN_SHIFT)
#define UART_CTL_RSTRXFIFO_SHIFT		6
#define UART_CTL_RSTRXFIFO_MASK		(1 << UART_CTL_RSTRXFIFO_SHIFT)
#define UART_CTL_RSTTXFIFO_SHIFT		7
#define UART_CTL_RSTTXFIFO_MASK		(1 << UART_CTL_RSTTXFIFO_SHIFT)
#define UART_CTL_STOPBITS_SHIFT		8
#define UART_CTL_STOPBITS_MASK		(0xf << UART_CTL_STOPBITS_SHIFT)
#define UART_CTL_STOPBITS_1		(0x7 << UART_CTL_STOPBITS_SHIFT)
#define UART_CTL_STOPBITS_2		(0xf << UART_CTL_STOPBITS_SHIFT)
#define UART_CTL_BITSPERSYM_SHIFT	12
#define UART_CTL_BITSPERSYM_MASK	(0x3 << UART_CTL_BITSPERSYM_SHIFT)
#define UART_CTL_XMITBRK_SHIFT		14
#define UART_CTL_XMITBRK_MASK		(1 << UART_CTL_XMITBRK_SHIFT)
#define UART_CTL_RSVD_SHIFT		15
#define UART_CTL_RSVD_MASK		(1 << UART_CTL_RSVD_SHIFT)
#define UART_CTL_RXPAREVEN_SHIFT		16
#define UART_CTL_RXPAREVEN_MASK		(1 << UART_CTL_RXPAREVEN_SHIFT)
#define UART_CTL_RXPAREN_SHIFT		17
#define UART_CTL_RXPAREN_MASK		(1 << UART_CTL_RXPAREN_SHIFT)
#define UART_CTL_TXPAREVEN_SHIFT		18
#define UART_CTL_TXPAREVEN_MASK		(1 << UART_CTL_TXPAREVEN_SHIFT)
#define UART_CTL_TXPAREN_SHIFT		18
#define UART_CTL_TXPAREN_MASK		(1 << UART_CTL_TXPAREN_SHIFT)
#define UART_CTL_LOOPBACK_SHIFT		20
#define UART_CTL_LOOPBACK_MASK		(1 << UART_CTL_LOOPBACK_SHIFT)
#define UART_CTL_RXEN_SHIFT		21
#define UART_CTL_RXEN_MASK		(1 << UART_CTL_RXEN_SHIFT)
#define UART_CTL_TXEN_SHIFT		22
#define UART_CTL_TXEN_MASK		(1 << UART_CTL_TXEN_SHIFT)
#define UART_CTL_BRGEN_SHIFT		23
#define UART_CTL_BRGEN_MASK		(1 << UART_CTL_BRGEN_SHIFT)

/* UART Baudword register */
#define UART_BAUD_REG			0x4

/* UART Misc Control register */
#define UART_MCTL_REG			0x8
#define UART_MCTL_DTR_SHIFT		0
#define UART_MCTL_DTR_MASK		(1 << UART_MCTL_DTR_SHIFT)
#define UART_MCTL_RTS_SHIFT		1
#define UART_MCTL_RTS_MASK		(1 << UART_MCTL_RTS_SHIFT)
#define UART_MCTL_RXFIFOTHRESH_SHIFT	8
#define UART_MCTL_RXFIFOTHRESH_MASK	(0xf << UART_MCTL_RXFIFOTHRESH_SHIFT)
#define UART_MCTL_TXFIFOTHRESH_SHIFT	12
#define UART_MCTL_TXFIFOTHRESH_MASK	(0xf << UART_MCTL_TXFIFOTHRESH_SHIFT)
#define UART_MCTL_RXFIFOFILL_SHIFT	16
#define UART_MCTL_RXFIFOFILL_MASK	(0x1f << UART_MCTL_RXFIFOFILL_SHIFT)
#define UART_MCTL_TXFIFOFILL_SHIFT	24
#define UART_MCTL_TXFIFOFILL_MASK	(0x1f << UART_MCTL_TXFIFOFILL_SHIFT)

/* UART External Input Configuration register */
#define UART_EXTINP_REG			0xc
#define UART_EXTINP_RI_SHIFT		0
#define UART_EXTINP_RI_MASK		(1 << UART_EXTINP_RI_SHIFT)
#define UART_EXTINP_CTS_SHIFT		1
#define UART_EXTINP_CTS_MASK		(1 << UART_EXTINP_CTS_SHIFT)
#define UART_EXTINP_DCD_SHIFT		2
#define UART_EXTINP_DCD_MASK		(1 << UART_EXTINP_DCD_SHIFT)
#define UART_EXTINP_DSR_SHIFT		3
#define UART_EXTINP_DSR_MASK		(1 << UART_EXTINP_DSR_SHIFT)
#define UART_EXTINP_IRSTAT(x)		(1 << (x + 4))
#define UART_EXTINP_IRMASK(x)		(1 << (x + 8))
#define UART_EXTINP_IR_RI		0
#define UART_EXTINP_IR_CTS		1
#define UART_EXTINP_IR_DCD		2
#define UART_EXTINP_IR_DSR		3
#define UART_EXTINP_RI_NOSENSE_SHIFT	16
#define UART_EXTINP_RI_NOSENSE_MASK	(1 << UART_EXTINP_RI_NOSENSE_SHIFT)
#define UART_EXTINP_CTS_NOSENSE_SHIFT	17
#define UART_EXTINP_CTS_NOSENSE_MASK	(1 << UART_EXTINP_CTS_NOSENSE_SHIFT)
#define UART_EXTINP_DCD_NOSENSE_SHIFT	18
#define UART_EXTINP_DCD_NOSENSE_MASK	(1 << UART_EXTINP_DCD_NOSENSE_SHIFT)
#define UART_EXTINP_DSR_NOSENSE_SHIFT	19
#define UART_EXTINP_DSR_NOSENSE_MASK	(1 << UART_EXTINP_DSR_NOSENSE_SHIFT)

/* UART Interrupt register */
#define UART_IR_REG			0x10
#define UART_IR_MASK(x)			(1 << (x + 16))
#define UART_IR_STAT(x)			(1 << (x))
#define UART_IR_EXTIP			0
#define UART_IR_TXUNDER			1
#define UART_IR_TXOVER			2
#define UART_IR_TXTRESH			3
#define UART_IR_TXRDLATCH		4
#define UART_IR_TXEMPTY			5
#define UART_IR_RXUNDER			6
#define UART_IR_RXOVER			7
#define UART_IR_RXTIMEOUT		8
#define UART_IR_RXFULL			9
#define UART_IR_RXTHRESH		10
#define UART_IR_RXNOTEMPTY		11
#define UART_IR_RXFRAMEERR		12
#define UART_IR_RXPARERR		13
#define UART_IR_RXBRK			14
#define UART_IR_TXDONE			15

/* UART Fifo register */
#define UART_FIFO_REG			0x14
#define UART_FIFO_VALID_SHIFT		0
#define UART_FIFO_VALID_MASK		0xff
#define UART_FIFO_FRAMEERR_SHIFT	8
#define UART_FIFO_FRAMEERR_MASK		(1 << UART_FIFO_FRAMEERR_SHIFT)
#define UART_FIFO_PARERR_SHIFT		9
#define UART_FIFO_PARERR_MASK		(1 << UART_FIFO_PARERR_SHIFT)
#define UART_FIFO_BRKDET_SHIFT		10
#define UART_FIFO_BRKDET_MASK		(1 << UART_FIFO_BRKDET_SHIFT)
#define UART_FIFO_ANYERR_MASK		(UART_FIFO_FRAMEERR_MASK |	\
					UART_FIFO_PARERR_MASK |		\
					UART_FIFO_BRKDET_MASK)

#endif /* _LINUX_SERIAL_BCM63XX_H */
