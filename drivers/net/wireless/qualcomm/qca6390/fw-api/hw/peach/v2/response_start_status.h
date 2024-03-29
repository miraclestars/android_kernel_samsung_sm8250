/*
 * Copyright (c) 2023-2024 Qualcomm Innovation Center, Inc. All rights reserved.
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


#ifndef _RESPONSE_START_STATUS_H_
#define _RESPONSE_START_STATUS_H_

#define NUM_OF_DWORDS_RESPONSE_START_STATUS 2

struct response_start_status {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             uint32_t generated_response                                      :  3,
                      __reserved_g_0012                                                  :  2,
                      trig_response_related                                   :  1,
                      response_sta_count                                      :  7,
                      reserved                                                : 19;
             uint32_t phy_ppdu_id                                             : 16,
                      sw_peer_id                                              : 16;
#else
             uint32_t reserved                                                : 19,
                      response_sta_count                                      :  7,
                      trig_response_related                                   :  1,
                      __reserved_g_0012                                                  :  2,
                      generated_response                                      :  3;
             uint32_t sw_peer_id                                              : 16,
                      phy_ppdu_id                                             : 16;
#endif
};

#define RESPONSE_START_STATUS_GENERATED_RESPONSE_OFFSET                             0x00000000
#define RESPONSE_START_STATUS_GENERATED_RESPONSE_LSB                                0
#define RESPONSE_START_STATUS_GENERATED_RESPONSE_MSB                                2
#define RESPONSE_START_STATUS_GENERATED_RESPONSE_MASK                               0x00000007

#define RESPONSE_START_STATUS_TRIG_RESPONSE_RELATED_OFFSET                          0x00000000
#define RESPONSE_START_STATUS_TRIG_RESPONSE_RELATED_LSB                             5
#define RESPONSE_START_STATUS_TRIG_RESPONSE_RELATED_MSB                             5
#define RESPONSE_START_STATUS_TRIG_RESPONSE_RELATED_MASK                            0x00000020

#define RESPONSE_START_STATUS_RESPONSE_STA_COUNT_OFFSET                             0x00000000
#define RESPONSE_START_STATUS_RESPONSE_STA_COUNT_LSB                                6
#define RESPONSE_START_STATUS_RESPONSE_STA_COUNT_MSB                                12
#define RESPONSE_START_STATUS_RESPONSE_STA_COUNT_MASK                               0x00001fc0

#define RESPONSE_START_STATUS_RESERVED_OFFSET                                       0x00000000
#define RESPONSE_START_STATUS_RESERVED_LSB                                          13
#define RESPONSE_START_STATUS_RESERVED_MSB                                          31
#define RESPONSE_START_STATUS_RESERVED_MASK                                         0xffffe000

#define RESPONSE_START_STATUS_PHY_PPDU_ID_OFFSET                                    0x00000004
#define RESPONSE_START_STATUS_PHY_PPDU_ID_LSB                                       0
#define RESPONSE_START_STATUS_PHY_PPDU_ID_MSB                                       15
#define RESPONSE_START_STATUS_PHY_PPDU_ID_MASK                                      0x0000ffff

#define RESPONSE_START_STATUS_SW_PEER_ID_OFFSET                                     0x00000004
#define RESPONSE_START_STATUS_SW_PEER_ID_LSB                                        16
#define RESPONSE_START_STATUS_SW_PEER_ID_MSB                                        31
#define RESPONSE_START_STATUS_SW_PEER_ID_MASK                                       0xffff0000

#endif
