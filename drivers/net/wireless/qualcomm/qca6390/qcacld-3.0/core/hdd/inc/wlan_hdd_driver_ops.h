/*
 * Copyright (c) 2015-2017, 2019 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __WLAN_HDD_DRIVER_OPS_H__
#define __WLAN_HDD_DRIVER_OPS_H__

#include "hif.h"

struct hdd_context;

/**
 * DOC: wlan_hdd_driver_ops.h
 *
 * Functions to register the wlan driver.
 */

/**
 * wlan_hdd_register_driver() - Register with platform layer
 *
 * This function is used to register HDD callbacks with the platform
 * layer.
 *
 * Return: 0 if registration is successful, negative errno if
 * registration fails
 */
int wlan_hdd_register_driver(void);

/**
 * wlan_hdd_unregister_driver() - Unregister from platform layer
 *
 * This function is used to unregister HDD callbacks from the platform
 * layer.
 *
 * Return: void
 */

void wlan_hdd_unregister_driver(void);

/**
 * wlan_hdd_bus_suspend() - suspend the wlan bus
 *
 * This function is called by the platform driver to suspend the
 * wlan bus
 *
 * Return: 0 on success, negative errno on error
 */
int wlan_hdd_bus_suspend(void);

/**
 * wlan_hdd_bus_suspend_noirq() - handle .suspend_noirq callback
 *
 * This function is called by the platform driver to complete the
 * bus suspend callback when device interrupts are disabled by kernel.
 * Call HIF and WMA suspend_noirq callbacks to make sure there is no
 * wake up pending from FW before allowing suspend.
 *
 * Return: 0 for success and -EBUSY if FW is requesting wake up
 */
int wlan_hdd_bus_suspend_noirq(void);

/**
 * wlan_hdd_bus_resume() - wake up the bus
 *
 * This function is called by the platform driver to resume wlan
 * bus
 *
 * Return: 0 for success and negative errno if failure
 */
int wlan_hdd_bus_resume(void);

/**
 * wlan_hdd_bus_resume_noirq() - handle bus resume no irq
 *
 * This function is called by the platform driver to do bus
 * resume no IRQ before calling resume callback. Call WMA and HIF
 * layers to complete the resume_noirq.
 *
 * Return: 0 for success and negative error code for failure
 */
int wlan_hdd_bus_resume_noirq(void);

/**
 * hdd_hif_close() - HIF close helper
 * @hdd_ctx: HDD context
 * @hif_ctx: HIF context
 *
 * Helper function to close HIF
 */
void hdd_hif_close(struct hdd_context *hdd_ctx, void *hif_ctx);

/**
 * hdd_hif_open() - HIF open helper
 * @dev: wlan device structure
 * @bdev: bus device structure
 * @bid: bus identifier for shared busses
 * @bus_type: underlying bus type
 * @reinit: true if we are reinitializing the driver during recovery phase
 *
 * This function brings-up HIF layer during load/recovery phase.
 *
 * Return: 0 on success and errno on failure.
 */
int hdd_hif_open(struct device *dev, void *bdev, const struct hif_bus_id *bid,
		 enum qdf_bus_type bus_type, bool reinit);

/**
 * hdd_soc_idle_restart_lock() - Takes wakelock for idle restart
 *
 * This function takes wakelock to prevent suspend during idle restart
 *
 * Return: none
 */
void hdd_soc_idle_restart_lock(void);

/**
 * hdd_soc_idle_restart_unlock() - Releases wakelock for idle restart
 *
 * This function releases wakelock to allow suspend after idle restart
 *
 * Return: none
 */
void hdd_soc_idle_restart_unlock(void);

#endif /* __WLAN_HDD_DRIVER_OPS_H__ */
