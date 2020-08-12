/*
 * FSI hub master driver
 *
 * Copyright (C) IBM Corporation 2016
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/delay.h>
#include <linux/fsi.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>

#include "fsi-master.h"

/* Control Registers */
#define FSI_MMODE		0x0		/* R/W: mode */
#define FSI_MDLYR		0x4		/* R/W: delay */
#define FSI_MCRSP		0x8		/* R/W: clock rate */
#define FSI_MENP0		0x10		/* R/W: enable */
#define FSI_MLEVP0		0x18		/* R: plug detect */
#define FSI_MSENP0		0x18		/* S: Set enable */
#define FSI_MCENP0		0x20		/* C: Clear enable */
#define FSI_MAEB		0x70		/* R: Error address */
#define FSI_MVER		0x74		/* R: master version/type */
#define FSI_MRESP0		0xd0		/* W: Port reset */
#define FSI_MESRB0		0x1d0		/* R: Master error status */
#define FSI_MRESB0		0x1d0		/* W: Reset bridge */
#define FSI_MECTRL		0x2e0		/* W: Error control */

/* MMODE: Mode control */
#define FSI_MMODE_EIP		0x80000000	/* Enable interrupt polling */
#define FSI_MMODE_ECRC		0x40000000	/* Enable error recovery */
#define FSI_MMODE_EPC		0x10000000	/* Enable parity checking */
#define FSI_MMODE_P8_TO_LSB	0x00000010	/* Timeout value LSB */
						/*   MSB=1, LSB=0 is 0.8 ms */
						/*   MSB=0, LSB=1 is 0.9 ms */
#define FSI_MMODE_CRS0SHFT	18		/* Clk rate selection 0 shift */
#define FSI_MMODE_CRS0MASK	0x3ff		/* Clk rate selection 0 mask */
#define FSI_MMODE_CRS1SHFT	8		/* Clk rate selection 1 shift */
#define FSI_MMODE_CRS1MASK	0x3ff		/* Clk rate selection 1 mask */

/* MRESB: Reset brindge */
#define FSI_MRESB_RST_GEN	0x80000000	/* General reset */
#define FSI_MRESB_RST_ERR	0x40000000	/* Error Reset */

/* MRESB: Reset port */
#define FSI_MRESP_RST_ALL_MASTER 0x20000000	/* Reset all FSI masters */
#define FSI_MRESP_RST_ALL_LINK	0x10000000	/* Reset all FSI port contr. */
#define FSI_MRESP_RST_MCR	0x08000000	/* Reset FSI master reg. */
#define FSI_MRESP_RST_PYE	0x04000000	/* Reset FSI parity error */
#define FSI_MRESP_RST_ALL	0xfc000000	/* Reset any error */

/* MECTRL: Error control */
#define FSI_MECTRL_EOAE		0x8000		/* Enable machine check when */
						/* master 0 in error */
#define FSI_MECTRL_P8_AUTO_TERM	0x4000		/* Auto terminate */

#define FSI_ENGID_HUB_MASTER		0x1c
#define FSI_HUB_LINK_OFFSET		0x80000
#define FSI_HUB_LINK_SIZE		0x80000
#define FSI_HUB_MASTER_MAX_LINKS	8

#define FSI_LINK_ENABLE_SETUP_TIME	10	/* in mS */

/*
 * FSI hub master support
 *
 * A hub master increases the number of potential target devices that the
 * primary FSI master can access. For each link a primary master supports,
 * each of those links can in turn be chained to a hub master with multiple
 * links of its own.
 *
 * The hub is controlled by a set of control registers exposed as a regular fsi
 * device (the hub->upstream device), and provides access to the downstream FSI
 * bus as through an address range on the slave itself (->addr and ->size).
 *
 * [This differs from "cascaded" masters, which expose the entire downstream
 * bus entirely through the fsi device address range, and so have a smaller
 * accessible address space.]
 */
struct fsi_master_hub {
	struct fsi_master	master;
	struct fsi_device	*upstream;
	uint32_t		addr, size;	/* slave-relative addr of */
						/* master address space */
};

#define to_fsi_master_hub(m) container_of(m, struct fsi_master_hub, master)

static int hub_master_read(struct fsi_master *master, int link,
			uint8_t id, uint32_t addr, void *val, size_t size)
{
	struct fsi_master_hub *hub = to_fsi_master_hub(master);

	if (id != 0)
		return -EINVAL;

	addr += hub->addr + (link * FSI_HUB_LINK_SIZE);
	return fsi_slave_read(hub->upstream->slave, addr, val, size);
}

static int hub_master_write(struct fsi_master *master, int link,
			uint8_t id, uint32_t addr, const void *val, size_t size)
{
	struct fsi_master_hub *hub = to_fsi_master_hub(master);

	if (id != 0)
		return -EINVAL;

	addr += hub->addr + (link * FSI_HUB_LINK_SIZE);
	return fsi_slave_write(hub->upstream->slave, addr, val, size);
}

static int hub_master_break(struct fsi_master *master, int link)
{
	uint32_t addr;
	__be32 cmd;

	addr = 0x4;
	cmd = cpu_to_be32(0xc0de0000);

	return hub_master_write(master, link, 0, addr, &cmd, sizeof(cmd));
}

static int hub_master_link_enable(struct fsi_master *master, int link)
{
	struct fsi_master_hub *hub = to_fsi_master_hub(master);
	int idx, bit;
	__be32 reg;
	int rc;

	idx = link / 32;
	bit = link % 32;

	reg = cpu_to_be32(0x80000000 >> bit);

	rc = fsi_device_write(hub->upstream, FSI_MSENP0 + (4 * idx), &reg, 4);

	mdelay(FSI_LINK_ENABLE_SETUP_TIME);

	fsi_device_read(hub->upstream, FSI_MENP0 + (4 * idx), &reg, 4);

	return rc;
}

static void hub_master_release(struct device *dev)
{
	struct fsi_master_hub *hub = to_fsi_master_hub(dev_to_fsi_master(dev));

	kfree(hub);
}

/* mmode encoders */
static inline u32 fsi_mmode_crs0(u32 x)
{
	return (x & FSI_MMODE_CRS0MASK) << FSI_MMODE_CRS0SHFT;
}

static inline u32 fsi_mmode_crs1(u32 x)
{
	return (x & FSI_MMODE_CRS1MASK) << FSI_MMODE_CRS1SHFT;
}

static int hub_master_init(struct fsi_master_hub *hub)
{
	struct fsi_device *dev = hub->upstream;
	__be32 reg;
	int rc;

	reg = cpu_to_be32(FSI_MRESP_RST_ALL_MASTER | FSI_MRESP_RST_ALL_LINK
			| FSI_MRESP_RST_MCR | FSI_MRESP_RST_PYE);
	rc = fsi_device_write(dev, FSI_MRESP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	/* Initialize the MFSI (hub master) engine */
	reg = cpu_to_be32(FSI_MRESP_RST_ALL_MASTER | FSI_MRESP_RST_ALL_LINK
			| FSI_MRESP_RST_MCR | FSI_MRESP_RST_PYE);
	rc = fsi_device_write(dev, FSI_MRESP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(FSI_MECTRL_EOAE | FSI_MECTRL_P8_AUTO_TERM);
	rc = fsi_device_write(dev, FSI_MECTRL, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(FSI_MMODE_EIP | FSI_MMODE_ECRC | FSI_MMODE_EPC
			| fsi_mmode_crs0(1) | fsi_mmode_crs1(1)
			| FSI_MMODE_P8_TO_LSB);
	rc = fsi_device_write(dev, FSI_MMODE, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(0xffff0000);
	rc = fsi_device_write(dev, FSI_MDLYR, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(~0);
	rc = fsi_device_write(dev, FSI_MSENP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	/* Leave enabled long enough for master logic to set up */
	mdelay(FSI_LINK_ENABLE_SETUP_TIME);

	rc = fsi_device_write(dev, FSI_MCENP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	rc = fsi_device_read(dev, FSI_MAEB, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(FSI_MRESP_RST_ALL_MASTER | FSI_MRESP_RST_ALL_LINK);
	rc = fsi_device_write(dev, FSI_MRESP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	rc = fsi_device_read(dev, FSI_MLEVP0, &reg, sizeof(reg));
	if (rc)
		return rc;

	/* Reset the master bridge */
	reg = cpu_to_be32(FSI_MRESB_RST_GEN);
	rc = fsi_device_write(dev, FSI_MRESB0, &reg, sizeof(reg));
	if (rc)
		return rc;

	reg = cpu_to_be32(FSI_MRESB_RST_ERR);
	return fsi_device_write(dev, FSI_MRESB0, &reg, sizeof(reg));
}

static int hub_master_probe(struct device *dev)
{
	struct fsi_device *fsi_dev = to_fsi_dev(dev);
	struct fsi_master_hub *hub;
	uint32_t reg, links;
	__be32 __reg;
	int rc;

	rc = fsi_device_read(fsi_dev, FSI_MVER, &__reg, sizeof(__reg));
	if (rc)
		return rc;

	reg = be32_to_cpu(__reg);
	links = (reg >> 8) & 0xff;
	dev_dbg(dev, "hub version %08x (%d links)\n", reg, links);

	rc = fsi_slave_claim_range(fsi_dev->slave, FSI_HUB_LINK_OFFSET,
			FSI_HUB_LINK_SIZE * links);
	if (rc) {
		dev_err(dev, "can't claim slave address range for links");
		return rc;
	}

	hub = kzalloc(sizeof(*hub), GFP_KERNEL);
	if (!hub) {
		rc = -ENOMEM;
		goto err_release;
	}

	hub->addr = FSI_HUB_LINK_OFFSET;
	hub->size = FSI_HUB_LINK_SIZE * links;
	hub->upstream = fsi_dev;

	hub->master.dev.parent = dev;
	hub->master.dev.release = hub_master_release;
	hub->master.dev.of_node = of_node_get(dev_of_node(dev));

	hub->master.n_links = links;
	hub->master.read = hub_master_read;
	hub->master.write = hub_master_write;
	hub->master.send_break = hub_master_break;
	hub->master.link_enable = hub_master_link_enable;

	dev_set_drvdata(dev, hub);

	hub_master_init(hub);

	rc = fsi_master_register(&hub->master);
	if (rc)
		goto err_release;

	/* At this point, fsi_master_register performs the device_initialize(),
	 * and holds the sole reference on master.dev. This means the device
	 * will be freed (via ->release) during any subsequent call to
	 * fsi_master_unregister.  We add our own reference to it here, so we
	 * can perform cleanup (in _remove()) without it being freed before
	 * we're ready.
	 */
	get_device(&hub->master.dev);
	return 0;

err_release:
	fsi_slave_release_range(fsi_dev->slave, FSI_HUB_LINK_OFFSET,
			FSI_HUB_LINK_SIZE * links);
	return rc;
}

static int hub_master_remove(struct device *dev)
{
	struct fsi_master_hub *hub = dev_get_drvdata(dev);

	fsi_master_unregister(&hub->master);
	fsi_slave_release_range(hub->upstream->slave, hub->addr, hub->size);
	of_node_put(hub->master.dev.of_node);

	/*
	 * master.dev will likely be ->release()ed after this, which free()s
	 * the hub
	 */
	put_device(&hub->master.dev);

	return 0;
}

static struct fsi_device_id hub_master_ids[] = {
	{
		.engine_type = FSI_ENGID_HUB_MASTER,
		.version = FSI_VERSION_ANY,
	},
	{ 0 }
};

static struct fsi_driver hub_master_driver = {
	.id_table = hub_master_ids,
	.drv = {
		.name = "fsi-master-hub",
		.bus = &fsi_bus_type,
		.probe = hub_master_probe,
		.remove = hub_master_remove,
	}
};

module_fsi_driver(hub_master_driver);
MODULE_LICENSE("GPL");
