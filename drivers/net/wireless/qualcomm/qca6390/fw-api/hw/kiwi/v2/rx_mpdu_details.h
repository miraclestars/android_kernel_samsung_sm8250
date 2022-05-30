
/*
 * Copyright (c) 2022 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

 

#ifndef _RX_MPDU_DETAILS_H_
#define _RX_MPDU_DETAILS_H_
#if !defined(__ASSEMBLER__)
#endif

#include "rx_mpdu_desc_info.h"
#include "buffer_addr_info.h"
#define NUM_OF_DWORDS_RX_MPDU_DETAILS 4

struct rx_mpdu_details {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             struct   buffer_addr_info                                          msdu_link_desc_addr_info;
             struct   rx_mpdu_desc_info                                         rx_mpdu_desc_info_details;
#else
             struct   buffer_addr_info                                          msdu_link_desc_addr_info;
             struct   rx_mpdu_desc_info                                         rx_mpdu_desc_info_details;
#endif
};

#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_31_0_OFFSET            0x00000000
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_31_0_LSB               0
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_31_0_MSB               31
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_31_0_MASK              0xffffffff

#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_39_32_OFFSET           0x00000004
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_39_32_LSB              0
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_39_32_MSB              7
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_BUFFER_ADDR_39_32_MASK             0x000000ff

#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_RETURN_BUFFER_MANAGER_OFFSET       0x00000004
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_RETURN_BUFFER_MANAGER_LSB          8
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_RETURN_BUFFER_MANAGER_MSB          11
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_RETURN_BUFFER_MANAGER_MASK         0x00000f00

#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_SW_BUFFER_COOKIE_OFFSET            0x00000004
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_SW_BUFFER_COOKIE_LSB               12
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_SW_BUFFER_COOKIE_MSB               31
#define RX_MPDU_DETAILS_MSDU_LINK_DESC_ADDR_INFO_SW_BUFFER_COOKIE_MASK              0xfffff000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MSDU_COUNT_OFFSET                 0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MSDU_COUNT_LSB                    0
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MSDU_COUNT_MSB                    7
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MSDU_COUNT_MASK                   0x000000ff

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_FRAGMENT_FLAG_OFFSET              0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_FRAGMENT_FLAG_LSB                 8
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_FRAGMENT_FLAG_MSB                 8
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_FRAGMENT_FLAG_MASK                0x00000100

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_RETRY_BIT_OFFSET             0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_RETRY_BIT_LSB                9
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_RETRY_BIT_MSB                9
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_RETRY_BIT_MASK               0x00000200

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_AMPDU_FLAG_OFFSET                 0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_AMPDU_FLAG_LSB                    10
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_AMPDU_FLAG_MSB                    10
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_AMPDU_FLAG_MASK                   0x00000400

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_BAR_FRAME_OFFSET                  0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_BAR_FRAME_LSB                     11
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_BAR_FRAME_MSB                     11
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_BAR_FRAME_MASK                    0x00000800

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PN_FIELDS_CONTAIN_VALID_INFO_OFFSET 0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PN_FIELDS_CONTAIN_VALID_INFO_LSB  12
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PN_FIELDS_CONTAIN_VALID_INFO_MSB  12
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PN_FIELDS_CONTAIN_VALID_INFO_MASK 0x00001000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_RAW_MPDU_OFFSET                   0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_RAW_MPDU_LSB                      13
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_RAW_MPDU_MSB                      13
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_RAW_MPDU_MASK                     0x00002000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MORE_FRAGMENT_FLAG_OFFSET         0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MORE_FRAGMENT_FLAG_LSB            14
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MORE_FRAGMENT_FLAG_MSB            14
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MORE_FRAGMENT_FLAG_MASK           0x00004000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_SRC_INFO_OFFSET                   0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_SRC_INFO_LSB                      15
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_SRC_INFO_MSB                      26
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_SRC_INFO_MASK                     0x07ff8000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_QOS_CONTROL_VALID_OFFSET     0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_QOS_CONTROL_VALID_LSB        27
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_QOS_CONTROL_VALID_MSB        27
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_MPDU_QOS_CONTROL_VALID_MASK       0x08000000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_TID_OFFSET                        0x00000008
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_TID_LSB                           28
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_TID_MSB                           31
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_TID_MASK                          0xf0000000

#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PEER_META_DATA_OFFSET             0x0000000c
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PEER_META_DATA_LSB                0
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PEER_META_DATA_MSB                31
#define RX_MPDU_DETAILS_RX_MPDU_DESC_INFO_DETAILS_PEER_META_DATA_MASK               0xffffffff

#endif
