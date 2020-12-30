/*
 * Copyright 2017 IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/clk.h>
#include <linux/mfd/syscon.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/regmap.h>

#include <linux/aspeed-lpc-ctrl.h>

#define DEVICE_NAME	"aspeed-lpc-ctrl"

#define HICR5 0x0
#define HICR5_ENL2H	BIT(8)
#define HICR5_ENFWH	BIT(10)

#define HICR7 0x8
#define HICR8 0xc

struct aspeed_lpc_ctrl {
	struct miscdevice	miscdev;
	struct regmap		*regmap;
	struct clk		*clk;
	phys_addr_t		mem_base;
	resource_size_t		mem_size;
	u32		pnor_size;
	u32		pnor_base;
};

static struct aspeed_lpc_ctrl *file_aspeed_lpc_ctrl(struct file *file)
{
	return container_of(file->private_data, struct aspeed_lpc_ctrl,
			miscdev);
}

static int aspeed_lpc_ctrl_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct aspeed_lpc_ctrl *lpc_ctrl = file_aspeed_lpc_ctrl(file);
	unsigned long vsize = vma->vm_end - vma->vm_start;
	pgprot_t prot = vma->vm_page_prot;

	if (vma->vm_pgoff + vsize > lpc_ctrl->mem_base + lpc_ctrl->mem_size)
		return -EINVAL;

	/* ast2400/2500 AHB accesses are not cache coherent */
	prot = pgprot_noncached(prot);

	if (remap_pfn_range(vma, vma->vm_start,
		(lpc_ctrl->mem_base >> PAGE_SHIFT) + vma->vm_pgoff,
		vsize, prot))
		return -EAGAIN;

	return 0;
}

static long aspeed_lpc_ctrl_ioctl(struct file *file, unsigned int cmd,
		unsigned long param)
{
	struct aspeed_lpc_ctrl *lpc_ctrl = file_aspeed_lpc_ctrl(file);
	void __user *p = (void __user *)param;
	struct aspeed_lpc_ctrl_mapping map;
	u32 addr;
	u32 size;
	long rc;

	if (copy_from_user(&map, p, sizeof(map)))
		return -EFAULT;

	if (map.flags != 0)
		return -EINVAL;

	switch (cmd) {
	case ASPEED_LPC_CTRL_IOCTL_GET_SIZE:
		/* The flash windows don't report their size */
		if (map.window_type != ASPEED_LPC_CTRL_WINDOW_MEMORY)
			return -EINVAL;

		/* Support more than one window id in the future */
		if (map.window_id != 0)
			return -EINVAL;

		map.size = lpc_ctrl->mem_size;

		return copy_to_user(p, &map, sizeof(map)) ? -EFAULT : 0;
	case ASPEED_LPC_CTRL_IOCTL_MAP:

		/*
		 * The top half of HICR7 is the MSB of the BMC address of the
		 * mapping.
		 * The bottom half of HICR7 is the MSB of the HOST LPC
		 * firmware space address of the mapping.
		 *
		 * The 1 bits in the top of half of HICR8 represent the bits
		 * (in the requested address) that should be ignored and
		 * replaced with those from the top half of HICR7.
		 * The 1 bits in the bottom half of HICR8 represent the bits
		 * (in the requested address) that should be kept and pass
		 * into the BMC address space.
		 */

		/*
		 * It doesn't make sense to talk about a size or offset with
		 * low 16 bits set. Both HICR7 and HICR8 talk about the top 16
		 * bits of addresses and sizes.
		 */

		if ((map.size & 0x0000ffff) || (map.offset & 0x0000ffff))
			return -EINVAL;

		/*
		 * Because of the way the masks work in HICR8 offset has to
		 * be a multiple of size.
		 */
		if (map.offset & (map.size - 1))
			return -EINVAL;

		if (map.window_type == ASPEED_LPC_CTRL_WINDOW_FLASH) {
			addr = lpc_ctrl->pnor_base;
			size = lpc_ctrl->pnor_size;
		} else if (map.window_type == ASPEED_LPC_CTRL_WINDOW_MEMORY) {
			addr = lpc_ctrl->mem_base;
			size = lpc_ctrl->mem_size;
		} else {
			return -EINVAL;
		}

		/* Check overflow first! */
		if (map.offset + map.size < map.offset ||
			map.offset + map.size > size)
			return -EINVAL;

		if (map.size == 0 || map.size > size)
			return -EINVAL;

		addr += map.offset;

		/*
		 * addr (host lpc address) is safe regardless of values. This
		 * simply changes the address the host has to request on its
		 * side of the LPC bus. This cannot impact the hosts own
		 * memory space by surprise as LPC specific accessors are
		 * required. The only strange thing that could be done is
		 * setting the lower 16 bits but the shift takes care of that.
		 */

		rc = regmap_write(lpc_ctrl->regmap, HICR7,
				(addr | (map.addr >> 16)));
		if (rc)
			return rc;

		rc = regmap_write(lpc_ctrl->regmap, HICR8,
				(~(map.size - 1)) | ((map.size >> 16) - 1));
		if (rc)
			return rc;

		/*
		 * Enable LPC FHW cycles. This is required for the host to
		 * access the regions specified.
		 */
		return regmap_update_bits(lpc_ctrl->regmap, HICR5,
				HICR5_ENFWH | HICR5_ENL2H,
				HICR5_ENFWH | HICR5_ENL2H);
	}

	return -EINVAL;
}

static const struct file_operations aspeed_lpc_ctrl_fops = {
	.owner		= THIS_MODULE,
	.mmap		= aspeed_lpc_ctrl_mmap,
	.unlocked_ioctl	= aspeed_lpc_ctrl_ioctl,
};

static int aspeed_lpc_ctrl_probe(struct platform_device *pdev)
{
	struct aspeed_lpc_ctrl *lpc_ctrl;
	struct device_node *node;
	struct resource resm;
	struct device *dev;
	int rc;

	dev = &pdev->dev;

	lpc_ctrl = devm_kzalloc(dev, sizeof(*lpc_ctrl), GFP_KERNEL);
	if (!lpc_ctrl)
		return -ENOMEM;

	node = of_parse_phandle(dev->of_node, "flash", 0);
	if (!node) {
		dev_err(dev, "Didn't find host pnor flash node\n");
		return -ENODEV;
	}

	rc = of_address_to_resource(node, 1, &resm);
	of_node_put(node);
	if (rc) {
		dev_err(dev, "Couldn't address to resource for flash\n");
		return rc;
	}

	lpc_ctrl->pnor_size = resource_size(&resm);
	lpc_ctrl->pnor_base = resm.start;

	dev_set_drvdata(&pdev->dev, lpc_ctrl);

	node = of_parse_phandle(dev->of_node, "memory-region", 0);
	if (!node) {
		dev_err(dev, "Didn't find reserved memory\n");
		return -EINVAL;
	}

	rc = of_address_to_resource(node, 0, &resm);
	of_node_put(node);
	if (rc) {
		dev_err(dev, "Couldn't address to resource for reserved memory\n");
		return -ENOMEM;
	}

	lpc_ctrl->mem_size = resource_size(&resm);
	lpc_ctrl->mem_base = resm.start;

	lpc_ctrl->regmap = syscon_node_to_regmap(
			pdev->dev.parent->of_node);
	if (IS_ERR(lpc_ctrl->regmap)) {
		dev_err(dev, "Couldn't get regmap\n");
		return -ENODEV;
	}

	lpc_ctrl->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(lpc_ctrl->clk)) {
		dev_err(dev, "couldn't get clock\n");
		return PTR_ERR(lpc_ctrl->clk);
	}
	rc = clk_prepare_enable(lpc_ctrl->clk);
	if (rc) {
		dev_err(dev, "couldn't enable clock\n");
		return rc;
	}

	lpc_ctrl->miscdev.minor = MISC_DYNAMIC_MINOR;
	lpc_ctrl->miscdev.name = DEVICE_NAME;
	lpc_ctrl->miscdev.fops = &aspeed_lpc_ctrl_fops;
	lpc_ctrl->miscdev.parent = dev;
	rc = misc_register(&lpc_ctrl->miscdev);
	if (rc) {
		dev_err(dev, "Unable to register device\n");
		goto err;
	}

	dev_info(dev, "Loaded at %pr\n", &resm);

	return 0;

err:
	clk_disable_unprepare(lpc_ctrl->clk);
	return rc;
}

static int aspeed_lpc_ctrl_remove(struct platform_device *pdev)
{
	struct aspeed_lpc_ctrl *lpc_ctrl = dev_get_drvdata(&pdev->dev);

	misc_deregister(&lpc_ctrl->miscdev);
	clk_disable_unprepare(lpc_ctrl->clk);

	return 0;
}

static const struct of_device_id aspeed_lpc_ctrl_match[] = {
	{ .compatible = "aspeed,ast2400-lpc-ctrl" },
	{ .compatible = "aspeed,ast2500-lpc-ctrl" },
	{ },
};

static struct platform_driver aspeed_lpc_ctrl_driver = {
	.driver = {
		.name		= DEVICE_NAME,
		.of_match_table = aspeed_lpc_ctrl_match,
	},
	.probe = aspeed_lpc_ctrl_probe,
	.remove = aspeed_lpc_ctrl_remove,
};

module_platform_driver(aspeed_lpc_ctrl_driver);

MODULE_DEVICE_TABLE(of, aspeed_lpc_ctrl_match);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cyril Bur <cyrilbur@gmail.com>");
MODULE_DESCRIPTION("Control for aspeed 2400/2500 LPC HOST to BMC mappings");
