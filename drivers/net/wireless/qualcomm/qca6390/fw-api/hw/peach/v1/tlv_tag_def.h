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


#ifndef _TLV_TAG_DEF_
#define _TLV_TAG_DEF_

typedef enum tlv_tag_def {
  WIFIMACTX_CBF_START_E                                    = 0  ,
  WIFIPHYRX_DATA_E                                         = 1  ,
  WIFIPHYRX_CBF_DATA_RESP_E                                = 2  ,
  WIFIPHYRX_ABORT_REQUEST_E                                = 3  ,
  WIFIPHYRX_USER_ABORT_NOTIFICATION_E                      = 4  ,
  WIFIMACTX_DATA_RESP_E                                    = 5  ,
  WIFIMACTX_CBF_DATA_E                                     = 6  ,
  WIFIMACTX_CBF_DONE_E                                     = 7  ,
  WIFIPHYRX_LMR_DATA_RESP_E                                = 8  ,
  WIFIRXPCU_TO_UCODE_START_E                               = 9  ,
  WIFIRXPCU_TO_UCODE_DELIMITER_FOR_FULL_MPDU_E             = 10  ,
  WIFIRXPCU_TO_UCODE_FULL_MPDU_DATA_E                      = 11  ,
  WIFIRXPCU_TO_UCODE_FCS_STATUS_E                          = 12  ,
  WIFIRXPCU_TO_UCODE_MPDU_DELIMITER_E                      = 13  ,
  WIFIRXPCU_TO_UCODE_DELIMITER_FOR_MPDU_HEADER_E           = 14  ,
  WIFIRXPCU_TO_UCODE_MPDU_HEADER_DATA_E                    = 15  ,
  WIFIRXPCU_TO_UCODE_END_E                                 = 16  ,
  WIFIPHYRX_RSSI_LEGACY_20MHZ_E                            = 28  ,
  WIFIPHYRX_NC_ABORT_REQUEST_E                             = 29  ,
  WIFIPHYRX_PKT_END_20MHZ_E                                = 30  ,
  WIFIPHYRX_NC_DATA_E                                      = 31  ,
  WIFIMACRX_CBF_READ_REQUEST_E                             = 32  ,
  WIFIMACRX_CBF_DATA_REQUEST_E                             = 33  ,
  WIFIMACRX_EXPECT_NDP_RECEPTION_E                         = 34  ,
  WIFIMACRX_FREEZE_CAPTURE_CHANNEL_E                       = 35  ,
  WIFIMACRX_NDP_TIMEOUT_E                                  = 36  ,
  WIFIMACRX_ABORT_ACK_E                                    = 37  ,
  WIFIMACRX_REQ_IMPLICIT_FB_E                              = 38  ,
  WIFIMACRX_CHAIN_MASK_E                                   = 39  ,
  WIFIMACRX_NAP_USER_E                                     = 40  ,
  WIFIMACRX_ABORT_REQUEST_E                                = 41  ,
  WIFIPHYTX_OTHER_TRANSMIT_INFO16_E                        = 42  ,
  WIFIPHYTX_ABORT_ACK_E                                    = 43  ,
  WIFIPHYTX_ABORT_REQUEST_E                                = 44  ,
  WIFIPHYTX_PKT_END_E                                      = 45  ,
  WIFIPHYTX_PPDU_HEADER_INFO_REQUEST_E                     = 46  ,
  WIFIPHYTX_REQUEST_CTRL_INFO_E                            = 47  ,
  WIFIPHYTX_DATA_REQUEST_E                                 = 48  ,
  WIFIPHYTX_BF_CV_LOADING_DONE_E                           = 49  ,
  WIFIPHYTX_NAP_ACK_E                                      = 50  ,
  WIFIPHYTX_NAP_DONE_E                                     = 51  ,
  WIFIPHYTX_OFF_ACK_E                                      = 52  ,
  WIFIPHYTX_ON_ACK_E                                       = 53  ,
  WIFIPHYTX_SYNTH_OFF_ACK_E                                = 54  ,
  WIFIPHYTX_DEBUG16_E                                      = 55  ,
  WIFIMACTX_ABORT_REQUEST_E                                = 56  ,
  WIFIMACTX_ABORT_ACK_E                                    = 57  ,
  WIFIMACTX_PKT_END_E                                      = 58  ,
  WIFIMACTX_PRE_PHY_DESC_E                                 = 59  ,
  WIFIMACTX_BF_PARAMS_COMMON_E                             = 60  ,
  WIFIMACTX_BF_PARAMS_PER_USER_E                           = 61  ,
  WIFIMACTX_PREFETCH_CV_E                                  = 62  ,
  WIFIMACTX_USER_DESC_COMMON_E                             = 63  ,
  WIFIMACTX_USER_DESC_PER_USER_E                           = 64  ,
  WIFIEXAMPLE_USER_TLV_16_E                                = 65  ,
  WIFIEXAMPLE_TLV_16_E                                     = 66  ,
  WIFIMACTX_PHY_OFF_E                                      = 67  ,
  WIFIMACTX_PHY_ON_E                                       = 68  ,
  WIFIMACTX_SYNTH_OFF_E                                    = 69  ,
  WIFIMACTX_EXPECT_CBF_COMMON_E                            = 70  ,
  WIFIMACTX_EXPECT_CBF_PER_USER_E                          = 71  ,
  WIFIMACTX_PHY_DESC_E                                     = 72  ,
  WIFIMACTX_L_SIG_A_E                                      = 73  ,
  WIFIMACTX_L_SIG_B_E                                      = 74  ,
  WIFIMACTX_HT_SIG_E                                       = 75  ,
  WIFIMACTX_VHT_SIG_A_E                                    = 76  ,
  WIFIMACTX_VHT_SIG_B_SU20_E                               = 77  ,
  WIFIMACTX_VHT_SIG_B_SU40_E                               = 78  ,
  WIFIMACTX_VHT_SIG_B_SU80_E                               = 79  ,
  WIFIMACTX_VHT_SIG_B_SU160_E                              = 80  ,
  WIFIMACTX_VHT_SIG_B_MU20_E                               = 81  ,
  WIFIMACTX_VHT_SIG_B_MU40_E                               = 82  ,
  WIFIMACTX_VHT_SIG_B_MU80_E                               = 83  ,
  WIFIMACTX_VHT_SIG_B_MU160_E                              = 84  ,
  WIFIMACTX_SERVICE_E                                      = 85  ,
  WIFIMACTX_HE_SIG_A_SU_E                                  = 86  ,
  WIFIMACTX_HE_SIG_A_MU_DL_E                               = 87  ,
  WIFIMACTX_HE_SIG_A_MU_UL_E                               = 88  ,
  WIFIMACTX_HE_SIG_B1_MU_E                                 = 89  ,
  WIFIMACTX_HE_SIG_B2_MU_E                                 = 90  ,
  WIFIMACTX_HE_SIG_B2_OFDMA_E                              = 91  ,
  WIFIMACTX_DELETE_CV_E                                    = 92  ,
  WIFIMACTX_MU_UPLINK_COMMON_E                             = 93  ,
  WIFIMACTX_MU_UPLINK_USER_SETUP_E                         = 94  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_E                          = 95  ,
  WIFIMACTX_PHY_NAP_E                                      = 96  ,
  WIFIMACTX_DEBUG_E                                        = 97  ,
  WIFIPHYRX_ABORT_ACK_E                                    = 98  ,
  WIFIPHYRX_GENERATED_CBF_DETAILS_E                        = 99  ,
  WIFIPHYRX_RSSI_LEGACY_E                                  = 100  ,
  WIFIPHYRX_RSSI_HT_E                                      = 101  ,
  WIFIPHYRX_USER_INFO_E                                    = 102  ,
  WIFIPHYRX_PKT_END_E                                      = 103  ,
  WIFIPHYRX_DEBUG_E                                        = 104  ,
  WIFIPHYRX_CBF_TRANSFER_DONE_E                            = 105  ,
  WIFIPHYRX_CBF_TRANSFER_ABORT_E                           = 106  ,
  WIFIPHYRX_L_SIG_A_E                                      = 107  ,
  WIFIPHYRX_L_SIG_B_E                                      = 108  ,
  WIFIPHYRX_HT_SIG_E                                       = 109  ,
  WIFIPHYRX_VHT_SIG_A_E                                    = 110  ,
  WIFIPHYRX_VHT_SIG_B_SU20_E                               = 111  ,
  WIFIPHYRX_VHT_SIG_B_SU40_E                               = 112  ,
  WIFIPHYRX_VHT_SIG_B_SU80_E                               = 113  ,
  WIFIPHYRX_VHT_SIG_B_SU160_E                              = 114  ,
  WIFIPHYRX_VHT_SIG_B_MU20_E                               = 115  ,
  WIFIPHYRX_VHT_SIG_B_MU40_E                               = 116  ,
  WIFIPHYRX_VHT_SIG_B_MU80_E                               = 117  ,
  WIFIPHYRX_VHT_SIG_B_MU160_E                              = 118  ,
  WIFIPHYRX_HE_SIG_A_SU_E                                  = 119  ,
  WIFIPHYRX_HE_SIG_A_MU_DL_E                               = 120  ,
  WIFIPHYRX_HE_SIG_A_MU_UL_E                               = 121  ,
  WIFIPHYRX_HE_SIG_B1_MU_E                                 = 122  ,
  WIFIPHYRX_HE_SIG_B2_MU_E                                 = 123  ,
  WIFIPHYRX_HE_SIG_B2_OFDMA_E                              = 124  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_E                           = 125  ,
  WIFIPHYRX_COMMON_USER_INFO_E                             = 126  ,
  WIFIPHYRX_DATA_DONE_E                                    = 127  ,
  WIFICOEX_TX_REQ_E                                        = 128  ,
  WIFIDUMMY_E                                              = 129  ,
  WIFIEXAMPLE_TLV_32_NAME_E                                = 130  ,
  WIFIMPDU_LIMIT_E                                         = 131  ,
  WIFINA_LENGTH_END_E                                      = 132  ,
  WIFIOLE_BUF_STATUS_E                                     = 133  ,
  WIFIPCU_PPDU_SETUP_DONE_E                                = 134  ,
  WIFIPCU_PPDU_SETUP_END_E                                 = 135  ,
  WIFIPCU_PPDU_SETUP_INIT_E                                = 136  ,
  WIFIPCU_PPDU_SETUP_START_E                               = 137  ,
  WIFIPDG_FES_SETUP_E                                      = 138  ,
  WIFIPDG_RESPONSE_E                                       = 139  ,
  WIFIPDG_TX_REQ_E                                         = 140  ,
  WIFISCH_WAIT_INSTR_E                                     = 141  ,
  WIFIMACTX_SWITCH_TO_MAIN_E                               = 142  ,
  WIFIPHYTX_LINK_STATE_E                                   = 143  ,
  WIFIAUX_PPDU_END_E                                       = 144  ,
  WIFITQM_GEN_MPDU_LENGTH_LIST_E                           = 145  ,
  WIFITQM_GEN_MPDU_LENGTH_LIST_STATUS_E                    = 146  ,
  WIFITQM_GEN_MPDUS_E                                      = 147  ,
  WIFITQM_GEN_MPDUS_STATUS_E                               = 148  ,
  WIFITQM_REMOVE_MPDU_E                                    = 149  ,
  WIFITQM_REMOVE_MPDU_STATUS_E                             = 150  ,
  WIFITQM_REMOVE_MSDU_E                                    = 151  ,
  WIFITQM_REMOVE_MSDU_STATUS_E                             = 152  ,
  WIFITQM_UPDATE_TX_MPDU_COUNT_E                           = 153  ,
  WIFITQM_WRITE_CMD_E                                      = 154  ,
  WIFIOFDMA_TRIGGER_DETAILS_E                              = 155  ,
  WIFITX_DATA_E                                            = 156  ,
  WIFITX_FES_SETUP_E                                       = 157  ,
  WIFIRX_PACKET_E                                          = 158  ,
  WIFIEXPECTED_RESPONSE_E                                  = 159  ,
  WIFITX_MPDU_END_E                                        = 160  ,
  WIFITX_MPDU_START_E                                      = 161  ,
  WIFITX_MSDU_END_E                                        = 162  ,
  WIFITX_MSDU_START_E                                      = 163  ,
  WIFITX_SW_MODE_SETUP_E                                   = 164  ,
  WIFITXPCU_BUFFER_STATUS_E                                = 165  ,
  WIFITXPCU_USER_BUFFER_STATUS_E                           = 166  ,
  WIFIDATA_TO_TIME_CONFIG_E                                = 167  ,
  WIFIEXAMPLE_USER_TLV_32_E                                = 168  ,
  WIFIMPDU_INFO_E                                          = 169  ,
  WIFIPDG_USER_SETUP_E                                     = 170  ,
  WIFITX_11AH_SETUP_E                                      = 171  ,
  WIFIREO_UPDATE_RX_REO_QUEUE_STATUS_E                     = 172  ,
  WIFITX_PEER_ENTRY_E                                      = 173  ,
  WIFITX_RAW_OR_NATIVE_FRAME_SETUP_E                       = 174  ,
  WIFIEXAMPLE_USER_TLV_44_E                                = 175  ,
  WIFITX_FLUSH_E                                           = 176  ,
  WIFITX_FLUSH_REQ_E                                       = 177  ,
  WIFITQM_WRITE_CMD_STATUS_E                               = 178  ,
  WIFITQM_GET_MPDU_QUEUE_STATS_E                           = 179  ,
  WIFITQM_GET_MSDU_FLOW_STATS_E                            = 180  ,
  WIFIEXAMPLE_USER_CTLV_44_E                               = 181  ,
  WIFITX_FES_STATUS_START_E                                = 182  ,
  WIFITX_FES_STATUS_USER_PPDU_E                            = 183  ,
  WIFITX_FES_STATUS_USER_RESPONSE_E                        = 184  ,
  WIFITX_FES_STATUS_END_E                                  = 185  ,
  WIFIRX_TRIG_INFO_E                                       = 186  ,
  WIFIRXPCU_TX_SETUP_CLEAR_E                               = 187  ,
  WIFIRX_FRAME_BITMAP_REQ_E                                = 188  ,
  WIFIRX_FRAME_BITMAP_ACK_E                                = 189  ,
  WIFICOEX_RX_STATUS_E                                     = 190  ,
  WIFIRX_START_PARAM_E                                     = 191  ,
  WIFIRX_PPDU_START_E                                      = 192  ,
  WIFIRX_PPDU_END_E                                        = 193  ,
  WIFIRX_MPDU_START_E                                      = 194  ,
  WIFIRX_MPDU_END_E                                        = 195  ,
  WIFIRX_MSDU_START_E                                      = 196  ,
  WIFIRX_MSDU_END_E                                        = 197  ,
  WIFIRX_ATTENTION_E                                       = 198  ,
  WIFIRECEIVED_RESPONSE_INFO_E                             = 199  ,
  WIFIRX_PHY_SLEEP_E                                       = 200  ,
  WIFIRX_HEADER_E                                          = 201  ,
  WIFIRX_PEER_ENTRY_E                                      = 202  ,
  WIFIRX_FLUSH_E                                           = 203  ,
  WIFIRX_RESPONSE_REQUIRED_INFO_E                          = 204  ,
  WIFIRX_FRAMELESS_BAR_DETAILS_E                           = 205  ,
  WIFITQM_GET_MPDU_QUEUE_STATS_STATUS_E                    = 206  ,
  WIFITQM_GET_MSDU_FLOW_STATS_STATUS_E                     = 207  ,
  WIFITX_CBF_INFO_E                                        = 208  ,
  WIFIPCU_PPDU_SETUP_USER_E                                = 209  ,
  WIFIRX_MPDU_PCU_START_E                                  = 210  ,
  WIFIRX_PM_INFO_E                                         = 211  ,
  WIFIRX_USER_PPDU_END_E                                   = 212  ,
  WIFIRX_PRE_PPDU_START_E                                  = 213  ,
  WIFIRX_PREAMBLE_E                                        = 214  ,
  WIFITX_FES_SETUP_COMPLETE_E                              = 215  ,
  WIFITX_LAST_MPDU_FETCHED_E                               = 216  ,
  WIFITXDMA_STOP_REQUEST_E                                 = 217  ,
  WIFIRXPCU_SETUP_E                                        = 218  ,
  WIFIRXPCU_USER_SETUP_E                                   = 219  ,
  WIFITX_FES_STATUS_ACK_OR_BA_E                            = 220  ,
  WIFITQM_ACKED_MPDU_E                                     = 221  ,
  WIFICOEX_TX_RESP_E                                       = 222  ,
  WIFICOEX_TX_STATUS_E                                     = 223  ,
  WIFIMACTX_COEX_PHY_CTRL_E                                = 224  ,
  WIFICOEX_STATUS_BROADCAST_E                              = 225  ,
  WIFIRESPONSE_START_STATUS_E                              = 226  ,
  WIFIRESPONSE_END_STATUS_E                                = 227  ,
  WIFICRYPTO_STATUS_E                                      = 228  ,
  WIFIRECEIVED_TRIGGER_INFO_E                              = 229  ,
  WIFICOEX_TX_STOP_CTRL_E                                  = 230  ,
  WIFIRX_PPDU_ACK_REPORT_E                                 = 231  ,
  WIFIRX_PPDU_NO_ACK_REPORT_E                              = 232  ,
  WIFISCH_COEX_STATUS_E                                    = 233  ,
  WIFISCHEDULER_COMMAND_STATUS_E                           = 234  ,
  WIFISCHEDULER_RX_PPDU_NO_RESPONSE_STATUS_E               = 235  ,
  WIFITX_FES_STATUS_PROT_E                                 = 236  ,
  WIFITX_FES_STATUS_START_PPDU_E                           = 237  ,
  WIFITX_FES_STATUS_START_PROT_E                           = 238  ,
  WIFITXPCU_PHYTX_DEBUG32_E                                = 239  ,
  WIFITXPCU_PHYTX_OTHER_TRANSMIT_INFO32_E                  = 240  ,
  WIFITX_MPDU_COUNT_TRANSFER_END_E                         = 241  ,
  WIFIWHO_ANCHOR_OFFSET_E                                  = 242  ,
  WIFIWHO_ANCHOR_VALUE_E                                   = 243  ,
  WIFIWHO_CCE_INFO_E                                       = 244  ,
  WIFIWHO_COMMIT_E                                         = 245  ,
  WIFIWHO_COMMIT_DONE_E                                    = 246  ,
  WIFIWHO_FLUSH_E                                          = 247  ,
  WIFIWHO_L2_LLC_E                                         = 248  ,
  WIFIWHO_L2_PAYLOAD_E                                     = 249  ,
  WIFIWHO_L3_CHECKSUM_E                                    = 250  ,
  WIFIWHO_L3_INFO_E                                        = 251  ,
  WIFIWHO_L4_CHECKSUM_E                                    = 252  ,
  WIFIWHO_L4_INFO_E                                        = 253  ,
  WIFIWHO_MSDU_E                                           = 254  ,
  WIFIWHO_MSDU_MISC_E                                      = 255  ,
  WIFIWHO_PACKET_DATA_E                                    = 256  ,
  WIFIWHO_PACKET_HDR_E                                     = 257  ,
  WIFIWHO_PPDU_END_E                                       = 258  ,
  WIFIWHO_PPDU_START_E                                     = 259  ,
  WIFIWHO_TSO_E                                            = 260  ,
  WIFIWHO_WMAC_HEADER_PV0_E                                = 261  ,
  WIFIWHO_WMAC_HEADER_PV1_E                                = 262  ,
  WIFIWHO_WMAC_IV_E                                        = 263  ,
  WIFIMPDU_INFO_END_E                                      = 264  ,
  WIFIMPDU_INFO_BITMAP_E                                   = 265  ,
  WIFITX_QUEUE_EXTENSION_E                                 = 266  ,
  WIFISCHEDULER_SELFGEN_RESPONSE_STATUS_E                  = 267  ,
  WIFITQM_UPDATE_TX_MPDU_COUNT_STATUS_E                    = 268  ,
  WIFITQM_ACKED_MPDU_STATUS_E                              = 269  ,
  WIFITQM_ADD_MSDU_STATUS_E                                = 270  ,
  WIFITQM_LIST_GEN_DONE_E                                  = 271  ,
  WIFIWHO_TERMINATE_E                                      = 272  ,
  WIFITX_LAST_MPDU_END_E                                   = 273  ,
  WIFITX_CV_DATA_E                                         = 274  ,
  WIFIPPDU_TX_END_E                                        = 275  ,
  WIFIPROT_TX_END_E                                        = 276  ,
  WIFIMPDU_INFO_GLOBAL_END_E                               = 277  ,
  WIFITQM_SCH_INSTR_GLOBAL_END_E                           = 278  ,
  WIFIRX_PPDU_END_USER_STATS_E                             = 279  ,
  WIFIRX_PPDU_END_USER_STATS_EXT_E                         = 280  ,
  WIFIREO_GET_QUEUE_STATS_E                                = 281  ,
  WIFIREO_FLUSH_QUEUE_E                                    = 282  ,
  WIFIREO_FLUSH_CACHE_E                                    = 283  ,
  WIFIREO_UNBLOCK_CACHE_E                                  = 284  ,
  WIFIREO_GET_QUEUE_STATS_STATUS_E                         = 285  ,
  WIFIREO_FLUSH_QUEUE_STATUS_E                             = 286  ,
  WIFIREO_FLUSH_CACHE_STATUS_E                             = 287  ,
  WIFIREO_UNBLOCK_CACHE_STATUS_E                           = 288  ,
  WIFITQM_FLUSH_CACHE_E                                    = 289  ,
  WIFITQM_UNBLOCK_CACHE_E                                  = 290  ,
  WIFITQM_FLUSH_CACHE_STATUS_E                             = 291  ,
  WIFITQM_UNBLOCK_CACHE_STATUS_E                           = 292  ,
  WIFIRX_PPDU_END_STATUS_DONE_E                            = 293  ,
  WIFIRX_STATUS_BUFFER_DONE_E                              = 294  ,
  WIFISCHEDULER_MLO_SW_MSG_STATUS_E                        = 295  ,
  WIFISCHEDULER_TXOP_DURATION_TRIGGER_E                    = 296  ,
  WIFITX_DATA_SYNC_E                                       = 297  ,
  WIFIPHYRX_CBF_READ_REQUEST_ACK_E                         = 298  ,
  WIFITQM_GET_MPDU_HEAD_INFO_E                             = 299  ,
  WIFITQM_SYNC_CMD_E                                       = 300  ,
  WIFITQM_GET_MPDU_HEAD_INFO_STATUS_E                      = 301  ,
  WIFITQM_SYNC_CMD_STATUS_E                                = 302  ,
  WIFITQM_THRESHOLD_DROP_NOTIFICATION_STATUS_E             = 303  ,
  WIFIREO_FLUSH_TIMEOUT_LIST_E                             = 305  ,
  WIFIREO_FLUSH_TIMEOUT_LIST_STATUS_E                      = 306  ,
  WIFIREO_DESCRIPTOR_THRESHOLD_REACHED_STATUS_E            = 307  ,
  WIFISCHEDULER_RX_SIFS_RESPONSE_TRIGGER_STATUS_E          = 308  ,
  WIFIEXAMPLE_USER_TLV_32_NAME_E                           = 309  ,
  WIFIRX_PPDU_START_USER_INFO_E                            = 310  ,
  WIFIRX_RING_MASK_E                                       = 311  ,
  WIFICOEX_MAC_NAP_E                                       = 312  ,
  WIFIRXPCU_PPDU_END_INFO_E                                = 313  ,
  WIFIWHO_MESH_CONTROL_E                                   = 314  ,
  WIFIPDG_SW_MODE_BW_START_E                               = 315  ,
  WIFIPDG_SW_MODE_BW_END_E                                 = 316  ,
  WIFIPDG_WAIT_FOR_MAC_REQUEST_E                           = 317  ,
  WIFIPDG_WAIT_FOR_PHY_REQUEST_E                           = 318  ,
  WIFISCHEDULER_END_E                                      = 319  ,
  WIFIRX_PPDU_START_DROPPED_E                              = 320  ,
  WIFIRX_PPDU_END_DROPPED_E                                = 321  ,
  WIFIRX_PPDU_END_STATUS_DONE_DROPPED_E                    = 322  ,
  WIFIRX_MPDU_START_DROPPED_E                              = 323  ,
  WIFIRX_MSDU_START_DROPPED_E                              = 324  ,
  WIFIRX_MSDU_END_DROPPED_E                                = 325  ,
  WIFIRX_MPDU_END_DROPPED_E                                = 326  ,
  WIFIRX_ATTENTION_DROPPED_E                               = 327  ,
  WIFITXPCU_USER_SETUP_E                                   = 328  ,
  WIFIRXPCU_USER_SETUP_EXT_E                               = 329  ,
  WIFICMD_PART_0_END_E                                     = 330  ,
  WIFIMACTX_SYNTH_ON_E                                     = 331  ,
  WIFISCH_CRITICAL_TLV_REFERENCE_E                         = 332  ,
  WIFITQM_MPDU_GLOBAL_START_E                              = 333  ,
  WIFIEXAMPLE_TLV_32_E                                     = 334  ,
  WIFITQM_UPDATE_TX_MSDU_FLOW_E                            = 335  ,
  WIFITQM_UPDATE_TX_MPDU_QUEUE_HEAD_E                      = 336  ,
  WIFITQM_UPDATE_TX_MSDU_FLOW_STATUS_E                     = 337  ,
  WIFITQM_UPDATE_TX_MPDU_QUEUE_HEAD_STATUS_E               = 338  ,
  WIFIREO_UPDATE_RX_REO_QUEUE_E                            = 339  ,
  WIFITQM_2_SCH_MPDU_AVAILABLE_E                           = 341  ,
  WIFIPDG_TRIG_RESPONSE_E                                  = 342  ,
  WIFITRIGGER_RESPONSE_TX_DONE_E                           = 343  ,
  WIFIABORT_FROM_PHYRX_DETAILS_E                           = 344  ,
  WIFISCH_TQM_CMD_WRAPPER_E                                = 345  ,
  WIFIMPDUS_AVAILABLE_E                                    = 346  ,
  WIFIRECEIVED_RESPONSE_INFO_PART2_E                       = 347  ,
  WIFIPHYRX_TX_START_TIMING_E                              = 348  ,
  WIFITXPCU_PREAMBLE_DONE_E                                = 349  ,
  WIFINDP_PREAMBLE_DONE_E                                  = 350  ,
  WIFISCH_TQM_CMD_WRAPPER_RBO_DROP_E                       = 351  ,
  WIFISCH_TQM_CMD_WRAPPER_CONT_DROP_E                      = 352  ,
  WIFIMACTX_CLEAR_PREV_TX_INFO_E                           = 353  ,
  WIFITX_PUNCTURE_SETUP_E                                  = 354  ,
  WIFIR2R_STATUS_END_E                                     = 355  ,
  WIFIMACTX_PREFETCH_CV_COMMON_E                           = 356  ,
  WIFIEND_OF_FLUSH_MARKER_E                                = 357  ,
  WIFIMACTX_MU_UPLINK_COMMON_PUNC_E                        = 358  ,
  WIFIMACTX_MU_UPLINK_USER_SETUP_PUNC_E                    = 359  ,
  WIFIRECEIVED_RESPONSE_USER_7_0_E                         = 360  ,
  WIFIRECEIVED_RESPONSE_USER_15_8_E                        = 361  ,
  WIFIRECEIVED_RESPONSE_USER_23_16_E                       = 362  ,
  WIFIRECEIVED_RESPONSE_USER_31_24_E                       = 363  ,
  WIFIRECEIVED_RESPONSE_USER_36_32_E                       = 364  ,
  WIFITX_LOOPBACK_SETUP_E                                  = 365  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_RU_DETAILS_E                = 366  ,
  WIFISCH_WAIT_INSTR_TX_PATH_E                             = 367  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_TX2TX_E                    = 368  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_EMUPHY_SETUP_E             = 369  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_EVM_DETAILS_E               = 370  ,
  WIFITX_WUR_DATA_E                                        = 371  ,
  WIFIRX_PPDU_END_START_E                                  = 372  ,
  WIFIRX_PPDU_END_MIDDLE_E                                 = 373  ,
  WIFIRX_PPDU_END_LAST_E                                   = 374  ,
  WIFIMACTX_BACKOFF_BASED_TRANSMISSION_E                   = 375  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_DL_OFDMA_TX_E              = 376  ,
  WIFISRP_INFO_E                                           = 377  ,
  WIFIOBSS_SR_INFO_E                                       = 378  ,
  WIFISCHEDULER_SW_MSG_STATUS_E                            = 379  ,
  WIFIHWSCH_RXPCU_MAC_INFO_ANNOUNCEMENT_E                  = 380  ,
  WIFIRXPCU_SETUP_COMPLETE_E                               = 381  ,
  WIFIMACTX_MCC_SWITCH_E                                   = 382  ,
  WIFIMACTX_MCC_SWITCH_BACK_E                              = 383  ,
  WIFIPHYTX_MCC_SWITCH_ACK_E                               = 384  ,
  WIFIPHYTX_MCC_SWITCH_BACK_ACK_E                          = 385  ,
  WIFIPHYTX_EMLSR_PRE_SWITCH_ACK_E                         = 386  ,
  WIFILMR_TX_END_E                                         = 389  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_MU_RSSI_COMMON_E            = 390  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_MU_RSSI_USER_E              = 391  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_SCH_DETAILS_E              = 392  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_108P_EVM_DETAILS_E          = 393  ,
  WIFISCH_TLV_WRAPPER_E                                    = 394  ,
  WIFISCHEDULER_STATUS_WRAPPER_E                           = 395  ,
  WIFIMPDU_INFO_6X_E                                       = 396  ,
  WIFIMACTX___RESERVED_G_0013                              = 397  ,
  WIFIMACTX_U_SIG_EHT_SU_MU_E                              = 398  ,
  WIFIMACTX_U_SIG_EHT_TB_E                                 = 399  ,
  WIFICOEX_TLV_ACC_TLV_TAG0_CFG_E                          = 400  ,
  WIFICOEX_TLV_ACC_TLV_TAG1_CFG_E                          = 401  ,
  WIFICOEX_TLV_ACC_TLV_TAG2_CFG_E                          = 402  ,
  WIFIPHYRX_U_SIG_EHT_SU_MU_E                              = 403  ,
  WIFIPHYRX_U_SIG_EHT_TB_E                                 = 404  ,
  WIFICOEX_TLV_ACC_TLV_TAG3_CFG_E                          = 405  ,
  WIFICOEX_TLV_ACC_TLV_TAG_CGIM_CFG_E                      = 406  ,
  WIFITX_PUNCTURE_6PATTERNS_SETUP_E                        = 407  ,
  WIFIMACRX_LMR_READ_REQUEST_E                             = 408  ,
  WIFIMACRX_LMR_DATA_REQUEST_E                             = 409  ,
  WIFIPHYRX_LMR_TRANSFER_DONE_E                            = 410  ,
  WIFIPHYRX_LMR_TRANSFER_ABORT_E                           = 411  ,
  WIFIPHYRX_LMR_READ_REQUEST_ACK_E                         = 412  ,
  WIFIMACRX_SECURE_LTF_SEQ_PTR_E                           = 413  ,
  WIFIPHYRX_USER_INFO_MU_UL_E                              = 414  ,
  WIFIMPDU_QUEUE_OVERVIEW_E                                = 415  ,
  WIFISCHEDULER_NAV_INFO_E                                 = 416  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_ENABLE_RX_E                = 417  ,
  WIFILMR_PEER_ENTRY_E                                     = 418  ,
  WIFILMR_MPDU_START_E                                     = 419  ,
  WIFILMR_DATA_E                                           = 420  ,
  WIFILMR_MPDU_END_E                                       = 421  ,
  WIFIREO_GET_QUEUE_1K_STATS_STATUS_E                      = 422  ,
  WIFIRX_FRAME_1K_BITMAP_ACK_E                             = 423  ,
  WIFITX_FES_STATUS_1K_BA_E                                = 424  ,
  WIFITQM_ACKED_1K_MPDU_E                                  = 425  ,
  WIFIMACRX_INBSS_OBSS_IND_E                               = 426  ,
  WIFIPHYRX_LOCATION_E                                     = 427  ,
  WIFIMLO_TX_NOTIFICATION_SU_E                             = 428  ,
  WIFIMLO_TX_NOTIFICATION_MU_E                             = 429  ,
  WIFIMLO_TX_REQ_SU_E                                      = 430  ,
  WIFIMLO_TX_REQ_MU_E                                      = 431  ,
  WIFIMLO_TX_RESP_E                                        = 432  ,
  WIFIMLO_RX_NOTIFICATION_E                                = 433  ,
  WIFIMLO_BKOFF_TRUNC_REQ_E                                = 434  ,
  WIFIMLO_TBTT_NOTIFICATION_E                              = 435  ,
  WIFIMLO_MESSAGE_E                                        = 436  ,
  WIFIMLO_TS_SYNC_MSG_E                                    = 437  ,
  WIFIMLO_FES_SETUP_E                                      = 438  ,
  WIFIMLO_PDG_FES_SETUP_SU_E                               = 439  ,
  WIFIMLO_PDG_FES_SETUP_MU_E                               = 440  ,
  WIFIMPDU_INFO_1K_BITMAP_E                                = 441  ,
  WIFIMON_BUFFER_ADDR_E                                    = 442  ,
  WIFITX_FRAG_STATE_E                                      = 443  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_PHY_CV_RESET_E             = 444  ,
  WIFIMACTX_OTHER_TRANSMIT_INFO_SW_PEER_IDS_E              = 445  ,
  WIFIMACTX_EHT_SIG_USR_OFDMA_E                            = 446  ,
  WIFIPHYRX_EHT_SIG_CMN_PUNC_E                             = 448  ,
  WIFIPHYRX_EHT_SIG_CMN_OFDMA_E                            = 450  ,
  WIFIPHYRX_EHT_SIG_USR_OFDMA_E                            = 454  ,
  WIFIPHYRX_PKT_END_PART1_E                                = 456  ,
  WIFIMACTX_EXPECT_NDP_RECEPTION_E                         = 457  ,
  WIFIMACTX_SECURE_LTF_SEQ_PTR_E                           = 458  ,
  WIFIMLO_PDG_BKOFF_TRUNC_NOTIFY_E                         = 460  ,
  WIFIPHYRX___RESERVED_G_0014                              = 461  ,
  WIFIPHYTX_LOCATION_E                                     = 462  ,
  WIFIPHYTX___RESERVED_G_0014                              = 463  ,
  WIFIMACTX_EHT_SIG_USR_SU_E                               = 466  ,
  WIFIMACTX_EHT_SIG_USR_MU_MIMO_E                          = 467  ,
  WIFIPHYRX_EHT_SIG_USR_SU_E                               = 468  ,
  WIFIPHYRX_EHT_SIG_USR_MU_MIMO_E                          = 469  ,
  WIFIPHYRX_GENERIC_U_SIG_E                                = 470  ,
  WIFIPHYRX_GENERIC_EHT_SIG_E                              = 471  ,
  WIFIOVERWRITE_RESP_START_E                               = 472  ,
  WIFIOVERWRITE_RESP_PREAMBLE_INFO_E                       = 473  ,
  WIFIOVERWRITE_RESP_FRAME_INFO_E                          = 474  ,
  WIFIOVERWRITE_RESP_END_E                                 = 475  ,
  WIFIRXPCU_EARLY_RX_INDICATION_E                          = 476  ,
  WIFIMON_DROP_E                                           = 477  ,
  WIFIMACRX_MU_UPLINK_COMMON_SNIFF_E                       = 478  ,
  WIFIMACRX_MU_UPLINK_USER_SETUP_SNIFF_E                   = 479  ,
  WIFIMACRX_MU_UPLINK_USER_SEL_SNIFF_E                     = 480  ,
  WIFIMACRX_MU_UPLINK_FCS_STATUS_SNIFF_E                   = 481  ,
  WIFIMACTX_PREFETCH_CV_DMA_E                              = 482  ,
  WIFIMACTX_PREFETCH_CV_PER_USER_E                         = 483  ,
  WIFIPHYRX_OTHER_RECEIVE_INFO_ALL_SIGB_DETAILS_E          = 484  ,
  WIFIMACTX_BF_PARAMS_UPDATE_COMMON_E                      = 485  ,
  WIFIMACTX_BF_PARAMS_UPDATE_PER_USER_E                    = 486  ,
  WIFIRANGING_USER_DETAILS_E                               = 487  ,
  WIFIPHYTX_CV_CORR_STATUS_E                               = 488  ,
  WIFIPHYTX_CV_CORR_COMMON_E                               = 489  ,
  WIFIPHYTX_CV_CORR_USER_E                                 = 490  ,
  WIFIMACTX_CV_CORR_COMMON_E                               = 491  ,
  WIFIMACTX_CV_CORR_MAC_INFO_GROUP_E                       = 492  ,
  WIFIBW_PUNCTURE_EVAL_WRAPPER_E                           = 493  ,
  WIFIMACTX_RX_NOTIFICATION_FOR_PHY_E                      = 494  ,
  WIFIMACTX_TX_NOTIFICATION_FOR_PHY_E                      = 495  ,
  WIFIMACTX_MU_UPLINK_COMMON_PER_BW_E                      = 496  ,
  WIFIMACTX_MU_UPLINK_USER_SETUP_PER_BW_E                  = 497  ,
  WIFIRX_PPDU_END_USER_STATS_EXT2_E                        = 498  ,
  WIFIFW2SW_MON_E                                          = 499  ,
  WIFIWSI_DIRECT_MESSAGE_E                                 = 500  ,
  WIFIMACTX_EMLSR_PRE_SWITCH_E                             = 501  ,
  WIFIMACTX_EMLSR_SWITCH_E                                 = 502  ,
  WIFIMACTX_EMLSR_SWITCH_BACK_E                            = 503  ,
  WIFIPHYTX_EMLSR_SWITCH_ACK_E                             = 504  ,
  WIFIPHYTX_EMLSR_SWITCH_BACK_ACK_E                        = 505  ,
  WIFISPARE_REUSE_TAG_0_E                                  = 506  ,
  WIFISPARE_REUSE_TAG_1_E                                  = 507  ,
  WIFISPARE_REUSE_TAG_2_E                                  = 508  ,
  WIFISPARE_REUSE_TAG_3_E                                  = 509
} tlv_tag_def__e;

#endif
