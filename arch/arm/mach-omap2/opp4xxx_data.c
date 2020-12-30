/*
 * OMAP4 OPP table definitions.
 *
 * Copyright (C) 2010-2012 Texas Instruments Incorporated - http://www.ti.com/
 *	Nishanth Menon
 *	Kevin Hilman
 *	Thara Gopinath
 * Copyright (C) 2010-2011 Nokia Corporation.
 *      Eduardo Valentin
 *      Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>

#include "soc.h"
#include "control.h"
#include "omap_opp_data.h"
#include "pm.h"

/*
 * Structures containing OMAP4430 voltage supported and various
 * voltage dependent data for each VDD.
 */

#define OMAP4430_VDD_MPU_OPP50_UV		1025000
#define OMAP4430_VDD_MPU_OPP100_UV		1200000
#define OMAP4430_VDD_MPU_OPPTURBO_UV		1313000
#define OMAP4430_VDD_MPU_OPPNITRO_UV		1375000

struct omap_volt_data omap443x_vdd_mpu_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPP50_UV, OMAP44XX_CONTROL_FUSE_MPU_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPP100_UV, OMAP44XX_CONTROL_FUSE_MPU_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPTURBO_UV, OMAP44XX_CONTROL_FUSE_MPU_OPPTURBO, 0xfa, 0x23),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITRO_UV, OMAP44XX_CONTROL_FUSE_MPU_OPPNITRO, 0xfa, 0x27),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};

#define OMAP4430_VDD_IVA_OPP50_UV		1013000
#define OMAP4430_VDD_IVA_OPP100_UV		1188000
#define OMAP4430_VDD_IVA_OPPTURBO_UV		1300000

struct omap_volt_data omap443x_vdd_iva_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPP50_UV, OMAP44XX_CONTROL_FUSE_IVA_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPP100_UV, OMAP44XX_CONTROL_FUSE_IVA_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPPTURBO_UV, OMAP44XX_CONTROL_FUSE_IVA_OPPTURBO, 0xfa, 0x23),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};

#define OMAP4430_VDD_CORE_OPP50_UV		1025000
#define OMAP4430_VDD_CORE_OPP100_UV		1200000

struct omap_volt_data omap443x_vdd_core_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_CORE_OPP50_UV, OMAP44XX_CONTROL_FUSE_CORE_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4430_VDD_CORE_OPP100_UV, OMAP44XX_CONTROL_FUSE_CORE_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};

#define OMAP4460_VDD_MPU_OPP50_UV		1025000
#define OMAP4460_VDD_MPU_OPP100_UV		1200000
#define OMAP4460_VDD_MPU_OPPTURBO_UV		1313000
#define OMAP4460_VDD_MPU_OPPNITRO_UV		1375000

struct omap_volt_data omap446x_vdd_mpu_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4460_VDD_MPU_OPP50_UV, OMAP44XX_CONTROL_FUSE_MPU_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4460_VDD_MPU_OPP100_UV, OMAP44XX_CONTROL_FUSE_MPU_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(OMAP4460_VDD_MPU_OPPTURBO_UV, OMAP44XX_CONTROL_FUSE_MPU_OPPTURBO, 0xfa, 0x23),
	VOLT_DATA_DEFINE(OMAP4460_VDD_MPU_OPPNITRO_UV, OMAP44XX_CONTROL_FUSE_MPU_OPPNITRO, 0xfa, 0x27),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};

#define OMAP4460_VDD_IVA_OPP50_UV		1025000
#define OMAP4460_VDD_IVA_OPP100_UV		1200000
#define OMAP4460_VDD_IVA_OPPTURBO_UV		1313000
#define OMAP4460_VDD_IVA_OPPNITRO_UV		1375000

struct omap_volt_data omap446x_vdd_iva_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4460_VDD_IVA_OPP50_UV, OMAP44XX_CONTROL_FUSE_IVA_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4460_VDD_IVA_OPP100_UV, OMAP44XX_CONTROL_FUSE_IVA_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(OMAP4460_VDD_IVA_OPPTURBO_UV, OMAP44XX_CONTROL_FUSE_IVA_OPPTURBO, 0xfa, 0x23),
	VOLT_DATA_DEFINE(OMAP4460_VDD_IVA_OPPNITRO_UV, OMAP44XX_CONTROL_FUSE_IVA_OPPNITRO, 0xfa, 0x23),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};

#define OMAP4460_VDD_CORE_OPP50_UV		1025000
#define OMAP4460_VDD_CORE_OPP100_UV		1200000
#define OMAP4460_VDD_CORE_OPP100_OV_UV		1250000

struct omap_volt_data omap446x_vdd_core_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4460_VDD_CORE_OPP50_UV, OMAP44XX_CONTROL_FUSE_CORE_OPP50, 0xf4, 0x0c),
	VOLT_DATA_DEFINE(OMAP4460_VDD_CORE_OPP100_UV, OMAP44XX_CONTROL_FUSE_CORE_OPP100, 0xf9, 0x16),
	VOLT_DATA_DEFINE(OMAP4460_VDD_CORE_OPP100_OV_UV, OMAP44XX_CONTROL_FUSE_CORE_OPP100OV, 0xf9, 0x16),
	VOLT_DATA_DEFINE(0, 0, 0, 0),
};
