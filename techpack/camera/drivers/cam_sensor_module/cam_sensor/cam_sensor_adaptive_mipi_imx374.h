/* Copyright (c) 2017, The Linux Foundation. All rights reserved.
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

#ifndef _CAM_SENSOR_ADAPTIVE_MIPI_IMX374_H_
#define _CAM_SENSOR_ADAPTIVE_MIPI_IMX374_H_

#include "cam_sensor_dev.h"

enum {
	CAM_IMX374_SET_A_FULL_909_MHZ = 0,
	CAM_IMX374_SET_A_FULL_906_MHZ = 1,
	CAM_IMX374_SET_A_FULL_903_MHZ = 2,
	CAM_IMX374_SET_A_FULL_897_MHZ = 3,
};

struct cam_sensor_i2c_reg_array MIPI_FULL_909_MHZ_REG_ARRAY[] = {
    {0x030F, 0x2F, 0x00, 0x00},
};

struct cam_sensor_i2c_reg_array MIPI_FULL_906_MHZ_REG_ARRAY[] = {
    {0x030F, 0x2E, 0x00, 0x00},
};

struct cam_sensor_i2c_reg_array MIPI_FULL_903_MHZ_REG_ARRAY[] = {
    {0x030F, 0x2D, 0x00, 0x00},
};

struct cam_sensor_i2c_reg_array MIPI_FULL_897_MHZ_REG_ARRAY[] = {
    {0x030F, 0x2B, 0x00, 0x00},
};

static const struct cam_sensor_i2c_reg_setting sensor_imx374_setfile_A_mipi_FULL_909_mhz[] = {
    { MIPI_FULL_909_MHZ_REG_ARRAY, ARRAY_SIZE(MIPI_FULL_909_MHZ_REG_ARRAY),
	  CAMERA_SENSOR_I2C_TYPE_WORD, CAMERA_SENSOR_I2C_TYPE_BYTE, 0 }
};

static const struct cam_sensor_i2c_reg_setting sensor_imx374_setfile_A_mipi_FULL_906_mhz[] = {
    { MIPI_FULL_906_MHZ_REG_ARRAY, ARRAY_SIZE(MIPI_FULL_906_MHZ_REG_ARRAY),
	  CAMERA_SENSOR_I2C_TYPE_WORD, CAMERA_SENSOR_I2C_TYPE_BYTE, 0 }
};

static const struct cam_sensor_i2c_reg_setting sensor_imx374_setfile_A_mipi_FULL_903_mhz[] = {
    { MIPI_FULL_903_MHZ_REG_ARRAY, ARRAY_SIZE(MIPI_FULL_903_MHZ_REG_ARRAY),
	  CAMERA_SENSOR_I2C_TYPE_WORD, CAMERA_SENSOR_I2C_TYPE_BYTE, 0 }
};

static const struct cam_sensor_i2c_reg_setting sensor_imx374_setfile_A_mipi_FULL_897_mhz[] = {
    { MIPI_FULL_897_MHZ_REG_ARRAY, ARRAY_SIZE(MIPI_FULL_897_MHZ_REG_ARRAY),
	  CAMERA_SENSOR_I2C_TYPE_WORD, CAMERA_SENSOR_I2C_TYPE_BYTE, 0 }
};

static const struct cam_mipi_setting sensor_imx374_setfile_A_mipi_setting_FULL[] = {
	{ "909 Mhz",
	  sensor_imx374_setfile_A_mipi_FULL_909_mhz, ARRAY_SIZE(sensor_imx374_setfile_A_mipi_FULL_909_mhz) },
	{ "906 Mhz",
	  sensor_imx374_setfile_A_mipi_FULL_906_mhz, ARRAY_SIZE(sensor_imx374_setfile_A_mipi_FULL_906_mhz) },
	{ "903 Mhz",
	  sensor_imx374_setfile_A_mipi_FULL_903_mhz, ARRAY_SIZE(sensor_imx374_setfile_A_mipi_FULL_903_mhz) },
	{ "897 Mhz",
	  sensor_imx374_setfile_A_mipi_FULL_897_mhz, ARRAY_SIZE(sensor_imx374_setfile_A_mipi_FULL_897_mhz) },
};

static const struct cam_mipi_channel sensor_imx374_setfile_A_mipi_channel_FULL[] = {
	{ CAM_RAT_BAND(CAM_RAT_1_GSM, CAM_BAND_001_GSM_GSM850), 0, 0, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_1_GSM, CAM_BAND_002_GSM_EGSM900), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_1_GSM, CAM_BAND_003_GSM_DCS1800), 0, 0, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_1_GSM, CAM_BAND_004_GSM_PCS1900), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_011_WCDMA_WB01), 10562, 10642, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_011_WCDMA_WB01), 10643, 10647, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_011_WCDMA_WB01), 10648, 10682, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_011_WCDMA_WB01), 10683, 10718, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_011_WCDMA_WB01), 10719, 10838, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_012_WCDMA_WB02), 9662, 9733, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_012_WCDMA_WB02), 9734, 9744, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_012_WCDMA_WB02), 9745, 9776, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_012_WCDMA_WB02), 9777, 9809, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_012_WCDMA_WB02), 9810, 9938, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1162, 1176, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1177, 1192, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1193, 1222, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1223, 1252, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1253, 1404, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1405, 1418, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1419, 1448, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1449, 1479, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_013_WCDMA_WB03), 1480, 1513, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_014_WCDMA_WB04), 1537, 1617, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_014_WCDMA_WB04), 1618, 1622, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_014_WCDMA_WB04), 1623, 1657, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_014_WCDMA_WB04), 1658, 1693, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_014_WCDMA_WB04), 1694, 1738, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_015_WCDMA_WB05), 4357, 4458, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2237, 2267, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2268, 2299, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2300, 2342, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2343, 2494, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2495, 2525, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_017_WCDMA_WB07), 2526, 2563, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_018_WCDMA_WB08), 2937, 3040, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_018_WCDMA_WB08), 3041, 3078, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_2_WCDMA, CAM_BAND_018_WCDMA_WB08), 3079, 3088, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_091_LTE_LB01), 0, 186, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_091_LTE_LB01), 187, 195, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_091_LTE_LB01), 196, 265, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_091_LTE_LB01), 266, 336, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_091_LTE_LB01), 337, 599, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_092_LTE_LB02), 600, 768, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_092_LTE_LB02), 769, 789, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_092_LTE_LB02), 790, 853, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_092_LTE_LB02), 854, 918, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_092_LTE_LB02), 919, 1199, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1200, 1254, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1255, 1284, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1285, 1344, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1345, 1404, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1405, 1709, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1710, 1736, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1737, 1797, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1798, 1859, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_093_LTE_LB03), 1860, 1949, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_094_LTE_LB04), 1950, 2136, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_094_LTE_LB04), 2137, 2145, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_094_LTE_LB04), 2146, 2215, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_094_LTE_LB04), 2216, 2286, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_094_LTE_LB04), 2287, 2399, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_095_LTE_LB05), 2400, 2420, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_095_LTE_LB05), 2421, 2649, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 2750, 2835, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 2836, 2898, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 2899, 2985, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 2986, 3290, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 3291, 3351, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 3352, 3440, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_097_LTE_LB07), 3441, 3449, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_098_LTE_LB08), 3450, 3681, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_098_LTE_LB08), 3682, 3756, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_098_LTE_LB08), 3757, 3787, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_098_LTE_LB08), 3788, 3799, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_102_LTE_LB12), 5010, 5018, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_102_LTE_LB12), 5019, 5042, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_102_LTE_LB12), 5043, 5066, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_102_LTE_LB12), 5067, 5179, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_103_LTE_LB13), 5180, 5279, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_104_LTE_LB14), 5280, 5379, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_107_LTE_LB17), 5730, 5736, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_107_LTE_LB17), 5737, 5849, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_108_LTE_LB18), 5850, 5903, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_108_LTE_LB18), 5904, 5931, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_108_LTE_LB18), 5932, 5960, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_108_LTE_LB18), 5961, 5999, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_109_LTE_LB19), 6000, 6149, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_110_LTE_LB20), 6150, 6366, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_110_LTE_LB20), 6367, 6441, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_110_LTE_LB20), 6442, 6449, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_111_LTE_LB21), 6450, 6465, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_111_LTE_LB21), 6466, 6514, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_111_LTE_LB21), 6515, 6564, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_111_LTE_LB21), 6565, 6599, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8040, 8208, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8209, 8229, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8230, 8293, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8294, 8358, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8359, 8662, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8663, 8680, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_115_LTE_LB25), 8681, 8689, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_116_LTE_LB26), 8690, 8753, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_116_LTE_LB26), 8754, 8781, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_116_LTE_LB26), 8782, 8810, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_116_LTE_LB26), 8811, 9039, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_118_LTE_LB28), 9210, 9305, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_118_LTE_LB28), 9306, 9380, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_118_LTE_LB28), 9381, 9405, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_118_LTE_LB28), 9406, 9431, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_118_LTE_LB28), 9432, 9659, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_119_LTE_LB29), 9660, 9713, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_119_LTE_LB29), 9714, 9769, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_120_LTE_LB30), 9770, 9828, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_120_LTE_LB30), 9829, 9869, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_122_LTE_LB32), 9920, 9922, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_122_LTE_LB32), 9923, 9970, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_122_LTE_LB32), 9971, 10018, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_122_LTE_LB32), 10019, 10323, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_122_LTE_LB32), 10324, 10359, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_124_LTE_LB34), 36200, 36349, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_128_LTE_LB38), 37750, 37881, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_128_LTE_LB38), 37882, 37945, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_128_LTE_LB38), 37946, 38031, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_128_LTE_LB38), 38032, 38249, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_129_LTE_LB39), 38250, 38463, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_129_LTE_LB39), 38464, 38487, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_129_LTE_LB39), 38488, 38550, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_129_LTE_LB39), 38551, 38613, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_129_LTE_LB39), 38614, 38649, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 38650, 38754, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 38755, 38827, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 38828, 38904, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 38905, 39208, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 39209, 39280, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 39281, 39358, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_130_LTE_LB40), 39359, 39649, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 39650, 39679, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 39680, 39762, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 39763, 40066, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40067, 40132, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40133, 40216, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40217, 40521, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40522, 40585, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40586, 40671, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40672, 40975, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 40976, 41038, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 41039, 41125, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 41126, 41430, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 41431, 41491, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 41492, 41580, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_131_LTE_LB41), 41581, 41589, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 41590, 41602, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 41603, 41639, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 41640, 41752, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 41753, 42057, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42058, 42093, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42094, 42207, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42208, 42511, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42512, 42546, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42547, 42661, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42662, 42966, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 42967, 42999, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 43000, 43116, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 43117, 43420, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 43421, 43452, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 43453, 43570, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_132_LTE_LB42), 43571, 43589, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 55240, 55266, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 55267, 55570, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 55571, 55602, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 55603, 55720, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 55721, 56025, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56026, 56055, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56056, 56175, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56176, 56479, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56480, 56508, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56509, 56629, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_138_LTE_LB48), 56630, 56739, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 66436, 66622, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 66623, 66631, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 66632, 66701, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 66702, 66772, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 66773, 67076, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 67077, 67082, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 67083, 67154, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 67155, 67226, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_156_LTE_LB66), 67227, 67335, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_161_LTE_LB71), 68586, 68736, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_161_LTE_LB71), 68737, 68811, CAM_IMX374_SET_A_FULL_897_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_161_LTE_LB71), 68812, 68832, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_161_LTE_LB71), 68833, 68853, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_3_LTE, CAM_BAND_161_LTE_LB71), 68854, 68935, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_051_TDSCDMA_A), 0, 0, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_052_TDSCDMA_B), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_053_TDSCDMA_C), 0, 0, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_054_TDSCDMA_D), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_055_TDSCDMA_E), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_4_TDSCDMA, CAM_BAND_056_TDSCDMA_F), 0, 0, CAM_IMX374_SET_A_FULL_909_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_5_CDMA, CAM_BAND_061_CDMA_BC0), 0, 0, CAM_IMX374_SET_A_FULL_903_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_5_CDMA, CAM_BAND_062_CDMA_BC1), 0, 0, CAM_IMX374_SET_A_FULL_906_MHZ },
	{ CAM_RAT_BAND(CAM_RAT_5_CDMA, CAM_BAND_071_CDMA_BC10), 0, 0, CAM_IMX374_SET_A_FULL_903_MHZ },
};

static const struct cam_mipi_sensor_mode sensor_imx374_setfile_A_mipi_sensor_mode[] = {
	{
		sensor_imx374_setfile_A_mipi_channel_FULL,	ARRAY_SIZE(sensor_imx374_setfile_A_mipi_channel_FULL),
		sensor_imx374_setfile_A_mipi_setting_FULL,	ARRAY_SIZE(sensor_imx374_setfile_A_mipi_setting_FULL)
	},
};
#endif /* _CAM_SENSOR_ADAPTIVE_MIPI_IMX374_H_ */
