
/*
 * Copyright (c) 2021 Qualcomm Innovation Center, Inc. All rights reserved.
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











#ifndef _PHYRX_VHT_SIG_A_H_
#define _PHYRX_VHT_SIG_A_H_
#if !defined(__ASSEMBLER__)
#endif

#include "vht_sig_a_info.h"
#define NUM_OF_DWORDS_PHYRX_VHT_SIG_A 2

#define NUM_OF_QWORDS_PHYRX_VHT_SIG_A 1


struct phyrx_vht_sig_a {
	     struct   vht_sig_a_info                                            phyrx_vht_sig_a_info_details;
};







#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BANDWIDTH_OFFSET               0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BANDWIDTH_LSB                  0
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BANDWIDTH_MSB                  1
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BANDWIDTH_MASK                 0x0000000000000003




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0_OFFSET         0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0_LSB            2
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0_MSB            2
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0_MASK           0x0000000000000004




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_STBC_OFFSET                    0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_STBC_LSB                       3
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_STBC_MSB                       3
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_STBC_MASK                      0x0000000000000008




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GROUP_ID_OFFSET                0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GROUP_ID_LSB                   4
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GROUP_ID_MSB                   9
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GROUP_ID_MASK                  0x00000000000003f0




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_N_STS_OFFSET                   0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_N_STS_LSB                      10
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_N_STS_MSB                      21
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_N_STS_MASK                     0x00000000003ffc00




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TXOP_PS_NOT_ALLOWED_OFFSET     0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TXOP_PS_NOT_ALLOWED_LSB        22
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TXOP_PS_NOT_ALLOWED_MSB        22
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TXOP_PS_NOT_ALLOWED_MASK       0x0000000000400000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0B_OFFSET        0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0B_LSB           23
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0B_MSB           23
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_0B_MASK          0x0000000000800000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_0_OFFSET              0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_0_LSB                 24
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_0_MSB                 31
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_0_MASK                0x00000000ff000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GI_SETTING_OFFSET              0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GI_SETTING_LSB                 32
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GI_SETTING_MSB                 33
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_GI_SETTING_MASK                0x0000000300000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_SU_MU_CODING_OFFSET            0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_SU_MU_CODING_LSB               34
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_SU_MU_CODING_MSB               34
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_SU_MU_CODING_MASK              0x0000000400000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_LDPC_EXTRA_SYMBOL_OFFSET       0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_LDPC_EXTRA_SYMBOL_LSB          35
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_LDPC_EXTRA_SYMBOL_MSB          35
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_LDPC_EXTRA_SYMBOL_MASK         0x0000000800000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_MCS_OFFSET                     0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_MCS_LSB                        36
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_MCS_MSB                        39
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_MCS_MASK                       0x000000f000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BEAMFORMED_OFFSET              0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BEAMFORMED_LSB                 40
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BEAMFORMED_MSB                 40
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_BEAMFORMED_MASK                0x0000010000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_1_OFFSET         0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_1_LSB            41
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_1_MSB            41
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_VHTA_RESERVED_1_MASK           0x0000020000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_CRC_OFFSET                     0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_CRC_LSB                        42
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_CRC_MSB                        49
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_CRC_MASK                       0x0003fc0000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TAIL_OFFSET                    0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TAIL_LSB                       50
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TAIL_MSB                       55
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_TAIL_MASK                      0x00fc000000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_1_OFFSET              0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_1_LSB                 56
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_1_MSB                 62
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RESERVED_1_MASK                0x7f00000000000000




#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_OFFSET 0x0000000000000000
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_LSB  63
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MSB  63
#define PHYRX_VHT_SIG_A_PHYRX_VHT_SIG_A_INFO_DETAILS_RX_INTEGRITY_CHECK_PASSED_MASK 0x8000000000000000



#endif
