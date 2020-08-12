/*
 * Copyright 2015 Linaro Limited
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _DT_BINDINGS_CLK_MSM_RPMCC_H
#define _DT_BINDINGS_CLK_MSM_RPMCC_H

/* RPM clocks */
#define RPM_PXO_CLK				0
#define RPM_PXO_A_CLK				1
#define RPM_CXO_CLK				2
#define RPM_CXO_A_CLK				3
#define RPM_APPS_FABRIC_CLK			4
#define RPM_APPS_FABRIC_A_CLK			5
#define RPM_CFPB_CLK				6
#define RPM_CFPB_A_CLK				7
#define RPM_QDSS_CLK				8
#define RPM_QDSS_A_CLK				9
#define RPM_DAYTONA_FABRIC_CLK			10
#define RPM_DAYTONA_FABRIC_A_CLK		11
#define RPM_EBI1_CLK				12
#define RPM_EBI1_A_CLK				13
#define RPM_MM_FABRIC_CLK			14
#define RPM_MM_FABRIC_A_CLK			15
#define RPM_MMFPB_CLK				16
#define RPM_MMFPB_A_CLK				17
#define RPM_SYS_FABRIC_CLK			18
#define RPM_SYS_FABRIC_A_CLK			19
#define RPM_SFPB_CLK				20
#define RPM_SFPB_A_CLK				21
#define RPM_SMI_CLK				22
#define RPM_SMI_A_CLK				23
#define RPM_PLL4_CLK				24
#define RPM_XO_D0				25
#define RPM_XO_D1				26
#define RPM_XO_A0				27
#define RPM_XO_A1				28
#define RPM_XO_A2				29

/* SMD RPM clocks */
#define RPM_SMD_XO_CLK_SRC			0
#define RPM_SMD_XO_A_CLK_SRC			1
#define RPM_SMD_PCNOC_CLK			2
#define RPM_SMD_PCNOC_A_CLK			3
#define RPM_SMD_SNOC_CLK			4
#define RPM_SMD_SNOC_A_CLK			5
#define RPM_SMD_BIMC_CLK			6
#define RPM_SMD_BIMC_A_CLK			7
#define RPM_SMD_QDSS_CLK			8
#define RPM_SMD_QDSS_A_CLK			9
#define RPM_SMD_IPA_CLK				10
#define RPM_SMD_IPA_A_CLK			11
#define RPM_SMD_QUP_CLK				12
#define RPM_SMD_QUP_A_CLK			13
#define RPM_SMD_MMRT_CLK			14
#define RPM_SMD_MMRT_A_CLK			15
#define RPM_SMD_MMNRT_CLK			16
#define RPM_SMD_MMNRT_A_CLK			17
#define RPM_SMD_SNOC_PERIPH_CLK			18
#define RPM_SMD_SNOC_PERIPH_A_CLK		19
#define RPM_SMD_SNOC_LPASS_CLK			20
#define RPM_SMD_SNOC_LPASS_A_CLK		21
#define RPM_SMD_BB_CLK1				22
#define RPM_SMD_BB_CLK1_A			23
#define RPM_SMD_BB_CLK2				24
#define RPM_SMD_BB_CLK2_A			25
#define RPM_SMD_RF_CLK1				26
#define RPM_SMD_RF_CLK1_A			27
#define RPM_SMD_RF_CLK2				28
#define RPM_SMD_RF_CLK2_A			29
#define RPM_SMD_BB_CLK1_PIN			30
#define RPM_SMD_BB_CLK1_A_PIN			31
#define RPM_SMD_BB_CLK2_PIN			32
#define RPM_SMD_BB_CLK2_A_PIN			33
#define RPM_SMD_RF_CLK1_PIN			34
#define RPM_SMD_RF_CLK1_A_PIN			35
#define RPM_SMD_RF_CLK2_PIN			36
#define RPM_SMD_RF_CLK2_A_PIN			37
#define RPM_SMD_PNOC_CLK			38
#define RPM_SMD_PNOC_A_CLK			39
#define RPM_SMD_CNOC_CLK			40
#define RPM_SMD_CNOC_A_CLK			41
#define RPM_SMD_MMSSNOC_AHB_CLK			42
#define RPM_SMD_MMSSNOC_AHB_A_CLK		43
#define RPM_SMD_GFX3D_CLK_SRC			44
#define RPM_SMD_GFX3D_A_CLK_SRC			45
#define RPM_SMD_OCMEMGX_CLK			46
#define RPM_SMD_OCMEMGX_A_CLK			47
#define RPM_SMD_CXO_D0				48
#define RPM_SMD_CXO_D0_A			49
#define RPM_SMD_CXO_D1				50
#define RPM_SMD_CXO_D1_A			51
#define RPM_SMD_CXO_A0				52
#define RPM_SMD_CXO_A0_A			53
#define RPM_SMD_CXO_A1				54
#define RPM_SMD_CXO_A1_A			55
#define RPM_SMD_CXO_A2				56
#define RPM_SMD_CXO_A2_A			57
#define RPM_SMD_DIV_CLK1			58
#define RPM_SMD_DIV_A_CLK1			59
#define RPM_SMD_DIV_CLK2			60
#define RPM_SMD_DIV_A_CLK2			61
#define RPM_SMD_DIV_CLK3			61
#define RPM_SMD_DIV_A_CLK3			62
#define RPM_SMD_DIFF_CLK			63
#define RPM_SMD_DIFF_A_CLK			64
#define RPM_SMD_CXO_D0_PIN			64
#define RPM_SMD_CXO_D0_A_PIN			65
#define RPM_SMD_CXO_D1_PIN			66
#define RPM_SMD_CXO_D1_A_PIN			67
#define RPM_SMD_CXO_A0_PIN			68
#define RPM_SMD_CXO_A0_A_PIN			69
#define RPM_SMD_CXO_A1_PIN			70
#define RPM_SMD_CXO_A1_A_PIN			71
#define RPM_SMD_CXO_A2_PIN			72
#define RPM_SMD_CXO_A2_A_PIN			73
#define RPM_SMD_QPIC_CLK			74
#define RPM_SMD_QPIC_A_CLK			75
#define RPM_SMD_CE1_CLK				76
#define RPM_SMD_CE1_A_CLK			77
#define RPM_SMD_BIMC_GPU_CLK			78
#define RPM_SMD_BIMC_GPU_A_CLK			79
#define RPM_SMD_LN_BB_CLK			80
#define RPM_SMD_LN_BB_CLK_A			81
#define RPM_SMD_LN_BB_CLK_PIN			82
#define RPM_SMD_LN_BB_CLK_A_PIN			83
#define RPM_SMD_RF_CLK3				84
#define RPM_SMD_RF_CLK3_A			85
#define RPM_SMD_RF_CLK3_PIN			86
#define RPM_SMD_RF_CLK3_A_PIN			87
#define RPM_SMD_LN_BB_CLK1			88
#define RPM_SMD_LN_BB_CLK1_A			89
#define RPM_SMD_LN_BB_CLK2			90
#define RPM_SMD_LN_BB_CLK2_A			91
#define RPM_SMD_LN_BB_CLK3			92
#define RPM_SMD_LN_BB_CLK3_A			93
#define RPM_SMD_MMAXI_CLK			94
#define RPM_SMD_MMAXI_A_CLK			95
#define RPM_SMD_AGGR1_NOC_CLK			96
#define RPM_SMD_AGGR1_NOC_A_CLK			97
#define RPM_SMD_AGGR2_NOC_CLK			98
#define RPM_SMD_AGGR2_NOC_A_CLK			99
#define PNOC_MSMBUS_CLK				100
#define PNOC_MSMBUS_A_CLK			101
#define PNOC_KEEPALIVE_A_CLK			102
#define SNOC_MSMBUS_CLK				103
#define SNOC_MSMBUS_A_CLK			104
#define BIMC_MSMBUS_CLK				105
#define BIMC_MSMBUS_A_CLK			106
#define PNOC_USB_CLK				107
#define PNOC_USB_A_CLK				108
#define SNOC_USB_CLK				109
#define SNOC_USB_A_CLK				110
#define BIMC_USB_CLK				111
#define BIMC_USB_A_CLK				112
#define SNOC_WCNSS_A_CLK			113
#define BIMC_WCNSS_A_CLK			114
#define MCD_CE1_CLK				115
#define QCEDEV_CE1_CLK				116
#define QCRYPTO_CE1_CLK				117
#define QSEECOM_CE1_CLK				118
#define SCM_CE1_CLK				119
#define CXO_SMD_OTG_CLK				120
#define CXO_SMD_LPM_CLK				121
#define CXO_SMD_PIL_PRONTO_CLK			122
#define CXO_SMD_PIL_MSS_CLK			123
#define CXO_SMD_WLAN_CLK			124
#define CXO_SMD_PIL_LPASS_CLK			125
#define CXO_SMD_PIL_CDSP_CLK			126
#define CNOC_MSMBUS_CLK				127
#define CNOC_MSMBUS_A_CLK			128
#define CNOC_KEEPALIVE_A_CLK			129
#define SNOC_KEEPALIVE_A_CLK			130
#define CPP_MMNRT_MSMBUS_CLK			131
#define CPP_MMNRT_MSMBUS_A_CLK			132
#define JPEG_MMNRT_MSMBUS_CLK			133
#define JPEG_MMNRT_MSMBUS_A_CLK			134
#define VENUS_MMNRT_MSMBUS_CLK			135
#define VENUS_MMNRT_MSMBUS_A_CLK		136
#define ARM9_MMNRT_MSMBUS_CLK			137
#define ARM9_MMNRT_MSMBUS_A_CLK			138
#define MDP_MMRT_MSMBUS_CLK			139
#define MDP_MMRT_MSMBUS_A_CLK			140
#define VFE_MMRT_MSMBUS_CLK			141
#define VFE_MMRT_MSMBUS_A_CLK			142
#define QUP0_MSMBUS_SNOC_PERIPH_CLK		143
#define QUP0_MSMBUS_SNOC_PERIPH_A_CLK		144
#define QUP1_MSMBUS_SNOC_PERIPH_CLK		145
#define QUP1_MSMBUS_SNOC_PERIPH_A_CLK		146
#define QUP2_MSMBUS_SNOC_PERIPH_CLK             147
#define QUP2_MSMBUS_SNOC_PERIPH_A_CLK           148
#define DAP_MSMBUS_SNOC_PERIPH_CLK		149
#define DAP_MSMBUS_SNOC_PERIPH_A_CLK		150
#define SDC1_MSMBUS_SNOC_PERIPH_CLK		151
#define SDC1_MSMBUS_SNOC_PERIPH_A_CLK		152
#define SDC2_MSMBUS_SNOC_PERIPH_CLK		153
#define SDC2_MSMBUS_SNOC_PERIPH_A_CLK		154
#define CRYPTO_MSMBUS_SNOC_PERIPH_CLK		155
#define CRYPTO_MSMBUS_SNOC_PERIPH_A_CLK		156
#define SDC1_SLV_MSMBUS_SNOC_PERIPH_CLK		157
#define SDC1_SLV_MSMBUS_SNOC_PERIPH_A_CLK	158
#define SDC2_SLV_MSMBUS_SNOC_PERIPH_CLK		159
#define SDC2_SLV_MSMBUS_SNOC_PERIPH_A_CLK	160

#endif
