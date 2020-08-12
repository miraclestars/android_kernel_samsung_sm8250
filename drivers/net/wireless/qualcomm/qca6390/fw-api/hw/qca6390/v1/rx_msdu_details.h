/*
 * Copyright (c) 2018 The Linux Foundation. All rights reserved.
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

#ifndef _RX_MSDU_DETAILS_H_
#define _RX_MSDU_DETAILS_H_
#if !defined(__ASSEMBLER__)
#endif

#include "buffer_addr_info.h"
#include "rx_msdu_desc_info.h"

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0-1	struct buffer_addr_info buffer_addr_info_details;
//	2-3	struct rx_msdu_desc_info rx_msdu_desc_info_details;
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_RX_MSDU_DETAILS 4

struct rx_msdu_details {
    struct            buffer_addr_info                       buffer_addr_info_details;
    struct            rx_msdu_desc_info                       rx_msdu_desc_info_details;
};

/*

struct buffer_addr_info buffer_addr_info_details
			
			Consumer: REO/SW
			
			Producer: RXDMA
			
			
			
			Details of the physical address of the buffer containing
			an MSDU (or entire MPDU)

struct rx_msdu_desc_info rx_msdu_desc_info_details
			
			Consumer: REO/SW
			
			Producer: RXDMA
			
			
			
			General information related to the MSDU that should be
			passed on from RXDMA all the way to to the REO destination
			ring.
*/

#define RX_MSDU_DETAILS_0_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_OFFSET 0x00000000
#define RX_MSDU_DETAILS_0_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_LSB 0
#define RX_MSDU_DETAILS_0_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_MASK 0xffffffff
#define RX_MSDU_DETAILS_1_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_OFFSET 0x00000004
#define RX_MSDU_DETAILS_1_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_LSB 0
#define RX_MSDU_DETAILS_1_BUFFER_ADDR_INFO_BUFFER_ADDR_INFO_DETAILS_MASK 0xffffffff
#define RX_MSDU_DETAILS_2_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_OFFSET 0x00000008
#define RX_MSDU_DETAILS_2_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_LSB 0
#define RX_MSDU_DETAILS_2_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_MASK 0xffffffff
#define RX_MSDU_DETAILS_3_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_OFFSET 0x0000000c
#define RX_MSDU_DETAILS_3_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_LSB 0
#define RX_MSDU_DETAILS_3_RX_MSDU_DESC_INFO_RX_MSDU_DESC_INFO_DETAILS_MASK 0xffffffff


#endif // _RX_MSDU_DETAILS_H_
