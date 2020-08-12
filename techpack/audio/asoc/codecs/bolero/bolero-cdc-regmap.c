// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2018-2019, The Linux Foundation. All rights reserved.
 */

#include <linux/regmap.h>
#include "bolero-cdc.h"
#include "internal.h"

static const struct reg_default bolero_defaults[] = {
	/* TX Macro */
	{ BOLERO_CDC_TX_CLK_RST_CTRL_MCLK_CONTROL, 0x00 },
	{ BOLERO_CDC_TX_CLK_RST_CTRL_FS_CNT_CONTROL, 0x00 },
	{ BOLERO_CDC_TX_CLK_RST_CTRL_SWR_CONTROL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_TOP_CFG0, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_ANC_CFG, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_CTRL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_FREQ_MCLK, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_DEBUG_BUS, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_DEBUG_EN, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_TX_I2S_CTL, 0x0C},
	{ BOLERO_CDC_TX_TOP_CSR_I2S_CLK, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_I2S_RESET, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_DMIC0_CTL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_DMIC1_CTL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_DMIC2_CTL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_DMIC3_CTL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_AMIC0_CTL, 0x00},
	{ BOLERO_CDC_TX_TOP_CSR_SWR_AMIC1_CTL, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX0_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX0_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX1_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX1_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX2_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX2_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX3_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX3_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX4_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX4_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX5_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX5_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX6_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX6_CFG1, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX7_CFG0, 0x00},
	{ BOLERO_CDC_TX_INP_MUX_ADC_MUX7_CFG1, 0x00},
	{ BOLERO_CDC_TX_ANC0_CLK_RESET_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_MODE_1_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_MODE_2_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_FF_SHIFT, 0x00},
	{ BOLERO_CDC_TX_ANC0_FB_SHIFT, 0x00},
	{ BOLERO_CDC_TX_ANC0_LPF_FF_A_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_LPF_FF_B_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_LPF_FB_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_SMLPF_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_DCFLT_SHIFT_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_IIR_ADAPT_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_IIR_COEFF_1_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_IIR_COEFF_2_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_FF_A_GAIN_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_FF_B_GAIN_CTL, 0x00},
	{ BOLERO_CDC_TX_ANC0_FB_GAIN_CTL, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX0_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX0_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX0_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX0_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX0_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX0_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX0_TX_PATH_SEC7, 0x25},
	{ BOLERO_CDC_TX1_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX1_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX1_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX1_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX1_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX1_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX1_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX1_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX1_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX1_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX1_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX2_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX2_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX2_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX2_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX2_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX2_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX2_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX2_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX2_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX2_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX2_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX3_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX3_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX3_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX3_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX3_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX3_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX3_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX3_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX3_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX3_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX3_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX4_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX4_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX4_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX4_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX4_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX4_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX4_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX4_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX4_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX4_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX4_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX5_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX5_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX5_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX5_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX5_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX5_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX5_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX5_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX5_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX5_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX5_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX6_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX6_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX6_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX6_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX6_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX6_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX6_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX6_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX6_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX6_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX6_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_TX7_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_TX7_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_TX7_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_TX7_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_TX7_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_TX7_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_TX7_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_TX7_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_TX7_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_TX7_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_TX7_TX_PATH_SEC6, 0x00},

	/* RX Macro */
	{ BOLERO_CDC_RX_TOP_TOP_CFG0, 0x00},
	{ BOLERO_CDC_RX_TOP_SWR_CTRL, 0x00},
	{ BOLERO_CDC_RX_TOP_DEBUG, 0x00},
	{ BOLERO_CDC_RX_TOP_DEBUG_BUS, 0x00},
	{ BOLERO_CDC_RX_TOP_DEBUG_EN0, 0x00},
	{ BOLERO_CDC_RX_TOP_DEBUG_EN1, 0x00},
	{ BOLERO_CDC_RX_TOP_DEBUG_EN2, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHL_COMP_WR_LSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHL_COMP_WR_MSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHL_COMP_LUT, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHL_COMP_RD_LSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHL_COMP_RD_MSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHR_COMP_WR_LSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHR_COMP_WR_MSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHR_COMP_LUT, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHR_COMP_RD_LSB, 0x00},
	{ BOLERO_CDC_RX_TOP_HPHR_COMP_RD_MSB, 0x00},
	{ BOLERO_CDC_RX_TOP_DSD0_DEBUG_CFG0, 0x11},
	{ BOLERO_CDC_RX_TOP_DSD0_DEBUG_CFG1, 0x20},
	{ BOLERO_CDC_RX_TOP_DSD0_DEBUG_CFG2, 0x00},
	{ BOLERO_CDC_RX_TOP_DSD0_DEBUG_CFG3, 0x00},
	{ BOLERO_CDC_RX_TOP_DSD1_DEBUG_CFG0, 0x11},
	{ BOLERO_CDC_RX_TOP_DSD1_DEBUG_CFG1, 0x20},
	{ BOLERO_CDC_RX_TOP_DSD1_DEBUG_CFG2, 0x00},
	{ BOLERO_CDC_RX_TOP_DSD1_DEBUG_CFG3, 0x00},
	{ BOLERO_CDC_RX_TOP_RX_I2S_CTL, 0x0C},
	{ BOLERO_CDC_RX_TOP_TX_I2S2_CTL, 0x0C},
	{ BOLERO_CDC_RX_TOP_I2S_CLK, 0x0C},
	{ BOLERO_CDC_RX_TOP_I2S_RESET, 0x00},
	{ BOLERO_CDC_RX_TOP_I2S_MUX, 0x00},
	{ BOLERO_CDC_RX_CLK_RST_CTRL_MCLK_CONTROL, 0x00},
	{ BOLERO_CDC_RX_CLK_RST_CTRL_FS_CNT_CONTROL, 0x00},
	{ BOLERO_CDC_RX_CLK_RST_CTRL_SWR_CONTROL, 0x00},
	{ BOLERO_CDC_RX_CLK_RST_CTRL_DSD_CONTROL, 0x00},
	{ BOLERO_CDC_RX_CLK_RST_CTRL_ASRC_SHARE_CONTROL, 0x08},
	{ BOLERO_CDC_RX_SOFTCLIP_CRC, 0x00},
	{ BOLERO_CDC_RX_SOFTCLIP_SOFTCLIP_CTRL, 0x38},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT0_CFG0, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT0_CFG1, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT1_CFG0, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT1_CFG1, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT2_CFG0, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_INT2_CFG1, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_MIX_CFG4, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_RX_MIX_CFG5, 0x00},
	{ BOLERO_CDC_RX_INP_MUX_SIDETONE_SRC_CFG0, 0x00},
	{ BOLERO_CDC_RX_CLSH_CRC, 0x00},
	{ BOLERO_CDC_RX_CLSH_DLY_CTRL, 0x03},
	{ BOLERO_CDC_RX_CLSH_DECAY_CTRL, 0x02},
	{ BOLERO_CDC_RX_CLSH_HPH_V_PA, 0x1C},
	{ BOLERO_CDC_RX_CLSH_EAR_V_PA, 0x39},
	{ BOLERO_CDC_RX_CLSH_HPH_V_HD, 0x0C},
	{ BOLERO_CDC_RX_CLSH_EAR_V_HD, 0x0C},
	{ BOLERO_CDC_RX_CLSH_K1_MSB, 0x01},
	{ BOLERO_CDC_RX_CLSH_K1_LSB, 0x00},
	{ BOLERO_CDC_RX_CLSH_K2_MSB, 0x00},
	{ BOLERO_CDC_RX_CLSH_K2_LSB, 0x80},
	{ BOLERO_CDC_RX_CLSH_IDLE_CTRL, 0x00},
	{ BOLERO_CDC_RX_CLSH_IDLE_HPH, 0x00},
	{ BOLERO_CDC_RX_CLSH_IDLE_EAR, 0x00},
	{ BOLERO_CDC_RX_CLSH_TEST0, 0x07},
	{ BOLERO_CDC_RX_CLSH_TEST1, 0x00},
	{ BOLERO_CDC_RX_CLSH_OVR_VREF, 0x00},
	{ BOLERO_CDC_RX_CLSH_CLSG_CTL, 0x02},
	{ BOLERO_CDC_RX_CLSH_CLSG_CFG1, 0x9A},
	{ BOLERO_CDC_RX_CLSH_CLSG_CFG2, 0x10},
	{ BOLERO_CDC_RX_BCL_VBAT_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_CFG, 0x10},
	{ BOLERO_CDC_RX_BCL_VBAT_ADC_CAL1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_ADC_CAL2, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_ADC_CAL3, 0x04},
	{ BOLERO_CDC_RX_BCL_VBAT_PK_EST1, 0xE0},
	{ BOLERO_CDC_RX_BCL_VBAT_PK_EST2, 0x01},
	{ BOLERO_CDC_RX_BCL_VBAT_PK_EST3, 0x40},
	{ BOLERO_CDC_RX_BCL_VBAT_RF_PROC1, 0x2A},
	{ BOLERO_CDC_RX_BCL_VBAT_RF_PROC1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_TAC1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_TAC2, 0x18},
	{ BOLERO_CDC_RX_BCL_VBAT_TAC3, 0x18},
	{ BOLERO_CDC_RX_BCL_VBAT_TAC4, 0x03},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD1, 0x01},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD2, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD3, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD4, 0x64},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD5, 0x01},
	{ BOLERO_CDC_RX_BCL_VBAT_DEBUG1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_UPD_MON, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_GAIN_MON_VAL, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_BAN, 0x0C},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD2, 0x77},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD3, 0x01},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD4, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD5, 0x4B},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD6, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD7, 0x01},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD8, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_BCL_GAIN_UPD9, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_ATTN1, 0x04},
	{ BOLERO_CDC_RX_BCL_VBAT_ATTN2, 0x08},
	{ BOLERO_CDC_RX_BCL_VBAT_ATTN3, 0x0C},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CTL1, 0xE0},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CTL2, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CFG1, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CFG2, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CFG3, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_CFG4, 0x00},
	{ BOLERO_CDC_RX_BCL_VBAT_DECODE_ST, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_CFG, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_CLR_COMMIT, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN1_MASK0, 0xFF},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN1_STATUS0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN1_CLEAR0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN2_MASK0, 0xFF},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN2_STATUS0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_PIN2_CLEAR0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_LEVEL0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_BYPASS0, 0x00},
	{ BOLERO_CDC_RX_INTR_CTRL_SET0, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_CTL, 0x04},
	{ BOLERO_CDC_RX_RX0_RX_PATH_CFG0, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_CFG1, 0x64},
	{ BOLERO_CDC_RX_RX0_RX_PATH_CFG2, 0x8F},
	{ BOLERO_CDC_RX_RX0_RX_PATH_CFG3, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_VOL_CTL, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_MIX_CTL, 0x04},
	{ BOLERO_CDC_RX_RX0_RX_PATH_MIX_CFG, 0x7E},
	{ BOLERO_CDC_RX_RX0_RX_VOL_MIX_CTL, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_SEC1, 0x08},
	{ BOLERO_CDC_RX_RX0_RX_PATH_SEC2, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_SEC3, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_SEC4, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_SEC7, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_MIX_SEC0, 0x08},
	{ BOLERO_CDC_RX_RX0_RX_PATH_MIX_SEC1, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_CTL, 0x08},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA1, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA2, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA3, 0x00},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA4, 0x55},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA5, 0x55},
	{ BOLERO_CDC_RX_RX0_RX_PATH_DSM_DATA6, 0x55},
	{ BOLERO_CDC_RX_RX1_RX_PATH_CTL, 0x04},
	{ BOLERO_CDC_RX_RX1_RX_PATH_CFG0, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_CFG1, 0x64},
	{ BOLERO_CDC_RX_RX1_RX_PATH_CFG2, 0x8F},
	{ BOLERO_CDC_RX_RX1_RX_PATH_CFG3, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_VOL_CTL, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_MIX_CTL, 0x04},
	{ BOLERO_CDC_RX_RX1_RX_PATH_MIX_CFG, 0x7E},
	{ BOLERO_CDC_RX_RX1_RX_VOL_MIX_CTL, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_SEC1, 0x08},
	{ BOLERO_CDC_RX_RX1_RX_PATH_SEC2, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_SEC3, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_SEC4, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_SEC7, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_MIX_SEC0, 0x08},
	{ BOLERO_CDC_RX_RX1_RX_PATH_MIX_SEC1, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_CTL, 0x08},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA1, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA2, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA3, 0x00},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA4, 0x55},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA5, 0x55},
	{ BOLERO_CDC_RX_RX1_RX_PATH_DSM_DATA6, 0x55},
	{ BOLERO_CDC_RX_RX2_RX_PATH_CTL, 0x04},
	{ BOLERO_CDC_RX_RX2_RX_PATH_CFG0, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_CFG1, 0x64},
	{ BOLERO_CDC_RX_RX2_RX_PATH_CFG2, 0x8F},
	{ BOLERO_CDC_RX_RX2_RX_PATH_CFG3, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_VOL_CTL, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_MIX_CTL, 0x04},
	{ BOLERO_CDC_RX_RX2_RX_PATH_MIX_CFG, 0x7E},
	{ BOLERO_CDC_RX_RX2_RX_VOL_MIX_CTL, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC0, 0x04},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC1, 0x08},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC2, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC3, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC4, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_SEC7, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_MIX_SEC0, 0x08},
	{ BOLERO_CDC_RX_RX2_RX_PATH_MIX_SEC1, 0x00},
	{ BOLERO_CDC_RX_RX2_RX_PATH_DSM_CTL, 0x00},
	{ BOLERO_CDC_RX_IDLE_DETECT_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_IDLE_DETECT_CFG0, 0x07},
	{ BOLERO_CDC_RX_IDLE_DETECT_CFG1, 0x3C},
	{ BOLERO_CDC_RX_IDLE_DETECT_CFG2, 0x00},
	{ BOLERO_CDC_RX_IDLE_DETECT_CFG3, 0x00},
	{ BOLERO_CDC_RX_COMPANDER0_CTL0, 0x60},
	{ BOLERO_CDC_RX_COMPANDER0_CTL1, 0xDB},
	{ BOLERO_CDC_RX_COMPANDER0_CTL2, 0xFF},
	{ BOLERO_CDC_RX_COMPANDER0_CTL3, 0x35},
	{ BOLERO_CDC_RX_COMPANDER0_CTL4, 0xFF},
	{ BOLERO_CDC_RX_COMPANDER0_CTL5, 0x00},
	{ BOLERO_CDC_RX_COMPANDER0_CTL6, 0x01},
	{ BOLERO_CDC_RX_COMPANDER0_CTL7, 0x28},
	{ BOLERO_CDC_RX_COMPANDER1_CTL0, 0x60},
	{ BOLERO_CDC_RX_COMPANDER1_CTL1, 0xDB},
	{ BOLERO_CDC_RX_COMPANDER1_CTL2, 0xFF},
	{ BOLERO_CDC_RX_COMPANDER1_CTL3, 0x35},
	{ BOLERO_CDC_RX_COMPANDER1_CTL4, 0xFF},
	{ BOLERO_CDC_RX_COMPANDER1_CTL5, 0x00},
	{ BOLERO_CDC_RX_COMPANDER1_CTL6, 0x01},
	{ BOLERO_CDC_RX_COMPANDER1_CTL7, 0x28},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B1_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B2_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B3_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B4_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B5_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B6_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B7_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_B8_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_CTL, 0x40},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_GAIN_TIMER_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_COEF_B1_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR0_IIR_COEF_B2_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B1_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B2_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B3_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B4_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B5_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B6_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B7_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_B8_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_CTL, 0x40},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_GAIN_TIMER_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_COEF_B1_CTL, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_IIR1_IIR_COEF_B2_CTL, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR0_MIX_CFG0, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR0_MIX_CFG1, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR0_MIX_CFG2, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR0_MIX_CFG3, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR1_MIX_CFG0, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR1_MIX_CFG1, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR1_MIX_CFG2, 0x00},
	{ BOLERO_CDC_RX_IIR_INP_MUX_IIR1_MIX_CFG3, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_SRC0_ST_SRC_PATH_CTL, 0x04},
	{ BOLERO_CDC_RX_SIDETONE_SRC0_ST_SRC_PATH_CFG1, 0x00},
	{ BOLERO_CDC_RX_SIDETONE_SRC1_ST_SRC_PATH_CTL, 0x04},
	{ BOLERO_CDC_RX_SIDETONE_SRC1_ST_SRC_PATH_CFG1, 0x00},
	{ BOLERO_CDC_RX_EC_REF_HQ0_EC_REF_HQ_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_REF_HQ0_EC_REF_HQ_CFG0, 0x01},
	{ BOLERO_CDC_RX_EC_REF_HQ1_EC_REF_HQ_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_REF_HQ1_EC_REF_HQ_CFG0, 0x01},
	{ BOLERO_CDC_RX_EC_REF_HQ2_EC_REF_HQ_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_REF_HQ2_EC_REF_HQ_CFG0, 0x01},
	{ BOLERO_CDC_RX_EC_ASRC0_CLK_RST_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_CTL0, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_CTL1, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_FIFO_CTL, 0xA8},
	{ BOLERO_CDC_RX_EC_ASRC0_STATUS_FMIN_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_STATUS_FMIN_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_STATUS_FMAX_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_STATUS_FMAX_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC0_STATUS_FIFO, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_CLK_RST_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_CTL0, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_CTL1, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_FIFO_CTL, 0xA8},
	{ BOLERO_CDC_RX_EC_ASRC1_STATUS_FMIN_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_STATUS_FMIN_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_STATUS_FMAX_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_STATUS_FMAX_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC1_STATUS_FIFO, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_CLK_RST_CTL, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_CTL0, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_CTL1, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_FIFO_CTL, 0xA8},
	{ BOLERO_CDC_RX_EC_ASRC2_STATUS_FMIN_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_STATUS_FMIN_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_STATUS_FMAX_CNTR_LSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_STATUS_FMAX_CNTR_MSB, 0x00},
	{ BOLERO_CDC_RX_EC_ASRC2_STATUS_FIFO, 0x00},
	{ BOLERO_CDC_RX_DSD0_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_DSD0_CFG0, 0x00},
	{ BOLERO_CDC_RX_DSD0_CFG1, 0x62},
	{ BOLERO_CDC_RX_DSD0_CFG2, 0x96},
	{ BOLERO_CDC_RX_DSD1_PATH_CTL, 0x00},
	{ BOLERO_CDC_RX_DSD1_CFG0, 0x00},
	{ BOLERO_CDC_RX_DSD1_CFG1, 0x62},
	{ BOLERO_CDC_RX_DSD1_CFG2, 0x96},

	/* WSA Macro */
	{ BOLERO_CDC_WSA_CLK_RST_CTRL_MCLK_CONTROL, 0x00},
	{ BOLERO_CDC_WSA_CLK_RST_CTRL_FS_CNT_CONTROL, 0x00},
	{ BOLERO_CDC_WSA_CLK_RST_CTRL_SWR_CONTROL, 0x00},
	{ BOLERO_CDC_WSA_TOP_TOP_CFG0, 0x00},
	{ BOLERO_CDC_WSA_TOP_TOP_CFG1, 0x00},
	{ BOLERO_CDC_WSA_TOP_FREQ_MCLK, 0x00},
	{ BOLERO_CDC_WSA_TOP_DEBUG_BUS_SEL, 0x00},
	{ BOLERO_CDC_WSA_TOP_DEBUG_EN0, 0x00},
	{ BOLERO_CDC_WSA_TOP_DEBUG_EN1, 0x00},
	{ BOLERO_CDC_WSA_TOP_DEBUG_DSM_LB, 0x88},
	{ BOLERO_CDC_WSA_TOP_RX_I2S_CTL, 0x0C},
	{ BOLERO_CDC_WSA_TOP_TX_I2S_CTL, 0x0C},
	{ BOLERO_CDC_WSA_TOP_I2S_CLK, 0x02},
	{ BOLERO_CDC_WSA_TOP_I2S_RESET, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_INT0_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_INT0_CFG1, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_INT1_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_INT1_CFG1, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_MIX_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_RX_EC_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX_INP_MUX_SOFTCLIP_CFG0, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_PATH_CTL, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_CFG, 0x10},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_ADC_CAL1, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_ADC_CAL2, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_ADC_CAL3, 0x04},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_PK_EST1, 0xE0},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_PK_EST2, 0x01},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_PK_EST3, 0x40},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_RF_PROC1, 0x2A},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_RF_PROC2, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_TAC1, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_TAC2, 0x18},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_TAC3, 0x18},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_TAC4, 0x03},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD1, 0x01},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD2, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD3, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD4, 0x64},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD5, 0x01},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DEBUG1, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_UPD_MON, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_MON_VAL, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BAN, 0x0C},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD1, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD2, 0x77},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD3, 0x01},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD4, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD5, 0x4B},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD6, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD7, 0x01},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD8, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_GAIN_UPD9, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_ATTN1, 0x04},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_ATTN2, 0x08},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_BCL_ATTN3, 0x0C},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CTL1, 0xE0},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CTL2, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CFG1, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CFG2, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CFG3, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_CFG4, 0x00},
	{ BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_ST, 0x00},
	{ BOLERO_CDC_WSA_TX0_SPKR_PROT_PATH_CTL, 0x02},
	{ BOLERO_CDC_WSA_TX0_SPKR_PROT_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_TX1_SPKR_PROT_PATH_CTL, 0x02},
	{ BOLERO_CDC_WSA_TX1_SPKR_PROT_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_TX2_SPKR_PROT_PATH_CTL, 0x02},
	{ BOLERO_CDC_WSA_TX2_SPKR_PROT_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_TX3_SPKR_PROT_PATH_CTL, 0x02},
	{ BOLERO_CDC_WSA_TX3_SPKR_PROT_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_CFG, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_CLR_COMMIT, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN1_MASK0, 0xFF},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN1_STATUS0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN1_CLEAR0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN2_MASK0, 0xFF},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN2_STATUS0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_PIN2_CLEAR0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_LEVEL0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_BYPASS0, 0x00},
	{ BOLERO_CDC_WSA_INTR_CTRL_SET0, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_CTL, 0x04},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_CFG1, 0x64},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_CFG2, 0x8F},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_CFG3, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_VOL_CTL, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_MIX_CTL, 0x04},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_MIX_CFG, 0x7E},
	{ BOLERO_CDC_WSA_RX0_RX_VOL_MIX_CTL, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC0, 0x04},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC1, 0x08},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC2, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC3, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_SEC7, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_MIX_SEC0, 0x08},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_MIX_SEC1, 0x00},
	{ BOLERO_CDC_WSA_RX0_RX_PATH_DSMDEM_CTL, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_CFG0, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_CFG1, 0x64},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_CFG2, 0x8F},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_CFG3, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_VOL_CTL, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_MIX_CTL, 0x04},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_MIX_CFG, 0x7E},
	{ BOLERO_CDC_WSA_RX1_RX_VOL_MIX_CTL, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC0, 0x04},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC1, 0x08},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC2, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC3, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_SEC7, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_MIX_SEC0, 0x08},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_MIX_SEC1, 0x00},
	{ BOLERO_CDC_WSA_RX1_RX_PATH_DSMDEM_CTL, 0x00},
	{ BOLERO_CDC_WSA_BOOST0_BOOST_PATH_CTL, 0x00},
	{ BOLERO_CDC_WSA_BOOST0_BOOST_CTL, 0xD0},
	{ BOLERO_CDC_WSA_BOOST0_BOOST_CFG1, 0x89},
	{ BOLERO_CDC_WSA_BOOST0_BOOST_CFG2, 0x04},
	{ BOLERO_CDC_WSA_BOOST1_BOOST_PATH_CTL, 0x00},
	{ BOLERO_CDC_WSA_BOOST1_BOOST_CTL, 0xD0},
	{ BOLERO_CDC_WSA_BOOST1_BOOST_CFG1, 0x89},
	{ BOLERO_CDC_WSA_BOOST1_BOOST_CFG2, 0x04},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL0, 0x60},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL1, 0xDB},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL2, 0xFF},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL3, 0x35},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL4, 0xFF},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL5, 0x00},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL6, 0x01},
	{ BOLERO_CDC_WSA_COMPANDER0_CTL7, 0x28},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL0, 0x60},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL1, 0xDB},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL2, 0xFF},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL3, 0x35},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL4, 0xFF},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL5, 0x00},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL6, 0x01},
	{ BOLERO_CDC_WSA_COMPANDER1_CTL7, 0x28},
	{ BOLERO_CDC_WSA_SOFTCLIP0_CRC, 0x00},
	{ BOLERO_CDC_WSA_SOFTCLIP0_SOFTCLIP_CTRL, 0x38},
	{ BOLERO_CDC_WSA_SOFTCLIP1_CRC, 0x00},
	{ BOLERO_CDC_WSA_SOFTCLIP1_SOFTCLIP_CTRL, 0x38},
	{ BOLERO_CDC_WSA_EC_HQ0_EC_REF_HQ_PATH_CTL, 0x00},
	{ BOLERO_CDC_WSA_EC_HQ0_EC_REF_HQ_CFG0, 0x01},
	{ BOLERO_CDC_WSA_EC_HQ1_EC_REF_HQ_PATH_CTL, 0x00},
	{ BOLERO_CDC_WSA_EC_HQ1_EC_REF_HQ_CFG0, 0x01},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_CLK_RST_CTL, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_CTL0, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_CTL1, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_FIFO_CTL, 0xA8},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMIN_CNTR_LSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMIN_CNTR_MSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMAX_CNTR_LSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMAX_CNTR_MSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FIFO, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_CLK_RST_CTL, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_CTL0, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_CTL1, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_FIFO_CTL, 0xA8},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMIN_CNTR_LSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMIN_CNTR_MSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMAX_CNTR_LSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMAX_CNTR_MSB, 0x00},
	{ BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FIFO, 0x00},

	/* VA macro */
	{ BOLERO_CDC_VA_CLK_RST_CTRL_MCLK_CONTROL, 0x00},
	{ BOLERO_CDC_VA_CLK_RST_CTRL_FS_CNT_CONTROL, 0x00},
	{ BOLERO_CDC_VA_CLK_RST_CTRL_SWR_CONTROL, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_TOP_CFG0, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DMIC0_CTL, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DMIC1_CTL, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DMIC2_CTL, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DMIC3_CTL, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DMIC_CFG, 0x80},
	{ BOLERO_CDC_VA_TOP_CSR_DEBUG_BUS, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_DEBUG_EN, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_TX_I2S_CTL, 0x0C},
	{ BOLERO_CDC_VA_TOP_CSR_I2S_CLK, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_I2S_RESET, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_CORE_ID_0, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_CORE_ID_1, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_CORE_ID_2, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_CORE_ID_3, 0x00},
	{ BOLERO_CDC_VA_TOP_CSR_SWR_MIC_CTL0, 0xEE},
	{ BOLERO_CDC_VA_TOP_CSR_SWR_MIC_CTL1, 0xEE},
	{ BOLERO_CDC_VA_TOP_CSR_SWR_MIC_CTL2, 0xEE},
	{ BOLERO_CDC_VA_TOP_CSR_SWR_CTRL, 0x06},

	/* VA core */
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX0_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX0_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX1_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX1_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX2_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX2_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX3_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX3_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX4_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX4_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX5_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX5_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX6_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX6_CFG1, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX7_CFG0, 0x00},
	{ BOLERO_CDC_VA_INP_MUX_ADC_MUX7_CFG1, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX0_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX0_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX0_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX0_TX_PATH_SEC7, 0x25},
	{ BOLERO_CDC_VA_TX1_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX1_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX1_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX1_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX1_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX2_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX2_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX2_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX2_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX2_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX3_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX3_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX3_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX3_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX3_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX4_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX4_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX4_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX4_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX4_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX5_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX5_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX5_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX5_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX5_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX6_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX6_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX6_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX6_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX6_TX_PATH_SEC6, 0x00},
	{ BOLERO_CDC_VA_TX7_TX_PATH_CTL, 0x04},
	{ BOLERO_CDC_VA_TX7_TX_PATH_CFG0, 0x10},
	{ BOLERO_CDC_VA_TX7_TX_PATH_CFG1, 0x0B},
	{ BOLERO_CDC_VA_TX7_TX_VOL_CTL, 0x00},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC0, 0x00},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC1, 0x00},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC2, 0x01},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC3, 0x3C},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC4, 0x20},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC5, 0x00},
	{ BOLERO_CDC_VA_TX7_TX_PATH_SEC6, 0x00},
};

static bool bolero_is_readable_register(struct device *dev,
					unsigned int reg)
{
	struct bolero_priv *priv = dev_get_drvdata(dev);
	u16 reg_offset;
	int macro_id;
	u8 *reg_tbl = NULL;

	if (!priv)
		return false;

	macro_id = bolero_get_macro_id(priv->va_without_decimation,
				       reg);
	if (macro_id < 0 || !priv->macros_supported[macro_id])
		return false;

	reg_tbl = bolero_reg_access[macro_id];
	reg_offset = (reg - macro_id_base_offset[macro_id])/4;

	if (reg_tbl)
		return (reg_tbl[reg_offset] & RD_REG);

	return false;
}

static bool bolero_is_writeable_register(struct device *dev,
					 unsigned int reg)
{
	struct bolero_priv *priv = dev_get_drvdata(dev);
	u16 reg_offset;
	int macro_id;
	const u8 *reg_tbl = NULL;

	if (!priv)
		return false;

	macro_id = bolero_get_macro_id(priv->va_without_decimation,
				       reg);
	if (macro_id < 0 || !priv->macros_supported[macro_id])
		return false;

	reg_tbl = bolero_reg_access[macro_id];
	reg_offset = (reg - macro_id_base_offset[macro_id])/4;

	if (reg_tbl)
		return (reg_tbl[reg_offset] & WR_REG);

	return false;
}

static bool bolero_is_volatile_register(struct device *dev,
					unsigned int reg)
{
	/* Update volatile list for rx/tx macros */
	switch (reg) {
	case BOLERO_CDC_VA_TOP_CSR_CORE_ID_0:
	case BOLERO_CDC_VA_TOP_CSR_CORE_ID_1:
	case BOLERO_CDC_VA_TOP_CSR_CORE_ID_2:
	case BOLERO_CDC_VA_TOP_CSR_CORE_ID_3:
	case BOLERO_CDC_TX_TOP_CSR_SWR_DMIC0_CTL:
	case BOLERO_CDC_TX_TOP_CSR_SWR_DMIC1_CTL:
	case BOLERO_CDC_TX_TOP_CSR_SWR_DMIC2_CTL:
	case BOLERO_CDC_TX_TOP_CSR_SWR_DMIC3_CTL:
	case BOLERO_CDC_WSA_VBAT_BCL_VBAT_GAIN_MON_VAL:
	case BOLERO_CDC_WSA_VBAT_BCL_VBAT_DECODE_ST:
	case BOLERO_CDC_WSA_INTR_CTRL_PIN1_STATUS0:
	case BOLERO_CDC_WSA_INTR_CTRL_PIN2_STATUS0:
	case BOLERO_CDC_WSA_COMPANDER0_CTL6:
	case BOLERO_CDC_WSA_COMPANDER1_CTL6:
	case BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMIN_CNTR_LSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMIN_CNTR_MSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMAX_CNTR_LSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FMAX_CNTR_MSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC0_STATUS_FIFO:
	case BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMIN_CNTR_LSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMIN_CNTR_MSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMAX_CNTR_LSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FMAX_CNTR_MSB:
	case BOLERO_CDC_WSA_SPLINE_ASRC1_STATUS_FIFO:
	case BOLERO_CDC_RX_TOP_HPHL_COMP_RD_LSB:
	case BOLERO_CDC_RX_TOP_HPHL_COMP_WR_LSB:
	case BOLERO_CDC_RX_TOP_HPHL_COMP_RD_MSB:
	case BOLERO_CDC_RX_TOP_HPHL_COMP_WR_MSB:
	case BOLERO_CDC_RX_TOP_HPHR_COMP_RD_LSB:
	case BOLERO_CDC_RX_TOP_HPHR_COMP_WR_LSB:
	case BOLERO_CDC_RX_TOP_HPHR_COMP_RD_MSB:
	case BOLERO_CDC_RX_TOP_HPHR_COMP_WR_MSB:
	case BOLERO_CDC_RX_TOP_DSD0_DEBUG_CFG2:
	case BOLERO_CDC_RX_TOP_DSD1_DEBUG_CFG2:
	case BOLERO_CDC_RX_BCL_VBAT_GAIN_MON_VAL:
	case BOLERO_CDC_RX_BCL_VBAT_DECODE_ST:
	case BOLERO_CDC_RX_INTR_CTRL_PIN1_STATUS0:
	case BOLERO_CDC_RX_INTR_CTRL_PIN2_STATUS0:
	case BOLERO_CDC_RX_COMPANDER0_CTL6:
	case BOLERO_CDC_RX_COMPANDER1_CTL6:
	case BOLERO_CDC_RX_EC_ASRC0_STATUS_FMIN_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC0_STATUS_FMIN_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC0_STATUS_FMAX_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC0_STATUS_FMAX_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC0_STATUS_FIFO:
	case BOLERO_CDC_RX_EC_ASRC1_STATUS_FMIN_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC1_STATUS_FMIN_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC1_STATUS_FMAX_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC1_STATUS_FMAX_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC1_STATUS_FIFO:
	case BOLERO_CDC_RX_EC_ASRC2_STATUS_FMIN_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC2_STATUS_FMIN_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC2_STATUS_FMAX_CNTR_LSB:
	case BOLERO_CDC_RX_EC_ASRC2_STATUS_FMAX_CNTR_MSB:
	case BOLERO_CDC_RX_EC_ASRC2_STATUS_FIFO:
		return true;
	}
	return false;
}

const struct regmap_config bolero_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.reg_stride = 4,
	.cache_type = REGCACHE_RBTREE,
	.reg_defaults = bolero_defaults,
	.num_reg_defaults = ARRAY_SIZE(bolero_defaults),
	.max_register = BOLERO_CDC_MAX_REGISTER,
	.writeable_reg = bolero_is_writeable_register,
	.volatile_reg = bolero_is_volatile_register,
	.readable_reg = bolero_is_readable_register,
};
