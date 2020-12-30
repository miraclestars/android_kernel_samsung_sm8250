// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2016, 2018-2020, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/pinctrl.h>

#include "pinctrl-msm.h"

#define FUNCTION(fname)					\
	[msm_mux_##fname] = {		                \
		.name = #fname,				\
		.groups = fname##_groups,               \
		.ngroups = ARRAY_SIZE(fname##_groups),	\
	}

#define NORTH	0x00900000
#define CENTER	0x00500000
#define SOUTH	0x00100000
#define REG_SIZE 0x1000
#define PINGROUP(id, base, f1, f2, f3, f4, f5, f6, f7, f8, f9)	\
	{					        \
		.name = "gpio" #id,			\
		.pins = gpio##id##_pins,		\
		.npins = (unsigned int)ARRAY_SIZE(gpio##id##_pins),	\
		.funcs = (int[]){			\
			msm_mux_gpio, /* gpio mode */	\
			msm_mux_##f1,			\
			msm_mux_##f2,			\
			msm_mux_##f3,			\
			msm_mux_##f4,			\
			msm_mux_##f5,			\
			msm_mux_##f6,			\
			msm_mux_##f7,			\
			msm_mux_##f8,			\
			msm_mux_##f9			\
		},				        \
		.nfuncs = 10,				\
		.ctl_reg = base + REG_SIZE * id,	\
		.io_reg = base + 0x4 + REG_SIZE * id,		\
		.intr_cfg_reg = base + 0x8 + REG_SIZE * id,		\
		.intr_status_reg = base + 0xc + REG_SIZE * id,	\
		.intr_target_reg = base + 0x8 + REG_SIZE * id,	\
		.mux_bit = 2,			\
		.pull_bit = 0,			\
		.drv_bit = 6,			\
		.oe_bit = 9,			\
		.in_bit = 0,			\
		.out_bit = 1,			\
		.intr_enable_bit = 0,		\
		.intr_status_bit = 0,		\
		.intr_target_bit = 5,		\
		.intr_target_kpss_val = 3,	\
		.intr_raw_status_bit = 4,	\
		.intr_polarity_bit = 1,		\
		.intr_detection_bit = 2,	\
		.intr_detection_width = 2,	\
	}

#define SDC_QDSD_PINGROUP(pg_name, ctl, pull, drv)	\
	{					        \
		.name = #pg_name,			\
		.pins = pg_name##_pins,			\
		.npins = (unsigned int)ARRAY_SIZE(pg_name##_pins),	\
		.ctl_reg = ctl,				\
		.io_reg = 0,				\
		.intr_cfg_reg = 0,			\
		.intr_status_reg = 0,			\
		.intr_target_reg = 0,			\
		.mux_bit = -1,				\
		.pull_bit = pull,			\
		.drv_bit = drv,				\
		.oe_bit = -1,				\
		.in_bit = -1,				\
		.out_bit = -1,				\
		.intr_enable_bit = -1,			\
		.intr_status_bit = -1,			\
		.intr_target_bit = -1,			\
		.intr_raw_status_bit = -1,		\
		.intr_polarity_bit = -1,		\
		.intr_detection_bit = -1,		\
		.intr_detection_width = -1,		\
	}

#define UFS_RESET(pg_name, offset)			\
	{						\
		.name = #pg_name,			\
		.pins = pg_name##_pins,			\
		.npins = (unsigned int)ARRAY_SIZE(pg_name##_pins),	\
		.ctl_reg = offset,			\
		.io_reg = offset + 0x4,			\
		.intr_cfg_reg = 0,			\
		.intr_status_reg = 0,			\
		.intr_target_reg = 0,			\
		.mux_bit = -1,				\
		.pull_bit = 3,				\
		.drv_bit = 0,				\
		.oe_bit = -1,				\
		.in_bit = -1,				\
		.out_bit = 0,				\
		.intr_enable_bit = -1,			\
		.intr_status_bit = -1,			\
		.intr_target_bit = -1,			\
		.intr_raw_status_bit = -1,		\
		.intr_polarity_bit = -1,		\
		.intr_detection_bit = -1,		\
		.intr_detection_width = -1,		\
	}
static const struct pinctrl_pin_desc sdm660_pins[] = {
	PINCTRL_PIN(0, "GPIO_0"),
	PINCTRL_PIN(1, "GPIO_1"),
	PINCTRL_PIN(2, "GPIO_2"),
	PINCTRL_PIN(3, "GPIO_3"),
	PINCTRL_PIN(4, "GPIO_4"),
	PINCTRL_PIN(5, "GPIO_5"),
	PINCTRL_PIN(6, "GPIO_6"),
	PINCTRL_PIN(7, "GPIO_7"),
	PINCTRL_PIN(8, "GPIO_8"),
	PINCTRL_PIN(9, "GPIO_9"),
	PINCTRL_PIN(10, "GPIO_10"),
	PINCTRL_PIN(11, "GPIO_11"),
	PINCTRL_PIN(12, "GPIO_12"),
	PINCTRL_PIN(13, "GPIO_13"),
	PINCTRL_PIN(14, "GPIO_14"),
	PINCTRL_PIN(15, "GPIO_15"),
	PINCTRL_PIN(16, "GPIO_16"),
	PINCTRL_PIN(17, "GPIO_17"),
	PINCTRL_PIN(18, "GPIO_18"),
	PINCTRL_PIN(19, "GPIO_19"),
	PINCTRL_PIN(20, "GPIO_20"),
	PINCTRL_PIN(21, "GPIO_21"),
	PINCTRL_PIN(22, "GPIO_22"),
	PINCTRL_PIN(23, "GPIO_23"),
	PINCTRL_PIN(24, "GPIO_24"),
	PINCTRL_PIN(25, "GPIO_25"),
	PINCTRL_PIN(26, "GPIO_26"),
	PINCTRL_PIN(27, "GPIO_27"),
	PINCTRL_PIN(28, "GPIO_28"),
	PINCTRL_PIN(29, "GPIO_29"),
	PINCTRL_PIN(30, "GPIO_30"),
	PINCTRL_PIN(31, "GPIO_31"),
	PINCTRL_PIN(32, "GPIO_32"),
	PINCTRL_PIN(33, "GPIO_33"),
	PINCTRL_PIN(34, "GPIO_34"),
	PINCTRL_PIN(35, "GPIO_35"),
	PINCTRL_PIN(36, "GPIO_36"),
	PINCTRL_PIN(37, "GPIO_37"),
	PINCTRL_PIN(38, "GPIO_38"),
	PINCTRL_PIN(39, "GPIO_39"),
	PINCTRL_PIN(40, "GPIO_40"),
	PINCTRL_PIN(41, "GPIO_41"),
	PINCTRL_PIN(42, "GPIO_42"),
	PINCTRL_PIN(43, "GPIO_43"),
	PINCTRL_PIN(44, "GPIO_44"),
	PINCTRL_PIN(45, "GPIO_45"),
	PINCTRL_PIN(46, "GPIO_46"),
	PINCTRL_PIN(47, "GPIO_47"),
	PINCTRL_PIN(48, "GPIO_48"),
	PINCTRL_PIN(49, "GPIO_49"),
	PINCTRL_PIN(50, "GPIO_50"),
	PINCTRL_PIN(51, "GPIO_51"),
	PINCTRL_PIN(52, "GPIO_52"),
	PINCTRL_PIN(53, "GPIO_53"),
	PINCTRL_PIN(54, "GPIO_54"),
	PINCTRL_PIN(55, "GPIO_55"),
	PINCTRL_PIN(56, "GPIO_56"),
	PINCTRL_PIN(57, "GPIO_57"),
	PINCTRL_PIN(58, "GPIO_58"),
	PINCTRL_PIN(59, "GPIO_59"),
	PINCTRL_PIN(60, "GPIO_60"),
	PINCTRL_PIN(61, "GPIO_61"),
	PINCTRL_PIN(62, "GPIO_62"),
	PINCTRL_PIN(63, "GPIO_63"),
	PINCTRL_PIN(64, "GPIO_64"),
	PINCTRL_PIN(65, "GPIO_65"),
	PINCTRL_PIN(66, "GPIO_66"),
	PINCTRL_PIN(67, "GPIO_67"),
	PINCTRL_PIN(68, "GPIO_68"),
	PINCTRL_PIN(69, "GPIO_69"),
	PINCTRL_PIN(70, "GPIO_70"),
	PINCTRL_PIN(71, "GPIO_71"),
	PINCTRL_PIN(72, "GPIO_72"),
	PINCTRL_PIN(73, "GPIO_73"),
	PINCTRL_PIN(74, "GPIO_74"),
	PINCTRL_PIN(75, "GPIO_75"),
	PINCTRL_PIN(76, "GPIO_76"),
	PINCTRL_PIN(77, "GPIO_77"),
	PINCTRL_PIN(78, "GPIO_78"),
	PINCTRL_PIN(79, "GPIO_79"),
	PINCTRL_PIN(80, "GPIO_80"),
	PINCTRL_PIN(81, "GPIO_81"),
	PINCTRL_PIN(82, "GPIO_82"),
	PINCTRL_PIN(83, "GPIO_83"),
	PINCTRL_PIN(84, "GPIO_84"),
	PINCTRL_PIN(85, "GPIO_85"),
	PINCTRL_PIN(86, "GPIO_86"),
	PINCTRL_PIN(87, "GPIO_87"),
	PINCTRL_PIN(88, "GPIO_88"),
	PINCTRL_PIN(89, "GPIO_89"),
	PINCTRL_PIN(90, "GPIO_90"),
	PINCTRL_PIN(91, "GPIO_91"),
	PINCTRL_PIN(92, "GPIO_92"),
	PINCTRL_PIN(93, "GPIO_93"),
	PINCTRL_PIN(94, "GPIO_94"),
	PINCTRL_PIN(95, "GPIO_95"),
	PINCTRL_PIN(96, "GPIO_96"),
	PINCTRL_PIN(97, "GPIO_97"),
	PINCTRL_PIN(98, "GPIO_98"),
	PINCTRL_PIN(99, "GPIO_99"),
	PINCTRL_PIN(100, "GPIO_100"),
	PINCTRL_PIN(101, "GPIO_101"),
	PINCTRL_PIN(102, "GPIO_102"),
	PINCTRL_PIN(103, "GPIO_103"),
	PINCTRL_PIN(104, "GPIO_104"),
	PINCTRL_PIN(105, "GPIO_105"),
	PINCTRL_PIN(106, "GPIO_106"),
	PINCTRL_PIN(107, "GPIO_107"),
	PINCTRL_PIN(108, "GPIO_108"),
	PINCTRL_PIN(109, "GPIO_109"),
	PINCTRL_PIN(110, "GPIO_110"),
	PINCTRL_PIN(111, "GPIO_111"),
	PINCTRL_PIN(112, "GPIO_112"),
	PINCTRL_PIN(113, "GPIO_113"),
	PINCTRL_PIN(114, "SDC1_CLK"),
	PINCTRL_PIN(115, "SDC1_CMD"),
	PINCTRL_PIN(116, "SDC1_DATA"),
	PINCTRL_PIN(117, "SDC2_CLK"),
	PINCTRL_PIN(118, "SDC2_CMD"),
	PINCTRL_PIN(119, "SDC2_DATA"),
	PINCTRL_PIN(120, "SDC1_RCLK"),
	PINCTRL_PIN(121, "UFS_RESET"),
};

#define DECLARE_MSM_GPIO_PINS(pin) \
	static const unsigned int gpio##pin##_pins[] = { pin }
DECLARE_MSM_GPIO_PINS(0);
DECLARE_MSM_GPIO_PINS(1);
DECLARE_MSM_GPIO_PINS(2);
DECLARE_MSM_GPIO_PINS(3);
DECLARE_MSM_GPIO_PINS(4);
DECLARE_MSM_GPIO_PINS(5);
DECLARE_MSM_GPIO_PINS(6);
DECLARE_MSM_GPIO_PINS(7);
DECLARE_MSM_GPIO_PINS(8);
DECLARE_MSM_GPIO_PINS(9);
DECLARE_MSM_GPIO_PINS(10);
DECLARE_MSM_GPIO_PINS(11);
DECLARE_MSM_GPIO_PINS(12);
DECLARE_MSM_GPIO_PINS(13);
DECLARE_MSM_GPIO_PINS(14);
DECLARE_MSM_GPIO_PINS(15);
DECLARE_MSM_GPIO_PINS(16);
DECLARE_MSM_GPIO_PINS(17);
DECLARE_MSM_GPIO_PINS(18);
DECLARE_MSM_GPIO_PINS(19);
DECLARE_MSM_GPIO_PINS(20);
DECLARE_MSM_GPIO_PINS(21);
DECLARE_MSM_GPIO_PINS(22);
DECLARE_MSM_GPIO_PINS(23);
DECLARE_MSM_GPIO_PINS(24);
DECLARE_MSM_GPIO_PINS(25);
DECLARE_MSM_GPIO_PINS(26);
DECLARE_MSM_GPIO_PINS(27);
DECLARE_MSM_GPIO_PINS(28);
DECLARE_MSM_GPIO_PINS(29);
DECLARE_MSM_GPIO_PINS(30);
DECLARE_MSM_GPIO_PINS(31);
DECLARE_MSM_GPIO_PINS(32);
DECLARE_MSM_GPIO_PINS(33);
DECLARE_MSM_GPIO_PINS(34);
DECLARE_MSM_GPIO_PINS(35);
DECLARE_MSM_GPIO_PINS(36);
DECLARE_MSM_GPIO_PINS(37);
DECLARE_MSM_GPIO_PINS(38);
DECLARE_MSM_GPIO_PINS(39);
DECLARE_MSM_GPIO_PINS(40);
DECLARE_MSM_GPIO_PINS(41);
DECLARE_MSM_GPIO_PINS(42);
DECLARE_MSM_GPIO_PINS(43);
DECLARE_MSM_GPIO_PINS(44);
DECLARE_MSM_GPIO_PINS(45);
DECLARE_MSM_GPIO_PINS(46);
DECLARE_MSM_GPIO_PINS(47);
DECLARE_MSM_GPIO_PINS(48);
DECLARE_MSM_GPIO_PINS(49);
DECLARE_MSM_GPIO_PINS(50);
DECLARE_MSM_GPIO_PINS(51);
DECLARE_MSM_GPIO_PINS(52);
DECLARE_MSM_GPIO_PINS(53);
DECLARE_MSM_GPIO_PINS(54);
DECLARE_MSM_GPIO_PINS(55);
DECLARE_MSM_GPIO_PINS(56);
DECLARE_MSM_GPIO_PINS(57);
DECLARE_MSM_GPIO_PINS(58);
DECLARE_MSM_GPIO_PINS(59);
DECLARE_MSM_GPIO_PINS(60);
DECLARE_MSM_GPIO_PINS(61);
DECLARE_MSM_GPIO_PINS(62);
DECLARE_MSM_GPIO_PINS(63);
DECLARE_MSM_GPIO_PINS(64);
DECLARE_MSM_GPIO_PINS(65);
DECLARE_MSM_GPIO_PINS(66);
DECLARE_MSM_GPIO_PINS(67);
DECLARE_MSM_GPIO_PINS(68);
DECLARE_MSM_GPIO_PINS(69);
DECLARE_MSM_GPIO_PINS(70);
DECLARE_MSM_GPIO_PINS(71);
DECLARE_MSM_GPIO_PINS(72);
DECLARE_MSM_GPIO_PINS(73);
DECLARE_MSM_GPIO_PINS(74);
DECLARE_MSM_GPIO_PINS(75);
DECLARE_MSM_GPIO_PINS(76);
DECLARE_MSM_GPIO_PINS(77);
DECLARE_MSM_GPIO_PINS(78);
DECLARE_MSM_GPIO_PINS(79);
DECLARE_MSM_GPIO_PINS(80);
DECLARE_MSM_GPIO_PINS(81);
DECLARE_MSM_GPIO_PINS(82);
DECLARE_MSM_GPIO_PINS(83);
DECLARE_MSM_GPIO_PINS(84);
DECLARE_MSM_GPIO_PINS(85);
DECLARE_MSM_GPIO_PINS(86);
DECLARE_MSM_GPIO_PINS(87);
DECLARE_MSM_GPIO_PINS(88);
DECLARE_MSM_GPIO_PINS(89);
DECLARE_MSM_GPIO_PINS(90);
DECLARE_MSM_GPIO_PINS(91);
DECLARE_MSM_GPIO_PINS(92);
DECLARE_MSM_GPIO_PINS(93);
DECLARE_MSM_GPIO_PINS(94);
DECLARE_MSM_GPIO_PINS(95);
DECLARE_MSM_GPIO_PINS(96);
DECLARE_MSM_GPIO_PINS(97);
DECLARE_MSM_GPIO_PINS(98);
DECLARE_MSM_GPIO_PINS(99);
DECLARE_MSM_GPIO_PINS(100);
DECLARE_MSM_GPIO_PINS(101);
DECLARE_MSM_GPIO_PINS(102);
DECLARE_MSM_GPIO_PINS(103);
DECLARE_MSM_GPIO_PINS(104);
DECLARE_MSM_GPIO_PINS(105);
DECLARE_MSM_GPIO_PINS(106);
DECLARE_MSM_GPIO_PINS(107);
DECLARE_MSM_GPIO_PINS(108);
DECLARE_MSM_GPIO_PINS(109);
DECLARE_MSM_GPIO_PINS(110);
DECLARE_MSM_GPIO_PINS(111);
DECLARE_MSM_GPIO_PINS(112);
DECLARE_MSM_GPIO_PINS(113);

static const unsigned int sdc1_clk_pins[] = { 114 };
static const unsigned int sdc1_cmd_pins[] = { 115 };
static const unsigned int sdc1_data_pins[] = { 116 };
static const unsigned int sdc2_clk_pins[] = { 117 };
static const unsigned int sdc2_cmd_pins[] = { 118 };
static const unsigned int sdc2_data_pins[] = { 119 };
static const unsigned int sdc1_rclk_pins[] = { 120 };
static const unsigned int ufs_reset_pins[] = { 121 };

enum sdm660_functions {
	msm_mux_blsp_spi1,
	msm_mux_gpio,
	msm_mux_blsp_uim1,
	msm_mux_tgu_ch0,
	msm_mux_qdss_gpio4,
	msm_mux_atest_gpsadc1,
	msm_mux_blsp_uart1,
	msm_mux_SMB_STAT,
	msm_mux_phase_flag14,
	msm_mux_blsp_i2c2,
	msm_mux_phase_flag31,
	msm_mux_blsp_spi3,
	msm_mux_blsp_spi3_cs1,
	msm_mux_blsp_spi3_cs2,
	msm_mux_wlan1_adc1,
	msm_mux_atest_usb13,
	msm_mux_tgu_ch1,
	msm_mux_qdss_gpio5,
	msm_mux_atest_gpsadc0,
	msm_mux_blsp_i2c1,
	msm_mux_ddr_bist,
	msm_mux_atest_tsens2,
	msm_mux_atest_usb1,
	msm_mux_blsp_spi2,
	msm_mux_blsp_uim2,
	msm_mux_phase_flag3,
	msm_mux_bimc_dte1,
	msm_mux_wlan1_adc0,
	msm_mux_atest_usb12,
	msm_mux_bimc_dte0,
	msm_mux_blsp_i2c3,
	msm_mux_wlan2_adc1,
	msm_mux_atest_usb11,
	msm_mux_dbg_out,
	msm_mux_wlan2_adc0,
	msm_mux_atest_usb10,
	msm_mux_RCM_MARKER,
	msm_mux_blsp_spi4,
	msm_mux_pri_mi2s,
	msm_mux_phase_flag26,
	msm_mux_qdss_cti0_a,
	msm_mux_qdss_cti0_b,
	msm_mux_qdss_cti1_a,
	msm_mux_qdss_cti1_b,
	msm_mux_DP_HOT,
	msm_mux_pri_mi2s_ws,
	msm_mux_phase_flag27,
	msm_mux_blsp_i2c4,
	msm_mux_phase_flag28,
	msm_mux_blsp_uart5,
	msm_mux_blsp_spi5,
	msm_mux_blsp_uim5,
	msm_mux_phase_flag5,
	msm_mux_blsp_i2c5,
	msm_mux_blsp_spi6,
	msm_mux_blsp_uart2,
	msm_mux_blsp_uim6,
	msm_mux_phase_flag11,
	msm_mux_vsense_data0,
	msm_mux_blsp_i2c6,
	msm_mux_phase_flag12,
	msm_mux_vsense_data1,
	msm_mux_phase_flag13,
	msm_mux_vsense_mode,
	msm_mux_blsp_spi7,
	msm_mux_blsp_uart6_a,
	msm_mux_blsp_uart6_b,
	msm_mux_sec_mi2s,
	msm_mux_sndwire_clk,
	msm_mux_phase_flag17,
	msm_mux_vsense_clkout,
	msm_mux_sndwire_data,
	msm_mux_phase_flag18,
	msm_mux_WSA_SPKR,
	msm_mux_blsp_i2c7,
	msm_mux_phase_flag19,
	msm_mux_vfr_1,
	msm_mux_phase_flag20,
	msm_mux_NFC_INT,
	msm_mux_blsp_spi8_cs1,
	msm_mux_blsp_spi8_cs2,
	msm_mux_m_voc,
	msm_mux_phase_flag21,
	msm_mux_NFC_EN,
	msm_mux_phase_flag22,
	msm_mux_NFC_DWL,
	msm_mux_blsp_i2c8_a,
	msm_mux_blsp_i2c8_b,
	msm_mux_phase_flag23,
	msm_mux_NFC_ESE,
	msm_mux_pwr_modem,
	msm_mux_phase_flag24,
	msm_mux_qdss_gpio,
	msm_mux_cam_mclk,
	msm_mux_pwr_nav,
	msm_mux_qdss_gpio0,
	msm_mux_qspi_data0,
	msm_mux_pwr_crypto,
	msm_mux_qdss_gpio1,
	msm_mux_qspi_data1,
	msm_mux_agera_pll,
	msm_mux_qdss_gpio2,
	msm_mux_qspi_data2,
	msm_mux_jitter_bist,
	msm_mux_qdss_gpio3,
	msm_mux_qdss_gpio7,
	msm_mux_FL_R3LED,
	msm_mux_CCI_TIMER0,
	msm_mux_FL_STROBE,
	msm_mux_CCI_TIMER1,
	msm_mux_CAM_LDO1,
	msm_mux_mdss_vsync0,
	msm_mux_mdss_vsync1,
	msm_mux_mdss_vsync2,
	msm_mux_mdss_vsync3,
	msm_mux_qdss_gpio9,
	msm_mux_CAM_IRQ,
	msm_mux_atest_usb2,
	msm_mux_cci_i2c,
	msm_mux_pll_bypassnl,
	msm_mux_atest_tsens,
	msm_mux_atest_usb21,
	msm_mux_pll_reset,
	msm_mux_atest_usb23,
	msm_mux_qdss_gpio6,
	msm_mux_CCI_TIMER3,
	msm_mux_CCI_ASYNC,
	msm_mux_qspi_cs,
	msm_mux_qdss_gpio10,
	msm_mux_CAM3_STANDBY,
	msm_mux_CCI_TIMER4,
	msm_mux_qdss_gpio11,
	msm_mux_CAM_LDO2,
	msm_mux_cci_async,
	msm_mux_qdss_gpio12,
	msm_mux_CAM0_RST,
	msm_mux_qdss_gpio13,
	msm_mux_CAM1_RST,
	msm_mux_qspi_clk,
	msm_mux_phase_flag30,
	msm_mux_qdss_gpio14,
	msm_mux_qspi_resetn,
	msm_mux_phase_flag1,
	msm_mux_qdss_gpio15,
	msm_mux_CAM0_STANDBY,
	msm_mux_phase_flag2,
	msm_mux_CAM1_STANDBY,
	msm_mux_phase_flag9,
	msm_mux_CAM2_STANDBY,
	msm_mux_qspi_data3,
	msm_mux_phase_flag15,
	msm_mux_qdss_gpio8,
	msm_mux_CAM3_RST,
	msm_mux_CCI_TIMER2,
	msm_mux_phase_flag16,
	msm_mux_LCD0_RESET,
	msm_mux_phase_flag6,
	msm_mux_SD_CARD,
	msm_mux_phase_flag29,
	msm_mux_DP_EN,
	msm_mux_phase_flag25,
	msm_mux_USBC_ORIENTATION,
	msm_mux_phase_flag10,
	msm_mux_atest_usb20,
	msm_mux_gcc_gp1,
	msm_mux_phase_flag4,
	msm_mux_atest_usb22,
	msm_mux_USB_PHY,
	msm_mux_gcc_gp2,
	msm_mux_atest_char,
	msm_mux_mdp_vsync,
	msm_mux_gcc_gp3,
	msm_mux_atest_char3,
	msm_mux_FORCE_TOUCH,
	msm_mux_cri_trng0,
	msm_mux_atest_char2,
	msm_mux_cri_trng1,
	msm_mux_atest_char1,
	msm_mux_AUDIO_USBC,
	msm_mux_audio_ref,
	msm_mux_MDP_VSYNC,
	msm_mux_cri_trng,
	msm_mux_atest_char0,
	msm_mux_US_EURO,
	msm_mux_LCD_BACKLIGHT,
	msm_mux_blsp_spi8_a,
	msm_mux_blsp_spi8_b,
	msm_mux_sp_cmu,
	msm_mux_nav_pps_a,
	msm_mux_nav_pps_b,
	msm_mux_nav_pps_c,
	msm_mux_gps_tx_a,
	msm_mux_gps_tx_b,
	msm_mux_gps_tx_c,
	msm_mux_adsp_ext,
	msm_mux_TS_RESET,
	msm_mux_ssc_irq,
	msm_mux_isense_dbg,
	msm_mux_phase_flag0,
	msm_mux_phase_flag7,
	msm_mux_phase_flag8,
	msm_mux_tsense_pwm1,
	msm_mux_tsense_pwm2,
	msm_mux_SENSOR_RST,
	msm_mux_WMSS_RESETN,
	msm_mux_HAPTICS_PWM,
	msm_mux_GPS_eLNA,
	msm_mux_mss_lte,
	msm_mux_uim2_data,
	msm_mux_uim2_clk,
	msm_mux_uim2_reset,
	msm_mux_uim2_present,
	msm_mux_uim1_data,
	msm_mux_uim1_clk,
	msm_mux_uim1_reset,
	msm_mux_uim1_present,
	msm_mux_uim_batt,
	msm_mux_pa_indicator,
	msm_mux_ldo_en,
	msm_mux_ldo_update,
	msm_mux_qlink_request,
	msm_mux_qlink_enable,
	msm_mux_prng_rosc,
	msm_mux_LCD_PWR,
	msm_mux_NA,
};

static const char * const blsp_spi1_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3", "gpio46",
};
static const char * const gpio_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3", "gpio4", "gpio5", "gpio6", "gpio7",
	"gpio8", "gpio9", "gpio10", "gpio11", "gpio12", "gpio13", "gpio14",
	"gpio15", "gpio16", "gpio17", "gpio18", "gpio19", "gpio20", "gpio21",
	"gpio22", "gpio23", "gpio24", "gpio25", "gpio26", "gpio27", "gpio28",
	"gpio29", "gpio30", "gpio31", "gpio32", "gpio33", "gpio34", "gpio35",
	"gpio36", "gpio37", "gpio38", "gpio39", "gpio40", "gpio41", "gpio42",
	"gpio43", "gpio44", "gpio45", "gpio46", "gpio47", "gpio48", "gpio49",
	"gpio50", "gpio51", "gpio52", "gpio53", "gpio54", "gpio55", "gpio56",
	"gpio57", "gpio58", "gpio59", "gpio60", "gpio61", "gpio62", "gpio63",
	"gpio64", "gpio65", "gpio66", "gpio67", "gpio68", "gpio69", "gpio70",
	"gpio71", "gpio72", "gpio73", "gpio74", "gpio75", "gpio76", "gpio77",
	"gpio78", "gpio79", "gpio80", "gpio81", "gpio82", "gpio83", "gpio84",
	"gpio85", "gpio86", "gpio87", "gpio88", "gpio89", "gpio90", "gpio91",
	"gpio92", "gpio93", "gpio94", "gpio95", "gpio96", "gpio97", "gpio98",
	"gpio99", "gpio100", "gpio101", "gpio102", "gpio103", "gpio104",
	"gpio105", "gpio106", "gpio107", "gpio108", "gpio109", "gpio110",
	"gpio111", "gpio112", "gpio113",
};
static const char * const blsp_uim1_groups[] = {
	"gpio0", "gpio1",
};
static const char * const tgu_ch0_groups[] = {
	"gpio0",
};
static const char * const qdss_gpio4_groups[] = {
	"gpio0", "gpio36",
};
static const char * const atest_gpsadc1_groups[] = {
	"gpio0",
};
static const char * const blsp_uart1_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3",
};
static const char * const SMB_STAT_groups[] = {
	"gpio5",
};
static const char * const phase_flag14_groups[] = {
	"gpio5",
};
static const char * const blsp_i2c2_groups[] = {
	"gpio6", "gpio7",
};
static const char * const phase_flag31_groups[] = {
	"gpio6",
};
static const char * const blsp_spi3_groups[] = {
	"gpio8", "gpio9", "gpio10", "gpio11",
};
static const char * const blsp_spi3_cs1_groups[] = {
	"gpio30",
};
static const char * const blsp_spi3_cs2_groups[] = {
	"gpio65",
};
static const char * const wlan1_adc1_groups[] = {
	"gpio8",
};
static const char * const atest_usb13_groups[] = {
	"gpio8",
};
static const char * const tgu_ch1_groups[] = {
	"gpio1",
};
static const char * const qdss_gpio5_groups[] = {
	"gpio1", "gpio37",
};
static const char * const atest_gpsadc0_groups[] = {
	"gpio1",
};
static const char * const blsp_i2c1_groups[] = {
	"gpio2", "gpio3",
};
static const char * const ddr_bist_groups[] = {
	"gpio3", "gpio8", "gpio9", "gpio10",
};
static const char * const atest_tsens2_groups[] = {
	"gpio3",
};
static const char * const atest_usb1_groups[] = {
	"gpio3",
};
static const char * const blsp_spi2_groups[] = {
	"gpio4", "gpio5", "gpio6", "gpio7",
};
static const char * const blsp_uim2_groups[] = {
	"gpio4", "gpio5",
};
static const char * const phase_flag3_groups[] = {
	"gpio4",
};
static const char * const bimc_dte1_groups[] = {
	"gpio8", "gpio10",
};
static const char * const wlan1_adc0_groups[] = {
	"gpio9",
};
static const char * const atest_usb12_groups[] = {
	"gpio9",
};
static const char * const bimc_dte0_groups[] = {
	"gpio9", "gpio11",
};
static const char * const blsp_i2c3_groups[] = {
	"gpio10", "gpio11",
};
static const char * const wlan2_adc1_groups[] = {
	"gpio10",
};
static const char * const atest_usb11_groups[] = {
	"gpio10",
};
static const char * const dbg_out_groups[] = {
	"gpio11",
};
static const char * const wlan2_adc0_groups[] = {
	"gpio11",
};
static const char * const atest_usb10_groups[] = {
	"gpio11",
};
static const char * const RCM_MARKER_groups[] = {
	"gpio12", "gpio13",
};
static const char * const blsp_spi4_groups[] = {
	"gpio12", "gpio13", "gpio14", "gpio15",
};
static const char * const pri_mi2s_groups[] = {
	"gpio12", "gpio14", "gpio15", "gpio61",
};
static const char * const phase_flag26_groups[] = {
	"gpio12",
};
static const char * const qdss_cti0_a_groups[] = {
	"gpio49", "gpio50",
};
static const char * const qdss_cti0_b_groups[] = {
	"gpio13", "gpio21",
};
static const char * const qdss_cti1_a_groups[] = {
	"gpio53", "gpio55",
};
static const char * const qdss_cti1_b_groups[] = {
	"gpio12", "gpio66",
};
static const char * const DP_HOT_groups[] = {
	"gpio13",
};
static const char * const pri_mi2s_ws_groups[] = {
	"gpio13",
};
static const char * const phase_flag27_groups[] = {
	"gpio13",
};
static const char * const blsp_i2c4_groups[] = {
	"gpio14", "gpio15",
};
static const char * const phase_flag28_groups[] = {
	"gpio14",
};
static const char * const blsp_uart5_groups[] = {
	"gpio16", "gpio17", "gpio18", "gpio19",
};
static const char * const blsp_spi5_groups[] = {
	"gpio16", "gpio17", "gpio18", "gpio19",
};
static const char * const blsp_uim5_groups[] = {
	"gpio16", "gpio17",
};
static const char * const phase_flag5_groups[] = {
	"gpio17",
};
static const char * const blsp_i2c5_groups[] = {
	"gpio18", "gpio19",
};
static const char * const blsp_spi6_groups[] = {
	"gpio49", "gpio52", "gpio22", "gpio23",
};
static const char * const blsp_uart2_groups[] = {
	"gpio4", "gpio5", "gpio6", "gpio7",
};
static const char * const blsp_uim6_groups[] = {
	"gpio20", "gpio21",
};
static const char * const phase_flag11_groups[] = {
	"gpio21",
};
static const char * const vsense_data0_groups[] = {
	"gpio21",
};
static const char * const blsp_i2c6_groups[] = {
	"gpio22", "gpio23",
};
static const char * const phase_flag12_groups[] = {
	"gpio22",
};
static const char * const vsense_data1_groups[] = {
	"gpio22",
};
static const char * const phase_flag13_groups[] = {
	"gpio23",
};
static const char * const vsense_mode_groups[] = {
	"gpio23",
};
static const char * const blsp_spi7_groups[] = {
	"gpio24", "gpio25", "gpio26", "gpio27",
};
static const char * const blsp_uart6_a_groups[] = {
	"gpio24", "gpio25", "gpio26", "gpio27",
};
static const char * const blsp_uart6_b_groups[] = {
	"gpio28", "gpio29", "gpio30", "gpio31",
};
static const char * const sec_mi2s_groups[] = {
	"gpio24", "gpio25", "gpio26", "gpio27", "gpio62",
};
static const char * const sndwire_clk_groups[] = {
	"gpio24",
};
static const char * const phase_flag17_groups[] = {
	"gpio24",
};
static const char * const vsense_clkout_groups[] = {
	"gpio24",
};
static const char * const sndwire_data_groups[] = {
	"gpio25",
};
static const char * const phase_flag18_groups[] = {
	"gpio25",
};
static const char * const WSA_SPKR_groups[] = {
	"gpio26", "gpio27",
};
static const char * const blsp_i2c7_groups[] = {
	"gpio26", "gpio27",
};
static const char * const phase_flag19_groups[] = {
	"gpio26",
};
static const char * const vfr_1_groups[] = {
	"gpio27",
};
static const char * const phase_flag20_groups[] = {
	"gpio27",
};
static const char * const NFC_INT_groups[] = {
	"gpio28",
};
static const char * const blsp_spi8_a_groups[] = {
	"gpio28", "gpio29", "gpio30", "gpio31",
};
static const char * const blsp_spi8_b_groups[] = {
	"gpio40", "gpio41", "gpio44", "gpio52",
};
static const char * const m_voc_groups[] = {
	"gpio28",
};
static const char * const phase_flag21_groups[] = {
	"gpio28",
};
static const char * const NFC_EN_groups[] = {
	"gpio29",
};
static const char * const phase_flag22_groups[] = {
	"gpio29",
};
static const char * const NFC_DWL_groups[] = {
	"gpio30",
};
static const char * const blsp_i2c8_a_groups[] = {
	"gpio30", "gpio31",
};
static const char * const blsp_i2c8_b_groups[] = {
	"gpio44", "gpio52",
};
static const char * const phase_flag23_groups[] = {
	"gpio30",
};
static const char * const NFC_ESE_groups[] = {
	"gpio31",
};
static const char * const pwr_modem_groups[] = {
	"gpio31",
};
static const char * const phase_flag24_groups[] = {
	"gpio31",
};
static const char * const qdss_gpio_groups[] = {
	"gpio31", "gpio52", "gpio68", "gpio69",
};
static const char * const cam_mclk_groups[] = {
	"gpio32", "gpio33", "gpio34", "gpio35",
};
static const char * const pwr_nav_groups[] = {
	"gpio32",
};
static const char * const qdss_gpio0_groups[] = {
	"gpio32", "gpio67",
};
static const char * const qspi_data0_groups[] = {
	"gpio33",
};
static const char * const pwr_crypto_groups[] = {
	"gpio33",
};
static const char * const qdss_gpio1_groups[] = {
	"gpio33", "gpio63",
};
static const char * const qspi_data1_groups[] = {
	"gpio34",
};
static const char * const agera_pll_groups[] = {
	"gpio34", "gpio36",
};
static const char * const qdss_gpio2_groups[] = {
	"gpio34", "gpio64",
};
static const char * const qspi_data2_groups[] = {
	"gpio35",
};
static const char * const jitter_bist_groups[] = {
	"gpio35",
};
static const char * const qdss_gpio3_groups[] = {
	"gpio35", "gpio56",
};
static const char * const qdss_gpio7_groups[] = {
	"gpio39", "gpio71",
};
static const char * const FL_R3LED_groups[] = {
	"gpio40",
};
static const char * const CCI_TIMER0_groups[] = {
	"gpio40",
};
static const char * const FL_STROBE_groups[] = {
	"gpio41",
};
static const char * const CCI_TIMER1_groups[] = {
	"gpio41",
};
static const char * const CAM_LDO1_groups[] = {
	"gpio42",
};
static const char * const mdss_vsync0_groups[] = {
	"gpio42",
};
static const char * const mdss_vsync1_groups[] = {
	"gpio42",
};
static const char * const mdss_vsync2_groups[] = {
	"gpio42",
};
static const char * const mdss_vsync3_groups[] = {
	"gpio42",
};
static const char * const qdss_gpio9_groups[] = {
	"gpio42", "gpio76",
};
static const char * const CAM_IRQ_groups[] = {
	"gpio43",
};
static const char * const atest_usb2_groups[] = {
	"gpio35",
};
static const char * const cci_i2c_groups[] = {
	"gpio36", "gpio37", "gpio38", "gpio39",
};
static const char * const pll_bypassnl_groups[] = {
	"gpio36",
};
static const char * const atest_tsens_groups[] = {
	"gpio36",
};
static const char * const atest_usb21_groups[] = {
	"gpio36",
};
static const char * const pll_reset_groups[] = {
	"gpio37",
};
static const char * const atest_usb23_groups[] = {
	"gpio37",
};
static const char * const qdss_gpio6_groups[] = {
	"gpio38", "gpio70",
};
static const char * const CCI_TIMER3_groups[] = {
	"gpio43",
};
static const char * const CCI_ASYNC_groups[] = {
	"gpio43", "gpio44",
};
static const char * const qspi_cs_groups[] = {
	"gpio43", "gpio50",
};
static const char * const qdss_gpio10_groups[] = {
	"gpio43", "gpio77",
};
static const char * const CAM3_STANDBY_groups[] = {
	"gpio44",
};
static const char * const CCI_TIMER4_groups[] = {
	"gpio44",
};
static const char * const qdss_gpio11_groups[] = {
	"gpio44", "gpio79",
};
static const char * const CAM_LDO2_groups[] = {
	"gpio45",
};
static const char * const cci_async_groups[] = {
	"gpio45",
};
static const char * const qdss_gpio12_groups[] = {
	"gpio45", "gpio80",
};
static const char * const CAM0_RST_groups[] = {
	"gpio46",
};
static const char * const qdss_gpio13_groups[] = {
	"gpio46", "gpio78",
};
static const char * const CAM1_RST_groups[] = {
	"gpio47",
};
static const char * const qspi_clk_groups[] = {
	"gpio47",
};
static const char * const phase_flag30_groups[] = {
	"gpio47",
};
static const char * const qdss_gpio14_groups[] = {
	"gpio47", "gpio72",
};
static const char * const qspi_resetn_groups[] = {
	"gpio48",
};
static const char * const phase_flag1_groups[] = {
	"gpio48",
};
static const char * const qdss_gpio15_groups[] = {
	"gpio48", "gpio73",
};
static const char * const CAM0_STANDBY_groups[] = {
	"gpio49",
};
static const char * const phase_flag2_groups[] = {
	"gpio49",
};
static const char * const CAM1_STANDBY_groups[] = {
	"gpio50",
};
static const char * const phase_flag9_groups[] = {
	"gpio50",
};
static const char * const CAM2_STANDBY_groups[] = {
	"gpio51",
};
static const char * const qspi_data3_groups[] = {
	"gpio51",
};
static const char * const phase_flag15_groups[] = {
	"gpio51",
};
static const char * const qdss_gpio8_groups[] = {
	"gpio51", "gpio75",
};
static const char * const CAM3_RST_groups[] = {
	"gpio52",
};
static const char * const CCI_TIMER2_groups[] = {
	"gpio52",
};
static const char * const phase_flag16_groups[] = {
	"gpio52",
};
static const char * const LCD0_RESET_groups[] = {
	"gpio53",
};
static const char * const phase_flag6_groups[] = {
	"gpio53",
};
static const char * const SD_CARD_groups[] = {
	"gpio54",
};
static const char * const phase_flag29_groups[] = {
	"gpio54",
};
static const char * const DP_EN_groups[] = {
	"gpio55",
};
static const char * const phase_flag25_groups[] = {
	"gpio55",
};
static const char * const USBC_ORIENTATION_groups[] = {
	"gpio56",
};
static const char * const phase_flag10_groups[] = {
	"gpio56",
};
static const char * const atest_usb20_groups[] = {
	"gpio56",
};
static const char * const gcc_gp1_groups[] = {
	"gpio57", "gpio78",
};
static const char * const phase_flag4_groups[] = {
	"gpio57",
};
static const char * const atest_usb22_groups[] = {
	"gpio57",
};
static const char * const USB_PHY_groups[] = {
	"gpio58",
};
static const char * const gcc_gp2_groups[] = {
	"gpio58", "gpio81",
};
static const char * const atest_char_groups[] = {
	"gpio58",
};
static const char * const mdp_vsync_groups[] = {
	"gpio59", "gpio74",
};
static const char * const gcc_gp3_groups[] = {
	"gpio59", "gpio82",
};
static const char * const atest_char3_groups[] = {
	"gpio59",
};
static const char * const FORCE_TOUCH_groups[] = {
	"gpio60", "gpio73",
};
static const char * const cri_trng0_groups[] = {
	"gpio60",
};
static const char * const atest_char2_groups[] = {
	"gpio60",
};
static const char * const cri_trng1_groups[] = {
	"gpio61",
};
static const char * const atest_char1_groups[] = {
	"gpio61",
};
static const char * const AUDIO_USBC_groups[] = {
	"gpio62",
};
static const char * const audio_ref_groups[] = {
	"gpio62",
};
static const char * const MDP_VSYNC_groups[] = {
	"gpio62",
};
static const char * const cri_trng_groups[] = {
	"gpio62",
};
static const char * const atest_char0_groups[] = {
	"gpio62",
};
static const char * const US_EURO_groups[] = {
	"gpio63",
};
static const char * const LCD_BACKLIGHT_groups[] = {
	"gpio64",
};
static const char * const blsp_spi8_cs1_groups[] = {
	"gpio64",
};
static const char * const blsp_spi8_cs2_groups[] = {
	"gpio76",
};
static const char * const sp_cmu_groups[] = {
	"gpio64",
};
static const char * const nav_pps_a_groups[] = {
	"gpio65",
};
static const char * const nav_pps_b_groups[] = {
	"gpio98",
};
static const char * const nav_pps_c_groups[] = {
	"gpio80",
};
static const char * const gps_tx_a_groups[] = {
	"gpio65",
};
static const char * const gps_tx_b_groups[] = {
	"gpio98",
};
static const char * const gps_tx_c_groups[] = {
	"gpio80",
};
static const char * const adsp_ext_groups[] = {
	"gpio65",
};
static const char * const TS_RESET_groups[] = {
	"gpio66",
};
static const char * const ssc_irq_groups[] = {
	"gpio67", "gpio68", "gpio69", "gpio70", "gpio71", "gpio72", "gpio74",
	"gpio75", "gpio76",
};
static const char * const isense_dbg_groups[] = {
	"gpio68",
};
static const char * const phase_flag0_groups[] = {
	"gpio68",
};
static const char * const phase_flag7_groups[] = {
	"gpio69",
};
static const char * const phase_flag8_groups[] = {
	"gpio70",
};
static const char * const tsense_pwm1_groups[] = {
	"gpio71",
};
static const char * const tsense_pwm2_groups[] = {
	"gpio71",
};
static const char * const SENSOR_RST_groups[] = {
	"gpio77",
};
static const char * const WMSS_RESETN_groups[] = {
	"gpio78",
};
static const char * const HAPTICS_PWM_groups[] = {
	"gpio79",
};
static const char * const GPS_eLNA_groups[] = {
	"gpio80",
};
static const char * const mss_lte_groups[] = {
	"gpio81", "gpio82",
};
static const char * const uim2_data_groups[] = {
	"gpio83",
};
static const char * const uim2_clk_groups[] = {
	"gpio84",
};
static const char * const uim2_reset_groups[] = {
	"gpio85",
};
static const char * const uim2_present_groups[] = {
	"gpio86",
};
static const char * const uim1_data_groups[] = {
	"gpio87",
};
static const char * const uim1_clk_groups[] = {
	"gpio88",
};
static const char * const uim1_reset_groups[] = {
	"gpio89",
};
static const char * const uim1_present_groups[] = {
	"gpio90",
};
static const char * const uim_batt_groups[] = {
	"gpio91",
};
static const char * const pa_indicator_groups[] = {
	"gpio92",
};
static const char * const ldo_en_groups[] = {
	"gpio97",
};
static const char * const ldo_update_groups[] = {
	"gpio98",
};
static const char * const qlink_request_groups[] = {
	"gpio99",
};
static const char * const qlink_enable_groups[] = {
	"gpio100",
};
static const char * const prng_rosc_groups[] = {
	"gpio102",
};
static const char * const LCD_PWR_groups[] = {
	"gpio113",
};

static const struct msm_function sdm660_functions[] = {
	FUNCTION(blsp_spi1),
	FUNCTION(gpio),
	FUNCTION(blsp_uim1),
	FUNCTION(tgu_ch0),
	FUNCTION(qdss_gpio4),
	FUNCTION(atest_gpsadc1),
	FUNCTION(blsp_uart1),
	FUNCTION(SMB_STAT),
	FUNCTION(phase_flag14),
	FUNCTION(blsp_i2c2),
	FUNCTION(phase_flag31),
	FUNCTION(blsp_spi3),
	FUNCTION(blsp_spi3_cs1),
	FUNCTION(blsp_spi3_cs2),
	FUNCTION(wlan1_adc1),
	FUNCTION(atest_usb13),
	FUNCTION(tgu_ch1),
	FUNCTION(qdss_gpio5),
	FUNCTION(atest_gpsadc0),
	FUNCTION(blsp_i2c1),
	FUNCTION(ddr_bist),
	FUNCTION(atest_tsens2),
	FUNCTION(atest_usb1),
	FUNCTION(blsp_spi2),
	FUNCTION(blsp_uim2),
	FUNCTION(phase_flag3),
	FUNCTION(bimc_dte1),
	FUNCTION(wlan1_adc0),
	FUNCTION(atest_usb12),
	FUNCTION(bimc_dte0),
	FUNCTION(blsp_i2c3),
	FUNCTION(wlan2_adc1),
	FUNCTION(atest_usb11),
	FUNCTION(dbg_out),
	FUNCTION(wlan2_adc0),
	FUNCTION(atest_usb10),
	FUNCTION(RCM_MARKER),
	FUNCTION(blsp_spi4),
	FUNCTION(pri_mi2s),
	FUNCTION(phase_flag26),
	FUNCTION(qdss_cti0_a),
	FUNCTION(qdss_cti0_b),
	FUNCTION(qdss_cti1_a),
	FUNCTION(qdss_cti1_b),
	FUNCTION(DP_HOT),
	FUNCTION(pri_mi2s_ws),
	FUNCTION(phase_flag27),
	FUNCTION(blsp_i2c4),
	FUNCTION(phase_flag28),
	FUNCTION(blsp_uart5),
	FUNCTION(blsp_spi5),
	FUNCTION(blsp_uim5),
	FUNCTION(phase_flag5),
	FUNCTION(blsp_i2c5),
	FUNCTION(blsp_spi6),
	FUNCTION(blsp_uart2),
	FUNCTION(blsp_uim6),
	FUNCTION(phase_flag11),
	FUNCTION(vsense_data0),
	FUNCTION(blsp_i2c6),
	FUNCTION(phase_flag12),
	FUNCTION(vsense_data1),
	FUNCTION(phase_flag13),
	FUNCTION(vsense_mode),
	FUNCTION(blsp_spi7),
	FUNCTION(blsp_uart6_a),
	FUNCTION(blsp_uart6_b),
	FUNCTION(sec_mi2s),
	FUNCTION(sndwire_clk),
	FUNCTION(phase_flag17),
	FUNCTION(vsense_clkout),
	FUNCTION(sndwire_data),
	FUNCTION(phase_flag18),
	FUNCTION(WSA_SPKR),
	FUNCTION(blsp_i2c7),
	FUNCTION(phase_flag19),
	FUNCTION(vfr_1),
	FUNCTION(phase_flag20),
	FUNCTION(NFC_INT),
	FUNCTION(blsp_spi8_cs1),
	FUNCTION(blsp_spi8_cs2),
	FUNCTION(m_voc),
	FUNCTION(phase_flag21),
	FUNCTION(NFC_EN),
	FUNCTION(phase_flag22),
	FUNCTION(NFC_DWL),
	FUNCTION(blsp_i2c8_a),
	FUNCTION(blsp_i2c8_b),
	FUNCTION(phase_flag23),
	FUNCTION(NFC_ESE),
	FUNCTION(pwr_modem),
	FUNCTION(phase_flag24),
	FUNCTION(qdss_gpio),
	FUNCTION(cam_mclk),
	FUNCTION(pwr_nav),
	FUNCTION(qdss_gpio0),
	FUNCTION(qspi_data0),
	FUNCTION(pwr_crypto),
	FUNCTION(qdss_gpio1),
	FUNCTION(qspi_data1),
	FUNCTION(agera_pll),
	FUNCTION(qdss_gpio2),
	FUNCTION(qspi_data2),
	FUNCTION(jitter_bist),
	FUNCTION(qdss_gpio3),
	FUNCTION(qdss_gpio7),
	FUNCTION(FL_R3LED),
	FUNCTION(CCI_TIMER0),
	FUNCTION(FL_STROBE),
	FUNCTION(CCI_TIMER1),
	FUNCTION(CAM_LDO1),
	FUNCTION(mdss_vsync0),
	FUNCTION(mdss_vsync1),
	FUNCTION(mdss_vsync2),
	FUNCTION(mdss_vsync3),
	FUNCTION(qdss_gpio9),
	FUNCTION(CAM_IRQ),
	FUNCTION(atest_usb2),
	FUNCTION(cci_i2c),
	FUNCTION(pll_bypassnl),
	FUNCTION(atest_tsens),
	FUNCTION(atest_usb21),
	FUNCTION(pll_reset),
	FUNCTION(atest_usb23),
	FUNCTION(qdss_gpio6),
	FUNCTION(CCI_TIMER3),
	FUNCTION(CCI_ASYNC),
	FUNCTION(qspi_cs),
	FUNCTION(qdss_gpio10),
	FUNCTION(CAM3_STANDBY),
	FUNCTION(CCI_TIMER4),
	FUNCTION(qdss_gpio11),
	FUNCTION(CAM_LDO2),
	FUNCTION(cci_async),
	FUNCTION(qdss_gpio12),
	FUNCTION(CAM0_RST),
	FUNCTION(qdss_gpio13),
	FUNCTION(CAM1_RST),
	FUNCTION(qspi_clk),
	FUNCTION(phase_flag30),
	FUNCTION(qdss_gpio14),
	FUNCTION(qspi_resetn),
	FUNCTION(phase_flag1),
	FUNCTION(qdss_gpio15),
	FUNCTION(CAM0_STANDBY),
	FUNCTION(phase_flag2),
	FUNCTION(CAM1_STANDBY),
	FUNCTION(phase_flag9),
	FUNCTION(CAM2_STANDBY),
	FUNCTION(qspi_data3),
	FUNCTION(phase_flag15),
	FUNCTION(qdss_gpio8),
	FUNCTION(CAM3_RST),
	FUNCTION(CCI_TIMER2),
	FUNCTION(phase_flag16),
	FUNCTION(LCD0_RESET),
	FUNCTION(phase_flag6),
	FUNCTION(SD_CARD),
	FUNCTION(phase_flag29),
	FUNCTION(DP_EN),
	FUNCTION(phase_flag25),
	FUNCTION(USBC_ORIENTATION),
	FUNCTION(phase_flag10),
	FUNCTION(atest_usb20),
	FUNCTION(gcc_gp1),
	FUNCTION(phase_flag4),
	FUNCTION(atest_usb22),
	FUNCTION(USB_PHY),
	FUNCTION(gcc_gp2),
	FUNCTION(atest_char),
	FUNCTION(mdp_vsync),
	FUNCTION(gcc_gp3),
	FUNCTION(atest_char3),
	FUNCTION(FORCE_TOUCH),
	FUNCTION(cri_trng0),
	FUNCTION(atest_char2),
	FUNCTION(cri_trng1),
	FUNCTION(atest_char1),
	FUNCTION(AUDIO_USBC),
	FUNCTION(audio_ref),
	FUNCTION(MDP_VSYNC),
	FUNCTION(cri_trng),
	FUNCTION(atest_char0),
	FUNCTION(US_EURO),
	FUNCTION(LCD_BACKLIGHT),
	FUNCTION(blsp_spi8_a),
	FUNCTION(blsp_spi8_b),
	FUNCTION(sp_cmu),
	FUNCTION(nav_pps_a),
	FUNCTION(nav_pps_b),
	FUNCTION(nav_pps_c),
	FUNCTION(gps_tx_a),
	FUNCTION(gps_tx_b),
	FUNCTION(gps_tx_c),
	FUNCTION(adsp_ext),
	FUNCTION(TS_RESET),
	FUNCTION(ssc_irq),
	FUNCTION(isense_dbg),
	FUNCTION(phase_flag0),
	FUNCTION(phase_flag7),
	FUNCTION(phase_flag8),
	FUNCTION(tsense_pwm1),
	FUNCTION(tsense_pwm2),
	FUNCTION(SENSOR_RST),
	FUNCTION(WMSS_RESETN),
	FUNCTION(HAPTICS_PWM),
	FUNCTION(GPS_eLNA),
	FUNCTION(mss_lte),
	FUNCTION(uim2_data),
	FUNCTION(uim2_clk),
	FUNCTION(uim2_reset),
	FUNCTION(uim2_present),
	FUNCTION(uim1_data),
	FUNCTION(uim1_clk),
	FUNCTION(uim1_reset),
	FUNCTION(uim1_present),
	FUNCTION(uim_batt),
	FUNCTION(pa_indicator),
	FUNCTION(ldo_en),
	FUNCTION(ldo_update),
	FUNCTION(qlink_request),
	FUNCTION(qlink_enable),
	FUNCTION(prng_rosc),
	FUNCTION(LCD_PWR),
};

static const struct msm_pingroup sdm660_groups[] = {
	PINGROUP(0, SOUTH, blsp_spi1, blsp_uart1, blsp_uim1, tgu_ch0, NA, NA,
		 qdss_gpio4, atest_gpsadc1, NA),
	PINGROUP(1, SOUTH, blsp_spi1, blsp_uart1, blsp_uim1, tgu_ch1, NA, NA,
		 qdss_gpio5, atest_gpsadc0, NA),
	PINGROUP(2, SOUTH, blsp_spi1, blsp_uart1, blsp_i2c1, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(3, SOUTH, blsp_spi1, blsp_uart1, blsp_i2c1, ddr_bist, NA, NA,
		 atest_tsens2, atest_usb1, NA),
	PINGROUP(4, NORTH, blsp_spi2, blsp_uim2, blsp_uart2, phase_flag3, NA,
		 NA, NA, NA, NA),
	PINGROUP(5, SOUTH, blsp_spi2, blsp_uim2, blsp_uart2, phase_flag14, NA,
		 NA, NA, NA, NA),
	PINGROUP(6, SOUTH, blsp_spi2, blsp_i2c2, blsp_uart2, phase_flag31, NA,
		 NA, NA, NA, NA),
	PINGROUP(7, SOUTH, blsp_spi2, blsp_i2c2, blsp_uart2, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(8, NORTH, blsp_spi3, ddr_bist, NA, NA, NA, wlan1_adc1,
		 atest_usb13, bimc_dte1, NA),
	PINGROUP(9, NORTH, blsp_spi3, ddr_bist, NA, NA, NA, wlan1_adc0,
		 atest_usb12, bimc_dte0, NA),
	PINGROUP(10, NORTH, blsp_spi3, blsp_i2c3, ddr_bist, NA, NA, wlan2_adc1,
		 atest_usb11, bimc_dte1, NA),
	PINGROUP(11, NORTH, blsp_spi3, blsp_i2c3, NA, dbg_out, wlan2_adc0,
		 atest_usb10, bimc_dte0, NA, NA),
	PINGROUP(12, NORTH, blsp_spi4, pri_mi2s, NA, phase_flag26, qdss_cti1_b,
		 NA, NA, NA, NA),
	PINGROUP(13, NORTH, blsp_spi4, DP_HOT, pri_mi2s_ws, NA, NA,
		 phase_flag27, qdss_cti0_b, NA, NA),
	PINGROUP(14, NORTH, blsp_spi4, blsp_i2c4, pri_mi2s, NA, phase_flag28,
		 NA, NA, NA, NA),
	PINGROUP(15, NORTH, blsp_spi4, blsp_i2c4, pri_mi2s, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(16, CENTER, blsp_uart5, blsp_spi5, blsp_uim5, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(17, CENTER, blsp_uart5, blsp_spi5, blsp_uim5, NA, phase_flag5,
		 NA, NA, NA, NA),
	PINGROUP(18, CENTER, blsp_uart5, blsp_spi5, blsp_i2c5, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(19, CENTER, blsp_uart5, blsp_spi5, blsp_i2c5, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(20, SOUTH, NA, NA, blsp_uim6, NA, NA, NA, NA,
		 NA, NA),
	PINGROUP(21, SOUTH, NA, NA, blsp_uim6, NA, phase_flag11,
		 qdss_cti0_b, vsense_data0, NA, NA),
	PINGROUP(22, CENTER, blsp_spi6, NA, blsp_i2c6, NA,
		 phase_flag12, vsense_data1, NA, NA, NA),
	PINGROUP(23, CENTER, blsp_spi6, NA, blsp_i2c6, NA,
		 phase_flag13, vsense_mode, NA, NA, NA),
	PINGROUP(24, NORTH, blsp_spi7, blsp_uart6_a, sec_mi2s, sndwire_clk, NA,
		 NA, phase_flag17, vsense_clkout, NA),
	PINGROUP(25, NORTH, blsp_spi7, blsp_uart6_a, sec_mi2s, sndwire_data, NA,
		 NA, phase_flag18, NA, NA),
	PINGROUP(26, NORTH, blsp_spi7, blsp_uart6_a, blsp_i2c7, sec_mi2s, NA,
		 phase_flag19, NA, NA, NA),
	PINGROUP(27, NORTH, blsp_spi7, blsp_uart6_a, blsp_i2c7, vfr_1, sec_mi2s,
		 NA, phase_flag20, NA, NA),
	PINGROUP(28, CENTER, blsp_spi8_a, blsp_uart6_b, m_voc, NA, phase_flag21,
		 NA, NA, NA, NA),
	PINGROUP(29, CENTER, blsp_spi8_a, blsp_uart6_b, NA, NA, phase_flag22,
		 NA, NA, NA, NA),
	PINGROUP(30, CENTER, blsp_spi8_a, blsp_uart6_b, blsp_i2c8_a,
		 blsp_spi3_cs1, NA, phase_flag23, NA, NA, NA),
	PINGROUP(31, CENTER, blsp_spi8_a, blsp_uart6_b, blsp_i2c8_a, pwr_modem,
		 NA, phase_flag24, qdss_gpio, NA, NA),
	PINGROUP(32, SOUTH, cam_mclk, pwr_nav, NA, NA, qdss_gpio0, NA, NA, NA,
		 NA),
	PINGROUP(33, SOUTH, cam_mclk, qspi_data0, pwr_crypto, NA, NA,
		 qdss_gpio1, NA, NA, NA),
	PINGROUP(34, SOUTH, cam_mclk, qspi_data1, agera_pll, NA, NA,
		 qdss_gpio2, NA, NA, NA),
	PINGROUP(35, SOUTH, cam_mclk, qspi_data2, jitter_bist, NA, NA,
		 qdss_gpio3, NA, atest_usb2, NA),
	PINGROUP(36, SOUTH, cci_i2c, pll_bypassnl, agera_pll, NA, NA,
		 qdss_gpio4, atest_tsens, atest_usb21, NA),
	PINGROUP(37, SOUTH, cci_i2c, pll_reset, NA, NA, qdss_gpio5,
		 atest_usb23, NA, NA, NA),
	PINGROUP(38, SOUTH, cci_i2c, NA, NA, qdss_gpio6, NA, NA, NA, NA, NA),
	PINGROUP(39, SOUTH, cci_i2c, NA, NA, qdss_gpio7, NA, NA, NA, NA, NA),
	PINGROUP(40, SOUTH, CCI_TIMER0, NA, blsp_spi8_b, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(41, SOUTH, CCI_TIMER1, NA, blsp_spi8_b, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(42, SOUTH, mdss_vsync0, mdss_vsync1, mdss_vsync2, mdss_vsync3,
		 NA, NA, qdss_gpio9, NA, NA),
	PINGROUP(43, SOUTH, CCI_TIMER3, CCI_ASYNC, qspi_cs, NA, NA,
		 qdss_gpio10, NA, NA, NA),
	PINGROUP(44, SOUTH, CCI_TIMER4, CCI_ASYNC, blsp_spi8_b, blsp_i2c8_b, NA,
		 NA, qdss_gpio11, NA, NA),
	PINGROUP(45, SOUTH, cci_async, NA, NA, qdss_gpio12, NA, NA, NA, NA, NA),
	PINGROUP(46, SOUTH, blsp_spi1, NA, NA, qdss_gpio13, NA, NA, NA, NA, NA),
	PINGROUP(47, SOUTH, qspi_clk, NA, phase_flag30, qdss_gpio14, NA, NA,
		 NA, NA, NA),
	PINGROUP(48, SOUTH, NA, phase_flag1, qdss_gpio15, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(49, SOUTH, blsp_spi6, phase_flag2, qdss_cti0_a, NA, NA, NA,
		 NA, NA, NA),
	PINGROUP(50, SOUTH, qspi_cs, NA, phase_flag9, qdss_cti0_a, NA, NA, NA,
		 NA, NA),
	PINGROUP(51, SOUTH, qspi_data3, NA, phase_flag15, qdss_gpio8, NA, NA,
		 NA, NA, NA),
	PINGROUP(52, SOUTH, CCI_TIMER2, blsp_spi8_b, blsp_i2c8_b, blsp_spi6,
		 phase_flag16, qdss_gpio, NA, NA, NA),
	PINGROUP(53, NORTH, NA, phase_flag6, qdss_cti1_a, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(54, NORTH, NA, NA, phase_flag29, NA, NA, NA, NA, NA, NA),
	PINGROUP(55, SOUTH, NA, phase_flag25, qdss_cti1_a, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(56, SOUTH, NA, phase_flag10, qdss_gpio3, NA, atest_usb20, NA,
		 NA, NA, NA),
	PINGROUP(57, SOUTH, gcc_gp1, NA, phase_flag4, atest_usb22, NA, NA, NA,
		 NA, NA),
	PINGROUP(58, SOUTH, USB_PHY, gcc_gp2, NA, NA, atest_char, NA, NA, NA,
		 NA),
	PINGROUP(59, NORTH, mdp_vsync, gcc_gp3, NA, NA, atest_char3, NA, NA,
		 NA, NA),
	PINGROUP(60, NORTH, cri_trng0, NA, NA, atest_char2, NA, NA, NA, NA, NA),
	PINGROUP(61, NORTH, pri_mi2s, cri_trng1, NA, NA, atest_char1, NA, NA,
		 NA, NA),
	PINGROUP(62, NORTH, sec_mi2s, audio_ref, MDP_VSYNC, cri_trng, NA, NA,
		 atest_char0, NA, NA),
	PINGROUP(63, NORTH, NA, NA, NA, qdss_gpio1, NA, NA, NA, NA, NA),
	PINGROUP(64, SOUTH, blsp_spi8_cs1, sp_cmu, NA, NA, qdss_gpio2, NA, NA,
		 NA, NA),
	PINGROUP(65, SOUTH, NA, nav_pps_a, nav_pps_a, gps_tx_a, blsp_spi3_cs2,
		 adsp_ext, NA, NA, NA),
	PINGROUP(66, NORTH, NA, NA, qdss_cti1_b, NA, NA, NA, NA, NA, NA),
	PINGROUP(67, NORTH, NA, NA, qdss_gpio0, NA, NA, NA, NA, NA, NA),
	PINGROUP(68, NORTH, isense_dbg, NA, phase_flag0, qdss_gpio, NA, NA, NA,
		 NA, NA),
	PINGROUP(69, NORTH, NA, phase_flag7, qdss_gpio, NA, NA, NA, NA, NA, NA),
	PINGROUP(70, NORTH, NA, phase_flag8, qdss_gpio6, NA, NA, NA, NA, NA,
		 NA),
	PINGROUP(71, NORTH, NA, NA, qdss_gpio7, tsense_pwm1, tsense_pwm2, NA,
		 NA, NA, NA),
	PINGROUP(72, NORTH, NA, qdss_gpio14, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(73, NORTH, NA, NA, qdss_gpio15, NA, NA, NA, NA, NA, NA),
	PINGROUP(74, NORTH, mdp_vsync, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(75, NORTH, NA, NA, qdss_gpio8, NA, NA, NA, NA, NA, NA),
	PINGROUP(76, NORTH, blsp_spi8_cs2, NA, NA, NA, qdss_gpio9, NA, NA, NA,
		 NA),
	PINGROUP(77, NORTH, NA, NA, qdss_gpio10, NA, NA, NA, NA, NA, NA),
	PINGROUP(78, NORTH, gcc_gp1, NA, qdss_gpio13, NA, NA, NA, NA, NA, NA),
	PINGROUP(79, SOUTH, NA, NA, qdss_gpio11, NA, NA, NA, NA, NA, NA),
	PINGROUP(80, SOUTH, nav_pps_b, nav_pps_b, gps_tx_c, NA, NA, qdss_gpio12,
		 NA, NA, NA),
	PINGROUP(81, CENTER, mss_lte, gcc_gp2, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(82, CENTER, mss_lte, gcc_gp3, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(83, SOUTH, uim2_data, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(84, SOUTH, uim2_clk, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(85, SOUTH, uim2_reset, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(86, SOUTH, uim2_present, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(87, SOUTH, uim1_data, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(88, SOUTH, uim1_clk, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(89, SOUTH, uim1_reset, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(90, SOUTH, uim1_present, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(91, SOUTH, uim_batt, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(92, SOUTH, NA, NA, pa_indicator, NA, NA, NA, NA, NA, NA),
	PINGROUP(93, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(94, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(95, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(96, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(97, SOUTH, NA, ldo_en, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(98, SOUTH, NA, nav_pps_c, nav_pps_c, gps_tx_b, ldo_update, NA,
		 NA, NA, NA),
	PINGROUP(99, SOUTH, qlink_request, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(100, SOUTH, qlink_enable, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(101, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(102, SOUTH, NA, prng_rosc, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(103, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(104, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(105, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(106, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(107, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(108, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(109, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(110, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(111, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(112, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	PINGROUP(113, SOUTH, NA, NA, NA, NA, NA, NA, NA, NA, NA),
	SDC_QDSD_PINGROUP(sdc1_clk, 0x99a000, 13, 6),
	SDC_QDSD_PINGROUP(sdc1_cmd, 0x99a000, 11, 3),
	SDC_QDSD_PINGROUP(sdc1_data, 0x99a000, 9, 0),
	SDC_QDSD_PINGROUP(sdc2_clk, 0x99b000, 14, 6),
	SDC_QDSD_PINGROUP(sdc2_cmd, 0x99b000, 11, 3),
	SDC_QDSD_PINGROUP(sdc2_data, 0x99b000, 9, 0),
	SDC_QDSD_PINGROUP(sdc1_rclk, 0x99a000, 15, 0),
	UFS_RESET(ufs_reset, 0x9a3000),
};

static const struct msm_pinctrl_soc_data sdm660_pinctrl = {
	.pins = sdm660_pins,
	.npins = ARRAY_SIZE(sdm660_pins),
	.functions = sdm660_functions,
	.nfunctions = ARRAY_SIZE(sdm660_functions),
	.groups = sdm660_groups,
	.ngroups = ARRAY_SIZE(sdm660_groups),
	.ngpios = 114,
};

static int sdm660_pinctrl_probe(struct platform_device *pdev)
{
	return msm_pinctrl_probe(pdev, &sdm660_pinctrl);
}

static const struct of_device_id sdm660_pinctrl_of_match[] = {
	{ .compatible = "qcom,sdm660-pinctrl", },
	{ },
};

static struct platform_driver sdm660_pinctrl_driver = {
	.driver = {
		.name = "sdm660-pinctrl",
		.of_match_table = sdm660_pinctrl_of_match,
	},
	.probe = sdm660_pinctrl_probe,
	.remove = msm_pinctrl_remove,
};

static int __init sdm660_pinctrl_init(void)
{
	return platform_driver_register(&sdm660_pinctrl_driver);
}
arch_initcall(sdm660_pinctrl_init);

static void __exit sdm660_pinctrl_exit(void)
{
	platform_driver_unregister(&sdm660_pinctrl_driver);
}
module_exit(sdm660_pinctrl_exit);

MODULE_DESCRIPTION("QTI sdm660 pinctrl driver");
MODULE_LICENSE("GPL v2");
MODULE_DEVICE_TABLE(of, sdm660_pinctrl_of_match);
