
/* Copyright (c) 2022, Qualcomm Innovation Center, Inc. All rights reserved.
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

 
 
 
 
 
 
 


#ifndef _MACTX_VHT_SIG_B_MU40_H_
#define _MACTX_VHT_SIG_B_MU40_H_
#if !defined(__ASSEMBLER__)
#endif

#include "vht_sig_b_mu40_info.h"
#define NUM_OF_DWORDS_MACTX_VHT_SIG_B_MU40 2

#define NUM_OF_QWORDS_MACTX_VHT_SIG_B_MU40 1


struct mactx_vht_sig_b_mu40 {
#ifndef WIFI_BIT_ORDER_BIG_ENDIAN
             struct   vht_sig_b_mu40_info                                       mactx_vht_sig_b_mu40_info_details;
#else
             struct   vht_sig_b_mu40_info                                       mactx_vht_sig_b_mu40_info_details;
#endif
};


/* Description		MACTX_VHT_SIG_B_MU40_INFO_DETAILS

			See detailed description of the STRUCT
*/


/* Description		LENGTH

			VHT-SIG-B Length (in units of 4 octets) = ceiling (LENGTH/4) <legal
			 all>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_OFFSET        0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_LSB           0
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_MSB           16
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_MASK          0x000000000001ffff


/* Description		MCS

			Modulation as described in vht_sig_a mcs field 
			<legal 0-11>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_OFFSET           0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_LSB              17
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_MSB              20
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_MASK             0x00000000001e0000


/* Description		TAIL

			Used to terminate the trellis of the convolutional decoder.
			
			Set to 0.  
			<legal 0>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_OFFSET          0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_LSB             21
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_MSB             26
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_MASK            0x0000000007e00000


/* Description		RESERVED_0

			Not part of VHT-SIG-B.
			Reserved: Set to 0 and ignored on receive  <legal 0>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_0_OFFSET    0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_0_LSB       27
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_0_MSB       28
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_0_MASK      0x0000000018000000


/* Description		MU_USER_NUMBER

			Not part of VHT-SIG-B.
			Mapping from user number (BFer hardware specific) to mu_user_number. 
			The reader is directed to the previous chapter (User Number) 
			for a definition of the terms user and mu_user.   <legal
			 0-3>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MU_USER_NUMBER_OFFSET 0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MU_USER_NUMBER_LSB   29
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MU_USER_NUMBER_MSB   31
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MU_USER_NUMBER_MASK  0x00000000e0000000


/* Description		LENGTH_COPY

			Same as "length". This field is not valid for RX packets. <legal
			 all>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_COPY_OFFSET   0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_COPY_LSB      32
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_COPY_MSB      48
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_LENGTH_COPY_MASK     0x0001ffff00000000


/* Description		MCS_COPY

			Same as "mcs". This field is not valid for RX packets. <legal
			 0-11>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_COPY_OFFSET      0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_COPY_LSB         49
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_COPY_MSB         52
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_MCS_COPY_MASK        0x001e000000000000


/* Description		TAIL_COPY

			Same as "tail". This field is not valid for RX packets. 
			 <legal 0>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_COPY_OFFSET     0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_COPY_LSB        53
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_COPY_MSB        58
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_TAIL_COPY_MASK       0x07e0000000000000


/* Description		RESERVED_1

			<legal 0>
*/

#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_1_OFFSET    0x0000000000000000
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_1_LSB       59
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_1_MSB       63
#define MACTX_VHT_SIG_B_MU40_MACTX_VHT_SIG_B_MU40_INFO_DETAILS_RESERVED_1_MASK      0xf800000000000000



#endif   // MACTX_VHT_SIG_B_MU40
