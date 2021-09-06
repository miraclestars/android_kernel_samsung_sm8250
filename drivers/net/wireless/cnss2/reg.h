/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2019-2021, The Linux Foundation. All rights reserved. */

#ifndef _CNSS_REG_H
#define _CNSS_REG_H

#define QCA6390_PCIE_REMAP_BAR_CTRL_OFFSET	0x310C

#define QCA6390_CE_SRC_RING_REG_BASE		0xA00000
#define QCA6390_CE_DST_RING_REG_BASE		0xA01000
#define QCA6390_CE_COMMON_REG_BASE		0xA18000

#define QCA6390_CE_SRC_RING_BASE_LSB_OFFSET	0x0
#define QCA6390_CE_SRC_RING_BASE_MSB_OFFSET	0x4
#define QCA6390_CE_SRC_RING_ID_OFFSET		0x8
#define QCA6390_CE_SRC_RING_MISC_OFFSET		0x10
#define QCA6390_CE_SRC_CTRL_OFFSET		0x58
#define QCA6390_CE_SRC_R0_CE_CH_SRC_IS_OFFSET	0x5C
#define QCA6390_CE_SRC_RING_HP_OFFSET		0x400
#define QCA6390_CE_SRC_RING_TP_OFFSET		0x404

#define QCA6390_CE_DEST_RING_BASE_LSB_OFFSET	0x0
#define QCA6390_CE_DEST_RING_BASE_MSB_OFFSET	0x4
#define QCA6390_CE_DEST_RING_ID_OFFSET		0x8
#define QCA6390_CE_DEST_RING_MISC_OFFSET	0x10
#define QCA6390_CE_DEST_CTRL_OFFSET		0xB0
#define QCA6390_CE_CH_DST_IS_OFFSET		0xB4
#define QCA6390_CE_CH_DEST_CTRL2_OFFSET		0xB8
#define QCA6390_CE_DEST_RING_HP_OFFSET		0x400
#define QCA6390_CE_DEST_RING_TP_OFFSET		0x404

#define QCA6390_CE_STATUS_RING_BASE_LSB_OFFSET	0x58
#define QCA6390_CE_STATUS_RING_BASE_MSB_OFFSET	0x5C
#define QCA6390_CE_STATUS_RING_ID_OFFSET	0x60
#define QCA6390_CE_STATUS_RING_MISC_OFFSET	0x68
#define QCA6390_CE_STATUS_RING_HP_OFFSET	0x408
#define QCA6390_CE_STATUS_RING_TP_OFFSET	0x40C

#define QCA6390_CE_COMMON_GXI_ERR_INTS		0x14
#define QCA6390_CE_COMMON_GXI_ERR_STATS		0x18
#define QCA6390_CE_COMMON_GXI_WDOG_STATUS	0x2C
#define QCA6390_CE_COMMON_TARGET_IE_0		0x48
#define QCA6390_CE_COMMON_TARGET_IE_1		0x4C

#define QCA6390_CE_REG_INTERVAL			0x2000

#define SHADOW_REG_COUNT			36
#define QCA6390_PCIE_SHADOW_REG_VALUE_0		0x8FC
#define QCA6390_PCIE_SHADOW_REG_VALUE_34	0x984
#define QCA6390_PCIE_SHADOW_REG_VALUE_35	0x988
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL3	0x1F80118
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL4	0x1F8011C
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL5	0x1F80120

#define SHADOW_REG_INTER_COUNT			43
#define QCA6390_PCIE_SHADOW_REG_INTER_0		0x1E05000
#define QCA6390_PCIE_SHADOW_REG_HUNG		0x1E050A8

#define QDSS_APB_DEC_CSR_BASE			0x1C01000

#define QDSS_APB_DEC_CSR_ETRIRQCTRL_OFFSET	0x6C
#define QDSS_APB_DEC_CSR_PRESERVEETF_OFFSET	0x70
#define QDSS_APB_DEC_CSR_PRESERVEETR0_OFFSET	0x74
#define QDSS_APB_DEC_CSR_PRESERVEETR1_OFFSET	0x78

#define MAX_UNWINDOWED_ADDRESS			0x80000
#define WINDOW_ENABLE_BIT			0x40000000
#define WINDOW_SHIFT				19
#define WINDOW_VALUE_MASK			0x3F
#define WINDOW_START				MAX_UNWINDOWED_ADDRESS
#define WINDOW_RANGE_MASK			0x7FFFF

#define QCA6390_TIME_SYNC_ENABLE		0x80000000
#define QCA6390_TIME_SYNC_CLEAR			0x0

#define QCA6390_PCIE_SOC_WDOG_DISC_BAD_DATA_LOW_CFG_SOC_PCIE_REG 0x01E04234
#define QCA6390_PCIE_SOC_WDOG_DISC_BAD_DATA_LOW_CFG_SOC_PCIE_REG_VAL 0xDEAD1234
#define QCA6390_PCIE_PCIE_WCSS_STATUS_FOR_DEBUG_LOW_PCIE_LOCAL_REG 0x01E03140
#define QCA6390_PCIE_SOC_PCIE_WRAP_INTR_MASK_SOC_PCIE_REG 0x1E04054
#define QCA6390_PCIE_SOC_PCIE_WRAP_INTR_STATUS_SOC_PCIE_REG 0x1E04058
#define QCA6390_PCIE_SOC_COMMIT_REPLAY_SOC_PCIE_REG 0x1E05090
#define QCA6390_PCIE_PCIE_PARF_LTSSM 0x01E081B0
#define QCA6390_PCIE_PCIE_PARF_PM_STTS 0x01E08024
#define QCA6390_PCIE_PCIE_PARF_PM_STTS_1 0x01E08028
#define QCA6390_PCIE_PCIE_PARF_INT_STATUS 0x01E08220
#define QCA6390_PCIE_PCIE_INT_ALL_STATUS 0x01E08224
#define QCA6390_PCIE_PCIE_INT_ALL_MASK 0x01E0822C
#define QCA6390_PCIE_PCIE_PARF_BDF_TO_SID_CFG 0x01E0AC00
#define QCA6390_PCIE_PCIE_PARF_L1SS_SLEEP_NO_MHI_ACCESS_HANDLER_RD_4 0x01E08530
#define QCA6390_PCIE_PCIE_PARF_L1SS_SLEEP_NO_MHI_ACCESS_HANDLER_RD_3 0x01E0852c
#define QCA6390_PCIE_PCIE_PARF_MHI_CLOCK_RESET_CTRL 0x01E08174
#define QCA6390_PCIE_PCIE_PARF_MHI_BASE_ADDR_LOWER 0x01E08178
#define QCA6390_PCIE_PCIE_PARF_L1SS_SLEEP_MODE_HANDLER_STATUS 0x01E084D0
#define QCA6390_PCIE_PCIE_PARF_L1SS_SLEEP_MODE_HANDLER_CFG 0x01E084d4
#define QCA6390_PCIE_PCIE_PARF_DEBUG_CNT_AUX_CLK_IN_L1SUB_L2 0x01E0ec88
#define QCA6390_PCIE_PCIE_PARF_DEBUG_CNT_PM_LINKST_IN_L1SUB 0x01E0ec08
#define QCA6390_PCIE_PCIE_CORE_CONFIG 0x01E08640
#define QCA6390_PCIE_PCIE_PARF_DEBUG_CNT_PM_LINKST_IN_L2 0x01E0EC04
#define QCA6390_PCIE_PCIE_PARF_DEBUG_CNT_PM_LINKST_IN_L1 0x01E0EC0C
#define QCA6390_PCIE_PCIE_PARF_DEBUG_CNT_AUX_CLK_IN_L1SUB_L1 0x01E0EC84
#define QCA6390_PCIE_PCIE_LOCAL_REG_WCSSAON_PCIE_SR_STATUS_HIGH 0x01E030C8
#define QCA6390_PCIE_PCIE_LOCAL_REG_WCSSAON_PCIE_SR_STATUS_LOW 0x01E030CC
#define QCA6390_PCIE_PCIE_LOCAL_REG_WCSS_STATUS_FOR_DEBUG_HIGH 0x01E0313C
#define QCA6390_PCIE_PCIE_LOCAL_REG_WCSS_STATUS_FOR_DEBUG_LOW 0x01E03140
#define QCA6390_PCIE_PCIE_BHI_EXECENV_REG 0x01E0E228

#define QCA6390_GCC_DEBUG_CLK_CTL 0x001E4025C

#define QCA6390_WCSS_Q6SS_PUBCSR_QDSP6SS_PLL_MODE 0x00D00200
#define QCA6390_WCSS_WFSS_PMM_WFSS_PMM_R0_PMM_CTRL 0x00B60164
#define QCA6390_WCSS_PMM_TOP_PMU_CX_CSR 0x00B70080
#define QCA6390_WCSS_PMM_TOP_AON_INT_RAW_STAT 0x00B700E0
#define QCA6390_WCSS_PMM_TOP_AON_INT_EN 0x00B700D0
#define QCA6390_WCSS_PMM_TOP_PMU_TESTBUS_STS 0x00B70020
#define QCA6390_WCSS_PMM_TOP_PMU_TESTBUS_CTL 0x00B7001C
#define QCA6390_WCSS_PMM_TOP_TESTBUS_STS 0x00B70028
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_CFG 0x00DB0008
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_CFG_MSK 0x20
#define QCA6390_WCSS_Q6SS_PUBCSR_QDSP6SS_TEST_BUS_CTL 0x00D02000
#define QCA6390_WCSS_Q6SS_PUBCSR_QDSP6SS_TEST_BUS_VALUE 0x00D02004
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_SPM_STS 0x00DB000C
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_SPM_CTL 0x00DB0030
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_SPM_SLP_SEQ_ENTRY_0 0x00DB0400
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_SAW2_SPM_SLP_SEQ_ENTRY_9 0x00DB0424
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS0 0x00D90380
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS1 0x00D90384
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS2 0x00D90388
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS3 0x00D9038C
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS4 0x00D90390
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS5 0x00D90394
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_STATUS6 0x00D90398
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE0 0x00D90100
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE1 0x00D90104
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE2 0x00D90108
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE3 0x00D9010C
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE4 0x00D90110
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE5 0x00D90114
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_ENABLE6 0x00D90118
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING0 0x00D90500
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING1 0x00D90504
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING2 0x00D90508
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING3 0x00D9050C
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING4 0x00D90510
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING5 0x00D90514
#define QCA6390_WCSS_Q6SS_PRIVCSR_QDSP6SS_L2VIC_INT_PENDING6 0x00D90518
#define QCA6390_WCSS_CC_WCSS_UMAC_NOC_CBCR 0x00C3029C
#define QCA6390_WCSS_CC_WCSS_UMAC_AHB_CBCR 0x00C302BC
#define QCA6390_WCSS_CC_WCSS_UMAC_GDSCR 0x00C30298
#define QCA6390_WCSS_CC_WCSS_WLAN1_GDSCR 0x00C300C4
#define QCA6390_WCSS_CC_WCSS_WLAN2_GDSCR 0x00C30138
#define QCA6390_WCSS_PMM_TOP_PMM_INT_CLR 0x00B70168
#define QCA6390_WCSS_PMM_TOP_AON_INT_STICKY_EN 0x00B700D8

#define QCA6390_TLMM_GPIO_IN_OUT57 0x01839004
#define QCA6390_TLMM_GPIO_INTR_CFG57 0x01839008
#define QCA6390_TLMM_GPIO_INTR_STATUS57 0x0183900C
#define QCA6390_TLMM_GPIO_IN_OUT59 0x0183b004
#define QCA6390_TLMM_GPIO_INTR_CFG59 0x0183b008
#define QCA6390_TLMM_GPIO_INTR_STATUS59 0x0183b00C

#define QCA6390_WFSS_PMM_WFSS_PMM_R0_WLAN1_STATUS_REG2 0x00B6017C
#define QCA6390_WFSS_PMM_WFSS_PMM_R0_WLAN2_STATUS_REG2 0x00B60190
#define QCA6390_WFSS_PMM_WFSS_PMM_R0_PMM_WLAN2_CFG_REG1 0x00B6018C
#define QCA6390_WFSS_PMM_WFSS_PMM_R0_PMM_WLAN1_CFG_REG1 0x00B60178
#define QCA6390_WFSS_PMM_WFSS_PMM_R0_WLAN2_APS_STATUS_REG1 0x00B600B0
#define QCA6390_WFSS_PMM_WFSS_PMM_R0_WLAN1_APS_STATUS_REG1 0x00B60044

#define QCA6390_WLAON_WCSSAON_PCIE_SR_STATUS_HI_REG 0x01F806C4
#define QCA6390_WLAON_SOC_POWER_CTRL 0x01F80000
#define QCA6390_WLAON_PCIE_PWR_CTRL_REG 0x01F806BC
#define QCA6390_WLAON_WCSSAON_PCIE_SR_STATUS_LO_REG 0x1F806C8
#define QCA6390_WLAON_WCSS_TCSR_PMM_SR_STATUS_HI_REG 0x1F806CC
#define QCA6390_WLAON_WCSS_TCSR_PMM_SR_STATUS_LO_REG 0x1F806D0
#define QCA6390_WLAON_SOC_PWR_WDG_BARK_THRSHD 0x1F80004
#define QCA6390_WLAON_SOC_PWR_WDG_BITE_THRSHD 0x1F80008
#define QCA6390_WLAON_SW_COLD_RESET 0x1F8000C
#define QCA6390_WLAON_RFA_MEM_SLP_NRET_N_OVERRIDE 0x1F8001C
#define QCA6390_WLAON_GDSC_DELAY_SETTING 0x1F80024
#define QCA6390_WLAON_GDSC_DELAY_SETTING2 0x1F80028
#define QCA6390_WLAON_WL_PWR_STATUS_REG 0x1F8002C
#define QCA6390_WLAON_WL_AON_DBG_CFG_REG 0x1F80030
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL1 0x1F80100
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL6 0x1F80108
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL7 0x1F8010C
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL3 0x1F80118
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL4 0x1F8011C
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL5 0x1F80120
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL8 0x1F801F0
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL2 0x1F801F4
#define QCA6390_WLAON_GLOBAL_COUNTER_CTRL9 0x1F801F8
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL1 0x1F80200
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL2 0x1F80204
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL3 0x1F80208
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL4 0x1F8020C
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL5 0x1F80210
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL6 0x1F80214
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL7 0x1F80218
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL8 0x1F8021C
#define QCA6390_WLAON_RTC_CLK_CAL_CTRL9 0x1F80220
#define QCA6390_WLAON_WCSSAON_CONFIG_REG 0x1F80300
#define QCA6390_WLAON_WLAN_OEM_DEBUG_REG 0x1F80304
#define QCA6390_WLAON_WLAN_RAM_DUMP_REG 0x1F80308
#define QCA6390_WLAON_QDSS_WCSS_REG 0x1F8030C
#define QCA6390_WLAON_QDSS_WCSS_ACK 0x1F80310
#define QCA6390_WLAON_WL_CLK_CNTL_KDF_REG 0x1F80314
#define QCA6390_WLAON_WL_CLK_CNTL_PMU_HFRC_REG 0x1F80318
#define QCA6390_WLAON_QFPROM_PWR_CTRL_REG 0x1F8031C
#define QFPROM_PWR_CTRL_VDD4BLOW_SW_EN_MASK 0x4
#define QFPROM_PWR_CTRL_SHUTDOWN_EN_MASK 0x1
#define QCA6390_WLAON_DLY_CONFIG 0x1F80400
#define QCA6390_WLAON_WLAON_Q6_IRQ_REG 0x1F80404
#define QCA6390_WLAON_PCIE_INTF_SW_CFG_REG 0x1F80408
#define QCA6390_WLAON_PCIE_INTF_STICKY_SW_CFG_REG 0x1F8040C
#define QCA6390_WLAON_PCIE_INTF_PHY_SW_CFG_REG 0x1F80410
#define QCA6390_WLAON_PCIE_INTF_PHY_NOCSR_SW_CFG_REG 0x1F80414
#define QCA6390_WLAON_Q6_COOKIE_BIT 0x1F80500
#define QCA6390_WLAON_WARM_SW_ENTRY 0x1F80504
#define QCA6390_WLAON_RESET_DBG_SW_ENTRY 0x1F80508
#define QCA6390_WLAON_WL_PMUNOC_CFG_REG 0x1F8050C
#define QCA6390_WLAON_RESET_CAUSE_CFG_REG 0x1F80510
#define QCA6390_WLAON_SOC_WCSSAON_WAKEUP_IRQ_7_EN_REG 0x1F80514
#define QCA6390_WLAON_DEBUG 0x1F80600
#define QCA6390_WLAON_SOC_PARAMETERS 0x1F80604
#define QCA6390_WLAON_WLPM_SIGNAL 0x1F80608
#define QCA6390_WLAON_SOC_RESET_CAUSE_REG 0x1F8060C
#define QCA6390_WLAON_WAKEUP_PCIE_SOC_REG 0x1F80610
#define QCA6390_WLAON_PBL_STACK_CANARY 0x1F80614
#define QCA6390_WLAON_MEM_TOT_NUM_GRP_REG 0x1F80618
#define QCA6390_WLAON_MEM_TOT_BANKS_IN_GRP0_REG 0x1F8061C
#define QCA6390_WLAON_MEM_TOT_BANKS_IN_GRP1_REG 0x1F80620
#define QCA6390_WLAON_MEM_TOT_BANKS_IN_GRP2_REG 0x1F80624
#define QCA6390_WLAON_MEM_TOT_BANKS_IN_GRP3_REG 0x1F80628
#define QCA6390_WLAON_MEM_TOT_SIZE_IN_GRP0_REG 0x1F8062C
#define QCA6390_WLAON_MEM_TOT_SIZE_IN_GRP1_REG 0x1F80630
#define QCA6390_WLAON_MEM_TOT_SIZE_IN_GRP2_REG 0x1F80634
#define QCA6390_WLAON_MEM_TOT_SIZE_IN_GRP3_REG 0x1F80638
#define QCA6390_WLAON_MEM_SLP_NRET_OVERRIDE_GRP0_REG 0x1F8063C
#define QCA6390_WLAON_MEM_SLP_NRET_OVERRIDE_GRP1_REG 0x1F80640
#define QCA6390_WLAON_MEM_SLP_NRET_OVERRIDE_GRP2_REG 0x1F80644
#define QCA6390_WLAON_MEM_SLP_NRET_OVERRIDE_GRP3_REG 0x1F80648
#define QCA6390_WLAON_MEM_SLP_RET_OVERRIDE_GRP0_REG 0x1F8064C
#define QCA6390_WLAON_MEM_SLP_RET_OVERRIDE_GRP1_REG 0x1F80650
#define QCA6390_WLAON_MEM_SLP_RET_OVERRIDE_GRP2_REG 0x1F80654
#define QCA6390_WLAON_MEM_SLP_RET_OVERRIDE_GRP3_REG 0x1F80658
#define QCA6390_WLAON_MEM_CNT_SEL_REG 0x1F8065C
#define QCA6390_WLAON_MEM_NO_EXTBHS_REG 0x1F80660
#define QCA6390_WLAON_MEM_DEBUG_REG 0x1F80664
#define QCA6390_WLAON_MEM_DEBUG_BUS_REG 0x1F80668
#define QCA6390_WLAON_MEM_REDUN_CFG_REG 0x1F8066C
#define QCA6390_WLAON_WL_AON_SPARE2 0x1F80670
#define QCA6390_WLAON_VSEL_CFG_FOR_WL_RET_DISABLE_REG 0x1F80680
#define QCA6390_WLAON_BTFM_WLAN_IPC_STATUS_REG 0x1F80690
#define QCA6390_WLAON_MPM_COUNTER_CHICKEN_BITS 0x1F806A0
#define QCA6390_WLAON_WLPM_CHICKEN_BITS 0x1F806A4
#define QCA6390_WLAON_PCIE_PHY_PWR_REG 0x1F806A8
#define QCA6390_WLAON_WL_CLK_CNTL_PMU_LPO2M_REG 0x1F806AC
#define QCA6390_WLAON_WL_SS_ROOT_CLK_SWITCH_REG 0x1F806B0
#define QCA6390_WLAON_POWERCTRL_PMU_REG 0x1F806B4
#define QCA6390_WLAON_POWERCTRL_MEM_REG 0x1F806B8
#define QCA6390_WLAON_SOC_PWR_PROFILE_REG 0x1F806C0
#define QCA6390_WLAON_MEM_SVS_CFG_REG 0x1F806D4
#define QCA6390_WLAON_CMN_AON_MISC_REG 0x1F806D8
#define QCA6390_WLAON_INTR_STATUS 0x1F80700

#define QCA6390_SYSPM_SYSPM_PWR_STATUS 0x1F82000
#define QCA6390_SYSPM_DBG_BTFM_AON_REG 0x1F82004
#define QCA6390_SYSPM_DBG_BUS_SEL_REG 0x1F82008
#define QCA6390_SYSPM_WCSSAON_SR_STATUS 0x1F8200C

#define QCA6490_DEBUG_PBL_LOG_SRAM_START 0x01403DA0
#define QCA6490_DEBUG_PBL_LOG_SRAM_MAX_SIZE 40
#define QCA6490_DEBUG_SBL_LOG_SRAM_MAX_SIZE 48
#define QCA6490_TCSR_PBL_LOGGING_REG 0x01B000F8
#define QCA6490_PCIE_BHI_ERRDBG2_REG 0x01E0E238
#define QCA6490_PCIE_BHI_ERRDBG3_REG 0x01E0E23C
#define QCA6490_PBL_WLAN_BOOT_CFG 0x01E22B34
#define QCA6490_PBL_BOOTSTRAP_STATUS 0x01910008

#define QCA6390_DEBUG_PBL_LOG_SRAM_START 0x01403D58
#define QCA6390_DEBUG_PBL_LOG_SRAM_MAX_SIZE 80
#define QCA6390_DEBUG_SBL_LOG_SRAM_MAX_SIZE 44
#define QCA6390_TCSR_PBL_LOGGING_REG 0x01B000F8
#define QCA6390_PCIE_BHI_ERRDBG2_REG 0x01E0E238
#define QCA6390_PCIE_BHI_ERRDBG3_REG 0x01E0E23C
#define QCA6390_PBL_WLAN_BOOT_CFG    0x01E22B34
#define QCA6390_PBL_BOOTSTRAP_STATUS 0x01910008

#define SRAM_START		0x01400000
#define SRAM_END		0x01800000

/* PCIE SOC scratch registers, address same for QCA6390 & QCA6490*/
#define PCIE_SCRATCH_0_SOC_PCIE_REG 0x1E04040
#define PCIE_SCRATCH_1_SOC_PCIE_REG 0x1E04044
#define PCIE_SCRATCH_2_SOC_PCIE_REG 0x1E0405C
#endif
