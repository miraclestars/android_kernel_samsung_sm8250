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


#ifndef _MACTX_HE_SIG_B2_MU_H_
#define _MACTX_HE_SIG_B2_MU_H_

#include "he_sig_b2_mu_info.h"
#define NUM_OF_DWORDS_MACTX_HE_SIG_B2_MU 2

struct mactx_he_sig_b2_mu {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             struct   he_sig_b2_mu_info                                         mactx_he_sig_b2_mu_info_details;
#else
             struct   he_sig_b2_mu_info                                         mactx_he_sig_b2_mu_info_details;
#endif
};

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_ID_OFFSET            0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_ID_LSB               0
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_ID_MSB               10
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_ID_MASK              0x000007ff

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_SPATIAL_CONFIG_OFFSET 0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_SPATIAL_CONFIG_LSB   11
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_SPATIAL_CONFIG_MSB   14
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_SPATIAL_CONFIG_MASK  0x00007800

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_MCS_OFFSET           0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_MCS_LSB              15
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_MCS_MSB              18
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_MCS_MASK             0x00078000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_SET_TO_1_OFFSET 0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_SET_TO_1_LSB    19
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_SET_TO_1_MSB    19
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_SET_TO_1_MASK   0x00080000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_CODING_OFFSET        0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_CODING_LSB           20
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_CODING_MSB           20
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_STA_CODING_MASK          0x00100000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_0A_OFFSET       0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_0A_LSB          21
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_0A_MSB          27
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_0A_MASK         0x0fe00000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_NSTS_OFFSET              0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_NSTS_LSB                 28
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_NSTS_MSB                 30
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_NSTS_MASK                0x70000000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_OFFSET 0x00000000
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_LSB 31
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MSB 31
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MASK 0x80000000

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_USER_ORDER_OFFSET        0x00000004
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_USER_ORDER_LSB           0
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_USER_ORDER_MSB           7
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_USER_ORDER_MASK          0x000000ff

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_CC_MASK_OFFSET           0x00000004
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_CC_MASK_LSB              8
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_CC_MASK_MSB              15
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_CC_MASK_MASK             0x0000ff00

#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_1A_OFFSET       0x00000004
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_1A_LSB          16
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_1A_MSB          31
#define MACTX_HE_SIG_B2_MU_MACTX_HE_SIG_B2_MU_INFO_DETAILS_RESERVED_1A_MASK         0xffff0000

#endif
