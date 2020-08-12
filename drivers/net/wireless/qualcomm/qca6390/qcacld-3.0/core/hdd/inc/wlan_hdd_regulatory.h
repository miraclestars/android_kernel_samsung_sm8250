/*
 * Copyright (c) 2016-2018 The Linux Foundation. All rights reserved.
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

#if !defined __HDD_REGULATORY_H
#define __HDD_REGULATORY_H

/**
 * DOC: wlan_hdd_regulatory.h
 *
 * HDD Regulatory prototype implementation
 */

struct hdd_context;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 14, 0)) || defined(WITH_BACKPORTS)
#define IEEE80211_CHAN_PASSIVE_SCAN IEEE80211_CHAN_NO_IR
#define IEEE80211_CHAN_NO_IBSS IEEE80211_CHAN_NO_IR
#endif

int hdd_regulatory_init(struct hdd_context *hdd_ctx, struct wiphy *wiphy);
void hdd_program_country_code(struct hdd_context *hdd_ctx);
void hdd_reset_global_reg_params(void);

/**
 * hdd_send_wiphy_regd_sync_event() - sends the regulatory sync event
 * @hdd_ctx: HDD context
 *
 * Return: None
 */
void hdd_send_wiphy_regd_sync_event(struct hdd_context *hdd_ctx);

/**
 * hdd_reg_set_country() - helper function for setting the regulatory country
 * @hdd_ctx: the HDD context to set the country for
 * @country_code: the two character country code to configure
 *
 * Return: zero for success, non-zero error code for failure
 */
int hdd_reg_set_country(struct hdd_context *hdd_ctx, char *country_code);

/**
 * hdd_reg_set_band() - helper function for setting the regulatory band
 * @hdd_ctx: the HDD context to set the band for
 * @ui_band: the UI band to configure
 *
 * Return: zero for success, non-zero error code for failure
 */
int hdd_reg_set_band(struct net_device *dev, u8 ui_band);

/**
 * hdd_update_indoor_channel() - enable/disable indoor channel
 * @hdd_ctx: hdd context
 * @disable: whether to enable / disable indoor channel
 *
 * enable/disable indoor channel in wiphy/cds
 *
 * Return: void
 */
void hdd_update_indoor_channel(struct  hdd_context *hdd_ctx,
					bool disable);
/**
 * hdd_modify_indoor_channel_state_flags() - modify wiphy flags and cds state
 * @wiphy_chan: wiphy channel number
 * @cds_chan: cds channel structure
 * @chan_enum: channel enum maintain in reg db
 * @chan_num: channel index
 * @disable: Disable/enable the flags
 *
 * Modify wiphy flags and cds state if channel is indoor.
 *
 * Return: void
 */
void hdd_modify_indoor_channel_state_flags(
	struct hdd_context *hdd_ctx,
	struct ieee80211_channel *wiphy_chan,
	struct regulatory_channel *cds_chan,
	enum channel_enum chan_enum, int chan_num, bool disable);

#endif
