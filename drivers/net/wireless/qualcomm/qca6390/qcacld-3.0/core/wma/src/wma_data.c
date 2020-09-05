/*
 * Copyright (c) 2013-2019 The Linux Foundation. All rights reserved.
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
 *  DOC:    wma_data.c
 *  This file contains tx/rx and data path related functions.
 */

/* Header files */

#include "wma.h"
#include "wma_api.h"
#include "cds_api.h"
#include "wmi_unified_api.h"
#include "wlan_qct_sys.h"
#include "wni_api.h"
#include "ani_global.h"
#include "wmi_unified.h"
#include "wni_cfg.h"
#include <cdp_txrx_tx_throttle.h>
#if defined(CONFIG_HL_SUPPORT)
#include "wlan_tgt_def_config_hl.h"
#else
#include "wlan_tgt_def_config.h"
#endif
#include "qdf_nbuf.h"
#include "qdf_types.h"
#include "qdf_mem.h"
#include "qdf_util.h"

#include "wma_types.h"
#include "lim_api.h"
#include "lim_session_utils.h"

#include "cds_utils.h"

#if !defined(REMOVE_PKT_LOG)
#include "pktlog_ac.h"
#endif /* REMOVE_PKT_LOG */

#include "dbglog_host.h"
#include "csr_api.h"
#include "ol_fw.h"

#include "wma_internal.h"
#include "cdp_txrx_flow_ctrl_legacy.h"
#include "cdp_txrx_cmn.h"
#include "cdp_txrx_misc.h"
#include <cdp_txrx_peer_ops.h>
#include <cdp_txrx_cfg.h>
#include "cdp_txrx_stats.h"
#include <cdp_txrx_misc.h>
#include "enet.h"
#include "wlan_mgmt_txrx_utils_api.h"
#include "wlan_objmgr_psoc_obj.h"
#include "wlan_objmgr_pdev_obj.h"
#include "wlan_objmgr_vdev_obj.h"
#include "wlan_objmgr_peer_obj.h"
#include <cdp_txrx_handle.h>
#include "cfg_ucfg_api.h"
#include "wlan_policy_mgr_ucfg.h"
#include <wlan_pmo_ucfg_api.h>
#include "wlan_lmac_if_api.h"
#include <wlan_cp_stats_mc_ucfg_api.h>
#include <wlan_crypto_global_api.h>
#include <wlan_mlme_main.h>

struct wma_search_rate {
	int32_t rate;
	uint8_t flag;
};

#define WMA_MAX_OFDM_CCK_RATE_TBL_SIZE 12
/* In ofdm_cck_rate_tbl->flag, if bit 7 is 1 it's CCK, otherwise it ofdm.
 * Lower bit carries the ofdm/cck index for encoding the rate
 */
static struct wma_search_rate ofdm_cck_rate_tbl[WMA_MAX_OFDM_CCK_RATE_TBL_SIZE] = {
	{540, 4},               /* 4: OFDM 54 Mbps */
	{480, 0},               /* 0: OFDM 48 Mbps */
	{360, 5},               /* 5: OFDM 36 Mbps */
	{240, 1},               /* 1: OFDM 24 Mbps */
	{180, 6},               /* 6: OFDM 18 Mbps */
	{120, 2},               /* 2: OFDM 12 Mbps */
	{110, (1 << 7)},        /* 0: CCK 11 Mbps Long */
	{90, 7},                /* 7: OFDM 9 Mbps  */
	{60, 3},                /* 3: OFDM 6 Mbps  */
	{55, ((1 << 7) | 1)},   /* 1: CCK 5.5 Mbps Long */
	{20, ((1 << 7) | 2)},   /* 2: CCK 2 Mbps Long   */
	{10, ((1 << 7) | 3)} /* 3: CCK 1 Mbps Long   */
};

#define WMA_MAX_VHT20_RATE_TBL_SIZE 9
/* In vht20_400ns_rate_tbl flag carries the mcs index for encoding the rate */
static struct wma_search_rate vht20_400ns_rate_tbl[WMA_MAX_VHT20_RATE_TBL_SIZE] = {
	{867, 8},               /* MCS8 1SS short GI */
	{722, 7},               /* MCS7 1SS short GI */
	{650, 6},               /* MCS6 1SS short GI */
	{578, 5},               /* MCS5 1SS short GI */
	{433, 4},               /* MCS4 1SS short GI */
	{289, 3},               /* MCS3 1SS short GI */
	{217, 2},               /* MCS2 1SS short GI */
	{144, 1},               /* MCS1 1SS short GI */
	{72, 0} /* MCS0 1SS short GI */
};

/* In vht20_800ns_rate_tbl flag carries the mcs index for encoding the rate */
static struct wma_search_rate vht20_800ns_rate_tbl[WMA_MAX_VHT20_RATE_TBL_SIZE] = {
	{780, 8},               /* MCS8 1SS long GI */
	{650, 7},               /* MCS7 1SS long GI */
	{585, 6},               /* MCS6 1SS long GI */
	{520, 5},               /* MCS5 1SS long GI */
	{390, 4},               /* MCS4 1SS long GI */
	{260, 3},               /* MCS3 1SS long GI */
	{195, 2},               /* MCS2 1SS long GI */
	{130, 1},               /* MCS1 1SS long GI */
	{65, 0} /* MCS0 1SS long GI */
};

#define WMA_MAX_VHT40_RATE_TBL_SIZE 10
/* In vht40_400ns_rate_tbl flag carries the mcs index for encoding the rate */
static struct wma_search_rate vht40_400ns_rate_tbl[WMA_MAX_VHT40_RATE_TBL_SIZE] = {
	{2000, 9},              /* MCS9 1SS short GI */
	{1800, 8},              /* MCS8 1SS short GI */
	{1500, 7},              /* MCS7 1SS short GI */
	{1350, 6},              /* MCS6 1SS short GI */
	{1200, 5},              /* MCS5 1SS short GI */
	{900, 4},               /* MCS4 1SS short GI */
	{600, 3},               /* MCS3 1SS short GI */
	{450, 2},               /* MCS2 1SS short GI */
	{300, 1},               /* MCS1 1SS short GI */
	{150, 0},               /* MCS0 1SS short GI */
};

static struct wma_search_rate vht40_800ns_rate_tbl[WMA_MAX_VHT40_RATE_TBL_SIZE] = {
	{1800, 9},              /* MCS9 1SS long GI */
	{1620, 8},              /* MCS8 1SS long GI */
	{1350, 7},              /* MCS7 1SS long GI */
	{1215, 6},              /* MCS6 1SS long GI */
	{1080, 5},              /* MCS5 1SS long GI */
	{810, 4},               /* MCS4 1SS long GI */
	{540, 3},               /* MCS3 1SS long GI */
	{405, 2},               /* MCS2 1SS long GI */
	{270, 1},               /* MCS1 1SS long GI */
	{135, 0} /* MCS0 1SS long GI */
};

#define WMA_MAX_VHT80_RATE_TBL_SIZE 10
static struct wma_search_rate vht80_400ns_rate_tbl[WMA_MAX_VHT80_RATE_TBL_SIZE] = {
	{4333, 9},              /* MCS9 1SS short GI */
	{3900, 8},              /* MCS8 1SS short GI */
	{3250, 7},              /* MCS7 1SS short GI */
	{2925, 6},              /* MCS6 1SS short GI */
	{2600, 5},              /* MCS5 1SS short GI */
	{1950, 4},              /* MCS4 1SS short GI */
	{1300, 3},              /* MCS3 1SS short GI */
	{975, 2},               /* MCS2 1SS short GI */
	{650, 1},               /* MCS1 1SS short GI */
	{325, 0} /* MCS0 1SS short GI */
};

static struct wma_search_rate vht80_800ns_rate_tbl[WMA_MAX_VHT80_RATE_TBL_SIZE] = {
	{3900, 9},              /* MCS9 1SS long GI */
	{3510, 8},              /* MCS8 1SS long GI */
	{2925, 7},              /* MCS7 1SS long GI */
	{2633, 6},              /* MCS6 1SS long GI */
	{2340, 5},              /* MCS5 1SS long GI */
	{1755, 4},              /* MCS4 1SS long GI */
	{1170, 3},              /* MCS3 1SS long GI */
	{878, 2},               /* MCS2 1SS long GI */
	{585, 1},               /* MCS1 1SS long GI */
	{293, 0} /* MCS0 1SS long GI */
};

#define WMA_MAX_HT20_RATE_TBL_SIZE 8
static struct wma_search_rate ht20_400ns_rate_tbl[WMA_MAX_HT20_RATE_TBL_SIZE] = {
	{722, 7},               /* MCS7 1SS short GI */
	{650, 6},               /* MCS6 1SS short GI */
	{578, 5},               /* MCS5 1SS short GI */
	{433, 4},               /* MCS4 1SS short GI */
	{289, 3},               /* MCS3 1SS short GI */
	{217, 2},               /* MCS2 1SS short GI */
	{144, 1},               /* MCS1 1SS short GI */
	{72, 0} /* MCS0 1SS short GI */
};

static struct wma_search_rate ht20_800ns_rate_tbl[WMA_MAX_HT20_RATE_TBL_SIZE] = {
	{650, 7},               /* MCS7 1SS long GI */
	{585, 6},               /* MCS6 1SS long GI */
	{520, 5},               /* MCS5 1SS long GI */
	{390, 4},               /* MCS4 1SS long GI */
	{260, 3},               /* MCS3 1SS long GI */
	{195, 2},               /* MCS2 1SS long GI */
	{130, 1},               /* MCS1 1SS long GI */
	{65, 0} /* MCS0 1SS long GI */
};

#define WMA_MAX_HT40_RATE_TBL_SIZE 8
static struct wma_search_rate ht40_400ns_rate_tbl[WMA_MAX_HT40_RATE_TBL_SIZE] = {
	{1500, 7},              /* MCS7 1SS short GI */
	{1350, 6},              /* MCS6 1SS short GI */
	{1200, 5},              /* MCS5 1SS short GI */
	{900, 4},               /* MCS4 1SS short GI */
	{600, 3},               /* MCS3 1SS short GI */
	{450, 2},               /* MCS2 1SS short GI */
	{300, 1},               /* MCS1 1SS short GI */
	{150, 0} /* MCS0 1SS short GI */
};

static struct wma_search_rate ht40_800ns_rate_tbl[WMA_MAX_HT40_RATE_TBL_SIZE] = {
	{1350, 7},              /* MCS7 1SS long GI */
	{1215, 6},              /* MCS6 1SS long GI */
	{1080, 5},              /* MCS5 1SS long GI */
	{810, 4},               /* MCS4 1SS long GI */
	{540, 3},               /* MCS3 1SS long GI */
	{405, 2},               /* MCS2 1SS long GI */
	{270, 1},               /* MCS1 1SS long GI */
	{135, 0} /* MCS0 1SS long GI */
};

/**
 * wma_bin_search_rate() - binary search function to find rate
 * @tbl: rate table
 * @tbl_size: table size
 * @mbpsx10_rate: return mbps rate
 * @ret_flag: return flag
 *
 * Return: none
 */
static void wma_bin_search_rate(struct wma_search_rate *tbl, int32_t tbl_size,
				int32_t *mbpsx10_rate, uint8_t *ret_flag)
{
	int32_t upper, lower, mid;

	/* the table is descenting. index holds the largest value and the
	 * bottom index holds the smallest value
	 */

	upper = 0;              /* index 0 */
	lower = tbl_size - 1;   /* last index */

	if (*mbpsx10_rate >= tbl[upper].rate) {
		/* use the largest rate */
		*mbpsx10_rate = tbl[upper].rate;
		*ret_flag = tbl[upper].flag;
		return;
	} else if (*mbpsx10_rate <= tbl[lower].rate) {
		/* use the smallest rate */
		*mbpsx10_rate = tbl[lower].rate;
		*ret_flag = tbl[lower].flag;
		return;
	}
	/* now we do binery search to get the floor value */
	while (lower - upper > 1) {
		mid = (upper + lower) >> 1;
		if (*mbpsx10_rate == tbl[mid].rate) {
			/* found the exact match */
			*mbpsx10_rate = tbl[mid].rate;
			*ret_flag = tbl[mid].flag;
			return;
		}
		/* not found. if mid's rate is larger than input move
		 * upper to mid. If mid's rate is larger than input
		 * move lower to mid.
		 */
		if (*mbpsx10_rate > tbl[mid].rate)
			lower = mid;
		else
			upper = mid;
	}
	/* after the bin search the index is the ceiling of rate */
	*mbpsx10_rate = tbl[upper].rate;
	*ret_flag = tbl[upper].flag;
	return;
}

/**
 * wma_fill_ofdm_cck_mcast_rate() - fill ofdm cck mcast rate
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_ofdm_cck_mcast_rate(int32_t mbpsx10_rate,
					       uint8_t nss, uint8_t *rate)
{
	uint8_t idx = 0;

	wma_bin_search_rate(ofdm_cck_rate_tbl, WMA_MAX_OFDM_CCK_RATE_TBL_SIZE,
			    &mbpsx10_rate, &idx);

	/* if bit 7 is set it uses CCK */
	if (idx & 0x80)
		*rate |= (1 << 6) | (idx & 0xF); /* set bit 6 to 1 for CCK */
	else
		*rate |= (idx & 0xF);
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_set_ht_vht_mcast_rate() - set ht/vht mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @sgi_idx: shortgi index
 * @sgi_rate: shortgi rate
 * @lgi_idx: longgi index
 * @lgi_rate: longgi rate
 * @premable: preamble
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: none
 */
static void wma_set_ht_vht_mcast_rate(uint32_t shortgi, int32_t mbpsx10_rate,
				      uint8_t sgi_idx, int32_t sgi_rate,
				      uint8_t lgi_idx, int32_t lgi_rate,
				      uint8_t premable, uint8_t *rate,
				      int32_t *streaming_rate)
{
	if (shortgi == 0) {
		*rate |= (premable << 6) | (lgi_idx & 0xF);
		*streaming_rate = lgi_rate;
	} else {
		*rate |= (premable << 6) | (sgi_idx & 0xF);
		*streaming_rate = sgi_rate;
	}
}

/**
 * wma_fill_ht20_mcast_rate() - fill ht20 mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_ht20_mcast_rate(uint32_t shortgi,
					   int32_t mbpsx10_rate, uint8_t nss,
					   uint8_t *rate,
					   int32_t *streaming_rate)
{
	uint8_t sgi_idx = 0, lgi_idx = 0;
	int32_t sgi_rate, lgi_rate;

	if (nss == 1)
		mbpsx10_rate = mbpsx10_rate >> 1;

	sgi_rate = mbpsx10_rate;
	lgi_rate = mbpsx10_rate;
	if (shortgi)
		wma_bin_search_rate(ht20_400ns_rate_tbl,
				    WMA_MAX_HT20_RATE_TBL_SIZE, &sgi_rate,
				    &sgi_idx);
	else
		wma_bin_search_rate(ht20_800ns_rate_tbl,
				    WMA_MAX_HT20_RATE_TBL_SIZE, &lgi_rate,
				    &lgi_idx);

	wma_set_ht_vht_mcast_rate(shortgi, mbpsx10_rate, sgi_idx, sgi_rate,
				  lgi_idx, lgi_rate, 2, rate, streaming_rate);
	if (nss == 1)
		*streaming_rate = *streaming_rate << 1;
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_fill_ht40_mcast_rate() - fill ht40 mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_ht40_mcast_rate(uint32_t shortgi,
					   int32_t mbpsx10_rate, uint8_t nss,
					   uint8_t *rate,
					   int32_t *streaming_rate)
{
	uint8_t sgi_idx = 0, lgi_idx = 0;
	int32_t sgi_rate, lgi_rate;

	/* for 2x2 divide the rate by 2 */
	if (nss == 1)
		mbpsx10_rate = mbpsx10_rate >> 1;

	sgi_rate = mbpsx10_rate;
	lgi_rate = mbpsx10_rate;
	if (shortgi)
		wma_bin_search_rate(ht40_400ns_rate_tbl,
				    WMA_MAX_HT40_RATE_TBL_SIZE, &sgi_rate,
				    &sgi_idx);
	else
		wma_bin_search_rate(ht40_800ns_rate_tbl,
				    WMA_MAX_HT40_RATE_TBL_SIZE, &lgi_rate,
				    &lgi_idx);

	wma_set_ht_vht_mcast_rate(shortgi, mbpsx10_rate, sgi_idx, sgi_rate,
				  lgi_idx, lgi_rate, 2, rate, streaming_rate);

	return QDF_STATUS_SUCCESS;
}

/**
 * wma_fill_vht20_mcast_rate() - fill vht20 mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_vht20_mcast_rate(uint32_t shortgi,
					    int32_t mbpsx10_rate, uint8_t nss,
					    uint8_t *rate,
					    int32_t *streaming_rate)
{
	uint8_t sgi_idx = 0, lgi_idx = 0;
	int32_t sgi_rate, lgi_rate;

	/* for 2x2 divide the rate by 2 */
	if (nss == 1)
		mbpsx10_rate = mbpsx10_rate >> 1;

	sgi_rate = mbpsx10_rate;
	lgi_rate = mbpsx10_rate;
	if (shortgi)
		wma_bin_search_rate(vht20_400ns_rate_tbl,
				    WMA_MAX_VHT20_RATE_TBL_SIZE, &sgi_rate,
				    &sgi_idx);
	else
		wma_bin_search_rate(vht20_800ns_rate_tbl,
				    WMA_MAX_VHT20_RATE_TBL_SIZE, &lgi_rate,
				    &lgi_idx);

	wma_set_ht_vht_mcast_rate(shortgi, mbpsx10_rate, sgi_idx, sgi_rate,
				  lgi_idx, lgi_rate, 3, rate, streaming_rate);
	if (nss == 1)
		*streaming_rate = *streaming_rate << 1;
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_fill_vht40_mcast_rate() - fill vht40 mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_vht40_mcast_rate(uint32_t shortgi,
					    int32_t mbpsx10_rate, uint8_t nss,
					    uint8_t *rate,
					    int32_t *streaming_rate)
{
	uint8_t sgi_idx = 0, lgi_idx = 0;
	int32_t sgi_rate, lgi_rate;

	/* for 2x2 divide the rate by 2 */
	if (nss == 1)
		mbpsx10_rate = mbpsx10_rate >> 1;

	sgi_rate = mbpsx10_rate;
	lgi_rate = mbpsx10_rate;
	if (shortgi)
		wma_bin_search_rate(vht40_400ns_rate_tbl,
				    WMA_MAX_VHT40_RATE_TBL_SIZE, &sgi_rate,
				    &sgi_idx);
	else
		wma_bin_search_rate(vht40_800ns_rate_tbl,
				    WMA_MAX_VHT40_RATE_TBL_SIZE, &lgi_rate,
				    &lgi_idx);

	wma_set_ht_vht_mcast_rate(shortgi, mbpsx10_rate,
				  sgi_idx, sgi_rate, lgi_idx, lgi_rate,
				  3, rate, streaming_rate);
	if (nss == 1)
		*streaming_rate = *streaming_rate << 1;
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_fill_vht80_mcast_rate() - fill vht80 mcast rate
 * @shortgi: short gaurd interval
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_vht80_mcast_rate(uint32_t shortgi,
					    int32_t mbpsx10_rate, uint8_t nss,
					    uint8_t *rate,
					    int32_t *streaming_rate)
{
	uint8_t sgi_idx = 0, lgi_idx = 0;
	int32_t sgi_rate, lgi_rate;

	/* for 2x2 divide the rate by 2 */
	if (nss == 1)
		mbpsx10_rate = mbpsx10_rate >> 1;

	sgi_rate = mbpsx10_rate;
	lgi_rate = mbpsx10_rate;
	if (shortgi)
		wma_bin_search_rate(vht80_400ns_rate_tbl,
				    WMA_MAX_VHT80_RATE_TBL_SIZE, &sgi_rate,
				    &sgi_idx);
	else
		wma_bin_search_rate(vht80_800ns_rate_tbl,
				    WMA_MAX_VHT80_RATE_TBL_SIZE, &lgi_rate,
				    &lgi_idx);

	wma_set_ht_vht_mcast_rate(shortgi, mbpsx10_rate, sgi_idx, sgi_rate,
				  lgi_idx, lgi_rate, 3, rate, streaming_rate);
	if (nss == 1)
		*streaming_rate = *streaming_rate << 1;
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_fill_ht_mcast_rate() - fill ht mcast rate
 * @shortgi: short gaurd interval
 * @chwidth: channel width
 * @chanmode: channel mode
 * @mhz: frequency
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_ht_mcast_rate(uint32_t shortgi,
					 uint32_t chwidth, int32_t mbpsx10_rate,
					 uint8_t nss, WMI_HOST_WLAN_PHY_MODE chanmode,
					 uint8_t *rate,
					 int32_t *streaming_rate)
{
	int32_t ret = 0;

	*streaming_rate = 0;
	if (chwidth == 0)
		ret = wma_fill_ht20_mcast_rate(shortgi, mbpsx10_rate,
					       nss, rate, streaming_rate);
	else if (chwidth == 1)
		ret = wma_fill_ht40_mcast_rate(shortgi, mbpsx10_rate,
					       nss, rate, streaming_rate);
	else
		WMA_LOGE("%s: Error, Invalid chwidth enum %d", __func__,
			 chwidth);
	return (*streaming_rate != 0) ? QDF_STATUS_SUCCESS : QDF_STATUS_E_INVAL;
}

/**
 * wma_fill_vht_mcast_rate() - fill vht mcast rate
 * @shortgi: short gaurd interval
 * @chwidth: channel width
 * @chanmode: channel mode
 * @mhz: frequency
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 * @streaming_rate: streaming rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_fill_vht_mcast_rate(uint32_t shortgi,
					  uint32_t chwidth,
					  int32_t mbpsx10_rate, uint8_t nss,
					  WMI_HOST_WLAN_PHY_MODE chanmode,
					  uint8_t *rate,
					  int32_t *streaming_rate)
{
	int32_t ret = 0;

	*streaming_rate = 0;
	if (chwidth == 0)
		ret = wma_fill_vht20_mcast_rate(shortgi, mbpsx10_rate, nss,
						rate, streaming_rate);
	else if (chwidth == 1)
		ret = wma_fill_vht40_mcast_rate(shortgi, mbpsx10_rate, nss,
						rate, streaming_rate);
	else if (chwidth == 2)
		ret = wma_fill_vht80_mcast_rate(shortgi, mbpsx10_rate, nss,
						rate, streaming_rate);
	else
		WMA_LOGE("%s: chwidth enum %d not supported",
			 __func__, chwidth);
	return (*streaming_rate != 0) ? QDF_STATUS_SUCCESS : QDF_STATUS_E_INVAL;
}

#define WMA_MCAST_1X1_CUT_OFF_RATE 2000
/**
 * wma_encode_mc_rate() - fill mc rates
 * @shortgi: short gaurd interval
 * @chwidth: channel width
 * @chanmode: channel mode
 * @mhz: frequency
 * @mbpsx10_rate: mbps rates
 * @nss: nss
 * @rate: rate
 *
 * Return: QDF status
 */
static QDF_STATUS wma_encode_mc_rate(uint32_t shortgi, uint32_t chwidth,
			     WMI_HOST_WLAN_PHY_MODE chanmode, A_UINT32 mhz,
			     int32_t mbpsx10_rate, uint8_t nss,
			     uint8_t *rate)
{
	int32_t ret = 0;

	/* nss input value: 0 - 1x1; 1 - 2x2; 2 - 3x3
	 * the phymode selection is based on following assumption:
	 * (1) if the app specifically requested 1x1 or 2x2 we hornor it
	 * (2) if mbpsx10_rate <= 540: always use BG
	 * (3) 540 < mbpsx10_rate <= 2000: use 1x1 HT/VHT
	 * (4) 2000 < mbpsx10_rate: use 2x2 HT/VHT
	 */
	WMA_LOGE("%s: Input: nss = %d, chanmode = %d, mbpsx10 = 0x%x, chwidth = %d, shortgi = %d",
		 __func__, nss, chanmode, mbpsx10_rate, chwidth, shortgi);
	if ((mbpsx10_rate & 0x40000000) && nss > 0) {
		/* bit 30 indicates user inputed nss,
		 * bit 28 and 29 used to encode nss
		 */
		uint8_t user_nss = (mbpsx10_rate & 0x30000000) >> 28;

		nss = (user_nss < nss) ? user_nss : nss;
		/* zero out bits 19 - 21 to recover the actual rate */
		mbpsx10_rate &= ~0x70000000;
	} else if (mbpsx10_rate <= WMA_MCAST_1X1_CUT_OFF_RATE) {
		/* if the input rate is less or equal to the
		 * 1x1 cutoff rate we use 1x1 only
		 */
		nss = 0;
	}
	/* encode NSS bits (bit 4, bit 5) */
	*rate = (nss & 0x3) << 4;
	/* if mcast input rate exceeds the ofdm/cck max rate 54mpbs
	 * we try to choose best ht/vht mcs rate
	 */
	if (540 < mbpsx10_rate) {
		/* cannot use ofdm/cck, choose closest ht/vht mcs rate */
		uint8_t rate_ht = *rate;
		uint8_t rate_vht = *rate;
		int32_t stream_rate_ht = 0;
		int32_t stream_rate_vht = 0;
		int32_t stream_rate = 0;

		ret = wma_fill_ht_mcast_rate(shortgi, chwidth, mbpsx10_rate,
					     nss, chanmode, &rate_ht,
					     &stream_rate_ht);
		if (ret != QDF_STATUS_SUCCESS)
			stream_rate_ht = 0;
		if (mhz < WMA_2_4_GHZ_MAX_FREQ) {
			/* not in 5 GHZ frequency */
			*rate = rate_ht;
			stream_rate = stream_rate_ht;
			goto ht_vht_done;
		}
		/* capable doing 11AC mcast so that search vht tables */
		ret = wma_fill_vht_mcast_rate(shortgi, chwidth, mbpsx10_rate,
					      nss, chanmode, &rate_vht,
					      &stream_rate_vht);
		if (ret != QDF_STATUS_SUCCESS) {
			if (stream_rate_ht != 0)
				ret = QDF_STATUS_SUCCESS;
			*rate = rate_ht;
			stream_rate = stream_rate_ht;
			goto ht_vht_done;
		}
		if (stream_rate_ht == 0) {
			/* only vht rate available */
			*rate = rate_vht;
			stream_rate = stream_rate_vht;
		} else {
			/* set ht as default first */
			*rate = rate_ht;
			stream_rate = stream_rate_ht;
			if (stream_rate < mbpsx10_rate) {
				if (mbpsx10_rate <= stream_rate_vht ||
				    stream_rate < stream_rate_vht) {
					*rate = rate_vht;
					stream_rate = stream_rate_vht;
				}
			} else {
				if (stream_rate_vht >= mbpsx10_rate &&
				    stream_rate_vht < stream_rate) {
					*rate = rate_vht;
					stream_rate = stream_rate_vht;
				}
			}
		}
ht_vht_done:
		WMA_LOGE("%s: NSS = %d, ucast_chanmode = %d, freq = %d",
			 __func__, nss, chanmode, mhz);
		WMA_LOGD(" %s: input_rate = %d, chwidth = %d rate = 0x%x, streaming_rate = %d",
			 __func__, mbpsx10_rate, chwidth, *rate, stream_rate);
	} else {
		if (mbpsx10_rate > 0)
			ret = wma_fill_ofdm_cck_mcast_rate(mbpsx10_rate,
							   nss, rate);
		else
			*rate = 0xFF;

		WMA_LOGE("%s: NSS = %d, ucast_chanmode = %d, input_rate = %d, rate = 0x%x",
			 __func__, nss, chanmode, mbpsx10_rate, *rate);
	}
	return ret;
}

#ifdef QCA_SUPPORT_CP_STATS
/**
 * wma_cp_stats_set_rate_flag() - set rate flags within cp_stats priv object
 * @wma: wma handle
 * @vdev_id: vdev id
 *
 * Return: none
 */
static void wma_cp_stats_set_rate_flag(tp_wma_handle wma, uint8_t vdev_id)
{
	struct wlan_objmgr_vdev *vdev;
	struct wlan_objmgr_psoc *psoc = wma->psoc;
	struct wma_txrx_node *iface = &wma->interfaces[vdev_id];

	vdev = wlan_objmgr_get_vdev_by_id_from_psoc(psoc, vdev_id,
						    WLAN_LEGACY_WMA_ID);
	if (!vdev) {
		WMA_LOGE("%s, vdev not found for id: %d", __func__,
			 vdev_id);
		return;
	}

	ucfg_mc_cp_stats_set_rate_flags(vdev, iface->rate_flags);
	wlan_objmgr_vdev_release_ref(vdev, WLAN_LEGACY_WMA_ID);
}
#else
static void wma_cp_stats_set_rate_flag(tp_wma_handle wma, uint8_t vdev_id) {}
#endif

#ifdef WLAN_FEATURE_11AX
/**
 * wma_set_bss_rate_flags_he() - set rate flags based on BSS capability
 * @iface: txrx_node ctx
 * @add_bss: add_bss params
 *
 * Return: QDF_STATUS
 */
static QDF_STATUS wma_set_bss_rate_flags_he(struct wma_txrx_node *iface,
					    tpAddBssParams add_bss)
{
	if (!add_bss->he_capable)
		return QDF_STATUS_E_NOSUPPORT;

	/*extend TX_RATE_HE160 in future*/
	if (add_bss->ch_width == CH_WIDTH_160MHZ ||
	    add_bss->ch_width == CH_WIDTH_80P80MHZ ||
	    add_bss->ch_width == CH_WIDTH_80MHZ)
		iface->rate_flags |= TX_RATE_HE80;

	else if (add_bss->ch_width)
		iface->rate_flags |= TX_RATE_HE40;
	else
		iface->rate_flags |= TX_RATE_HE20;

	wma_debug("he_capable %d", add_bss->he_capable);
	return QDF_STATUS_SUCCESS;
}
#else
static QDF_STATUS wma_set_bss_rate_flags_he(struct wma_txrx_node *iface,
					    tpAddBssParams add_bss)
{
	return QDF_STATUS_E_NOSUPPORT;
}
#endif
/**
 * wma_set_bss_rate_flags() - 11ax set rate flags based on BSS capability
 * @iface: txrx_node ctx
 * @add_bss: add_bss params
 *
 * Return: none
 */
void wma_set_bss_rate_flags(tp_wma_handle wma, uint8_t vdev_id,
			    tpAddBssParams add_bss)
{
	struct wma_txrx_node *iface = &wma->interfaces[vdev_id];

	iface->rate_flags = 0;

	if (QDF_STATUS_SUCCESS != wma_set_bss_rate_flags_he(iface, add_bss)) {
		if (add_bss->vhtCapable) {
			if (add_bss->ch_width == CH_WIDTH_80P80MHZ)
				iface->rate_flags |= TX_RATE_VHT80;
			if (add_bss->ch_width == CH_WIDTH_160MHZ)
				iface->rate_flags |= TX_RATE_VHT80;
			if (add_bss->ch_width == CH_WIDTH_80MHZ)
				iface->rate_flags |= TX_RATE_VHT80;
			else if (add_bss->ch_width)
				iface->rate_flags |= TX_RATE_VHT40;
			else
				iface->rate_flags |= TX_RATE_VHT20;
		}
		/* avoid to conflict with htCapable flag */
		else if (add_bss->htCapable) {
			if (add_bss->ch_width)
				iface->rate_flags |= TX_RATE_HT40;
			else
				iface->rate_flags |= TX_RATE_HT20;
		}
	}
	if (add_bss->staContext.fShortGI20Mhz ||
	    add_bss->staContext.fShortGI40Mhz)
		iface->rate_flags |= TX_RATE_SGI;

	if (!add_bss->htCapable && !add_bss->vhtCapable)
		iface->rate_flags = TX_RATE_LEGACY;

	wma_debug("capable: vht %u, ht %u, rate_flags %x, ch_width %d",
		  add_bss->vhtCapable, add_bss->htCapable,
		  iface->rate_flags, add_bss->ch_width);

	wma_cp_stats_set_rate_flag(wma, vdev_id);
}

/**
 * wmi_unified_send_txbf() - set txbf parameter to fw
 * @wma: wma handle
 * @params: txbf parameters
 *
 * Return: 0 for success or error code
 */
int32_t wmi_unified_send_txbf(tp_wma_handle wma, tpAddStaParams params)
{
	wmi_vdev_txbf_en txbf_en = {0};

	/* This is set when Other partner is Bformer
	 * and we are capable bformee(enabled both in ini and fw)
	 */
	txbf_en.sutxbfee = params->vhtTxBFCapable;
	txbf_en.mutxbfee = params->vhtTxMUBformeeCapable;
	txbf_en.sutxbfer = params->enable_su_tx_bformer;

	/* When MU TxBfee is set, SU TxBfee must be set by default */
	if (txbf_en.mutxbfee)
		txbf_en.sutxbfee = txbf_en.mutxbfee;

	WMA_LOGD("txbf_en.sutxbfee %d txbf_en.mutxbfee %d, sutxbfer %d",
		 txbf_en.sutxbfee, txbf_en.mutxbfee, txbf_en.sutxbfer);

	return wma_vdev_set_param(wma->wmi_handle,
						params->smesessionId,
						WMI_VDEV_PARAM_TXBF,
						*((A_UINT8 *) &txbf_en));
}

/**
 * wma_data_tx_ack_work_handler() - process data tx ack
 * @ack_work: work structure
 *
 * Return: none
 */
static void wma_data_tx_ack_work_handler(void *ack_work)
{
	struct wma_tx_ack_work_ctx *work;
	tp_wma_handle wma_handle;
	wma_tx_ota_comp_callback ack_cb;

	if (cds_is_load_or_unload_in_progress()) {
		WMA_LOGE("%s: Driver load/unload in progress", __func__);
		return;
	}

	work = (struct wma_tx_ack_work_ctx *)ack_work;

	wma_handle = work->wma_handle;
	ack_cb = wma_handle->umac_data_ota_ack_cb;

	if (work->status)
		WMA_LOGE("Data Tx Ack Cb Status %d", work->status);
	else
		WMA_LOGD("Data Tx Ack Cb Status %d", work->status);

	/* Call the Ack Cb registered by UMAC */
	if (ack_cb)
		ack_cb(wma_handle->mac_context, NULL, work->status, NULL);
	else
		WMA_LOGE("Data Tx Ack Cb is NULL");

	wma_handle->umac_data_ota_ack_cb = NULL;
	wma_handle->last_umac_data_nbuf = NULL;
	qdf_mem_free(work);
	wma_handle->ack_work_ctx = NULL;
}

/**
 * wma_data_tx_ack_comp_hdlr() - handles tx data ack completion
 * @context: context with which the handler is registered
 * @netbuf: tx data nbuf
 * @err: status of tx completion
 *
 * This is the cb registered with TxRx for
 * Ack Complete
 *
 * Return: none
 */
void
wma_data_tx_ack_comp_hdlr(void *wma_context, qdf_nbuf_t netbuf, int32_t status)
{
	void *pdev;
	tp_wma_handle wma_handle = (tp_wma_handle) wma_context;

	if (!wma_handle) {
		WMA_LOGE("%s: Invalid WMA Handle", __func__);
		return;
	}

	pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!pdev)
		return;

	/*
	 * if netBuf does not match with pending nbuf then just free the
	 * netbuf and do not call ack cb
	 */
	if (wma_handle->last_umac_data_nbuf != netbuf) {
		if (wma_handle->umac_data_ota_ack_cb) {
			WMA_LOGE("%s: nbuf does not match but umac_data_ota_ack_cb is not null",
				__func__);
		} else {
			WMA_LOGE("%s: nbuf does not match and umac_data_ota_ack_cb is also null",
				__func__);
		}
		goto free_nbuf;
	}

	if (wma_handle && wma_handle->umac_data_ota_ack_cb) {
		struct wma_tx_ack_work_ctx *ack_work;

		ack_work = qdf_mem_malloc(sizeof(struct wma_tx_ack_work_ctx));
		wma_handle->ack_work_ctx = ack_work;
		if (ack_work) {
			ack_work->wma_handle = wma_handle;
			ack_work->sub_type = 0;
			ack_work->status = status;

			qdf_create_work(0, &ack_work->ack_cmp_work,
					wma_data_tx_ack_work_handler,
					ack_work);
			qdf_sched_work(0, &ack_work->ack_cmp_work);
		}
	}

free_nbuf:
	/* unmap and freeing the tx buf as txrx is not taking care */
	qdf_nbuf_unmap_single(wma_handle->qdf_dev, netbuf, QDF_DMA_TO_DEVICE);
	qdf_nbuf_free(netbuf);
}

/**
 * wma_check_txrx_chainmask() - check txrx chainmask
 * @num_rf_chains: number of rf chains
 * @cmd_value: command value
 *
 * Return: QDF_STATUS_SUCCESS for success or error code
 */
QDF_STATUS wma_check_txrx_chainmask(int num_rf_chains, int cmd_value)
{
	if ((cmd_value > WMA_MAX_RF_CHAINS(num_rf_chains)) ||
	    (cmd_value < WMA_MIN_RF_CHAINS)) {
		WMA_LOGE("%s: Requested value %d over the range",
			__func__, cmd_value);
		return QDF_STATUS_E_INVAL;
	}
	return QDF_STATUS_SUCCESS;
}

/**
 * wma_peer_state_change_event_handler() - peer state change event handler
 * @handle: wma handle
 * @event_buff: event buffer
 * @len: length of buffer
 *
 * This event handler unpauses vdev if peer state change to AUTHORIZED STATE
 *
 * Return: 0 for success or error code
 */
int wma_peer_state_change_event_handler(void *handle,
					uint8_t *event_buff,
					uint32_t len)
{
	WMI_PEER_STATE_EVENTID_param_tlvs *param_buf;
	wmi_peer_state_event_fixed_param *event;
	struct cdp_vdev *vdev;
	tp_wma_handle wma_handle = (tp_wma_handle) handle;

	if (!event_buff) {
		WMA_LOGE("%s: Received NULL event ptr from FW", __func__);
		return -EINVAL;
	}
	param_buf = (WMI_PEER_STATE_EVENTID_param_tlvs *) event_buff;
	if (!param_buf) {
		WMA_LOGE("%s: Received NULL buf ptr from FW", __func__);
		return -ENOMEM;
	}

	event = param_buf->fixed_param;
	vdev = wma_find_vdev_by_id(wma_handle, event->vdev_id);
	if (!vdev) {
		WMA_LOGD("%s: Couldn't find vdev for vdev_id: %d",
			 __func__, event->vdev_id);
		return -EINVAL;
	}

	if ((cdp_get_opmode(cds_get_context(QDF_MODULE_ID_SOC),
			vdev) ==
			wlan_op_mode_sta) &&
		event->state == WMI_PEER_STATE_AUTHORIZED) {
		/*
		 * set event so that hdd
		 * can procced and unpause tx queue
		 */
#ifdef QCA_LL_LEGACY_TX_FLOW_CONTROL
		if (!wma_handle->peer_authorized_cb) {
			WMA_LOGE("%s: peer authorized cb not registered",
				 __func__);
			return -EINVAL;
		}
		wma_handle->peer_authorized_cb(event->vdev_id);
#endif
	}

	return 0;
}

/**
 * wma_set_enable_disable_mcc_adaptive_scheduler() -enable/disable mcc scheduler
 * @mcc_adaptive_scheduler: enable/disable
 *
 * This function enable/disable mcc adaptive scheduler in fw.
 *
 * Return: QDF_STATUS_SUCCESS for success or error code
 */
QDF_STATUS wma_set_enable_disable_mcc_adaptive_scheduler(uint32_t
							 mcc_adaptive_scheduler)
{
	tp_wma_handle wma = NULL;
	uint32_t pdev_id;

	wma = cds_get_context(QDF_MODULE_ID_WMA);
	if (!wma)
		return QDF_STATUS_E_FAULT;

	/*
	 * Since there could be up to two instances of OCS in FW (one per MAC),
	 * FW provides the option of enabling and disabling MAS on a per MAC
	 * basis. But, Host does not have enable/disable option for individual
	 * MACs. So, FW agreed for the Host to send down a 'pdev id' of 0.
	 * When 'pdev id' of 0 is used, FW treats this as a SOC level command
	 * and applies the same value to both MACs. Irrespective of the value
	 * of 'WMI_SERVICE_DEPRECATED_REPLACE', the pdev id needs to be '0'
	 * (SOC level) for WMI_RESMGR_ADAPTIVE_OCS_ENABLE_DISABLE_CMDID
	 */
	pdev_id = WMI_PDEV_ID_SOC;

	return wmi_unified_set_enable_disable_mcc_adaptive_scheduler_cmd(
			wma->wmi_handle, mcc_adaptive_scheduler, pdev_id);
}

/**
 * wma_set_mcc_channel_time_latency() -set MCC channel time latency
 * @wma: wma handle
 * @mcc_channel: mcc channel
 * @mcc_channel_time_latency: MCC channel time latency.
 *
 * Currently used to set time latency for an MCC vdev/adapter using operating
 * channel of it and channel number. The info is provided run time using
 * iwpriv command: iwpriv <wlan0 | p2p0> setMccLatency <latency in ms>.
 *
 * Return: QDF status
 */
QDF_STATUS wma_set_mcc_channel_time_latency(tp_wma_handle wma,
	uint32_t mcc_channel, uint32_t mcc_channel_time_latency)
{
	uint8_t mcc_adapt_sch = 0;
	struct mac_context *mac = NULL;
	uint32_t channel1 = mcc_channel;
	uint32_t chan1_freq = cds_chan_to_freq(channel1);

	if (!wma) {
		WMA_LOGE("%s:NULL wma ptr. Exiting", __func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}
	mac = cds_get_context(QDF_MODULE_ID_PE);
	if (!mac) {
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	/* First step is to confirm if MCC is active */
	if (!lim_is_in_mcc(mac)) {
		WMA_LOGE("%s: MCC is not active. Exiting", __func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}
	/* Confirm MCC adaptive scheduler feature is disabled */
	if (policy_mgr_get_mcc_adaptive_sch(mac->psoc,
					    &mcc_adapt_sch) ==
	    QDF_STATUS_SUCCESS) {
		if (mcc_adapt_sch ==
		    cfg_max(CFG_ENABLE_MCC_ADAPTIVE_SCH_ENABLED_NAME)) {
			WMA_LOGD("%s: Can't set channel latency while MCC ADAPTIVE SCHED is enabled. Exit",
				__func__);
			return QDF_STATUS_SUCCESS;
		}
	} else {
		WMA_LOGE("%s: Failed to get value for MCC_ADAPTIVE_SCHED, "
			 "Exit w/o setting latency", __func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	return wmi_unified_set_mcc_channel_time_latency_cmd(wma->wmi_handle,
						chan1_freq,
						mcc_channel_time_latency);
}

/**
 * wma_set_mcc_channel_time_quota() -set MCC channel time quota
 * @wma: wma handle
 * @adapter_1_chan_number: adapter 1 channel number
 * @adapter_1_quota: adapter 1 quota
 * @adapter_2_chan_number: adapter 2 channel number
 *
 * Currently used to set time quota for 2 MCC vdevs/adapters using (operating
 * channel, quota) for each mode . The info is provided run time using
 * iwpriv command: iwpriv <wlan0 | p2p0> setMccQuota <quota in ms>.
 * Note: the quota provided in command is for the same mode in cmd. HDD
 * checks if MCC mode is active, gets the second mode and its operating chan.
 * Quota for the 2nd role is calculated as 100 - quota of first mode.
 *
 * Return: QDF status
 */
QDF_STATUS wma_set_mcc_channel_time_quota(tp_wma_handle wma,
		uint32_t adapter_1_chan_number,	uint32_t adapter_1_quota,
		uint32_t adapter_2_chan_number)
{
	uint8_t mcc_adapt_sch = 0;
	struct mac_context *mac = NULL;
	uint32_t chan1_freq = cds_chan_to_freq(adapter_1_chan_number);
	uint32_t chan2_freq = cds_chan_to_freq(adapter_2_chan_number);

	if (!wma) {
		WMA_LOGE("%s:NULL wma ptr. Exiting", __func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}
	mac = cds_get_context(QDF_MODULE_ID_PE);
	if (!mac) {
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	/* First step is to confirm if MCC is active */
	if (!lim_is_in_mcc(mac)) {
		WMA_LOGD("%s: MCC is not active. Exiting", __func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	/* Confirm MCC adaptive scheduler feature is disabled */
	if (policy_mgr_get_mcc_adaptive_sch(mac->psoc,
					    &mcc_adapt_sch) ==
	    QDF_STATUS_SUCCESS) {
		if (mcc_adapt_sch ==
		    cfg_max(CFG_ENABLE_MCC_ADAPTIVE_SCH_ENABLED_NAME)) {
			WMA_LOGD("%s: Can't set channel quota while MCC_ADAPTIVE_SCHED is enabled. Exit",
				 __func__);
			return QDF_STATUS_SUCCESS;
		}
	} else {
		WMA_LOGE("%s: Failed to retrieve WNI_CFG_ENABLE_MCC_ADAPTIVE_SCHED. Exit",
			__func__);
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	return wmi_unified_set_mcc_channel_time_quota_cmd(wma->wmi_handle,
						chan1_freq,
						adapter_1_quota,
						chan2_freq);
}

/**
 * wma_set_linkstate() - set wma linkstate
 * @wma: wma handle
 * @params: link state params
 *
 * Return: none
 */
void wma_set_linkstate(tp_wma_handle wma, tpLinkStateParams params)
{
	struct cdp_pdev *pdev;
	struct cdp_vdev *vdev;
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);
	uint8_t vdev_id;
	bool roam_synch_in_progress = false;
	QDF_STATUS status;
	struct wma_target_req *msg;

	params->status = true;
	WMA_LOGD("%s: state %d selfmac %pM", __func__,
		 params->state, params->self_mac_addr);
	if ((params->state != eSIR_LINK_PREASSOC_STATE) &&
	    (params->state != eSIR_LINK_DOWN_STATE)) {
		WMA_LOGD("%s: unsupported link state %d",
			 __func__, params->state);
		params->status = false;
		goto out;
	}

	pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!pdev) {
		params->status = false;
		goto out;
	}

	vdev = wma_find_vdev_by_addr(wma, params->self_mac_addr, &vdev_id);
	if (!vdev) {
		WMA_LOGP("%s: vdev not found for addr: %pM",
			 __func__, params->self_mac_addr);
		params->status = false;
		goto out;
	}

	if (wma_is_vdev_in_ap_mode(wma, vdev_id)) {
		WMA_LOGD("%s: Ignoring set link req in ap mode", __func__);
		params->status = false;
		goto out;
	}

	if (params->state == eSIR_LINK_PREASSOC_STATE) {
		if (wma_is_roam_synch_in_progress(wma, vdev_id))
			roam_synch_in_progress = true;
		status = wma_create_peer(wma, pdev, vdev, params->bssid,
				WMI_PEER_TYPE_DEFAULT, vdev_id,
				roam_synch_in_progress);
		if (status != QDF_STATUS_SUCCESS) {
			WMA_LOGE("%s: Unable to create peer", __func__);
			params->status = false;
		}
		if (roam_synch_in_progress)
			return;
	} else {
		WMA_LOGD("%s, vdev_id: %d, pausing tx_ll_queue for VDEV_STOP",
			 __func__, vdev_id);
		cdp_fc_vdev_pause(soc,
			wma->interfaces[vdev_id].handle,
			OL_TXQ_PAUSE_REASON_VDEV_STOP);
		msg = wma_fill_vdev_req(wma, vdev_id,
				WMA_SET_LINK_STATE,
				WMA_TARGET_REQ_TYPE_VDEV_STOP, params,
				WMA_VDEV_STOP_REQUEST_TIMEOUT);
		if (!msg) {
			WMA_LOGP(FL("Failed to fill vdev request for vdev_id %d"),
				 vdev_id);
			params->status = false;
			status = QDF_STATUS_E_NOMEM;
			goto out;
		}
		wma_vdev_set_pause_bit(vdev_id, PAUSE_TYPE_HOST);
		if (wma_send_vdev_stop_to_fw(wma, vdev_id)) {
			WMA_LOGP("%s: %d Failed to send vdev stop",
				 __func__, __LINE__);
			params->status = false;
			wma_remove_vdev_req(wma, vdev_id,
				WMA_TARGET_REQ_TYPE_VDEV_STOP);
		} else {
			WMA_LOGP("%s: %d vdev stop sent vdev %d",
				 __func__, __LINE__, vdev_id);
			/*
			 * Remove peer, Vdev down and sending set link
			 * response will be handled in vdev stop response
			 * handler
			 */
			return;
		}
	}
out:
	wma_send_msg(wma, WMA_SET_LINK_STATE_RSP, (void *)params, 0);
}

/**
 * wma_process_rate_update_indate() - rate update indication
 * @wma: wma handle
 * @pRateUpdateParams: Rate update params
 *
 * This function update rate & short GI interval to fw based on params
 * send by SME.
 *
 * Return: QDF status
 */
QDF_STATUS wma_process_rate_update_indicate(tp_wma_handle wma,
					    tSirRateUpdateInd *
					    pRateUpdateParams)
{
	int32_t ret = 0;
	uint8_t vdev_id = 0;
	void *pdev;
	int32_t mbpsx10_rate = -1;
	uint32_t paramId;
	uint8_t rate = 0;
	uint32_t short_gi;
	struct wma_txrx_node *intr = wma->interfaces;
	QDF_STATUS status;

	/* Get the vdev id */
	pdev = wma_find_vdev_by_addr(wma, pRateUpdateParams->bssid.bytes,
					&vdev_id);
	if (!pdev) {
		WMA_LOGE("vdev handle is invalid for %pM",
			 pRateUpdateParams->bssid.bytes);
		qdf_mem_free(pRateUpdateParams);
		return QDF_STATUS_E_INVAL;
	}
	short_gi = intr[vdev_id].config.shortgi;
	if (short_gi == 0)
		short_gi = (intr[vdev_id].rate_flags & TX_RATE_SGI) ?
								 true : false;
	/* first check if reliable TX mcast rate is used. If not check the bcast
	 * Then is mcast. Mcast rate is saved in mcastDataRate24GHz
	 */
	if (pRateUpdateParams->reliableMcastDataRateTxFlag > 0) {
		mbpsx10_rate = pRateUpdateParams->reliableMcastDataRate;
		paramId = WMI_VDEV_PARAM_MCAST_DATA_RATE;
		if (pRateUpdateParams->
		    reliableMcastDataRateTxFlag & TX_RATE_SGI)
			short_gi = 1;   /* upper layer specified short GI */
	} else if (pRateUpdateParams->bcastDataRate > -1) {
		mbpsx10_rate = pRateUpdateParams->bcastDataRate;
		paramId = WMI_VDEV_PARAM_BCAST_DATA_RATE;
	} else {
		mbpsx10_rate = pRateUpdateParams->mcastDataRate24GHz;
		paramId = WMI_VDEV_PARAM_MCAST_DATA_RATE;
		if (pRateUpdateParams->
		    mcastDataRate24GHzTxFlag & TX_RATE_SGI)
			short_gi = 1;   /* upper layer specified short GI */
	}
	WMA_LOGE("%s: dev_id = %d, dev_type = %d, dev_mode = %d,",
		 __func__, vdev_id, intr[vdev_id].type,
		 pRateUpdateParams->dev_mode);
	WMA_LOGE("%s: mac = %pM, config.shortgi = %d, rate_flags = 0x%x",
		 __func__, pRateUpdateParams->bssid.bytes,
		 intr[vdev_id].config.shortgi, intr[vdev_id].rate_flags);
	ret = wma_encode_mc_rate(short_gi, intr[vdev_id].config.chwidth,
				 intr[vdev_id].chanmode, intr[vdev_id].mhz,
				 mbpsx10_rate, pRateUpdateParams->nss, &rate);
	if (ret != QDF_STATUS_SUCCESS) {
		WMA_LOGE("%s: Error, Invalid input rate value", __func__);
		qdf_mem_free(pRateUpdateParams);
		return ret;
	}
	status = wma_vdev_set_param(wma->wmi_handle, vdev_id,
					      WMI_VDEV_PARAM_SGI, short_gi);
	if (QDF_IS_STATUS_ERROR(status)) {
		WMA_LOGE("%s: Fail to Set WMI_VDEV_PARAM_SGI(%d), status = %d",
			 __func__, short_gi, status);
		qdf_mem_free(pRateUpdateParams);
		return status;
	}
	status = wma_vdev_set_param(wma->wmi_handle,
					      vdev_id, paramId, rate);
	qdf_mem_free(pRateUpdateParams);
	if (QDF_IS_STATUS_ERROR(status)) {
		WMA_LOGE("%s: Fail to Set rate, status = %d", __func__, status);
		return status;
	}

	return QDF_STATUS_SUCCESS;
}

/**
 * wma_mgmt_tx_ack_comp_hdlr() - handles tx ack mgmt completion
 * @context: context with which the handler is registered
 * @netbuf: tx mgmt nbuf
 * @status: status of tx completion
 *
 * This is callback registered with TxRx for
 * Ack Complete.
 *
 * Return: none
 */
static void
wma_mgmt_tx_ack_comp_hdlr(void *wma_context, qdf_nbuf_t netbuf, int32_t status)
{
	tp_wma_handle wma_handle = (tp_wma_handle) wma_context;
	struct wlan_objmgr_pdev *pdev = (struct wlan_objmgr_pdev *)
					wma_handle->pdev;
	uint16_t desc_id;

	desc_id = QDF_NBUF_CB_MGMT_TXRX_DESC_ID(netbuf);

	mgmt_txrx_tx_completion_handler(pdev, desc_id, status, NULL);
}

/**
 * wma_mgmt_tx_dload_comp_hldr() - handles tx mgmt completion
 * @context: context with which the handler is registered
 * @netbuf: tx mgmt nbuf
 * @status: status of tx completion
 *
 * This function calls registered download callback while sending mgmt packet.
 *
 * Return: none
 */
static void
wma_mgmt_tx_dload_comp_hldr(void *wma_context, qdf_nbuf_t netbuf,
			    int32_t status)
{
	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;

	tp_wma_handle wma_handle = (tp_wma_handle) wma_context;
	void *mac_context = wma_handle->mac_context;

	WMA_LOGD("Tx Complete Status %d", status);

	if (!wma_handle->tx_frm_download_comp_cb) {
		WMA_LOGE("Tx Complete Cb not registered by umac");
		return;
	}

	/* Call Tx Mgmt Complete Callback registered by umac */
	wma_handle->tx_frm_download_comp_cb(mac_context, netbuf, 0);

	/* Reset Callback */
	wma_handle->tx_frm_download_comp_cb = NULL;

	/* Set the Tx Mgmt Complete Event */
	qdf_status = qdf_event_set(&wma_handle->tx_frm_download_comp_event);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status))
		WMA_LOGP("%s: Event Set failed - tx_frm_comp_event", __func__);
}

/**
 * wma_tx_attach() - attach tx related callbacks
 * @pwmaCtx: wma context
 *
 * attaches tx fn with underlying layer.
 *
 * Return: QDF status
 */
QDF_STATUS wma_tx_attach(tp_wma_handle wma_handle)
{
	/* Get the Vos Context */
	struct cds_context *cds_handle =
		(struct cds_context *) (wma_handle->cds_context);

	/* Get the txRx Pdev handle */
	struct cdp_pdev *txrx_pdev = cds_handle->pdev_txrx_ctx;
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);

	/* Register for Tx Management Frames */
	cdp_mgmt_tx_cb_set(soc, txrx_pdev, 0,
			wma_mgmt_tx_dload_comp_hldr,
			wma_mgmt_tx_ack_comp_hdlr, wma_handle);

	/* Register callback to send PEER_UNMAP_RESPONSE cmd*/
	if (cdp_cfg_get_peer_unmap_conf_support(soc))
		cdp_peer_unmap_sync_cb_set(soc, txrx_pdev,
					   wma_peer_unmap_conf_cb);

	/* Store the Mac Context */
	wma_handle->mac_context = cds_handle->mac_context;

	return QDF_STATUS_SUCCESS;
}

/**
 * wma_tx_detach() - detach tx related callbacks
 * @tp_wma_handle: wma context
 *
 * Deregister with TxRx for Tx Mgmt Download and Ack completion.
 *
 * Return: QDF status
 */
QDF_STATUS wma_tx_detach(tp_wma_handle wma_handle)
{
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);

	/* Get the Vos Context */
	struct cds_context *cds_handle =
		(struct cds_context *) (wma_handle->cds_context);

	/* Get the txRx Pdev handle */
	struct cdp_pdev *txrx_pdev = cds_handle->pdev_txrx_ctx;

	if (!soc) {
		WMA_LOGE("%s:SOC context is NULL", __func__);
		return QDF_STATUS_E_FAILURE;
	}

	if (txrx_pdev) {
		/* Deregister with TxRx for Tx Mgmt completion call back */
		cdp_mgmt_tx_cb_set(soc, txrx_pdev, 0, NULL, NULL, txrx_pdev);
	}

	/* Reset Tx Frm Callbacks */
	wma_handle->tx_frm_download_comp_cb = NULL;

	/* Reset Tx Data Frame Ack Cb */
	wma_handle->umac_data_ota_ack_cb = NULL;

	/* Reset last Tx Data Frame nbuf ptr */
	wma_handle->last_umac_data_nbuf = NULL;

	return QDF_STATUS_SUCCESS;
}

#if defined(QCA_LL_LEGACY_TX_FLOW_CONTROL) || \
	defined(QCA_LL_TX_FLOW_CONTROL_V2) || defined(CONFIG_HL_SUPPORT)

/**
 * wma_mcc_vdev_tx_pause_evt_handler() - pause event handler
 * @handle: wma handle
 * @event: event buffer
 * @len: data length
 *
 * This function handle pause event from fw and pause/unpause
 * vdev.
 *
 * Return: 0 for success or error code.
 */
int wma_mcc_vdev_tx_pause_evt_handler(void *handle, uint8_t *event,
				      uint32_t len)
{
	tp_wma_handle wma = (tp_wma_handle) handle;
	WMI_TX_PAUSE_EVENTID_param_tlvs *param_buf;
	wmi_tx_pause_event_fixed_param *wmi_event;
	uint8_t vdev_id;
	A_UINT32 vdev_map;
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);

	param_buf = (WMI_TX_PAUSE_EVENTID_param_tlvs *) event;
	if (!param_buf) {
		WMA_LOGE("Invalid roam event buffer");
		return -EINVAL;
	}

	if (ucfg_pmo_get_wow_bus_suspend(wma->psoc)) {
		WMA_LOGD(" Suspend is in progress: Pause/Unpause Tx is NoOp");
		return 0;
	}

	if (!soc) {
		WMA_LOGE("%s:SOC context is NULL", __func__);
		return -EINVAL;
	}

	wmi_event = param_buf->fixed_param;
	vdev_map = wmi_event->vdev_map;
	/* FW mapped vdev from ID
	 * vdev_map = (1 << vdev_id)
	 * So, host should unmap to ID
	 */
	for (vdev_id = 0; vdev_map != 0 && vdev_id < wma->max_bssid;
	     vdev_id++) {
		if (!(vdev_map & 0x1)) {
			/* No Vdev */
		} else {
			if (!wma->interfaces[vdev_id].handle) {
				WMA_LOGE("%s: invalid vdev ID %d", __func__,
					 vdev_id);
				/* Test Next VDEV */
				vdev_map >>= 1;
				continue;
			}

			/* PAUSE action, add bitmap */
			if (ACTION_PAUSE == wmi_event->action) {
				/*
				 * Now only support per-dev pause so it is not
				 * necessary to pause a paused queue again.
				 */
				if (!wma_vdev_get_pause_bitmap(vdev_id))
					cdp_fc_vdev_pause(soc,
						wma->
						interfaces[vdev_id].handle,
						OL_TXQ_PAUSE_REASON_FW);
				wma_vdev_set_pause_bit(vdev_id,
					wmi_event->pause_type);
			}
			/* UNPAUSE action, clean bitmap */
			else if (ACTION_UNPAUSE == wmi_event->action) {
				/* Handle unpause only if already paused */
				if (wma_vdev_get_pause_bitmap(vdev_id)) {
					wma_vdev_clear_pause_bit(vdev_id,
						wmi_event->pause_type);

					if (!wma->interfaces[vdev_id].
					    pause_bitmap) {
						/* PAUSE BIT MAP is cleared
						 * UNPAUSE VDEV
						 */
						cdp_fc_vdev_unpause(soc,
							wma->interfaces[vdev_id]
							.handle,
							OL_TXQ_PAUSE_REASON_FW);
					}
				}
			} else {
				WMA_LOGE("Not Valid Action Type %d",
					 wmi_event->action);
			}

			WMA_LOGD
				("vdev_id %d, pause_map 0x%x, pause type %d, action %d",
				vdev_id, wma_vdev_get_pause_bitmap(vdev_id),
				wmi_event->pause_type, wmi_event->action);
		}
		/* Test Next VDEV */
		vdev_map >>= 1;
	}

	return 0;
}

#endif /* QCA_LL_LEGACY_TX_FLOW_CONTROL */

#if defined(CONFIG_HL_SUPPORT) && defined(QCA_BAD_PEER_TX_FLOW_CL)

/**
 * wma_set_peer_rate_report_condition -
 *                    this function set peer rate report
 *                    condition info to firmware.
 * @handle:	Handle of WMA
 * @config:	Bad peer configuration from SIR module
 *
 * It is a wrapper function to sent WMI_PEER_SET_RATE_REPORT_CONDITION_CMDID
 * to the firmare\target.If the command sent to firmware failed, free the
 * buffer that allocated.
 *
 * Return: QDF_STATUS based on values sent to firmware
 */
static
QDF_STATUS wma_set_peer_rate_report_condition(WMA_HANDLE handle,
			struct t_bad_peer_txtcl_config *config)
{
	tp_wma_handle wma_handle = (tp_wma_handle)handle;
	struct wmi_peer_rate_report_params rate_report_params = {0};
	u_int32_t i, j;

	rate_report_params.rate_report_enable = config->enable;
	rate_report_params.backoff_time = config->tgt_backoff;
	rate_report_params.timer_period = config->tgt_report_prd;
	for (i = 0; i < WMI_PEER_RATE_REPORT_COND_MAX_NUM; i++) {
		rate_report_params.report_per_phy[i].cond_flags =
			config->threshold[i].cond;
		rate_report_params.report_per_phy[i].delta.delta_min  =
			config->threshold[i].delta;
		rate_report_params.report_per_phy[i].delta.percent =
			config->threshold[i].percentage;
		for (j = 0; j < WMI_MAX_NUM_OF_RATE_THRESH; j++) {
			rate_report_params.report_per_phy[i].
				report_rate_threshold[j] =
					config->threshold[i].thresh[j];
		}
	}

	return wmi_unified_peer_rate_report_cmd(wma_handle->wmi_handle,
						&rate_report_params);
}

/**
 * wma_process_init_bad_peer_tx_ctl_info -
 *                this function to initialize peer rate report config info.
 * @handle:	Handle of WMA
 * @config:	Bad peer configuration from SIR module
 *
 * This function initializes the bad peer tx control data structure in WMA,
 * sends down the initial configuration to the firmware and configures
 * the peer status update seeting in the tx_rx module.
 *
 * Return: QDF_STATUS based on procedure status
 */

QDF_STATUS wma_process_init_bad_peer_tx_ctl_info(tp_wma_handle wma,
					struct t_bad_peer_txtcl_config *config)
{
	/* Parameter sanity check */
	struct cdp_pdev *curr_pdev;
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);

	if (!wma || !config) {
		WMA_LOGE("%s Invalid input\n", __func__);
		return QDF_STATUS_E_FAILURE;
	}

	curr_pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!curr_pdev)
		return QDF_STATUS_E_FAILURE;

	WMA_LOGE("%s enable %d period %d txq limit %d\n", __func__,
		 config->enable,
		 config->period,
		 config->txq_limit);

	/* Only need to initialize the setting
	 * when the feature is enabled
	 */
	if (config->enable) {
		int i = 0;

		cdp_bad_peer_txctl_set_setting(soc,
					curr_pdev,
					config->enable,
					config->period,
					config->txq_limit);

		for (i = 0; i < WLAN_WMA_IEEE80211_MAX_LEVEL; i++) {
			u_int32_t threshold, limit;

			threshold = config->threshold[i].thresh[0];
			limit =	config->threshold[i].txlimit;
			cdp_bad_peer_txctl_update_threshold(soc,
						curr_pdev,
						i,
						threshold,
						limit);
		}
	}

	return wma_set_peer_rate_report_condition(wma, config);
}
#endif /* defined(CONFIG_HL_SUPPORT) && defined(QCA_BAD_PEER_TX_FLOW_CL) */

#ifdef FW_THERMAL_THROTTLE_SUPPORT
/**
 * wma_update_thermal_mitigation_to_fw - update thermal mitigation to fw
 * @wma: wma handle
 * @thermal_level: thermal level
 *
 * This function sends down thermal mitigation params to the fw
 *
 * Returns: QDF_STATUS_SUCCESS for success otherwise failure
 */
static QDF_STATUS wma_update_thermal_mitigation_to_fw(tp_wma_handle wma,
						      u_int8_t thermal_level)
{
	struct thermal_mitigation_params therm_data;

	/* Check if vdev is in mcc, if in mcc set dc value as 10, else 100 */
	therm_data.dc = 100;
	therm_data.enable = 1;
	therm_data.levelconf[0].dcoffpercent =
		wma->thermal_mgmt_info.throttle_duty_cycle_tbl[thermal_level];
	therm_data.levelconf[0].priority = 0;

	return wmi_unified_thermal_mitigation_param_cmd_send(wma, &therm_data);
}
#else /* FW_THERMAL_THROTTLE_SUPPORT */
/**
 * wma_update_thermal_mitigation_to_fw - update thermal mitigation to fw
 * @wma: wma handle
 * @thermal_level: thermal level
 *
 * This function sends down thermal mitigation params to the fw
 *
 * Returns: QDF_STATUS_SUCCESS for success otherwise failure
 */
static QDF_STATUS wma_update_thermal_mitigation_to_fw(tp_wma_handle wma,
						      u_int8_t thermal_level)
{
	return QDF_STATUS_SUCCESS;
}
#endif

/**
 * wma_update_thermal_cfg_to_fw() - update thermal configuration to FW
 * @wma: Pointer to WMA handle
 *
 * This function update thermal configuration to FW
 *
 * Returns: QDF_STATUS_SUCCESS for success otherwise failure
 */
static QDF_STATUS wma_update_thermal_cfg_to_fw(tp_wma_handle wma)
{
	t_thermal_cmd_params thermal_params;

	/* Get the temperature thresholds to set in firmware */
	thermal_params.minTemp =
		wma->thermal_mgmt_info.thermalLevels[WLAN_WMA_THERMAL_LEVEL_0].
		minTempThreshold;
	thermal_params.maxTemp =
		wma->thermal_mgmt_info.thermalLevels[WLAN_WMA_THERMAL_LEVEL_0].
		maxTempThreshold;
	thermal_params.thermalEnable =
		wma->thermal_mgmt_info.thermalMgmtEnabled;

	WMA_LOGE("TM sending to fw: min_temp %d max_temp %d enable %d",
		 thermal_params.minTemp, thermal_params.maxTemp,
		 thermal_params.thermalEnable);

	return wma_set_thermal_mgmt(wma, thermal_params);
}

/**
 * wma_process_init_thermal_info() - initialize thermal info
 * @wma: Pointer to WMA handle
 * @pThermalParams: Pointer to thermal mitigation parameters
 *
 * This function initializes the thermal management table in WMA,
 * sends down the initial temperature thresholds to the firmware
 * and configures the throttle period in the tx rx module
 *
 * Returns: QDF_STATUS_SUCCESS for success otherwise failure
 */
QDF_STATUS wma_process_init_thermal_info(tp_wma_handle wma,
					 t_thermal_mgmt *pThermalParams)
{
	struct cdp_pdev *curr_pdev;
	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;
#ifdef FW_THERMAL_THROTTLE_SUPPORT
	int i = 0;
#endif /* FW_THERMAL_THROTTLE_SUPPORT */

	if (!wma || !pThermalParams) {
		WMA_LOGE("TM Invalid input");
		return QDF_STATUS_E_FAILURE;
	}

	curr_pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!curr_pdev)
		return QDF_STATUS_E_FAILURE;

	WMA_LOGD("TM enable %d period %d", pThermalParams->thermalMgmtEnabled,
		 pThermalParams->throttlePeriod);

	WMA_LOGD("Throttle Duty Cycle Level in percentage:\n"
		 "0 %d\n"
		 "1 %d\n"
		 "2 %d\n"
		 "3 %d",
		 pThermalParams->throttle_duty_cycle_tbl[0],
		 pThermalParams->throttle_duty_cycle_tbl[1],
		 pThermalParams->throttle_duty_cycle_tbl[2],
		 pThermalParams->throttle_duty_cycle_tbl[3]);

	wma->thermal_mgmt_info.thermalMgmtEnabled =
		pThermalParams->thermalMgmtEnabled;
	wma->thermal_mgmt_info.thermalLevels[0].minTempThreshold =
		pThermalParams->thermalLevels[0].minTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[0].maxTempThreshold =
		pThermalParams->thermalLevels[0].maxTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[1].minTempThreshold =
		pThermalParams->thermalLevels[1].minTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[1].maxTempThreshold =
		pThermalParams->thermalLevels[1].maxTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[2].minTempThreshold =
		pThermalParams->thermalLevels[2].minTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[2].maxTempThreshold =
		pThermalParams->thermalLevels[2].maxTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[3].minTempThreshold =
		pThermalParams->thermalLevels[3].minTempThreshold;
	wma->thermal_mgmt_info.thermalLevels[3].maxTempThreshold =
		pThermalParams->thermalLevels[3].maxTempThreshold;
	wma->thermal_mgmt_info.thermalCurrLevel = WLAN_WMA_THERMAL_LEVEL_0;

	WMA_LOGD("TM level min max:\n"
		 "0 %d   %d\n"
		 "1 %d   %d\n"
		 "2 %d   %d\n"
		 "3 %d   %d",
		 wma->thermal_mgmt_info.thermalLevels[0].minTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[0].maxTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[1].minTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[1].maxTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[2].minTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[2].maxTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[3].minTempThreshold,
		 wma->thermal_mgmt_info.thermalLevels[3].maxTempThreshold);

#ifdef FW_THERMAL_THROTTLE_SUPPORT
	for (i = 0; i < THROTTLE_LEVEL_MAX; i++)
		wma->thermal_mgmt_info.throttle_duty_cycle_tbl[i] =
				pThermalParams->throttle_duty_cycle_tbl[i];
#endif /* FW_THERMAL_THROTTLE_SUPPORT */

	if (wma->thermal_mgmt_info.thermalMgmtEnabled) {
		if (!wma->fw_therm_throt_support) {
			cdp_throttle_init_period(
				cds_get_context(QDF_MODULE_ID_SOC),
				curr_pdev, pThermalParams->throttlePeriod,
				&pThermalParams->throttle_duty_cycle_tbl[0]);
		} else {
			qdf_status = wma_update_thermal_mitigation_to_fw(
					wma, WLAN_WMA_THERMAL_LEVEL_0);
			if (QDF_STATUS_SUCCESS != qdf_status)
				return qdf_status;
		}
		qdf_status = wma_update_thermal_cfg_to_fw(wma);
	}
	return qdf_status;
}

/**
 * wma_set_thermal_level_ind() - send SME set thermal level indication message
 * @level:  thermal level
 *
 * Send SME SET_THERMAL_LEVEL_IND message
 *
 * Returns: none
 */
static void wma_set_thermal_level_ind(u_int8_t level)
{
	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;
	struct scheduler_msg sme_msg = {0};

	WMA_LOGI(FL("Thermal level: %d"), level);

	sme_msg.type = eWNI_SME_SET_THERMAL_LEVEL_IND;
	sme_msg.bodyptr = NULL;
	sme_msg.bodyval = level;

	qdf_status = scheduler_post_message(QDF_MODULE_ID_WMA,
					    QDF_MODULE_ID_SME,
					    QDF_MODULE_ID_SME, &sme_msg);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status))
		WMA_LOGE(FL(
			"Fail to post set thermal level ind msg"));
}

/**
 * wma_process_set_thermal_level() - sets thermal level
 * @wma: Pointer to WMA handle
 * @thermal_level : Thermal level
 *
 * This function sets the new thermal throttle level in the
 * txrx module and sends down the corresponding temperature
 * thresholds to the firmware
 *
 * Returns: QDF_STATUS_SUCCESS for success otherwise failure
 */
QDF_STATUS wma_process_set_thermal_level(tp_wma_handle wma,
					 uint8_t thermal_level)
{
	struct cdp_pdev *curr_pdev;

	if (!wma) {
		WMA_LOGE("TM Invalid input");
		return QDF_STATUS_E_FAILURE;
	}

	curr_pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!curr_pdev)
		return QDF_STATUS_E_FAILURE;

	WMA_LOGE("TM set level %d", thermal_level);

	/* Check if thermal mitigation is enabled */
	if (!wma->thermal_mgmt_info.thermalMgmtEnabled) {
		WMA_LOGE("Thermal mgmt is not enabled, ignoring set level command");
		return QDF_STATUS_E_FAILURE;
	}

	if (thermal_level >= WLAN_WMA_MAX_THERMAL_LEVELS) {
		WMA_LOGE("Invalid thermal level set %d", thermal_level);
		return QDF_STATUS_E_FAILURE;
	}

	if (thermal_level == wma->thermal_mgmt_info.thermalCurrLevel) {
		WMA_LOGD("Current level %d is same as the set level, ignoring",
			 wma->thermal_mgmt_info.thermalCurrLevel);
		return QDF_STATUS_SUCCESS;
	}

	wma->thermal_mgmt_info.thermalCurrLevel = thermal_level;

	cdp_throttle_set_level(cds_get_context(QDF_MODULE_ID_SOC),
			       curr_pdev, thermal_level);

	/* Send SME SET_THERMAL_LEVEL_IND message */
	wma_set_thermal_level_ind(thermal_level);

	return QDF_STATUS_SUCCESS;
}


/**
 * wma_set_thermal_mgmt() - set thermal mgmt command to fw
 * @wma_handle: Pointer to WMA handle
 * @thermal_info: Thermal command information
 *
 * This function sends the thermal management command
 * to the firmware
 *
 * Return: QDF_STATUS_SUCCESS for success otherwise failure
 */
QDF_STATUS wma_set_thermal_mgmt(tp_wma_handle wma_handle,
				t_thermal_cmd_params thermal_info)
{
	struct thermal_cmd_params mgmt_thermal_info = {0};

	if (!wma_handle) {
		wma_err("Invalid input");
		QDF_ASSERT(0);
		return QDF_STATUS_E_FAILURE;
	}

	mgmt_thermal_info.min_temp = thermal_info.minTemp;
	mgmt_thermal_info.max_temp = thermal_info.maxTemp;
	mgmt_thermal_info.thermal_enable = thermal_info.thermalEnable;

	return wmi_unified_set_thermal_mgmt_cmd(wma_handle->wmi_handle,
						&mgmt_thermal_info);
}

/**
 * wma_thermal_mgmt_get_level() - returns throttle level
 * @handle: Pointer to WMA handle
 * @temp: temperature
 *
 * This function returns the thermal(throttle) level
 * given the temperature
 *
 * Return: thermal (throttle) level
 */
static uint8_t wma_thermal_mgmt_get_level(void *handle, uint32_t temp)
{
	tp_wma_handle wma = (tp_wma_handle) handle;
	int i;
	uint8_t level;

	level = i = wma->thermal_mgmt_info.thermalCurrLevel;
	while (temp < wma->thermal_mgmt_info.thermalLevels[i].minTempThreshold
	       && i > 0) {
		i--;
		level = i;
	}

	i = wma->thermal_mgmt_info.thermalCurrLevel;
	while (temp > wma->thermal_mgmt_info.thermalLevels[i].maxTempThreshold
	       && i < (WLAN_WMA_MAX_THERMAL_LEVELS - 1)) {
		i++;
		level = i;
	}

	WMA_LOGW("Change thermal level from %d -> %d\n",
		 wma->thermal_mgmt_info.thermalCurrLevel, level);

	return level;
}

/**
 * wma_thermal_mgmt_evt_handler() - thermal mgmt event handler
 * @wma_handle: Pointer to WMA handle
 * @event: Thermal event information
 * @len: length of the event
 *
 * This function handles the thermal mgmt event from the firmware
 *
 * Return: 0 for success otherwise failure
 */
int wma_thermal_mgmt_evt_handler(void *handle, uint8_t *event, uint32_t len)
{
	tp_wma_handle wma;
	wmi_thermal_mgmt_event_fixed_param *tm_event;
	uint8_t thermal_level;
	t_thermal_cmd_params thermal_params;
	WMI_THERMAL_MGMT_EVENTID_param_tlvs *param_buf;
	struct cdp_pdev *curr_pdev;

	if (!event || !handle) {
		WMA_LOGE("Invalid thermal mitigation event buffer");
		return -EINVAL;
	}

	wma = (tp_wma_handle) handle;

	if (!wma) {
		WMA_LOGE("%s: Failed to get wma handle", __func__);
		return -EINVAL;
	}

	param_buf = (WMI_THERMAL_MGMT_EVENTID_param_tlvs *) event;

	curr_pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!curr_pdev)
		return -EINVAL;

	/* Check if thermal mitigation is enabled */
	if (!wma->thermal_mgmt_info.thermalMgmtEnabled) {
		WMA_LOGE("Thermal mgmt is not enabled, ignoring event");
		return -EINVAL;
	}

	tm_event = param_buf->fixed_param;
	WMA_LOGD("Thermal mgmt event received with temperature %d",
		 tm_event->temperature_degreeC);

	/* Get the thermal mitigation level for the reported temperature */
	thermal_level = wma_thermal_mgmt_get_level(handle,
					tm_event->temperature_degreeC);
	WMA_LOGD("Thermal mgmt level  %d", thermal_level);

	if (thermal_level == wma->thermal_mgmt_info.thermalCurrLevel) {
		WMA_LOGD("Current level %d is same as the set level, ignoring",
			 wma->thermal_mgmt_info.thermalCurrLevel);
		return 0;
	}

	wma->thermal_mgmt_info.thermalCurrLevel = thermal_level;

	if (!wma->fw_therm_throt_support) {
		/* Inform txrx */
		cdp_throttle_set_level(cds_get_context(QDF_MODULE_ID_SOC),
				       curr_pdev, thermal_level);
	}

	/* Send SME SET_THERMAL_LEVEL_IND message */
	wma_set_thermal_level_ind(thermal_level);

	if (wma->fw_therm_throt_support) {
		/* Send duty cycle info to firmware for fw to throttle */
		if (QDF_STATUS_SUCCESS !=
			wma_update_thermal_mitigation_to_fw(wma, thermal_level))
			return QDF_STATUS_E_FAILURE;
	}

	/* Get the temperature thresholds to set in firmware */
	thermal_params.minTemp =
		wma->thermal_mgmt_info.thermalLevels[thermal_level].
		minTempThreshold;
	thermal_params.maxTemp =
		wma->thermal_mgmt_info.thermalLevels[thermal_level].
		maxTempThreshold;
	thermal_params.thermalEnable =
		wma->thermal_mgmt_info.thermalMgmtEnabled;

	if (QDF_STATUS_SUCCESS != wma_set_thermal_mgmt(wma, thermal_params)) {
		WMA_LOGE("Could not send thermal mgmt command to the firmware!");
		return -EINVAL;
	}

	return 0;
}

/**
 * wma_ibss_peer_info_event_handler() - IBSS peer info event handler
 * @handle: wma handle
 * @data: event data
 * @len: length of data
 *
 * This function handles IBSS peer info event from FW.
 *
 * Return: 0 for success or error code
 */
int wma_ibss_peer_info_event_handler(void *handle, uint8_t *data,
					    uint32_t len)
{
	struct scheduler_msg cds_msg = {0};
	wmi_peer_info *peer_info;
	void *pdev;
	tSirIbssPeerInfoParams *pSmeRsp;
	uint32_t count, num_peers, status;
	tSirIbssGetPeerInfoRspParams *pRsp;
	WMI_PEER_INFO_EVENTID_param_tlvs *param_tlvs;
	wmi_peer_info_event_fixed_param *fix_param;
	uint8_t peer_mac[QDF_MAC_ADDR_SIZE];
	tp_wma_handle wma = (tp_wma_handle)handle;

	if (!wma) {
		WMA_LOGE("Invalid wma");
		return 0;
	}

	pdev = cds_get_context(QDF_MODULE_ID_TXRX);
	if (!pdev)
		return 0;

	param_tlvs = (WMI_PEER_INFO_EVENTID_param_tlvs *) data;
	fix_param = param_tlvs->fixed_param;
	peer_info = param_tlvs->peer_info;
	num_peers = fix_param->num_peers;
	status = 0;

	WMA_LOGE("%s: num_peers %d", __func__, num_peers);

	pRsp = qdf_mem_malloc(sizeof(tSirIbssGetPeerInfoRspParams));
	if (!pRsp)
		return 0;

	/*sanity check */
	if (!(num_peers) || (num_peers > 32) ||
	     (num_peers > param_tlvs->num_peer_info) ||
	     (!peer_info)) {
		WMA_LOGE("%s: Invalid event data from target num_peers %d peer_info %pK",
			__func__, num_peers, peer_info);
		status = 1;
		goto send_response;
	}

	/*
	 *For displaying only connected IBSS peer info, iterate till
	 *last but one entry only as last entry is used for IBSS creator
	 */
	for (count = 0; count < num_peers-1; count++) {
		pSmeRsp = &pRsp->ibssPeerInfoRspParams.peerInfoParams[count];

		WMI_MAC_ADDR_TO_CHAR_ARRAY(&peer_info->peer_mac_address,
					   peer_mac);
		qdf_mem_copy(pSmeRsp->mac_addr, peer_mac,
			sizeof(pSmeRsp->mac_addr));
		pSmeRsp->mcsIndex = 0;
		if (wmi_service_enabled(wma->wmi_handle,
					wmi_service_hw_db2dbm_support))
			pSmeRsp->rssi = peer_info->rssi;
		else
			pSmeRsp->rssi = peer_info->rssi +
						WMA_TGT_NOISE_FLOOR_DBM;

		pSmeRsp->txRate = peer_info->data_rate;

		wma_err("peer " QDF_MAC_ADDR_STR "rssi %d txRate %d",
			QDF_MAC_ADDR_ARRAY(peer_mac),
			pSmeRsp->rssi, pSmeRsp->txRate);

		peer_info++;
	}

send_response:
	/* message header */
	pRsp->mesgType = eWNI_SME_IBSS_PEER_INFO_RSP;
	pRsp->mesgLen = sizeof(tSirIbssGetPeerInfoRspParams);
	pRsp->ibssPeerInfoRspParams.status = status;
	pRsp->ibssPeerInfoRspParams.numPeers = num_peers;

	/* cds message wrapper */
	cds_msg.type = eWNI_SME_IBSS_PEER_INFO_RSP;
	cds_msg.bodyptr = (void *)pRsp;
	cds_msg.bodyval = 0;

	if (QDF_STATUS_SUCCESS !=
	    scheduler_post_message(QDF_MODULE_ID_WMA,
				   QDF_MODULE_ID_SME,
				   QDF_MODULE_ID_SME,  &cds_msg)) {
		WMA_LOGE("%s: could not post peer info rsp msg to SME",
			 __func__);
		/* free the mem and return */
		qdf_mem_free((void *)pRsp);
	}

	return 0;
}

/**
 * wma_fast_tx_fail_event_handler() -tx failure event handler
 * @handle: wma handle
 * @data: event data
 * @len: data length
 *
 * Handle fast tx failure indication event from FW
 *
 * Return: 0 for success or error code.
 */
int wma_fast_tx_fail_event_handler(void *handle, uint8_t *data,
					  uint32_t len)
{
	uint8_t tx_fail_cnt;
	uint8_t peer_mac[QDF_MAC_ADDR_SIZE];
	tp_wma_handle wma = (tp_wma_handle) handle;
	WMI_PEER_TX_FAIL_CNT_THR_EVENTID_param_tlvs *param_tlvs;
	wmi_peer_tx_fail_cnt_thr_event_fixed_param *fix_param;

	param_tlvs = (WMI_PEER_TX_FAIL_CNT_THR_EVENTID_param_tlvs *) data;
	fix_param = param_tlvs->fixed_param;

	WMI_MAC_ADDR_TO_CHAR_ARRAY(&fix_param->peer_mac_address, peer_mac);
	WMA_LOGE("%s: received fast tx failure event for peer 0x:%2x:0x%2x:0x%2x:0x%2x:0x%2x:0x%2x seq No %d",
		 __func__,
		 peer_mac[0], peer_mac[1], peer_mac[2], peer_mac[3],
		 peer_mac[4], peer_mac[5], fix_param->seq_no);

	tx_fail_cnt = fix_param->seq_no;

	/*call HDD callback */
	if (wma->hddTxFailCb)
		wma->hddTxFailCb(peer_mac, tx_fail_cnt);
	else
		WMA_LOGE("%s: HDD callback is %pK", __func__, wma->hddTxFailCb);

	return 0;
}

/**
 * wma_decap_to_8023() - Decapsulate to 802.3 format
 * @msdu: skb buffer
 * @info: decapsulate info
 *
 * Return: none
 */
static void wma_decap_to_8023(qdf_nbuf_t msdu, struct wma_decap_info_t *info)
{
	struct llc_snap_hdr_t *llc_hdr;
	uint16_t ether_type;
	uint16_t l2_hdr_space;
	struct ieee80211_qosframe_addr4 *wh;
	uint8_t local_buf[ETHERNET_HDR_LEN];
	uint8_t *buf;
	struct ethernet_hdr_t *ethr_hdr;

	buf = (uint8_t *) qdf_nbuf_data(msdu);
	llc_hdr = (struct llc_snap_hdr_t *)buf;
	ether_type = (llc_hdr->ethertype[0] << 8) | llc_hdr->ethertype[1];
	/* do llc remove if needed */
	l2_hdr_space = 0;
	if (IS_SNAP(llc_hdr)) {
		if (IS_BTEP(llc_hdr)) {
			/* remove llc */
			l2_hdr_space += sizeof(struct llc_snap_hdr_t);
			llc_hdr = NULL;
		} else if (IS_RFC1042(llc_hdr)) {
			if (!(ether_type == ETHERTYPE_AARP ||
			      ether_type == ETHERTYPE_IPX)) {
				/* remove llc */
				l2_hdr_space += sizeof(struct llc_snap_hdr_t);
				llc_hdr = NULL;
			}
		}
	}
	if (l2_hdr_space > ETHERNET_HDR_LEN)
		buf = qdf_nbuf_pull_head(msdu, l2_hdr_space - ETHERNET_HDR_LEN);
	else if (l2_hdr_space < ETHERNET_HDR_LEN)
		buf = qdf_nbuf_push_head(msdu, ETHERNET_HDR_LEN - l2_hdr_space);

	/* mpdu hdr should be present in info,re-create ethr_hdr based on
	 * mpdu hdr
	 */
	wh = (struct ieee80211_qosframe_addr4 *)info->hdr;
	ethr_hdr = (struct ethernet_hdr_t *)local_buf;
	switch (wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) {
	case IEEE80211_FC1_DIR_NODS:
		qdf_mem_copy(ethr_hdr->dest_addr, wh->i_addr1,
			     QDF_MAC_ADDR_SIZE);
		qdf_mem_copy(ethr_hdr->src_addr, wh->i_addr2,
			     QDF_MAC_ADDR_SIZE);
		break;
	case IEEE80211_FC1_DIR_TODS:
		qdf_mem_copy(ethr_hdr->dest_addr, wh->i_addr3,
			     QDF_MAC_ADDR_SIZE);
		qdf_mem_copy(ethr_hdr->src_addr, wh->i_addr2,
			     QDF_MAC_ADDR_SIZE);
		break;
	case IEEE80211_FC1_DIR_FROMDS:
		qdf_mem_copy(ethr_hdr->dest_addr, wh->i_addr1,
			     QDF_MAC_ADDR_SIZE);
		qdf_mem_copy(ethr_hdr->src_addr, wh->i_addr3,
			     QDF_MAC_ADDR_SIZE);
		break;
	case IEEE80211_FC1_DIR_DSTODS:
		qdf_mem_copy(ethr_hdr->dest_addr, wh->i_addr3,
			     QDF_MAC_ADDR_SIZE);
		qdf_mem_copy(ethr_hdr->src_addr, wh->i_addr4,
			     QDF_MAC_ADDR_SIZE);
		break;
	}

	if (!llc_hdr) {
		ethr_hdr->ethertype[0] = (ether_type >> 8) & 0xff;
		ethr_hdr->ethertype[1] = (ether_type) & 0xff;
	} else {
		uint32_t pktlen =
			qdf_nbuf_len(msdu) - sizeof(ethr_hdr->ethertype);
		ether_type = (uint16_t) pktlen;
		ether_type = qdf_nbuf_len(msdu) - sizeof(struct ethernet_hdr_t);
		ethr_hdr->ethertype[0] = (ether_type >> 8) & 0xff;
		ethr_hdr->ethertype[1] = (ether_type) & 0xff;
	}
	qdf_mem_copy(buf, ethr_hdr, ETHERNET_HDR_LEN);
}

/**
 * wma_ieee80211_hdrsize() - get 802.11 header size
 * @data: 80211 frame
 *
 * Return: size of header
 */
static int32_t wma_ieee80211_hdrsize(const void *data)
{
	const struct ieee80211_frame *wh = (const struct ieee80211_frame *)data;
	int32_t size = sizeof(struct ieee80211_frame);

	if ((wh->i_fc[1] & IEEE80211_FC1_DIR_MASK) == IEEE80211_FC1_DIR_DSTODS)
		size += QDF_MAC_ADDR_SIZE;
	if (IEEE80211_QOS_HAS_SEQ(wh))
		size += sizeof(uint16_t);
	return size;
}

/**
 * rate_pream: Mapping from data rates to preamble.
 */
static uint32_t rate_pream[] = {WMI_RATE_PREAMBLE_CCK, WMI_RATE_PREAMBLE_CCK,
				WMI_RATE_PREAMBLE_CCK, WMI_RATE_PREAMBLE_CCK,
				WMI_RATE_PREAMBLE_OFDM, WMI_RATE_PREAMBLE_OFDM,
				WMI_RATE_PREAMBLE_OFDM, WMI_RATE_PREAMBLE_OFDM,
				WMI_RATE_PREAMBLE_OFDM, WMI_RATE_PREAMBLE_OFDM,
				WMI_RATE_PREAMBLE_OFDM, WMI_RATE_PREAMBLE_OFDM};

/**
 * rate_mcs: Mapping from data rates to MCS (+4 for OFDM to keep the sequence).
 */
static uint32_t rate_mcs[] = {WMI_MAX_CCK_TX_RATE_1M, WMI_MAX_CCK_TX_RATE_2M,
			      WMI_MAX_CCK_TX_RATE_5_5M, WMI_MAX_CCK_TX_RATE_11M,
			      WMI_MAX_OFDM_TX_RATE_6M + 4,
			      WMI_MAX_OFDM_TX_RATE_9M + 4,
			      WMI_MAX_OFDM_TX_RATE_12M + 4,
			      WMI_MAX_OFDM_TX_RATE_18M + 4,
			      WMI_MAX_OFDM_TX_RATE_24M + 4,
			      WMI_MAX_OFDM_TX_RATE_36M + 4,
			      WMI_MAX_OFDM_TX_RATE_48M + 4,
			      WMI_MAX_OFDM_TX_RATE_54M + 4};

#define WMA_TX_SEND_MGMT_TYPE 0
#define WMA_TX_SEND_DATA_TYPE 1

/**
 * wma_update_tx_send_params() - Update tx_send_params TLV info
 * @tx_param: Pointer to tx_send_params
 * @rid: rate ID passed by PE
 *
 * Return: None
 */
static void wma_update_tx_send_params(struct tx_send_params *tx_param,
				      enum rateid rid)
{
	uint8_t  preamble = 0, nss = 0, rix = 0;

	preamble = rate_pream[rid];
	rix = rate_mcs[rid];

	tx_param->mcs_mask = (1 << rix);
	tx_param->nss_mask = (1 << nss);
	tx_param->preamble_type = (1 << preamble);
	tx_param->frame_type = WMA_TX_SEND_MGMT_TYPE;

	WMA_LOGD(FL("rate_id: %d, mcs: %0x, nss: %0x, preamble: %0x"),
		     rid, tx_param->mcs_mask, tx_param->nss_mask,
		     tx_param->preamble_type);
}

#ifdef CRYPTO_SET_KEY_CONVERGED
uint8_t *wma_get_igtk(struct wma_txrx_node *iface, uint16_t *key_len)
{
	struct wlan_crypto_key *crypto_key;

	crypto_key = wlan_crypto_get_key(iface->vdev, WMA_IGTK_KEY_INDEX_4);
	if (!crypto_key) {
		wma_err("IGTK not found");
		*key_len = 0;
		return NULL;
	}
	*key_len = crypto_key->keylen;

	return &crypto_key->keyval[0];
}
#else
uint8_t *wma_get_igtk(struct wma_txrx_node *iface, uint16_t *key_len)
{
	*key_len = iface->key.key_length;
	return iface->key.key;
}
#endif

QDF_STATUS wma_tx_packet(void *wma_context, void *tx_frame, uint16_t frmLen,
			 eFrameType frmType, eFrameTxDir txDir, uint8_t tid,
			 wma_tx_dwnld_comp_callback tx_frm_download_comp_cb,
			 void *pData,
			 wma_tx_ota_comp_callback tx_frm_ota_comp_cb,
			 uint8_t tx_flag, uint8_t vdev_id, bool tdlsFlag,
			 uint16_t channel_freq, enum rateid rid)
{
	tp_wma_handle wma_handle = (tp_wma_handle) (wma_context);
	int32_t status;
	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;
	int32_t is_high_latency;
	bool is_wmi_mgmt_tx = false;
	struct cdp_vdev *txrx_vdev;
	enum frame_index tx_frm_index = GENERIC_NODOWNLD_NOACK_COMP_INDEX;
	tpSirMacFrameCtl pFc = (tpSirMacFrameCtl) (qdf_nbuf_data(tx_frame));
	uint8_t use_6mbps = 0;
	uint8_t downld_comp_required = 0;
	uint16_t chanfreq;
#ifdef WLAN_FEATURE_11W
	uint8_t *pFrame = NULL;
	void *pPacket = NULL;
	uint16_t newFrmLen = 0;
#endif /* WLAN_FEATURE_11W */
	struct wma_txrx_node *iface;
	struct mac_context *mac;
	tpSirMacMgmtHdr mHdr;
	struct wmi_mgmt_params mgmt_param = {0};
	struct cdp_cfg *ctrl_pdev;
	void *soc = cds_get_context(QDF_MODULE_ID_SOC);
	struct ieee80211_frame *wh;
	struct wlan_objmgr_peer *peer = NULL;
	struct wlan_objmgr_psoc *psoc;
	void *mac_addr;
	bool is_5g = false;
	uint8_t pdev_id;
	uint8_t *igtk;
	uint16_t key_len;

	if (!wma_handle) {
		WMA_LOGE("wma_handle is NULL");
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}
	iface = &wma_handle->interfaces[vdev_id];
	/* Get the vdev handle from vdev id */
	txrx_vdev = wma_handle->interfaces[vdev_id].handle;

	if (!txrx_vdev) {
		WMA_LOGE("TxRx Vdev Handle is NULL");
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}

	if (!soc) {
		WMA_LOGE("%s:SOC context is NULL", __func__);
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}

	cdp_hl_tdls_flag_reset(soc, txrx_vdev, false);

	if (frmType >= TXRX_FRM_MAX) {
		WMA_LOGE("Invalid Frame Type Fail to send Frame");
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}

	mac = cds_get_context(QDF_MODULE_ID_PE);
	if (!mac) {
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}
	/*
	 * Currently only support to
	 * send 80211 Mgmt and 80211 Data are added.
	 */
	if (!((frmType == TXRX_FRM_802_11_MGMT) ||
	      (frmType == TXRX_FRM_802_11_DATA))) {
		WMA_LOGE("No Support to send other frames except 802.11 Mgmt/Data");
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}

#ifdef WLAN_FEATURE_11W
	if ((iface && (iface->rmfEnabled || tx_flag & HAL_USE_PMF)) &&
	    (frmType == TXRX_FRM_802_11_MGMT) &&
	    (pFc->subType == SIR_MAC_MGMT_DISASSOC ||
	     pFc->subType == SIR_MAC_MGMT_DEAUTH ||
	     pFc->subType == SIR_MAC_MGMT_ACTION)) {
		struct ieee80211_frame *wh =
			(struct ieee80211_frame *)qdf_nbuf_data(tx_frame);
		if (!QDF_IS_ADDR_BROADCAST(wh->i_addr1) &&
		    !IEEE80211_IS_MULTICAST(wh->i_addr1)) {
			if (pFc->wep) {
				uint8_t mic_len, hdr_len, pdev_id;

				/* Allocate extra bytes for privacy header and
				 * trailer
				 */
				if (iface->type == WMI_VDEV_TYPE_NDI &&
				    (tx_flag & HAL_USE_PMF)) {
					hdr_len = IEEE80211_CCMP_HEADERLEN;
					mic_len = IEEE80211_CCMP_MICLEN;
				} else {
					pdev_id = wlan_objmgr_pdev_get_pdev_id(
							wma_handle->pdev);
					qdf_status = mlme_get_peer_mic_len(
							wma_handle->psoc,
							pdev_id, wh->i_addr1,
							&mic_len, &hdr_len);

					if (QDF_IS_STATUS_ERROR(qdf_status)) {
						cds_packet_free((void *)
								tx_frame);
						return qdf_status;
					}
				}

				newFrmLen = frmLen + hdr_len + mic_len;
				qdf_status =
					cds_packet_alloc((uint16_t) newFrmLen,
							 (void **)&pFrame,
							 (void **)&pPacket);

				if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
					WMA_LOGP("%s: Failed to allocate %d bytes for RMF status code (%x)",
						__func__, newFrmLen,
						qdf_status);
					/* Free the original packet memory */
					cds_packet_free((void *)tx_frame);
					goto error;
				}

				/*
				 * Initialize the frame with 0's and only fill
				 * MAC header and data, Keep the CCMP header and
				 * trailer as 0's, firmware shall fill this
				 */
				qdf_mem_zero(pFrame, newFrmLen);
				qdf_mem_copy(pFrame, wh, sizeof(*wh));
				qdf_mem_copy(pFrame + sizeof(*wh) +
					     hdr_len,
					     pData + sizeof(*wh),
					     frmLen - sizeof(*wh));

				cds_packet_free((void *)tx_frame);
				tx_frame = pPacket;
				pData = pFrame;
				frmLen = newFrmLen;
				pFc = (tpSirMacFrameCtl)
						(qdf_nbuf_data(tx_frame));
			}
		} else {
			/* Allocate extra bytes for MMIE */
			newFrmLen = frmLen + IEEE80211_MMIE_LEN;
			qdf_status = cds_packet_alloc((uint16_t) newFrmLen,
						      (void **)&pFrame,
						      (void **)&pPacket);

			if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
				WMA_LOGP("%s: Failed to allocate %d bytes for RMF status code (%x)",
					__func__, newFrmLen,
					qdf_status);
				/* Free the original packet memory */
				cds_packet_free((void *)tx_frame);
				goto error;
			}
			/*
			 * Initialize the frame with 0's and only fill
			 * MAC header and data. MMIE field will be
			 * filled by cds_attach_mmie API
			 */
			qdf_mem_zero(pFrame, newFrmLen);
			qdf_mem_copy(pFrame, wh, sizeof(*wh));
			qdf_mem_copy(pFrame + sizeof(*wh),
				     pData + sizeof(*wh), frmLen - sizeof(*wh));
			igtk = wma_get_igtk(iface, &key_len);
			if (!igtk) {
				wma_alert("IGTK not present");
				cds_packet_free((void *)tx_frame);
				cds_packet_free((void *)pPacket);
				goto error;
			}
			if (!cds_attach_mmie(igtk,
					     iface->key.key_id[0].ipn,
					     WMA_IGTK_KEY_INDEX_4,
					     pFrame,
					     pFrame + newFrmLen, newFrmLen)) {
				wma_alert("Failed to attach MMIE");
				/* Free the original packet memory */
				cds_packet_free((void *)tx_frame);
				cds_packet_free((void *)pPacket);
				goto error;
			}
			cds_packet_free((void *)tx_frame);
			tx_frame = pPacket;
			pData = pFrame;
			frmLen = newFrmLen;
			pFc = (tpSirMacFrameCtl) (qdf_nbuf_data(tx_frame));
		}
		/*
		 * Some target which support sending mgmt frame based on htt
		 * would DMA write this PMF tx frame buffer, it may cause smmu
		 * check permission fault, set a flag to do bi-direction DMA
		 * map, normal tx unmap is enough for this case.
		 */
		QDF_NBUF_CB_TX_DMA_BI_MAP((qdf_nbuf_t)tx_frame) = 1;
	}
#endif /* WLAN_FEATURE_11W */
	mHdr = (tpSirMacMgmtHdr)qdf_nbuf_data(tx_frame);
	if ((frmType == TXRX_FRM_802_11_MGMT) &&
	    (pFc->subType == SIR_MAC_MGMT_PROBE_RSP)) {
		uint64_t adjusted_tsf_le;
		struct ieee80211_frame *wh =
			(struct ieee80211_frame *)qdf_nbuf_data(tx_frame);

		/* Make the TSF offset negative to match TSF in beacons */
		adjusted_tsf_le = cpu_to_le64(0ULL -
					      wma_handle->interfaces[vdev_id].
					      tsfadjust);
		A_MEMCPY(&wh[1], &adjusted_tsf_le, sizeof(adjusted_tsf_le));
	}
	if (frmType == TXRX_FRM_802_11_DATA) {
		qdf_nbuf_t ret;
		qdf_nbuf_t skb = (qdf_nbuf_t) tx_frame;
		void *pdev = cds_get_context(QDF_MODULE_ID_TXRX);

		struct wma_decap_info_t decap_info;
		struct ieee80211_frame *wh =
			(struct ieee80211_frame *)qdf_nbuf_data(skb);
		unsigned long curr_timestamp = qdf_mc_timer_get_system_ticks();

		if (!pdev) {
			cds_packet_free((void *)tx_frame);
			return QDF_STATUS_E_FAULT;
		}

		/*
		 * 1) TxRx Module expects data input to be 802.3 format
		 * So Decapsulation has to be done.
		 * 2) Only one Outstanding Data pending for Ack is allowed
		 */
		if (tx_frm_ota_comp_cb) {
			if (wma_handle->umac_data_ota_ack_cb) {
				/*
				 * If last data frame was sent more than 5 secs
				 * ago and still we didn't receive ack/nack from
				 * fw then allow Tx of this data frame
				 */
				if (curr_timestamp >=
				    wma_handle->last_umac_data_ota_timestamp +
				    500) {
					WMA_LOGE("%s: No Tx Ack for last data frame for more than 5 secs, allow Tx of current data frame",
						__func__);
				} else {
					WMA_LOGE("%s: Already one Data pending for Ack, reject Tx of data frame",
						__func__);
					cds_packet_free((void *)tx_frame);
					return QDF_STATUS_E_FAILURE;
				}
			}
		} else {
			/*
			 * Data Frames are sent through TxRx Non Standard Data
			 * path so Ack Complete Cb is must
			 */
			WMA_LOGE("No Ack Complete Cb. Don't Allow");
			cds_packet_free((void *)tx_frame);
			return QDF_STATUS_E_FAILURE;
		}

		/* Take out 802.11 header from skb */
		decap_info.hdr_len = wma_ieee80211_hdrsize(wh);
		qdf_mem_copy(decap_info.hdr, wh, decap_info.hdr_len);
		qdf_nbuf_pull_head(skb, decap_info.hdr_len);

		/*  Decapsulate to 802.3 format */
		wma_decap_to_8023(skb, &decap_info);

		/* Zero out skb's context buffer for the driver to use */
		qdf_mem_zero(skb->cb, sizeof(skb->cb));

		/* Terminate the (single-element) list of tx frames */
		skb->next = NULL;

		/* Store the Ack Complete Cb */
		wma_handle->umac_data_ota_ack_cb = tx_frm_ota_comp_cb;

		/* Store the timestamp and nbuf for this data Tx */
		wma_handle->last_umac_data_ota_timestamp = curr_timestamp;
		wma_handle->last_umac_data_nbuf = skb;

		/* Send the Data frame to TxRx in Non Standard Path */
		cdp_hl_tdls_flag_reset(soc,
			txrx_vdev, tdlsFlag);

		ret = cdp_tx_non_std(soc,
			txrx_vdev,
			OL_TX_SPEC_NO_FREE, skb);

		cdp_hl_tdls_flag_reset(soc,
			txrx_vdev, false);

		if (ret) {
			WMA_LOGE("TxRx Rejected. Fail to do Tx");
			/* Call Download Cb so that umac can free the buffer */
			if (tx_frm_download_comp_cb)
				tx_frm_download_comp_cb(wma_handle->mac_context,
						tx_frame,
						WMA_TX_FRAME_BUFFER_FREE);
			wma_handle->umac_data_ota_ack_cb = NULL;
			wma_handle->last_umac_data_nbuf = NULL;
			return QDF_STATUS_E_FAILURE;
		}

		/* Call Download Callback if passed */
		if (tx_frm_download_comp_cb)
			tx_frm_download_comp_cb(wma_handle->mac_context,
						tx_frame,
						WMA_TX_FRAME_BUFFER_NO_FREE);

		return QDF_STATUS_SUCCESS;
	}

	ctrl_pdev = cdp_get_ctrl_pdev_from_vdev(soc,
				txrx_vdev);
	if (!ctrl_pdev) {
		WMA_LOGE("ol_pdev_handle is NULL\n");
		cds_packet_free((void *)tx_frame);
		return QDF_STATUS_E_FAILURE;
	}
	is_high_latency = cdp_cfg_is_high_latency(soc, ctrl_pdev);
	is_wmi_mgmt_tx = wmi_service_enabled(wma_handle->wmi_handle,
					     wmi_service_mgmt_tx_wmi);

	downld_comp_required = tx_frm_download_comp_cb && is_high_latency &&
				(!is_wmi_mgmt_tx) && tx_frm_ota_comp_cb;

	/* Fill the frame index to send */
	if (pFc->type == SIR_MAC_MGMT_FRAME) {
		if (tx_frm_ota_comp_cb) {
			if (downld_comp_required)
				tx_frm_index =
					GENERIC_DOWNLD_COMP_ACK_COMP_INDEX;
			else
				tx_frm_index = GENERIC_NODOWLOAD_ACK_COMP_INDEX;

		} else {
			tx_frm_index =
				GENERIC_NODOWNLD_NOACK_COMP_INDEX;
		}
	}

	/*
	 * If Dowload Complete is required
	 * Wait for download complete
	 */
	if (downld_comp_required) {
		/* Store Tx Comp Cb */
		wma_handle->tx_frm_download_comp_cb = tx_frm_download_comp_cb;

		/* Reset the Tx Frame Complete Event */
		qdf_status = qdf_event_reset(
				&wma_handle->tx_frm_download_comp_event);

		if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
			WMA_LOGP("%s: Event Reset failed tx comp event %x",
				 __func__, qdf_status);
			cds_packet_free((void *)tx_frame);
			goto error;
		}
	}

	/* If the frame has to be sent at BD Rate2 inform TxRx */
	if (tx_flag & HAL_USE_BD_RATE2_FOR_MANAGEMENT_FRAME)
		use_6mbps = 1;

	if (pFc->subType == SIR_MAC_MGMT_PROBE_RSP) {
		if ((wma_is_vdev_in_ap_mode(wma_handle, vdev_id)) &&
		    (0 != wma_handle->interfaces[vdev_id].mhz))
			chanfreq = wma_handle->interfaces[vdev_id].mhz;
		else
			chanfreq = channel_freq;
		WMA_LOGD("%s: Probe response frame on channel %d vdev:%d",
			__func__, chanfreq, vdev_id);
		if (wma_is_vdev_in_ap_mode(wma_handle, vdev_id) && !chanfreq)
			WMA_LOGE("%s: AP oper chan is zero", __func__);
	} else if (pFc->subType == SIR_MAC_MGMT_ACTION ||
			pFc->subType == SIR_MAC_MGMT_AUTH) {
		chanfreq = channel_freq;
	} else {
		chanfreq = 0;
	}

	if (mac->mlme_cfg->gen.debug_packet_log & 0x1) {
		if ((pFc->type == SIR_MAC_MGMT_FRAME) &&
		    (pFc->subType != SIR_MAC_MGMT_PROBE_REQ) &&
		    (pFc->subType != SIR_MAC_MGMT_PROBE_RSP)) {
			WMA_LOGD("TX MGMT - Type %hu, SubType %hu seq_num[%d]",
				 pFc->type, pFc->subType,
				 ((mHdr->seqControl.seqNumHi << 4) |
				 mHdr->seqControl.seqNumLo));
		}
	}

	if (wma_handle->interfaces[vdev_id].channel >= SIR_11A_CHANNEL_BEGIN)
		is_5g = true;

	mgmt_param.tx_frame = tx_frame;
	mgmt_param.frm_len = frmLen;
	mgmt_param.vdev_id = vdev_id;
	mgmt_param.pdata = pData;
	mgmt_param.chanfreq = chanfreq;
	mgmt_param.qdf_ctx = cds_get_context(QDF_MODULE_ID_QDF_DEVICE);
	mgmt_param.use_6mbps = use_6mbps;
	mgmt_param.tx_type = tx_frm_index;

	/*
	 * Update the tx_params TLV only for rates
	 * other than 1Mbps and 6 Mbps
	 */
	if (rid < RATEID_DEFAULT &&
	    (rid != RATEID_1MBPS && !(rid == RATEID_6MBPS && is_5g))) {
		WMA_LOGD(FL("using rate id: %d for Tx"), rid);
		mgmt_param.tx_params_valid = true;
		wma_update_tx_send_params(&mgmt_param.tx_param, rid);
	}

	psoc = wma_handle->psoc;
	if (!psoc) {
		WMA_LOGE("%s: psoc ctx is NULL", __func__);
		cds_packet_free((void *)tx_frame);
		goto error;
	}

	if (!wma_handle->pdev) {
		WMA_LOGE("%s: pdev ctx is NULL", __func__);
		cds_packet_free((void *)tx_frame);
		goto error;
	}

	pdev_id = wlan_objmgr_pdev_get_pdev_id(wma_handle->pdev);
	wh = (struct ieee80211_frame *)(qdf_nbuf_data(tx_frame));
	mac_addr = wh->i_addr1;
	peer = wlan_objmgr_get_peer(psoc, pdev_id, mac_addr, WLAN_MGMT_NB_ID);
	if (!peer) {
		mac_addr = wh->i_addr2;
		peer = wlan_objmgr_get_peer(psoc, pdev_id, mac_addr,
					WLAN_MGMT_NB_ID);
	}

	status = wlan_mgmt_txrx_mgmt_frame_tx(peer, wma_handle->mac_context,
					      (qdf_nbuf_t)tx_frame, NULL,
					      tx_frm_ota_comp_cb,
					      WLAN_UMAC_COMP_MLME,
					      &mgmt_param);

	wlan_objmgr_peer_release_ref(peer, WLAN_MGMT_NB_ID);
	if (status != QDF_STATUS_SUCCESS) {
		WMA_LOGE("%s: mgmt tx failed", __func__);
		qdf_nbuf_free((qdf_nbuf_t)tx_frame);
		goto error;
	}

	/*
	 * Failed to send Tx Mgmt Frame
	 */
	if (status) {
	/* Call Download Cb so that umac can free the buffer */
		uint32_t rem;

		if (tx_frm_download_comp_cb)
			tx_frm_download_comp_cb(wma_handle->mac_context,
						tx_frame,
						WMA_TX_FRAME_BUFFER_FREE);
		rem = qdf_do_div_rem(wma_handle->tx_fail_cnt,
				     MAX_PRINT_FAILURE_CNT);
		if (!rem)
			WMA_LOGE("%s: Failed to send Mgmt Frame", __func__);
		else
			WMA_LOGD("%s: Failed to send Mgmt Frame", __func__);
		wma_handle->tx_fail_cnt++;
		goto error;
	}

	if (!tx_frm_download_comp_cb)
		return QDF_STATUS_SUCCESS;

	/*
	 * Wait for Download Complete
	 * if required
	 */
	if (downld_comp_required) {
		/*
		 * Wait for Download Complete
		 * @ Integrated : Dxe Complete
		 * @ Discrete : Target Download Complete
		 */
		qdf_status =
			qdf_wait_for_event_completion(&wma_handle->
					      tx_frm_download_comp_event,
					      WMA_TX_FRAME_COMPLETE_TIMEOUT);

		if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
			WMA_LOGP("Wait Event failed txfrm_comp_event");
			/*
			 * @Integrated: Something Wrong with Dxe
			 *   TODO: Some Debug Code
			 * Here We need to trigger SSR since
			 * since system went into a bad state where
			 * we didn't get Download Complete for almost
			 * WMA_TX_FRAME_COMPLETE_TIMEOUT (1 sec)
			 */
			/* display scheduler stats */
			return cdp_display_stats(soc, CDP_SCHEDULER_STATS,
						QDF_STATS_VERBOSITY_LEVEL_HIGH);
		}
	}

	return QDF_STATUS_SUCCESS;

error:
	wma_handle->tx_frm_download_comp_cb = NULL;
	wma_handle->umac_data_ota_ack_cb = NULL;
	return QDF_STATUS_E_FAILURE;
}

/**
 * wma_ds_peek_rx_packet_info() - peek rx packet info
 * @pkt: packet
 * @pkt_meta: packet meta
 * @bSwap: byte swap
 *
 * Function fills the rx packet meta info from the the cds packet
 *
 * Return: QDF status
 */
QDF_STATUS wma_ds_peek_rx_packet_info(cds_pkt_t *pkt, void **pkt_meta,
				      bool bSwap)
{
	/* Sanity Check */
	if (!pkt) {
		WMA_LOGE("wma:Invalid parameter sent on wma_peek_rx_pkt_info");
		return QDF_STATUS_E_FAULT;
	}

	*pkt_meta = &(pkt->pkt_meta);

	return QDF_STATUS_SUCCESS;
}

#ifdef HL_RX_AGGREGATION_HOLE_DETECTION
void ol_rx_aggregation_hole(uint32_t hole_info)
{
	struct sir_sme_rx_aggr_hole_ind *rx_aggr_hole_event;
	uint32_t alloc_len;
	cds_msg_t cds_msg = { 0 };
	QDF_STATUS status;

	alloc_len = sizeof(*rx_aggr_hole_event) +
		sizeof(rx_aggr_hole_event->hole_info_array[0]);
	rx_aggr_hole_event = qdf_mem_malloc(alloc_len);
	if (!rx_aggr_hole_event)
		return;

	rx_aggr_hole_event->hole_cnt = 1;
	rx_aggr_hole_event->hole_info_array[0] = hole_info;

	cds_msg.type = eWNI_SME_RX_AGGR_HOLE_IND;
	cds_msg.bodyptr = rx_aggr_hole_event;
	cds_msg.bodyval = 0;

	status = cds_mq_post_message(CDS_MQ_ID_SME, &cds_msg);
	if (status != QDF_STATUS_SUCCESS) {
		qdf_mem_free(rx_aggr_hole_event);
		return;
	}
}
#endif

/**
 * ol_rx_err() - ol rx err handler
 * @pdev: ol pdev
 * @vdev_id: vdev id
 * @peer_mac_addr: peer mac address
 * @tid: TID
 * @tsf32: TSF
 * @err_type: error type
 * @rx_frame: rx frame
 * @pn: PN Number
 * @key_id: key id
 *
 * This function handles rx error and send MIC error failure to LIM
 *
 * Return: none
 */
/*
 * Local prototype added to temporarily address warning caused by
 * -Wmissing-prototypes. A more correct solution will come later
 * as a solution to IR-196435 at whihc point this prototype will
 * be removed.
 */
void ol_rx_err(void *pdev, uint8_t vdev_id,
	       uint8_t *peer_mac_addr, int tid, uint32_t tsf32,
	       enum ol_rx_err_type err_type, qdf_nbuf_t rx_frame,
	       uint64_t *pn, uint8_t key_id);
void ol_rx_err(void *pdev, uint8_t vdev_id,
	       uint8_t *peer_mac_addr, int tid, uint32_t tsf32,
	       enum ol_rx_err_type err_type, qdf_nbuf_t rx_frame,
	       uint64_t *pn, uint8_t key_id)
{
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);
	struct mic_failure_ind *mic_err_ind;
	qdf_ether_header_t *eth_hdr;
	struct scheduler_msg cds_msg = {0};

	if (!wma)
		return;

	if (err_type != OL_RX_ERR_TKIP_MIC)
		return;

	if (qdf_nbuf_len(rx_frame) < sizeof(*eth_hdr))
		return;
	eth_hdr = (qdf_ether_header_t *)qdf_nbuf_data(rx_frame);
	mic_err_ind = qdf_mem_malloc(sizeof(*mic_err_ind));
	if (!mic_err_ind)
		return;

	mic_err_ind->messageType = eWNI_SME_MIC_FAILURE_IND;
	mic_err_ind->length = sizeof(*mic_err_ind);
	mic_err_ind->sessionId = vdev_id;
	qdf_copy_macaddr(&mic_err_ind->bssId,
		     (struct qdf_mac_addr *) &wma->interfaces[vdev_id].bssid);
	qdf_mem_copy(mic_err_ind->info.taMacAddr,
		     (struct qdf_mac_addr *) peer_mac_addr,
			sizeof(tSirMacAddr));
	qdf_mem_copy(mic_err_ind->info.srcMacAddr,
		     (struct qdf_mac_addr *) eth_hdr->ether_shost,
			sizeof(tSirMacAddr));
	qdf_mem_copy(mic_err_ind->info.dstMacAddr,
		     (struct qdf_mac_addr *) eth_hdr->ether_dhost,
			sizeof(tSirMacAddr));
	mic_err_ind->info.keyId = key_id;
	mic_err_ind->info.multicast =
		IEEE80211_IS_MULTICAST(eth_hdr->ether_dhost);
	qdf_mem_copy(mic_err_ind->info.TSC, pn, SIR_CIPHER_SEQ_CTR_SIZE);

	qdf_mem_zero(&cds_msg, sizeof(struct scheduler_msg));
	cds_msg.type = eWNI_SME_MIC_FAILURE_IND;
	cds_msg.bodyptr = (void *) mic_err_ind;

	if (QDF_STATUS_SUCCESS !=
		scheduler_post_message(QDF_MODULE_ID_TXRX,
				       QDF_MODULE_ID_SME,
				       QDF_MODULE_ID_SME,
				       &cds_msg)) {
		WMA_LOGE("%s: could not post mic failure indication to SME",
			 __func__);
		qdf_mem_free((void *)mic_err_ind);
	}
}

/**
 * wma_tx_abort() - abort tx
 * @vdev_id: vdev id
 *
 * In case of deauth host abort transmitting packet.
 *
 * Return: none
 */
void wma_tx_abort(uint8_t vdev_id)
{
#define PEER_ALL_TID_BITMASK 0xffffffff
	tp_wma_handle wma;
	uint32_t peer_tid_bitmap = PEER_ALL_TID_BITMASK;
	struct wma_txrx_node *iface;
	struct peer_flush_params param = {0};

	wma = cds_get_context(QDF_MODULE_ID_WMA);
	if (!wma)
		return;

	iface = &wma->interfaces[vdev_id];
	if (!iface->handle) {
		WMA_LOGE("%s: Failed to get iface handle: %pK",
			 __func__, iface->handle);
		return;
	}
	WMA_LOGD("%s: vdevid %d bssid %pM", __func__, vdev_id, iface->bssid);
	wma_vdev_set_pause_bit(vdev_id, PAUSE_TYPE_HOST);
	cdp_fc_vdev_pause(cds_get_context(QDF_MODULE_ID_SOC),
			iface->handle,
			OL_TXQ_PAUSE_REASON_TX_ABORT);

	/* Flush all TIDs except MGMT TID for this peer in Target */
	peer_tid_bitmap &= ~(0x1 << WMI_MGMT_TID);
	param.peer_tid_bitmap = peer_tid_bitmap;
	param.vdev_id = vdev_id;
	wmi_unified_peer_flush_tids_send(wma->wmi_handle, iface->bssid,
					 &param);
}

/**
 * wma_lro_config_cmd() - process the LRO config command
 * @wma: Pointer to WMA handle
 * @wma_lro_cmd: Pointer to LRO configuration parameters
 *
 * This function sends down the LRO configuration parameters to
 * the firmware to enable LRO, sets the TCP flags and sets the
 * seed values for the toeplitz hash generation
 *
 * Return: QDF_STATUS_SUCCESS for success otherwise failure
 */
QDF_STATUS wma_lro_config_cmd(void *handle,
	 struct cdp_lro_hash_config *wma_lro_cmd)
{
	struct wmi_lro_config_cmd_t wmi_lro_cmd = {0};
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);

	if (!wma || !wma_lro_cmd) {
		wma_err("Invalid input!");
		return QDF_STATUS_E_FAILURE;
	}

	wmi_lro_cmd.lro_enable = wma_lro_cmd->lro_enable;
	wmi_lro_cmd.tcp_flag = wma_lro_cmd->tcp_flag;
	wmi_lro_cmd.tcp_flag_mask = wma_lro_cmd->tcp_flag_mask;
	qdf_mem_copy(wmi_lro_cmd.toeplitz_hash_ipv4,
			wma_lro_cmd->toeplitz_hash_ipv4,
			LRO_IPV4_SEED_ARR_SZ * sizeof(uint32_t));
	qdf_mem_copy(wmi_lro_cmd.toeplitz_hash_ipv6,
			wma_lro_cmd->toeplitz_hash_ipv6,
			LRO_IPV6_SEED_ARR_SZ * sizeof(uint32_t));

	return wmi_unified_lro_config_cmd(wma->wmi_handle,
						&wmi_lro_cmd);
}

/**
 * wma_indicate_err() - indicate an error to the protocol stack
 * @err_type: error type
 * @err_info: information associated with the error
 *
 * This function indicates an error encountered in the data path
 * to the protocol stack
 *
 * Return: none
 */
void
wma_indicate_err(
	enum ol_rx_err_type err_type,
	struct ol_error_info *err_info)
{
	switch (err_type) {
	case OL_RX_ERR_TKIP_MIC:
	{
		tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);
		struct mic_failure_ind *mic_err_ind;
		struct scheduler_msg cds_msg = {0};
		uint8_t vdev_id;

		if (!wma) {
			WMA_LOGE("%s: Failed to get wma context",
				 __func__);
			return;
		}

		mic_err_ind = qdf_mem_malloc(sizeof(*mic_err_ind));
		if (!mic_err_ind)
			return;

		qdf_mem_zero((void *)mic_err_ind,
			 sizeof(*mic_err_ind));
		mic_err_ind->messageType = eWNI_SME_MIC_FAILURE_IND;
		mic_err_ind->length = sizeof(*mic_err_ind);
		vdev_id = err_info->u.mic_err.vdev_id;
		qdf_copy_macaddr(&mic_err_ind->bssId,
		     (struct qdf_mac_addr *) &wma->interfaces[vdev_id].bssid);
		wma_err("MIC error: BSSID:"QDF_MAC_ADDR_STR,
			QDF_MAC_ADDR_ARRAY(mic_err_ind->bssId.bytes));
		qdf_mem_copy(mic_err_ind->info.taMacAddr,
			 (struct qdf_mac_addr *) err_info->u.mic_err.ta,
			 sizeof(tSirMacAddr));
		qdf_mem_copy(mic_err_ind->info.srcMacAddr,
			 (struct qdf_mac_addr *) err_info->u.mic_err.sa,
			 sizeof(tSirMacAddr));
		qdf_mem_copy(mic_err_ind->info.dstMacAddr,
			(struct qdf_mac_addr *) err_info->u.mic_err.da,
			 sizeof(tSirMacAddr));
		mic_err_ind->info.keyId = err_info->u.mic_err.key_id;
		mic_err_ind->info.multicast =
			 IEEE80211_IS_MULTICAST(err_info->u.mic_err.da);
		qdf_mem_copy(mic_err_ind->info.TSC,
			 (void *)&err_info->
			 u.mic_err.pn, SIR_CIPHER_SEQ_CTR_SIZE);

		qdf_mem_zero(&cds_msg, sizeof(struct scheduler_msg));
		cds_msg.type = eWNI_SME_MIC_FAILURE_IND;
		cds_msg.bodyptr = (void *) mic_err_ind;
		if (QDF_STATUS_SUCCESS !=
			scheduler_post_message(QDF_MODULE_ID_WMA,
					       QDF_MODULE_ID_SME,
					       QDF_MODULE_ID_SME,
				  &cds_msg)) {
			WMA_LOGE("%s: mic failure ind post to SME failed",
					 __func__);
			qdf_mem_free((void *)mic_err_ind);
		}
		break;
	}
	default:
	{
		WMA_LOGE("%s: unhandled ol error type %d", __func__, err_type);
		break;
	}
	}
}

void wma_rx_mic_error_ind(void *scn_handle, uint16_t vdev_id, void *wh)
{
	struct ieee80211_frame *w = (struct ieee80211_frame *)wh;
	struct ol_error_info err_info;

	err_info.u.mic_err.vdev_id = vdev_id;
	qdf_mem_copy(err_info.u.mic_err.da, w->i_addr1, QDF_MAC_ADDR_SIZE);
	qdf_mem_copy(err_info.u.mic_err.ta, w->i_addr2, QDF_MAC_ADDR_SIZE);

	WMA_LOGD("MIC vdev_id %d\n", vdev_id);
	wma_debug("MIC DA: "QDF_MAC_ADDR_STR,
		  QDF_MAC_ADDR_ARRAY(err_info.u.mic_err.da));
	wma_debug("MIC TA: "QDF_MAC_ADDR_STR,
		  QDF_MAC_ADDR_ARRAY(err_info.u.mic_err.ta));

	wma_indicate_err(OL_RX_ERR_TKIP_MIC, &err_info);
}

void wma_delete_invalid_peer_entries(uint8_t vdev_id, uint8_t *peer_mac_addr)
{
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);
	uint8_t i;
	struct wma_txrx_node *iface;

	if (!wma) {
		wma_err("wma handle is NULL");
		return;
	}

	iface = &wma->interfaces[vdev_id];

	if (peer_mac_addr) {
		for (i = 0; i < INVALID_PEER_MAX_NUM; i++) {
			if (qdf_mem_cmp
				      (iface->invalid_peers[i].rx_macaddr,
				      peer_mac_addr,
				      QDF_MAC_ADDR_SIZE) == 0) {
				qdf_mem_zero(iface->invalid_peers[i].rx_macaddr,
					     sizeof(QDF_MAC_ADDR_SIZE));
				break;
			}
		}
		if (i == INVALID_PEER_MAX_NUM)
			wma_debug("peer_mac_addr %pM is not found", peer_mac_addr);
	} else {
		qdf_mem_zero(iface->invalid_peers,
			     sizeof(iface->invalid_peers));
	}
}

uint8_t wma_rx_invalid_peer_ind(uint8_t vdev_id, void *wh)
{
	struct ol_rx_inv_peer_params *rx_inv_msg;
	struct ieee80211_frame *wh_l = (struct ieee80211_frame *)wh;
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);
	uint8_t i, index;
	bool invalid_peer_found = false;
	struct wma_txrx_node *iface;

	if (!wma) {
		wma_err("wma handle is NULL");
		return -EINVAL;
	}

	iface = &wma->interfaces[vdev_id];
	rx_inv_msg = qdf_mem_malloc(sizeof(struct ol_rx_inv_peer_params));
	if (!rx_inv_msg)
		return -ENOMEM;

	index = iface->invalid_peer_idx;
	rx_inv_msg->vdev_id = vdev_id;
	qdf_mem_copy(rx_inv_msg->ra, wh_l->i_addr1, QDF_MAC_ADDR_SIZE);
	qdf_mem_copy(rx_inv_msg->ta, wh_l->i_addr2, QDF_MAC_ADDR_SIZE);


	for (i = 0; i < INVALID_PEER_MAX_NUM; i++) {
		if (qdf_mem_cmp
			      (iface->invalid_peers[i].rx_macaddr,
			      rx_inv_msg->ta,
			      QDF_MAC_ADDR_SIZE) == 0) {
			invalid_peer_found = true;
			break;
		}
	}

	if (!invalid_peer_found) {
		qdf_mem_copy(iface->invalid_peers[index].rx_macaddr,
			     rx_inv_msg->ta,
			    QDF_MAC_ADDR_SIZE);

		/* reset count if reached max */
		iface->invalid_peer_idx =
			(index + 1) % INVALID_PEER_MAX_NUM;

		/* send deauth */
		WMA_LOGD("%s: vdev_id %d", __func__, vdev_id);
		wma_debug(" RA: " QDF_MAC_ADDR_STR,
			  QDF_MAC_ADDR_ARRAY(rx_inv_msg->ra));
		wma_debug(" TA: " QDF_MAC_ADDR_STR,
			  QDF_MAC_ADDR_ARRAY(rx_inv_msg->ta));

		wma_send_msg(wma,
			     SIR_LIM_RX_INVALID_PEER,
			     (void *)rx_inv_msg, 0);
	} else {
		wma_debug_rl("Ignore invalid peer indication as received more than once "
			QDF_MAC_ADDR_STR,
			QDF_MAC_ADDR_ARRAY(rx_inv_msg->ta));
		qdf_mem_free(rx_inv_msg);
	}

	return 0;
}

int wma_dp_send_delba_ind(uint8_t vdev_id, uint8_t *peer_macaddr,
			  uint8_t tid, uint8_t reason_code)
{
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);
	struct lim_delba_req_info *req;

	if (!wma || !peer_macaddr) {
		wma_err("wma handle or mac addr is NULL");
		return -EINVAL;
	}
	req = qdf_mem_malloc(sizeof(*req));
	if (!req)
		return -ENOMEM;
	req->vdev_id = vdev_id;
	qdf_mem_copy(req->peer_macaddr, peer_macaddr, QDF_MAC_ADDR_SIZE);
	req->tid = tid;
	req->reason_code = reason_code;
	WMA_LOGD("req delba_ind vdev %d %pM tid %d reason %d",
		 vdev_id, peer_macaddr, tid, reason_code);
	wma_send_msg_high_priority(wma, SIR_HAL_REQ_SEND_DELBA_REQ_IND,
				   (void *)req, 0);

	return 0;
}

bool wma_is_roam_in_progress(uint32_t vdev_id)
{
	tp_wma_handle wma = cds_get_context(QDF_MODULE_ID_WMA);

	if (!wma)
		return false;

	return wma->interfaces[vdev_id].roaming_in_progress;
}
