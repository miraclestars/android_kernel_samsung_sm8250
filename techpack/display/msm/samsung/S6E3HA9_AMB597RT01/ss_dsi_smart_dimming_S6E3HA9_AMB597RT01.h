/*
 * =================================================================
 *
 *       Filename:  smart_mtp_s6e3.h
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
 */

#ifndef _SS_DSI_SMART_DIMMING_S6E3HA9_AMB597RT01_H_
#define _SS_DSI_SMART_DIMMING_S6E3HA9_AMB597RT01_H_

#include "ss_dsi_panel_common.h"
#include "ss_dsi_smart_dimming_common.h"

static int id1, id2, id3;

#define LUMINANCE_MAX 74
#define HMT_LUMINANCE_MAX 37
#define GAMMA_SET_MAX 34
#define GRAY_SCALE_MAX 256
#define RGB_COMPENSATION 27

/* BIT_SHIFT is used for right bit shfit */
#define BIT_SHIFT 22
#define BIT_SHFIT_MUL 4194304 // pow(2,BIT_SHIFT)

#define VREG1_REF_6P8 28521267	/* 6.8*4194304 */
#define VREG1_REF_6P5 27262976	/* 6.5*4194304 */
#define VREG1_REF_6P4 26843546	/* 6.4*4194304 */
#define VREG1_REF_6P3 26424115	/* 6.3*4194304 */
#define VREG1_REF_6P2 26004685	/* 6.2*4194304 */

#define VREF_1P0 4194304 /* 1.0*4194304 */

#define HBM_INTERPOLATION_STEP 60

enum {
	GAMMA_CURVE_1P9 = 0,
	GAMMA_CURVE_2P15,
	GAMMA_CURVE_2P2,
	GAMMA_CURVE_2P2_400CD,
};

/* TP */
enum {
	VT = 0,
	V0,
	V1,
	V7,
	V11,
	V23,
	V35,
	V51,
	V87,
	V151,
	V203,
	V255,
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
	"V0",
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
	int vref;
	char panel_revision;

	/* HBM interpolation */
	struct illuminance_table hbm_interpolation_table[HBM_INTERPOLATION_STEP + 1];
	char *hbm_payload;
	int hbm_brightness_level;

	struct illuminance_table hmt_gen_table[HMT_LUMINANCE_MAX];
} __packed;

/* V0,V1,V7,V11,V23,V35,V51,V87,V151,V203,V255 */
static int INFLECTION_VOLTAGE_ARRAY[V_MAX] = {0, 0, 1, 7, 11, 23, 35, 51, 87, 151, 203, 255};

static int vt_coefficient[] = {
	0, 24, 48, 72,
	96, 120, 144, 168,
	192, 216, 276, 296,
	316, 336, 356, 372
};

static int v0_coefficient[] = {
	0, 12, 24, 36,
	48, 60, 72, 84,
	96, 108, 120, 132,
	144, 156, 168, 180
};

/* center (max) gamma value (Hex) */
static int center_gamma[V_MAX][RGB_MAX] = {
	{0x0, 0x0, 0x0},	/* VT */
	{0x3, 0x3, 0x3},	/* V0 */
	{0x80, 0x80, 0x80},	/* V1 */
	{0x80, 0x80, 0x80},	/* V7 */
	{0x80, 0x80, 0x80},	/* V11 */
	{0x80, 0x80, 0x80},	/* V23 */
	{0x80, 0x80, 0x80},	/* V35 */
	{0x80, 0x80, 0x80},	/* V51 */
	{0x80, 0x80, 0x80},	/* V87 */
	{0x80, 0x80, 0x80},	/* V151 */
	{0x80, 0x80, 0x80},	/* V203 */
	{0x200, 0x200, 0x200},	/* V255 */
};

/* fraction for gamma code */
static int fraction[V_MAX][2] = {
/* {numerator, denominator} */
	{0,  1200},		/* VT */
	{0,  1200},		/* V0 */
	{0,  256},		/* V1 */
	{64, 320},		/* V7 */
	{64, 320},		/* V11 */
	{64, 320},		/* V23 */
	{64, 320},		/* V35 */
	{64, 320},		/* V51 */
	{64, 320},		/* V87 */
	{64, 320},		/* V151 */
	{64, 320},		/* V203 */
	{177, 1200},		/* V255 */
};

static int hbm_interpolation_candela_table_revA[HBM_INTERPOLATION_STEP] = {
	405, 409, 416, 420, 425, 430, 434, 441, 445, 450,
	455, 459, 466, 470, 475, 480, 484, 491, 495, 500,
	505, 509, 516, 520, 525, 530, 534, 541, 545, 550,
	555, 559, 566, 570, 575, 580, 584, 591, 595, 600,
	605, 610, 615, 620, 625, 630, 634, 641, 645, 650,
	655, 659, 666, 670, 675, 680, 684, 691, 695, 700,
};

/* Beyond1 rev */
static unsigned int base_luminance_star_revA[LUMINANCE_MAX][2] = {
	{2, 103},
	{3, 103},
	{4, 103},
	{5, 103},
	{6, 103},
	{7, 103},
	{8, 103},
	{9, 103},
	{10, 103},
	{11, 103},
	{12, 103},
	{13, 103},
	{14, 103},
	{15, 103},
	{16, 103},
	{17, 103},
	{19, 103},
	{20, 103},
	{21, 103},
	{22, 103},
	{24, 103},
	{25, 103},
	{27, 103},
	{29, 103},
	{30, 103},
	{32, 103},
	{34, 103},
	{37, 103},
	{39, 103},
	{41, 103},
	{44, 103},
	{47, 103},
	{50, 103},
	{53, 103},
	{56, 103},
	{60, 109},
	{64, 114},
	{68, 120},
	{72, 126},
	{77, 134},
	{82, 143},
	{87, 151},
	{93, 160},
	{98, 172},
	{105, 183},
	{111, 196},
	{119, 205},
	{126, 215},
	{134, 232},
	{143, 242},
	{152, 247},
	{162, 266},
	{172, 278},
	{183, 295},
	{195, 295},
	{207, 295},
	{220, 295},
	{234, 295},
	{249, 297},
	{265, 306},
	{282, 318},
	{300, 336},
	{316, 353},
	{333, 367},
	{350, 385},
	{357, 395},
	{365, 403},
	{372, 403},
	{380, 403},
	{387, 403},
	{395, 403},
	{403, 407},
	{412, 412},
	{420, 420},
};

static int gradation_offset_star_revA[LUMINANCE_MAX][9] = {
/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 2, 3, 7, 12, 15, 17, 24, 25},
	{0, 4, 7, 14, 19, 22, 26, 30, 31},
	{0, 5, 7, 13, 17, 19, 23, 27, 28},
	{0, 4, 5, 11, 14, 16, 19, 23, 25},
	{0, 4, 5, 9, 14, 15, 18, 22, 24},
	{0, 3, 4, 8, 12, 14, 17, 21, 22},
	{0, 2, 4, 8, 12, 13, 16, 20, 22},
	{0, 3, 4, 7, 11, 13, 16, 19, 21},
	{0, 3, 4, 7, 11, 12, 15, 19, 20},
	{0, 3, 4, 7, 11, 12, 15, 19, 20},
	{0, 3, 4, 7, 11, 12, 15, 19, 20},
	{0, 3, 4, 7, 11, 12, 15, 19, 21},
	{0, 3, 5, 7, 11, 13, 16, 19, 21},
	{0, 4, 4, 7, 11, 12, 15, 19, 21},
	{0, 3, 4, 7, 10, 12, 15, 18, 20},
	{0, 3, 4, 7, 10, 11, 14, 17, 20},
	{0, 2, 4, 6, 9, 11, 14, 17, 19},
	{0, 1, 0, 2, 5, 6, 7, 11, 12},
	{0, 2, 3, 6, 8, 10, 12, 16, 18},
	{0, 2, 3, 5, 8, 9, 12, 15, 17},
	{0, 2, 3, 5, 7, 8, 11, 14, 16},
	{0, 2, 3, 5, 7, 8, 10, 14, 16},
	{0, 2, 3, 5, 7, 8, 10, 13, 15},
	{0, 2, 3, 4, 6, 7, 9, 13, 15},
	{0, 2, 3, 4, 6, 7, 9, 12, 14},
	{0, 2, 3, 3, 6, 6, 8, 11, 13},
	{0, 2, 3, 3, 5, 6, 8, 11, 13},
	{0, 2, 2, 3, 5, 5, 7, 10, 12},
	{0, 2, 2, 3, 5, 5, 7, 10, 12},
	{0, 2, 2, 3, 5, 5, 7, 9, 11},
	{0, 2, 2, 3, 4, 4, 6, 9, 10},
	{0, 3, 2, 2, 4, 4, 5, 8, 10},
	{0, 1, 1, 2, 4, 3, 5, 7, 9},
	{0, 1, 1, 1, 3, 3, 5, 7, 8},
	{0, 1, 1, 1, 3, 2, 4, 6, 8},
	{0, 2, 1, 1, 2, 2, 4, 6, 8},
	{0, 2, 2, 2, 3, 3, 4, 5, 8},
	{0, 2, 1, 2, 3, 3, 4, 5, 8},
	{0, 2, 2, 3, 3, 3, 4, 5, 8},
	{0, 2, 3, 3, 2, 3, 4, 5, 8},
	{0, 2, 3, 2, 3, 3, 4, 5, 7},
	{0, 2, 2, 2, 2, 3, 4, 6, 7},
	{0, 3, 3, 4, 3, 3, 3, 5, 7},
	{0, 2, 1, 1, 1, 1, 0, 1, 3},
	{0, 2, 3, 3, 3, 3, 3, 5, 7},
	{0, 0, 2, 3, 3, 2, 3, 5, 7},
	{0, 3, 3, 3, 2, 3, 4, 5, 7},
	{0, 1, 3, 3, 3, 3, 3, 4, 7},
	{0, -2, 2, 3, 2, 2, 2, 4, 6},
	{0, -3, 3, 2, 2, 2, 3, 5, 6},
	{0, 1, 4, 4, 2, 2, 3, 4, 6},
	{0, 1, 3, 3, 2, 2, 2, 3, 6},
	{0, 1, 5, 3, 2, 3, 3, 3, 6},
	{0, 1, 2, 2, 1, 1, 0, 0, 3},
	{0, 3, 4, 3, 2, 2, 3, 3, 5},
	{0, 1, 3, 3, 1, 1, 2, 2, 4},
	{0, 1, 3, 2, 1, 1, 2, 2, 3},
	{0, 0, 2, 2, 1, 1, 1, 1, 3},
	{0, 0, 1, 1, 1, 1, 0, 1, 2},
	{0, -1, 1, 1, 1, 0, 1, 1, 1},
	{0, 0, 1, 1, 0, 0, 0, 0, 1},
	{0, 2, 3, 1, 1, 1, 0, 0, 1},
	{0, 2, 2, 1, 0, 0, 1, 0, 1},
	{0, 1, 1, 0, 0, 0, 0, 0, 1},
	{0, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 0, 1, -1, -1, 0, 1},
	{0, 1, 0, 0, -1, -1, -1, -1, 0},
	{0, 1, 0, 0, -1, -1, -1, -1, 0},
	{0, 2, 0, 0, -1, -1, -1, -1, 0},
	{0, 3, -1, -1, -2, -1, -1, -2, 0},
	{0, -1, -1, -1, -1, -1, -1, -1, 1},
	{0, -1, -1, -1, -1, -1, -1, -1, 0},
	{0, -1, -1, -2, -1, -1, -1, -1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static int rgb_offset_star_revA[LUMINANCE_MAX][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{-14, -4, -8, -3, 0, -1, -4, 0, -1, -20, 0, -3, -56, 0, -4, -95, -1, -9, -105, -1, -9, -120, 0, -13, -120, 1, -15},
	{-2, 0, -2, -3, 0, -2, -2, 0, -2, -5, 1, -8, -8, 0, -10, -11, 0, -11, -11, -1, -12, -12, 0, -13, 2, 2, -5},
	{-2, 0, -2, -2, 0, -1, -2, 0, -2, -5, 0, -8, -7, 1, -9, -8, 0, -10, -10, -2, -13, -9, -1, -15, 2, 3, 2},
	{-2, 0, -2, -1, 0, 0, -2, 0, -2, -6, -1, -8, -6, 1, -8, -8, -1, -11, -8, 3, -9, -10, -2, -14, -1, -1, -1},
	{-1, 0, -1, -2, 0, 0, -2, -1, -3, -3, 1, -5, -9, -2, -11, -5, 1, -9, -8, 3, -9, -9, -2, -16, -1, -1, -10},
	{-1, 0, -1, -2, 0, 0, 0, 0, -1, -4, 0, -7, -6, 1, -9, -8, -2, -11, -7, 2, -10, -9, -2, -17, 3, 3, -4},
	{-1, 0, -1, -1, 0, 1, -1, 0, -2, -4, 0, -6, -6, 0, -9, -4, 1, -9, -7, 1, -11, -8, -3, -16, -1, -1, -1},
	{0, 0, 0, -2, 0, 0, -1, 0, -1, -3, 0, -5, -6, 0, -9, -7, -2, -12, -6, 0, -11, -4, 2, -13, -1, -1, -9},
	{0, 0, 0, -2, 0, 0, 0, 0, -1, -3, 0, -4, -7, -1, -10, -4, 1, -9, -6, 0, -10, -7, -2, -17, 3, 3, -7},
	{1, 0, 1, -2, 0, 0, 0, 0, -1, -4, 0, -5, -6, -1, -10, -2, 2, -7, -6, -1, -12, -5, -2, -17, 3, 3, -8},
	{1, 0, 1, -1, 0, 0, -1, 0, -1, -3, 0, -4, -6, -1, -10, -3, 1, -9, -4, 0, -11, -4, -2, -17, 3, 3, -6},
	{1, 0, 1, -1, 0, 0, -1, 0, -1, -3, 0, -4, -5, -1, -10, -2, 2, -7, -4, 0, -12, -3, -2, -17, -1, -1, -7},
	{2, 0, 2, -1, 0, 0, -2, -1, -2, -1, 1, -4, -5, 0, -9, -6, -2, -12, -3, 0, -12, 1, 2, -15, -1, -1, -10},
	{2, 0, 2, -1, 0, 0, -1, 0, -1, -2, 0, -4, -5, 0, -9, -1, 1, -8, -3, 0, -12, -2, -1, -16, -1, -1, -1},
	{2, 0, 2, 0, 0, 1, -1, 0, -1, -4, -1, -5, -2, 2, -7, -5, -2, -11, -4, -1, -13, 1, 3, -13, -1, -1, -12},
	{2, 0, 2, -1, 0, 0, -1, 0, 0, -3, -1, -5, -3, 1, -7, -2, 1, -8, -4, -1, -14, 0, 2, -12, -5, -2, -14},
	{2, 0, 2, 0, 0, 1, -2, 0, -1, -2, 0, -5, -2, 1, -6, -5, -3, -11, -5, 0, -13, -1, 1, -14, -1, -1, -11},
	{1, 0, 2, 1, 0, 2, -1, 0, -1, -3, 0, -3, -6, 0, -4, -8, -2, -5, -23, 0, -9, -13, -3, -15, -15, 0, -15},
	{2, 0, 2, 0, 0, 1, -2, 0, -1, -2, -1, -5, -2, 2, -6, -5, -2, -9, -2, 3, -11, -5, -4, -20, -1, -1, -10},
	{2, 0, 2, 0, 0, 1, -2, 0, -1, -2, 0, -3, -2, 0, -7, -2, 2, -6, -6, -3, -15, 0, 2, -13, -1, -1, -1},
	{2, 0, 2, 0, 0, 1, -2, -1, -1, -2, 0, -3, -1, 2, -5, -3, 1, -7, -7, -3, -16, 1, 3, -12, -1, -1, -13},
	{2, 0, 2, 0, 0, 1, -2, -1, -1, -2, 0, -3, -2, 1, -6, -2, 1, -6, -4, 0, -13, -4, -3, -18, -1, -1, -12},
	{2, 0, 2, 0, 0, 1, -2, -1, -1, -2, 0, -3, -3, 0, -6, -4, -1, -8, -5, -1, -13, 2, 3, -12, -1, -1, -1},
	{2, 0, 2, 0, 0, 1, -2, -1, -1, 0, 0, -2, -2, 1, -5, -3, 0, -6, -4, 1, -11, -6, -4, -20, -1, -1, -12},
	{3, 1, 3, -1, 0, 1, -2, -1, -2, 0, 0, -2, -2, 0, -5, -4, -1, -6, -5, -1, -13, -1, 2, -15, -1, -1, -12},
	{3, 1, 3, -1, 0, 1, -2, -1, -2, 0, 1, -1, -4, -2, -6, 0, 3, -4, -5, 0, -11, -1, 1, -15, -1, -1, -12},
	{3, 1, 3, -1, 0, 1, -2, -1, -2, 0, 1, 0, -1, 1, -4, -5, -2, -7, -5, -1, -11, -3, 1, -16, -1, -1, -11},
	{3, 1, 3, -1, 0, 1, -1, 0, -1, -1, 0, -2, 0, 1, -3, -2, 1, -4, -4, 0, -9, -4, 0, -16, -1, -1, -12},
	{3, 1, 3, -1, 0, 1, 0, 0, 0, -2, 0, -3, 0, 0, -3, -3, 0, -5, -4, 0, -9, -7, -2, -19, -1, -1, -12},
	{3, 1, 3, -1, 0, 1, 0, 0, 0, -2, -1, -3, -1, 0, -4, -3, 0, -4, -5, -2, -10, -1, 5, -14, -1, -1, -10},
	{3, 1, 3, -1, 0, 1, 0, 0, 0, -2, -1, -3, 1, 2, -1, -2, 0, -5, -3, -1, -8, -8, -3, -21, 6, 5, -4},
	{3, 1, 3, -1, 0, 1, -1, -1, -2, 0, 1, -1, -1, -1, -3, -3, -1, -6, 2, 5, -3, -9, -4, -21, -1, -1, -10},
	{3, 1, 3, -2, 0, 0, 0, 0, 0, 0, 0, -1, -2, -1, -4, 0, 2, -2, -2, -1, -7, -5, 1, -16, -1, -1, -10},
	{3, 1, 3, -2, 0, 0, 1, 0, 1, 0, 1, -1, 0, 0, -2, -5, -3, -7, -3, -2, -7, -6, 0, -17, 6, 5, -3},
	{3, 1, 3, -2, 0, 0, 1, 0, 1, 0, 1, -1, -1, -2, -3, 2, 3, 0, -4, -1, -7, -7, -1, -20, -2, -2, -10},
	{2, 1, 2, -1, -1, -1, 0, 0, 1, 1, 2, -1, -1, -2, -2, 1, 3, -1, -3, -3, -7, -10, -2, -20, -1, -1, -1},
	{3, 1, 3, -1, -1, -1, -1, 0, 0, 1, 1, 0, 0, -1, -2, -1, 0, -3, 1, 2, -3, -11, -3, -21, -1, -1, -9},
	{2, 0, 2, 1, 1, 1, -1, -1, 0, -1, 0, -2, 0, -1, -1, -2, 0, -3, -2, -1, -6, -5, 3, -14, -1, 0, -8},
	{3, 0, 3, -1, 1, 0, 0, 0, 1, -1, 0, -2, 1, 1, 0, -2, 0, -3, -2, -1, -5, -6, 1, -16, -3, -3, -11},
	{2, 1, 2, 0, 0, 0, -1, -1, -1, 0, 0, -1, 0, 1, -1, -1, -2, -3, 2, 3, -1, -8, 0, -17, -3, -3, -11},
	{3, 0, 2, 0, 0, 0, -1, 0, 0, -1, 0, -2, 0, -1, -2, 1, 2, 0, -4, -3, -7, -3, 4, -13, -4, -3, -12},
	{3, 0, 3, -1, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -2, 0, 1, -2, -1, 0, -4, -10, -3, -18, 2, 2, -6},
	{3, 1, 3, -1, 0, 0, 1, 1, 0, -1, -1, -2, -1, -1, -2, -1, -1, -2, 2, 3, -1, -9, -4, -18, 2, 2, -6},
	{3, 0, 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, -1, -2, 1, 1, -24, 2, -9, -3, 2, -12},
	{3, 0, 3, -2, 0, -1, 1, 1, 1, -1, -1, -1, -1, 0, -2, 1, 0, -2, 0, 1, -2, -6, -1, -15, 0, 1, -8},
	{3, 0, 2, 0, 1, 1, 0, -1, -1, -1, 0, -1, 0, -1, -1, 2, 3, 1, 0, 0, -3, -7, -1, -15, 0, 1, -8},
	{6, 1, 5, 0, 0, 0, 0, 0, 1, -1, 0, -1, 1, 1, 0, -2, 0, -3, -2, -3, -5, -3, 3, -10, -1, 1, -8},
	{4, 1, 4, -1, 0, -1, 0, 0, 0, 0, 1, 0, 0, -1, -2, 0, 0, -2, 1, 2, -1, -5, 1, -12, -8, -5, -14},
	{3, 1, 3, -1, 0, 0, -1, 0, 0, 0, 0, -1, 1, 2, 1, 0, 0, -2, 0, 0, -2, -6, -1, -12, -3, 0, -10},
	{4, 0, 3, -1, 0, 0, -1, 0, 0, 1, 1, 0, 1, 1, 0, -1, -1, -3, -3, -2, -5, -7, -2, -12, 3, 6, -4},
	{5, 1, 4, -2, 0, -1, 0, 0, 0, 0, 0, -1, 1, 1, 1, -1, 1, -1, -2, -3, -4, -3, 2, -9, -4, -1, -10},
	{3, 0, 3, 0, 0, -1, 0, 0, 0, -1, -1, -1, 2, 2, 1, 0, 0, -1, 0, 1, -1, -3, 1, -8, -6, -2, -12},
	{3, 1, 3, 0, 0, 0, -1, 0, 0, 0, 0, 0, 1, 1, 0, -3, -2, -4, -1, -1, -3, 0, 4, -4, -8, -3, -13},
	{3, 0, 4, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 1, -17, -1, -1, -19, 1, -15},
	{3, 1, 4, -1, -1, -1, 0, 0, 1, 0, 1, 0, -1, -1, -2, 0, 1, 0, -2, -3, -4, -3, 1, -6, -1, 3, -7},
	{3, 1, 3, -1, 0, -1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, -1, -1, -1, -4, 0, -6, -1, 1, -6},
	{3, 1, 3, -1, 0, -1, 0, 0, 0, 1, 0, 1, 0, 0, -1, 1, 0, 0, -1, -2, -1, -6, -2, -7, 5, 5, 0},
	{5, 1, 5, -1, 0, -1, 1, 0, 1, -1, -1, -1, 1, 0, 0, 0, -1, -1, 4, 3, 3, -4, -1, -6, -3, -2, -8},
	{3, 0, 3, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, -1, 0, -1, 0, 4, 3, 3, -4, -3, -7, 4, 2, 0},
	{2, 0, 2, -1, 0, -1, 1, 0, 0, 0, 0, 1, 0, 0, -1, 2, 1, 1, -2, -3, -2, 1, 2, -1, 2, 0, -2},
	{2, 1, 2, -1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, -1, -2, -1, 1, 0, 1, 0, 1, -2, 0, -1, -3},
	{2, 1, 2, 0, -1, 0, 0, 0, 0, 0, 1, 1, -1, -1, -2, 1, 0, 0, 3, 3, 3, -1, -1, -3, -1, -2, -3},
	{2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, -1, -3, -2, 3, 4, 2, 0, -3, -3},
	{2, 1, 2, -1, 0, 0, -1, 0, -1, 2, 1, 2, 1, 1, 0, -2, -3, -1, 2, 1, 1, 2, 2, 1, -2, -3, -4},
	{2, 1, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 1, 1, 1, 1, 3, 1, 1, 4, 1, 2},
	{0, 0, 0, 1, 1, 1, -1, -1, 0, 0, 0, 0, -1, -1, -1, 2, 2, 3, 1, 1, 1, 1, 0, 0, -3, -4, -4},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, -1, -1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, -1, 0, 3, 0, 2},
	{0, 0, 0, 0, 0, 0, 1, 1, 1, -1, -1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 2, -1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, -1, -2, -1, 2, -1, 1},
	{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, -2, -3, -2, 1, 0, 1, 6, 3, 5, -5, -6, -6},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, 1, 0, 1, 0, 1, 0, 1, 1, 0, -7, -4, -6},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

/************
 *	  HMT   *
 ************/
static unsigned int base_luminance_reverse_hmt_single[][2] = {
	{10, 40},
	{11, 45},
	{12, 49},
	{13, 53},
	{14, 57},
	{15, 62},
	{16, 66},
	{17, 70},
	{19, 78},
	{20, 82},
	{21, 86},
	{22, 90},
	{23, 94},
	{25, 102},
	{27, 109},
	{29, 117},
	{31, 124},
	{33, 132},
	{35, 140},
	{37, 145},
	{39, 153},
	{41, 161},
	{44, 172},
	{47, 183},
	{50, 194},
	{53, 206},
	{56, 217},
	{60, 231},
	{64, 246},
	{68, 257},
	{72, 271},
	{77, 204},
	{82, 217},
	{87, 230},
	{93, 245},
	{99, 257},
	{105, 272},
};

static int gradation_offset_reverse_hmt_single[][9] = {
/*	V255 V203 V151 V87 V51 V35 V23 V11 V7 */
	{0, 1, 1, 2, 4, 5, 5, 6, 5},
	{0, 0, 1, 1, 3, 5, 5, 6, 6},
	{0, 0, 1, 2, 4, 5, 5, 5, 6},
	{0, 1, 1, 1, 4, 5, 5, 6, 7},
	{0, 0, 1, 1, 3, 5, 6, 6, 7},
	{0, 0, 2, 2, 3, 5, 6, 6, 7},
	{0, 0, 2, 2, 4, 5, 5, 6, 7},
	{0, 0, 1, 1, 3, 4, 5, 6, 7},
	{0, 1, 1, 2, 4, 5, 4, 5, 6},
	{0, 2, 2, 2, 3, 4, 5, 6, 7},
	{0, 1, 1, 2, 4, 5, 5, 6, 7},
	{0, 0, 0, 1, 3, 4, 5, 6, 7},
	{0, 1, 1, 1, 3, 4, 5, 6, 7},
	{0, 2, 1, 1, 3, 4, 5, 6, 7},
	{0, 1, 0, 1, 3, 5, 5, 5, 7},
	{0, 2, 1, 1, 3, 4, 4, 5, 7},
	{0, 3, 2, 2, 3, 5, 5, 6, 8},
	{0, 3, 2, 1, 2, 4, 4, 6, 7},
	{0, 2, 2, 2, 2, 3, 5, 6, 7},
	{0, 2, 3, 2, 2, 4, 5, 6, 7},
	{0, 3, 3, 2, 2, 3, 4, 5, 7},
	{0, 3, 3, 3, 3, 4, 4, 5, 7},
	{0, 3, 5, 3, 3, 4, 4, 6, 7},
	{0, 3, 4, 3, 3, 4, 4, 6, 7},
	{0, 2, 4, 3, 3, 4, 5, 6, 7},
	{0, 3, 5, 3, 3, 4, 4, 5, 7},
	{0, 2, 4, 3, 2, 3, 4, 5, 6},
	{0, 3, 5, 4, 3, 3, 4, 6, 7},
	{0, 3, 5, 4, 3, 4, 4, 6, 7},
	{0, 4, 6, 4, 3, 4, 4, 5, 7},
	{0, 5, 7, 4, 3, 3, 5, 5, 7},
	{0, 2, 2, 1, 1, 1, 2, 3, 3},
	{0, 1, 2, 0, 0, 1, 2, 3, 3},
	{0, 2, 3, 2, 1, 1, 1, 3, 3},
	{0, 2, 3, 2, 1, 2, 2, 3, 4},
	{0, 3, 4, 2, 1, 1, 1, 2, 4},
	{0, 5, 5, 2, 2, 1, 2, 3, 4},
};

static int rgb_offset_reverse_hmt_single[][RGB_COMPENSATION] = {
/*	R255 G255 B255 R203 G203 B203 R151 G151 B151
	R87 G87 B87 R51 G51 B51 R35 G35 B35
	R23 G23 B23 R11 G11 B11
*/
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 1, -3, -5, 2, -6, -4, 3, -8, -2, 2, -5, 6, 3, -8, 16, -8, 17},
	{-1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, -2, -5, 3, -6, 0, 2, -4, -3, 3, -8, 6, 4, -8, 3, -1, 3},
	{0, 0, 0, 0, 0, 0, 0, 0, -1, -3, 1, -3, -4, 2, -5, -2, 3, -6, -3, 2, -6, 5, 3, -7, 5, 0, 0},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -2, 1, -2, -5, 2, -6, -3, 2, -6, -3, 2, -6, 2, 4, -8, 5, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -2, -5, 2, -6, -4, 2, -6, -2, 2, -5, 2, 4, -9, 7, 0, 1},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, -2, -4, 2, -5, -4, 2, -6, -2, 2, -6, 3, 4, -9, 6, 0, 0},
	{0, 0, 0, 0, 0, 0, -1, 0, 0, -2, 0, -2, -4, 2, -4, -3, 3, -6, -2, 2, -6, 0, 4, -10, 1, 0, 1},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -2, -3, 2, -4, -4, 2, -6, -2, 3, -6, 1, 4, -10, 6, 0, 1},
	{0, 0, 1, 0, 0, 0, 0, 0, -1, -2, 0, -2, -4, 2, -4, -2, 2, -5, -2, 2, -6, -1, 4, -9, 2, 1, -4},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -1, -4, 2, -4, -3, 2, -5, -2, 2, -6, -2, 4, -10, 5, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, -4, 1, -4, -4, 2, -6, -1, 3, -6, 0, 4, -9, 1, 1, -2},
	{0, 0, 1, 1, 0, 0, 0, 0, 0, -1, 0, -2, -4, 1, -4, -4, 2, -5, -1, 3, -6, 0, 4, -8, 2, 1, -2},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, -4, 1, -4, -4, 2, -6, -2, 2, -6, 0, 5, -10, 2, 0, -2},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -4, 1, -4, -4, 2, -4, -1, 2, -6, -1, 5, -10, 0, 1, -2},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, -1, 0, -1, -5, 1, -4, -3, 2, -4, -2, 3, -6, 0, 4, -10, 0, 1, -3},
	{0, 0, 2, 1, 0, 0, 0, 0, 0, -2, 0, -2, -2, 1, -2, -4, 2, -5, -2, 2, -6, -2, 5, -10, 5, 1, -3},
	{0, 0, 1, 0, 0, 0, 0, 0, -1, -1, 0, -1, -4, 1, -3, -3, 1, -4, -2, 2, -6, -3, 5, -11, 2, 0, -2},
	{0, 0, 2, 1, 0, 0, 1, 0, 0, 0, 0, -1, -4, 1, -4, -3, 1, -4, -1, 2, -6, -4, 4, -10, 2, 1, -2},
	{0, 0, 2, 1, 0, 0, 0, 0, -1, -1, 0, -1, -2, 1, -2, -2, 2, -4, -3, 2, -5, -3, 4, -10, 0, 1, -4},
	{1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 1, -3, -2, 2, -4, -3, 2, -6, -3, 4, -10, 0, 1, -3},
	{0, 0, 2, 0, 0, 0, 0, 0, -1, 0, 0, -1, -2, 1, -2, -4, 2, -4, -3, 2, -6, -2, 5, -10, 0, 1, -3},
	{0, 0, 1, 1, 0, 1, 0, 0, 0, -1, 0, -2, -2, 1, -2, -4, 2, -4, -3, 2, -6, -2, 4, -8, 2, 2, -5},
	{0, 0, 1, 0, 0, 0, 0, 0, 0, -1, 0, -1, -3, 0, -2, -3, 1, -4, -3, 2, -6, -2, 4, -10, 0, 2, -5},
	{1, 0, 3, 0, 0, 0, 0, 0, -1, -1, 0, -1, -3, 0, -2, -2, 1, -3, -3, 2, -5, -3, 4, -10, 0, 2, -5},
	{1, 0, 2, 1, 0, 1, 0, 0, 0, 0, 0, -1, -3, 1, -2, -4, 1, -4, -3, 2, -5, -3, 5, -11, 1, 2, -4},
	{1, 0, 2, 0, 0, 1, 0, 0, 0, -1, 0, -2, -3, 0, -2, -3, 1, -3, -2, 2, -4, -4, 5, -11, 0, 1, -3},
	{1, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, -1, -2, 1, -2, -3, 1, -3, -3, 2, -4, -3, 5, -11, 0, 2, -5},
	{2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 0, -2, -3, 1, -3, -3, 2, -5, -4, 4, -10, 0, 2, -5},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2, 0, -2, -3, 1, -3, -4, 2, -5, -5, 4, -10, 0, 2, -5},
	{3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, -2, -3, 0, -2, -2, 1, -2, -3, 2, -5, -2, 4, -10, 0, 2, -4},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, -1, -3, 0, -2, -3, 1, -3, -2, 2, -4, -4, 4, -10, 0, 1, -4},
	{1, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -2, -2, 0, -1, -1, 1, -3, -1, 4, -8, 3, 1, -2},
	{1, 0, 2, 0, 0, 1, 1, 0, 0, 0, 0, -1, -2, 0, 0, -1, 0, -2, -1, 1, -4, -1, 4, -8, 3, 1, -3},
	{2, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, -1, 0, -1, -3, 0, -2, 0, 1, -3, -1, 4, -9, 4, 1, -2},
	{1, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, -2, 0, -1, -1, 0, -1, 0, 1, -3, -3, 4, -9, 0, 0, -2},
	{3, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, -2, 1, -2, 0, 1, -3, -2, 4, -8, 0, 1, -2},
	{1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, -3, 0, -1, -2, 0, -2, -1, 1, -3, -2, 3, -8, 0, 1, -4},
};
#endif
