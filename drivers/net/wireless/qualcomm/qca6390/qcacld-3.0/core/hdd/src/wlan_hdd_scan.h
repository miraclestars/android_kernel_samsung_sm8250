/*
 * Copyright (c) 2012-2019 The Linux Foundation. All rights reserved.
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

/**
 * DOC : wlan_hdd_scan.h
 *
 * WLAN Host Device Driver scan related implementation
 *
 */

#if !defined(WLAN_HDD_SCAN_H)
#define WLAN_HDD_SCAN_H

#include "wlan_hdd_main.h"
#include "csr_inside_api.h"
#include <wlan_cfg80211_scan.h>

int hdd_scan_context_init(struct hdd_context *hdd_ctx);
void hdd_scan_context_destroy(struct hdd_context *hdd_ctx);

int wlan_hdd_cfg80211_scan(struct wiphy *wiphy,
			   struct cfg80211_scan_request *request);

#ifdef FEATURE_WLAN_SCAN_PNO
int wlan_hdd_cfg80211_sched_scan_start(struct wiphy *wiphy,
				       struct net_device *dev,
				       struct cfg80211_sched_scan_request
				       *request);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 12, 0)
/**
 * wlan_hdd_cfg80211_sched_scan_stop() - stop cfg80211 scheduled (PNO) scan
 * @wiphy: Pointer to wiphy
 * @dev: Pointer network device
 *
 * Note, this returns success if the driver is recovering or unloading to
 * prevent race conditions between PLD initiating an unload and kernel
 * initiating a scheduled scan stop via cfg80211. Unload is expected to stop
 * any pending scheduled scans in this case.
 *
 * Return: 0 for success, non zero for failure
 */
int wlan_hdd_cfg80211_sched_scan_stop(struct wiphy *wiphy,
				      struct net_device *dev);
#else
int wlan_hdd_cfg80211_sched_scan_stop(struct wiphy *wiphy,
				      struct net_device *dev,
				      uint64_t reqid);

#endif /* KERNEL_VERSION(4, 12, 0) */

/**
 * wlan_hdd_sched_scan_stop() - stop scheduled (PNO) scans
 * @dev: Pointer network device
 *
 * Return: 0 for success, non zero for failure
 */
int wlan_hdd_sched_scan_stop(struct net_device *dev);
#else
static inline int wlan_hdd_sched_scan_stop(struct net_device *dev)
{
	return 0;
}
#endif /* End of FEATURE_WLAN_SCAN_PNO */

int wlan_hdd_cfg80211_vendor_scan(struct wiphy *wiphy,
		struct wireless_dev *wdev, const void *data,
		int data_len);

/**
 * wlan_hdd_vendor_abort_scan() - API to process vendor command for
 * abort scan
 * @wiphy: Pointer to wiphy
 * @wdev: Pointer to net device
 * @data : Pointer to the data
 * @data_len : length of the data
 *
 * This is called from supplicant to abort scan
 *
 * Return: zero for success and non zero for failure.
 */
int wlan_hdd_vendor_abort_scan(
	struct wiphy *wiphy, struct wireless_dev *wdev,
	const void *data, int data_len);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 5, 0)) || \
	defined(CFG80211_ABORT_SCAN)
void wlan_hdd_cfg80211_abort_scan(struct wiphy *wiphy,
				  struct wireless_dev *wdev);
#endif

/**
 * hdd_init_scan_reject_params() - init scan reject params
 * @hdd_ctx: hdd contxt
 *
 * Return: None
 */
void hdd_init_scan_reject_params(struct hdd_context *hdd_ctx);

/**
 * hdd_reset_scan_reject_params() - reset scan reject params per roam stats
 * @hdd_ctx: hdd contxt
 * @roam_status: roam status
 * @roam_result: roam result
 *
 * Return: None
 */
void hdd_reset_scan_reject_params(struct hdd_context *hdd_ctx,
				  eRoamCmdStatus roam_status,
				  eCsrRoamResult roam_result);

/**
 * wlan_hdd_cfg80211_scan_block() - scan block handler
 * @adapter: HDD adapter to work against
 *
 * Return: none
 */
void wlan_hdd_cfg80211_scan_block(struct hdd_adapter *adapter);
#endif /* end #if !defined(WLAN_HDD_SCAN_H) */

