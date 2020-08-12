/*
 * Intel PCH/PCU SPI flash PCI driver.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Mika Westerberg <mika.westerberg@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>

#include "intel-spi.h"

#define BCR		0xdc
#define BCR_WPD		BIT(0)

static const struct intel_spi_boardinfo bxt_info = {
	.type = INTEL_SPI_BXT,
};

static int intel_spi_pci_probe(struct pci_dev *pdev,
			       const struct pci_device_id *id)
{
	struct intel_spi_boardinfo *info;
	struct intel_spi *ispi;
	u32 bcr;
	int ret;

	ret = pcim_enable_device(pdev);
	if (ret)
		return ret;

	info = devm_kmemdup(&pdev->dev, (void *)id->driver_data, sizeof(*info),
			    GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	/* Try to make the chip read/write */
	pci_read_config_dword(pdev, BCR, &bcr);
	if (!(bcr & BCR_WPD)) {
		bcr |= BCR_WPD;
		pci_write_config_dword(pdev, BCR, bcr);
		pci_read_config_dword(pdev, BCR, &bcr);
	}
	info->writeable = !!(bcr & BCR_WPD);

	ispi = intel_spi_probe(&pdev->dev, &pdev->resource[0], info);
	if (IS_ERR(ispi))
		return PTR_ERR(ispi);

	pci_set_drvdata(pdev, ispi);
	return 0;
}

static void intel_spi_pci_remove(struct pci_dev *pdev)
{
	intel_spi_remove(pci_get_drvdata(pdev));
}

static const struct pci_device_id intel_spi_pci_ids[] = {
	{ PCI_VDEVICE(INTEL, 0x18e0), (unsigned long)&bxt_info },
	{ PCI_VDEVICE(INTEL, 0x19e0), (unsigned long)&bxt_info },
	{ PCI_VDEVICE(INTEL, 0x34a4), (unsigned long)&bxt_info },
	{ PCI_VDEVICE(INTEL, 0xa1a4), (unsigned long)&bxt_info },
	{ PCI_VDEVICE(INTEL, 0xa224), (unsigned long)&bxt_info },
	{ },
};
MODULE_DEVICE_TABLE(pci, intel_spi_pci_ids);

static struct pci_driver intel_spi_pci_driver = {
	.name = "intel-spi",
	.id_table = intel_spi_pci_ids,
	.probe = intel_spi_pci_probe,
	.remove = intel_spi_pci_remove,
};

module_pci_driver(intel_spi_pci_driver);

MODULE_DESCRIPTION("Intel PCH/PCU SPI flash PCI driver");
MODULE_AUTHOR("Mika Westerberg <mika.westerberg@linux.intel.com>");
MODULE_LICENSE("GPL v2");
