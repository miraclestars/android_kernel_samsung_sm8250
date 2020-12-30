/*
 * Copyright (c) 2016-2017 The Linux Foundation. All rights reserved.
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

#ifndef _REO_FLUSH_TIMEOUT_LIST_STATUS_H_
#define _REO_FLUSH_TIMEOUT_LIST_STATUS_H_
#if !defined(__ASSEMBLER__)
#endif

#include "uniform_reo_status_header.h"

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0-1	struct uniform_reo_status_header status_header;
//	2	error_detected[0], timout_list_empty[1], reserved_2a[31:2]
//	3	release_desc_count[15:0], forward_buf_count[31:16]
//	4	reserved_4a[31:0]
//	5	reserved_5a[31:0]
//	6	reserved_6a[31:0]
//	7	reserved_7a[31:0]
//	8	reserved_8a[31:0]
//	9	reserved_9a[31:0]
//	10	reserved_10a[31:0]
//	11	reserved_11a[31:0]
//	12	reserved_12a[31:0]
//	13	reserved_13a[31:0]
//	14	reserved_14a[31:0]
//	15	reserved_15a[31:0]
//	16	reserved_16a[31:0]
//	17	reserved_17a[31:0]
//	18	reserved_18a[31:0]
//	19	reserved_19a[31:0]
//	20	reserved_20a[31:0]
//	21	reserved_21a[31:0]
//	22	reserved_22a[31:0]
//	23	reserved_23a[31:0]
//	24	reserved_24a[27:0], looping_count[31:28]
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_REO_FLUSH_TIMEOUT_LIST_STATUS 25

struct reo_flush_timeout_list_status {
    struct            uniform_reo_status_header                       status_header;
             uint32_t error_detected                  :  1, //[0]
                      timout_list_empty               :  1, //[1]
                      reserved_2a                     : 30; //[31:2]
             uint32_t release_desc_count              : 16, //[15:0]
                      forward_buf_count               : 16; //[31:16]
             uint32_t reserved_4a                     : 32; //[31:0]
             uint32_t reserved_5a                     : 32; //[31:0]
             uint32_t reserved_6a                     : 32; //[31:0]
             uint32_t reserved_7a                     : 32; //[31:0]
             uint32_t reserved_8a                     : 32; //[31:0]
             uint32_t reserved_9a                     : 32; //[31:0]
             uint32_t reserved_10a                    : 32; //[31:0]
             uint32_t reserved_11a                    : 32; //[31:0]
             uint32_t reserved_12a                    : 32; //[31:0]
             uint32_t reserved_13a                    : 32; //[31:0]
             uint32_t reserved_14a                    : 32; //[31:0]
             uint32_t reserved_15a                    : 32; //[31:0]
             uint32_t reserved_16a                    : 32; //[31:0]
             uint32_t reserved_17a                    : 32; //[31:0]
             uint32_t reserved_18a                    : 32; //[31:0]
             uint32_t reserved_19a                    : 32; //[31:0]
             uint32_t reserved_20a                    : 32; //[31:0]
             uint32_t reserved_21a                    : 32; //[31:0]
             uint32_t reserved_22a                    : 32; //[31:0]
             uint32_t reserved_23a                    : 32; //[31:0]
             uint32_t reserved_24a                    : 28, //[27:0]
                      looping_count                   :  4; //[31:28]
};

/*

struct uniform_reo_status_header status_header
			
			Consumer: SW
			
			Producer: REO
			
			
			
			Details that can link this status with the original
			command. It also contains info on how long REO took to
			execute this command.

error_detected
			
			0: No error has been detected while executing this
			command
			
			1: command not properly executed and returned with an
			error
			
			
			
			NOTE: Current no error is defined, but field is put in
			place to avoid data structure changes in future...

timout_list_empty
			
			When set, REO has depleted the timeout list and all
			entries are gone.
			
			<legal all>

reserved_2a
			
			<legal 0>

release_desc_count
			
			Consumer: REO
			
			Producer: SW
			
			
			
			The number of link descriptors released
			
			<legal all>

forward_buf_count
			
			Consumer: REO
			
			Producer: SW
			
			
			
			The number of buffers forwarded to the REO destination
			rings
			
			<legal all>

reserved_4a
			
			<legal 0>

reserved_5a
			
			<legal 0>

reserved_6a
			
			<legal 0>

reserved_7a
			
			<legal 0>

reserved_8a
			
			<legal 0>

reserved_9a
			
			<legal 0>

reserved_10a
			
			<legal 0>

reserved_11a
			
			<legal 0>

reserved_12a
			
			<legal 0>

reserved_13a
			
			<legal 0>

reserved_14a
			
			<legal 0>

reserved_15a
			
			<legal 0>

reserved_16a
			
			<legal 0>

reserved_17a
			
			<legal 0>

reserved_18a
			
			<legal 0>

reserved_19a
			
			<legal 0>

reserved_20a
			
			<legal 0>

reserved_21a
			
			<legal 0>

reserved_22a
			
			<legal 0>

reserved_23a
			
			<legal 0>

reserved_24a
			
			<legal 0>

looping_count
			
			A count value that indicates the number of times the
			producer of entries into this Ring has looped around the
			ring.
			
			At initialization time, this value is set to 0. On the
			first loop, this value is set to 1. After the max value is
			reached allowed by the number of bits for this field, the
			count value continues with 0 again.
			
			
			
			In case SW is the consumer of the ring entries, it can
			use this field to figure out up to where the producer of
			entries has created new entries. This eliminates the need to
			check where the head pointer' of the ring is located once
			the SW starts processing an interrupt indicating that new
			entries have been put into this ring...
			
			
			
			Also note that SW if it wants only needs to look at the
			LSB bit of this count value.
			
			<legal all>
*/

#define REO_FLUSH_TIMEOUT_LIST_STATUS_0_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_OFFSET 0x00000000
#define REO_FLUSH_TIMEOUT_LIST_STATUS_0_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_LSB 28
#define REO_FLUSH_TIMEOUT_LIST_STATUS_0_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_MASK 0xffffffff
#define REO_FLUSH_TIMEOUT_LIST_STATUS_1_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_OFFSET 0x00000004
#define REO_FLUSH_TIMEOUT_LIST_STATUS_1_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_LSB 28
#define REO_FLUSH_TIMEOUT_LIST_STATUS_1_UNIFORM_REO_STATUS_HEADER_STATUS_HEADER_MASK 0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_2_ERROR_DETECTED
			
			0: No error has been detected while executing this
			command
			
			1: command not properly executed and returned with an
			error
			
			
			
			NOTE: Current no error is defined, but field is put in
			place to avoid data structure changes in future...
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_ERROR_DETECTED_OFFSET        0x00000008
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_ERROR_DETECTED_LSB           0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_ERROR_DETECTED_MASK          0x00000001

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_2_TIMOUT_LIST_EMPTY
			
			When set, REO has depleted the timeout list and all
			entries are gone.
			
			<legal all>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_TIMOUT_LIST_EMPTY_OFFSET     0x00000008
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_TIMOUT_LIST_EMPTY_LSB        1
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_TIMOUT_LIST_EMPTY_MASK       0x00000002

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_2_RESERVED_2A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_RESERVED_2A_OFFSET           0x00000008
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_RESERVED_2A_LSB              2
#define REO_FLUSH_TIMEOUT_LIST_STATUS_2_RESERVED_2A_MASK             0xfffffffc

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_3_RELEASE_DESC_COUNT
			
			Consumer: REO
			
			Producer: SW
			
			
			
			The number of link descriptors released
			
			<legal all>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_RELEASE_DESC_COUNT_OFFSET    0x0000000c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_RELEASE_DESC_COUNT_LSB       0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_RELEASE_DESC_COUNT_MASK      0x0000ffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_3_FORWARD_BUF_COUNT
			
			Consumer: REO
			
			Producer: SW
			
			
			
			The number of buffers forwarded to the REO destination
			rings
			
			<legal all>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_FORWARD_BUF_COUNT_OFFSET     0x0000000c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_FORWARD_BUF_COUNT_LSB        16
#define REO_FLUSH_TIMEOUT_LIST_STATUS_3_FORWARD_BUF_COUNT_MASK       0xffff0000

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_4_RESERVED_4A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_4_RESERVED_4A_OFFSET           0x00000010
#define REO_FLUSH_TIMEOUT_LIST_STATUS_4_RESERVED_4A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_4_RESERVED_4A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_5_RESERVED_5A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_5_RESERVED_5A_OFFSET           0x00000014
#define REO_FLUSH_TIMEOUT_LIST_STATUS_5_RESERVED_5A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_5_RESERVED_5A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_6_RESERVED_6A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_6_RESERVED_6A_OFFSET           0x00000018
#define REO_FLUSH_TIMEOUT_LIST_STATUS_6_RESERVED_6A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_6_RESERVED_6A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_7_RESERVED_7A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_7_RESERVED_7A_OFFSET           0x0000001c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_7_RESERVED_7A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_7_RESERVED_7A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_8_RESERVED_8A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_8_RESERVED_8A_OFFSET           0x00000020
#define REO_FLUSH_TIMEOUT_LIST_STATUS_8_RESERVED_8A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_8_RESERVED_8A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_9_RESERVED_9A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_9_RESERVED_9A_OFFSET           0x00000024
#define REO_FLUSH_TIMEOUT_LIST_STATUS_9_RESERVED_9A_LSB              0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_9_RESERVED_9A_MASK             0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_10_RESERVED_10A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_10_RESERVED_10A_OFFSET         0x00000028
#define REO_FLUSH_TIMEOUT_LIST_STATUS_10_RESERVED_10A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_10_RESERVED_10A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_11_RESERVED_11A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_11_RESERVED_11A_OFFSET         0x0000002c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_11_RESERVED_11A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_11_RESERVED_11A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_12_RESERVED_12A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_12_RESERVED_12A_OFFSET         0x00000030
#define REO_FLUSH_TIMEOUT_LIST_STATUS_12_RESERVED_12A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_12_RESERVED_12A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_13_RESERVED_13A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_13_RESERVED_13A_OFFSET         0x00000034
#define REO_FLUSH_TIMEOUT_LIST_STATUS_13_RESERVED_13A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_13_RESERVED_13A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_14_RESERVED_14A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_14_RESERVED_14A_OFFSET         0x00000038
#define REO_FLUSH_TIMEOUT_LIST_STATUS_14_RESERVED_14A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_14_RESERVED_14A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_15_RESERVED_15A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_15_RESERVED_15A_OFFSET         0x0000003c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_15_RESERVED_15A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_15_RESERVED_15A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_16_RESERVED_16A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_16_RESERVED_16A_OFFSET         0x00000040
#define REO_FLUSH_TIMEOUT_LIST_STATUS_16_RESERVED_16A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_16_RESERVED_16A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_17_RESERVED_17A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_17_RESERVED_17A_OFFSET         0x00000044
#define REO_FLUSH_TIMEOUT_LIST_STATUS_17_RESERVED_17A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_17_RESERVED_17A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_18_RESERVED_18A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_18_RESERVED_18A_OFFSET         0x00000048
#define REO_FLUSH_TIMEOUT_LIST_STATUS_18_RESERVED_18A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_18_RESERVED_18A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_19_RESERVED_19A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_19_RESERVED_19A_OFFSET         0x0000004c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_19_RESERVED_19A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_19_RESERVED_19A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_20_RESERVED_20A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_20_RESERVED_20A_OFFSET         0x00000050
#define REO_FLUSH_TIMEOUT_LIST_STATUS_20_RESERVED_20A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_20_RESERVED_20A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_21_RESERVED_21A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_21_RESERVED_21A_OFFSET         0x00000054
#define REO_FLUSH_TIMEOUT_LIST_STATUS_21_RESERVED_21A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_21_RESERVED_21A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_22_RESERVED_22A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_22_RESERVED_22A_OFFSET         0x00000058
#define REO_FLUSH_TIMEOUT_LIST_STATUS_22_RESERVED_22A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_22_RESERVED_22A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_23_RESERVED_23A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_23_RESERVED_23A_OFFSET         0x0000005c
#define REO_FLUSH_TIMEOUT_LIST_STATUS_23_RESERVED_23A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_23_RESERVED_23A_MASK           0xffffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_24_RESERVED_24A
			
			<legal 0>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_RESERVED_24A_OFFSET         0x00000060
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_RESERVED_24A_LSB            0
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_RESERVED_24A_MASK           0x0fffffff

/* Description		REO_FLUSH_TIMEOUT_LIST_STATUS_24_LOOPING_COUNT
			
			A count value that indicates the number of times the
			producer of entries into this Ring has looped around the
			ring.
			
			At initialization time, this value is set to 0. On the
			first loop, this value is set to 1. After the max value is
			reached allowed by the number of bits for this field, the
			count value continues with 0 again.
			
			
			
			In case SW is the consumer of the ring entries, it can
			use this field to figure out up to where the producer of
			entries has created new entries. This eliminates the need to
			check where the head pointer' of the ring is located once
			the SW starts processing an interrupt indicating that new
			entries have been put into this ring...
			
			
			
			Also note that SW if it wants only needs to look at the
			LSB bit of this count value.
			
			<legal all>
*/
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_LOOPING_COUNT_OFFSET        0x00000060
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_LOOPING_COUNT_LSB           28
#define REO_FLUSH_TIMEOUT_LIST_STATUS_24_LOOPING_COUNT_MASK          0xf0000000


#endif // _REO_FLUSH_TIMEOUT_LIST_STATUS_H_
