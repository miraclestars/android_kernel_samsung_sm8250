/*
 * A new exposure driver based on SDE dim layer for OLED devices
 *
 * Copyright (C) 2012-2014, The Linux Foundation. All rights reserved.
 * Copyright (C) 2019, Devries <therkduan@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef SDE_EXPO_DIM_LAYER_H
#define SDE_EXPO_DIM_LAYER_H

#define DIM_THRES_LEVEL 9900
#define LEVEL_BR_SCALE  100

enum {
	BRIGHTNESS = 0,
	ALPHA = 1,
	LUT_MAX,
};

static const uint8_t brightness_alpha_lut[][LUT_MAX] = {
/* {brightness, alpha} */
	{0, 0xD5},
	{1, 0xD2},
	{2, 0xD0},
	{3, 0xCE},
	{4, 0xCC},
	{5, 0xCA},
	{6, 0xC8},
	{7, 0xC6},
	{8, 0xC4},
	{10, 0xBA},
	{12, 0xB0},
	{15, 0xA0},
	{20, 0x8B},
	{32, 0x72},
	{45, 0x5A},
	{68, 0x38},
	{90, 0x0E},
	{99, 0x00}
};

int expo_map_dim_level(int level, struct dsi_display *display);

#endif /* SDE_EXPO_DIM_LAYER_H */
