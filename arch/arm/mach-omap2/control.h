/*
 * arch/arm/mach-omap2/control.h
 *
 * OMAP2/3/4 System Control Module definitions
 *
 * Copyright (C) 2007-2010 Texas Instruments, Inc.
 * Copyright (C) 2007-2008, 2010 Nokia Corporation
 *
 * Written by Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_CONTROL_H
#define __ARCH_ARM_MACH_OMAP2_CONTROL_H

#include "am33xx.h"

#ifndef __ASSEMBLY__
#define OMAP242X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP242X_CTRL_BASE + (reg))
#define OMAP243X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP243X_CTRL_BASE + (reg))
#define OMAP343X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP343X_CTRL_BASE + (reg))
#define AM33XX_CTRL_REGADDR(reg)					\
		AM33XX_L4_WK_IO_ADDRESS(AM33XX_SCM_BASE + (reg))
#else
#define OMAP242X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP242X_CTRL_BASE + (reg))
#define OMAP243X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP243X_CTRL_BASE + (reg))
#define OMAP343X_CTRL_REGADDR(reg)					\
		OMAP2_L4_IO_ADDRESS(OMAP343X_CTRL_BASE + (reg))
#define AM33XX_CTRL_REGADDR(reg)					\
		AM33XX_L4_WK_IO_ADDRESS(AM33XX_SCM_BASE + (reg))
#endif /* __ASSEMBLY__ */

/*
 * As elsewhere, the "OMAP2_" prefix indicates that the macro is valid for
 * OMAP24XX and OMAP34XX.
 */

/* Control submodule offsets */

#define OMAP2_CONTROL_INTERFACE		0x000
#define OMAP2_CONTROL_PADCONFS		0x030
#define OMAP2_CONTROL_GENERAL		0x270
#define OMAP343X_CONTROL_MEM_WKUP	0x600
#define OMAP343X_CONTROL_PADCONFS_WKUP	0xa00
#define OMAP343X_CONTROL_GENERAL_WKUP	0xa60

/* TI81XX spefic control submodules */
#define TI81XX_CONTROL_DEVBOOT		0x040
#define TI81XX_CONTROL_DEVCONF		0x600

/* Control register offsets - read/write with omap_ctrl_{read,write}{bwl}() */

#define OMAP2_CONTROL_SYSCONFIG		(OMAP2_CONTROL_INTERFACE + 0x10)

/* CONTROL_GENERAL register offsets common to OMAP2 & 3 */
#define OMAP2_CONTROL_DEVCONF0		(OMAP2_CONTROL_GENERAL + 0x0004)
#define OMAP2_CONTROL_MSUSPENDMUX_0	(OMAP2_CONTROL_GENERAL + 0x0020)
#define OMAP2_CONTROL_MSUSPENDMUX_1	(OMAP2_CONTROL_GENERAL + 0x0024)
#define OMAP2_CONTROL_MSUSPENDMUX_2	(OMAP2_CONTROL_GENERAL + 0x0028)
#define OMAP2_CONTROL_MSUSPENDMUX_3	(OMAP2_CONTROL_GENERAL + 0x002c)
#define OMAP2_CONTROL_MSUSPENDMUX_4	(OMAP2_CONTROL_GENERAL + 0x0030)
#define OMAP2_CONTROL_MSUSPENDMUX_5	(OMAP2_CONTROL_GENERAL + 0x0034)
#define OMAP2_CONTROL_SEC_CTRL		(OMAP2_CONTROL_GENERAL + 0x0040)
#define OMAP2_CONTROL_RPUB_KEY_H_0	(OMAP2_CONTROL_GENERAL + 0x0090)
#define OMAP2_CONTROL_RPUB_KEY_H_1	(OMAP2_CONTROL_GENERAL + 0x0094)
#define OMAP2_CONTROL_RPUB_KEY_H_2	(OMAP2_CONTROL_GENERAL + 0x0098)
#define OMAP2_CONTROL_RPUB_KEY_H_3	(OMAP2_CONTROL_GENERAL + 0x009c)

/* 242x-only CONTROL_GENERAL register offsets */
#define OMAP242X_CONTROL_DEVCONF	OMAP2_CONTROL_DEVCONF0 /* match TRM */
#define OMAP242X_CONTROL_OCM_RAM_PERM	(OMAP2_CONTROL_GENERAL + 0x0068)

/* 243x-only CONTROL_GENERAL register offsets */
/* CONTROL_IVA2_BOOT{ADDR,MOD} are at the same place on 343x - noted below */
#define OMAP243X_CONTROL_DEVCONF1	(OMAP2_CONTROL_GENERAL + 0x0078)
#define OMAP243X_CONTROL_CSIRXFE	(OMAP2_CONTROL_GENERAL + 0x007c)
#define OMAP243X_CONTROL_IVA2_BOOTADDR	(OMAP2_CONTROL_GENERAL + 0x0190)
#define OMAP243X_CONTROL_IVA2_BOOTMOD	(OMAP2_CONTROL_GENERAL + 0x0194)
#define OMAP243X_CONTROL_IVA2_GEMCFG	(OMAP2_CONTROL_GENERAL + 0x0198)
#define OMAP243X_CONTROL_PBIAS_LITE	(OMAP2_CONTROL_GENERAL + 0x0230)

/* 24xx-only CONTROL_GENERAL register offsets */
#define OMAP24XX_CONTROL_DEBOBS		(OMAP2_CONTROL_GENERAL + 0x0000)
#define OMAP24XX_CONTROL_EMU_SUPPORT	(OMAP2_CONTROL_GENERAL + 0x0008)
#define OMAP24XX_CONTROL_SEC_TEST	(OMAP2_CONTROL_GENERAL + 0x0044)
#define OMAP24XX_CONTROL_PSA_CTRL	(OMAP2_CONTROL_GENERAL + 0x0048)
#define OMAP24XX_CONTROL_PSA_CMD	(OMAP2_CONTROL_GENERAL + 0x004c)
#define OMAP24XX_CONTROL_PSA_VALUE	(OMAP2_CONTROL_GENERAL + 0x0050)
#define OMAP24XX_CONTROL_SEC_EMU	(OMAP2_CONTROL_GENERAL + 0x0060)
#define OMAP24XX_CONTROL_SEC_TAP	(OMAP2_CONTROL_GENERAL + 0x0064)
#define OMAP24XX_CONTROL_OCM_PUB_RAM_ADD	(OMAP2_CONTROL_GENERAL + 0x006c)
#define OMAP24XX_CONTROL_EXT_SEC_RAM_START_ADD	(OMAP2_CONTROL_GENERAL + 0x0070)
#define OMAP24XX_CONTROL_EXT_SEC_RAM_STOP_ADD	(OMAP2_CONTROL_GENERAL + 0x0074)
#define OMAP24XX_CONTROL_SEC_STATUS		(OMAP2_CONTROL_GENERAL + 0x0080)
#define OMAP24XX_CONTROL_SEC_ERR_STATUS		(OMAP2_CONTROL_GENERAL + 0x0084)
#define OMAP24XX_CONTROL_STATUS			(OMAP2_CONTROL_GENERAL + 0x0088)
#define OMAP24XX_CONTROL_GENERAL_PURPOSE_STATUS	(OMAP2_CONTROL_GENERAL + 0x008c)
#define OMAP24XX_CONTROL_RAND_KEY_0	(OMAP2_CONTROL_GENERAL + 0x00a0)
#define OMAP24XX_CONTROL_RAND_KEY_1	(OMAP2_CONTROL_GENERAL + 0x00a4)
#define OMAP24XX_CONTROL_RAND_KEY_2	(OMAP2_CONTROL_GENERAL + 0x00a8)
#define OMAP24XX_CONTROL_RAND_KEY_3	(OMAP2_CONTROL_GENERAL + 0x00ac)
#define OMAP24XX_CONTROL_CUST_KEY_0	(OMAP2_CONTROL_GENERAL + 0x00b0)
#define OMAP24XX_CONTROL_CUST_KEY_1	(OMAP2_CONTROL_GENERAL + 0x00b4)
#define OMAP24XX_CONTROL_TEST_KEY_0	(OMAP2_CONTROL_GENERAL + 0x00c0)
#define OMAP24XX_CONTROL_TEST_KEY_1	(OMAP2_CONTROL_GENERAL + 0x00c4)
#define OMAP24XX_CONTROL_TEST_KEY_2	(OMAP2_CONTROL_GENERAL + 0x00c8)
#define OMAP24XX_CONTROL_TEST_KEY_3	(OMAP2_CONTROL_GENERAL + 0x00cc)
#define OMAP24XX_CONTROL_TEST_KEY_4	(OMAP2_CONTROL_GENERAL + 0x00d0)
#define OMAP24XX_CONTROL_TEST_KEY_5	(OMAP2_CONTROL_GENERAL + 0x00d4)
#define OMAP24XX_CONTROL_TEST_KEY_6	(OMAP2_CONTROL_GENERAL + 0x00d8)
#define OMAP24XX_CONTROL_TEST_KEY_7	(OMAP2_CONTROL_GENERAL + 0x00dc)
#define OMAP24XX_CONTROL_TEST_KEY_8	(OMAP2_CONTROL_GENERAL + 0x00e0)
#define OMAP24XX_CONTROL_TEST_KEY_9	(OMAP2_CONTROL_GENERAL + 0x00e4)

#define OMAP343X_CONTROL_PADCONF_SYSNIRQ (OMAP2_CONTROL_INTERFACE + 0x01b0)

/* 34xx-only CONTROL_GENERAL register offsets */
#define OMAP343X_CONTROL_PADCONF_OFF	(OMAP2_CONTROL_GENERAL + 0x0000)
#define OMAP343X_CONTROL_MEM_DFTRW0	(OMAP2_CONTROL_GENERAL + 0x0008)
#define OMAP343X_CONTROL_MEM_DFTRW1	(OMAP2_CONTROL_GENERAL + 0x000c)
#define OMAP343X_CONTROL_DEVCONF1	(OMAP2_CONTROL_GENERAL + 0x0068)
#define OMAP343X_CONTROL_CSIRXFE		(OMAP2_CONTROL_GENERAL + 0x006c)
#define OMAP343X_CONTROL_SEC_STATUS		(OMAP2_CONTROL_GENERAL + 0x0070)
#define OMAP343X_CONTROL_SEC_ERR_STATUS		(OMAP2_CONTROL_GENERAL + 0x0074)
#define OMAP343X_CONTROL_SEC_ERR_STATUS_DEBUG	(OMAP2_CONTROL_GENERAL + 0x0078)
#define OMAP343X_CONTROL_STATUS			(OMAP2_CONTROL_GENERAL + 0x0080)
#define OMAP343X_CONTROL_GENERAL_PURPOSE_STATUS	(OMAP2_CONTROL_GENERAL + 0x0084)
#define OMAP343X_CONTROL_RPUB_KEY_H_4	(OMAP2_CONTROL_GENERAL + 0x00a0)
#define OMAP343X_CONTROL_RAND_KEY_0	(OMAP2_CONTROL_GENERAL + 0x00a8)
#define OMAP343X_CONTROL_RAND_KEY_1	(OMAP2_CONTROL_GENERAL + 0x00ac)
#define OMAP343X_CONTROL_RAND_KEY_2	(OMAP2_CONTROL_GENERAL + 0x00b0)
#define OMAP343X_CONTROL_RAND_KEY_3	(OMAP2_CONTROL_GENERAL + 0x00b4)
#define OMAP343X_CONTROL_TEST_KEY_0	(OMAP2_CONTROL_GENERAL + 0x00c8)
#define OMAP343X_CONTROL_TEST_KEY_1	(OMAP2_CONTROL_GENERAL + 0x00cc)
#define OMAP343X_CONTROL_TEST_KEY_2	(OMAP2_CONTROL_GENERAL + 0x00d0)
#define OMAP343X_CONTROL_TEST_KEY_3	(OMAP2_CONTROL_GENERAL + 0x00d4)
#define OMAP343X_CONTROL_TEST_KEY_4	(OMAP2_CONTROL_GENERAL + 0x00d8)
#define OMAP343X_CONTROL_TEST_KEY_5	(OMAP2_CONTROL_GENERAL + 0x00dc)
#define OMAP343X_CONTROL_TEST_KEY_6	(OMAP2_CONTROL_GENERAL + 0x00e0)
#define OMAP343X_CONTROL_TEST_KEY_7	(OMAP2_CONTROL_GENERAL + 0x00e4)
#define OMAP343X_CONTROL_TEST_KEY_8	(OMAP2_CONTROL_GENERAL + 0x00e8)
#define OMAP343X_CONTROL_TEST_KEY_9	(OMAP2_CONTROL_GENERAL + 0x00ec)
#define OMAP343X_CONTROL_TEST_KEY_10	(OMAP2_CONTROL_GENERAL + 0x00f0)
#define OMAP343X_CONTROL_TEST_KEY_11	(OMAP2_CONTROL_GENERAL + 0x00f4)
#define OMAP343X_CONTROL_TEST_KEY_12	(OMAP2_CONTROL_GENERAL + 0x00f8)
#define OMAP343X_CONTROL_TEST_KEY_13	(OMAP2_CONTROL_GENERAL + 0x00fc)
#define OMAP343X_CONTROL_FUSE_OPP1_VDD1 (OMAP2_CONTROL_GENERAL + 0x0110)
#define OMAP343X_CONTROL_FUSE_OPP2_VDD1 (OMAP2_CONTROL_GENERAL + 0x0114)
#define OMAP343X_CONTROL_FUSE_OPP3_VDD1 (OMAP2_CONTROL_GENERAL + 0x0118)
#define OMAP343X_CONTROL_FUSE_OPP4_VDD1 (OMAP2_CONTROL_GENERAL + 0x011c)
#define OMAP343X_CONTROL_FUSE_OPP5_VDD1 (OMAP2_CONTROL_GENERAL + 0x0120)
#define OMAP343X_CONTROL_FUSE_OPP1_VDD2 (OMAP2_CONTROL_GENERAL + 0x0124)
#define OMAP343X_CONTROL_FUSE_OPP2_VDD2 (OMAP2_CONTROL_GENERAL + 0x0128)
#define OMAP343X_CONTROL_FUSE_OPP3_VDD2 (OMAP2_CONTROL_GENERAL + 0x012c)
#define OMAP343X_CONTROL_FUSE_SR        (OMAP2_CONTROL_GENERAL + 0x0130)
#define OMAP343X_CONTROL_IVA2_BOOTADDR	(OMAP2_CONTROL_GENERAL + 0x0190)
#define OMAP343X_CONTROL_IVA2_BOOTMOD	(OMAP2_CONTROL_GENERAL + 0x0194)
#define OMAP343X_CONTROL_DEBOBS(i)	(OMAP2_CONTROL_GENERAL + 0x01B0 \
					+ ((i) >> 1) * 4 + (!((i) & 1)) * 2)
#define OMAP343X_CONTROL_PROG_IO0	(OMAP2_CONTROL_GENERAL + 0x01D4)
#define OMAP343X_CONTROL_PROG_IO1	(OMAP2_CONTROL_GENERAL + 0x01D8)
#define OMAP343X_CONTROL_DSS_DPLL_SPREADING	(OMAP2_CONTROL_GENERAL + 0x01E0)
#define OMAP343X_CONTROL_CORE_DPLL_SPREADING	(OMAP2_CONTROL_GENERAL + 0x01E4)
#define OMAP343X_CONTROL_PER_DPLL_SPREADING	(OMAP2_CONTROL_GENERAL + 0x01E8)
#define OMAP343X_CONTROL_USBHOST_DPLL_SPREADING	(OMAP2_CONTROL_GENERAL + 0x01EC)
#define OMAP343X_CONTROL_PBIAS_LITE	(OMAP2_CONTROL_GENERAL + 0x02B0)
#define OMAP343X_CONTROL_TEMP_SENSOR	(OMAP2_CONTROL_GENERAL + 0x02B4)
#define OMAP343X_CONTROL_SRAMLDO4	(OMAP2_CONTROL_GENERAL + 0x02B8)
#define OMAP343X_CONTROL_SRAMLDO5	(OMAP2_CONTROL_GENERAL + 0x02C0)
#define OMAP343X_CONTROL_CSI		(OMAP2_CONTROL_GENERAL + 0x02C4)

/* OMAP3630 only CONTROL_GENERAL register offsets */
#define OMAP3630_CONTROL_FUSE_OPP1G_VDD1        (OMAP2_CONTROL_GENERAL + 0x0110)
#define OMAP3630_CONTROL_FUSE_OPP50_VDD1        (OMAP2_CONTROL_GENERAL + 0x0114)
#define OMAP3630_CONTROL_FUSE_OPP100_VDD1       (OMAP2_CONTROL_GENERAL + 0x0118)
#define OMAP3630_CONTROL_FUSE_OPP120_VDD1       (OMAP2_CONTROL_GENERAL + 0x0120)
#define OMAP3630_CONTROL_FUSE_OPP50_VDD2        (OMAP2_CONTROL_GENERAL + 0x0128)
#define OMAP3630_CONTROL_FUSE_OPP100_VDD2       (OMAP2_CONTROL_GENERAL + 0x012C)
#define OMAP3630_CONTROL_CAMERA_PHY_CTRL	(OMAP2_CONTROL_GENERAL + 0x02f0)

/* OMAP44xx control efuse offsets */
#define OMAP44XX_CONTROL_FUSE_IVA_OPP50		0x22C
#define OMAP44XX_CONTROL_FUSE_IVA_OPP100	0x22F
#define OMAP44XX_CONTROL_FUSE_IVA_OPPTURBO	0x232
#define OMAP44XX_CONTROL_FUSE_IVA_OPPNITRO	0x235
#define OMAP44XX_CONTROL_FUSE_MPU_OPP50		0x240
#define OMAP44XX_CONTROL_FUSE_MPU_OPP100	0x243
#define OMAP44XX_CONTROL_FUSE_MPU_OPPTURBO	0x246
#define OMAP44XX_CONTROL_FUSE_MPU_OPPNITRO	0x249
#define OMAP44XX_CONTROL_FUSE_CORE_OPP50	0x254
#define OMAP44XX_CONTROL_FUSE_CORE_OPP100	0x257
#define OMAP44XX_CONTROL_FUSE_CORE_OPP100OV	0x25A

/* AM35XX only CONTROL_GENERAL register offsets */
#define AM35XX_CONTROL_MSUSPENDMUX_6    (OMAP2_CONTROL_GENERAL + 0x0038)
#define AM35XX_CONTROL_DEVCONF2         (OMAP2_CONTROL_GENERAL + 0x0310)
#define AM35XX_CONTROL_DEVCONF3         (OMAP2_CONTROL_GENERAL + 0x0314)
#define AM35XX_CONTROL_CBA_PRIORITY     (OMAP2_CONTROL_GENERAL + 0x0320)
#define AM35XX_CONTROL_LVL_INTR_CLEAR   (OMAP2_CONTROL_GENERAL + 0x0324)
#define AM35XX_CONTROL_IP_SW_RESET      (OMAP2_CONTROL_GENERAL + 0x0328)
#define AM35XX_CONTROL_IPSS_CLK_CTRL    (OMAP2_CONTROL_GENERAL + 0x032C)

/* 34xx PADCONF register offsets */
#define OMAP343X_PADCONF_ETK(i)		(OMAP2_CONTROL_PADCONFS + 0x5a8 + \
						(i)*2)
#define OMAP343X_PADCONF_ETK_CLK	OMAP343X_PADCONF_ETK(0)
#define OMAP343X_PADCONF_ETK_CTL	OMAP343X_PADCONF_ETK(1)
#define OMAP343X_PADCONF_ETK_D0		OMAP343X_PADCONF_ETK(2)
#define OMAP343X_PADCONF_ETK_D1		OMAP343X_PADCONF_ETK(3)
#define OMAP343X_PADCONF_ETK_D2		OMAP343X_PADCONF_ETK(4)
#define OMAP343X_PADCONF_ETK_D3		OMAP343X_PADCONF_ETK(5)
#define OMAP343X_PADCONF_ETK_D4		OMAP343X_PADCONF_ETK(6)
#define OMAP343X_PADCONF_ETK_D5		OMAP343X_PADCONF_ETK(7)
#define OMAP343X_PADCONF_ETK_D6		OMAP343X_PADCONF_ETK(8)
#define OMAP343X_PADCONF_ETK_D7		OMAP343X_PADCONF_ETK(9)
#define OMAP343X_PADCONF_ETK_D8		OMAP343X_PADCONF_ETK(10)
#define OMAP343X_PADCONF_ETK_D9		OMAP343X_PADCONF_ETK(11)
#define OMAP343X_PADCONF_ETK_D10	OMAP343X_PADCONF_ETK(12)
#define OMAP343X_PADCONF_ETK_D11	OMAP343X_PADCONF_ETK(13)
#define OMAP343X_PADCONF_ETK_D12	OMAP343X_PADCONF_ETK(14)
#define OMAP343X_PADCONF_ETK_D13	OMAP343X_PADCONF_ETK(15)
#define OMAP343X_PADCONF_ETK_D14	OMAP343X_PADCONF_ETK(16)
#define OMAP343X_PADCONF_ETK_D15	OMAP343X_PADCONF_ETK(17)

/* 34xx GENERAL_WKUP register offsets */
#define OMAP34XX_CONTROL_WKUP_CTRL	(OMAP343X_CONTROL_GENERAL_WKUP - 0x4)
#define OMAP36XX_GPIO_IO_PWRDNZ		BIT(6)

#define OMAP343X_CONTROL_WKUP_DEBOBSMUX(i) (OMAP343X_CONTROL_GENERAL_WKUP + \
						0x008 + (i))
#define OMAP343X_CONTROL_WKUP_DEBOBS0 (OMAP343X_CONTROL_GENERAL_WKUP + 0x008)
#define OMAP343X_CONTROL_WKUP_DEBOBS1 (OMAP343X_CONTROL_GENERAL_WKUP + 0x00C)
#define OMAP343X_CONTROL_WKUP_DEBOBS2 (OMAP343X_CONTROL_GENERAL_WKUP + 0x010)
#define OMAP343X_CONTROL_WKUP_DEBOBS3 (OMAP343X_CONTROL_GENERAL_WKUP + 0x014)
#define OMAP343X_CONTROL_WKUP_DEBOBS4 (OMAP343X_CONTROL_GENERAL_WKUP + 0x018)

/* 36xx-only RTA - Retention till Access control registers and bits */
#define OMAP36XX_CONTROL_MEM_RTA_CTRL	0x40C
#define OMAP36XX_RTA_DISABLE		0x0

/* 34xx D2D idle-related pins, handled by PM core */
#define OMAP3_PADCONF_SAD2D_MSTANDBY   0x250
#define OMAP3_PADCONF_SAD2D_IDLEACK    0x254

/* TI81XX CONTROL_DEVBOOT register offsets */
#define TI81XX_CONTROL_STATUS		(TI81XX_CONTROL_DEVBOOT + 0x000)

/* TI81XX CONTROL_DEVCONF register offsets */
#define TI81XX_CONTROL_DEVICE_ID	(TI81XX_CONTROL_DEVCONF + 0x000)

/* OMAP4 CONTROL MODULE */
#define OMAP4_CTRL_MODULE_PAD_WKUP			0x4a31e000
#define OMAP4_CTRL_MODULE_PAD_WKUP_CONTROL_I2C_2	0x0604
#define OMAP4_CTRL_MODULE_CORE_STATUS			0x02c4
#define OMAP4_CTRL_MODULE_CORE_STD_FUSE_PROD_ID_1	0x0218
#define OMAP4_CTRL_MODULE_CORE_DSP_BOOTADDR		0x0304
#define OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_DSIPHY	0x0618
#define OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_CAMERA_RX	0x0608

/* OMAP4 CONTROL_DSIPHY */
#define OMAP4_DSI2_LANEENABLE_SHIFT			29
#define OMAP4_DSI2_LANEENABLE_MASK			(0x7 << 29)
#define OMAP4_DSI1_LANEENABLE_SHIFT			24
#define OMAP4_DSI1_LANEENABLE_MASK			(0x1f << 24)
#define OMAP4_DSI1_PIPD_SHIFT				19
#define OMAP4_DSI1_PIPD_MASK				(0x1f << 19)
#define OMAP4_DSI2_PIPD_SHIFT				14
#define OMAP4_DSI2_PIPD_MASK				(0x1f << 14)

/* OMAP4 CONTROL_CAMERA_RX */
#define OMAP4_CAMERARX_CSI21_LANEENABLE_SHIFT		24
#define OMAP4_CAMERARX_CSI21_LANEENABLE_MASK		(0x1f << 24)
#define OMAP4_CAMERARX_CSI22_LANEENABLE_SHIFT		29
#define OMAP4_CAMERARX_CSI22_LANEENABLE_MASK		(0x3 << 29)
#define OMAP4_CAMERARX_CSI22_CTRLCLKEN_SHIFT		21
#define OMAP4_CAMERARX_CSI22_CTRLCLKEN_MASK		(1 << 21)
#define OMAP4_CAMERARX_CSI22_CAMMODE_SHIFT		19
#define OMAP4_CAMERARX_CSI22_CAMMODE_MASK		(0x3 << 19)
#define OMAP4_CAMERARX_CSI21_CTRLCLKEN_SHIFT		18
#define OMAP4_CAMERARX_CSI21_CTRLCLKEN_MASK		(1 << 18)
#define OMAP4_CAMERARX_CSI21_CAMMODE_SHIFT		16
#define OMAP4_CAMERARX_CSI21_CAMMODE_MASK		(0x3 << 16)

/* OMAP54XX CONTROL STATUS register */
#define OMAP5XXX_CONTROL_STATUS                0x134
#define OMAP5_DEVICETYPE_MASK          (0x7 << 6)

/* DRA7XX CONTROL CORE BOOTSTRAP */
#define DRA7_CTRL_CORE_BOOTSTRAP	0x6c4
#define DRA7_SPEEDSELECT_MASK		(0x3 << 8)

/*
 * REVISIT: This list of registers is not comprehensive - there are more
 * that should be added.
 */

/*
 * Control module register bit defines - these should eventually go into
 * their own regbits file.  Some of these will be complicated, depending
 * on the device type (general-purpose, emulator, test, secure, bad, other)
 * and the security mode (secure, non-secure, don't care)
 */
/* CONTROL_DEVCONF0 bits */
#define OMAP2_MMCSDIO1ADPCLKISEL	(1 << 24) /* MMC1 loop back clock */
#define OMAP24XX_USBSTANDBYCTRL		(1 << 15)
#define OMAP2_MCBSP2_CLKS_MASK		(1 << 6)
#define OMAP2_MCBSP1_FSR_MASK		(1 << 4)
#define OMAP2_MCBSP1_CLKR_MASK		(1 << 3)
#define OMAP2_MCBSP1_CLKS_MASK		(1 << 2)

/* CONTROL_DEVCONF1 bits */
#define OMAP243X_MMC1_ACTIVE_OVERWRITE	(1 << 31)
#define OMAP2_MMCSDIO2ADPCLKISEL	(1 << 6) /* MMC2 loop back clock */
#define OMAP2_MCBSP5_CLKS_MASK		(1 << 4) /* > 242x */
#define OMAP2_MCBSP4_CLKS_MASK		(1 << 2) /* > 242x */
#define OMAP2_MCBSP3_CLKS_MASK		(1 << 0) /* > 242x */

/* CONTROL_STATUS bits */
#define OMAP2_DEVICETYPE_MASK		(0x7 << 8)
#define OMAP2_SYSBOOT_5_MASK		(1 << 5)
#define OMAP2_SYSBOOT_4_MASK		(1 << 4)
#define OMAP2_SYSBOOT_3_MASK		(1 << 3)
#define OMAP2_SYSBOOT_2_MASK		(1 << 2)
#define OMAP2_SYSBOOT_1_MASK		(1 << 1)
#define OMAP2_SYSBOOT_0_MASK		(1 << 0)

/* CONTROL_PBIAS_LITE bits */
#define OMAP343X_PBIASLITESUPPLY_HIGH1	(1 << 15)
#define OMAP343X_PBIASLITEVMODEERROR1	(1 << 11)
#define OMAP343X_PBIASSPEEDCTRL1	(1 << 10)
#define OMAP343X_PBIASLITEPWRDNZ1	(1 << 9)
#define OMAP343X_PBIASLITEVMODE1	(1 << 8)
#define OMAP343X_PBIASLITESUPPLY_HIGH0	(1 << 7)
#define OMAP343X_PBIASLITEVMODEERROR0	(1 << 3)
#define OMAP2_PBIASSPEEDCTRL0		(1 << 2)
#define OMAP2_PBIASLITEPWRDNZ0		(1 << 1)
#define OMAP2_PBIASLITEVMODE0		(1 << 0)

/* CONTROL_PROG_IO1 bits */
#define OMAP3630_PRG_SDMMC1_SPEEDCTRL	(1 << 20)

/* CONTROL_IVA2_BOOTMOD bits */
#define OMAP3_IVA2_BOOTMOD_SHIFT	0
#define OMAP3_IVA2_BOOTMOD_MASK		(0xf << 0)
#define OMAP3_IVA2_BOOTMOD_IDLE		(0x1 << 0)

/* CONTROL_PADCONF_X bits */
#define OMAP3_PADCONF_WAKEUPEVENT0	(1 << 15)
#define OMAP3_PADCONF_WAKEUPENABLE0	(1 << 14)

#define OMAP343X_SCRATCHPAD_ROM		(OMAP343X_CTRL_BASE + 0x860)
#define OMAP343X_SCRATCHPAD		(OMAP343X_CTRL_BASE + 0x910)
#define OMAP343X_SCRATCHPAD_ROM_OFFSET	0x19C
#define OMAP343X_SCRATCHPAD_REGADDR(reg)	OMAP2_L4_IO_ADDRESS(\
						OMAP343X_SCRATCHPAD + reg)

/* AM35XX_CONTROL_IPSS_CLK_CTRL bits */
#define AM35XX_USBOTG_VBUSP_CLK_SHIFT	0
#define AM35XX_CPGMAC_VBUSP_CLK_SHIFT	1
#define AM35XX_VPFE_VBUSP_CLK_SHIFT	2
#define AM35XX_HECC_VBUSP_CLK_SHIFT	3
#define AM35XX_USBOTG_FCLK_SHIFT	8
#define AM35XX_CPGMAC_FCLK_SHIFT	9
#define AM35XX_VPFE_FCLK_SHIFT		10

/* AM35XX CONTROL_LVL_INTR_CLEAR bits */
#define AM35XX_CPGMAC_C0_MISC_PULSE_CLR	BIT(0)
#define AM35XX_CPGMAC_C0_RX_PULSE_CLR	BIT(1)
#define AM35XX_CPGMAC_C0_RX_THRESH_CLR	BIT(2)
#define AM35XX_CPGMAC_C0_TX_PULSE_CLR	BIT(3)
#define AM35XX_USBOTGSS_INT_CLR		BIT(4)
#define AM35XX_VPFE_CCDC_VD0_INT_CLR	BIT(5)
#define AM35XX_VPFE_CCDC_VD1_INT_CLR	BIT(6)
#define AM35XX_VPFE_CCDC_VD2_INT_CLR	BIT(7)

/* AM35XX CONTROL_IP_SW_RESET bits */
#define AM35XX_USBOTGSS_SW_RST		BIT(0)
#define AM35XX_CPGMACSS_SW_RST		BIT(1)
#define AM35XX_VPFE_VBUSP_SW_RST	BIT(2)
#define AM35XX_HECC_SW_RST		BIT(3)
#define AM35XX_VPFE_PCLK_SW_RST		BIT(4)

/* AM33XX CONTROL_STATUS register */
#define AM33XX_CONTROL_STATUS		0x040
#define AM33XX_CONTROL_SEC_CLK_CTRL	0x1bc

/* AM33XX CONTROL_STATUS bitfields (partial) */
#define AM33XX_CONTROL_STATUS_SYSBOOT1_SHIFT		22
#define AM33XX_CONTROL_STATUS_SYSBOOT1_WIDTH		0x2
#define AM33XX_CONTROL_STATUS_SYSBOOT1_MASK		(0x3 << 22)

/* AM33XX PWMSS Control register */
#define AM33XX_PWMSS_TBCLK_CLKCTRL			0x664

/* AM33XX  PWMSS Control bitfields */
#define AM33XX_PWMSS0_TBCLKEN_SHIFT			0
#define AM33XX_PWMSS1_TBCLKEN_SHIFT			1
#define AM33XX_PWMSS2_TBCLKEN_SHIFT			2

/* DEV Feature register to identify AM33XX features */
#define AM33XX_DEV_FEATURE		0x604
#define AM33XX_SGX_MASK			BIT(29)

/* Additional AM33XX/AM43XX CONTROL registers */
#define AM33XX_CONTROL_SYSCONFIG_OFFSET			0x0010
#define AM33XX_CONTROL_STATUS_OFFSET			0x0040
#define AM43XX_CONTROL_MPU_L2_CTRL_OFFSET		0x01e0
#define AM33XX_CONTROL_CORTEX_VBBLDO_CTRL_OFFSET	0x041c
#define AM33XX_CONTROL_CORE_SLDO_CTRL_OFFSET		0x0428
#define AM33XX_CONTROL_MPU_SLDO_CTRL_OFFSET		0x042c
#define AM33XX_CONTROL_CLK32KDIVRATIO_CTRL_OFFSET	0x0444
#define AM33XX_CONTROL_BANDGAP_CTRL_OFFSET		0x0448
#define AM33XX_CONTROL_BANDGAP_TRIM_OFFSET		0x044c
#define AM33XX_CONTROL_PLL_CLKINPULOW_CTRL_OFFSET	0x0458
#define AM33XX_CONTROL_MOSC_CTRL_OFFSET			0x0468
#define AM33XX_CONTROL_RCOSC_CTRL_OFFSET		0x046c
#define AM33XX_CONTROL_DEEPSLEEP_CTRL_OFFSET		0x0470
#define AM43XX_CONTROL_DISPLAY_PLL_SEL_OFFSET		0x0534
#define AM33XX_CONTROL_INIT_PRIORITY_0_OFFSET		0x0608
#define AM33XX_CONTROL_INIT_PRIORITY_1_OFFSET		0x060c
#define AM33XX_CONTROL_MMU_CFG_OFFSET			0x0610
#define AM33XX_CONTROL_TPTC_CFG_OFFSET			0x0614
#define AM33XX_CONTROL_USB_CTRL0_OFFSET			0x0620
#define AM33XX_CONTROL_USB_CTRL1_OFFSET			0x0628
#define AM33XX_CONTROL_USB_WKUP_CTRL_OFFSET		0x0648
#define AM43XX_CONTROL_USB_CTRL2_OFFSET			0x064c
#define AM43XX_CONTROL_GMII_SEL_OFFSET			0x0650
#define AM43XX_CONTROL_MPUSS_CTRL_OFFSET		0x0654
#define AM43XX_CONTROL_TIMER_CASCADE_CTRL_OFFSET	0x0658
#define AM43XX_CONTROL_PWMSS_CTRL_OFFSET		0x0664
#define AM33XX_CONTROL_MREQPRIO_0_OFFSET		0x0670
#define AM33XX_CONTROL_MREQPRIO_1_OFFSET		0x0674
#define AM33XX_CONTROL_HW_EVENT_SEL_GRP1_OFFSET		0x0690
#define AM33XX_CONTROL_HW_EVENT_SEL_GRP2_OFFSET		0x0694
#define AM33XX_CONTROL_HW_EVENT_SEL_GRP3_OFFSET		0x0698
#define AM33XX_CONTROL_HW_EVENT_SEL_GRP4_OFFSET		0x069c
#define AM33XX_CONTROL_SMRT_CTRL_OFFSET			0x06a0
#define AM33XX_CONTROL_MPUSS_HW_DEBUG_SEL_OFFSET	0x06a4
#define AM43XX_CONTROL_CQDETECT_STS_OFFSET		0x0e00
#define AM43XX_CONTROL_CQDETECT_STS2_OFFSET		0x0e08
#define AM43XX_CONTROL_VTP_CTRL_OFFSET			0x0e0c
#define AM33XX_CONTROL_VREF_CTRL_OFFSET			0x0e14
#define AM33XX_CONTROL_TPCC_EVT_MUX_0_3_OFFSET		0x0f90
#define AM33XX_CONTROL_TPCC_EVT_MUX_4_7_OFFSET		0x0f94
#define AM33XX_CONTROL_TPCC_EVT_MUX_8_11_OFFSET		0x0f98
#define AM33XX_CONTROL_TPCC_EVT_MUX_12_15_OFFSET	0x0f9c
#define AM33XX_CONTROL_TPCC_EVT_MUX_16_19_OFFSET	0x0fa0
#define AM33XX_CONTROL_TPCC_EVT_MUX_20_23_OFFSET	0x0fa4
#define AM33XX_CONTROL_TPCC_EVT_MUX_24_27_OFFSET	0x0fa8
#define AM33XX_CONTROL_TPCC_EVT_MUX_28_31_OFFSET	0x0fac
#define AM33XX_CONTROL_TPCC_EVT_MUX_32_35_OFFSET	0x0fb0
#define AM33XX_CONTROL_TPCC_EVT_MUX_36_39_OFFSET	0x0fb4
#define AM33XX_CONTROL_TPCC_EVT_MUX_40_43_OFFSET	0x0fb8
#define AM33XX_CONTROL_TPCC_EVT_MUX_44_47_OFFSET	0x0fbc
#define AM33XX_CONTROL_TPCC_EVT_MUX_48_51_OFFSET	0x0fc0
#define AM33XX_CONTROL_TPCC_EVT_MUX_52_55_OFFSET	0x0fc4
#define AM33XX_CONTROL_TPCC_EVT_MUX_56_59_OFFSET	0x0fc8
#define AM33XX_CONTROL_TPCC_EVT_MUX_60_63_OFFSET	0x0fcc
#define AM33XX_CONTROL_TIMER_EVT_CAPT_OFFSET		0x0fd0
#define AM33XX_CONTROL_ECAP_EVT_CAPT_OFFSET		0x0fd4
#define AM33XX_CONTROL_ADC_EVT_CAPT_OFFSET		0x0fd8
#define AM43XX_CONTROL_ADC1_EVT_CAPT_OFFSET		0x0fdc
#define AM33XX_CONTROL_RESET_ISO_OFFSET			0x1000

/* CONTROL OMAP STATUS register to identify OMAP3 features */
#define OMAP3_CONTROL_OMAP_STATUS	0x044c

#define OMAP3_SGX_SHIFT			13
#define OMAP3_SGX_MASK			(3 << OMAP3_SGX_SHIFT)
#define		FEAT_SGX_FULL		0
#define		FEAT_SGX_HALF		1
#define		FEAT_SGX_NONE		2

#define OMAP3_IVA_SHIFT			12
#define OMAP3_IVA_MASK			(1 << OMAP3_IVA_SHIFT)
#define		FEAT_IVA		0
#define		FEAT_IVA_NONE		1

#define OMAP3_L2CACHE_SHIFT		10
#define OMAP3_L2CACHE_MASK		(3 << OMAP3_L2CACHE_SHIFT)
#define		FEAT_L2CACHE_NONE	0
#define		FEAT_L2CACHE_64KB	1
#define		FEAT_L2CACHE_128KB	2
#define		FEAT_L2CACHE_256KB	3

#define OMAP3_ISP_SHIFT			5
#define OMAP3_ISP_MASK			(1 << OMAP3_ISP_SHIFT)
#define		FEAT_ISP		0
#define		FEAT_ISP_NONE		1

#define OMAP3_NEON_SHIFT		4
#define OMAP3_NEON_MASK			(1 << OMAP3_NEON_SHIFT)
#define		FEAT_NEON		0
#define		FEAT_NEON_NONE		1


#ifndef __ASSEMBLY__
#ifdef CONFIG_ARCH_OMAP2PLUS
extern u8 omap_ctrl_readb(u16 offset);
extern u16 omap_ctrl_readw(u16 offset);
extern u32 omap_ctrl_readl(u16 offset);
extern void omap_ctrl_writeb(u8 val, u16 offset);
extern void omap_ctrl_writew(u16 val, u16 offset);
extern void omap_ctrl_writel(u32 val, u16 offset);

extern void omap3_save_scratchpad_contents(void);
extern void omap3_clear_scratchpad_contents(void);
extern void omap3_restore(void);
extern void omap3_restore_es3(void);
extern void omap3_restore_3630(void);
extern u32 omap3_arm_context[128];
extern void omap3_control_save_context(void);
extern void omap3_control_restore_context(void);
extern void omap3_ctrl_write_boot_mode(u8 bootmode);
extern void omap_ctrl_write_dsp_boot_addr(u32 bootaddr);
extern void omap_ctrl_write_dsp_boot_mode(u8 bootmode);
extern void omap3630_ctrl_disable_rta(void);
extern int omap3_ctrl_save_padconf(void);
void omap3_ctrl_init(void);
int omap2_control_base_init(void);
int omap_control_init(void);
void omap2_set_globals_control(void __iomem *ctrl);
void __init omap3_control_legacy_iomap_init(void);
#else
#define omap_ctrl_readb(x)		0
#define omap_ctrl_readw(x)		0
#define omap_ctrl_readl(x)		0
#define omap4_ctrl_pad_readl(x)		0
#define omap_ctrl_writeb(x, y)		WARN_ON(1)
#define omap_ctrl_writew(x, y)		WARN_ON(1)
#define omap_ctrl_writel(x, y)		WARN_ON(1)
#define omap4_ctrl_pad_writel(x, y)	WARN_ON(1)
#endif
#endif	/* __ASSEMBLY__ */

#endif /* __ARCH_ARM_MACH_OMAP2_CONTROL_H */

