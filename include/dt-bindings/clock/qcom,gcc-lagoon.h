/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 */

#ifndef _DT_BINDINGS_CLK_QCOM_GCC_LAGOON_H
#define _DT_BINDINGS_CLK_QCOM_GCC_LAGOON_H

/* GCC clocks */
#define GPLL0					0
#define GPLL0_OUT_EVEN				1
#define GPLL0_OUT_ODD				2
#define GPLL6					3
#define GPLL6_OUT_EVEN				4
#define GPLL7					5
#define GCC_AGGRE_CNOC_PERIPH_CENTER_AHB_CLK	6
#define GCC_AGGRE_NOC_CENTER_AHB_CLK		7
#define GCC_AGGRE_NOC_PCIE_SF_AXI_CLK		8
#define GCC_AGGRE_NOC_PCIE_TBU_CLK		9
#define GCC_AGGRE_NOC_WLAN_AXI_CLK		10
#define GCC_AGGRE_UFS_PHY_AXI_CLK		11
#define GCC_AGGRE_USB3_PRIM_AXI_CLK		12
#define GCC_BOOT_ROM_AHB_CLK			13
#define GCC_CAMERA_AHB_CLK			14
#define GCC_CAMERA_AXI_CLK			15
#define GCC_CAMERA_THROTTLE_NRT_AXI_CLK		16
#define GCC_CAMERA_THROTTLE_RT_AXI_CLK		17
#define GCC_CAMERA_XO_CLK			18
#define GCC_CE1_AHB_CLK				19
#define GCC_CE1_AXI_CLK				20
#define GCC_CE1_CLK				21
#define GCC_CFG_NOC_USB3_PRIM_AXI_CLK		22
#define GCC_CPUSS_AHB_CLK			23
#define GCC_CPUSS_AHB_CLK_SRC			24
#define GCC_CPUSS_AHB_DIV_CLK_SRC		25
#define GCC_CPUSS_GNOC_CLK			26
#define GCC_CPUSS_RBCPR_CLK			27
#define GCC_DDRSS_GPU_AXI_CLK			28
#define GCC_DISP_AHB_CLK			29
#define GCC_DISP_AXI_CLK			30
#define GCC_DISP_CC_SLEEP_CLK			31
#define GCC_DISP_CC_XO_CLK			32
#define GCC_DISP_GPLL0_CLK			33
#define GCC_DISP_THROTTLE_AXI_CLK		34
#define GCC_DISP_XO_CLK				35
#define GCC_GP1_CLK				36
#define GCC_GP1_CLK_SRC				37
#define GCC_GP2_CLK				38
#define GCC_GP2_CLK_SRC				39
#define GCC_GP3_CLK				40
#define GCC_GP3_CLK_SRC				41
#define GCC_GPU_CFG_AHB_CLK			42
#define GCC_GPU_GPLL0_CLK			43
#define GCC_GPU_GPLL0_DIV_CLK			44
#define GCC_GPU_MEMNOC_GFX_CLK			45
#define GCC_GPU_SNOC_DVM_GFX_CLK		46
#define GCC_NPU_AXI_CLK				47
#define GCC_NPU_BWMON_AXI_CLK			48
#define GCC_NPU_BWMON_DMA_CFG_AHB_CLK		49
#define GCC_NPU_BWMON_DSP_CFG_AHB_CLK		50
#define GCC_NPU_CFG_AHB_CLK			51
#define GCC_NPU_DMA_CLK				52
#define GCC_NPU_GPLL0_CLK			53
#define GCC_NPU_GPLL0_DIV_CLK			54
#define GCC_PCIE_0_AUX_CLK			55
#define GCC_PCIE_0_AUX_CLK_SRC			56
#define GCC_PCIE_0_CFG_AHB_CLK			57
#define GCC_PCIE_0_MSTR_AXI_CLK			58
#define GCC_PCIE_0_PIPE_CLK			59
#define GCC_PCIE_0_SLV_AXI_CLK			60
#define GCC_PCIE_0_SLV_Q2A_AXI_CLK		61
#define GCC_PCIE_PHY_RCHNG_CLK			62
#define GCC_PCIE_PHY_RCHNG_CLK_SRC		63
#define GCC_PDM2_CLK				64
#define GCC_PDM2_CLK_SRC			65
#define GCC_PDM_AHB_CLK				66
#define GCC_PDM_XO4_CLK				67
#define GCC_PRNG_AHB_CLK			68
#define GCC_QUPV3_WRAP0_CORE_2X_CLK		69
#define GCC_QUPV3_WRAP0_CORE_CLK		70
#define GCC_QUPV3_WRAP0_S0_CLK			71
#define GCC_QUPV3_WRAP0_S0_CLK_SRC		72
#define GCC_QUPV3_WRAP0_S1_CLK			73
#define GCC_QUPV3_WRAP0_S1_CLK_SRC		74
#define GCC_QUPV3_WRAP0_S2_CLK			75
#define GCC_QUPV3_WRAP0_S2_CLK_SRC		76
#define GCC_QUPV3_WRAP0_S3_CLK			77
#define GCC_QUPV3_WRAP0_S3_CLK_SRC		78
#define GCC_QUPV3_WRAP0_S4_CLK			79
#define GCC_QUPV3_WRAP0_S4_CLK_SRC		80
#define GCC_QUPV3_WRAP0_S5_CLK			81
#define GCC_QUPV3_WRAP0_S5_CLK_SRC		82
#define GCC_QUPV3_WRAP1_CORE_2X_CLK		83
#define GCC_QUPV3_WRAP1_CORE_CLK		84
#define GCC_QUPV3_WRAP1_S0_CLK			85
#define GCC_QUPV3_WRAP1_S0_CLK_SRC		86
#define GCC_QUPV3_WRAP1_S1_CLK			87
#define GCC_QUPV3_WRAP1_S1_CLK_SRC		88
#define GCC_QUPV3_WRAP1_S2_CLK			89
#define GCC_QUPV3_WRAP1_S2_CLK_SRC		90
#define GCC_QUPV3_WRAP1_S3_CLK			91
#define GCC_QUPV3_WRAP1_S3_CLK_SRC		92
#define GCC_QUPV3_WRAP1_S4_CLK			93
#define GCC_QUPV3_WRAP1_S4_CLK_SRC		94
#define GCC_QUPV3_WRAP1_S5_CLK			95
#define GCC_QUPV3_WRAP1_S5_CLK_SRC		96
#define GCC_QUPV3_WRAP_0_M_AHB_CLK		97
#define GCC_QUPV3_WRAP_0_S_AHB_CLK		98
#define GCC_QUPV3_WRAP_1_M_AHB_CLK		99
#define GCC_QUPV3_WRAP_1_S_AHB_CLK		100
#define GCC_SDCC1_AHB_CLK			101
#define GCC_SDCC1_APPS_CLK			102
#define GCC_SDCC1_APPS_CLK_SRC			103
#define GCC_SDCC1_ICE_CORE_CLK			104
#define GCC_SDCC1_ICE_CORE_CLK_SRC		105
#define GCC_SDCC2_AHB_CLK			106
#define GCC_SDCC2_APPS_CLK			107
#define GCC_SDCC2_APPS_CLK_SRC			108
#define GCC_SYS_NOC_CPUSS_AHB_CLK		109
#define GCC_UFS_MEM_CLKREF_CLK			110
#define GCC_UFS_PHY_AHB_CLK			111
#define GCC_UFS_PHY_AXI_CLK			112
#define GCC_UFS_PHY_AXI_CLK_SRC			113
#define GCC_UFS_PHY_ICE_CORE_CLK		114
#define GCC_UFS_PHY_ICE_CORE_CLK_SRC		115
#define GCC_UFS_PHY_PHY_AUX_CLK			116
#define GCC_UFS_PHY_PHY_AUX_CLK_SRC		117
#define GCC_UFS_PHY_RX_SYMBOL_0_CLK		118
#define GCC_UFS_PHY_RX_SYMBOL_1_CLK		119
#define GCC_UFS_PHY_TX_SYMBOL_0_CLK		120
#define GCC_UFS_PHY_UNIPRO_CORE_CLK		121
#define GCC_UFS_PHY_UNIPRO_CORE_CLK_SRC		122
#define GCC_USB30_PRIM_MASTER_CLK		123
#define GCC_USB30_PRIM_MASTER_CLK_SRC		124
#define GCC_USB30_PRIM_MOCK_UTMI_CLK		125
#define GCC_USB30_PRIM_MOCK_UTMI_CLK_SRC	126
#define GCC_USB30_PRIM_MOCK_UTMI_DIV_CLK_SRC	127
#define GCC_USB3_PRIM_CLKREF_CLK		128
#define GCC_USB30_PRIM_SLEEP_CLK		129
#define GCC_USB3_PRIM_PHY_AUX_CLK		130
#define GCC_USB3_PRIM_PHY_AUX_CLK_SRC		131
#define GCC_USB3_PRIM_PHY_COM_AUX_CLK		132
#define GCC_USB3_PRIM_PHY_PIPE_CLK		133
#define GCC_VIDEO_AHB_CLK			134
#define GCC_VIDEO_AXI_CLK			135
#define GCC_VIDEO_THROTTLE_AXI_CLK		136
#define GCC_VIDEO_XO_CLK			137
#define GCC_UFS_PHY_PHY_AUX_HW_CTL_CLK		138
#define GCC_UFS_PHY_AXI_HW_CTL_CLK		139
#define GCC_AGGRE_UFS_PHY_AXI_HW_CTL_CLK	140
#define GCC_UFS_PHY_UNIPRO_CORE_HW_CTL_CLK	141
#define GCC_UFS_PHY_ICE_CORE_HW_CTL_CLK		142
#define GCC_RX5_PCIE_CLKREF_CLK			143
#define GCC_GPU_GPLL0_MAIN_DIV_CLK_SRC		144

/* GCC resets */
#define GCC_QUSB2PHY_PRIM_BCR			0
#define GCC_QUSB2PHY_SEC_BCR			1
#define GCC_SDCC1_BCR				2
#define GCC_SDCC2_BCR				3
#define GCC_UFS_PHY_BCR				4
#define GCC_USB30_PRIM_BCR			5
#define GCC_PCIE_0_BCR				6
#define GCC_PCIE_0_PHY_BCR			7
#define GCC_QUPV3_WRAPPER_0_BCR			8
#define GCC_QUPV3_WRAPPER_1_BCR			9
#define GCC_USB3_PHY_PRIM_BCR			10
#define GCC_USB3_DP_PHY_PRIM_BCR		11

#endif
