/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
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

#ifndef _RX_MPDU_END_H_
#define _RX_MPDU_END_H_
#if !defined(__ASSEMBLER__)
#endif


// ################ START SUMMARY #################
//
//	Dword	Fields
//	0	rxpcu_mpdu_filter_in_category[1:0], sw_frame_group_id[8:2], reserved_0[15:9], phy_ppdu_id[31:16]
//	1	reserved_1a[10:0], unsup_ktype_short_frame[11], rx_in_tx_decrypt_byp[12], overflow_err[13], mpdu_length_err[14], tkip_mic_err[15], decrypt_err[16], unencrypted_frame_err[17], pn_fields_contain_valid_info[18], fcs_err[19], msdu_length_err[20], rxdma0_destination_ring[22:21], rxdma1_destination_ring[24:23], decrypt_status_code[27:25], rx_bitmap_not_updated[28], reserved_1b[31:29]
//
// ################ END SUMMARY #################

#define NUM_OF_DWORDS_RX_MPDU_END 2

struct rx_mpdu_end {
             uint32_t rxpcu_mpdu_filter_in_category   :  2, //[1:0]
                      sw_frame_group_id               :  7, //[8:2]
                      reserved_0                      :  7, //[15:9]
                      phy_ppdu_id                     : 16; //[31:16]
             uint32_t reserved_1a                     : 11, //[10:0]
                      unsup_ktype_short_frame         :  1, //[11]
                      rx_in_tx_decrypt_byp            :  1, //[12]
                      overflow_err                    :  1, //[13]
                      mpdu_length_err                 :  1, //[14]
                      tkip_mic_err                    :  1, //[15]
                      decrypt_err                     :  1, //[16]
                      unencrypted_frame_err           :  1, //[17]
                      pn_fields_contain_valid_info    :  1, //[18]
                      fcs_err                         :  1, //[19]
                      msdu_length_err                 :  1, //[20]
                      rxdma0_destination_ring         :  2, //[22:21]
                      rxdma1_destination_ring         :  2, //[24:23]
                      decrypt_status_code             :  3, //[27:25]
                      rx_bitmap_not_updated           :  1, //[28]
                      reserved_1b                     :  3; //[31:29]
};

/*

rxpcu_mpdu_filter_in_category
			
			Field indicates what the reason was that this MPDU frame
			was allowed to come into the receive path by RXPCU
			
			<enum 0 rxpcu_filter_pass> This MPDU passed the normal
			frame filter programming of rxpcu
			
			<enum 1 rxpcu_monitor_client> This MPDU did NOT pass the
			regular frame filter and would have been dropped, were it
			not for the frame fitting into the 'monitor_client'
			category.
			
			<enum 2 rxpcu_monitor_other> This MPDU did NOT pass the
			regular frame filter and also did not pass the
			rxpcu_monitor_client filter. It would have been dropped
			accept that it did pass the 'monitor_other' category.
			
			<legal 0-2>

sw_frame_group_id
			
			SW processes frames based on certain classifications.
			This field indicates to what sw classification this MPDU is
			mapped.
			
			The classification is given in priority order
			
			
			
			<enum 0 sw_frame_group_NDP_frame> 
			
			
			
			<enum 1 sw_frame_group_Multicast_data> 
			
			<enum 2 sw_frame_group_Unicast_data> 
			
			<enum 3 sw_frame_group_Null_data > This includes mpdus
			of type Data Null as well as QoS Data Null
			
			
			
			<enum 4 sw_frame_group_mgmt_0000 > 
			
			<enum 5 sw_frame_group_mgmt_0001 > 
			
			<enum 6 sw_frame_group_mgmt_0010 > 
			
			<enum 7 sw_frame_group_mgmt_0011 > 
			
			<enum 8 sw_frame_group_mgmt_0100 > 
			
			<enum 9 sw_frame_group_mgmt_0101 > 
			
			<enum 10 sw_frame_group_mgmt_0110 > 
			
			<enum 11 sw_frame_group_mgmt_0111 > 
			
			<enum 12 sw_frame_group_mgmt_1000 > 
			
			<enum 13 sw_frame_group_mgmt_1001 > 
			
			<enum 14 sw_frame_group_mgmt_1010 > 
			
			<enum 15 sw_frame_group_mgmt_1011 > 
			
			<enum 16 sw_frame_group_mgmt_1100 > 
			
			<enum 17 sw_frame_group_mgmt_1101 > 
			
			<enum 18 sw_frame_group_mgmt_1110 > 
			
			<enum 19 sw_frame_group_mgmt_1111 > 
			
			
			
			<enum 20 sw_frame_group_ctrl_0000 > 
			
			<enum 21 sw_frame_group_ctrl_0001 > 
			
			<enum 22 sw_frame_group_ctrl_0010 > 
			
			<enum 23 sw_frame_group_ctrl_0011 > 
			
			<enum 24 sw_frame_group_ctrl_0100 > 
			
			<enum 25 sw_frame_group_ctrl_0101 > 
			
			<enum 26 sw_frame_group_ctrl_0110 > 
			
			<enum 27 sw_frame_group_ctrl_0111 > 
			
			<enum 28 sw_frame_group_ctrl_1000 > 
			
			<enum 29 sw_frame_group_ctrl_1001 > 
			
			<enum 30 sw_frame_group_ctrl_1010 > 
			
			<enum 31 sw_frame_group_ctrl_1011 > 
			
			<enum 32 sw_frame_group_ctrl_1100 > 
			
			<enum 33 sw_frame_group_ctrl_1101 > 
			
			<enum 34 sw_frame_group_ctrl_1110 > 
			
			<enum 35 sw_frame_group_ctrl_1111 > 
			
			
			
			<enum 36 sw_frame_group_unsupported> This covers type 3
			and protocol version != 0
			
			
			
			
			
			
			<legal 0-37>

reserved_0
			
			<legal 0>

phy_ppdu_id
			
			A ppdu counter value that PHY increments for every PPDU
			received. The counter value wraps around  
			
			<legal all>

reserved_1a
			
			<legal 0>

unsup_ktype_short_frame
			
			This bit will be '1' when WEP or TKIP or WAPI key type
			is received for 11ah short frame.  Crypto will bypass the
			received packet without decryption to RxOLE after setting
			this bit.

rx_in_tx_decrypt_byp
			
			Indicates that RX packet is not decrypted as Crypto is
			busy with TX packet processing.

overflow_err
			
			RXPCU Receive FIFO ran out of space to receive the full
			MPDU. Therefor this MPDU is terminated early and is thus
			corrupted.  
			
			
			
			This MPDU will not be ACKed.
			
			RXPCU might still be able to correctly receive the
			following MPDUs in the PPDU if enough fifo space became
			available in time

mpdu_length_err
			
			Set by RXPCU if the expected MPDU length does not
			correspond with the actually received number of bytes in the
			MPDU.

tkip_mic_err
			
			Set by RX CRYPTO when CRYPTO detected a TKIP MIC error
			for this MPDU

decrypt_err
			
			Set by RX CRYPTO when CRYPTO detected a decrypt error
			for this MPDU or CRYPTO received an encrypted frame, but did
			not get a valid corresponding key id in the peer entry.

unencrypted_frame_err
			
			Set by RX CRYPTO when CRYPTO detected an unencrypted
			frame while in the peer entry field
			'All_frames_shall_be_encrypted' is set.

pn_fields_contain_valid_info
			
			Set by RX CRYPTO to indicate that there is a valid PN
			field present in this MPDU

fcs_err
			
			Set by RXPCU when there is an FCS error detected for
			this MPDU
			
			NOTE that when this field is set, all other (error)
			field settings should be ignored as modules could have made
			wrong decisions based on the corrupted data.

msdu_length_err
			
			Set by RXOLE when there is an msdu length error detected
			in at least 1 of the MSDUs embedded within the MPDU

rxdma0_destination_ring
			
			The ring to which RXDMA0 shall push the frame, assuming
			no MPDU level errors are detected. In case of MPDU level
			errors, RXDMA0 might change the RXDMA0 destination
			
			
			
			<enum 0  rxdma_release_ring >  RXDMA0 shall push the
			frame to the Release ring. Effectively this means the frame
			needs to be dropped.
			
			
			
			<enum 1  rxdma2fw_ring >  RXDMA0 shall push the frame to
			the FW ring 
			
			
			
			<enum 2  rxdma2sw_ring >  RXDMA0 shall push the frame to
			the SW ring 
			
			
			
			<enum 3  rxdma2reo_ring >  RXDMA0 shall push the frame
			to the REO entrance ring 
			
			
			
			<legal all>

rxdma1_destination_ring
			
			The ring to which RXDMA1 shall push the frame, assuming
			no MPDU level errors are detected. In case of MPDU level
			errors, RXDMA1 might change the RXDMA destination
			
			
			
			<enum 0  rxdma_release_ring >  RXDMA1 shall push the
			frame to the Release ring. Effectively this means the frame
			needs to be dropped.
			
			
			
			<enum 1  rxdma2fw_ring >  RXDMA1 shall push the frame to
			the FW ring 
			
			
			
			<enum 2  rxdma2sw_ring >  RXDMA1 shall push the frame to
			the SW ring 
			
			
			
			<enum 3  rxdma2reo_ring >  RXDMA1 shall push the frame
			to the REO entrance ring 
			
			
			
			<legal all>

decrypt_status_code
			
			Field provides insight into the decryption performed
			
			
			
			<enum 0 decrypt_ok> Frame had protection enabled and
			decrypted properly 
			
			<enum 1 decrypt_unprotected_frame > Frame is unprotected
			and hence bypassed 
			
			<enum 2 decrypt_data_err > Frame has protection enabled
			and could not be properly decrypted due to MIC/ICV mismatch
			etc. 
			
			<enum 3 decrypt_key_invalid > Frame has protection
			enabled but the key that was required to decrypt this frame
			was not valid 
			
			<enum 4 decrypt_peer_entry_invalid > Frame has
			protection enabled but the key that was required to decrypt
			this frame was not valid
			
			<enum 5 decrypt_other > Reserved for other indications
			
			
			
			<legal 0 - 5>

rx_bitmap_not_updated
			
			Frame is received, but RXPCU could not update the
			receive bitmap due to (temporary) fifo contraints.
			
			<legal all>

reserved_1b
			
			<legal 0>
*/


/* Description		RX_MPDU_END_0_RXPCU_MPDU_FILTER_IN_CATEGORY
			
			Field indicates what the reason was that this MPDU frame
			was allowed to come into the receive path by RXPCU
			
			<enum 0 rxpcu_filter_pass> This MPDU passed the normal
			frame filter programming of rxpcu
			
			<enum 1 rxpcu_monitor_client> This MPDU did NOT pass the
			regular frame filter and would have been dropped, were it
			not for the frame fitting into the 'monitor_client'
			category.
			
			<enum 2 rxpcu_monitor_other> This MPDU did NOT pass the
			regular frame filter and also did not pass the
			rxpcu_monitor_client filter. It would have been dropped
			accept that it did pass the 'monitor_other' category.
			
			<legal 0-2>
*/
#define RX_MPDU_END_0_RXPCU_MPDU_FILTER_IN_CATEGORY_OFFSET           0x00000000
#define RX_MPDU_END_0_RXPCU_MPDU_FILTER_IN_CATEGORY_LSB              0
#define RX_MPDU_END_0_RXPCU_MPDU_FILTER_IN_CATEGORY_MASK             0x00000003

/* Description		RX_MPDU_END_0_SW_FRAME_GROUP_ID
			
			SW processes frames based on certain classifications.
			This field indicates to what sw classification this MPDU is
			mapped.
			
			The classification is given in priority order
			
			
			
			<enum 0 sw_frame_group_NDP_frame> 
			
			
			
			<enum 1 sw_frame_group_Multicast_data> 
			
			<enum 2 sw_frame_group_Unicast_data> 
			
			<enum 3 sw_frame_group_Null_data > This includes mpdus
			of type Data Null as well as QoS Data Null
			
			
			
			<enum 4 sw_frame_group_mgmt_0000 > 
			
			<enum 5 sw_frame_group_mgmt_0001 > 
			
			<enum 6 sw_frame_group_mgmt_0010 > 
			
			<enum 7 sw_frame_group_mgmt_0011 > 
			
			<enum 8 sw_frame_group_mgmt_0100 > 
			
			<enum 9 sw_frame_group_mgmt_0101 > 
			
			<enum 10 sw_frame_group_mgmt_0110 > 
			
			<enum 11 sw_frame_group_mgmt_0111 > 
			
			<enum 12 sw_frame_group_mgmt_1000 > 
			
			<enum 13 sw_frame_group_mgmt_1001 > 
			
			<enum 14 sw_frame_group_mgmt_1010 > 
			
			<enum 15 sw_frame_group_mgmt_1011 > 
			
			<enum 16 sw_frame_group_mgmt_1100 > 
			
			<enum 17 sw_frame_group_mgmt_1101 > 
			
			<enum 18 sw_frame_group_mgmt_1110 > 
			
			<enum 19 sw_frame_group_mgmt_1111 > 
			
			
			
			<enum 20 sw_frame_group_ctrl_0000 > 
			
			<enum 21 sw_frame_group_ctrl_0001 > 
			
			<enum 22 sw_frame_group_ctrl_0010 > 
			
			<enum 23 sw_frame_group_ctrl_0011 > 
			
			<enum 24 sw_frame_group_ctrl_0100 > 
			
			<enum 25 sw_frame_group_ctrl_0101 > 
			
			<enum 26 sw_frame_group_ctrl_0110 > 
			
			<enum 27 sw_frame_group_ctrl_0111 > 
			
			<enum 28 sw_frame_group_ctrl_1000 > 
			
			<enum 29 sw_frame_group_ctrl_1001 > 
			
			<enum 30 sw_frame_group_ctrl_1010 > 
			
			<enum 31 sw_frame_group_ctrl_1011 > 
			
			<enum 32 sw_frame_group_ctrl_1100 > 
			
			<enum 33 sw_frame_group_ctrl_1101 > 
			
			<enum 34 sw_frame_group_ctrl_1110 > 
			
			<enum 35 sw_frame_group_ctrl_1111 > 
			
			
			
			<enum 36 sw_frame_group_unsupported> This covers type 3
			and protocol version != 0
			
			
			
			
			
			
			<legal 0-37>
*/
#define RX_MPDU_END_0_SW_FRAME_GROUP_ID_OFFSET                       0x00000000
#define RX_MPDU_END_0_SW_FRAME_GROUP_ID_LSB                          2
#define RX_MPDU_END_0_SW_FRAME_GROUP_ID_MASK                         0x000001fc

/* Description		RX_MPDU_END_0_RESERVED_0
			
			<legal 0>
*/
#define RX_MPDU_END_0_RESERVED_0_OFFSET                              0x00000000
#define RX_MPDU_END_0_RESERVED_0_LSB                                 9
#define RX_MPDU_END_0_RESERVED_0_MASK                                0x0000fe00

/* Description		RX_MPDU_END_0_PHY_PPDU_ID
			
			A ppdu counter value that PHY increments for every PPDU
			received. The counter value wraps around  
			
			<legal all>
*/
#define RX_MPDU_END_0_PHY_PPDU_ID_OFFSET                             0x00000000
#define RX_MPDU_END_0_PHY_PPDU_ID_LSB                                16
#define RX_MPDU_END_0_PHY_PPDU_ID_MASK                               0xffff0000

/* Description		RX_MPDU_END_1_RESERVED_1A
			
			<legal 0>
*/
#define RX_MPDU_END_1_RESERVED_1A_OFFSET                             0x00000004
#define RX_MPDU_END_1_RESERVED_1A_LSB                                0
#define RX_MPDU_END_1_RESERVED_1A_MASK                               0x000007ff

/* Description		RX_MPDU_END_1_UNSUP_KTYPE_SHORT_FRAME
			
			This bit will be '1' when WEP or TKIP or WAPI key type
			is received for 11ah short frame.  Crypto will bypass the
			received packet without decryption to RxOLE after setting
			this bit.
*/
#define RX_MPDU_END_1_UNSUP_KTYPE_SHORT_FRAME_OFFSET                 0x00000004
#define RX_MPDU_END_1_UNSUP_KTYPE_SHORT_FRAME_LSB                    11
#define RX_MPDU_END_1_UNSUP_KTYPE_SHORT_FRAME_MASK                   0x00000800

/* Description		RX_MPDU_END_1_RX_IN_TX_DECRYPT_BYP
			
			Indicates that RX packet is not decrypted as Crypto is
			busy with TX packet processing.
*/
#define RX_MPDU_END_1_RX_IN_TX_DECRYPT_BYP_OFFSET                    0x00000004
#define RX_MPDU_END_1_RX_IN_TX_DECRYPT_BYP_LSB                       12
#define RX_MPDU_END_1_RX_IN_TX_DECRYPT_BYP_MASK                      0x00001000

/* Description		RX_MPDU_END_1_OVERFLOW_ERR
			
			RXPCU Receive FIFO ran out of space to receive the full
			MPDU. Therefor this MPDU is terminated early and is thus
			corrupted.  
			
			
			
			This MPDU will not be ACKed.
			
			RXPCU might still be able to correctly receive the
			following MPDUs in the PPDU if enough fifo space became
			available in time
*/
#define RX_MPDU_END_1_OVERFLOW_ERR_OFFSET                            0x00000004
#define RX_MPDU_END_1_OVERFLOW_ERR_LSB                               13
#define RX_MPDU_END_1_OVERFLOW_ERR_MASK                              0x00002000

/* Description		RX_MPDU_END_1_MPDU_LENGTH_ERR
			
			Set by RXPCU if the expected MPDU length does not
			correspond with the actually received number of bytes in the
			MPDU.
*/
#define RX_MPDU_END_1_MPDU_LENGTH_ERR_OFFSET                         0x00000004
#define RX_MPDU_END_1_MPDU_LENGTH_ERR_LSB                            14
#define RX_MPDU_END_1_MPDU_LENGTH_ERR_MASK                           0x00004000

/* Description		RX_MPDU_END_1_TKIP_MIC_ERR
			
			Set by RX CRYPTO when CRYPTO detected a TKIP MIC error
			for this MPDU
*/
#define RX_MPDU_END_1_TKIP_MIC_ERR_OFFSET                            0x00000004
#define RX_MPDU_END_1_TKIP_MIC_ERR_LSB                               15
#define RX_MPDU_END_1_TKIP_MIC_ERR_MASK                              0x00008000

/* Description		RX_MPDU_END_1_DECRYPT_ERR
			
			Set by RX CRYPTO when CRYPTO detected a decrypt error
			for this MPDU or CRYPTO received an encrypted frame, but did
			not get a valid corresponding key id in the peer entry.
*/
#define RX_MPDU_END_1_DECRYPT_ERR_OFFSET                             0x00000004
#define RX_MPDU_END_1_DECRYPT_ERR_LSB                                16
#define RX_MPDU_END_1_DECRYPT_ERR_MASK                               0x00010000

/* Description		RX_MPDU_END_1_UNENCRYPTED_FRAME_ERR
			
			Set by RX CRYPTO when CRYPTO detected an unencrypted
			frame while in the peer entry field
			'All_frames_shall_be_encrypted' is set.
*/
#define RX_MPDU_END_1_UNENCRYPTED_FRAME_ERR_OFFSET                   0x00000004
#define RX_MPDU_END_1_UNENCRYPTED_FRAME_ERR_LSB                      17
#define RX_MPDU_END_1_UNENCRYPTED_FRAME_ERR_MASK                     0x00020000

/* Description		RX_MPDU_END_1_PN_FIELDS_CONTAIN_VALID_INFO
			
			Set by RX CRYPTO to indicate that there is a valid PN
			field present in this MPDU
*/
#define RX_MPDU_END_1_PN_FIELDS_CONTAIN_VALID_INFO_OFFSET            0x00000004
#define RX_MPDU_END_1_PN_FIELDS_CONTAIN_VALID_INFO_LSB               18
#define RX_MPDU_END_1_PN_FIELDS_CONTAIN_VALID_INFO_MASK              0x00040000

/* Description		RX_MPDU_END_1_FCS_ERR
			
			Set by RXPCU when there is an FCS error detected for
			this MPDU
			
			NOTE that when this field is set, all other (error)
			field settings should be ignored as modules could have made
			wrong decisions based on the corrupted data.
*/
#define RX_MPDU_END_1_FCS_ERR_OFFSET                                 0x00000004
#define RX_MPDU_END_1_FCS_ERR_LSB                                    19
#define RX_MPDU_END_1_FCS_ERR_MASK                                   0x00080000

/* Description		RX_MPDU_END_1_MSDU_LENGTH_ERR
			
			Set by RXOLE when there is an msdu length error detected
			in at least 1 of the MSDUs embedded within the MPDU
*/
#define RX_MPDU_END_1_MSDU_LENGTH_ERR_OFFSET                         0x00000004
#define RX_MPDU_END_1_MSDU_LENGTH_ERR_LSB                            20
#define RX_MPDU_END_1_MSDU_LENGTH_ERR_MASK                           0x00100000

/* Description		RX_MPDU_END_1_RXDMA0_DESTINATION_RING
			
			The ring to which RXDMA0 shall push the frame, assuming
			no MPDU level errors are detected. In case of MPDU level
			errors, RXDMA0 might change the RXDMA0 destination
			
			
			
			<enum 0  rxdma_release_ring >  RXDMA0 shall push the
			frame to the Release ring. Effectively this means the frame
			needs to be dropped.
			
			
			
			<enum 1  rxdma2fw_ring >  RXDMA0 shall push the frame to
			the FW ring 
			
			
			
			<enum 2  rxdma2sw_ring >  RXDMA0 shall push the frame to
			the SW ring 
			
			
			
			<enum 3  rxdma2reo_ring >  RXDMA0 shall push the frame
			to the REO entrance ring 
			
			
			
			<legal all>
*/
#define RX_MPDU_END_1_RXDMA0_DESTINATION_RING_OFFSET                 0x00000004
#define RX_MPDU_END_1_RXDMA0_DESTINATION_RING_LSB                    21
#define RX_MPDU_END_1_RXDMA0_DESTINATION_RING_MASK                   0x00600000

/* Description		RX_MPDU_END_1_RXDMA1_DESTINATION_RING
			
			The ring to which RXDMA1 shall push the frame, assuming
			no MPDU level errors are detected. In case of MPDU level
			errors, RXDMA1 might change the RXDMA destination
			
			
			
			<enum 0  rxdma_release_ring >  RXDMA1 shall push the
			frame to the Release ring. Effectively this means the frame
			needs to be dropped.
			
			
			
			<enum 1  rxdma2fw_ring >  RXDMA1 shall push the frame to
			the FW ring 
			
			
			
			<enum 2  rxdma2sw_ring >  RXDMA1 shall push the frame to
			the SW ring 
			
			
			
			<enum 3  rxdma2reo_ring >  RXDMA1 shall push the frame
			to the REO entrance ring 
			
			
			
			<legal all>
*/
#define RX_MPDU_END_1_RXDMA1_DESTINATION_RING_OFFSET                 0x00000004
#define RX_MPDU_END_1_RXDMA1_DESTINATION_RING_LSB                    23
#define RX_MPDU_END_1_RXDMA1_DESTINATION_RING_MASK                   0x01800000

/* Description		RX_MPDU_END_1_DECRYPT_STATUS_CODE
			
			Field provides insight into the decryption performed
			
			
			
			<enum 0 decrypt_ok> Frame had protection enabled and
			decrypted properly 
			
			<enum 1 decrypt_unprotected_frame > Frame is unprotected
			and hence bypassed 
			
			<enum 2 decrypt_data_err > Frame has protection enabled
			and could not be properly decrypted due to MIC/ICV mismatch
			etc. 
			
			<enum 3 decrypt_key_invalid > Frame has protection
			enabled but the key that was required to decrypt this frame
			was not valid 
			
			<enum 4 decrypt_peer_entry_invalid > Frame has
			protection enabled but the key that was required to decrypt
			this frame was not valid
			
			<enum 5 decrypt_other > Reserved for other indications
			
			
			
			<legal 0 - 5>
*/
#define RX_MPDU_END_1_DECRYPT_STATUS_CODE_OFFSET                     0x00000004
#define RX_MPDU_END_1_DECRYPT_STATUS_CODE_LSB                        25
#define RX_MPDU_END_1_DECRYPT_STATUS_CODE_MASK                       0x0e000000

/* Description		RX_MPDU_END_1_RX_BITMAP_NOT_UPDATED
			
			Frame is received, but RXPCU could not update the
			receive bitmap due to (temporary) fifo contraints.
			
			<legal all>
*/
#define RX_MPDU_END_1_RX_BITMAP_NOT_UPDATED_OFFSET                   0x00000004
#define RX_MPDU_END_1_RX_BITMAP_NOT_UPDATED_LSB                      28
#define RX_MPDU_END_1_RX_BITMAP_NOT_UPDATED_MASK                     0x10000000

/* Description		RX_MPDU_END_1_RESERVED_1B
			
			<legal 0>
*/
#define RX_MPDU_END_1_RESERVED_1B_OFFSET                             0x00000004
#define RX_MPDU_END_1_RESERVED_1B_LSB                                29
#define RX_MPDU_END_1_RESERVED_1B_MASK                               0xe0000000


#endif // _RX_MPDU_END_H_
