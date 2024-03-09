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


#ifndef _COEX_TX_REQ_H_
#define _COEX_TX_REQ_H_

#define NUM_OF_DWORDS_COEX_TX_REQ 4

struct coex_tx_req {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             uint32_t tx_pwr                                                  :  8,
                      min_tx_pwr                                              :  8,
                      nss                                                     :  3,
                      tx_chain_mask                                           :  8,
                      bw                                                      :  3,
                      reserved_0                                              :  2;
             uint32_t alt_tx_pwr                                              :  8,
                      alt_min_tx_pwr                                          :  8,
                      alt_nss                                                 :  3,
                      alt_tx_chain_mask                                       :  8,
                      alt_bw                                                  :  3,
                      reserved_1                                              :  2;
             uint32_t tx_pwr_1                                                :  8,
                      alt_tx_pwr_1                                            :  8,
                      wlan_request_duration                                   : 16;
             uint32_t wlan_pkt_type                                           :  4,
                      coex_tx_reason                                          :  2,
                      response_frame_type                                     :  5,
                      wlan_low_priority_slicing_allowed                       :  1,
                      wlan_high_priority_slicing_allowed                      :  1,
                      sch_tx_burst_ongoing                                    :  1,
                      coex_tx_priority                                        :  4,
                      reserved_3a                                             : 14;
#else
             uint32_t reserved_0                                              :  2,
                      bw                                                      :  3,
                      tx_chain_mask                                           :  8,
                      nss                                                     :  3,
                      min_tx_pwr                                              :  8,
                      tx_pwr                                                  :  8;
             uint32_t reserved_1                                              :  2,
                      alt_bw                                                  :  3,
                      alt_tx_chain_mask                                       :  8,
                      alt_nss                                                 :  3,
                      alt_min_tx_pwr                                          :  8,
                      alt_tx_pwr                                              :  8;
             uint32_t wlan_request_duration                                   : 16,
                      alt_tx_pwr_1                                            :  8,
                      tx_pwr_1                                                :  8;
             uint32_t reserved_3a                                             : 14,
                      coex_tx_priority                                        :  4,
                      sch_tx_burst_ongoing                                    :  1,
                      wlan_high_priority_slicing_allowed                      :  1,
                      wlan_low_priority_slicing_allowed                       :  1,
                      response_frame_type                                     :  5,
                      coex_tx_reason                                          :  2,
                      wlan_pkt_type                                           :  4;
#endif
};

#define COEX_TX_REQ_TX_PWR_OFFSET                                                   0x00000000
#define COEX_TX_REQ_TX_PWR_LSB                                                      0
#define COEX_TX_REQ_TX_PWR_MSB                                                      7
#define COEX_TX_REQ_TX_PWR_MASK                                                     0x000000ff

#define COEX_TX_REQ_MIN_TX_PWR_OFFSET                                               0x00000000
#define COEX_TX_REQ_MIN_TX_PWR_LSB                                                  8
#define COEX_TX_REQ_MIN_TX_PWR_MSB                                                  15
#define COEX_TX_REQ_MIN_TX_PWR_MASK                                                 0x0000ff00

#define COEX_TX_REQ_NSS_OFFSET                                                      0x00000000
#define COEX_TX_REQ_NSS_LSB                                                         16
#define COEX_TX_REQ_NSS_MSB                                                         18
#define COEX_TX_REQ_NSS_MASK                                                        0x00070000

#define COEX_TX_REQ_TX_CHAIN_MASK_OFFSET                                            0x00000000
#define COEX_TX_REQ_TX_CHAIN_MASK_LSB                                               19
#define COEX_TX_REQ_TX_CHAIN_MASK_MSB                                               26
#define COEX_TX_REQ_TX_CHAIN_MASK_MASK                                              0x07f80000

#define COEX_TX_REQ_BW_OFFSET                                                       0x00000000
#define COEX_TX_REQ_BW_LSB                                                          27
#define COEX_TX_REQ_BW_MSB                                                          29
#define COEX_TX_REQ_BW_MASK                                                         0x38000000

#define COEX_TX_REQ_RESERVED_0_OFFSET                                               0x00000000
#define COEX_TX_REQ_RESERVED_0_LSB                                                  30
#define COEX_TX_REQ_RESERVED_0_MSB                                                  31
#define COEX_TX_REQ_RESERVED_0_MASK                                                 0xc0000000

#define COEX_TX_REQ_ALT_TX_PWR_OFFSET                                               0x00000004
#define COEX_TX_REQ_ALT_TX_PWR_LSB                                                  0
#define COEX_TX_REQ_ALT_TX_PWR_MSB                                                  7
#define COEX_TX_REQ_ALT_TX_PWR_MASK                                                 0x000000ff

#define COEX_TX_REQ_ALT_MIN_TX_PWR_OFFSET                                           0x00000004
#define COEX_TX_REQ_ALT_MIN_TX_PWR_LSB                                              8
#define COEX_TX_REQ_ALT_MIN_TX_PWR_MSB                                              15
#define COEX_TX_REQ_ALT_MIN_TX_PWR_MASK                                             0x0000ff00

#define COEX_TX_REQ_ALT_NSS_OFFSET                                                  0x00000004
#define COEX_TX_REQ_ALT_NSS_LSB                                                     16
#define COEX_TX_REQ_ALT_NSS_MSB                                                     18
#define COEX_TX_REQ_ALT_NSS_MASK                                                    0x00070000

#define COEX_TX_REQ_ALT_TX_CHAIN_MASK_OFFSET                                        0x00000004
#define COEX_TX_REQ_ALT_TX_CHAIN_MASK_LSB                                           19
#define COEX_TX_REQ_ALT_TX_CHAIN_MASK_MSB                                           26
#define COEX_TX_REQ_ALT_TX_CHAIN_MASK_MASK                                          0x07f80000

#define COEX_TX_REQ_ALT_BW_OFFSET                                                   0x00000004
#define COEX_TX_REQ_ALT_BW_LSB                                                      27
#define COEX_TX_REQ_ALT_BW_MSB                                                      29
#define COEX_TX_REQ_ALT_BW_MASK                                                     0x38000000

#define COEX_TX_REQ_RESERVED_1_OFFSET                                               0x00000004
#define COEX_TX_REQ_RESERVED_1_LSB                                                  30
#define COEX_TX_REQ_RESERVED_1_MSB                                                  31
#define COEX_TX_REQ_RESERVED_1_MASK                                                 0xc0000000

#define COEX_TX_REQ_TX_PWR_1_OFFSET                                                 0x00000008
#define COEX_TX_REQ_TX_PWR_1_LSB                                                    0
#define COEX_TX_REQ_TX_PWR_1_MSB                                                    7
#define COEX_TX_REQ_TX_PWR_1_MASK                                                   0x000000ff

#define COEX_TX_REQ_ALT_TX_PWR_1_OFFSET                                             0x00000008
#define COEX_TX_REQ_ALT_TX_PWR_1_LSB                                                8
#define COEX_TX_REQ_ALT_TX_PWR_1_MSB                                                15
#define COEX_TX_REQ_ALT_TX_PWR_1_MASK                                               0x0000ff00

#define COEX_TX_REQ_WLAN_REQUEST_DURATION_OFFSET                                    0x00000008
#define COEX_TX_REQ_WLAN_REQUEST_DURATION_LSB                                       16
#define COEX_TX_REQ_WLAN_REQUEST_DURATION_MSB                                       31
#define COEX_TX_REQ_WLAN_REQUEST_DURATION_MASK                                      0xffff0000

#define COEX_TX_REQ_WLAN_PKT_TYPE_OFFSET                                            0x0000000c
#define COEX_TX_REQ_WLAN_PKT_TYPE_LSB                                               0
#define COEX_TX_REQ_WLAN_PKT_TYPE_MSB                                               3
#define COEX_TX_REQ_WLAN_PKT_TYPE_MASK                                              0x0000000f

#define COEX_TX_REQ_COEX_TX_REASON_OFFSET                                           0x0000000c
#define COEX_TX_REQ_COEX_TX_REASON_LSB                                              4
#define COEX_TX_REQ_COEX_TX_REASON_MSB                                              5
#define COEX_TX_REQ_COEX_TX_REASON_MASK                                             0x00000030

#define COEX_TX_REQ_RESPONSE_FRAME_TYPE_OFFSET                                      0x0000000c
#define COEX_TX_REQ_RESPONSE_FRAME_TYPE_LSB                                         6
#define COEX_TX_REQ_RESPONSE_FRAME_TYPE_MSB                                         10
#define COEX_TX_REQ_RESPONSE_FRAME_TYPE_MASK                                        0x000007c0

#define COEX_TX_REQ_WLAN_LOW_PRIORITY_SLICING_ALLOWED_OFFSET                        0x0000000c
#define COEX_TX_REQ_WLAN_LOW_PRIORITY_SLICING_ALLOWED_LSB                           11
#define COEX_TX_REQ_WLAN_LOW_PRIORITY_SLICING_ALLOWED_MSB                           11
#define COEX_TX_REQ_WLAN_LOW_PRIORITY_SLICING_ALLOWED_MASK                          0x00000800

#define COEX_TX_REQ_WLAN_HIGH_PRIORITY_SLICING_ALLOWED_OFFSET                       0x0000000c
#define COEX_TX_REQ_WLAN_HIGH_PRIORITY_SLICING_ALLOWED_LSB                          12
#define COEX_TX_REQ_WLAN_HIGH_PRIORITY_SLICING_ALLOWED_MSB                          12
#define COEX_TX_REQ_WLAN_HIGH_PRIORITY_SLICING_ALLOWED_MASK                         0x00001000

#define COEX_TX_REQ_SCH_TX_BURST_ONGOING_OFFSET                                     0x0000000c
#define COEX_TX_REQ_SCH_TX_BURST_ONGOING_LSB                                        13
#define COEX_TX_REQ_SCH_TX_BURST_ONGOING_MSB                                        13
#define COEX_TX_REQ_SCH_TX_BURST_ONGOING_MASK                                       0x00002000

#define COEX_TX_REQ_COEX_TX_PRIORITY_OFFSET                                         0x0000000c
#define COEX_TX_REQ_COEX_TX_PRIORITY_LSB                                            14
#define COEX_TX_REQ_COEX_TX_PRIORITY_MSB                                            17
#define COEX_TX_REQ_COEX_TX_PRIORITY_MASK                                           0x0003c000

#define COEX_TX_REQ_RESERVED_3A_OFFSET                                              0x0000000c
#define COEX_TX_REQ_RESERVED_3A_LSB                                                 18
#define COEX_TX_REQ_RESERVED_3A_MSB                                                 31
#define COEX_TX_REQ_RESERVED_3A_MASK                                                0xfffc0000

#endif
