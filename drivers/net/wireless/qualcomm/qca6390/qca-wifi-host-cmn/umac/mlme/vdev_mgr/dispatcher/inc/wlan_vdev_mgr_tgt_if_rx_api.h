/*
 * Copyright (c) 2019 The Linux Foundation. All rights reserved.
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
 * DOC: wlan_vdev_mgr_rx_ops.h
 *
 * API declarations to handle WMI response event corresponding to vdev mgmt
 */

#ifndef __WLAN_VDEV_MGR_RX_OPS_H__
#define __WLAN_VDEV_MGR_RX_OPS_H__

#ifdef CMN_VDEV_MGR_TGT_IF_ENABLE
#include <wlan_objmgr_vdev_obj.h>
#include <wlan_vdev_mgr_tgt_if_rx_defs.h>

/**
 * tgt_vdev_mgr_register_rx_ops(): API to register rx ops with lmac
 * @rx_ops: rx ops struct
 *
 * Return: none
 */
void tgt_vdev_mgr_register_rx_ops(struct wlan_lmac_if_rx_ops *rx_ops);
#else
/**
 * tgt_vdev_mgr_register_rx_ops(): API to register rx ops with lmac
 * @rx_ops: rx ops struct
 *
 * Return: none
 */
static inline void
tgt_vdev_mgr_register_rx_ops(struct wlan_lmac_if_rx_ops *rx_ops) {}
#endif /* CMN_VDEV_MGR_TGT_IF_ENABLE */

#endif /* __WLAN_VDEV_MGR_RX_OPS_H__ */
