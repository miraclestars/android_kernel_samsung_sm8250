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

#ifndef _REO_ENTRANCE_RING_H_
#define _REO_ENTRANCE_RING_H_
#if !defined(__ASSEMBLER__)
#endif

#include "rx_mpdu_details.h"

// ################ START SUMMARY #################
//
//	Dword	Fields
//	0-3	struct rx_mpdu_details reo_level_mpdu_frame_info;
//	4	rx_reo_queue_desc_addr_31_0[31:0]
//	5	rx_reo_queue_desc_addr_39_32[7:0], rounded_mpdu_byte_count[21:8], reo_destination_indication[26:22], frameless_bar[27], reserved_5a[31:28]
//	6	rxdma_push_reason[1:0], rxdma_error_code[6:2], mpdu_fragment_number[10:7], reserved_6a[31:11]
//	7	reserved_7a[19:0], ring_id[27:20], looping_count[31:28]
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_REO_ENTRANCE_RING 8

struct reo_entrance_ring {
    struct            rx_mpdu_details                       reo_level_mpdu_frame_info;
             uint32_t rx_reo_queue_desc_addr_31_0     : 32; //[31:0]
             uint32_t rx_reo_queue_desc_addr_39_32    :  8, //[7:0]
                      rounded_mpdu_byte_count         : 14, //[21:8]
                      reo_destination_indication      :  5, //[26:22]
                      frameless_bar                   :  1, //[27]
                      reserved_5a                     :  4; //[31:28]
             uint32_t rxdma_push_reason               :  2, //[1:0]
                      rxdma_error_code                :  5, //[6:2]
                      mpdu_fragment_number            :  4, //[10:7]
                      reserved_6a                     : 21; //[31:11]
             uint32_t reserved_7a                     : 20, //[19:0]
                      ring_id                         :  8, //[27:20]
                      looping_count                   :  4; //[31:28]
};

/*

struct rx_mpdu_details reo_level_mpdu_frame_info
			
			Consumer: REO
			
			Producer: RXDMA
			
			
			
			Details related to the MPDU being pushed into the REO

rx_reo_queue_desc_addr_31_0
			
			Consumer: REO
			
			Producer: RXDMA
			
			
			
			Address (lower 32 bits) of the REO queue descriptor. 
			
			<legal all>

rx_reo_queue_desc_addr_39_32
			
			Consumer: REO
			
			Producer: RXDMA
			
			
			
			Address (upper 8 bits) of the REO queue descriptor. 
			
			<legal all>

rounded_mpdu_byte_count
			
			An approximation of the number of bytes received in this
			MPDU. 
			
			Used to keeps stats on the amount of data flowing
			through a queue.
			
			<legal all>

reo_destination_indication
			
			RXDMA copy the MPDU's first MSDU's destination
			indication field here. This is used for REO to be able to
			re-route the packet to a different SW destination ring if
			the packet is detected as error in REO.
			
			
			
			The ID of the REO exit ring where the MSDU frame shall
			push after (MPDU level) reordering has finished.
			
			
			
			<enum 0 reo_destination_tcl> Reo will push the frame
			into the REO2TCL ring
			
			<enum 1 reo_destination_sw1> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 2 reo_destination_sw2> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 3 reo_destination_sw3> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 4 reo_destination_sw4> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 5 reo_destination_release> Reo will push the frame
			into the REO_release ring
			
			<enum 6 reo_destination_fw> Reo will push the frame into
			the REO2FW ring
			
			<enum 7 reo_destination_7> REO remaps this
			
			<enum 8 reo_destination_8> REO remaps this <enum 9
			reo_destination_9> REO remaps this <enum 10
			reo_destination_10> REO remaps this 
			
			<enum 11 reo_destination_11> REO remaps this 
			
			<enum 12 reo_destination_12> REO remaps this <enum 13
			reo_destination_13> REO remaps this 
			
			<enum 14 reo_destination_14> REO remaps this 
			
			<enum 15 reo_destination_15> REO remaps this 
			
			<enum 16 reo_destination_16> REO remaps this 
			
			<enum 17 reo_destination_17> REO remaps this 
			
			<enum 18 reo_destination_18> REO remaps this 
			
			<enum 19 reo_destination_19> REO remaps this 
			
			<enum 20 reo_destination_20> REO remaps this 
			
			<enum 21 reo_destination_21> REO remaps this 
			
			<enum 22 reo_destination_22> REO remaps this 
			
			<enum 23 reo_destination_23> REO remaps this 
			
			<enum 24 reo_destination_24> REO remaps this 
			
			<enum 25 reo_destination_25> REO remaps this 
			
			<enum 26 reo_destination_26> REO remaps this 
			
			<enum 27 reo_destination_27> REO remaps this 
			
			<enum 28 reo_destination_28> REO remaps this 
			
			<enum 29 reo_destination_29> REO remaps this 
			
			<enum 30 reo_destination_30> REO remaps this 
			
			<enum 31 reo_destination_31> REO remaps this 
			
			
			
			<legal all>

frameless_bar
			
			When set, this REO entrance ring struct contains BAR
			info from a multi TID BAR frame. The original multi TID BAR
			frame itself contained all the REO info for the first TID,
			but all the subsequent TID info and their linkage to the REO
			descriptors is passed down as 'frameless' BAR info.
			
			
			
			The only fields valid in this descriptor when this bit
			is set are:
			
			Rx_reo_queue_desc_addr_31_0
			
			RX_reo_queue_desc_addr_39_32
			
			
			
			And within the
			
			Reo_level_mpdu_frame_info:    
			
			   Within Rx_mpdu_desc_info_details:
			
			Mpdu_Sequence_number
			
			BAR_frame
			
			Peer_meta_data
			
			All other fields shall be set to 0
			
			
			
			<legal all>

reserved_5a
			
			<legal 0>

rxdma_push_reason
			
			Indicates why rxdma pushed the frame to this ring
			
			
			
			This field is ignored by REO. 
			
			
			
			<enum 0 rxdma_error_detected> RXDMA detected an error an
			pushed this frame to this queue
			
			<enum 1 rxdma_routing_instruction> RXDMA pushed the
			frame to this queue per received routing instructions. No
			error within RXDMA was detected
			
			<enum 2 rxdma_rx_flush> RXDMA received an RX_FLUSH. As a
			result the MSDU link descriptor might not have the
			last_msdu_in_mpdu_flag set, but instead WBM might just see a
			NULL pointer in the MSDU link descriptor. This is to be
			considered a normal condition for this scenario.
			
			
			
			<legal 0 - 2>

rxdma_error_code
			
			Field only valid when 'rxdma_push_reason' set to
			'rxdma_error_detected'.
			
			
			
			This field is ignored by REO.
			
			
			
			<enum 0 rxdma_overflow_err>MPDU frame is not complete
			due to a FIFO overflow error in RXPCU.
			
			<enum 1 rxdma_mpdu_length_err>MPDU frame is not complete
			due to receiving incomplete MPDU from the PHY
			
			
			<enum 3 rxdma_decrypt_err>CRYPTO reported a decryption
			error or CRYPTO received an encrypted frame, but did not get
			a valid corresponding key id in the peer entry.
			
			<enum 4 rxdma_tkip_mic_err>CRYPTO reported a TKIP MIC
			error
			
			<enum 5 rxdma_unecrypted_err>CRYPTO reported an
			unencrypted frame error when encrypted was expected
			
			<enum 6 rxdma_msdu_len_err>RX OLE reported an MSDU
			length error
			
			<enum 7 rxdma_msdu_limit_err>RX OLE reported that max
			number of MSDUs allowed in an MPDU got exceeded
			
			<enum 8 rxdma_wifi_parse_err>RX OLE reported a parsing
			error
			
			<enum 9 rxdma_amsdu_parse_err>RX OLE reported an A-MSDU
			parsing error
			
			<enum 10 rxdma_sa_timeout_err>RX OLE reported a timeout
			during SA search
			
			<enum 11 rxdma_da_timeout_err>RX OLE reported a timeout
			during DA search
			
			<enum 12 rxdma_flow_timeout_err>RX OLE reported a
			timeout during flow search
			
			<enum 13 rxdma_flush_request>RXDMA received a flush
			request
			
			<enum 14 rxdma_amsdu_fragment_err>Rx PCU reported A-MSDU
			present as well as a fragmented MPDU. A-MSDU defragmentation
			is not supported in Lithium SW so this is treated as an
			error.

mpdu_fragment_number
			
			Field only valid when Reo_level_mpdu_frame_info.
			Rx_mpdu_desc_info_details.Fragment_flag is set.
			
			
			
			The fragment number from the 802.11 header.
			
			
			
			Note that the sequence number is embedded in the field:
			Reo_level_mpdu_frame_info. Rx_mpdu_desc_info_details.
			Mpdu_sequence_number
			
			
			
			<legal all>

reserved_6a
			
			<legal 0>

reserved_7a
			
			<legal 0>

ring_id
			
			Consumer: SW/REO/DEBUG
			
			Producer: SRNG (of RXDMA)
			
			
			
			For debugging. 
			
			This field is filled in by the SRNG module.
			
			It help to identify the ring that is being looked <legal
			all>

looping_count
			
			Consumer: SW/REO/DEBUG
			
			Producer: SRNG (of RXDMA)
			
			
			
			For debugging. 
			
			This field is filled in by the SRNG module.
			
			
			
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

#define REO_ENTRANCE_RING_0_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_OFFSET 0x00000000
#define REO_ENTRANCE_RING_0_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_LSB 28
#define REO_ENTRANCE_RING_0_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_MASK 0xffffffff
#define REO_ENTRANCE_RING_1_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_OFFSET 0x00000004
#define REO_ENTRANCE_RING_1_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_LSB 28
#define REO_ENTRANCE_RING_1_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_MASK 0xffffffff
#define REO_ENTRANCE_RING_2_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_OFFSET 0x00000008
#define REO_ENTRANCE_RING_2_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_LSB 28
#define REO_ENTRANCE_RING_2_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_MASK 0xffffffff
#define REO_ENTRANCE_RING_3_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_OFFSET 0x0000000c
#define REO_ENTRANCE_RING_3_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_LSB 28
#define REO_ENTRANCE_RING_3_RX_MPDU_DETAILS_REO_LEVEL_MPDU_FRAME_INFO_MASK 0xffffffff

/* Description		REO_ENTRANCE_RING_4_RX_REO_QUEUE_DESC_ADDR_31_0
			
			Consumer: REO
			
			Producer: RXDMA
			
			
			
			Address (lower 32 bits) of the REO queue descriptor. 
			
			<legal all>
*/
#define REO_ENTRANCE_RING_4_RX_REO_QUEUE_DESC_ADDR_31_0_OFFSET       0x00000010
#define REO_ENTRANCE_RING_4_RX_REO_QUEUE_DESC_ADDR_31_0_LSB          0
#define REO_ENTRANCE_RING_4_RX_REO_QUEUE_DESC_ADDR_31_0_MASK         0xffffffff

/* Description		REO_ENTRANCE_RING_5_RX_REO_QUEUE_DESC_ADDR_39_32
			
			Consumer: REO
			
			Producer: RXDMA
			
			
			
			Address (upper 8 bits) of the REO queue descriptor. 
			
			<legal all>
*/
#define REO_ENTRANCE_RING_5_RX_REO_QUEUE_DESC_ADDR_39_32_OFFSET      0x00000014
#define REO_ENTRANCE_RING_5_RX_REO_QUEUE_DESC_ADDR_39_32_LSB         0
#define REO_ENTRANCE_RING_5_RX_REO_QUEUE_DESC_ADDR_39_32_MASK        0x000000ff

/* Description		REO_ENTRANCE_RING_5_ROUNDED_MPDU_BYTE_COUNT
			
			An approximation of the number of bytes received in this
			MPDU. 
			
			Used to keeps stats on the amount of data flowing
			through a queue.
			
			<legal all>
*/
#define REO_ENTRANCE_RING_5_ROUNDED_MPDU_BYTE_COUNT_OFFSET           0x00000014
#define REO_ENTRANCE_RING_5_ROUNDED_MPDU_BYTE_COUNT_LSB              8
#define REO_ENTRANCE_RING_5_ROUNDED_MPDU_BYTE_COUNT_MASK             0x003fff00

/* Description		REO_ENTRANCE_RING_5_REO_DESTINATION_INDICATION
			
			RXDMA copy the MPDU's first MSDU's destination
			indication field here. This is used for REO to be able to
			re-route the packet to a different SW destination ring if
			the packet is detected as error in REO.
			
			
			
			The ID of the REO exit ring where the MSDU frame shall
			push after (MPDU level) reordering has finished.
			
			
			
			<enum 0 reo_destination_tcl> Reo will push the frame
			into the REO2TCL ring
			
			<enum 1 reo_destination_sw1> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 2 reo_destination_sw2> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 3 reo_destination_sw3> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 4 reo_destination_sw4> Reo will push the frame
			into the REO2SW1 ring
			
			<enum 5 reo_destination_release> Reo will push the frame
			into the REO_release ring
			
			<enum 6 reo_destination_fw> Reo will push the frame into
			the REO2FW ring
			
			<enum 7 reo_destination_7> REO remaps this
			
			<enum 8 reo_destination_8> REO remaps this <enum 9
			reo_destination_9> REO remaps this <enum 10
			reo_destination_10> REO remaps this 
			
			<enum 11 reo_destination_11> REO remaps this 
			
			<enum 12 reo_destination_12> REO remaps this <enum 13
			reo_destination_13> REO remaps this 
			
			<enum 14 reo_destination_14> REO remaps this 
			
			<enum 15 reo_destination_15> REO remaps this 
			
			<enum 16 reo_destination_16> REO remaps this 
			
			<enum 17 reo_destination_17> REO remaps this 
			
			<enum 18 reo_destination_18> REO remaps this 
			
			<enum 19 reo_destination_19> REO remaps this 
			
			<enum 20 reo_destination_20> REO remaps this 
			
			<enum 21 reo_destination_21> REO remaps this 
			
			<enum 22 reo_destination_22> REO remaps this 
			
			<enum 23 reo_destination_23> REO remaps this 
			
			<enum 24 reo_destination_24> REO remaps this 
			
			<enum 25 reo_destination_25> REO remaps this 
			
			<enum 26 reo_destination_26> REO remaps this 
			
			<enum 27 reo_destination_27> REO remaps this 
			
			<enum 28 reo_destination_28> REO remaps this 
			
			<enum 29 reo_destination_29> REO remaps this 
			
			<enum 30 reo_destination_30> REO remaps this 
			
			<enum 31 reo_destination_31> REO remaps this 
			
			
			
			<legal all>
*/
#define REO_ENTRANCE_RING_5_REO_DESTINATION_INDICATION_OFFSET        0x00000014
#define REO_ENTRANCE_RING_5_REO_DESTINATION_INDICATION_LSB           22
#define REO_ENTRANCE_RING_5_REO_DESTINATION_INDICATION_MASK          0x07c00000

/* Description		REO_ENTRANCE_RING_5_FRAMELESS_BAR
			
			When set, this REO entrance ring struct contains BAR
			info from a multi TID BAR frame. The original multi TID BAR
			frame itself contained all the REO info for the first TID,
			but all the subsequent TID info and their linkage to the REO
			descriptors is passed down as 'frameless' BAR info.
			
			
			
			The only fields valid in this descriptor when this bit
			is set are:
			
			Rx_reo_queue_desc_addr_31_0
			
			RX_reo_queue_desc_addr_39_32
			
			
			
			And within the
			
			Reo_level_mpdu_frame_info:    
			
			   Within Rx_mpdu_desc_info_details:
			
			Mpdu_Sequence_number
			
			BAR_frame
			
			Peer_meta_data
			
			All other fields shall be set to 0
			
			
			
			<legal all>
*/
#define REO_ENTRANCE_RING_5_FRAMELESS_BAR_OFFSET                     0x00000014
#define REO_ENTRANCE_RING_5_FRAMELESS_BAR_LSB                        27
#define REO_ENTRANCE_RING_5_FRAMELESS_BAR_MASK                       0x08000000

/* Description		REO_ENTRANCE_RING_5_RESERVED_5A
			
			<legal 0>
*/
#define REO_ENTRANCE_RING_5_RESERVED_5A_OFFSET                       0x00000014
#define REO_ENTRANCE_RING_5_RESERVED_5A_LSB                          28
#define REO_ENTRANCE_RING_5_RESERVED_5A_MASK                         0xf0000000

/* Description		REO_ENTRANCE_RING_6_RXDMA_PUSH_REASON
			
			Indicates why rxdma pushed the frame to this ring
			
			
			
			This field is ignored by REO. 
			
			
			
			<enum 0 rxdma_error_detected> RXDMA detected an error an
			pushed this frame to this queue
			
			<enum 1 rxdma_routing_instruction> RXDMA pushed the
			frame to this queue per received routing instructions. No
			error within RXDMA was detected
			
			<enum 2 rxdma_rx_flush> RXDMA received an RX_FLUSH. As a
			result the MSDU link descriptor might not have the
			last_msdu_in_mpdu_flag set, but instead WBM might just see a
			NULL pointer in the MSDU link descriptor. This is to be
			considered a normal condition for this scenario.
			
			
			
			<legal 0 - 2>
*/
#define REO_ENTRANCE_RING_6_RXDMA_PUSH_REASON_OFFSET                 0x00000018
#define REO_ENTRANCE_RING_6_RXDMA_PUSH_REASON_LSB                    0
#define REO_ENTRANCE_RING_6_RXDMA_PUSH_REASON_MASK                   0x00000003

/* Description		REO_ENTRANCE_RING_6_RXDMA_ERROR_CODE
			
			Field only valid when 'rxdma_push_reason' set to
			'rxdma_error_detected'.
			
			
			
			This field is ignored by REO.
			
			
			
			<enum 0 rxdma_overflow_err>MPDU frame is not complete
			due to a FIFO overflow error in RXPCU.
			
			<enum 1 rxdma_mpdu_length_err>MPDU frame is not complete
			due to receiving incomplete MPDU from the PHY
			
			
			<enum 3 rxdma_decrypt_err>CRYPTO reported a decryption
			error or CRYPTO received an encrypted frame, but did not get
			a valid corresponding key id in the peer entry.
			
			<enum 4 rxdma_tkip_mic_err>CRYPTO reported a TKIP MIC
			error
			
			<enum 5 rxdma_unecrypted_err>CRYPTO reported an
			unencrypted frame error when encrypted was expected
			
			<enum 6 rxdma_msdu_len_err>RX OLE reported an MSDU
			length error
			
			<enum 7 rxdma_msdu_limit_err>RX OLE reported that max
			number of MSDUs allowed in an MPDU got exceeded
			
			<enum 8 rxdma_wifi_parse_err>RX OLE reported a parsing
			error
			
			<enum 9 rxdma_amsdu_parse_err>RX OLE reported an A-MSDU
			parsing error
			
			<enum 10 rxdma_sa_timeout_err>RX OLE reported a timeout
			during SA search
			
			<enum 11 rxdma_da_timeout_err>RX OLE reported a timeout
			during DA search
			
			<enum 12 rxdma_flow_timeout_err>RX OLE reported a
			timeout during flow search
			
			<enum 13 rxdma_flush_request>RXDMA received a flush
			request
			
			<enum 14 rxdma_amsdu_fragment_err>Rx PCU reported A-MSDU
			present as well as a fragmented MPDU. A-MSDU defragmentation
			is not supported in Lithium SW so this is treated as an
			error.
*/
#define REO_ENTRANCE_RING_6_RXDMA_ERROR_CODE_OFFSET                  0x00000018
#define REO_ENTRANCE_RING_6_RXDMA_ERROR_CODE_LSB                     2
#define REO_ENTRANCE_RING_6_RXDMA_ERROR_CODE_MASK                    0x0000007c

/* Description		REO_ENTRANCE_RING_6_MPDU_FRAGMENT_NUMBER
			
			Field only valid when Reo_level_mpdu_frame_info.
			Rx_mpdu_desc_info_details.Fragment_flag is set.
			
			
			
			The fragment number from the 802.11 header.
			
			
			
			Note that the sequence number is embedded in the field:
			Reo_level_mpdu_frame_info. Rx_mpdu_desc_info_details.
			Mpdu_sequence_number
			
			
			
			<legal all>
*/
#define REO_ENTRANCE_RING_6_MPDU_FRAGMENT_NUMBER_OFFSET              0x00000018
#define REO_ENTRANCE_RING_6_MPDU_FRAGMENT_NUMBER_LSB                 7
#define REO_ENTRANCE_RING_6_MPDU_FRAGMENT_NUMBER_MASK                0x00000780

/* Description		REO_ENTRANCE_RING_6_RESERVED_6A
			
			<legal 0>
*/
#define REO_ENTRANCE_RING_6_RESERVED_6A_OFFSET                       0x00000018
#define REO_ENTRANCE_RING_6_RESERVED_6A_LSB                          11
#define REO_ENTRANCE_RING_6_RESERVED_6A_MASK                         0xfffff800

/* Description		REO_ENTRANCE_RING_7_RESERVED_7A
			
			<legal 0>
*/
#define REO_ENTRANCE_RING_7_RESERVED_7A_OFFSET                       0x0000001c
#define REO_ENTRANCE_RING_7_RESERVED_7A_LSB                          0
#define REO_ENTRANCE_RING_7_RESERVED_7A_MASK                         0x000fffff

/* Description		REO_ENTRANCE_RING_7_RING_ID
			
			Consumer: SW/REO/DEBUG
			
			Producer: SRNG (of RXDMA)
			
			
			
			For debugging. 
			
			This field is filled in by the SRNG module.
			
			It help to identify the ring that is being looked <legal
			all>
*/
#define REO_ENTRANCE_RING_7_RING_ID_OFFSET                           0x0000001c
#define REO_ENTRANCE_RING_7_RING_ID_LSB                              20
#define REO_ENTRANCE_RING_7_RING_ID_MASK                             0x0ff00000

/* Description		REO_ENTRANCE_RING_7_LOOPING_COUNT
			
			Consumer: SW/REO/DEBUG
			
			Producer: SRNG (of RXDMA)
			
			
			
			For debugging. 
			
			This field is filled in by the SRNG module.
			
			
			
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
#define REO_ENTRANCE_RING_7_LOOPING_COUNT_OFFSET                     0x0000001c
#define REO_ENTRANCE_RING_7_LOOPING_COUNT_LSB                        28
#define REO_ENTRANCE_RING_7_LOOPING_COUNT_MASK                       0xf0000000


#endif // _REO_ENTRANCE_RING_H_
