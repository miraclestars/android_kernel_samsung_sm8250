/*
 * Copyright (c) 2017-2019 The Linux Foundation. All rights reserved.
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
 * DOC: defines driver functions interfacing with linux kernel
 */

#include <qdf_list.h>
#include <qdf_status.h>
#include <linux/wireless.h>
#include <linux/netdevice.h>
#include <net/cfg80211.h>
#include <wlan_cfg80211.h>
#include <wlan_osif_priv.h>
#include <qdf_mem.h>
#include <wlan_spectral_ucfg_api.h>
#include <wlan_cfg80211_spectral.h>
#include <spectral_ioctl.h>
#include "qal_devcfg.h"

static const struct nla_policy spectral_scan_policy[
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX + 1] = {
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_COUNT] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_PERIOD] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PRIORITY] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_SIZE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_GC_ENA] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RESTART_ENA] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NOISE_FLOOR_REF] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_INIT_DELAY] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NB_TONE_THR] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_STR_BIN_THR] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_WB_RPT_MODE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_RPT_MODE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_THR] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PWR_FORMAT] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RPT_MODE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_BIN_SCALE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DBM_ADJ] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_CHN_MASK] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_REQUEST_TYPE] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_COOKIE] = {
							.type = NLA_U64},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_PERIOD] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SHORT_REPORT] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DEBUG_LEVEL] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FREQUENCY] = {
							.type = NLA_U32},
	[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE] = {
						.type = NLA_U32},
};

static void wlan_spectral_intit_config(struct spectral_config *config_req)
{
	config_req->ss_period =          SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_count =           SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_fft_period =      SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_short_report =    SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_spectral_pri =    SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_fft_size =        SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_gc_ena =          SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_restart_ena =     SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_noise_floor_ref = SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_init_delay =      SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_nb_tone_thr =     SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_str_bin_thr =     SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_wb_rpt_mode =     SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_rssi_rpt_mode =   SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_rssi_thr =        SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_pwr_format =      SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_rpt_mode =        SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_bin_scale =       SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_dbm_adj =         SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_chn_mask =        SPECTRAL_PHYERR_PARAM_NOVAL;
	config_req->ss_frequency =       SPECTRAL_PHYERR_PARAM_NOVAL;
}

/**
 * convert_spectral_mode_nl_to_internal() - Get Spectral mode
 * @nl_spectral_mode: Spectral mode in vendor attribute
 * @mode: Converted Spectral mode
 *
 * Return: QDF_STATUS_SUCCESS on success, else QDF_STATUS_E_FAILURE
 */
static QDF_STATUS
convert_spectral_mode_nl_to_internal
		(enum qca_wlan_vendor_spectral_scan_mode nl_spectral_mode,
		 enum spectral_scan_mode *mode)
{
	switch (nl_spectral_mode) {
	case QCA_WLAN_VENDOR_SPECTRAL_SCAN_MODE_NORMAL:
		*mode = SPECTRAL_SCAN_MODE_NORMAL;
		break;

	case QCA_WLAN_VENDOR_SPECTRAL_SCAN_MODE_AGILE:
		*mode = SPECTRAL_SCAN_MODE_AGILE;
		break;

	default:
		cfg80211_err("Invalid spectral mode %u", nl_spectral_mode);
		return QDF_STATUS_E_FAILURE;
	}

	return QDF_STATUS_SUCCESS;
}

int wlan_cfg80211_spectral_scan_config_and_start(struct wiphy *wiphy,
						 struct wlan_objmgr_pdev *pdev,
						 const void *data,
						 int data_len)
{
	struct nlattr *tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX + 1];
	struct spectral_config config_req;
	QDF_STATUS status;
	uint64_t cookie;
	struct sk_buff *skb;
	uint32_t spectral_dbg_level;
	uint32_t scan_req_type = 0;
	struct spectral_cp_request sscan_req;
	enum spectral_scan_mode sscan_mode = SPECTRAL_SCAN_MODE_NORMAL;

	if (wlan_cfg80211_nla_parse(
			tb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX,
			data,
			data_len,
			spectral_scan_policy)) {
		cfg80211_err("Invalid Spectral Scan config ATTR");
		return -EINVAL;
	}

	wlan_spectral_intit_config(&config_req);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_COUNT])
		config_req.ss_count = nla_get_u32(tb
			[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_COUNT]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_PERIOD])
		config_req.ss_period = nla_get_u32(tb
		[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_PERIOD]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PRIORITY])
		config_req.ss_spectral_pri = nla_get_u32(tb
			[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PRIORITY]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_SIZE])
		config_req.ss_fft_size = nla_get_u32(tb
			[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_SIZE]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_GC_ENA])
		config_req.ss_gc_ena = nla_get_u32(tb
			[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_GC_ENA]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RESTART_ENA])
		config_req.ss_restart_ena = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RESTART_ENA]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NOISE_FLOOR_REF])
		config_req.ss_noise_floor_ref = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NOISE_FLOOR_REF]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_INIT_DELAY])
		config_req.ss_init_delay = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_INIT_DELAY]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NB_TONE_THR])
		config_req.ss_nb_tone_thr = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NB_TONE_THR]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_STR_BIN_THR])
		config_req.ss_str_bin_thr = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_STR_BIN_THR]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_WB_RPT_MODE])
		config_req.ss_wb_rpt_mode = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_WB_RPT_MODE]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_RPT_MODE])
		config_req.ss_rssi_rpt_mode = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_RPT_MODE]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_THR])
		config_req.ss_rssi_thr = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_THR]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PWR_FORMAT])
		config_req.ss_pwr_format = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PWR_FORMAT]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RPT_MODE])
		config_req.ss_rpt_mode = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RPT_MODE]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_BIN_SCALE])
		config_req.ss_bin_scale = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_BIN_SCALE]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DBM_ADJ])
		config_req.ss_dbm_adj = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DBM_ADJ]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_CHN_MASK])
		config_req.ss_chn_mask = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_CHN_MASK]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_PERIOD])
		config_req.ss_fft_period = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_PERIOD]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SHORT_REPORT])
		config_req.ss_short_report = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SHORT_REPORT]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FREQUENCY])
		config_req.ss_frequency = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FREQUENCY]);

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]) {
		status = convert_spectral_mode_nl_to_internal(nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]), &sscan_mode);

		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DEBUG_LEVEL]) {
		spectral_dbg_level = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DEBUG_LEVEL]);
		sscan_req.ss_mode = sscan_mode;
		sscan_req.debug_req.spectral_dbg_level = spectral_dbg_level;
		sscan_req.req_id = SPECTRAL_SET_DEBUG_LEVEL;
		status = ucfg_spectral_control(pdev, &sscan_req);
		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_REQUEST_TYPE])
		scan_req_type = nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_REQUEST_TYPE]);

	if (CONFIG_REQUESTED(scan_req_type)) {
		sscan_req.ss_mode = sscan_mode;
		sscan_req.req_id = SPECTRAL_SET_CONFIG;
		qdf_mem_copy(&sscan_req.config_req.sscan_config, &config_req,
			     qdf_min(sizeof(sscan_req.config_req.sscan_config),
				     sizeof(config_req)));
		status = ucfg_spectral_control(pdev, &sscan_req);
		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	if (SCAN_REQUESTED(scan_req_type)) {
		sscan_req.ss_mode = sscan_mode;
		sscan_req.req_id = SPECTRAL_ACTIVATE_SCAN;
		status = ucfg_spectral_control(pdev, &sscan_req);
		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, sizeof(u64) +
		NLA_HDRLEN + NLMSG_HDRLEN);
	if (!skb) {
		cfg80211_err(" reply skb alloc failed");
		return -ENOMEM;
	}

	cookie = 0;
	if (wlan_cfg80211_nla_put_u64(skb,
				      QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_COOKIE,
				      cookie)) {
		kfree_skb(skb);
		return -EINVAL;
	}

	qal_devcfg_send_response((qdf_nbuf_t)skb);

	return 0;
}

int wlan_cfg80211_spectral_scan_stop(struct wiphy *wiphy,
				     struct wlan_objmgr_pdev *pdev,
				     const void *data,
				     int data_len)
{
	struct nlattr *tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX + 1];
	QDF_STATUS status;
	struct spectral_cp_request sscan_req;
	enum spectral_scan_mode sscan_mode = SPECTRAL_SCAN_MODE_NORMAL;

	if (wlan_cfg80211_nla_parse(
			tb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX,
			data,
			data_len,
			spectral_scan_policy)) {
		cfg80211_err("Invalid Spectral Scan stop ATTR");
		return -EINVAL;
	}

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]) {
		status = convert_spectral_mode_nl_to_internal(nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]), &sscan_mode);

		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	sscan_req.ss_mode = sscan_mode;
	sscan_req.req_id = SPECTRAL_STOP_SCAN;
	status = ucfg_spectral_control(pdev, &sscan_req);
	if (QDF_IS_STATUS_ERROR(status))
		return -EINVAL;

	return 0;
}

int wlan_cfg80211_spectral_scan_get_config(struct wiphy *wiphy,
					   struct wlan_objmgr_pdev *pdev,
					   const void *data,
					   int data_len)
{
	struct nlattr *tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX + 1];
	struct spectral_config *sconfig;
	uint32_t spectral_dbg_level;
	struct sk_buff *skb;
	struct spectral_cp_request sscan_req;
	enum spectral_scan_mode sscan_mode = SPECTRAL_SCAN_MODE_NORMAL;
	QDF_STATUS status;

	if (wlan_cfg80211_nla_parse(
			tb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX,
			data,
			data_len,
			spectral_scan_policy)) {
		cfg80211_err("Invalid Spectral Scan config ATTR");
		return -EINVAL;
	}

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]) {
		status = convert_spectral_mode_nl_to_internal(nla_get_u32(tb
		   [QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_MODE]), &sscan_mode);

		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(u32) +
		NLA_HDRLEN) * QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_MAX +
		NLMSG_HDRLEN);
	if (!skb) {
		cfg80211_err(" reply skb alloc failed");
		return -ENOMEM;
	}

	sscan_req.ss_mode = sscan_mode;
	sscan_req.req_id = SPECTRAL_GET_CONFIG;
	status = ucfg_spectral_control(pdev, &sscan_req);
	sconfig = &sscan_req.config_req.sscan_config;
	if (nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_COUNT,
			sconfig->ss_count) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SCAN_PERIOD,
			sconfig->ss_period) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PRIORITY,
			sconfig->ss_spectral_pri) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_SIZE,
			sconfig->ss_fft_size) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_GC_ENA,
			sconfig->ss_gc_ena) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RESTART_ENA,
			sconfig->ss_restart_ena) ||
	    nla_put_u32(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NOISE_FLOOR_REF,
		sconfig->ss_noise_floor_ref) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_INIT_DELAY,
			sconfig->ss_init_delay) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_NB_TONE_THR,
			sconfig->ss_nb_tone_thr) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_STR_BIN_THR,
			sconfig->ss_str_bin_thr) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_WB_RPT_MODE,
			sconfig->ss_wb_rpt_mode) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_RPT_MODE,
			sconfig->ss_rssi_rpt_mode) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RSSI_THR,
			sconfig->ss_rssi_thr) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_PWR_FORMAT,
			sconfig->ss_pwr_format) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_RPT_MODE,
			sconfig->ss_rpt_mode) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_BIN_SCALE,
			sconfig->ss_bin_scale) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DBM_ADJ,
			sconfig->ss_dbm_adj) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_CHN_MASK,
			sconfig->ss_chn_mask) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FFT_PERIOD,
			sconfig->ss_fft_period) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_SHORT_REPORT,
			sconfig->ss_short_report) ||
	    nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_FREQUENCY,
			sconfig->ss_frequency)) {
		kfree_skb(skb);
		return -EINVAL;
	}

	sscan_req.ss_mode = sscan_mode;
	sscan_req.req_id = SPECTRAL_GET_DEBUG_LEVEL;
	status = ucfg_spectral_control(pdev, &sscan_req);
	spectral_dbg_level = sscan_req.debug_req.spectral_dbg_level;
	if (nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CONFIG_DEBUG_LEVEL,
			spectral_dbg_level)) {
		kfree_skb(skb);
		return -EINVAL;
	}
	qal_devcfg_send_response((qdf_nbuf_t)skb);

	return 0;
}

int wlan_cfg80211_spectral_scan_get_cap(struct wiphy *wiphy,
					struct wlan_objmgr_pdev *pdev,
					const void *data,
					int data_len)
{
	struct spectral_caps *scaps;
	struct sk_buff *skb;
	struct spectral_cp_request sscan_req;
	QDF_STATUS status;

	sscan_req.req_id = SPECTRAL_GET_CAPABILITY_INFO;
	status = ucfg_spectral_control(pdev, &sscan_req);
	scaps = &sscan_req.caps_req.sscan_caps;

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(u32) +
		NLA_HDRLEN) * QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_MAX +
		NLMSG_HDRLEN);
	if (!skb) {
		cfg80211_err(" reply skb alloc failed");
		return -ENOMEM;
	}

	if (scaps->phydiag_cap)
		if (nla_put_flag(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_PHYDIAG))
			goto fail;

	if (scaps->radar_cap)
		if (nla_put_flag(skb,
				 QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_RADAR))
			goto fail;

	if (scaps->spectral_cap)
		if (nla_put_flag(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_SPECTRAL))
			goto fail;

	if (scaps->advncd_spectral_cap)
		if (nla_put_flag(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_ADVANCED_SPECTRAL))
			goto fail;

	if (nla_put_u32(skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_HW_GEN,
			scaps->hw_gen))
		goto fail;

	if (scaps->is_scaling_params_populated) {
		if (nla_put_u16(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_FORMULA_ID,
			scaps->formula_id))
			goto fail;

		if (nla_put_u16(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_LOW_LEVEL_OFFSET,
			scaps->low_level_offset))
			goto fail;

		if (nla_put_u16(
		       skb,
		       QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_HIGH_LEVEL_OFFSET,
		       scaps->high_level_offset))
			goto fail;

		if (nla_put_u16(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_RSSI_THR,
			scaps->rssi_thr))
			goto fail;

		if (nla_put_u8(
		    skb,
		    QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_DEFAULT_AGC_MAX_GAIN,
		    scaps->default_agc_max_gain))
			goto fail;
	}

	if (scaps->agile_spectral_cap) {
		int ret;

		ret = nla_put_flag
			(skb,
			 QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_CAP_AGILE_SPECTRAL);
		if (ret)
			goto fail;
	}
	qal_devcfg_send_response((qdf_nbuf_t)skb);

	return 0;

fail:
	kfree_skb(skb);
	return -EINVAL;
}

int wlan_cfg80211_spectral_scan_get_diag_stats(struct wiphy *wiphy,
					       struct wlan_objmgr_pdev *pdev,
					       const void *data,
					       int data_len)
{
	struct spectral_diag_stats *spetcral_diag;
	struct sk_buff *skb;
	struct spectral_cp_request sscan_req;
	QDF_STATUS status;

	sscan_req.req_id = SPECTRAL_GET_DIAG_STATS;
	status = ucfg_spectral_control(pdev, &sscan_req);
	spetcral_diag = &sscan_req.diag_req.sscan_diag;

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, (sizeof(u64) +
		NLA_HDRLEN) * QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_MAX +
		NLMSG_HDRLEN);
	if (!skb) {
		cfg80211_err(" reply skb alloc failed");
		return -ENOMEM;
	}

	if (wlan_cfg80211_nla_put_u64(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_SIG_MISMATCH,
		spetcral_diag->spectral_mismatch) ||
	    wlan_cfg80211_nla_put_u64(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_SEC80_SFFT_INSUFFLEN,
		spetcral_diag->spectral_sec80_sfft_insufflen) ||
	    wlan_cfg80211_nla_put_u64(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_NOSEC80_SFFT,
		spetcral_diag->spectral_no_sec80_sfft) ||
	    wlan_cfg80211_nla_put_u64(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_VHTSEG1ID_MISMATCH,
		spetcral_diag->spectral_vhtseg1id_mismatch) ||
	    wlan_cfg80211_nla_put_u64(
		skb,
		QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_DIAG_VHTSEG2ID_MISMATCH,
		spetcral_diag->spectral_vhtseg2id_mismatch)) {
		kfree_skb(skb);
		return -EINVAL;
	}
	qal_devcfg_send_response((qdf_nbuf_t)skb);

	return 0;
}

int wlan_cfg80211_spectral_scan_get_status(struct wiphy *wiphy,
					   struct wlan_objmgr_pdev *pdev,
					   const void *data,
					   int data_len)
{
	struct nlattr *tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_MAX + 1];
	struct spectral_scan_state sscan_state = { 0 };
	struct sk_buff *skb;
	struct spectral_cp_request sscan_req;
	enum spectral_scan_mode sscan_mode = SPECTRAL_SCAN_MODE_NORMAL;
	QDF_STATUS status;

	if (wlan_cfg80211_nla_parse(
			tb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_MAX,
			data,
			data_len,
			NULL)) {
		cfg80211_err("Invalid Spectral Scan config ATTR");
		return -EINVAL;
	}

	if (tb[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_MODE]) {
		status = convert_spectral_mode_nl_to_internal(nla_get_u32(tb
		[QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_MODE]), &sscan_mode);

		if (QDF_IS_STATUS_ERROR(status))
			return -EINVAL;
	}

	/* Sending a request and extracting response from it has to be atomic */
	sscan_req.ss_mode = sscan_mode;
	sscan_req.req_id = SPECTRAL_IS_ACTIVE;
	status = ucfg_spectral_control(pdev, &sscan_req);
	sscan_state.is_active = sscan_req.status_req.is_active;

	sscan_req.ss_mode = sscan_mode;
	sscan_req.req_id = SPECTRAL_IS_ENABLED;
	status = ucfg_spectral_control(pdev, &sscan_req);
	sscan_state.is_enabled = sscan_req.status_req.is_enabled;

	skb = cfg80211_vendor_cmd_alloc_reply_skb(wiphy, 2 * (sizeof(u32) +
		NLA_HDRLEN) + NLMSG_HDRLEN);
	if (!skb) {
		cfg80211_err(" reply skb alloc failed");
		return -ENOMEM;
	}

	if (sscan_state.is_enabled)
		if (nla_put_flag(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_IS_ENABLED))
			goto fail;

	if (sscan_state.is_active)
		if (nla_put_flag(
			skb,
			QCA_WLAN_VENDOR_ATTR_SPECTRAL_SCAN_STATUS_IS_ACTIVE))
			goto fail;
	qal_devcfg_send_response((qdf_nbuf_t)skb);

	return 0;
fail:
	kfree_skb(skb);
	return -EINVAL;
}
