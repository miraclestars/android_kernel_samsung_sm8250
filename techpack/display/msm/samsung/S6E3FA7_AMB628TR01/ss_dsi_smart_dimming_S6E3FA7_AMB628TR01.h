/*
 * =================================================================
 *
 *       Filename:  ss_dsi_smart_dimming_S6E3FA7_AMB628TR01.h
 *
 *    Description:  Smart dimming algorithm implementation
 *
 *        Company:  Samsung Electronics
 *
 * ================================================================
 */
/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012, Samsung Electronics. All rights reserved.

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
 * You should have received a copy of the GNU General Public License
 *
*/
#ifndef _SS_DSI_SMART_DIMMING_S6E3FA7_AMB628TR01_H_
#define _SS_DSI_SMART_DIMMING_S6E3FA7_AMB628TR01_H_

#include "ss_dsi_panel_common.h"
#include "ss_dsi_smart_dimming_common.h"

static int id1, id2, id3;

#define LUMINANCE_MAX 74
#define HMT_LUMINANCE_MAX 37
#define GAMMA_SET_MAX 35
#define GRAY_SCALE_MAX 256
#define RGB_COMPENSATION 27

/* BIT_SHIFT is used for right bit shfit */
#define BIT_SHIFT 22
#define BIT_SHFIT_MUL 4194304 // pow(2,BIT_SHIFT)

#define VREG0_REF_6P5 27262976	/* 6.5*4194304 */

#define HBM_INTERPOLATION_STEP 8

enum {
	GAMMA_CURVE_1P9 = 0,
	GAMMA_CURVE_2P15,
	GAMMA_CURVE_2P2,
};

/* TP */
enum {
	VT = 0,
	V1 = 1,
	V7 = 2,
	V11 = 3,
	V23 = 4,
	V35 = 5,
	V51 = 6,
	V87 = 7,
	V151 = 8,
	V203 = 9,
	V255 = 10,
	V_MAX,
};

/* RGB */
enum {
	R = 0,
	G = 1,
	B = 2,
	RGB_MAX,
};

static char V_LIST[V_MAX][5] = {
	"VT",
	"V1",
	"V7",
	"V11",
	"V23",
	"V35",
	"V51",
	"V87",
	"V151",
	"V203",
	"V255",
};

struct illuminance_table {
	int lux;
	char gamma_setting[GAMMA_SET_MAX];
	char gamma_setting_log[GAMMA_SET_MAX];
} __packed;

struct SMART_DIM {
	/* read C8h 1st ~ 35th */
	char MTP_ORIGN[GAMMA_SET_MAX];

	/* copy MTP_ORIGN -> MTP */
	int MTP[V_MAX][RGB_MAX];

	/* TP's gamma voltage */
	int RGB_OUTPUT[V_MAX][RGB_MAX];

	/* GRAY (0~255) */
	int GRAY[GRAY_SCALE_MAX][RGB_MAX];

	/* Because of AID funtion, below members are added*/
	int lux_table_max;
	int *plux_table;
	struct illuminance_table gen_table[LUMINANCE_MAX];

	int brightness_level;
	int ldi_revision;
	int vregout_voltage;
	char panel_revision;

	/* HBM interpolation */
	struct illuminance_table hbm_interpolation_table[HBM_INTERPOLATION_STEP];
	char *hbm_payload;
	int hbm_brightness_level;

	struct illuminance_table hmt_gen_table[HMT_LUMINANCE_MAX];
} __packed;

/* VT,V1,V7,V11,V23,V35,V51,V87,V151,V203,V255 */
static const int INFLECTION_VOLTAGE_ARRAY[V_MAX] = {0, 1, 7, 11, 23, 35, 51, 87, 151, 203, 255};

static const int vt_coefficient[] = {
	0, 12, 24, 36,
	48, 60, 72, 84,
	96, 108, 138, 148,
	158, 168, 178, 186,
};

// center (max) gamma value (Hex)
static const int center_gamma[V_MAX][RGB_MAX] = {
	{0x0, 0x0, 0x0},		/* VT */
	{0x80, 0x80, 0x80},		/* V1 */
	{0x80, 0x80, 0x80},		/* V7 */
	{0x80, 0x80, 0x80},		/* V11 */
	{0x80, 0x80, 0x80},		/* V23 */
	{0x80, 0x80, 0x80},		/* V35 */
	{0x80, 0x80, 0x80},		/* V51 */
	{0x80, 0x80, 0x80},		/* V87 */
	{0x80, 0x80, 0x80},		/* V151 */
	{0x80, 0x80, 0x80},		/* V203 */
	{0x100, 0x100, 0x100},	/* V255 */
};

/* fraction for gamma code */
static const int fraction[V_MAX][2] = {
/* {numerator, denominator} */
	{0,  860},		/* VT */
	{0,  256},		/* V1 */
	{64, 320},		/* V7 */
	{64, 320},		/* V11 */
	{64, 320},		/* V23 */
	{64, 320},		/* V35 */
	{64, 320},		/* V51 */
	{64, 320},		/* V87 */
	{64, 320},		/* V151 */
	{64, 320},		/* V203 */
	{129, 860},		/* V255 */
};

/* 443 is made but not used */
static const int hbm_interpolation_candela_table[HBM_INTERPOLATION_STEP] = {443, 465, 488, 510, 533, 555, 578, 600};

static const unsigned int base_luminance[LUMINANCE_MAX][2] = {
	{2, 107},
	{3, 107},
	{4, 107},
	{5, 107},
	{6, 107},
	{7, 107},
	{8, 107},
	{9, 107},
	{10, 107},
	{11, 107},
	{12, 107},
	{13, 107},
	{14, 107},
	{15, 107},
	{16, 107},
	{17, 107},
	{19, 107},
	{20, 107},
	{21, 107},
	{22, 107},
	{24, 107},
	{25, 107},
	{27, 107},
	{29, 107},
	{30, 107},
	{32, 107},
	{34, 107},
	{37, 107},
	{39, 107},
	{41, 107},
	{44, 107},
	{47, 107},
	{50, 107},
	{53, 107},
	{56, 107},
	{60, 107},
	{64, 109},
	{68, 115},
	{72, 120},
	{77, 127},
	{82, 135},
	{87, 142},
	{93, 151},
	{98, 161},
	{105, 170},
	{111, 181},
	{119, 192},
	{126, 202},
	{134, 214},
	{143, 228},
	{152, 241},
	{162, 252},
	{172, 268},
	{183, 277},
	{195, 277},
	{207, 277},
	{220, 277},
	{234, 286},
	{249, 292},
	{265, 308},
	{282, 324},
	{300, 341},
	{316, 355},
	{333, 371},
	{350, 387},
	{357, 395},
	{365, 402},
	{372, 402},
	{380, 402},
	{387, 402},
	{395, 404},
	{403, 406},
	{412, 409},
	{420, 420},
};

static const int gradation_offset[LUMINANCE_MAX][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 2, 8, 17, 28, 29, 30, 31, 27},
	{0, 1, 5, 13, 20, 22, 22, 25, 22},
	{0, 1, 4, 10, 17, 20, 22, 24, 21},
	{0, 1, 3, 9, 13, 17, 18, 21, 18},
	{0, 1, 2, 8, 12, 16, 17, 20, 18},
	{0, 1, 2, 6, 10, 14, 16, 19, 18},
	{0, 0, 2, 5, 9, 13, 15, 18, 17},
	{0, 1, 2, 5, 9, 13, 15, 18, 17},
	{0, 1, 2, 5, 9, 12, 14, 17, 16},
	{0, 1, 2, 5, 9, 12, 14, 17, 16},
	{0, 1, 2, 5, 9, 12, 14, 17, 16},
	{0, 1, 2, 5, 9, 12, 14, 17, 16},
	{0, 1, 2, 5, 9, 12, 14, 17, 16},
	{0, 1, 2, 5, 9, 11, 13, 16, 15},
	{0, 1, 2, 4, 8, 10, 12, 15, 14},
	{0, 1, 2, 4, 8, 9, 12, 15, 14},
	{0, 0, 1, 3, 7, 8, 11, 14, 13},
	{0, 0, 1, 3, 7, 8, 11, 15, 14},
	{0, 0, 1, 3, 7, 8, 11, 15, 14},
	{0, 0, 1, 3, 6, 7, 11, 14, 13},
	{0, 0, 1, 2, 6, 6, 9, 12, 11},
	{0, 0, 1, 2, 5, 5, 8, 11, 10},
	{0, 0, 1, 2, 5, 5, 8, 11, 10},
	{0, 0, 1, 2, 5, 4, 7, 11, 10},
	{0, 0, 1, 2, 5, 4, 7, 11, 10},
	{0, 0, 1, 1, 4, 3, 6, 10, 9},
	{0, 0, 1, 1, 4, 3, 6, 10, 9},
	{0, 0, 0, 1, 3, 2, 5, 9, 8},
	{0, 0, 0, 1, 3, 2, 5, 9, 8},
	{0, 0, 0, 1, 3, 2, 5, 9, 8},
	{0, 0, 0, 0, 2, 1, 4, 8, 7},
	{0, 0, 0, 0, 2, 1, 3, 7, 6},
	{0, 0, 0, 0, 2, 1, 3, 7, 6},
	{0, 0, 0, 0, 1, 1, 3, 6, 5},
	{0, 0, 0, 0, 1, 1, 3, 6, 5},
	{0, 0, 0, 0, 1, 1, 3, 5, 4},
	{0, 0, 0, 0, 0, 1, 3, 5, 4},
	{0, 1, 0, 0, 1, 2, 3, 4, 3},
	{0, 1, 0, 0, 0, 0, 1, 3, 3},
	{0, 2, 0, 0, 0, 1, 2, 4, 3},
	{0, 2, 2, 1, 0, 1, 3, 4, 3},
	{0, 1, 2, 0, 0, 1, 2, 4, 3},
	{0, 1, 1, 0, 0, 1, 2, 5, 4},
	{0, 1, 1, 0, 0, 0, 1, 4, 3},
	{0, 1, 2, 0, 0, 1, 2, 4, 3},
	{0, 1, 2, 0, 1, 2, 3, 6, 5},
	{0, 0, 2, 0, 1, 2, 3, 5, 4},
	{0, 0, 2, 0, 1, 2, 3, 5, 4},
	{0, 0, 2, 0, 1, 2, 3, 4, 3},
	{0, -1, 2, 0, 1, 2, 3, 4, 3},
	{0, -1, 2, 0, 1, 2, 2, 4, 3},
	{0, 0, 2, 0, 1, 2, 2, 3, 2},
	{0, 0, 1, 0, 0, 1, 1, 2, 1},
	{0, 0, 2, 0, 0, 1, 1, 2, 2},
	{0, 0, 2, 0, 0, 1, 1, 2, 2},
	{0, 0, 1, 0, 0, 1, 1, 2, 2},
	{0, 0, 1, 0, 0, 1, 1, 2, 2},
	{0, 0, 0, 0, 0, 0, 1, 2, 2},
	{0, 0, 0, -1, -1, -1, -1, 1, 1},
	{0, 0, 0, -1, -2, -2, -1, 1, 1},
	{0, 1, 0, -1, -2, -2, -1, 0, 0},
	{0, 0, 1, -1, -2, -2, -1, -1, 0},
	{0, 1, 0, -1, -2, -2, -1, -1, 0},
	{0, 0, 0, -1, -2, -2, -1, -1, 0},
	{0, 0, 0, -1, -2, -2, -1, -1, 0},
	{0, 0, 0, -1, -2, -2, -2, -1, 0},
	{0, 0, 0, -1, -2, -2, -2, -2, 0},
	{0, -1, -1, -1, -2, -2, -2, -2, 0},
	{0, -1, -1, -2, -2, -2, -2, -2, 0},
	{0, -1, -2, -3, -3, -2, -2, -2, 0},
	{0, 0, -2, -2, -2, -2, -2, -2, 0},
	{0, -2, -2, -3, -2, -2, -2, -2, 0},
	{0, -2, -2, -3, -2, -2, -1, -1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static const int rgb_offset[LUMINANCE_MAX][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-4, 2, -1, -1, 3, 0, -7, 0, -5, -15, 3, -11, -27, -2, -22, -2, -1, -3, 0, 1, -1, 0, 2, 0, 2, 3, 1},
	{-1, 1, 0, -1, 3, 0, -6, 0, -4, -12, 1, -8, -24, 0, -20, -8, 1, -6, 1, 2, 0, 1, 2, 0, 1, 2, 1},
	{0, 1, 0, 0, 2, 0, -5, 0, -3, -12, 2, -8, -18, 1, -14, -22, -1, -20, 2, 3, 1, 0, 1, -1, 1, 2, 1},
	{0, 0, -1, -1, 2, 0, -3, 1, -2, -11, 0, -9, -13, 6, -9, -28, -4, -24, 4, 6, 3, 0, 1, -1, 5, 5, 5},
	{0, 0, 0, -1, 2, -1, -2, 1, 0, -10, 0, -8, -15, 3, -11, -27, -5, -24, 4, 6, 3, 0, 1, -1, 1, 1, 0},
	{0, 0, 0, -1, 2, -1, -2, 1, 0, -7, 1, -6, -14, 4, -10, -28, -6, -27, 3, 4, 1, -1, 1, -2, -1, -1, -1},
	{0, 0, 0, 1, 2, 0, -4, 0, -2, -7, 2, -4, -12, 4, -9, -29, -6, -27, 3, 4, 1, -1, 1, -2, 0, -1, -1},
	{0, -1, -2, -1, 2, 0, -2, 0, -1, -7, 2, -5, -13, 3, -9, -28, -6, -27, 3, 4, 1, -1, 1, -2, 0, -1, -1},
	{0, -1, -2, -1, 2, 1, -2, 0, -1, -7, 1, -6, -13, 2, -9, -26, -4, -25, 3, 4, 1, -1, 1, -2, 2, 0, 2},
	{0, -1, -2, -1, 2, 0, -2, 0, 0, -7, 1, -6, -12, 2, -9, -24, -4, -24, 3, 4, 1, -1, 1, -2, 2, 0, 2},
	{0, -1, -2, -1, 2, 0, -1, 0, 0, -7, 1, -6, -12, 2, -9, -23, -4, -24, 3, 4, 1, -1, 1, -2, 2, 0, 2},
	{-1, -1, -2, 0, 1, 0, -2, 1, -1, -6, 1, -5, -14, 0, -11, -21, -3, -23, 3, 4, 2, 0, 1, -1, 2, 0, 2},
	{-1, -1, -2, 0, 1, 0, -2, 1, -1, -6, 1, -5, -13, 0, -11, -21, -3, -23, 3, 4, 2, 0, 1, -1, 2, 0, 2},
	{-1, -1, -2, 0, 1, 0, -2, 1, -1, -6, 1, -5, -11, 1, -10, -18, -1, -22, 3, 5, 2, 3, 3, 2, 3, 0, 2},
	{0, -1, -2, -1, 1, 0, -2, 1, -1, -4, 1, -4, -11, 1, -10, -18, -1, -22, 3, 5, 3, 6, 4, 5, 3, 1, 2},
	{0, -1, -2, -1, 1, -1, -2, 1, 0, -4, 1, -4, -11, 0, -10, -17, 2, -21, 0, 1, -1, 6, 4, 4, 3, 1, 2},
	{0, -1, -1, 0, 2, 0, -1, 1, -1, -4, 1, -4, -11, 0, -9, -19, 0, -23, 0, 1, -1, 6, 5, 5, 3, 1, 2},
	{-1, -1, -1, 0, 2, 0, -1, 1, -1, -4, 1, -4, -10, 0, -8, -18, 0, -20, -2, 0, -10, -2, 0, -2, 3, 1, 2},
	{0, -1, -1, 0, 2, 0, -1, 1, -1, -4, 0, -4, -11, 0, -9, -17, 0, -19, -4, 0, -12, -1, 0, -2, 3, 1, 2},
	{0, -1, -1, 0, 2, 0, -1, 1, -1, -4, 0, -4, -8, 2, -6, -17, -1, -19, -15, -3, -23, 8, 7, 7, 3, 1, 2},
	{0, -1, -1, 0, 2, 0, -1, 0, -1, -3, 1, -3, -9, 0, -8, -15, 1, -15, -14, 2, -20, 12, 9, 11, 4, 1, 4},
	{0, -1, -1, 0, 2, 0, -1, 0, -1, -3, 1, -3, -7, 2, -6, -14, 1, -14, -12, 3, -18, 12, 9, 11, 4, 2, 4},
	{0, -1, -1, 0, 2, 0, -1, 0, -1, -3, 1, -3, -7, 0, -6, -16, 0, -15, -12, 2, -19, 12, 9, 11, 4, 2, 4},
	{0, -1, -1, 0, 2, 0, -1, 0, -1, -3, 0, -3, -7, 0, -6, -14, 2, -14, -10, 3, -16, 6, 4, 5, 4, 2, 4},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -4, 0, -4, -6, 1, -5, -14, 0, -14, -12, 3, -17, 6, 4, 6, 5, 2, 4},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -2, 1, -2, -6, 1, -5, -14, 1, -13, -10, 4, -15, 5, 3, 5, 5, 2, 4},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -2, 1, -2, -6, 0, -5, -14, 0, -13, -13, 3, -17, 5, 3, 5, 5, 2, 4},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -2, 0, -2, -4, 3, -3, -13, 0, -12, -13, 2, -18, 4, 2, 4, 5, 2, 5},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -2, 0, -2, -4, 2, -3, -13, 0, -12, -15, 1, -19, 3, 1, 3, 5, 2, 5},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -2, 0, -2, -5, 1, -4, -13, -1, -12, -15, 0, -19, 3, 1, 3, 6, 3, 6},
	{0, -1, -1, 0, 1, 0, -1, 0, -1, -1, 1, -1, -3, 2, -2, -10, -1, -9, -16, 0, -19, 1, 0, 1, 6, 2, 5},
	{0, -1, -1, 0, 0, 0, -1, 1, -1, 0, 1, 0, -4, 1, -3, -11, -2, -10, -12, 4, -15, 1, 0, 1, 8, 2, 7},
	{0, -1, -1, 0, 0, 0, 0, 1, 0, -1, 1, -1, -5, 0, -4, -10, -3, -9, -14, 2, -17, 1, 0, 1, 7, 1, 6},
	{0, -1, -1, 0, 0, 0, -1, 0, -1, -1, 1, -1, -2, 1, -2, -4, 3, -2, -10, 1, -12, 2, 0, 2, 9, 0, 8},
	{0, -1, -1, 0, 0, 0, -1, 0, -1, -1, 0, -1, -2, 2, -2, -3, 2, -2, -11, 0, -12, 1, -2, 1, 9, 0, 8},
	{0, -1, -1, 0, 0, 0, -1, 0, -1, -1, 0, -1, -3, 0, -3, -3, 1, -2, -11, 0, -11, 5, 3, 5, 10, -2, 9},
	{1, -1, -1, -1, 0, 0, -2, 0, -1, 0, 0, 0, -2, 1, -1, -4, 0, -3, -12, -2, -12, 4, 2, 4, 10, -2, 9},
	{-1, -3, -3, 0, 0, 0, -1, 0, -1, 0, 1, 0, -4, -1, -3, -6, -2, -6, -6, 3, -6, 4, 2, 4, 11, -2, 10},
	{0, -2, -1, 0, 0, 0, -2, -1, -2, 0, 1, 0, -1, 1, -1, 0, 2, 0, -7, 2, -6, 1, 0, 1, 11, -2, 10},
	{1, -1, -1, -1, 0, 0, -2, 0, -2, -1, 0, -1, -1, 1, -1, -3, 1, -3, -7, 2, -6, -2, 0, -2, 19, 3, 18},
	{0, -2, -1, -1, -1, -1, -2, -1, -2, 1, 1, 0, -2, 1, -1, -6, -1, -5, -7, 0, -7, 1, 2, 1, 17, 1, 16},
	{1, -1, 0, -1, 0, -2, -1, 0, -1, 1, 1, 1, -1, 2, -1, -5, -1, -4, -9, -1, -8, 2, 3, 2, 11, -1, 10},
	{0, -1, 0, 0, 0, 0, -2, 0, -1, 1, 2, 1, -2, 0, -2, -6, -2, -6, -4, 2, -3, -7, -4, -7, 9, 0, 7},
	{-1, -2, -2, -1, -1, -1, -1, 0, -1, 0, 0, 0, -3, 0, -1, -3, 2, -2, -4, 0, -4, -16, 0, -18, 10, -2, 9},
	{0, -1, -1, -1, 0, 0, -1, 0, -1, 2, 2, 0, -4, -2, -3, -3, 1, -3, -5, -1, -5, -17, 4, -18, 17, 4, 16},
	{0, -2, -1, -2, -1, -1, -1, 1, 0, 1, 1, 1, -4, -2, -4, -4, -2, -4, -8, -3, -7, -20, 1, -21, 14, 6, 13},
	{1, -1, 0, -1, -1, -1, -1, 0, -1, 2, 3, 2, -5, -3, -4, -5, -2, -5, -4, 0, -3, -15, 6, -15, 17, 6, 16},
	{1, -1, 0, -2, -1, 0, 0, 0, -1, 1, 2, 1, -4, -3, -4, -4, 0, -3, -5, -1, -3, -15, 5, -15, 15, 5, 14},
	{1, -1, 0, -2, -1, -1, 0, 0, -1, 0, 1, 1, -3, -2, -3, -5, -2, -4, -8, -3, -6, -11, 8, -12, 15, 5, 14},
	{1, -1, 0, 0, -1, -1, -1, 0, -1, 1, 1, 1, -4, -2, -3, -4, -2, -4, -5, -1, -4, -12, 7, -12, 15, 5, 14},
	{0, -1, 1, -1, -1, -2, 0, 1, 0, 1, 2, 1, -3, -3, -3, -5, -2, -4, -3, 0, -3, -12, 5, -12, 15, 5, 14},
	{2, 0, 1, -2, -1, -2, 0, 1, 0, 1, 1, 1, -3, -2, -3, -4, -3, -3, 0, 3, 0, -8, 9, -7, 17, 4, 16},
	{0, -1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, -2, -1, -1, -3, -2, -2, -1, 2, -1, -7, 9, -6, 19, 3, 18},
	{1, 0, 2, -1, -1, -1, 0, 1, 0, 0, 1, 0, -1, -1, -1, -2, 0, -1, -1, 2, -1, -9, 6, -8, 17, 4, 16},
	{1, -1, 0, -1, -1, -1, 0, 1, 0, 0, 1, 0, -1, -2, -1, -2, 0, -1, -1, 1, -1, -11, 4, -9, 17, 4, 16},
	{2, 0, 2, -1, -1, -1, 0, 1, 0, 0, 0, 0, -2, -2, -2, -2, 0, -1, -1, 0, -1, -14, 1, -12, 18, 5, 17},
	{1, -1, 1, -1, -1, -2, 1, 1, 1, -1, -1, -1, -1, -1, -1, -3, -1, -2, -1, -1, -1, -14, 0, -12, 17, 4, 16},
	{0, -1, 0, 0, -1, -2, 0, 1, 1, 0, -1, 0, -1, 0, -1, -3, -2, -2, -1, -1, -1, -14, -1, -12, 18, 5, 17},
	{0, -1, 0, -1, -1, -1, 0, 1, 0, 0, 0, 1, 1, 0, 0, -1, 0, 0, 2, 1, 2, -15, -2, -12, 15, 1, 14},
	{0, 0, -1, 0, -1, -1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, -1, 1, -15, -3, -12, 10, -2, 9},
	{-1, -2, -2, 0, -1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -2, -1, -4, 4, -1, 11, -2, 10},
	{0, -1, -1, -1, -1, -1, 0, 1, 0, 0, 1, 0, 2, 2, 2, 2, 3, 2, -1, -3, -2, -4, 3, -1, 9, -2, 8},
	{0, -1, 0, 0, -1, -1, 0, 0, 0, -1, 0, 0, 2, 1, 1, 1, 2, 1, 1, 0, 1, -4, 1, -1, 7, -1, 6},
	{0, -1, -2, 0, -1, 0, -1, 1, -1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 3, 2, 4, -1, 3},
	{-1, -2, -2, 1, 0, 0, -1, 0, 0, -1, 0, 0, 2, 1, 1, 1, 2, 3, 0, -1, 0, 1, 3, 3, -2, -2, -3},
	{0, -1, -2, 1, 0, 1, -1, 0, 0, -1, 1, 0, 2, 1, 1, -1, 0, 1, 3, 1, 2, 1, 3, 4, -3, -2, -4},
	{0, -1, -1, 1, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 2, 2, 4, -8, -2, -7},
	{0, -1, -1, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 3, -8, -2, -7},
	{0, -1, -1, 0, 0, 0, -1, -1, -1, 1, 1, 1, 0, 0, 0, 1, 1, 2, 2, 1, 2, 1, 0, 3, -9, -3, -7},
	{0, -1, -1, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 1, -1, -1, 0, 2, 1, 1, 2, 1, 4, -7, -2, -6},
	{0, -1, -1, 0, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, 0, 1, 1, 2, 3, 2, 2, 3, 1, 4, -6, -3, -6},
	{1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 2, 3, 1, 4, -6, -3, -6},
	{2, 1, 2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, -2, -2, -3, 0, -1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/************
 *	  HMT   *
 ************/
static unsigned int base_luminance_reverse_hmt_single[][2] = {
	{10, 48},
	{11, 52},
	{12, 57},
	{13, 62},
	{14, 66},
	{15, 71},
	{16, 76},
	{17, 80},
	{19, 90},
	{20, 94},
	{21, 98},
	{22, 102},
	{23, 106},
	{25, 115},
	{27, 123},
	{29, 130},
	{31, 140},
	{33, 149},
	{35, 157},
	{37, 166},
	{39, 175},
	{41, 181},
	{44, 192},
	{47, 204},
	{50, 216},
	{53, 228},
	{56, 240},
	{60, 257},
	{64, 269},
	{68, 285},
	{72, 297},
	{77, 230},
	{82, 244},
	{87, 256},
	{93, 271},
	{99, 287},
	{105, 299},
};

static int gradation_offset_reverse_hmt_single[][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 1, 2, 4, 6, 8, 10, 12, 13},
	{0, 1, 2, 4, 6, 8, 10, 12, 13},
	{0, 1, 2, 4, 6, 8, 9, 12, 13},
	{0, 0, 3, 4, 6, 8, 10, 12, 13},
	{0, 0, 2, 3, 5, 7, 10, 12, 13},
	{0, 0, 2, 3, 6, 7, 9, 11, 13},
	{0, 0, 2, 3, 6, 7, 9, 11, 12},
	{0, 0, 1, 3, 6, 7, 9, 11, 12},
	{0, 1, 1, 3, 6, 7, 9, 11, 12},
	{0, 1, 2, 2, 6, 7, 9, 11, 12},
	{0, 1, 1, 2, 5, 7, 8, 11, 12},
	{0, 1, 1, 2, 5, 7, 8, 11, 12},
	{0, 1, 1, 2, 5, 6, 8, 11, 12},
	{0, 2, 2, 3, 5, 7, 8, 11, 12},
	{0, 2, 3, 3, 6, 7, 8, 11, 12},
	{0, 2, 3, 3, 5, 7, 8, 11, 12},
	{0, 2, 3, 3, 5, 7, 7, 11, 12},
	{0, 2, 3, 3, 5, 7, 7, 11, 12},
	{0, 2, 3, 3, 5, 7, 7, 10, 12},
	{0, 2, 4, 4, 5, 6, 7, 10, 12},
	{0, 2, 4, 4, 4, 6, 7, 10, 12},
	{0, 2, 5, 4, 5, 6, 6, 10, 12},
	{0, 2, 5, 4, 5, 5, 7, 10, 12},
	{0, 2, 5, 4, 5, 6, 7, 10, 12},
	{0, 1, 5, 4, 4, 5, 6, 9, 11},
	{0, 2, 6, 5, 5, 6, 7, 10, 12},
	{0, 3, 7, 5, 5, 6, 6, 9, 11},
	{0, 3, 6, 5, 5, 6, 7, 10, 12},
	{0, 3, 6, 5, 5, 5, 6, 9, 11},
	{0, 5, 8, 5, 5, 6, 6, 9, 12},
	{0, 6, 7, 5, 5, 6, 6, 9, 12},
	{0, 2, 5, 3, 3, 3, 4, 5, 7},
	{0, 2, 5, 3, 3, 3, 3, 6, 7},
	{0, 3, 6, 4, 3, 3, 4, 6, 7},
	{0, 4, 6, 4, 4, 3, 3, 5, 7},
	{0, 4, 6, 4, 4, 3, 4, 6, 8},
	{0, 5, 7, 5, 4, 3, 3, 6, 8},
};

static int rgb_offset_reverse_hmt_single[][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{0, 0, 1, -1, 0, -1, -3, 0, -2, -4, 2, -4, -11, 3, -11, -11, 1, -11, -10, 4, -9, -7, 2, -6, -11, 1, -4},
	{0, 0, 1, -1, -1, -1, -1, 1, 0, -5, 2, -4, -10, 3, -11, -11, 1, -12, -12, 3, -9, -10, 0, -8, -4, 1, -3},
	{0, 0, 1, -1, -1, -1, -2, 0, -2, -5, 1, -5, -9, 3, -10, -10, 1, -10, -12, 3, -10, -10, 0, -8, -4, 1, -3},
	{-1, 0, 1, -2, -2, -2, -1, -1, -2, -4, 0, -4, -8, 4, -9, -9, 2, -8, -11, 2, -10, -7, 3, -6, -4, 1, -2},
	{0, 0, 1, -2, -1, -1, 0, 0, 0, -4, 2, -4, -9, 2, -10, -12, 1, -12, -12, 1, -11, -8, 4, -9, -4, 1, -2},
	{0, 0, 1, -1, 0, 0, -1, 0, -1, -4, 1, -4, -10, 1, -10, -10, 4, -10, -8, 2, -9, -7, 3, -9, -3, 1, -4},
	{0, 0, 0, -1, -1, -1, -1, 0, 0, -4, 0, -4, -9, 1, -9, -10, 3, -11, -7, 2, -7, -5, 4, -10, -6, 1, -4},
	{0, 0, 1, -1, -1, -1, -1, 0, -2, -3, 0, -3, -8, 2, -8, -7, 3, -7, -9, 4, -10, -5, 2, -8, -4, 1, -4},
	{0, 0, 1, 0, 0, 0, -1, 0, -1, -3, 0, -4, -9, 1, -9, -9, 2, -9, -10, 3, -9, -3, 3, -8, -4, 1, -4},
	{-1, 0, 0, 0, 0, 0, -1, 0, -1, -2, 1, -3, -9, -1, -9, -8, 2, -10, -10, 4, -8, -10, 4, -8, -3, 1, -3},
	{-1, 0, 1, 0, 0, 0, 0, 0, 0, -2, 1, -2, -8, 0, -9, -9, 1, -10, -9, 3, -8, -10, 3, -8, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 1, -3, -8, 0, -7, -7, 2, -7, -7, 3, -7, -10, 3, -10, -3, 1, -4},
	{0, 0, 1, 0, 0, 0, -2, -1, -2, -2, 1, -3, -7, 0, -7, -7, 3, -8, -6, 4, -6, -11, 3, -11, -6, 1, -4},
	{0, 0, 1, 0, 0, 0, -2, -1, -1, -1, 1, -2, -9, 0, -8, -8, 2, -9, -8, 3, -8, -11, 2, -12, -3, 1, -3},
	{0, 0, 1, 0, 0, 0, -2, -1, -1, -2, 1, -2, -8, 0, -8, -8, 2, -7, -8, 3, -10, -10, 3, -10, -5, 1, -4},
	{0, 0, 1, 0, 0, 0, -1, -1, -1, -2, 0, -3, -9, 0, -8, -8, 2, -8, -7, 4, -10, -10, 3, -10, -3, 1, -4},
	{0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, -2, -8, 0, -7, -7, 2, -7, -8, 3, -8, -12, 3, -12, -3, 1, -4},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -2, 1, -2, -7, -1, -8, -6, 2, -7, -8, 3, -8, -8, 4, -10, -4, 1, -4},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, -2, 0, -1, -8, -1, -8, -5, 1, -6, -9, 3, -8, -11, 4, -11, -3, 0, -4},
	{0, 0, 1, 0, 0, 1, 0, 0, 1, -2, 0, -1, -7, 0, -6, -7, 1, -6, -7, 3, -9, -10, 4, -12, -3, 0, -4},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -4, 0, -3, -7, 0, -6, -7, 1, -6, -7, 3, -9, -10, 4, -12, -3, 0, -4},
	{1, 0, 2, 1, 0, 0, -1, 0, 0, -3, 0, -2, -6, 1, -5, -5, 2, -6, -8, 3, -8, -10, 4, -12, -3, 0, -4},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -2, -4, 2, -4, -5, 2, -6, -8, 3, -8, -11, 4, -12, -3, 0, -4},
	{0, 0, 2, 1, 0, 0, 0, 0, 0, -2, 0, -2, -6, 1, -5, -4, 2, -6, -8, 3, -8, -10, 4, -10, -6, 0, -6},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -2, 0, -1, -5, 1, -4, -5, 2, -5, -8, 3, -7, -12, 4, -12, -2, 0, -4},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, -2, 0, -1, -4, 0, -4, -5, 0, -6, -8, 3, -8, -11, 4, -10, -5, 0, -4},
	{0, 0, 2, 0, 0, 0, 0, 0, 0, -1, 0, -1, -4, 1, -4, -4, 2, -5, -7, 3, -7, -14, 4, -14, -5, 0, -4},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -3, 1, -3, -4, 2, -6, -7, 3, -7, -14, 4, -14, -5, 0, -4},
	{-1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, -4, 1, -3, -4, 2, -5, -5, 3, -6, -14, 4, -14, -4, 1, -5},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -4, 1, -4, -5, 2, -5, -6, 4, -6, -14, 4, -14, -4, 1, -5},
	{1, 0, 2, 0, 0, 0, 0, 0, -1, -2, 0, -1, -3, 0, -4, -5, 1, -5, -6, 3, -6, -13, 6, -12, -6, 2, -6},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, -1, 0, -1, -2, 0, -2, -5, 1, -4, -6, 2, -5, -11, 5, -12, -3, 1, -4},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, -1, 0, 0, -3, 0, -2, -5, 0, -4, -5, 2, -6, -11, 5, -11, -4, 1, -4},
	{1, 0, 3, 0, 0, 0, 0, 0, -1, -1, 0, -1, -2, 0, -2, -3, 1, -4, -5, 2, -6, -10, 5, -11, -4, 1, -4},
	{0, 0, 2, 0, 0, 0, 0, 0, -1, -1, -1, 0, -1, 1, -2, -3, 1, -3, -4, 2, -5, -12, 5, -11, -4, 1, -4},
	{1, 0, 2, 0, 0, -1, 0, 0, 0, 0, 0, 0, -2, 0, -2, -3, 1, -4, -5, 2, -5, -11, 4, -11, -6, 2, -6},
	{2, 0, 2, 0, 0, 0, -1, 0, 0, -2, 0, -1, -2, 0, -3, -3, 1, -4, -5, 2, -5, -11, 4, -11, -6, 2, -6},
};

#endif
