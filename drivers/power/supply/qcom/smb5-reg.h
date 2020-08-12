/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2018-2019 The Linux Foundation. All rights reserved.
 */

#ifndef __SMB5_CHARGER_REG_H
#define __SMB5_CHARGER_REG_H

#include <linux/bitops.h>

#define CHGR_BASE	0x1000
#define DCDC_BASE	0x1100
#define BATIF_BASE	0x1200
#define USBIN_BASE	0x1300
#define DCIN_BASE	0x1400
#define TYPEC_BASE	0X1500
#define MISC_BASE	0x1600
#define MISC_PBS_BASE	0x7500

#define PERPH_TYPE_OFFSET	0x04
#define TYPE_MASK		GENMASK(7, 0)
#define PERPH_SUBTYPE_OFFSET	0x05
#define SUBTYPE_MASK		GENMASK(7, 0)
#define INT_RT_STS_OFFSET	0x10

/********************************
 *  CHGR Peripheral Registers  *
 ********************************/
#define BATTERY_CHARGER_STATUS_1_REG		(CHGR_BASE + 0x06)
#define BATTERY_CHARGER_STATUS_MASK		GENMASK(2, 0)
enum {
	INHIBIT_CHARGE = 0,
	TRICKLE_CHARGE,
	PRE_CHARGE,
	FULLON_CHARGE,
	TAPER_CHARGE,
	TERMINATE_CHARGE,
	PAUSE_CHARGE,
	DISABLE_CHARGE,
};

#define BATTERY_CHARGER_STATUS_2_REG		(CHGR_BASE + 0x07)
#define CHARGER_ERROR_STATUS_BAT_OV_BIT		BIT(1)

#define BATTERY_CHARGER_STATUS_5_REG		(CHGR_BASE + 0x0B)
#define ENABLE_TRICKLE_BIT			BIT(2)
#define ENABLE_PRE_CHARGING_BIT			BIT(1)
#define ENABLE_FULLON_MODE_BIT			BIT(0)

#define BATTERY_CHARGER_STATUS_7_REG		(CHGR_BASE + 0x0D)
#define BAT_TEMP_STATUS_SOFT_LIMIT_MASK		GENMASK(5, 4)
#define BAT_TEMP_STATUS_HOT_SOFT_BIT		BIT(5)
#define BAT_TEMP_STATUS_COLD_SOFT_BIT		BIT(4)
#define BAT_TEMP_STATUS_HARD_LIMIT_MASK		GENMASK(3, 2)
#define BAT_TEMP_STATUS_TOO_HOT_BIT		BIT(3)
#define BAT_TEMP_STATUS_TOO_COLD_BIT		BIT(2)
#define BAT_TEMP_STATUS_TOO_HOT_AFP_BIT		BIT(1)
#define BAT_TEMP_STATUS_TOO_COLD_AFP_BIT	BIT(0)

#define CHARGING_ENABLE_CMD_REG			(CHGR_BASE + 0x42)
#define CHARGING_ENABLE_CMD_BIT			BIT(0)

#define CHARGING_PAUSE_CMD_REG			(CHGR_BASE + 0x43)
#define CHARGING_PAUSE_CMD_BIT			BIT(0)

#define CHGR_CFG2_REG				(CHGR_BASE + 0x51)
#define RECHG_MASK				GENMASK(2, 1)
#define VBAT_BASED_RECHG_BIT			BIT(2)
#define SOC_BASED_RECHG_BIT			GENMASK(2, 1)
#define CHARGER_INHIBIT_BIT			BIT(0)

#define CHGR_FAST_CHARGE_CURRENT_CFG_REG	(CHGR_BASE + 0x61)

#define CHGR_ADC_ITERM_UP_THD_MSB_REG		(CHGR_BASE + 0x67)
#define CHGR_ADC_ITERM_UP_THD_LSB_REG		(CHGR_BASE + 0x68)
#define CHGR_ADC_ITERM_LO_THD_MSB_REG		(CHGR_BASE + 0x69)
#define CHGR_ADC_ITERM_LO_THD_LSB_REG		(CHGR_BASE + 0x6A)

#define CHGR_NO_SAMPLE_TERM_RCHG_CFG_REG	(CHGR_BASE + 0x6B)
#define NO_OF_SAMPLE_FOR_RCHG_SHIFT		2
#define NO_OF_SAMPLE_FOR_RCHG			GENMASK(3, 2)

#define CHGR_ADC_TERM_CFG_REG			(CHGR_BASE + 0x6C)
#define TERM_BASED_ON_SYNC_CONV_OR_SAMPLE_CNT	BIT(0)
#define TERM_BASED_ON_SYNC_CONV			0
#define TERM_BASED_ON_SAMPLE_CNT		1

#define CHGR_FLOAT_VOLTAGE_CFG_REG		(CHGR_BASE + 0x70)

#define CHARGE_INHIBIT_THRESHOLD_CFG_REG	(CHGR_BASE + 0x72)
#define CHARGE_INHIBIT_THRESHOLD_MASK		GENMASK(1, 0)
#define INHIBIT_ANALOG_VFLT_MINUS_50MV		0
#define INHIBIT_ANALOG_VFLT_MINUS_100MV		1
#define INHIBIT_ANALOG_VFLT_MINUS_200MV		2
#define INHIBIT_ANALOG_VFLT_MINUS_300MV		3

#define CHARGE_RCHG_SOC_THRESHOLD_CFG_REG	(CHGR_BASE + 0x7D)

#define CHGR_ADC_RECHARGE_THRESHOLD_MSB_REG	(CHGR_BASE + 0x7E)

#define CHGR_ADC_RECHARGE_THRESHOLD_LSB_REG	(CHGR_BASE + 0x7F)

#define JEITA_EN_CFG_REG			(CHGR_BASE + 0x90)
#define JEITA_EN_HOT_SL_FCV_BIT			BIT(3)
#define JEITA_EN_COLD_SL_FCV_BIT		BIT(2)
#define JEITA_EN_HOT_SL_CCC_BIT			BIT(1)
#define JEITA_EN_COLD_SL_CCC_BIT		BIT(0)

#define JEITA_CCCOMP_CFG_HOT_REG		(CHGR_BASE + 0x92)
#define JEITA_CCCOMP_CFG_COLD_REG		(CHGR_BASE + 0x93)

#define CHGR_JEITA_THRESHOLD_BASE_REG(i)	(CHGR_BASE + 0x94 + (i * 4))
#define CHGR_JEITA_HOT_THRESHOLD_MSB_REG	CHGR_JEITA_THRESHOLD_BASE_REG(0)

#define CHGR_FAST_CHARGE_SAFETY_TIMER_CFG_REG	(CHGR_BASE + 0xA2)
#define FAST_CHARGE_SAFETY_TIMER_192_MIN	0x0
#define FAST_CHARGE_SAFETY_TIMER_384_MIN	0x1
#define FAST_CHARGE_SAFETY_TIMER_768_MIN	0x2
#define FAST_CHARGE_SAFETY_TIMER_1536_MIN	0x3

#define CHGR_ENG_CHARGING_CFG_REG		(CHGR_BASE + 0xC0)
#define CHGR_ITERM_USE_ANALOG_BIT		BIT(3)

/********************************
 *  DCDC Peripheral Registers  *
 ********************************/
#define ICL_MAX_STATUS_REG			(DCDC_BASE + 0x06)
#define ICL_STATUS_REG				(DCDC_BASE + 0x07)
#define AICL_ICL_STATUS_REG			(DCDC_BASE + 0x08)

#define AICL_STATUS_REG				(DCDC_BASE + 0x0A)
#define SOFT_ILIMIT_BIT				BIT(6)
#define AICL_DONE_BIT				BIT(0)

#define POWER_PATH_STATUS_REG			(DCDC_BASE + 0x0B)
#define USBIN_SUSPEND_STS_BIT			BIT(6)
#define USE_USBIN_BIT				BIT(4)
#define USE_DCIN_BIT				BIT(3)
#define POWER_PATH_MASK				GENMASK(2, 1)
#define VALID_INPUT_POWER_SOURCE_STS_BIT	BIT(0)

#define DCDC_CMD_OTG_REG			(DCDC_BASE + 0x40)
#define OTG_EN_BIT				BIT(0)

#define DCDC_FSW_SEL_REG			(DCDC_BASE + 0x50)

#define DCDC_OTG_CURRENT_LIMIT_CFG_REG		(DCDC_BASE + 0x52)

#define DCDC_OTG_CFG_REG			(DCDC_BASE + 0x53)
#define OTG_EN_SRC_CFG_BIT			BIT(1)

#define OTG_FAULT_CONDITION_CFG_REG		(DCDC_BASE + 0x56)
#define USBIN_MID_COMP_FAULT_EN_BIT		BIT(5)
#define USBIN_COLLAPSE_FAULT_EN_BIT		BIT(4)

#define DCDC_CFG_REF_MAX_PSNS_REG		(DCDC_BASE + 0x8C)

#define DCDC_ENG_SDCDC_CFG5_REG			(DCDC_BASE + 0xC4)
#define ENG_SDCDC_BAT_HPWR_MASK			GENMASK(7, 6)
enum {
	BOOST_MODE_THRESH_3P3_V,
	BOOST_MODE_THRESH_3P4_V = 0x40,
	BOOST_MODE_THRESH_3P5_V = 0x80,
	BOOST_MODE_THRESH_3P6_V = 0xC0
};

/********************************
 *  BATIF Peripheral Registers  *
 ********************************/

/* BATIF Interrupt Bits	 */
#define VPH_OV_RT_STS_BIT			BIT(7)
#define BUCK_OC_RT_STS_BIT			BIT(6)
#define BAT_TERMINAL_MISSING_RT_STS_BIT		BIT(5)
#define BAT_THERM_OR_ID_MISSING_RT_STS_BIT      BIT(4)
#define BAT_LOW_RT_STS_BIT			BIT(3)
#define BAT_OV_RT_STS_BIT			BIT(2)
#define ALL_CHNL_CONV_DONE_RT_STS		BIT(1)
#define BAT_TEMP_RT_STS_BIT			BIT(0)

#define SHIP_MODE_REG				(BATIF_BASE + 0x40)
#define SHIP_MODE_EN_BIT			BIT(0)

#define BATIF_ADC_CHANNEL_EN_REG		(BATIF_BASE + 0x82)
#define IBATT_CHANNEL_EN_BIT			BIT(6)
#define CONN_THM_CHANNEL_EN_BIT			BIT(4)
#define DIE_TEMP_CHANNEL_EN_BIT			BIT(2)
#define MISC_THM_CHANNEL_EN_BIT			BIT(1)

#define BATIF_ADC_INTERNAL_PULL_UP_REG		(BATIF_BASE + 0x86)
#define INTERNAL_PULL_UP_CONN_THM_MASK		GENMASK(5, 4)
#define CONN_THM_SHIFT				4
#define INTERNAL_PULL_NO_PULL			0x00
#define INTERNAL_PULL_30K_PULL			0x01
#define INTERNAL_PULL_100K_PULL			0x02
#define INTERNAL_PULL_400K_PULL			0x03

/********************************
 *  USBIN Peripheral Registers  *
 ********************************/
#define APSD_STATUS_REG				(USBIN_BASE + 0x07)
#define APSD_STATUS_7_BIT			BIT(7)
#define HVDCP_CHECK_TIMEOUT_BIT			BIT(6)
#define SLOW_PLUGIN_TIMEOUT_BIT			BIT(5)
#define ENUMERATION_DONE_BIT			BIT(4)
#define VADP_CHANGE_DONE_AFTER_AUTH_BIT		BIT(3)
#define QC_AUTH_DONE_STATUS_BIT			BIT(2)
#define QC_CHARGER_BIT				BIT(1)
#define APSD_DTC_STATUS_DONE_BIT		BIT(0)

#define APSD_RESULT_STATUS_REG			(USBIN_BASE + 0x08)
#define APSD_RESULT_STATUS_7_BIT		BIT(7)
#define APSD_RESULT_STATUS_MASK			GENMASK(6, 0)
#define QC_3P0_BIT				BIT(6)
#define QC_2P0_BIT				BIT(5)
#define FLOAT_CHARGER_BIT			BIT(4)
#define DCP_CHARGER_BIT				BIT(3)
#define CDP_CHARGER_BIT				BIT(2)
#define OCP_CHARGER_BIT				BIT(1)
#define SDP_CHARGER_BIT				BIT(0)

#define QC_CHANGE_STATUS_REG			(USBIN_BASE + 0x09)
#define QC_12V_BIT				BIT(2)
#define QC_9V_BIT				BIT(1)
#define QC_5V_BIT				BIT(0)
#define QC_2P0_STATUS_MASK			GENMASK(2, 0)

/* USBIN Interrupt Bits */
#define USBIN_ICL_CHANGE_RT_STS_BIT		BIT(7)
#define USBIN_SOURCE_CHANGE_RT_STS_BIT		BIT(6)
#define USBIN_REVI_RT_STS_BIT			BIT(5)
#define USBIN_PLUGIN_RT_STS_BIT			BIT(4)
#define USBIN_OV_RT_STS_BIT			BIT(3)
#define USBIN_UV_RT_STS_BIT			BIT(2)
#define USBIN_VASHDN_RT_STS_BIT			BIT(1)
#define USBIN_COLLAPSE_RT_STS_BIT		BIT(0)

#define USBIN_CMD_IL_REG			(USBIN_BASE + 0x40)
#define USBIN_SUSPEND_BIT			BIT(0)

#define CMD_APSD_REG				(USBIN_BASE + 0x41)
#define APSD_RERUN_BIT				BIT(0)

#define CMD_ICL_OVERRIDE_REG			(USBIN_BASE + 0x42)
#define ICL_OVERRIDE_BIT			BIT(0)

#define CMD_HVDCP_2_REG				(USBIN_BASE + 0x43)
#define FORCE_12V_BIT				BIT(5)
#define FORCE_9V_BIT				BIT(4)
#define FORCE_5V_BIT				BIT(3)
#define IDLE_BIT				BIT(2)
#define SINGLE_DECREMENT_BIT			BIT(1)
#define SINGLE_INCREMENT_BIT			BIT(0)

#define USBIN_ADAPTER_ALLOW_OVERRIDE_REG	(USBIN_BASE + 0x44)
#define CONTINUOUS				BIT(3)
#define FORCE_12V				BIT(2)
#define FORCE_9V				BIT(1)
#define FORCE_5V				BIT(0)
#define FORCE_NULL				0

#define USB_CMD_PULLDOWN_REG			(USBIN_BASE + 0x45)
#define EN_PULLDOWN_USB_IN_BIT			BIT(0)

#define TYPE_C_CFG_REG				(USBIN_BASE + 0x58)
#define BC1P2_START_ON_CC_BIT			BIT(7)

#define HVDCP_PULSE_COUNT_MAX_REG              (USBIN_BASE + 0x5B)
#define HVDCP_PULSE_COUNT_MAX_QC2_MASK         GENMASK(7, 6)
enum {
	HVDCP_PULSE_COUNT_MAX_QC2_5V = 0,
	HVDCP_PULSE_COUNT_MAX_QC2_9V = 0x40,
	HVDCP_PULSE_COUNT_MAX_QC2_12V = 0x80,
	HVDCP_PULSE_COUNT_MAX_QC2_INVALID = 0xC0
};

#define USBIN_OPTIONS_1_CFG_REG			(USBIN_BASE + 0x62)
#define HVDCP_AUTH_ALG_EN_CFG_BIT		BIT(6)
#define HVDCP_AUTONOMOUS_MODE_EN_CFG_BIT	BIT(5)
#define BC1P2_SRC_DETECT_BIT			BIT(3)
#define HVDCP_EN_BIT				BIT(2)

#define USBIN_OPTIONS_2_CFG_REG			(USBIN_BASE + 0x63)
#define DCD_TIMEOUT_SEL_BIT			BIT(5)
#define FLOAT_OPTIONS_MASK			GENMASK(2, 0)
#define FLOAT_DIS_CHGING_CFG_BIT		BIT(2)
#define SUSPEND_FLOAT_CFG_BIT			BIT(1)
#define FORCE_FLOAT_SDP_CFG_BIT			BIT(0)

#define USBIN_LOAD_CFG_REG			(USBIN_BASE + 0x65)
#define ICL_OVERRIDE_AFTER_APSD_BIT		BIT(4)
#define USBIN_AICL_STEP_TIMING_SEL_MASK		GENMASK(3, 2)
#define USBIN_IN_COLLAPSE_GF_SEL_MASK		GENMASK(1, 0)

#define USBIN_ICL_OPTIONS_REG			(USBIN_BASE + 0x66)
#define CFG_USB3P0_SEL_BIT			BIT(2)
#define	USB51_MODE_BIT				BIT(1)
#define USBIN_MODE_CHG_BIT			BIT(0)

#define USBIN_CURRENT_LIMIT_CFG_REG		(USBIN_BASE + 0x70)

#define USBIN_AICL_OPTIONS_CFG_REG		(USBIN_BASE + 0x80)
#define SUSPEND_ON_COLLAPSE_USBIN_BIT		BIT(7)
#define USBIN_AICL_PERIODIC_RERUN_EN_BIT	BIT(4)
#define USBIN_AICL_ADC_EN_BIT			BIT(3)
#define USBIN_AICL_EN_BIT			BIT(2)

#define USB_ENG_SSUPPLY_USB2_REG		(USBIN_BASE + 0xC0)
#define ENG_SSUPPLY_12V_OV_OPT_BIT		BIT(1)

#define USBIN_5V_AICL_THRESHOLD_REG		(USBIN_BASE + 0x81)
#define USBIN_CONT_AICL_THRESHOLD_REG		(USBIN_BASE + 0x84)
/********************************
 *  DCIN Peripheral Registers   *
 ********************************/

/* DCIN Interrupt Bits */
#define DCIN_PLUGIN_RT_STS_BIT			BIT(4)

#define DCIN_CMD_IL_REG				(DCIN_BASE + 0x40)
#define DCIN_SUSPEND_BIT			BIT(0)
#define DCIN_EN_OVERRIDE_BIT			BIT(1)
#define DCIN_EN_MASK				GENMASK(2, 1)

#define DCIN_CMD_PON_REG			(DCIN_BASE + 0x45)
#define DCIN_PON_BIT				BIT(0)
#define MID_CHG_BIT					BIT(1)

#define DCIN_LOAD_CFG_REG			(DCIN_BASE + 0x65)
#define INPUT_MISS_POLL_EN_BIT			BIT(5)

/********************************
 *  TYPEC Peripheral Registers  *
 ********************************/
#define TYPE_C_SNK_STATUS_REG			(TYPEC_BASE + 0x06)
#define DETECTED_SRC_TYPE_MASK			GENMASK(6, 0)
#define SNK_DAM_500MA_BIT			BIT(6)
#define SNK_DAM_1500MA_BIT			BIT(5)
#define SNK_DAM_3000MA_BIT			BIT(4)
#define SNK_RP_STD_BIT				BIT(3)
#define SNK_RP_1P5_BIT				BIT(2)
#define SNK_RP_3P0_BIT				BIT(1)
#define SNK_RP_SHORT_BIT			BIT(0)

#define TYPE_C_SRC_STATUS_REG			(TYPEC_BASE + 0x08)
#define DETECTED_SNK_TYPE_MASK			GENMASK(4, 0)
#define SRC_HIGH_BATT_BIT			BIT(5)
#define SRC_DEBUG_ACCESS_BIT			BIT(4)
#define SRC_RD_OPEN_BIT				BIT(3)
#define SRC_RD_RA_VCONN_BIT			BIT(2)
#define SRC_RA_OPEN_BIT				BIT(1)
#define AUDIO_ACCESS_RA_RA_BIT			BIT(0)

#define TYPE_C_STATE_MACHINE_STATUS_REG		(TYPEC_BASE + 0x09)
#define TYPEC_ATTACH_DETACH_STATE_BIT		BIT(5)

#define TYPE_C_MISC_STATUS_REG			(TYPEC_BASE + 0x0B)
#define TYPEC_WATER_DETECTION_STATUS_BIT	BIT(7)
#define SNK_SRC_MODE_BIT			BIT(6)
#define TYPEC_VBUS_ERROR_STATUS_BIT		BIT(4)
#define TYPEC_TCCDEBOUNCE_DONE_STATUS_BIT	BIT(3)
#define CC_ORIENTATION_BIT			BIT(1)
#define CC_ATTACHED_BIT				BIT(0)

#define LEGACY_CABLE_STATUS_REG			(TYPEC_BASE + 0x0D)
#define TYPEC_LEGACY_CABLE_STATUS_BIT		BIT(1)
#define TYPEC_NONCOMP_LEGACY_CABLE_STATUS_BIT	BIT(0)

#define TYPEC_U_USB_STATUS_REG			(TYPEC_BASE + 0x0F)
#define U_USB_GROUND_NOVBUS_BIT			BIT(6)
#define U_USB_GROUND_BIT			BIT(4)
#define U_USB_FMB1_BIT				BIT(3)
#define U_USB_FLOAT1_BIT			BIT(2)
#define U_USB_FMB2_BIT				BIT(1)
#define U_USB_FLOAT2_BIT			BIT(0)

#define TYPE_C_MODE_CFG_REG			(TYPEC_BASE + 0x44)
#define TYPEC_TRY_MODE_MASK			GENMASK(4, 3)
#define EN_TRY_SNK_BIT				BIT(4)
#define EN_TRY_SRC_BIT				BIT(3)
#define TYPEC_POWER_ROLE_CMD_MASK		GENMASK(2, 0)
#define EN_SRC_ONLY_BIT				BIT(2)
#define EN_SNK_ONLY_BIT				BIT(1)
#define TYPEC_DISABLE_CMD_BIT			BIT(0)

#define TYPE_C_VCONN_CONTROL_REG		(TYPEC_BASE + 0x46)
#define VCONN_EN_ORIENTATION_BIT		BIT(2)
#define VCONN_EN_VALUE_BIT			BIT(1)
#define VCONN_EN_SRC_BIT			BIT(0)

#define TYPE_C_CCOUT_CONTROL_REG		(TYPEC_BASE + 0x48)
#define TYPEC_CCOUT_BUFFER_EN_BIT		BIT(2)
#define TYPEC_CCOUT_VALUE_BIT			BIT(1)
#define TYPEC_CCOUT_SRC_BIT			BIT(0)

#define DEBUG_ACCESS_SRC_CFG_REG		(TYPEC_BASE + 0x4C)
#define EN_UNORIENTED_DEBUG_ACCESS_SRC_BIT	BIT(0)

#define TYPE_C_CRUDE_SENSOR_CFG_REG		(TYPEC_BASE + 0x4e)
#define EN_SRC_CRUDE_SENSOR_BIT			BIT(1)
#define EN_SNK_CRUDE_SENSOR_BIT			BIT(0)

#define TYPE_C_EXIT_STATE_CFG_REG		(TYPEC_BASE + 0x50)
#define BYPASS_VSAFE0V_DURING_ROLE_SWAP_BIT	BIT(3)
#define SEL_SRC_UPPER_REF_BIT			BIT(2)
#define EXIT_SNK_BASED_ON_CC_BIT		BIT(0)

#define TYPE_C_CURRSRC_CFG_REG			(TYPEC_BASE + 0x52)
#define TYPEC_SRC_RP_SEL_MASK			GENMASK(1, 0)
enum {
	TYPEC_SRC_RP_STD,
	TYPEC_SRC_RP_1P5A,
	TYPEC_SRC_RP_3A,
	TYPEC_SRC_RP_3A_DUPLICATE,
	TYPEC_SRC_RP_MAX_ELEMENTS
};

#define TYPE_C_INTERRUPT_EN_CFG_1_REG			(TYPEC_BASE + 0x5E)
#define TYPEC_LEGACY_CABLE_INT_EN_BIT			BIT(7)
#define TYPEC_NONCOMPLIANT_LEGACY_CABLE_INT_EN_BIT	BIT(6)
#define TYPEC_TRYSOURCE_DETECT_INT_EN_BIT		BIT(5)
#define TYPEC_TRYSINK_DETECT_INT_EN_BIT			BIT(4)
#define TYPEC_CCOUT_DETACH_INT_EN_BIT			BIT(3)
#define TYPEC_CCOUT_ATTACH_INT_EN_BIT			BIT(2)
#define TYPEC_VBUS_DEASSERT_INT_EN_BIT			BIT(1)
#define TYPEC_VBUS_ASSERT_INT_EN_BIT			BIT(0)

#define TYPE_C_INTERRUPT_EN_CFG_2_REG		(TYPEC_BASE + 0x60)
#define TYPEC_SRC_BATT_HPWR_INT_EN_BIT		BIT(6)
#define MICRO_USB_STATE_CHANGE_INT_EN_BIT	BIT(5)
#define TYPEC_STATE_MACHINE_CHANGE_INT_EN_BIT	BIT(4)
#define TYPEC_DEBUG_ACCESS_DETECT_INT_EN_BIT	BIT(3)
#define TYPEC_WATER_DETECTION_INT_EN_BIT	BIT(2)
#define TYPEC_VBUS_ERROR_INT_EN_BIT		BIT(1)
#define TYPEC_DEBOUNCE_DONE_INT_EN_BIT		BIT(0)

#define TYPE_C_DEBOUNCE_OPTION_REG		(TYPEC_BASE + 0x62)
#define REDUCE_TCCDEBOUNCE_TO_2MS_BIT		BIT(2)

#define TYPE_C_SBU_CFG_REG			(TYPEC_BASE + 0x6A)
#define SEL_SBU1_ISRC_VAL			0x04
#define SEL_SBU2_ISRC_VAL			0x01

#define TYPEC_U_USB_CFG_REG			(TYPEC_BASE + 0x70)
#define EN_MICRO_USB_FACTORY_MODE_BIT		BIT(1)
#define EN_MICRO_USB_MODE_BIT			BIT(0)

#define PMI632_TYPEC_U_USB_WATER_PROTECTION_CFG_REG	(TYPEC_BASE + 0x72)
#define TYPEC_U_USB_WATER_PROTECTION_CFG_REG		(TYPEC_BASE + 0x73)
#define EN_MICRO_USB_WATER_PROTECTION_BIT		BIT(4)
#define MICRO_USB_DETECTION_ON_TIME_CFG_MASK		GENMASK(3, 2)
#define MICRO_USB_DETECTION_PERIOD_CFG_MASK		GENMASK(1, 0)

#define PMI632_TYPEC_MICRO_USB_MODE_REG		(TYPEC_BASE + 0x73)
#define MICRO_USB_MODE_ONLY_BIT			BIT(0)
/********************************
 *  MISC Peripheral Registers  *
 ********************************/
#define TEMP_RANGE_STATUS_REG			(MISC_BASE + 0x06)
#define THERM_REG_ACTIVE_BIT			BIT(6)
#define TLIM_BIT				BIT(5)
#define TEMP_RANGE_MASK				GENMASK(4, 1)
#define ALERT_LEVEL_BIT				BIT(4)
#define TEMP_ABOVE_RANGE_BIT			BIT(3)
#define TEMP_WITHIN_RANGE_BIT			BIT(2)
#define TEMP_BELOW_RANGE_BIT			BIT(1)
#define THERMREG_DISABLED_BIT			BIT(0)

#define DIE_TEMP_STATUS_REG			(MISC_BASE + 0x07)
#define DIE_TEMP_SHDN_BIT			BIT(3)
#define DIE_TEMP_RST_BIT			BIT(2)
#define DIE_TEMP_UB_BIT				BIT(1)
#define DIE_TEMP_LB_BIT				BIT(0)

#define SKIN_TEMP_STATUS_REG			(MISC_BASE + 0x08)
#define SKIN_TEMP_SHDN_BIT			BIT(3)
#define SKIN_TEMP_RST_BIT			BIT(2)
#define SKIN_TEMP_UB_BIT			BIT(1)
#define SKIN_TEMP_LB_BIT			BIT(0)

#define CONNECTOR_TEMP_STATUS_REG		(MISC_BASE + 0x09)
#define CONNECTOR_TEMP_SHDN_BIT			BIT(3)
#define CONNECTOR_TEMP_RST_BIT			BIT(2)
#define CONNECTOR_TEMP_UB_BIT			BIT(1)
#define CONNECTOR_TEMP_LB_BIT			BIT(0)

#define SMB_TEMP_STATUS_REG			(MISC_BASE + 0x0A)
#define SMB_TEMP_SHDN_BIT			BIT(3)
#define SMB_TEMP_RST_BIT			BIT(2)
#define SMB_TEMP_UB_BIT				BIT(1)
#define SMB_TEMP_LB_BIT				BIT(0)

#define BARK_BITE_WDOG_PET_REG			(MISC_BASE + 0x43)
#define BARK_BITE_WDOG_PET_BIT			BIT(0)

#define AICL_CMD_REG				(MISC_BASE + 0x44)
#define RESTART_AICL_BIT			BIT(1)
#define RERUN_AICL_BIT				BIT(0)

#define MISC_SMB_EN_CMD_REG			(MISC_BASE + 0x48)
#define SMB_EN_OVERRIDE_VALUE_BIT		BIT(4)
#define SMB_EN_OVERRIDE_BIT			BIT(3)
#define EN_STAT_CMD_BIT				BIT(2)
#define EN_CP_FPF_CMD_BIT			BIT(1)
#define EN_CP_CMD_BIT				BIT(0)

#define WD_CFG_REG				(MISC_BASE + 0x51)
#define WATCHDOG_TRIGGER_AFP_EN_BIT		BIT(7)
#define BARK_WDOG_INT_EN_BIT			BIT(6)
#define WDOG_TIMER_EN_ON_PLUGIN_BIT		BIT(1)

#define SNARL_BARK_BITE_WD_CFG_REG		(MISC_BASE + 0x53)
#define BITE_WDOG_DISABLE_CHARGING_CFG_BIT	BIT(7)
#define SNARL_WDOG_TIMEOUT_MASK                 GENMASK(6, 4)
#define SNARL_WDOG_TIMEOUT_SHIFT		4
#define SNARL_WDOG_TMOUT_62P5MS			0x00
#define SNARL_WDOG_TMOUT_1S			0x40
#define SNARL_WDOG_TMOUT_8S			0x70
#define BARK_WDOG_TIMEOUT_MASK			GENMASK(3, 2)
#define BARK_WDOG_TIMEOUT_SHIFT			2
#define BITE_WDOG_TIMEOUT_MASK			GENMASK(1, 0)
#define BITE_WDOG_TIMEOUT_8S			0x3
#define MIN_WD_BARK_TIME			16

#define AICL_RERUN_TIME_CFG_REG			(MISC_BASE + 0x61)
#define AICL_RERUN_TIME_12S_VAL			0x01

#define MISC_THERMREG_SRC_CFG_REG		(MISC_BASE + 0x70)
#define THERMREG_SW_ICL_ADJUST_BIT		BIT(7)
#define DIE_ADC_SEL_BIT				BIT(6)
#define THERMREG_SMB_ADC_SRC_EN_BIT		BIT(5)
#define THERMREG_CONNECTOR_ADC_SRC_EN_BIT	BIT(4)
#define SKIN_ADC_CFG_BIT			BIT(3)
#define THERMREG_SKIN_ADC_SRC_EN_BIT		BIT(2)
#define THERMREG_DIE_ADC_SRC_EN_BIT		BIT(1)
#define THERMREG_DIE_CMP_SRC_EN_BIT		BIT(0)

#define MISC_SMB_CFG_REG			(MISC_BASE + 0x90)
#define SMB_EN_SEL_BIT				BIT(4)
#define CP_EN_POLARITY_CFG_BIT			BIT(3)
#define STAT_POLARITY_CFG_BIT			BIT(2)
#define STAT_FUNCTION_CFG_BIT			BIT(1)
#define STAT_IRQ_PULSING_EN_BIT			BIT(0)

#define DIE_REG_H_THRESHOLD_MSB_REG		(MISC_BASE + 0xA0)

#define SMB_REG_H_THRESHOLD_MSB_REG		(MISC_BASE + 0XBC)

/* SDAM regs */
#define MISC_PBS_RT_STS_REG			(MISC_PBS_BASE + 0x10)
#define PULSE_SKIP_IRQ_BIT			BIT(4)
#endif /* __SMB5_CHARGER_REG_H */
