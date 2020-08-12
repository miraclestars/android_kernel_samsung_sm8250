/*
 * =================================================================
 *
 *       Filename:  ss_dsi_smart_dimming_S6E3FA7_AMS628RF01.h
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
#ifndef _SS_DSI_SMART_DIMMING_S6E3FA7_AMS628RF01_H_
#define _SS_DSI_SMART_DIMMING_S6E3FA7_AMS628RF01_H_

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

#define VREG0_REF_6P8 28521267	/* 6.8*4194304 */
#define VREG0_REF_6P5 27262976	/* 6.5*4194304 */
#define VREG0_REF_6P4 26843546	/* 6.4*4194304 */
#define VREG0_REF_6P3 26424115	/* 6.3*4194304 */
#define VREG0_REF_6P2 26004685	/* 6.2*4194304 */

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

/* Jackpot2 rev.A */
static const unsigned int base_luminance_revA[LUMINANCE_MAX][2] = {
	{2, 131},
	{3, 131},
	{4, 131},
	{5, 131},
	{6, 131},
	{7, 131},
	{8, 131},
	{9, 131},
	{10, 131},
	{11, 131},
	{12, 131},
	{13, 131},
	{14, 131},
	{15, 131},
	{16, 131},
	{17, 131},
	{19, 131},
	{20, 131},
	{21, 131},
	{22, 131},
	{24, 131},
	{25, 131},
	{27, 131},
	{29, 131},
	{30, 131},
	{32, 131},
	{34, 131},
	{37, 131},
	{39, 131},
	{41, 131},
	{44, 131},
	{47, 131},
	{50, 131},
	{53, 131},
	{56, 131},
	{60, 131},
	{64, 131},
	{68, 138},
	{72, 147},
	{77, 155},
	{82, 163},
	{87, 174},
	{93, 183},
	{98, 186},
	{105, 205},
	{111, 212},
	{119, 222},
	{126, 235},
	{134, 249},
	{143, 260},
	{152, 277},
	{162, 289},
	{172, 300},
	{183, 307},
	{195, 307},
	{207, 307},
	{220, 307},
	{234, 307},
	{249, 313},
	{265, 319},
	{282, 335},
	{300, 358},
	{316, 368},
	{333, 385},
	{350, 399},
	{357, 402},
	{365, 409},
	{372, 409},
	{380, 409},
	{387, 409},
	{395, 409},
	{403, 413},
	{412, 416},
	{420, 420},
};

static const unsigned int base_luminance_revC[LUMINANCE_MAX][2] = {
	{ 2, 118 },
	{ 3, 118 },
	{ 4, 118 },
	{ 5, 118 },
	{ 6, 118 },
	{ 7, 118 },
	{ 8, 118 },
	{ 9, 118 },
	{ 10, 118 },
	{ 11, 118 },
	{ 12, 118 },
	{ 13, 118 },
	{ 14, 118 },
	{ 15, 118 },
	{ 16, 118 },
	{ 17, 118 },
	{ 19, 118 },
	{ 20, 118 },
	{ 21, 118 },
	{ 22, 118 },
	{ 24, 118 },
	{ 25, 118 },
	{ 27, 118 },
	{ 29, 118 },
	{ 30, 118 },
	{ 32, 118 },
	{ 34, 118 },
	{ 37, 118 },
	{ 39, 118 },
	{ 41, 118 },
	{ 44, 118 },
	{ 47, 118 },
	{ 50, 118 },
	{ 53, 118 },
	{ 56, 118 },
	{ 60, 118 },
	{ 64, 118 },
	{ 68, 123 },
	{ 72, 131 },
	{ 77, 138 },
	{ 82, 147 },
	{ 87, 155 },
	{ 93, 163 },
	{ 98, 172 },
	{ 105, 183 },
	{ 111, 195 },
	{ 119, 207 },
	{ 126, 215 },
	{ 134, 230 },
	{ 143, 241 },
	{ 152, 257 },
	{ 162, 265 },
	{ 172, 277 },
	{ 183, 283 },
	{ 195, 283 },
	{ 207, 283 },
	{ 220, 283 },
	{ 234, 283 },
	{ 249, 304 },
	{ 265, 319 },
	{ 282, 335 },
	{ 300, 351 },
	{ 316, 364 },
	{ 333, 378 },
	{ 350, 392 },
	{ 357, 395 },
	{ 365, 402 },
	{ 372, 402 },
	{ 380, 402 },
	{ 387, 402 },
	{ 395, 402 },
	{ 403, 409 },
	{ 412, 416 },
	{ 420, 420 },
};

static const int gradation_offset_revA[LUMINANCE_MAX][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 5, 10, 16, 21, 26, 31, 34, 29},
	{0, 4, 7, 12, 17, 22, 27, 30, 29},
	{0, 4, 6, 11, 15, 19, 24, 27, 28},
	{0, 3, 5, 9, 13, 17, 22, 25, 26},
	{0, 3, 5, 8, 11, 15, 20, 24, 25},
	{0, 3, 4, 7, 11, 14, 19, 23, 23},
	{0, 3, 4, 7, 10, 13, 18, 22, 22},
	{0, 3, 4, 7, 9, 12, 17, 21, 22},
	{0, 3, 4, 7, 9, 12, 17, 20, 22},
	{0, 3, 4, 7, 9, 12, 16, 20, 21},
	{0, 3, 4, 7, 9, 12, 16, 20, 21},
	{0, 3, 4, 7, 9, 12, 16, 20, 21},
	{0, 3, 4, 7, 9, 12, 17, 20, 21},
	{0, 3, 4, 7, 9, 12, 17, 21, 22},
	{0, 3, 4, 7, 9, 12, 17, 21, 22},
	{0, 3, 4, 6, 9, 12, 16, 20, 21},
	{0, 2, 3, 5, 7, 10, 14, 18, 19},
	{0, 2, 3, 5, 7, 10, 14, 18, 19},
	{0, 2, 3, 5, 7, 9, 14, 17, 18},
	{0, 2, 3, 5, 6, 9, 13, 16, 18},
	{0, 2, 3, 4, 6, 9, 12, 15, 17},
	{0, 2, 3, 4, 5, 8, 12, 15, 17},
	{0, 2, 2, 4, 5, 7, 11, 14, 16},
	{0, 2, 2, 3, 5, 7, 10, 14, 15},
	{0, 2, 2, 3, 4, 6, 10, 14, 14},
	{0, 2, 2, 2, 4, 5, 9, 13, 15},
	{0, 3, 4, 5, 6, 7, 9, 13, 15},
	{0, 3, 4, 5, 5, 6, 9, 12, 14},
	{0, 3, 4, 5, 5, 6, 8, 12, 14},
	{0, 3, 3, 4, 4, 5, 8, 11, 13},
	{0, 3, 3, 4, 4, 5, 7, 11, 13},
	{0, 3, 3, 4, 4, 4, 7, 10, 12},
	{0, 3, 3, 4, 4, 4, 6, 9, 11},
	{0, 3, 3, 4, 3, 4, 6, 9, 11},
	{0, 2, 2, 3, 3, 3, 6, 9, 11},
	{0, 1, 0, 1, 2, 2, 5, 8, 11},
	{0, 1, 1, 1, 2, 3, 5, 7, 9},
	{0, 1, 1, 1, 2, 3, 4, 7, 8},
	{0, 2, 1, 1, 2, 3, 4, 8, 9},
	{0, 1, 1, 1, 1, 3, 5, 8, 10},
	{0, 1, 2, 1, 2, 2, 4, 6, 9},
	{0, 0, 1, 1, 2, 2, 4, 6, 9},
	{0, 0, 1, 1, 1, 2, 3, 7, 9},
	{2, 2, 3, 1, 1, 2, 3, 6, 9},
	{0, 0, 2, 1, 1, 2, 3, 5, 9},
	{0, -1, 1, 1, 0, 1, 2, 5, 8},
	{0, -1, 1, 2, 2, 2, 3, 6, 8},
	{0, -1, 2, 2, 2, 2, 3, 7, 9},
	{0, 0, 2, 2, 2, 2, 4, 7, 9},
	{0, -1, 1, 1, 1, 2, 3, 8, 10},
	{0, 0, 2, 1, 1, 2, 3, 6, 9},
	{0, 1, 3, 1, 0, 1, 2, 4, 7},
	{0, 0, 1, 0, 0, 0, 1, 3, 5},
	{0, 3, 4, 3, 2, 1, 1, 4, 4},
	{0, 3, 4, 2, 0, 0, 1, 3, 5},
	{0, 3, 3, 2, 1, 0, 1, 5, 5},
	{0, 3, 3, 1, 0, 0, 1, 3, 5},
	{0, 3, 3, 1, 0, 0, 0, 2, 3},
	{0, 3, 2, 0, 0, 0, 0, 2, 3},
	{0, 3, 2, 0, 0, 0, 0, 2, 3},
	{0, 0, 1, 0, 0, 0, 0, 0, 2},
	{0, 0, 1, 0, 0, 0, 0, 0, 2},
	{0, 0, 1, 0, 0, 0, -1, 0, 2},
	{0, 0, 1, -1, -1, -1, 0, 0, 1},
	{0, 0, -1, -1, -1, 0, -1, 0, 1},
	{0, 0, 0, -1, -2, -2, -1, -1, 1},
	{0, 0, 0, 0, -1, -1, -1, -1, 0},
	{0, 0, -1, 0, -1, -1, -1, -1, 0},
	{0, 0, -1, 0, 0, -1, -1, -1, 0},
	{0, 0, -1, 0, 0, -1, -1, -1, 0},
	{0, 0, -1, 0, 0, -1, -1, -1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static const int gradation_offset_revC[LUMINANCE_MAX][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 6, 8, 15, 22, 27, 29, 30, 31},
	{0, 5, 6, 12, 18, 23, 27, 28, 29},
	{0, 4, 4, 10, 16, 20, 24, 27, 28},
	{0, 3, 3, 8, 14, 18, 22, 25, 26},
	{0, 3, 2, 7, 13, 16, 20, 24, 25},
	{0, 3, 2, 6, 12, 14, 18, 23, 24},
	{0, 3, 2, 6, 11, 14, 18, 23, 24},
	{0, 3, 2, 5, 10, 13, 16, 21, 23},
	{0, 3, 2, 5, 10, 13, 16, 21, 23},
	{0, 3, 2, 5, 9, 12, 16, 21, 22},
	{0, 3, 2, 5, 9, 12, 16, 21, 22},
	{0, 3, 2, 5, 9, 12, 16, 21, 22},
	{0, 3, 2, 5, 9, 12, 16, 21, 22},
	{0, 3, 2, 5, 9, 12, 16, 20, 21},
	{0, 3, 2, 5, 9, 11, 15, 20, 21},
	{0, 3, 1, 5, 8, 11, 14, 18, 20},
	{0, 2, 1, 4, 8, 10, 13, 17, 19},
	{0, 2, 1, 4, 7, 9, 12, 16, 18},
	{0, 2, 1, 3, 6, 9, 12, 16, 18},
	{0, 2, 1, 3, 6, 8, 12, 15, 18},
	{0, 2, 1, 3, 5, 8, 11, 15, 18},
	{0, 2, 1, 3, 5, 7, 10, 14, 18},
	{0, 2, 0, 2, 5, 7, 10, 14, 17},
	{0, 2, 0, 2, 4, 6, 9, 13, 16},
	{0, 2, 0, 2, 4, 6, 9, 13, 16},
	{0, 2, 0, 1, 4, 5, 8, 12, 15},
	{0, 2, 0, 1, 4, 5, 8, 12, 15},
	{0, 2, 0, 1, 3, 4, 7, 11, 14},
	{0, 2, 0, 1, 3, 4, 7, 11, 13},
	{0, 2, 0, 1, 3, 4, 6, 10, 12},
	{0, 1, 0, 1, 2, 3, 5, 9, 11},
	{0, 1, 0, 1, 2, 3, 5, 9, 10},
	{0, 1, 0, 1, 2, 2, 5, 8, 10},
	{0, 1, 0, 1, 2, 2, 5, 8, 9},
	{0, 1, 0, 1, 2, 2, 4, 7, 8},
	{0, 1, 0, 1, 2, 2, 4, 6, 8},
	{0, 1, 0, 0, 1, 2, 4, 6, 8},
	{0, 1, 1, 1, 2, 2, 4, 6, 8},
	{0, 1, 1, 1, 1, 2, 4, 6, 8},
	{0, 1, 1, 1, 1, 2, 4, 7, 8},
	{0, 1, 1, 1, 1, 2, 4, 6, 7},
	{0, 0, 1, 1, 1, 2, 4, 6, 7},
	{0, 0, 1, 1, 1, 2, 4, 6, 7},
	{0, -1, 1, 1, 1, 1, 3, 5, 7},
	{0, -1, 1, 1, 1, 1, 3, 5, 6},
	{0, -1, 1, 1, 1, 1, 3, 5, 6},
	{0, -1, 1, 1, 1, 1, 2, 4, 5},
	{0, -1, 1, 1, 1, 1, 2, 4, 5},
	{0, -1, 1, 1, 1, 0, 2, 4, 5},
	{0, -1, 1, 1, 1, 1, 2, 4, 5},
	{0, -1, 1, 1, 1, 0, 1, 3, 4},
	{0, 0, 2, 1, 1, 0, 1, 3, 4},
	{0, 0, 2, 1, 1, 0, 1, 3, 4},
	{0, 0, 2, 1, 1, 0, 1, 3, 4},
	{0, 0, 2, 1, 1, 0, 1, 3, 4},
	{0, 0, 1, 1, 0, 0, 1, 2, 4},
	{0, 0, 0, 0, 0, 0, 1, 2, 3},
	{0, -1, 3, 1, 0, 0, 0, 2, 3},
	{0, 0, 1, 0, 0, 0, 0, 2, 3},
	{0, 0, 1, 0, 0, 0, 0, 2, 2},
	{0, 0, 1, 0, 0, 0, 0, 0, 2},
	{0, 0, 1, 0, 0, 0, 0, 0, 2},
	{0, 1, 1, 0, 0, 0, -1, 0, 2},
	{0, 0, 1, -1, -1, -1, 0, 0, 1},
	{0, 0, -1, -1, -1, 0, -1, 0, 1},
	{0, 0, 0, -1, -1, -2, -1, -1, 1},
	{0, 0, 0, 0, -1, -1, -1, -1, 0},
	{0, 0, -1, 0, -1, -1, -1, -1, 0},
	{0, 0, -1, -1, -1, -1, -1, -1, 0},
	{0, 0, -1, -1, -1, -1, -1, -1, 0},
	{0, 0, -1, -1, -1, -1, -1, -1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},

};

static const int rgb_offset_revA[LUMINANCE_MAX][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-6, 0, -3, -5, 0, -2, -7, 0, -3, -9, 0, -9, -13, 0, -9, -13, 0, -8, -8, 0, -6, -6, 0, -3, -1, 0, 17},
	{-3, 0, -1, -4, 0, -1, -6, 0, -2, -8, 0, -10, -13, 0, -10, -13, 0, -9, -8, 0, -8, -7, 0, -7, -2, 0, 12},
	{-3, 0, -1, -4, 0, -1, -6, 0, -2, -7, 0, -8, -13, 0, -11, -12, 0, -8, -8, 0, -8, -6, 0, -8, -2, 0, 10},
	{-1, 0, 0, -4, 0, -1, -6, 0, -2, -6, 0, -8, -11, 0, -13, -14, 0, -10, -11, 0, -10, -9, 0, -10, -4, 0, 8},
	{1, 0, 2, -2, 0, -1, -6, 0, -1, -5, 0, -6, -9, 0, -11, -12, 0, -9, -11, 0, -11, -9, 0, -9, -4, 0, 5},
	{1, 0, 2, -2, 0, -1, -6, 0, -1, -5, 0, -6, -9, 0, -11, -11, 0, -9, -11, 0, -11, -9, 0, -8, -4, 0, 6},
	{1, 0, 2, -2, 0, -1, -6, 0, -1, -5, 0, -6, -9, 0, -11, -10, 0, -8, -10, 0, -10, -8, 0, -8, -6, 0, 6},
	{1, 0, 2, -1, 0, -1, -5, 0, -1, -5, 0, -4, -9, 0, -9, -10, 0, -10, -9, 0, -13, -7, 0, -9, -5, 0, 8},
	{1, 0, 3, 0, 0, 0, -4, 0, -2, -4, 0, -5, -7, 0, -8, -12, 0, -9, -11, 0, -14, -9, 0, -11, -6, 0, 6},
	{1, 0, 3, 0, 0, 1, -3, 0, -1, -3, 0, -5, -7, 0, -8, -12, 0, -9, -11, 0, -14, -8, 0, -11, -7, 0, 6},
	{1, 0, 3, 0, 0, 1, -3, 0, -1, -3, 0, -5, -6, 0, -8, -11, 0, -9, -11, 0, -14, -8, 0, -12, -8, 0, 6},
	{1, 0, 4, 0, 0, 1, -3, 0, -1, -3, 0, -5, -6, 0, -8, -10, 0, -9, -10, 0, -14, -8, 0, -13, -8, 0, 6},
	{1, 0, 4, 0, 0, 1, -3, 0, -1, -3, 0, -5, -6, 0, -8, -9, 0, -7, -7, 0, -12, -5, 0, -12, -6, 0, 5},
	{1, 0, 4, 0, 0, 2, -3, 0, 0, -2, 0, -5, -5, 0, -9, -9, 0, -8, -6, 0, -13, -4, 0, -13, -5, 0, 7},
	{1, 0, 4, 0, 0, 2, -3, 0, 0, -2, 0, -5, -5, 0, -9, -9, 0, -8, -7, 0, -13, -3, 0, -13, -4, 0, 7},
	{1, 0, 4, 0, 0, 2, -3, 0, 0, -2, 0, -5, -5, 0, -10, -11, 0, -11, -9, 0, -16, -8, 0, -15, -6, 0, 6},
	{1, 0, 4, 0, 0, 2, -2, 0, 1, -1, 0, -4, -4, 0, -9, -10, 0, -10, -11, 0, -15, -8, 0, -15, -6, 0, 6},
	{2, 0, 2, 0, 0, 2, -2, 0, 1, -1, 0, -4, -4, 0, -8, -9, 0, -9, -10, 0, -15, -9, 0, -15, -7, 0, 5},
	{1, 0, 4, 0, 0, 2, -2, 0, 1, -1, 0, -4, -4, 0, -8, -9, 0, -8, -9, 0, -16, -9, 0, -15, -6, 0, 5},
	{1, 0, 4, 0, 0, 2, -2, 0, 1, -1, 0, -4, -4, 0, -8, -9, 0, -8, -8, 0, -14, -7, 0, -15, -6, 0, 7},
	{1, 0, 4, 0, 0, 2, -2, 0, 1, -1, 0, -4, -3, 0, -7, -8, 0, -6, -9, 0, -14, -7, 0, -15, -5, 0, 6},
	{1, 0, 4, 0, 0, 2, -1, 0, 1, -1, 0, -4, -3, 0, -7, -8, 0, -6, -10, 0, -14, -7, 0, -13, -4, 0, 6},
	{1, 0, 4, 0, 0, 2, -1, 0, 1, 0, 0, -3, -3, 0, -7, -7, 0, -5, -9, 0, -16, -9, 0, -15, -6, 0, 5},
	{1, 0, 4, 1, 0, 2, -1, 0, 1, -1, 0, -3, -2, 0, -7, -8, 0, -5, -8, 0, -13, -12, 0, -16, -4, 0, 0},
	{1, 0, 4, 0, 0, 2, -1, 0, 1, 1, 0, -2, -2, 0, -7, -7, 0, -5, -11, 0, -12, -14, 0, -13, -4, 0, 0},
	{1, 0, 4, 1, 0, 2, -1, 0, 1, 1, 0, -2, -3, 0, -7, -4, 0, -6, -10, 0, -14, -14, 0, -18, -5, 0, -1},
	{4, 0, 4, -1, 0, 2, -2, 0, 1, -1, 0, 0, -2, 0, -6, -4, 0, -4, -9, 0, -13, -7, 0, -16, -2, 0, -6},
	{4, 0, 4, -1, 0, 2, -2, 0, 1, -1, 0, 0, -2, 0, -6, -4, 0, -4, -9, 0, -10, -9, 0, -20, -1, 0, -6},
	{4, 0, 4, -1, 0, 2, -2, 0, 1, -1, 0, 0, -1, 0, -5, -4, 0, -4, -9, 0, -11, -7, 0, -15, -1, 0, -6},
	{4, 0, 4, -1, 0, 2, -1, 0, 1, 0, 0, 0, -1, 0, -5, -4, 0, -3, -8, 0, -10, -9, 0, -20, -2, 0, -8},
	{4, 0, 4, -1, 0, 2, -1, 0, 1, 0, 0, 0, -1, 0, -5, -4, 0, -2, -8, 0, -12, -8, 0, -16, -2, 0, -8},
	{4, 0, 4, -1, 0, 2, -1, 0, 1, 0, 0, 0, 0, 0, -4, -4, 0, -2, -7, 0, -9, -9, 0, -20, -1, 0, -7},
	{4, 0, 4, -1, 0, 2, -1, 0, 1, 0, 0, 0, 0, 0, -4, -3, 0, -1, -7, 0, -11, -10, 0, -22, -1, 0, -6},
	{4, 0, 4, -1, 0, 2, -1, 0, 1, 0, 0, 0, 1, 0, -3, -3, 0, -2, -5, 0, -8, -10, 0, -20, -2, 0, -8},
	{3, 0, 4, 0, 0, 2, 0, 0, 2, 0, 0, 0, -1, 0, -4, -3, 0, -2, -5, 0, -7, -9, 0, -17, -2, 0, -8},
	{3, 0, 5, 2, 0, 2, 0, 0, 2, 0, 0, -2, -1, 0, -4, -4, 0, -2, -9, 0, -12, -15, 0, -17, -5, 0, -13},
	{3, 0, 5, 2, 0, 2, 0, 0, 2, 0, 0, -2, 0, 0, -3, -4, 0, -2, -9, 0, -11, -15, 0, -19, 2, 0, -21},
	{2, 0, 5, 2, 0, 2, -1, 0, 2, 1, 0, -1, 1, 0, -3, -2, 0, -2, -7, 0, -9, -16, 0, -19, -4, 0, -20},
	{2, 0, 5, 1, 0, 1, 0, 0, 4, 1, 0, -1, -1, 0, -4, -3, 0, -2, -7, 0, -8, -12, 0, -16, 0, 0, -17},
	{2, 0, 6, 1, 0, 0, -1, 0, 3, 1, 0, 1, 0, 0, -4, -2, 0, -1, -7, 0, -6, -12, 0, -17, -1, 0, -17},
	{2, 0, 6, 1, 0, 0, 0, 0, 2, 1, 0, -1, 1, 0, -3, -2, 0, -2, -7, 0, -7, -12, 0, -15, -8, 0, -19},
	{1, 0, 4, 2, 0, 2, 1, 0, 3, 0, 0, -2, 0, 0, -3, -3, 0, -1, -6, 0, -7, -10, 0, -15, -6, 0, -16},
	{2, 0, 4, 2, 0, 2, 0, 0, 1, 1, 0, 1, 0, 0, -4, -3, 0, -1, -4, 0, -6, -13, 0, -15, -5, 0, -18},
	{3, 0, 4, 0, 0, 0, 1, 0, 3, 1, 0, -1, -1, 0, -2, -2, 0, -2, -7, 0, -6, -11, 0, -16, -8, 0, -14},
	{2, 0, 5, 1, 0, 1, -1, 0, 2, 1, 0, 0, 1, 0, -2, 0, 0, -1, -6, 0, -6, -12, 0, -13, -2, 0, -21},
	{2, 0, 5, 2, 0, -1, 0, 0, 3, 1, 0, 1, 1, 0, -2, -3, 0, -1, -7, 0, -7, -10, 0, -14, -3, 0, -14},
	{4, 0, 5, 1, 0, 0, -2, 0, 2, 0, 0, 0, 1, 0, -1, 1, 0, -1, -5, 0, -6, -13, 0, -11, -6, 0, -18},
	{3, 0, 4, 0, 0, 1, -2, 0, 1, 1, 0, 0, 1, 0, -2, 1, 0, -2, -6, 0, -7, -8, 0, -14, -7, 0, -15},
	{3, 0, 4, 1, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, -1, 1, 0, -2, -5, 0, -6, -12, 0, -13, -3, 0, -12},
	{3, 0, 4, 2, 0, 1, -1, 0, 1, 2, 0, 1, -1, 0, -2, 0, 0, -2, -4, 0, -4, -13, 0, -18, -10, 0, -17},
	{2, 0, 3, 3, 0, 1, -1, 0, 2, -1, 0, 0, 1, 0, -1, 1, 0, 0, -6, 0, -6, -10, 0, -12, -5, 0, -18},
	{2, 0, 3, 0, 0, 0, 0, 0, 2, 1, 0, 1, 0, 0, -2, -1, 0, -2, -4, 0, -4, -11, 0, -11, -11, 0, -22},
	{1, 0, 2, 2, 0, 1, 0, 0, 2, -1, 0, 0, 2, 0, -1, 0, 0, -1, -3, 0, -2, -13, 0, -13, -9, 0, -22},
	{1, 0, 2, 1, 0, 1, 0, 0, 1, 0, 0, 3, 0, 0, -1, 0, 0, -1, -2, 0, -3, -9, 0, -10, -8, 0, -17},
	{1, 0, 1, 2, 0, 2, -1, 0, 0, -2, 0, 2, 1, 0, 0, 2, 0, 1, -3, 0, -3, -7, 0, -9, -10, 0, -9},
	{1, 0, 1, 2, 0, 2, -1, 0, 1, -2, 0, 3, 2, 0, 0, 2, 0, 0, -1, 0, -3, -5, 0, -5, -5, 0, -3},
	{2, 0, 2, 1, 0, 1, 0, 0, 1, -2, 0, 2, 0, 0, 0, 1, 0, 1, 0, 0, -2, -5, 0, -9, -4, 0, -5},
	{2, 0, 2, 0, 0, 1, 1, 0, 1, -1, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, -2, -7, 0, -9, -6, 0, -8},
	{1, 0, 1, 1, 0, 1, -1, 0, 1, -1, 0, 2, 2, 0, 1, 1, 0, 1, -1, 0, -2, -7, 0, -8, -4, 0, -6},
	{1, 0, 1, 2, 0, 2, 0, 0, 1, -2, 0, 2, 1, 0, -1, 2, 0, 2, -2, 0, -3, -6, 0, -8, -7, 0, -3},
	{1, 0, 0, 2, 0, 2, 1, 0, 1, -2, 0, 3, 1, 0, 1, 0, 0, 0, 1, 0, -2, -7, 0, -10, -7, 0, -7},
	{0, 0, -1, 1, 0, 1, 0, 0, 1, -1, 0, 3, 1, 0, 0, 3, 0, -1, -2, 0, 1, -7, 0, -6, -5, 0, -4},
	{1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 3, 0, 1, -2, 0, -1, -5, 0, -7, -4, 0, -4},
	{0, 0, -1, 1, 0, 1, 1, 0, 2, -2, 0, 2, 0, 0, 0, 3, 0, 1, 0, 0, -1, -3, 0, -5, -7, 0, -6},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, -2, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, -1, 0, 0, -2, -9, 0, -8},
	{1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, -1, 1, 0, 2, -1, 0, -2, -1, 0, -3, -4, 0, -3},
	{-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 1, 0, -1, -5, 0, -4, -5, 0, -6},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, -2, 2, 0, 2, 2, 0, 2, 0, 0, -1, -3, 0, -7},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, -2, -1, 2, 1},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static const int rgb_offset_revC[LUMINANCE_MAX][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-7, 0, -3, -6, 0, -4, -4, 0, -3, -10, 0, -8, -9, 0, -8, -6, 0, -5, -7, 0, -7, -6, 0, -3, 10, 0, 16},
	{-3, 0, -2, -5, 0, -3, -4, 0, -2, -8, 0, -8, -9, 0, -9, -10, -1, -9, -6, 0, -8, -6, 0, -5, 10, 0, 16},
	{-1, 0, -1, -3, 0, -2, -5, 0, -3, -10, 0, -8, -9, 0, -9, -7, 0, -8, -6, -2, -8, -4, -1, -5, 10, 0, 16},
	{0, 0, -1, -3, 0, -2, -4, 0, -2, -9, 0, -6, -8, 0, -10, -10, 0, -8, -7, -2, -9, -1, 0, -5, 10, 0, 10},
	{1, 0, 0, -2, 0, -1, -3, 0, -2, -8, 0, -6, -7, -1, -10, -9, -1, -9, -8, -2, -9, -2, -3, -8, 10, 0, 5},
	{1, 0, 0, -2, 0, -1, -3, 0, -2, -7, 0, -6, -8, -1, -9, -8, 0, -9, -6, -1, -8, -7, -4, -9, 3, 0, 2},
	{1, 0, 1, -1, 0, -1, -2, 0, -1, -7, 0, -6, -8, 0, -9, -10, -2, -10, -7, -2, -9, -7, -4, -10, 1, 0, 0},
	{1, 0, 1, -1, 0, -1, -2, 0, -1, -6, 0, -5, -7, 0, -9, -9, -1, -9, -5, -1, -9, -6, -4, -10, 1, 0, -1},
	{1, 0, 1, -1, 0, -1, -2, 0, -1, -5, 0, -4, -7, -1, -9, -9, -2, -10, -7, -1, -10, -7, -4, -10, 1, 0, -1},
	{2, 0, 2, -1, 0, -1, -2, 0, -1, -5, 0, -3, -5, 0, -9, -8, -1, -9, -10, -5, -12, -8, -5, -13, 0, 0, -1},
	{2, 0, 2, -1, 0, -1, -2, 0, -1, -5, 0, -3, -5, 0, -9, -7, -2, -9, -10, -4, -12, -9, -5, -14, 0, 0, -1},
	{3, 0, 3, 0, 0, -1, -2, 0, -1, -6, 0, -3, -3, 0, -8, -8, -2, -10, -9, -4, -12, -9, -5, -14, 4, 0, -3},
	{4, 0, 4, 0, 0, -1, -2, 0, -1, -6, 0, -3, -3, 0, -8, -6, -1, -9, -9, -4, -12, -8, -5, -15, 0, 0, 0},
	{4, 0, 5, 1, 0, 0, -1, 0, -1, -5, 0, -4, -5, 0, -8, -6, -1, -10, -9, -4, -11, -5, -2, -15, 0, 0, -1},
	{4, 0, 5, 1, 0, 0, -1, 0, 0, -5, 0, -4, -4, 0, -8, -6, 0, -10, -7, -4, -11, -8, -2, -14, 2, 0, -2},
	{4, 0, 5, 1, 0, 0, -1, 0, 0, -3, 0, -4, -5, 0, -7, -6, -1, -8, -6, -2, -11, -5, 0, -13, 2, 0, -3},
	{4, 0, 5, 2, 0, 0, -1, 0, 0, -3, 0, -3, -4, -2, -8, -8, 0, -8, -8, -2, -14, -5, -1, -13, 0, 0, -5},
	{2, 0, 3, 2, 0, 0, -1, 0, 0, -3, 0, -3, -3, -1, -6, -6, 0, -8, -6, -2, -11, -7, -1, -15, 0, 0, -2},
	{4, 0, 5, 2, 0, 0, -1, 0, -1, -3, 0, -2, -4, 0, -7, -5, -1, -7, -7, -3, -12, -9, -2, -16, 2, 0, -5},
	{4, 0, 5, 2, 0, 0, -1, 0, -1, -3, 0, -2, -4, 0, -6, -5, -1, -7, -8, -4, -12, -7, 0, -16, 0, 0, -1},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -2, 0, -2, -3, 0, -7, -7, -2, -7, -9, -4, -12, -9, -3, -18, 0, 0, -6},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -2, 0, -2, -3, 0, -7, -5, 0, -6, -8, -3, -10, -7, -1, -18, 2, 0, -5},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -2, 0, -2, -3, 0, -7, -5, -1, -5, -10, -6, -13, -9, -3, -18, 2, 0, -6},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, -2, -2, 0, -6, -5, 0, -6, -9, -5, -11, -7, -3, -18, 1, 0, -6},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, -2, -2, 0, -5, -5, -2, -6, -9, -5, -11, -10, -2, -17, 2, 0, -8},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, -2, -2, 0, -6, -3, 0, -5, -8, -4, -11, -9, -3, -17, 1, 0, -6},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, -2, -2, 0, -6, -4, -2, -5, -8, -5, -11, -9, -4, -17, -4, -1, -9},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, -1, -1, 0, -6, -5, 0, -5, -7, -5, -10, -9, -3, -18, 1, -5, -4},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, 0, -2, 0, -6, -1, 0, -4, -9, -7, -10, -11, -5, -17, 1, 0, -4},
	{4, 0, 5, 2, 0, 1, -1, 0, 0, -1, 0, 0, -1, 0, -6, -2, 0, -3, -6, -2, -8, -11, -4, -18, 1, 0, -4},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -2, 0, -5, -5, 0, -3, -4, 0, -7, -10, -3, -17, 0, 0, -6},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -4, -3, -1, -3, -6, -1, -7, -14, -5, -21, 3, 0, -7},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -4, -5, 0, -2, -8, -3, -10, -10, 0, -16, -1, 0, -7},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -4, -5, 0, -2, -8, -4, -9, -9, -2, -14, -1, 0, -7},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -4, -4, 0, -2, -7, -3, -8, -8, 0, -14, -1, 0, -8},
	{4, 0, 5, 2, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -3, -3, 0, -1, -5, 0, -8, -10, 0, -17, -1, 0, -9},
	{3, 0, 5, 2, 0, 0, 1, 0, 1, 0, 0, 1, -2, 0, -4, -4, 0, -1, -3, 0, -7, -10, 0, -15, -1, -3, -10},
	{3, 0, 6, 2, 0, 1, 1, 0, 0, 0, 0, 0, -2, 0, -4, -4, 0, -1, -3, -2, -7, -10, 0, -17, -1, -3, -11},
	{3, 0, 5, 2, 0, 0, 0, 0, 0, -1, 0, 0, -2, 0, -3, -2, 0, 0, -3, -1, -6, -10, -1, -16, -2, -5, -15},
	{2, 0, 4, 2, 0, 1, 1, 0, 1, -1, 0, 0, -2, 0, -4, -2, 0, -1, -5, -3, -7, -13, -2, -15, -3, -5, -15},
	{2, 0, 4, 2, 0, 1, 1, 0, 1, -1, 0, 0, -1, 0, -4, -1, 0, -1, -5, -3, -5, -12, 0, -16, 0, -3, -13},
	{2, 0, 4, 3, 0, 1, 0, 0, 1, -1, 0, 0, -1, 0, -3, -1, 0, 0, -2, 0, -4, -11, 0, -14, -2, -4, -15},
	{2, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 1, -1, 0, -3, -2, 0, -1, -6, -4, -7, -11, -1, -13, 0, -1, -15},
	{1, 0, 4, 3, 0, 1, 0, 0, 0, 0, 0, 2, -1, 0, -3, -2, 0, -1, -5, 0, -6, -8, 0, -11, -5, -5, -15},
	{2, 0, 4, 3, 0, 2, 0, 0, 0, 0, 0, 1, -1, 0, -3, -2, 0, -1, -6, 0, -6, -8, 0, -10, -2, -3, -16},
	{1, 0, 3, 3, 0, 2, 0, 0, 0, 1, 0, 1, 0, 0, -2, -1, 0, 0, -6, 0, -5, -8, 0, -10, -5, -3, -17},
	{1, 0, 3, 2, 0, 2, 0, 0, 1, 0, 0, 0, -1, 0, -3, -1, 0, 0, -4, -1, -5, -6, -1, -11, -2, -1, -14},
	{1, 0, 3, 2, 0, 2, 1, 0, 1, 0, 0, 1, 0, 0, -2, -1, 0, 0, -5, 0, -5, -8, -1, -12, -3, -2, -16},
	{0, 0, 2, 2, 0, 2, 1, 0, 1, 0, 0, 1, -1, 0, -1, -1, 0, 1, -4, 0, -5, -6, 0, -10, -3, -2, -15},
	{0, 0, 2, 2, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, -2, -1, 0, 1, -3, -2, -3, -8, 0, -9, -5, -1, -13},
	{0, 0, 2, 2, 0, 2, 1, 0, 0, -1, 0, 0, 0, 0, -1, -2, 0, 1, -3, -1, -5, -7, 0, -13, -3, -1, -12},
	{0, 0, 1, 1, 0, 2, 1, 0, 1, -1, 0, 0, 0, 0, -1, 0, 0, 1, -4, 0, -2, -9, -4, -12, -7, -3, -14},
	{0, 0, 1, 1, 0, 2, 2, 0, 2, 0, 0, 1, 1, 0, -1, -1, 0, -1, -5, 0, -4, -8, -3, -8, -12, 0, -13},
	{1, 0, 2, 1, 0, 1, 2, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, 0, -4, 0, -3, -6, -2, -5, -9, 0, -15},
	{0, 0, 0, 1, 0, 1, 2, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, 0, -4, -3, -3, -8, -2, -8, -9, 0, -14},
	{0, 0, 0, 1, 0, 1, 2, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, 0, -2, 0, 1, -7, 0, -7, -8, 0, -12},
	{0, 0, 0, 1, 0, 1, 2, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 1, -2, 0, -2, -11, -2, -9, -5, 0, -12},
	{0, 0, 0, 1, 0, 1, 2, 0, 2, 0, 0, 0, -1, 0, -1, 1, 0, 0, -1, -3, 2, -12, 0, -9, -4, 0, -12},
	{0, 0, 0, 1, 0, 1, 2, 0, 2, 0, 0, 0, -2, 0, -1, 0, 0, 0, -1, -2, -1, -11, -4, -7, -7, 0, -8},
	{0, 0, 1, 1, 0, 2, 1, 0, 1, 1, 0, 1, 0, 0, 0, 2, 0, 1, 0, 0, -2, -10, 0, -8, -5, 0, -8},
	{0, 0, 1, 0, 0, 2, 1, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0, 1, -1, 0, -2, -11, 0, -9, -6, 0, -7},
	{0, 0, 0, 0, 0, 1, 1, 0, 1, 2, 0, 1, -1, 0, -1, 0, 0, 1, -1, 0, -2, -8, 0, -7, -7, 0, -6},
	{0, 0, 0, 0, 0, 1, 1, 0, 1, 2, 0, 0, -1, 0, -1, 0, 0, 1, 0, 0, -1, -6, 0, -6, -5, 0, -6},
	{0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 0, 0, 1, 1, 0, -2, -7, 0, -5, -6, 0, -6},
	{0, 0, 0, 0, 0, 0, 1, 0, 2, 1, 0, 1, -1, 0, -1, -1, 0, 0, -1, 0, -1, -5, 0, -5, -5, 0, -5},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, 0, -1, -1, 0, 0, -1, 0, 0, -4, 0, -3, -4, 0, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, 0, -1, 0, 0, 0, -1, 0, -1, -5, 0, -3, -4, 0, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, 0, -1, 0, 0, 0, -1, 0, -1, -5, 0, -3, -4, 0, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, -2, 0, -1, -2, 0, -1, -3, 0, -4, -3, 0, -2},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, -2, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/************
 *	  HMT   *
 ************/
static unsigned int base_luminance_reverse_hmt_single[][2] = {
	{10, 49},
	{11, 54},
	{12, 60},
	{13, 65},
	{14, 69},
	{15, 74},
	{16, 77},
	{17, 83},
	{19, 92},
	{20, 97},
	{21, 101},
	{22, 105},
	{23, 109},
	{25, 117},
	{27, 126},
	{29, 133},
	{31, 144},
	{33, 152},
	{35, 162},
	{37, 171},
	{39, 178},
	{41, 188},
	{44, 201},
	{47, 213},
	{50, 227},
	{53, 237},
	{56, 251},
	{60, 266},
	{64, 280},
	{68, 292},
	{72, 308},
	{77, 235},
	{82, 252},
	{87, 262},
	{93, 278},
	{99, 291},
	{105, 304},
};

static int gradation_offset_reverse_hmt_single[][9] = {
	/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 1, 1 , 2, 4, 5, 7, 10, 10},
	{0, 0, 1 , 2, 3, 6, 7, 9, 12},
	{0, 0, 1 , 1, 4, 5, 7, 9, 10},
	{0, 0, 2 , 2, 3, 5, 7, 9, 12},
	{0, -1, 2 , 2, 4, 5, 6, 9, 10},
	{0, -1, 2 , 2, 4, 5, 7, 9, 10},
	{0, 0, 1 , 2, 4, 5, 7, 9, 12},
	{0, 0, 1 , 1, 4, 4, 6, 8, 9},
	{0, 1, 1 , 1, 3, 5, 7, 9, 10},
	{0, 1, 2 , 1, 3, 4, 7, 9, 10},
	{0, 0, 1 , 1, 3, 4, 6, 9, 10},
	{0, 2, 2 , 2, 4, 5, 6, 9, 10},
	{0, 1, 1 , 2, 3, 4, 6, 9, 10},
	{0, 2, 2 , 1, 3, 5, 6, 8, 10},
	{0, 3, 3 , 2, 3, 5, 6, 8, 12},
	{0, 3, 2 , 2, 3, 4, 6, 8, 10},
	{0, 3, 3 , 3, 3, 4, 6, 8, 9},
	{0, 2, 4 , 3, 4, 4, 6, 9, 9},
	{0, 2, 2 , 2, 3, 4, 5, 8, 9},
	{0, 1, 3 , 3, 3, 4, 5, 8, 9},
	{0, 2, 4 , 4, 3, 4, 6, 8, 9},
	{0, 1, 3 , 3, 3, 4, 5, 8, 10},
	{0, 1, 4 , 3, 3, 4, 6, 8, 10},
	{0, 1, 4 , 4, 3, 4, 5, 7, 9},
	{0, 2, 4 , 4, 3, 3, 5, 8, 9},
	{0, 2, 4 , 4, 3, 4, 5, 8, 9},
	{0, 3, 5 , 4, 3, 4, 5, 8, 9},
	{0, 2, 5 , 4, 3, 3, 4, 7, 9},
	{0, 4, 6 , 5, 4, 5, 5, 7, 9},
	{0, 5, 6 , 4, 4, 4, 4, 7, 9},
	{0, 5, 6 , 5, 4, 4, 5, 8, 9},
	{0, 1, 4 , 2, 1, 2, 2, 4, 5},
	{0, 1, 3 , 2, 1, 2, 3, 4, 5},
	{0, 2, 5 , 2, 1, 1, 2, 4, 5},
	{0, 3, 4 , 3, 2, 2, 2, 4, 6},
	{0, 4, 6 , 4, 2, 2, 2, 4, 6},
	{0, 5, 6 , 4, 2, 2, 2, 4, 5},
};

static int rgb_offset_reverse_hmt_single[][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{0, 0, 0, 0, 0, 0, -1, 0, -1, -2, 2, -4, -7, 3, -7, -8, 4, -8, -10, 4, -9, -7, 3, -7, -2, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 2, -5, -7, 3, -7, -6, 2, -6, -9, 3, -8, -11, 5, -12, -1, 0, -1},
	{0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 1, -4, -5, 3, -6, -9, 3, -8, -8, 4, -8, -9, 5, -11, -3, 1, -3},
	{-1, 0, 0, 0, 0, 0, -1, 0, 0, -1, 1, -3, -6, 3, -7, -6, 3, -6, -10, 4, -10, -11, 5, -11, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 1, -3, -6, 3, -7, -7, 3, -7, -11, 4, -10, -10, 5, -12, -1, 1, -2},
	{0, 0, 0, -1, 0, 0, 0, 0, -1, -1, 1, -3, -6, 2, -6, -6, 3, -6, -8, 4, -9, -12, 6, -12, -1, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -3, -6, 3, -6, -6, 2, -6, -9, 4, -9, -13, 6, -13, -1, 0, -1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -3, -6, 2, -6, -6, 3, -6, -9, 4, -9, -13, 6, -13, -1, 1, -3},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -3, -5, 2, -5, -6, 2, -6, -9, 4, -8, -13, 6, -13, -3, 1, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -2, -5, 2, -6, -6, 2, -6, -8, 4, -8, -13, 6, -12, -3, 1, -4},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, -1, 1, -3, -6, 2, -5, -7, 2, -6, -8, 4, -8, -12, 6, -12, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -4, 3, -6, -7, 2, -6, -9, 4, -9, -11, 6, -12, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, -1, -1, 0, -2, -3, 2, -6, -7, 3, -6, -8, 4, -8, -13, 6, -13, -4, 1, -4},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -4, 3, -6, -5, 2, -5, -9, 3, -8, -14, 6, -14, -4, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -4, 3, -6, -6, 1, -4, -8, 4, -8, -16, 6, -14, 0, 0, -1},
	{0, 0, 1, 0, 0, 0, -1, 0, -1, 0, 0, 0, -3, 3, -6, -5, 1, -4, -9, 4, -8, -14, 7, -14, -3, 1, -3},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 2, -5, -4, 2, -4, -8, 3, -7, -16, 7, -15, -5, 1, -4},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, -3, 2, -5, -4, 2, -4, -7, 3, -7, -13, 6, -13, -6, 2, -6},
	{2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -4, 2, -5, -4, 2, -5, -7, 3, -8, -13, 6, -13, -6, 2, -5},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 2, -4, -4, 2, -4, -7, 4, -8, -12, 6, -12, -6, 2, -5},
	{0, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 2, -5, -3, 2, -4, -7, 3, -6, -14, 7, -14, -7, 2, -6},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, -2, 1, -4, -3, 2, -4, -5, 3, -6, -16, 7, -15, -4, 1, -4},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, -1, -2, 1, -3, -3, 2, -4, -6, 2, -6, -17, 7, -16, -4, 1, -4},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 1, -3, -3, 1, -4, -6, 3, -6, -16, 7, -16, -5, 1, -4},
	{0, 0, 0, 1, 0, 0, 0, 0, 0, -2, 0, 0, 0, 1, -2, -2, 1, -4, -7, 3, -7, -15, 6, -14, -4, 2, -6},
	{1, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 1, -2, -2, 2, -4, -7, 2, -6, -13, 7, -14, -6, 2, -6},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 1, -2, -2, 2, -4, -6, 3, -6, -13, 6, -14, -6, 2, -6},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -2, -2, 2, -4, -6, 3, -6, -13, 6, -14, -5, 2, -6},
	{1, 0, 2, 0, 0, 0, -1, 0, -1, -1, 0, 0, -2, 1, -2, -2, 2, -4, -6, 2, -6, -15, 7, -14, -4, 2, -5},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0, -1, -1, 2, -4, -7, 2, -6, -14, 6, -14, -5, 2, -6},
	{1, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, -2, 2, -4, -6, 2, -6, -12, 6, -12, -7, 2, -6},
	{0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -2, -1, 1, -2, -3, 2, -4, -11, 6, -14, -6, 3, -6},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -2, 0, 1, -2, -2, 1, -3, -12, 7, -15, -5, 3, -6},
	{0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -2, -1, 1, -2, -3, 1, -3, -10, 6, -13, -6, 4, -8},
	{1, 0, 2, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, -1, -1, 1, -3, -2, 1, -3, -10, 6, -13, -5, 2, -6},
	{1, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0, -1, -2, 2, -4, -2, 1, -2, -10, 6, -12, -4, 2, -6},
	{1, 0, 2, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, 0, -1, -2, 2, -4, -2, 1, -2, -10, 6, -12, -4, 2, -6},
};

#endif
