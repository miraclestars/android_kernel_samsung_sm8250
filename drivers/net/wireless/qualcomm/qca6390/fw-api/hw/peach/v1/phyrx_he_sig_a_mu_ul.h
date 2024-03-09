/*
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#ifndef _PHYRX_HE_SIG_A_MU_UL_H_
#define _PHYRX_HE_SIG_A_MU_UL_H_

#include "he_sig_a_mu_ul_info.h"
#define NUM_OF_DWORDS_PHYRX_HE_SIG_A_MU_UL 2

struct phyrx_he_sig_a_mu_ul {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             struct   he_sig_a_mu_ul_info                                       phyrx_he_sig_a_mu_ul_info_details;
#else
             struct   he_sig_a_mu_ul_info                                       phyrx_he_sig_a_mu_ul_info_details;
#endif
};

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_FORMAT_INDICATION_OFFSET 0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_FORMAT_INDICATION_LSB 0
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_FORMAT_INDICATION_MSB 0
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_FORMAT_INDICATION_MASK 0x00000001

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_BSS_COLOR_ID_OFFSET  0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_BSS_COLOR_ID_LSB     1
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_BSS_COLOR_ID_MSB     6
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_BSS_COLOR_ID_MASK    0x0000007e

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_SPATIAL_REUSE_OFFSET 0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_SPATIAL_REUSE_LSB    7
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_SPATIAL_REUSE_MSB    22
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_SPATIAL_REUSE_MASK   0x007fff80

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0A_OFFSET   0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0A_LSB      23
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0A_MSB      23
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0A_MASK     0x00800000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TRANSMIT_BW_OFFSET   0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TRANSMIT_BW_LSB      24
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TRANSMIT_BW_MSB      25
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TRANSMIT_BW_MASK     0x03000000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0B_OFFSET   0x00000000
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0B_LSB      26
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0B_MSB      31
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_0B_MASK     0xfc000000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TXOP_DURATION_OFFSET 0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TXOP_DURATION_LSB    0
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TXOP_DURATION_MSB    6
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TXOP_DURATION_MASK   0x0000007f

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1A_OFFSET   0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1A_LSB      7
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1A_MSB      15
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1A_MASK     0x0000ff80

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_CRC_OFFSET           0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_CRC_LSB              16
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_CRC_MSB              19
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_CRC_MASK             0x000f0000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TAIL_OFFSET          0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TAIL_LSB             20
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TAIL_MSB             25
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_TAIL_MASK            0x03f00000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1B_OFFSET   0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1B_LSB      26
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1B_MSB      30
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RESERVED_1B_MASK     0x7c000000

#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_OFFSET 0x00000004
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_LSB 31
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MSB 31
#define PHYRX_HE_SIG_A_MU_UL_PHYRX_HE_SIG_A_MU_UL_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MASK 0x80000000

#endif
