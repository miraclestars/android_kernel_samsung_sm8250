/*
 * Copyright (c) 2019 The Linux Foundation. All rights reserved.
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

#ifndef _PHYRX_HE_SIG_B1_MU_H_
#define _PHYRX_HE_SIG_B1_MU_H_
#if !defined(__ASSEMBLER__)
#endif

#include "he_sig_b1_mu_info.h"

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0	struct he_sig_b1_mu_info phyrx_he_sig_b1_mu_info_details;
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_PHYRX_HE_SIG_B1_MU 1

struct phyrx_he_sig_b1_mu {
    struct            he_sig_b1_mu_info                       phyrx_he_sig_b1_mu_info_details;
};

/*

struct he_sig_b1_mu_info phyrx_he_sig_b1_mu_info_details

			See detailed description of the STRUCT
*/


 /* EXTERNAL REFERENCE : struct he_sig_b1_mu_info phyrx_he_sig_b1_mu_info_details */


/* Description		PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RU_ALLOCATION

			RU allocation for the user(s) following this common
			portion of the SIG



			For details, refer to  RU_TYPE description

			<legal all>
*/
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RU_ALLOCATION_OFFSET 0x00000000
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RU_ALLOCATION_LSB 0
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RU_ALLOCATION_MASK 0x000000ff

/* Description		PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RESERVED_0

			<legal 0>
*/
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RESERVED_0_OFFSET 0x00000000
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RESERVED_0_LSB 8
#define PHYRX_HE_SIG_B1_MU_0_PHYRX_HE_SIG_B1_MU_INFO_DETAILS_RESERVED_0_MASK 0xffffff00


#endif // _PHYRX_HE_SIG_B1_MU_H_
