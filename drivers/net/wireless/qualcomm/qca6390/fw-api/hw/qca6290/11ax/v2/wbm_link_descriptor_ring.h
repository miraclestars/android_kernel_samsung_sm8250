/*
 * Copyright (c) 2016-2018 The Linux Foundation. All rights reserved.
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

#ifndef _WBM_LINK_DESCRIPTOR_RING_H_
#define _WBM_LINK_DESCRIPTOR_RING_H_
#if !defined(__ASSEMBLER__)
#endif

#include "buffer_addr_info.h"

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0-1	struct buffer_addr_info desc_addr_info;
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_WBM_LINK_DESCRIPTOR_RING 2

struct wbm_link_descriptor_ring {
    struct            buffer_addr_info                       desc_addr_info;
};

/*

struct buffer_addr_info desc_addr_info
			
			Consumer: WBM
			
			Producer: WBM
			
			
			
			Details of the physical address of the buffer + source
			buffer owner +  some SW meta data
			
			All modules getting this link descriptor address info,
			shall keep all the 64 bits in this descriptor together and
			eventually all 64 bits shall be given back to WBM when the
			link descriptor is released.
*/

#define WBM_LINK_DESCRIPTOR_RING_0_BUFFER_ADDR_INFO_DESC_ADDR_INFO_OFFSET 0x00000000
#define WBM_LINK_DESCRIPTOR_RING_0_BUFFER_ADDR_INFO_DESC_ADDR_INFO_LSB 0
#define WBM_LINK_DESCRIPTOR_RING_0_BUFFER_ADDR_INFO_DESC_ADDR_INFO_MASK 0xffffffff
#define WBM_LINK_DESCRIPTOR_RING_1_BUFFER_ADDR_INFO_DESC_ADDR_INFO_OFFSET 0x00000004
#define WBM_LINK_DESCRIPTOR_RING_1_BUFFER_ADDR_INFO_DESC_ADDR_INFO_LSB 0
#define WBM_LINK_DESCRIPTOR_RING_1_BUFFER_ADDR_INFO_DESC_ADDR_INFO_MASK 0xffffffff


#endif // _WBM_LINK_DESCRIPTOR_RING_H_
