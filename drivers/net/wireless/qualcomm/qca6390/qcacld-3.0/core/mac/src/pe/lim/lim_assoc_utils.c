/*
 * Copyright (c) 2011-2019 The Linux Foundation. All rights reserved.
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

/*
 * This file lim_assoc_utils.cc contains the utility functions
 * LIM uses while processing (Re) Association messages.
 * Author:        Chandra Modumudi
 * Date:          02/13/02
 * History:-
 * Date           Modified by    Modification Information
 * --------------------------------------------------------------------
 * 05/26/10       js             WPA handling in (Re)Assoc frames
 *
 */

#include "cds_api.h"
#include "ani_global.h"
#include "wni_api.h"
#include "sir_common.h"

#include "wni_cfg.h"
#include "cfg_ucfg_api.h"

#include "sch_api.h"
#include "utils_api.h"
#include "lim_utils.h"
#include "lim_assoc_utils.h"
#include "lim_security_utils.h"
#include "lim_ser_des_utils.h"
#include "lim_sta_hash_api.h"
#include "lim_admit_control.h"
#include "lim_send_messages.h"
#include "lim_ibss_peer_mgmt.h"
#include "lim_ft_defs.h"
#include "lim_session.h"
#include "lim_process_fils.h"

#include "qdf_types.h"
#include "wma_types.h"
#include "lim_types.h"
#include "wlan_utility.h"
#include "wlan_mlme_api.h"

#ifdef FEATURE_WLAN_TDLS
#define IS_TDLS_PEER(type)  ((type) == STA_ENTRY_TDLS_PEER)
#else
#define IS_TDLS_PEER(type) 0
#endif

/**
 * lim_cmp_ssid() - utility function to compare SSIDs
 * @rx_ssid: Received SSID
 * @session_entry: Session entry
 *
 * This function is called in various places within LIM code
 * to determine whether received SSID is same as SSID in use.
 *
 * Return: zero if SSID matched, non-zero otherwise.
 */
uint32_t lim_cmp_ssid(tSirMacSSid *rx_ssid, struct pe_session *session_entry)
{
	return qdf_mem_cmp(rx_ssid, &session_entry->ssId,
				session_entry->ssId.length);
}

/**
 * lim_compare_capabilities()
 *
 ***FUNCTION:
 * This function is called during Association/Reassociation
 * frame handling to determine whether received capabilities
 * match with local capabilities or not.
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  mac         - Pointer to Global MAC structure
 * @param  pAssocReq    - Pointer to received Assoc Req frame
 * @param  pLocalCapabs - Pointer to local capabilities
 *
 * @return status - true for Capabilitity match else false.
 */

uint8_t
lim_compare_capabilities(struct mac_context *mac,
			 tSirAssocReq *pAssocReq,
			 tSirMacCapabilityInfo *pLocalCapabs,
			 struct pe_session *pe_session)
{
	if (LIM_IS_AP_ROLE(pe_session) &&
	    (pAssocReq->capabilityInfo.ibss)) {
		/* Requesting STA asserting IBSS capability. */
		pe_debug("Requesting STA asserting IBSS capability");
		return false;
	}
	/* Compare CF capabilities */
	if (pAssocReq->capabilityInfo.cfPollable ||
	    pAssocReq->capabilityInfo.cfPollReq) {
		/* AP does not support PCF functionality */
		pe_debug(" AP does not support PCF functionality");
		return false;
	}
	/* Compare short preamble capability */
	if (pAssocReq->capabilityInfo.shortPreamble &&
	    (pAssocReq->capabilityInfo.shortPreamble !=
	     pLocalCapabs->shortPreamble)) {
		/* Allowing a STA requesting short preamble while */
		/* AP does not support it */
	}

	pe_debug("QoS in AssocReq: %d, local capabs qos: %d",
		pAssocReq->capabilityInfo.qos, pLocalCapabs->qos);

	/* Compare QoS capability */
	if (pAssocReq->capabilityInfo.qos &&
	    (pAssocReq->capabilityInfo.qos != pLocalCapabs->qos))
		pe_debug("Received unmatched QOS but cfg to suppress - continuing");

	/*
	 * If AP supports shortSlot and if apple user has
	 * enforced association only from shortSlot station,
	 * then AP must reject any station that does not support
	 * shortSlot
	 */
	if (LIM_IS_AP_ROLE(pe_session) &&
	    (pLocalCapabs->shortSlotTime == 1)) {
		if (mac->mlme_cfg->feature_flags.accept_short_slot_assoc) {
			if (pAssocReq->capabilityInfo.shortSlotTime !=
			    pLocalCapabs->shortSlotTime) {
				pe_err("AP rejects association as station doesn't support shortslot time");
				return false;
			}
			return false;
		}
	}

	return true;
} /****** end lim_compare_capabilities() ******/

/**
 * lim_check_rx_basic_rates()
 *
 ***FUNCTION:
 * This function is called during Association/Reassociation
 * frame handling to determine whether received rates in
 * Assoc/Reassoc request frames include all BSS basic rates
 * or not.
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  rxRateSet - pointer to SSID structure
 *
 * @return status - true if ALL BSS basic rates are present in the
 *                  received rateset else false.
 */

uint8_t
lim_check_rx_basic_rates(struct mac_context *mac, tSirMacRateSet rxRateSet,
			 struct pe_session *pe_session)
{
	tSirMacRateSet *pRateSet, basicRate;
	uint8_t i, j, k, match;

	pRateSet = qdf_mem_malloc(sizeof(tSirMacRateSet));
	if (!pRateSet)
		return false;

	/* Copy operational rate set from session Entry */
	qdf_mem_copy(pRateSet->rate, (pe_session->rateSet.rate),
		     pe_session->rateSet.numRates);

	pRateSet->numRates = pe_session->rateSet.numRates;

	/* Extract BSS basic rateset from operational rateset */
	for (i = 0, j = 0;
	     ((i < pRateSet->numRates) && (i < WLAN_SUPPORTED_RATES_IE_MAX_LEN)); i++) {
		if ((pRateSet->rate[i] & 0x80) == 0x80) {
			/* msb is set, so this is a basic rate */
			basicRate.rate[j++] = pRateSet->rate[i];
		}
	}

	/*
	 * For each BSS basic rate, find if it is present in the
	 * received rateset.
	 */
	for (k = 0; k < j; k++) {
		match = 0;
		for (i = 0;
		     ((i < rxRateSet.numRates)
		      && (i < WLAN_SUPPORTED_RATES_IE_MAX_LEN)); i++) {
			if ((rxRateSet.rate[i] | 0x80) == basicRate.rate[k])
				match = 1;
		}

		if (!match) {
			/* Free up memory allocated for rateset */
			qdf_mem_free((uint8_t *) pRateSet);

			return false;
		}
	}

	/* Free up memory allocated for rateset */
	qdf_mem_free((uint8_t *) pRateSet);

	return true;
} /****** end lim_check_rx_basic_rates() ******/

/**
 * lim_check_mcs_set()
 *
 ***FUNCTION:
 * This function is called during Association/Reassociation
 * frame handling to determine whether received MCS rates in
 * Assoc/Reassoc request frames includes all Basic MCS Rate Set or not.
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  supportedMCSSet - pointer to Supported MCS Rate Set
 *
 * @return status - true if ALL MCS Basic Rate Set rates are present in the
 *                  received rateset else false.
 */

uint8_t lim_check_mcs_set(struct mac_context *mac, uint8_t *supportedMCSSet)
{
	uint8_t basicMCSSet[SIZE_OF_BASIC_MCS_SET] = { 0 };
	qdf_size_t cfg_len = 0;
	uint8_t i;
	uint8_t validBytes;
	uint8_t lastByteMCSMask = 0x1f;

	cfg_len = mac->mlme_cfg->rates.basic_mcs_set.len;
	if (wlan_mlme_get_cfg_str((uint8_t *)basicMCSSet,
				  &mac->mlme_cfg->rates.basic_mcs_set,
				  &cfg_len) != QDF_STATUS_SUCCESS) {
		/* / Could not get Basic MCS rateset from CFG. Log error. */
		pe_err("could not retrieve Basic MCS rateset");
		return false;
	}

	validBytes = VALID_MCS_SIZE / 8;

	/* check if all the Basic MCS Bits are set in supported MCS bitmap */
	for (i = 0; i < validBytes; i++) {
		if ((basicMCSSet[i] & supportedMCSSet[i]) != basicMCSSet[i]) {
			pe_warn("One of Basic MCS Set Rates is not supported by the Station");
			return false;
		}
	}

	/* check the last 5 bits of the valid MCS bitmap */
	if (((basicMCSSet[i] & lastByteMCSMask) &
	     (supportedMCSSet[i] & lastByteMCSMask)) !=
	    (basicMCSSet[i] & lastByteMCSMask)) {
		pe_warn("One of Basic MCS Set Rates is not supported by the Station");
		return false;
	}

	return true;
}

#define SECURITY_SUITE_TYPE_MASK 0xFF
#define SECURITY_SUITE_TYPE_WEP40 0x1
#define SECURITY_SUITE_TYPE_TKIP 0x2
#define SECURITY_SUITE_TYPE_CCMP 0x4
#define SECURITY_SUITE_TYPE_WEP104 0x4
#define SECURITY_SUITE_TYPE_GCMP 0x8
#define SECURITY_SUITE_TYPE_GCMP_256 0x9

#ifndef WLAN_CONV_CRYPTO_IE_SUPPORT
/**
 * is_non_rsn_cipher()- API to check whether cipher suit is rsn or not
 * @cipher_suite: cipher suit
 *
 * Return: True in case non ht cipher else false
 */
static inline bool is_non_rsn_cipher(uint8_t cipher_suite)
{
	uint8_t cipher_mask;

	cipher_mask = cipher_suite & SECURITY_SUITE_TYPE_MASK;
	if ((cipher_mask == SECURITY_SUITE_TYPE_CCMP) ||
	    (cipher_mask == SECURITY_SUITE_TYPE_GCMP) ||
	    (cipher_mask == SECURITY_SUITE_TYPE_GCMP_256))
		return false;

	return true;
}

/**
 * lim_check_rx_rsn_ie_match()- validate received rsn ie with supported cipher
 *      suites.
 * @mac_ctx: pointer to global mac structure
 * @rx_rsn_ie: received rsn IE pointer
 * @session_entry: pe session entry
 * @sta_is_ht: peer station HT capability
 * @pmf_connection: set to true if this is pmf connection
 *
 * This function is called during Association/Reassociation
 * frame handling to determine whether received RSN in
 * Assoc/Reassoc request frames include supported cipher suites or not.
 *
 * Return: QDF_STATUS_SUCCESS if ALL supported cipher suites are present in the
 *                  received rsn IE else failure status.
 */

uint8_t lim_check_rx_rsn_ie_match(struct mac_context *mac_ctx,
				  tDot11fIERSN * const rx_rsn_ie,
				  struct pe_session *session_entry, uint8_t sta_is_ht,
				  bool *pmf_connection)
{
	tDot11fIERSN *rsn_ie;
	bool match = false;
	uint8_t i, j, only_non_ht_cipher = 1;
#ifdef WLAN_FEATURE_11W
	bool we_are_pmf_capable;
	bool we_require_pmf;
	bool they_are_pmf_capable;
	bool they_require_pmf;
#endif

	/* RSN IE should be received from PE */
	rsn_ie = &session_entry->gStartBssRSNIe;

	if (!rx_rsn_ie) {
		pe_debug("Rx RSN IE is NULL");
		return eSIR_MAC_UNSPEC_FAILURE_STATUS;
	}

	/* We should have only one AKM in assoc/reassoc request */
	if (rx_rsn_ie->akm_suite_cnt != 1) {
		pe_debug("Invalid RX akm_suite_cnt %d",
			 rx_rsn_ie->akm_suite_cnt);
		return eSIR_MAC_INVALID_AKMP_STATUS;
	}
	/* Check if we support the received AKM */
	for (i = 0; i < rsn_ie->akm_suite_cnt; i++)
		if (!qdf_mem_cmp(&rx_rsn_ie->akm_suite[0],
				 &rsn_ie->akm_suite[i],
				 sizeof(rsn_ie->akm_suite[i]))) {
			match = true;
			break;
		}
	if (!match) {
		pe_debug("Invalid RX akm_suite");
		return eSIR_MAC_INVALID_AKMP_STATUS;
	}

	/* Check groupwise cipher suite */
	for (i = 0; i < sizeof(rx_rsn_ie->gp_cipher_suite); i++)
		if (rsn_ie->gp_cipher_suite[i] !=
				 rx_rsn_ie->gp_cipher_suite[i]) {
			pe_debug("Invalid groupwise cipher suite");
			return eSIR_MAC_INVALID_GROUP_CIPHER_STATUS;
		}

	/*
	 * For each Pairwise cipher suite check whether we support
	 * received pairwise
	 */
	match = false;
	for (i = 0; i < rx_rsn_ie->pwise_cipher_suite_count; i++) {
		for (j = 0; j < rsn_ie->pwise_cipher_suite_count; j++) {
			if (!qdf_mem_cmp(&rx_rsn_ie->pwise_cipher_suites[i],
			    &rsn_ie->pwise_cipher_suites[j],
			    sizeof(rsn_ie->pwise_cipher_suites[j]))) {
				match = true;
				break;
			}
		}

		if (sta_is_ht)
#ifdef ANI_LITTLE_BYTE_ENDIAN
			only_non_ht_cipher = is_non_rsn_cipher(
				rx_rsn_ie->pwise_cipher_suites[i][3]);
#else
			only_non_ht_cipher = is_non_rsn_cipher(
				rx_rsn_ie->pwise_cipher_suites[i][0]);
#endif
	}

	if ((!match) || ((sta_is_ht) && only_non_ht_cipher)) {
		pe_debug("Invalid pairwise cipher suite");
		return eSIR_MAC_INVALID_PAIRWISE_CIPHER_STATUS;
	}
	/*
	 * Check RSN capabilities
	 * Bit 0 of First Byte - PreAuthentication Capability
	 */
	if (((rx_rsn_ie->RSN_Cap[0] >> 0) & 0x1) == true) {
		/* this is supported by AP only */
		pe_debug("Invalid RSN information element capabilities");
		return eSIR_MAC_INVALID_RSN_IE_CAPABILITIES_STATUS;
	}

	*pmf_connection = false;

#ifdef WLAN_FEATURE_11W
	we_are_pmf_capable = session_entry->pLimStartBssReq->pmfCapable;
	we_require_pmf = session_entry->pLimStartBssReq->pmfRequired;
	they_are_pmf_capable = (rx_rsn_ie->RSN_Cap[0] >> 7) & 0x1;
	they_require_pmf = (rx_rsn_ie->RSN_Cap[0] >> 6) & 0x1;

	if ((they_require_pmf && they_are_pmf_capable && !we_are_pmf_capable) ||
	    (we_require_pmf && !they_are_pmf_capable)) {
		pe_debug("Association fail, robust management frames policy"
				" violation they_require_pmf =%d"
				" theyArePMFCapable %d weArePMFCapable %d"
				" weRequirePMF %d theyArePMFCapable %d",
			they_require_pmf, they_are_pmf_capable,
			we_are_pmf_capable, we_require_pmf,
			they_are_pmf_capable);
		return eSIR_MAC_ROBUST_MGMT_FRAMES_POLICY_VIOLATION;
	}

	if (they_are_pmf_capable && we_are_pmf_capable)
		*pmf_connection = true;

	pe_debug("weAreCapable %d, weRequire %d, theyAreCapable %d,"
			" theyRequire %d, PMFconnection %d",
		we_are_pmf_capable, we_require_pmf, they_are_pmf_capable,
		they_require_pmf, *pmf_connection);
#endif

	return eSIR_MAC_SUCCESS_STATUS;
}

/**
 * lim_check_rx_wpa_ie_match() - to check supported cipher suites
 *
 * @mac: pointer to global mac structure
 * @rx_wpaie: Received WPA IE in (Re)Assco req
 * @session_entry: pointer to PE session
 * @sta_is_ht: peer station is HT
 *
 * This function is called during Association/Reassociation
 * frame handling to determine whether received RSN in
 * Assoc/Reassoc request frames include supported cipher suites or not.
 *
 * Return: Success if ALL supported cipher suites are present in the
 *                  received wpa IE else failure status.
 */

uint8_t
lim_check_rx_wpa_ie_match(struct mac_context *mac, tDot11fIEWPA *rx_wpaie,
			  struct pe_session *session_entry, uint8_t sta_is_ht)
{
	tDot11fIEWPA *wpa_ie;
	bool match = false;
	uint8_t i, j, only_non_ht_cipher = 1;

	/* WPA IE should be received from PE */
	wpa_ie = &session_entry->gStartBssWPAIe;

	/* We should have only one AKM in assoc/reassoc request */
	if (rx_wpaie->auth_suite_count != 1) {
		pe_debug("Invalid RX auth_suite_count %d",
			 rx_wpaie->auth_suite_count);
		return eSIR_MAC_INVALID_AKMP_STATUS;
	}
	/* Check if we support the received AKM */
	for (i = 0; i < wpa_ie->auth_suite_count; i++)
		if (!qdf_mem_cmp(&rx_wpaie->auth_suites[0],
				 &wpa_ie->auth_suites[i],
				 sizeof(wpa_ie->auth_suites[i]))) {
			match = true;
			break;
		}
	if (!match) {
		pe_debug("Invalid RX auth_suites");
		return eSIR_MAC_INVALID_AKMP_STATUS;
	}

	/* Check groupwise cipher suite */
	for (i = 0; i < 4; i++) {
		if (wpa_ie->multicast_cipher[i] !=
				rx_wpaie->multicast_cipher[i]) {
			pe_debug("Invalid groupwise cipher suite");
			return eSIR_MAC_INVALID_GROUP_CIPHER_STATUS;
		}
	}

	/*
	 * For each Pairwise cipher suite check whether we support
	 * received pairwise
	 */
	match = false;
	for (i = 0; i < rx_wpaie->unicast_cipher_count; i++) {
		for (j = 0; j < wpa_ie->unicast_cipher_count; j++) {
			if (!qdf_mem_cmp(rx_wpaie->unicast_ciphers[i],
					    wpa_ie->unicast_ciphers[j], 4)) {
				match = true;
				break;
			}
		}

		if ((sta_is_ht)
#ifdef ANI_LITTLE_BYTE_ENDIAN
		    &&
		    ((rx_wpaie->
		      unicast_ciphers[i][3] & SECURITY_SUITE_TYPE_MASK) ==
		     SECURITY_SUITE_TYPE_CCMP))
#else
		    &&
		    ((rx_wpaie->
		      unicast_ciphers[i][0] & SECURITY_SUITE_TYPE_MASK) ==
		     SECURITY_SUITE_TYPE_CCMP))
#endif
		{
			only_non_ht_cipher = 0;
		}

	}

	if ((!match) || ((sta_is_ht) && only_non_ht_cipher)) {
		pe_debug("Invalid pairwise cipher suite");
		return eSIR_MAC_CIPHER_SUITE_REJECTED_STATUS;
	}

	return eSIR_MAC_SUCCESS_STATUS;
}
#endif

/**
 * lim_cleanup_rx_path()
 *
 ***FUNCTION:
 * This function is called to cleanup STA state at SP & RFP.
 *
 ***LOGIC:
 * To circumvent RFP's handling of dummy packet when it does not
 * have an incomplete packet for the STA to be deleted, a packet
 * with 'more framgents' bit set will be queued to RFP's WQ before
 * queuing 'dummy packet'.
 * A 'dummy' BD is pushed into RFP's WQ with type=00, subtype=1010
 * (Disassociation frame) and routing flags in BD set to eCPU's
 * Low Priority WQ.
 * RFP cleans up its local context for the STA id mentioned in the
 * BD and then pushes BD to eCPU's low priority WQ.
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param mac    Pointer to Global MAC structure
 * @param sta  Pointer to the per STA data structure
 *                initialized by LIM and maintained at DPH
 *
 * @return None
 */

QDF_STATUS
lim_cleanup_rx_path(struct mac_context *mac, tpDphHashNode sta,
		    struct pe_session *pe_session)
{
	QDF_STATUS retCode = QDF_STATUS_SUCCESS;

	pe_debug("Cleanup Rx Path for AID: %d"
		"pe_session->limSmeState: %d, mlmState: %d",
		sta->assocId, pe_session->limSmeState,
		sta->mlmStaContext.mlmState);

	pe_session->isCiscoVendorAP = false;

	if (mac->lim.gLimAddtsSent) {
		MTRACE(mac_trace
			       (mac, TRACE_CODE_TIMER_DEACTIVATE,
			       pe_session->peSessionId, eLIM_ADDTS_RSP_TIMER));
		tx_timer_deactivate(&mac->lim.limTimers.gLimAddtsRspTimer);
		pe_debug("Reset gLimAddtsSent flag and send addts timeout to SME");
		lim_process_sme_addts_rsp_timeout(mac,
					mac->lim.gLimAddtsRspTimerCount);
	}

	if (sta->mlmStaContext.mlmState == eLIM_MLM_WT_ASSOC_CNF_STATE) {
		lim_deactivate_and_change_per_sta_id_timer(mac, eLIM_CNF_WAIT_TIMER,
							   sta->assocId);

		if (!sta->mlmStaContext.updateContext) {
			/**
			 * There is no context at Polaris to delete.
			 * Release our assigned AID back to the free pool
			 */
			if (LIM_IS_AP_ROLE(pe_session)) {
				lim_del_sta(mac, sta, false, pe_session);
				lim_release_peer_idx(mac, sta->assocId,
						     pe_session);
			}
			lim_delete_dph_hash_entry(mac, sta->staAddr,
						  sta->assocId, pe_session);

			return retCode;
		}
	}
	/* delete all tspecs associated with this sta. */
	lim_admit_control_delete_sta(mac, sta->assocId);

	/**
	 * Make STA hash entry invalid at eCPU so that DPH
	 * does not process any more data packets and
	 * releases those BDs
	 */
	sta->valid = 0;
	lim_send_sme_tsm_ie_ind(mac, pe_session, 0, 0, 0);
	/* Any roaming related changes should be above this line */
	if (lim_is_roam_synch_in_progress(pe_session))
		return QDF_STATUS_SUCCESS;
	sta->mlmStaContext.mlmState = eLIM_MLM_WT_DEL_STA_RSP_STATE;

	if (LIM_IS_STA_ROLE(pe_session)) {
		MTRACE(mac_trace
		       (mac, TRACE_CODE_MLM_STATE, pe_session->peSessionId,
		       eLIM_MLM_WT_DEL_STA_RSP_STATE));
		pe_session->limMlmState = eLIM_MLM_WT_DEL_STA_RSP_STATE;
		/* Deactivating probe after heart beat timer */
		lim_deactivate_and_change_timer(mac, eLIM_PROBE_AFTER_HB_TIMER);
		lim_deactivate_and_change_timer(mac, eLIM_JOIN_FAIL_TIMER);
	}
#ifdef WLAN_DEBUG
	/* increment a debug count */
	mac->lim.gLimNumRxCleanup++;
#endif
	/* Do DEL BSS or DEL STA only if ADD BSS was success */
	if (!pe_session->add_bss_failed) {
		if (pe_session->limSmeState == eLIM_SME_JOIN_FAILURE_STATE) {
			retCode =
				lim_del_bss(mac, sta, pe_session->bss_idx,
					    pe_session);
		} else
			retCode = lim_del_sta(mac,
					 sta, true, pe_session);
	}

	return retCode;

} /*** end lim_cleanup_rx_path() ***/

/**
 * lim_send_del_sta_cnf() - Send Del sta confirmation
 * @mac: Pointer to Global MAC structure
 * @sta_dsaddr: sta ds address
 * @staDsAssocId: sta ds association id
 * @mlmStaContext: MLM station context
 * @status_code: Status code
 * @pe_session: Session entry
 *
 * This function is called to send appropriate CNF message to SME.
 *
 * Return: None
 */
void
lim_send_del_sta_cnf(struct mac_context *mac, struct qdf_mac_addr sta_dsaddr,
		     uint16_t staDsAssocId, tLimMlmStaContext mlmStaContext,
		     tSirResultCodes status_code, struct pe_session *pe_session)
{
	tLimMlmDisassocCnf mlmDisassocCnf;
	tLimMlmDeauthCnf mlmDeauthCnf;
	tLimMlmPurgeStaInd mlmPurgeStaInd;

	pe_debug("Sessionid: %d staDsAssocId: %d Trigger: %d status_code: %d sta_dsaddr: "QDF_MAC_ADDR_STR,
		pe_session->peSessionId, staDsAssocId,
		mlmStaContext.cleanupTrigger, status_code,
		QDF_MAC_ADDR_ARRAY(sta_dsaddr.bytes));

	if (LIM_IS_STA_ROLE(pe_session)) {
		/* Set BSSID at CFG to null */
		tSirMacAddr nullAddr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		sir_copy_mac_addr(nullAddr, pe_session->bssId);

		/* Free up buffer allocated for JoinReq held by */
		/* MLM state machine */
		if (pe_session->pLimMlmJoinReq) {
			qdf_mem_free(pe_session->pLimMlmJoinReq);
			pe_session->pLimMlmJoinReq = NULL;
		}

		pe_session->limAID = 0;
	}

	if ((mlmStaContext.cleanupTrigger ==
					eLIM_HOST_DISASSOC) ||
		(mlmStaContext.cleanupTrigger ==
					eLIM_LINK_MONITORING_DISASSOC) ||
		(mlmStaContext.cleanupTrigger ==
					eLIM_PROMISCUOUS_MODE_DISASSOC)) {
		/**
		 * Host or LMM driven Disassociation.
		 * Issue Disassoc Confirm to SME.
		 */
		pe_debug("Lim Posting DISASSOC_CNF to Sme. Trigger: %d",
			mlmStaContext.cleanupTrigger);

		qdf_mem_copy((uint8_t *) &mlmDisassocCnf.peerMacAddr,
			     (uint8_t *) sta_dsaddr.bytes, QDF_MAC_ADDR_SIZE);
		mlmDisassocCnf.resultCode = status_code;
		mlmDisassocCnf.disassocTrigger = mlmStaContext.cleanupTrigger;
		/* Update PE session Id */
		mlmDisassocCnf.sessionId = pe_session->peSessionId;

		lim_post_sme_message(mac,
				     LIM_MLM_DISASSOC_CNF,
				     (uint32_t *) &mlmDisassocCnf);
	} else if ((mlmStaContext.cleanupTrigger ==
					eLIM_HOST_DEAUTH) ||
			(mlmStaContext.cleanupTrigger ==
					eLIM_LINK_MONITORING_DEAUTH)) {
		/**
		 * Host or LMM driven Deauthentication.
		 * Issue Deauth Confirm to SME.
		 */
		pe_debug("Lim Posting DEAUTH_CNF to Sme. Trigger: %d",
			mlmStaContext.cleanupTrigger);
		qdf_copy_macaddr(&mlmDeauthCnf.peer_macaddr, &sta_dsaddr);
		mlmDeauthCnf.resultCode = status_code;
		mlmDeauthCnf.deauthTrigger = mlmStaContext.cleanupTrigger;
		/* PE session Id */
		mlmDeauthCnf.sessionId = pe_session->peSessionId;

		lim_post_sme_message(mac,
				     LIM_MLM_DEAUTH_CNF,
				     (uint32_t *) &mlmDeauthCnf);
	} else if ((mlmStaContext.cleanupTrigger ==
		    eLIM_PEER_ENTITY_DISASSOC) ||
		   (mlmStaContext.cleanupTrigger == eLIM_PEER_ENTITY_DEAUTH)) {
		/**
		 * Received Disassociation/Deauthentication from peer.
		 * Issue Purge Ind to SME.
		 */
		pe_debug("Lim Posting PURGE_STA_IND to Sme. Trigger: %d",
			mlmStaContext.cleanupTrigger);
		qdf_mem_copy((uint8_t *) &mlmPurgeStaInd.peerMacAddr,
			     (uint8_t *) sta_dsaddr.bytes, QDF_MAC_ADDR_SIZE);
		mlmPurgeStaInd.reasonCode =
			(uint8_t) mlmStaContext.disassocReason;
		mlmPurgeStaInd.aid = staDsAssocId;
		mlmPurgeStaInd.purgeTrigger = mlmStaContext.cleanupTrigger;
		mlmPurgeStaInd.sessionId = pe_session->peSessionId;

		lim_post_sme_message(mac,
				     LIM_MLM_PURGE_STA_IND,
				     (uint32_t *) &mlmPurgeStaInd);
	} else if (mlmStaContext.cleanupTrigger == eLIM_JOIN_FAILURE) {
		/* PE setup the peer entry in HW upfront, right after join is completed. */
		/* If there is a failure during rest of the assoc sequence, this context needs to be cleaned up. */
		uint8_t smesessionId;

		smesessionId = pe_session->smeSessionId;
		pe_session->limSmeState = eLIM_SME_JOIN_FAILURE_STATE;
		MTRACE(mac_trace
			       (mac, TRACE_CODE_SME_STATE, pe_session->peSessionId,
			       pe_session->limSmeState));

		/* if it is a reassoc failure to join new AP */
		if ((mlmStaContext.resultCode ==
		     eSIR_SME_FT_REASSOC_TIMEOUT_FAILURE)
		    || (mlmStaContext.resultCode == eSIR_SME_FT_REASSOC_FAILURE)
		    || (mlmStaContext.resultCode ==
			eSIR_SME_REASSOC_TIMEOUT_RESULT_CODE)) {
			pe_debug("Lim Posting eWNI_SME_REASSOC_RSP to SME"
				"resultCode: %d, status_code: %d,"
				"sessionId: %d",
				mlmStaContext.resultCode,
				mlmStaContext.protStatusCode,
				pe_session->peSessionId);

			lim_send_sme_join_reassoc_rsp(mac, eWNI_SME_REASSOC_RSP,
						      mlmStaContext.resultCode,
						      mlmStaContext.protStatusCode,
						      pe_session, smesessionId);
			if (mlmStaContext.resultCode != eSIR_SME_SUCCESS) {
				pe_delete_session(mac, pe_session);
				pe_session = NULL;
			}
		} else {
			qdf_mem_free(pe_session->lim_join_req);
			pe_session->lim_join_req = NULL;

			pe_debug("Lim Posting eWNI_SME_JOIN_RSP to SME."
				"resultCode: %d,status_code: %d,"
				"sessionId: %d",
				mlmStaContext.resultCode,
				mlmStaContext.protStatusCode,
				pe_session->peSessionId);

			lim_send_sme_join_reassoc_rsp(mac, eWNI_SME_JOIN_RSP,
						      mlmStaContext.resultCode,
						      mlmStaContext.protStatusCode,
						      pe_session, smesessionId);

			if (mlmStaContext.resultCode != eSIR_SME_SUCCESS) {
				pe_delete_session(mac, pe_session);
				pe_session = NULL;
			}
		}

	} else if (mlmStaContext.cleanupTrigger == eLIM_DUPLICATE_ENTRY) {
		/**
		 * LIM driven Disassociation.
		 * Issue Disassoc Confirm to SME.
		 */
		pe_debug("Lim Posting DISASSOC_CNF to Sme. Trigger: %d",
			mlmStaContext.cleanupTrigger);

		qdf_mem_copy((uint8_t *) &mlmDisassocCnf.peerMacAddr,
			     (uint8_t *) sta_dsaddr.bytes, QDF_MAC_ADDR_SIZE);
		mlmDisassocCnf.resultCode = status_code;
		mlmDisassocCnf.disassocTrigger = eLIM_DUPLICATE_ENTRY;
		/* Update PE session Id */
		mlmDisassocCnf.sessionId = pe_session->peSessionId;

		lim_post_sme_message(mac,
				     LIM_MLM_DISASSOC_CNF,
				     (uint32_t *) &mlmDisassocCnf);
	}

	if (pe_session && !LIM_IS_AP_ROLE(pe_session)) {
		pe_delete_session(mac, pe_session);
		pe_session = NULL;
	}
}

/**
 * lim_reject_association() - function to reject Re/Association Request
 *
 * @mac_ctx: pointer to global mac structure
 * @peer_addr: mac address of the peer
 * @sub_type: Indicates whether it is Association Request (=0) or
 *            Reassociation Request (=1) frame
 * @add_pre_auth_context:Indicates whether pre-auth context
 *                     to be added for this STA
 * @auth_type: Indicates auth type to be added
 * @sta_id: Indicates staId of the STA being rejected
 *          association
 * @delete_sta: Indicates whether to delete STA context
 *              at Polaris
 * @result_code: Indicates what reasonCode to be sent in
 *          Re/Assoc response to STA
 * @session_entry: pointer to PE session
 *
 * This function is called whenever Re/Association Request need
 * to be rejected due to failure in assigning an AID or failure
 * in adding STA context at Polaris or reject by applications.
 * Resources allocated if any are freedup and (Re) Association
 * Response frame is sent to requesting STA. Pre-Auth context
 * will be added for this STA if it does not exist already
 *
 * Return: none
 */

void
lim_reject_association(struct mac_context *mac_ctx, tSirMacAddr peer_addr,
			uint8_t sub_type, uint8_t add_pre_auth_context,
			tAniAuthType auth_type, uint16_t sta_id,
			uint8_t delete_sta, enum mac_status_code result_code,
			struct pe_session *session_entry)
{
	tpDphHashNode sta_ds;

	pe_debug("Sessionid: %d auth_type: %d sub_type: %d add_pre_auth_context: %d sta_id: %d delete_sta: %d result_code : %d peer_addr: " QDF_MAC_ADDR_STR,
		session_entry->peSessionId, auth_type, sub_type,
		add_pre_auth_context, sta_id, delete_sta, result_code,
		QDF_MAC_ADDR_ARRAY(peer_addr));

	if (add_pre_auth_context) {
		/* Create entry for this STA in pre-auth list */
		struct tLimPreAuthNode *auth_node;

		auth_node = lim_acquire_free_pre_auth_node(mac_ctx,
			       &mac_ctx->lim.gLimPreAuthTimerTable);

		if (auth_node) {
			qdf_mem_copy((uint8_t *) auth_node->peerMacAddr,
				     peer_addr, sizeof(tSirMacAddr));
			auth_node->fTimerStarted = 0;
			auth_node->mlmState = eLIM_MLM_AUTHENTICATED_STATE;
			auth_node->authType = (tAniAuthType) auth_type;
			auth_node->timestamp = qdf_mc_timer_get_system_ticks();
			lim_add_pre_auth_node(mac_ctx, auth_node);
		}
	}

	if (delete_sta == false) {
		lim_send_assoc_rsp_mgmt_frame(mac_ctx,
				eSIR_MAC_MAX_ASSOC_STA_REACHED_STATUS,
				1, peer_addr, sub_type, 0, session_entry);
		pe_warn("received Re/Assoc req when max associated STAs reached from");
		lim_print_mac_addr(mac_ctx, peer_addr, LOGW);
		lim_send_sme_max_assoc_exceeded_ntf(mac_ctx, peer_addr,
					session_entry->smeSessionId);
		return;
	}

	sta_ds = dph_get_hash_entry(mac_ctx, sta_id,
		   &session_entry->dph.dphHashTable);

	if (!sta_ds) {
		pe_err("No STA context, yet rejecting Association");
		return;
	}

	/*
	 * Polaris has state for this STA.
	 * Trigger cleanup.
	 */
	sta_ds->mlmStaContext.cleanupTrigger = eLIM_REASSOC_REJECT;

	/* Receive path cleanup */
	lim_cleanup_rx_path(mac_ctx, sta_ds, session_entry);

	/*
	 * Send Re/Association Response with
	 * status code to requesting STA.
	 */
	lim_send_assoc_rsp_mgmt_frame(mac_ctx, result_code, 0, peer_addr,
					 sub_type, 0, session_entry);

	if (session_entry->parsedAssocReq[sta_ds->assocId]) {
		uint8_t *assoc_req_frame;

		assoc_req_frame = (uint8_t *)((tpSirAssocReq) (session_entry->
			parsedAssocReq[sta_ds->assocId]))->assocReqFrame;
		/*
		 *Assoction confirmation is complete,
		 *free the copy of association request frame.
		 */
		if (assoc_req_frame) {
			qdf_mem_free(assoc_req_frame);
			assoc_req_frame = NULL;
		}

		qdf_mem_free(session_entry->parsedAssocReq[sta_ds->assocId]);
		session_entry->parsedAssocReq[sta_ds->assocId] = NULL;
	}
}

/**
 * lim_decide_ap_protection_on_ht20_delete() - function to update protection
 *                                              parameters.
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @beacon_params: ap beacon parameters
 * @session_entry: pe session entry
 *
 * protection related function while HT20 station is getting deleted.
 *
 * Return: none
 */
static void
lim_decide_ap_protection_on_ht20_delete(struct mac_context *mac_ctx,
					tpDphHashNode sta_ds,
					tpUpdateBeaconParams beacon_params,
					struct pe_session *session_entry)
{
	uint32_t i = 0;

	pe_debug("(%d) A HT 20 STA is disassociated. Addr is %pM",
		session_entry->gLimHt20Params.numSta, sta_ds->staAddr);

	if (session_entry->gLimHt20Params.numSta > 0) {
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (!session_entry->protStaCache[i].active)
				continue;

			if (!qdf_mem_cmp(session_entry->protStaCache[i].addr,
				sta_ds->staAddr, sizeof(tSirMacAddr))) {
				session_entry->gLimHt20Params.numSta--;
				session_entry->protStaCache[i].active =
						false;
				break;
			}
		}
	}

	if (session_entry->gLimHt20Params.numSta == 0) {
		/* disable protection */
		pe_debug("No 11B STA exists, PESessionID %d",
				session_entry->peSessionId);
		lim_enable_ht20_protection(mac_ctx, false, false, beacon_params,
					session_entry);
	}
}

/**
 * lim_decide_ap_protection_on_delete() - update SAP protection on station
 *                                       deletion.
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @beacon_params: ap beacon parameters
 * @session_entry: pe session entry
 *
 * Decides about protection related settings when a station is getting deleted.
 *
 * Return: none
 */
void
lim_decide_ap_protection_on_delete(struct mac_context *mac_ctx,
				   tpDphHashNode sta_ds,
				   tpUpdateBeaconParams beacon_params,
				   struct pe_session *session_entry)
{
	uint32_t phy_mode;
	tHalBitVal erp_enabled = eHAL_CLEAR;
	enum band_info rf_band = BAND_UNKNOWN;
	uint32_t i;

	if (!sta_ds)
		return;

	lim_get_rf_band_new(mac_ctx, &rf_band, session_entry);
	lim_get_phy_mode(mac_ctx, &phy_mode, session_entry);
	erp_enabled = sta_ds->erpEnabled;

	if ((BAND_5G == rf_band) &&
		(true == session_entry->htCapability) &&
		(session_entry->beaconParams.llaCoexist) &&
		(false == sta_ds->mlmStaContext.htCapability)) {
		/*
		 * we are HT. if we are 11A, then protection is not required or
		 * we are HT and 11A station is leaving.
		 * protection consideration required.
		 * HT station leaving ==> this case is commonly handled
		 * between both the bands below.
		 */
		pe_debug("(%d) A 11A STA is disassociated. Addr is %pM",
			session_entry->gLim11aParams.numSta, sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->protStaCache[i].active &&
				(!qdf_mem_cmp(
					session_entry->protStaCache[i].addr,
					 sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				session_entry->protStaCache[i].active = false;
				break;
			}
		}

		if (session_entry->gLim11aParams.numSta == 0) {
			/* disable protection */
			lim_update_11a_protection(mac_ctx, false, false,
				beacon_params, session_entry);
		}
	}

	/* we are HT or 11G and 11B station is getting deleted */
	if ((BAND_2G == rf_band) &&
		(phy_mode == WNI_CFG_PHY_MODE_11G ||
		session_entry->htCapability) &&
		(erp_enabled == eHAL_CLEAR)) {
		pe_debug("(%d) A legacy STA is disassociated. Addr is %pM",
			session_entry->gLim11bParams.numSta, sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->protStaCache[i].active &&
				(!qdf_mem_cmp(
					session_entry->protStaCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
					session_entry->gLim11bParams.numSta--;
					session_entry->protStaCache[i].active =
						 false;
				break;
			}
		}

		if (session_entry->gLim11bParams.numSta == 0) {
			/* disable protection */
			lim_enable11g_protection(mac_ctx, false, false,
						 beacon_params, session_entry);
		}
	}

	/*
	 * we are HT AP and non-11B station is leaving.
	 * 11g station is leaving
	 */
	if ((BAND_2G == rf_band) &&
		session_entry->htCapability &&
		!sta_ds->mlmStaContext.htCapability) {
		pe_debug("(%d) A 11g STA is disassociated. Addr is %pM",
			session_entry->gLim11bParams.numSta, sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->protStaCache[i].active &&
				(!qdf_mem_cmp(
					session_entry->protStaCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				session_entry->gLim11gParams.numSta--;
				session_entry->protStaCache[i].active = false;
				break;
			}
		}

		if (session_entry->gLim11gParams.numSta == 0) {
		    /* disable protection */
		    lim_enable_ht_protection_from11g(mac_ctx, false, false,
							 beacon_params,
							 session_entry);
		}
	}

	if (!((true == session_entry->htCapability) &&
		(true == sta_ds->mlmStaContext.htCapability)))
		return;

	/*
	 * Applies to 2.4 as well as 5 GHZ.
	 * HT non-GF leaving
	 */
	if (!sta_ds->htGreenfield) {
		pe_debug("(%d) A non-GF STA is disassociated. Addr is %pM",
			session_entry->gLimNonGfParams.numSta, sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->protStaCache[i].active &&
				(!qdf_mem_cmp(
					session_entry->protStaCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				session_entry->protStaCache[i].active = false;
				break;
			}
		}

		if (session_entry->gLimNonGfParams.numSta == 0) {
			/* disable protection */
			lim_enable_ht_non_gf_protection(mac_ctx, false, false,
					beacon_params, session_entry);
		}
	}

	/*
	 * Applies to 2.4 as well as 5 GHZ.
	 * HT 20Mhz station leaving
	 */
	if (session_entry->beaconParams.ht20Coexist &&
		(eHT_CHANNEL_WIDTH_20MHZ ==
			 sta_ds->htSupportedChannelWidthSet)) {
		lim_decide_ap_protection_on_ht20_delete(mac_ctx, sta_ds,
					beacon_params, session_entry);
	}

	/*
	 * Applies to 2.4 as well as 5 GHZ.
	 * LSIG TXOP not supporting staiton leaving
	 */
	if ((false == session_entry->beaconParams.
				fLsigTXOPProtectionFullSupport) &&
		(false == sta_ds->htLsigTXOPProtection)) {
		pe_debug("(%d) A HT LSIG not supporting STA is disassociated. Addr is %pM",
			session_entry->gLimLsigTxopParams.numSta,
			sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->protStaCache[i].active &&
				(!qdf_mem_cmp(
					session_entry->protStaCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				session_entry->protStaCache[i].active = false;
				break;
			}
		}

		if (session_entry->gLimLsigTxopParams.numSta == 0) {
			/* disable protection */
			lim_enable_ht_lsig_txop_protection(mac_ctx, true,
				false, beacon_params, session_entry);
		}
	}
}

/**
 * lim_decide_short_preamble() - update short preamble parameters
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @beacon_params: ap beacon parameters
 * @session_entry: pe session entry
 *
 * Decides about any short preamble related change because of new station
 * joining.
 *
 * Return: None
 */
static void lim_decide_short_preamble(struct mac_context *mac_ctx,
				      tpDphHashNode sta_ds,
				      tpUpdateBeaconParams beacon_params,
				      struct pe_session *session_entry)
{
	uint32_t i;

	if (sta_ds->shortPreambleEnabled == eHAL_CLEAR) {
		pe_debug("(%d) A non-short preamble STA is disassociated. Addr is %pM",
		       session_entry->gLimNoShortParams.numNonShortPreambleSta,
			 sta_ds->staAddr);
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->gLimNoShortParams.
				staNoShortCache[i].active &&
				(!qdf_mem_cmp(session_entry->
					gLimNoShortParams.
					staNoShortCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				session_entry->gLimNoShortParams.
					numNonShortPreambleSta--;
				session_entry->gLimNoShortParams.
					staNoShortCache[i].active = false;
				break;
			}
		}

		if (session_entry->gLimNoShortParams.numNonShortPreambleSta)
			return;

		/*
		 * enable short preamble
		 * reset the cache
		 */
		qdf_mem_zero((uint8_t *) &session_entry->gLimNoShortParams,
				sizeof(tLimNoShortParams));
		if (lim_enable_short_preamble(mac_ctx, true,
			beacon_params, session_entry) != QDF_STATUS_SUCCESS)
			pe_err("Cannot enable short preamble");
	}
}

/**
 * lim_decide_short_slot() - update short slot time related  parameters
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @beacon_params: ap beacon parameters
 * @session_entry: pe session entry
 *
 * Decides about any short slot time related change because of station leaving
 *        the BSS.
 * Return: None
 */
static void
lim_decide_short_slot(struct mac_context *mac_ctx, tpDphHashNode sta_ds,
		      tpUpdateBeaconParams beacon_params,
		      struct pe_session *session_entry)
{
	uint32_t i, val, non_short_slot_sta_count;

	if (sta_ds->shortSlotTimeEnabled != eHAL_CLEAR)
		return;

	pe_debug("(%d) A non-short slottime STA is disassociated. Addr is %pM",
		mac_ctx->lim.gLimNoShortSlotParams.numNonShortSlotSta,
		sta_ds->staAddr);

	val = mac_ctx->mlme_cfg->feature_flags.enable_short_slot_time_11g;

	if (LIM_IS_AP_ROLE(session_entry)) {
		non_short_slot_sta_count =
		      session_entry->gLimNoShortSlotParams.numNonShortSlotSta;
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (session_entry->gLimNoShortSlotParams.
				staNoShortSlotCache[i].active &&
				(!qdf_mem_cmp(session_entry->
					gLimNoShortSlotParams.
						staNoShortSlotCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				non_short_slot_sta_count--;
				session_entry->gLimNoShortSlotParams.
					staNoShortSlotCache[i].active = false;
				break;
			}
		}

		if (non_short_slot_sta_count == 0 && val) {
			/*
			 * enable short slot time
			 * reset the cache
			 */
			qdf_mem_zero((uint8_t *) &session_entry->
				gLimNoShortSlotParams,
				sizeof(tLimNoShortSlotParams));
			beacon_params->fShortSlotTime = true;
			beacon_params->paramChangeBitmap |=
				PARAM_SHORT_SLOT_TIME_CHANGED;
			session_entry->shortSlotTimeSupported = true;
		}
		session_entry->gLimNoShortSlotParams.numNonShortSlotSta =
			non_short_slot_sta_count;
	} else {
		non_short_slot_sta_count =
			mac_ctx->lim.gLimNoShortSlotParams.numNonShortSlotSta;
		for (i = 0; i < LIM_PROT_STA_CACHE_SIZE; i++) {
			if (mac_ctx->lim.gLimNoShortSlotParams.
				staNoShortSlotCache[i].active &&
				(!qdf_mem_cmp(
					mac_ctx->lim.gLimNoShortSlotParams.
						staNoShortSlotCache[i].addr,
					sta_ds->staAddr,
					sizeof(tSirMacAddr)))) {
				non_short_slot_sta_count--;
				mac_ctx->lim.gLimNoShortSlotParams.
					staNoShortSlotCache[i].active = false;
				break;
			}
		}

		if (val && !non_short_slot_sta_count) {
			/*
			 * enable short slot time
			 * reset the cache
			 */
			qdf_mem_zero(
				(uint8_t *) &mac_ctx->lim.gLimNoShortSlotParams,
				sizeof(tLimNoShortSlotParams));
			/*in case of AP set SHORT_SLOT_TIME to enable*/
			if (LIM_IS_AP_ROLE(session_entry)) {
				beacon_params->fShortSlotTime = true;
				beacon_params->paramChangeBitmap |=
					PARAM_SHORT_SLOT_TIME_CHANGED;
				session_entry->shortSlotTimeSupported = true;
			}
		}
		mac_ctx->lim.gLimNoShortSlotParams.numNonShortSlotSta =
			non_short_slot_sta_count;
	}
}

QDF_STATUS lim_populate_vht_mcs_set(struct mac_context *mac_ctx,
				    struct supported_rates *rates,
				    tDot11fIEVHTCaps *peer_vht_caps,
				    struct pe_session *session_entry,
				    uint8_t nss)
{
	uint32_t self_sta_dot11mode = 0;
	uint16_t mcs_map_mask = MCSMAPMASK1x1;
	uint16_t mcs_map_mask2x2 = 0;
	struct mlme_vht_capabilities_info *vht_cap_info;

	self_sta_dot11mode = mac_ctx->mlme_cfg->dot11_mode.dot11_mode;

	if (!IS_DOT11_MODE_VHT(self_sta_dot11mode))
		return QDF_STATUS_SUCCESS;

	vht_cap_info = &mac_ctx->mlme_cfg->vht_caps.vht_cap_info;

	rates->vhtRxMCSMap = (uint16_t)vht_cap_info->rx_mcs_map;
	rates->vhtTxMCSMap = (uint16_t)vht_cap_info->tx_mcs_map;
	rates->vhtRxHighestDataRate =
			(uint16_t)vht_cap_info->rx_supp_data_rate;
	rates->vhtTxHighestDataRate =
			(uint16_t)vht_cap_info->tx_supp_data_rate;

	if (NSS_1x1_MODE == nss) {
		rates->vhtRxMCSMap |= VHT_MCS_1x1;
		rates->vhtTxMCSMap |= VHT_MCS_1x1;
		rates->vhtTxHighestDataRate =
			VHT_TX_HIGHEST_SUPPORTED_DATA_RATE_1_1;
		rates->vhtRxHighestDataRate =
			VHT_RX_HIGHEST_SUPPORTED_DATA_RATE_1_1;
		if (session_entry && !session_entry->ch_width &&
		    !vht_cap_info->enable_vht20_mcs9 &&
		    ((rates->vhtRxMCSMap & VHT_1x1_MCS_MASK) ==
				 VHT_1x1_MCS9_MAP)) {
			DISABLE_VHT_MCS_9(rates->vhtRxMCSMap,
					NSS_1x1_MODE);
			DISABLE_VHT_MCS_9(rates->vhtTxMCSMap,
					NSS_1x1_MODE);
		}
	} else {
		if (session_entry && !session_entry->ch_width &&
			!vht_cap_info->enable_vht20_mcs9 &&
			((rates->vhtRxMCSMap & VHT_2x2_MCS_MASK) ==
			VHT_2x2_MCS9_MAP)) {
			DISABLE_VHT_MCS_9(rates->vhtRxMCSMap,
					NSS_2x2_MODE);
			DISABLE_VHT_MCS_9(rates->vhtTxMCSMap,
					NSS_2x2_MODE);
		}
	}

	if ((!peer_vht_caps) || (!peer_vht_caps->present))
		return QDF_STATUS_SUCCESS;

	rates->vhtTxHighestDataRate =
		QDF_MIN(rates->vhtTxHighestDataRate,
			peer_vht_caps->txSupDataRate);
	rates->vhtRxHighestDataRate =
		QDF_MIN(rates->vhtRxHighestDataRate,
			peer_vht_caps->rxHighSupDataRate);

	if (session_entry && session_entry->nss == NSS_2x2_MODE)
		mcs_map_mask2x2 = MCSMAPMASK2x2;

	if ((peer_vht_caps->txMCSMap & mcs_map_mask) <
	    (rates->vhtRxMCSMap & mcs_map_mask)) {
		rates->vhtRxMCSMap &= ~(mcs_map_mask);
		rates->vhtRxMCSMap |=
			(peer_vht_caps->txMCSMap & mcs_map_mask);
	}
	if ((peer_vht_caps->rxMCSMap & mcs_map_mask) <
	    (rates->vhtTxMCSMap & mcs_map_mask)) {
		rates->vhtTxMCSMap &= ~(mcs_map_mask);
		rates->vhtTxMCSMap |=
			(peer_vht_caps->rxMCSMap & mcs_map_mask);
	}

	if (mcs_map_mask2x2) {

		uint16_t peer_mcs_map, self_mcs_map;

		peer_mcs_map =
			peer_vht_caps->txMCSMap & mcs_map_mask2x2;
		self_mcs_map =
			rates->vhtRxMCSMap & mcs_map_mask2x2;

		if ((self_mcs_map != mcs_map_mask2x2) &&
		    ((peer_mcs_map == mcs_map_mask2x2) ||
		     (peer_mcs_map < self_mcs_map))) {
			rates->vhtRxMCSMap &= ~mcs_map_mask2x2;
			rates->vhtRxMCSMap |= peer_mcs_map;
		}

		peer_mcs_map =
		 (peer_vht_caps->rxMCSMap & mcs_map_mask2x2);
		self_mcs_map =
			(rates->vhtTxMCSMap & mcs_map_mask2x2);

		if ((self_mcs_map != mcs_map_mask2x2) &&
		    ((peer_mcs_map == mcs_map_mask2x2) ||
		     (peer_mcs_map < self_mcs_map))) {
			rates->vhtTxMCSMap &= ~mcs_map_mask2x2;
			rates->vhtTxMCSMap |= peer_mcs_map;
		}
	}

	pe_debug("enable2x2 - %d nss %d vhtRxMCSMap - %x vhtTxMCSMap - %x",
		vht_cap_info->enable2x2, nss,
		rates->vhtRxMCSMap, rates->vhtTxMCSMap);

	if (session_entry) {
		session_entry->supported_nss_1x1 =
			((rates->vhtTxMCSMap & VHT_MCS_1x1) ==
			 VHT_MCS_1x1) ? true : false;
		pe_debug("VHT supported nss 1x1: %d",
		       session_entry->supported_nss_1x1);
	}

	return QDF_STATUS_SUCCESS;
}

QDF_STATUS lim_populate_own_rate_set(struct mac_context *mac_ctx,
				     struct supported_rates *rates,
				     uint8_t *supported_mcs_set,
				     uint8_t basic_only,
				     struct pe_session *session_entry,
				     struct sDot11fIEVHTCaps *vht_caps,
				     struct sDot11fIEhe_cap *he_caps)
{
	tSirMacRateSet temp_rate_set;
	tSirMacRateSet temp_rate_set2;
	uint32_t i, j, val, min, is_arate;
	uint32_t phy_mode = 0;
	uint32_t self_sta_dot11mode = 0;
	uint8_t a_rate_index = 0;
	uint8_t b_rate_index = 0;
	qdf_size_t val_len;

	is_arate = 0;

	self_sta_dot11mode = mac_ctx->mlme_cfg->dot11_mode.dot11_mode;
	lim_get_phy_mode(mac_ctx, &phy_mode, session_entry);

	/*
	 * Include 11b rates only when the device configured in
	 *  auto, 11a/b/g or 11b_only
	 */
	if ((self_sta_dot11mode == MLME_DOT11_MODE_ALL) ||
	    (self_sta_dot11mode == MLME_DOT11_MODE_11A) ||
	    (self_sta_dot11mode == MLME_DOT11_MODE_11AC) ||
	    (self_sta_dot11mode == MLME_DOT11_MODE_11N) ||
	    (self_sta_dot11mode == MLME_DOT11_MODE_11G) ||
	    (self_sta_dot11mode == MLME_DOT11_MODE_11B)) {
		val_len = mac_ctx->mlme_cfg->rates.supported_11b.len;
		wlan_mlme_get_cfg_str((uint8_t *)&temp_rate_set.rate,
				      &mac_ctx->mlme_cfg->rates.supported_11b,
				      &val_len);
		temp_rate_set.numRates = (uint8_t)val_len;
	} else {
		temp_rate_set.numRates = 0;
	}

	/* Include 11a rates when the device configured in non-11b mode */
	if (!IS_DOT11_MODE_11B(self_sta_dot11mode)) {
		val_len = mac_ctx->mlme_cfg->rates.supported_11a.len;
		wlan_mlme_get_cfg_str((uint8_t *)&temp_rate_set2.rate,
				      &mac_ctx->mlme_cfg->rates.supported_11a,
				      &val_len);
		temp_rate_set2.numRates = (uint8_t)val_len;
	} else {
		temp_rate_set2.numRates = 0;
	}

	if ((temp_rate_set.numRates + temp_rate_set2.numRates) > 12) {
		pe_err("more than 12 rates in CFG");
		return QDF_STATUS_E_FAILURE;
	}
	/* copy all rates in temp_rate_set, there are 12 rates max */
	for (i = 0; i < temp_rate_set2.numRates; i++)
		temp_rate_set.rate[i + temp_rate_set.numRates] =
			temp_rate_set2.rate[i];

	temp_rate_set.numRates += temp_rate_set2.numRates;

	/**
	 * Sort rates in temp_rate_set (they are likely to be already sorted)
	 * put the result in pSupportedRates
	 */

	qdf_mem_zero(rates, sizeof(*rates));
	for (i = 0; i < temp_rate_set.numRates; i++) {
		min = 0;
		val = 0xff;
		is_arate = 0;

		for (j = 0; (j < temp_rate_set.numRates) &&
			 (j < WLAN_SUPPORTED_RATES_IE_MAX_LEN); j++) {
			if ((uint32_t) (temp_rate_set.rate[j] & 0x7f) <
					val) {
				val = temp_rate_set.rate[j] & 0x7f;
				min = j;
			}
		}

		if (sirIsArate(temp_rate_set.rate[min] & 0x7f))
			is_arate = 1;

		if (is_arate)
			rates->llaRates[a_rate_index++] =
						temp_rate_set.rate[min];
		else
			rates->llbRates[b_rate_index++] =
						temp_rate_set.rate[min];
		temp_rate_set.rate[min] = 0xff;
	}

	if (IS_DOT11_MODE_HT(self_sta_dot11mode)) {
		val_len = SIZE_OF_SUPPORTED_MCS_SET;
		if (wlan_mlme_get_cfg_str(
			rates->supportedMCSSet,
			&mac_ctx->mlme_cfg->rates.supported_mcs_set,
			&val_len) != QDF_STATUS_SUCCESS) {
			pe_err("could not retrieve supportedMCSSet");
			return QDF_STATUS_E_FAILURE;
		}

		if (session_entry->nss == NSS_1x1_MODE)
			rates->supportedMCSSet[1] = 0;
		/*
		 * if supported MCS Set of the peer is passed in,
		 * then do the intersection
		 * else use the MCS set from local CFG.
		 */

		if (supported_mcs_set) {
			for (i = 0; i < SIR_MAC_MAX_SUPPORTED_MCS_SET; i++)
				rates->supportedMCSSet[i] &=
					 supported_mcs_set[i];
		}

		pe_debug("MCS Rate Set Bitmap: ");
		for (i = 0; i < SIR_MAC_MAX_SUPPORTED_MCS_SET; i++)
			pe_debug("%x ", rates->supportedMCSSet[i]);
	}
	lim_populate_vht_mcs_set(mac_ctx, rates, vht_caps,
			session_entry, session_entry->nss);
	lim_populate_he_mcs_set(mac_ctx, rates, he_caps,
			session_entry, session_entry->nss);

	return QDF_STATUS_SUCCESS;
}

#ifdef WLAN_FEATURE_11AX
/**
 * lim_calculate_he_nss() - function to calculate new nss from he rates
 * @rates: supported rtes struct object
 * @session: pe session entry
 * This function calculates nss from rx_he_mcs_map_lt_80 within rates struct
 * object and assigns new value to nss within pe_session
 *
 * Return: None
 */
static void lim_calculate_he_nss(struct supported_rates *rates,
				 struct pe_session *session)
{
	HE_GET_NSS(rates->rx_he_mcs_map_lt_80, session->nss);
}
#else
static void lim_calculate_he_nss(struct supported_rates *rates,
				 struct pe_session *session)
{
}
#endif

QDF_STATUS lim_populate_peer_rate_set(struct mac_context *mac,
				      struct supported_rates *pRates,
				      uint8_t *pSupportedMCSSet,
				      uint8_t basicOnly,
				      struct pe_session *pe_session,
				      tDot11fIEVHTCaps *pVHTCaps,
				      tDot11fIEhe_cap *he_caps)
{
	tSirMacRateSet tempRateSet;
	tSirMacRateSet tempRateSet2;
	uint32_t i, j, val, min, isArate = 0;
	qdf_size_t val_len;

	/* copy operational rate set from pe_session */
	if (pe_session->rateSet.numRates <= WLAN_SUPPORTED_RATES_IE_MAX_LEN) {
		qdf_mem_copy((uint8_t *) tempRateSet.rate,
			     (uint8_t *) (pe_session->rateSet.rate),
			     pe_session->rateSet.numRates);
		tempRateSet.numRates = pe_session->rateSet.numRates;
	} else {
		pe_err("more than WLAN_SUPPORTED_RATES_IE_MAX_LEN rates");
		return QDF_STATUS_E_FAILURE;
	}
	if ((pe_session->dot11mode == MLME_DOT11_MODE_11G) ||
		(pe_session->dot11mode == MLME_DOT11_MODE_11A) ||
		(pe_session->dot11mode == MLME_DOT11_MODE_11AC) ||
		(pe_session->dot11mode == MLME_DOT11_MODE_11N)) {
		if (pe_session->extRateSet.numRates <=
		    WLAN_SUPPORTED_RATES_IE_MAX_LEN) {
			qdf_mem_copy((uint8_t *) tempRateSet2.rate,
				     (uint8_t *) (pe_session->extRateSet.
						  rate),
				     pe_session->extRateSet.numRates);
			tempRateSet2.numRates =
				pe_session->extRateSet.numRates;
		} else {
			pe_err("pe_session->extRateSet.numRates more than WLAN_SUPPORTED_RATES_IE_MAX_LEN rates");
			return QDF_STATUS_E_FAILURE;
		}
	} else
		tempRateSet2.numRates = 0;
	if ((tempRateSet.numRates + tempRateSet2.numRates) >
	    WLAN_SUPPORTED_RATES_IE_MAX_LEN) {
		pe_err("more than 12 rates in CFG");
		return QDF_STATUS_E_FAILURE;
	}

	/* copy all rates in tempRateSet, there are 12 rates max */
	for (i = 0; i < tempRateSet2.numRates; i++)
		tempRateSet.rate[i + tempRateSet.numRates] =
			tempRateSet2.rate[i];
	tempRateSet.numRates += tempRateSet2.numRates;
	/**
	 * Sort rates in tempRateSet (they are likely to be already sorted)
	 * put the result in pSupportedRates
	 */
	{
		uint8_t aRateIndex = 0;
		uint8_t bRateIndex = 0;

		qdf_mem_zero(pRates, sizeof(*pRates));
		for (i = 0; i < tempRateSet.numRates; i++) {
			min = 0;
			val = 0xff;
			isArate = 0;
			for (j = 0;
			     (j < tempRateSet.numRates)
			     && (j < WLAN_SUPPORTED_RATES_IE_MAX_LEN); j++) {
				if ((uint32_t) (tempRateSet.rate[j] & 0x7f) <
				    val) {
					val = tempRateSet.rate[j] & 0x7f;
					min = j;
				}
			}
			if (sirIsArate(tempRateSet.rate[min] & 0x7f))
				isArate = 1;
			/*
			 * HAL needs to know whether the rate is basic rate or not, as it needs to
			 * update the response rate table accordingly. e.g. if one of the 11a rates is
			 * basic rate, then that rate can be used for sending control frames.
			 * HAL updates the response rate table whenever basic rate set is changed.
			 */
			if (basicOnly) {
				if (tempRateSet.rate[min] & 0x80) {
					if (isArate)
						pRates->llaRates[aRateIndex++] =
							tempRateSet.rate[min];
					else
						pRates->llbRates[bRateIndex++] =
							tempRateSet.rate[min];
				}
			} else {
				if (isArate)
					pRates->llaRates[aRateIndex++] =
						tempRateSet.rate[min];
				else
					pRates->llbRates[bRateIndex++] =
						tempRateSet.rate[min];
			}
			tempRateSet.rate[min] = 0xff;
		}
	}

	if (IS_DOT11_MODE_HT(pe_session->dot11mode)) {
		val_len = SIZE_OF_SUPPORTED_MCS_SET;
		if (wlan_mlme_get_cfg_str(
			pRates->supportedMCSSet,
			&mac->mlme_cfg->rates.supported_mcs_set,
			&val_len) != QDF_STATUS_SUCCESS) {
			pe_err("could not retrieve supportedMCSSet");
			return QDF_STATUS_E_FAILURE;
		}
		if (pe_session->nss == NSS_1x1_MODE)
			pRates->supportedMCSSet[1] = 0;

		/* if supported MCS Set of the peer is passed in, then do the
		 * intersection, else use the MCS set from local CFG.
		 */
		if (pSupportedMCSSet) {
			for (i = 0; i < SIR_MAC_MAX_SUPPORTED_MCS_SET; i++)
				pRates->supportedMCSSet[i] &=
					pSupportedMCSSet[i];
		}
		pe_debug("MCS Rate Set Bitmap: ");
		for (i = 0; i < SIR_MAC_MAX_SUPPORTED_MCS_SET; i++)
			pe_debug("%x ", pRates->supportedMCSSet[i]);

		if (pRates->supportedMCSSet[0] == 0) {
			pe_debug("Incorrect MCS 0 - 7. They must be supported");
			pRates->supportedMCSSet[0] = 0xFF;
		}

		pe_session->supported_nss_1x1 =
			((pRates->supportedMCSSet[1] != 0) ? false : true);
		pe_debug("HT supported nss 1x1: %d",
			pe_session->supported_nss_1x1);
	}
	lim_populate_vht_mcs_set(mac, pRates, pVHTCaps,
			pe_session, pe_session->nss);

	lim_populate_he_mcs_set(mac, pRates, he_caps,
			pe_session, pe_session->nss);

	if (IS_DOT11_MODE_HE(pe_session->dot11mode) && he_caps) {
		lim_calculate_he_nss(pRates, pe_session);
	} else if (IS_DOT11_MODE_VHT(pe_session->dot11mode)) {
		if ((pRates->vhtRxMCSMap & MCSMAPMASK2x2) == MCSMAPMASK2x2)
			pe_session->nss = NSS_1x1_MODE;
	} else if (pRates->supportedMCSSet[1] == 0) {
		pe_session->nss = NSS_1x1_MODE;
	}
	pe_debug("nss: %d", pe_session->nss);

	return QDF_STATUS_SUCCESS;
} /*** lim_populate_peer_rate_set() ***/

/**
 * lim_populate_matching_rate_set() -process the CFG rate sets and
 *          the rate sets received in the Assoc request on AP.
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @oper_rate_set: pointer to operating rate set
 * @ext_rate_set: pointer to extended rate set
 * @supported_mcs_set: pointer to supported rate set
 * @session_entry: pointer to pe session entry
 * @vht_caps: pointer to vht capabilities
 *
 * This is called at the time of Association Request
 * processing on AP and while adding peer's context
 * in IBSS role to process the CFG rate sets and
 * the rate sets received in the Assoc request on AP
 * or Beacon/Probe Response from peer in IBSS.
 *
 * 1. It makes the intersection between our own rate Sat
 *    and extemcded rate set and the ones received in the
 *    association request.
 * 2. It creates a combined rate set of 12 rates max which
 *    comprised the basic and extended rates
 * 3. It sorts the combined rate Set and copy it in the
 *    rate array of the pSTA descriptor
 *
 * The parser has already ensured unicity of the rates in the
 * association request structure
 *
 * Return: QDF_STATUS_SUCCESS on success else QDF_STATUS_E_FAILURE
 */
QDF_STATUS lim_populate_matching_rate_set(struct mac_context *mac_ctx,
					  tpDphHashNode sta_ds,
					  tSirMacRateSet *oper_rate_set,
					  tSirMacRateSet *ext_rate_set,
					  uint8_t *supported_mcs_set,
					  struct pe_session *session_entry,
					  tDot11fIEVHTCaps *vht_caps,
					  tDot11fIEhe_cap *he_caps)
{
	tSirMacRateSet temp_rate_set;
	tSirMacRateSet temp_rate_set2;
	uint32_t i, j, val, min, is_arate;
	uint32_t phy_mode;
	uint8_t mcs_set[SIZE_OF_SUPPORTED_MCS_SET];
	struct supported_rates *rates;
	uint8_t a_rate_index = 0;
	uint8_t b_rate_index = 0;
	qdf_size_t val_len;

	is_arate = 0;

	lim_get_phy_mode(mac_ctx, &phy_mode, session_entry);

	/* copy operational rate set from session_entry */
	qdf_mem_copy((temp_rate_set.rate), (session_entry->rateSet.rate),
		     session_entry->rateSet.numRates);
	temp_rate_set.numRates = (uint8_t) session_entry->rateSet.numRates;

	if (phy_mode == WNI_CFG_PHY_MODE_11G) {
		qdf_mem_copy((temp_rate_set2.rate),
			     (session_entry->extRateSet.rate),
			     session_entry->extRateSet.numRates);
		temp_rate_set2.numRates =
			(uint8_t) session_entry->extRateSet.numRates;
	} else {
		temp_rate_set2.numRates = 0;
	}

	/*
	 * absolute sum of both num_rates should be less than 12. following
	 * 16-bit sum avoids false codition where 8-bit arthematic overflow
	 * might have caused total sum to be less than 12
	 */
	if (((uint16_t)temp_rate_set.numRates +
		(uint16_t)temp_rate_set2.numRates) > 12) {
		pe_err("more than 12 rates in CFG");
		return QDF_STATUS_E_FAILURE;
	}

	/*
	 * Handling of the rate set IEs is the following:
	 * - keep only rates that we support and that the station supports
	 * - sort and the rates into the pSta->rate array
	 */

	/* Copy all rates in temp_rate_set, there are 12 rates max */
	for (i = 0; i < temp_rate_set2.numRates; i++)
		temp_rate_set.rate[i + temp_rate_set.numRates] =
			temp_rate_set2.rate[i];

	temp_rate_set.numRates += temp_rate_set2.numRates;

	/*
	 * Sort rates in temp_rate_set (they are likely to be already sorted)
	 * put the result in temp_rate_set2
	 */
	temp_rate_set2.numRates = 0;

	for (i = 0; i < temp_rate_set.numRates; i++) {
		min = 0;
		val = 0xff;

		for (j = 0; j < temp_rate_set.numRates; j++)
			if ((uint32_t) (temp_rate_set.rate[j] & 0x7f) < val) {
				val = temp_rate_set.rate[j] & 0x7f;
				min = j;
			}

		temp_rate_set2.rate[temp_rate_set2.numRates++] =
			temp_rate_set.rate[min];
		temp_rate_set.rate[min] = 0xff;
	}

	/*
	 * Copy received rates in temp_rate_set, the parser has ensured
	 * unicity of the rates so there cannot be more than 12
	 */
	for (i = 0; (i < oper_rate_set->numRates &&
			 i < WLAN_SUPPORTED_RATES_IE_MAX_LEN); i++)
		temp_rate_set.rate[i] = oper_rate_set->rate[i];

	temp_rate_set.numRates = oper_rate_set->numRates;

	pe_debug("Sum of SUPPORTED and EXTENDED Rate Set (%1d)",
		temp_rate_set.numRates + ext_rate_set->numRates);

	if (ext_rate_set->numRates &&
		((temp_rate_set.numRates + ext_rate_set->numRates) > 12) &&
		temp_rate_set.numRates < 12) {
		int found = 0;
		int tail = temp_rate_set.numRates;

		for (i = 0; (i < ext_rate_set->numRates &&
				i < WLAN_SUPPORTED_RATES_IE_MAX_LEN); i++) {
			found = 0;
			for (j = 0; j < (uint32_t) tail; j++) {
				if ((temp_rate_set.rate[j] & 0x7F) ==
					(ext_rate_set->rate[i] & 0x7F)) {
					found = 1;
					break;
				}
			}

			if (!found) {
				temp_rate_set.rate[temp_rate_set.numRates++] =
						ext_rate_set->rate[i];
				if (temp_rate_set.numRates >= 12)
					break;
			}
		}
	} else if (ext_rate_set->numRates &&
		 ((temp_rate_set.numRates + ext_rate_set->numRates) <= 12)) {
		for (j = 0; ((j < ext_rate_set->numRates) &&
				 (j < WLAN_SUPPORTED_RATES_IE_MAX_LEN) &&
				 ((i + j) < WLAN_SUPPORTED_RATES_IE_MAX_LEN)); j++)
			temp_rate_set.rate[i + j] = ext_rate_set->rate[j];

		temp_rate_set.numRates += ext_rate_set->numRates;
	} else if (ext_rate_set->numRates) {
		pe_debug("Relying only on the SUPPORTED Rate Set IE");
	}

	rates = &sta_ds->supportedRates;
	qdf_mem_zero(rates, sizeof(*rates));
	for (i = 0; (i < temp_rate_set2.numRates &&
			 i < WLAN_SUPPORTED_RATES_IE_MAX_LEN); i++) {
		for (j = 0; (j < temp_rate_set.numRates &&
				 j < WLAN_SUPPORTED_RATES_IE_MAX_LEN); j++) {
			if ((temp_rate_set2.rate[i] & 0x7F) !=
				(temp_rate_set.rate[j] & 0x7F))
				continue;

			if (sirIsArate(temp_rate_set2.rate[i] & 0x7f) &&
				a_rate_index < SIR_NUM_11A_RATES) {
				is_arate = 1;
				rates->llaRates[a_rate_index++] =
							temp_rate_set2.rate[i];
			} else if ((b_rate_index < SIR_NUM_11B_RATES) &&
				!(sirIsArate(temp_rate_set2.rate[i] & 0x7f))) {
				rates->llbRates[b_rate_index++] =
					temp_rate_set2.rate[i];
			}
			break;
		}
	}

	/*
	 * Now add the Polaris rates only when Proprietary rates are enabled.
	 * compute the matching MCS rate set, if peer is 11n capable and self
	 * mode is 11n
	 */
#ifdef FEATURE_WLAN_TDLS
	if (sta_ds->mlmStaContext.htCapability)
#else
	if (IS_DOT11_MODE_HT(session_entry->dot11mode) &&
		(sta_ds->mlmStaContext.htCapability))
#endif
	{
		val_len = SIZE_OF_SUPPORTED_MCS_SET;
		if (wlan_mlme_get_cfg_str(
			mcs_set,
			&mac_ctx->mlme_cfg->rates.supported_mcs_set,
			&val_len) != QDF_STATUS_SUCCESS) {
			pe_err("could not retrieve supportedMCSet");
			return QDF_STATUS_E_FAILURE;
		}

		if (session_entry->nss == NSS_1x1_MODE)
			mcs_set[1] = 0;

		for (i = 0; i < val_len; i++)
			sta_ds->supportedRates.supportedMCSSet[i] =
				mcs_set[i] & supported_mcs_set[i];

		pe_debug("MCS Rate Set Bitmap from CFG and DPH: ");
		for (i = 0; i < SIR_MAC_MAX_SUPPORTED_MCS_SET; i++) {
			pe_debug("%x %x ", mcs_set[i],
			    sta_ds->supportedRates.supportedMCSSet[i]);
		}
	}
	lim_populate_vht_mcs_set(mac_ctx, &sta_ds->supportedRates, vht_caps,
				 session_entry, session_entry->nss);
	lim_populate_he_mcs_set(mac_ctx, &sta_ds->supportedRates, he_caps,
				session_entry, session_entry->nss);
	/*
	 * Set the erpEnabled bit if the phy is in G mode and at least
	 * one A rate is supported
	 */
	if ((phy_mode == WNI_CFG_PHY_MODE_11G) && is_arate)
		sta_ds->erpEnabled = eHAL_SET;

	return QDF_STATUS_SUCCESS;
}

/**
 * lim_populate_vht_caps() - populates vht capabilities based on input
 *        capabilities
 * @input_caps: input capabilities based on which we format the vht
 *      capabilities
 *
 * function to populate the supported vht capabilities.
 *
 * Return: vht capabilities derived based on input parameters.
 */
static uint32_t lim_populate_vht_caps(tDot11fIEVHTCaps input_caps)
{
	uint32_t vht_caps;

	vht_caps = ((input_caps.maxMPDULen << SIR_MAC_VHT_CAP_MAX_MPDU_LEN) |
			(input_caps.supportedChannelWidthSet <<
				 SIR_MAC_VHT_CAP_SUPP_CH_WIDTH_SET) |
			(input_caps.ldpcCodingCap <<
				SIR_MAC_VHT_CAP_LDPC_CODING_CAP) |
			(input_caps.shortGI80MHz <<
				 SIR_MAC_VHT_CAP_SHORTGI_80MHZ) |
			(input_caps.shortGI160and80plus80MHz <<
				SIR_MAC_VHT_CAP_SHORTGI_160_80_80MHZ) |
			(input_caps.txSTBC << SIR_MAC_VHT_CAP_TXSTBC) |
			(input_caps.rxSTBC << SIR_MAC_VHT_CAP_RXSTBC) |
			(input_caps.suBeamFormerCap <<
				SIR_MAC_VHT_CAP_SU_BEAMFORMER_CAP) |
			(input_caps.suBeamformeeCap <<
				SIR_MAC_VHT_CAP_SU_BEAMFORMEE_CAP) |
			(input_caps.csnofBeamformerAntSup <<
				SIR_MAC_VHT_CAP_CSN_BEAMORMER_ANT_SUP) |
			(input_caps.numSoundingDim <<
				SIR_MAC_VHT_CAP_NUM_SOUNDING_DIM) |
			(input_caps.muBeamformerCap <<
				SIR_MAC_VHT_CAP_NUM_BEAM_FORMER_CAP) |
			(input_caps.muBeamformeeCap <<
				SIR_MAC_VHT_CAP_NUM_BEAM_FORMEE_CAP) |
			(input_caps.vhtTXOPPS <<
				SIR_MAC_VHT_CAP_TXOPPS) |
			(input_caps.htcVHTCap <<
				 SIR_MAC_VHT_CAP_HTC_CAP) |
			(input_caps.maxAMPDULenExp <<
				SIR_MAC_VHT_CAP_MAX_AMDU_LEN_EXPO) |
			(input_caps.vhtLinkAdaptCap <<
				SIR_MAC_VHT_CAP_LINK_ADAPT_CAP) |
			(input_caps.rxAntPattern <<
				SIR_MAC_VHT_CAP_RX_ANTENNA_PATTERN) |
			(input_caps.txAntPattern <<
				SIR_MAC_VHT_CAP_TX_ANTENNA_PATTERN) |
			(input_caps.reserved1 <<
				SIR_MAC_VHT_CAP_RESERVED2));

	return vht_caps;
}

/**
 * lim_update_he_stbc_capable() - Update stbc capable flag based on
 * HE capability
 * @add_sta_params: add sta related parameters
 *
 * Update stbc cpable flag based on HE capability
 *
 * Return: None
 */
#ifdef WLAN_FEATURE_11AX
static void lim_update_he_stbc_capable(tpAddStaParams add_sta_params)
{
	if (add_sta_params &&
	    add_sta_params->he_capable &&
	    add_sta_params->stbc_capable)
		add_sta_params->stbc_capable =
			add_sta_params->he_config.rx_stbc_lt_80mhz;
}
#else
static void lim_update_he_stbc_capable(tpAddStaParams add_sta_params)
{}
#endif

/**
 * lim_add_sta()- called to add an STA context at hardware
 * @mac_ctx: pointer to global mac structure
 * @sta_ds: station node
 * @update_entry: set to true for updating the entry
 * @session_entry: pe session entry
 *
 * This function is called to add an STA context at hardware
 * whenever a STA is (Re) Associated.
 *
 * Return: QDF_STATUS_SUCCESS on success else QDF_STATUS failure codes
 */

QDF_STATUS
lim_add_sta(struct mac_context *mac_ctx,
	tpDphHashNode sta_ds, uint8_t update_entry, struct pe_session *session_entry)
{
	tpAddStaParams add_sta_params = NULL;
	struct scheduler_msg msg_q = {0};
	QDF_STATUS ret_code = QDF_STATUS_SUCCESS;
	tSirMacAddr sta_mac, *sta_Addr;
	tpSirAssocReq assoc_req;
	uint8_t i, nw_type_11b = 0;
	tLimIbssPeerNode *peer_node; /* for IBSS mode */
	const uint8_t *p2p_ie = NULL;
	tDot11fIEVHTCaps vht_caps;
	struct mlme_vht_capabilities_info *vht_cap_info;

	vht_cap_info = &mac_ctx->mlme_cfg->vht_caps.vht_cap_info;

	sir_copy_mac_addr(sta_mac, session_entry->self_mac_addr);

	pe_debug("sessionid: %d update_entry = %d limsystemrole = %d",
		session_entry->smeSessionId, update_entry,
		GET_LIM_SYSTEM_ROLE(session_entry));

	add_sta_params = qdf_mem_malloc(sizeof(tAddStaParams));
	if (!add_sta_params)
		return QDF_STATUS_E_NOMEM;

	if (LIM_IS_AP_ROLE(session_entry) || LIM_IS_IBSS_ROLE(session_entry) ||
		LIM_IS_NDI_ROLE(session_entry))
		sta_Addr = &sta_ds->staAddr;
#ifdef FEATURE_WLAN_TDLS
	/* SystemRole shouldn't be matter if staType is TDLS peer */
	else if (STA_ENTRY_TDLS_PEER == sta_ds->staType)
		sta_Addr = &sta_ds->staAddr;
#endif
	else
		sta_Addr = &sta_mac;

	pe_debug(QDF_MAC_ADDR_STR ": Subtype(Assoc/Reassoc): %d",
		QDF_MAC_ADDR_ARRAY(*sta_Addr), sta_ds->mlmStaContext.subType);

	qdf_mem_copy((uint8_t *) add_sta_params->staMac,
		     (uint8_t *) *sta_Addr, sizeof(tSirMacAddr));
	qdf_mem_copy((uint8_t *) add_sta_params->bssId,
		     session_entry->bssId, sizeof(tSirMacAddr));
	qdf_mem_copy(&add_sta_params->capab_info,
		     &sta_ds->mlmStaContext.capabilityInfo,
		     sizeof(add_sta_params->capab_info));

	/* Copy legacy rates */
	qdf_mem_copy(&add_sta_params->supportedRates,
		     &sta_ds->supportedRates,
		     sizeof(sta_ds->supportedRates));

	add_sta_params->assocId = sta_ds->assocId;

	add_sta_params->wmmEnabled = sta_ds->qosMode;
	add_sta_params->listenInterval = sta_ds->mlmStaContext.listenInterval;
	add_sta_params->shortPreambleSupported = sta_ds->shortPreambleEnabled;
	if (LIM_IS_AP_ROLE(session_entry) &&
	   (sta_ds->mlmStaContext.subType == LIM_REASSOC)) {
		/*
		 * TBD - need to remove this REASSOC check
		 * after fixinf rmmod issue
		 */
		add_sta_params->updateSta = sta_ds->mlmStaContext.updateContext;
	}
	sta_ds->valid = 0;
	sta_ds->mlmStaContext.mlmState = eLIM_MLM_WT_ADD_STA_RSP_STATE;

	pe_debug("Assoc ID: %d wmmEnabled: %d listenInterval: %d"
		 " shortPreambleSupported: %d", add_sta_params->assocId,
		add_sta_params->wmmEnabled, add_sta_params->listenInterval,
		add_sta_params->shortPreambleSupported);
	/* This will indicate HAL to "allocate" a new STA index */
#ifdef FEATURE_WLAN_TDLS
	/*
	 * As there is corner case in-between add_sta and change_sta,if del_sta
	 * for other staIdx happened, firmware return wrong staIdx
	 * (recently removed staIdx). Until we get a confirmation from the
	 * firmware team it is now return correct staIdx for same sta_mac_addr
	 * for update case, we want to get around it by passing valid staIdx
	 * given by add_sta time.
	 */
	if ((STA_ENTRY_TDLS_PEER == sta_ds->staType) && (true == update_entry))
		add_sta_params->staIdx = sta_ds->staIndex;
	else
#endif
	add_sta_params->staIdx = STA_INVALID_IDX;
	add_sta_params->staType = sta_ds->staType;

	add_sta_params->updateSta = update_entry;

	add_sta_params->status = QDF_STATUS_SUCCESS;
	add_sta_params->respReqd = 1;

	/* Update VHT/HT Capability */
	if (LIM_IS_AP_ROLE(session_entry) ||
	    LIM_IS_IBSS_ROLE(session_entry)) {
		add_sta_params->htCapable = sta_ds->mlmStaContext.htCapability;
		add_sta_params->vhtCapable =
			 sta_ds->mlmStaContext.vhtCapability;
	}
#ifdef FEATURE_WLAN_TDLS
	/* SystemRole shouldn't be matter if staType is TDLS peer */
	else if (STA_ENTRY_TDLS_PEER == sta_ds->staType) {
		add_sta_params->htCapable = sta_ds->mlmStaContext.htCapability;
		add_sta_params->vhtCapable =
			 sta_ds->mlmStaContext.vhtCapability;
	}
#endif
	else {
		add_sta_params->htCapable = session_entry->htCapability;
		add_sta_params->vhtCapable = session_entry->vhtCapability;
	}

	pe_debug("StaIdx: %d updateSta: %d htcapable: %d vhtCapable: %d",
		add_sta_params->staIdx, add_sta_params->updateSta,
		add_sta_params->htCapable, add_sta_params->vhtCapable);

	/*
	 * If HT client is connected to SAP DUT and self cap is NSS = 2 then
	 * disable ASYNC DBS scan by sending WMI_VDEV_PARAM_SMPS_INTOLERANT
	 * to FW, because HT client's can't drop down chain using SMPS frames.
	 */
	if (!policy_mgr_is_hw_dbs_2x2_capable(mac_ctx->psoc) &&
		LIM_IS_AP_ROLE(session_entry) &&
		(STA_ENTRY_PEER == sta_ds->staType) &&
		!add_sta_params->vhtCapable &&
		(session_entry->nss == 2)) {
		session_entry->ht_client_cnt++;
		if (session_entry->ht_client_cnt == 1) {
			pe_debug("setting SMPS intolrent vdev_param");
			wma_cli_set_command(session_entry->smeSessionId,
				(int)WMI_VDEV_PARAM_SMPS_INTOLERANT,
				1, VDEV_CMD);
		}
	}

	lim_update_sta_he_capable(mac_ctx, add_sta_params, sta_ds,
				  session_entry);

	add_sta_params->greenFieldCapable = sta_ds->htGreenfield;
	add_sta_params->maxAmpduDensity = sta_ds->htAMpduDensity;
	add_sta_params->maxAmpduSize = sta_ds->htMaxRxAMpduFactor;
	add_sta_params->fDsssCckMode40Mhz = sta_ds->htDsssCckRate40MHzSupport;
	add_sta_params->fShortGI20Mhz = sta_ds->htShortGI20Mhz;
	add_sta_params->fShortGI40Mhz = sta_ds->htShortGI40Mhz;
	add_sta_params->lsigTxopProtection = sta_ds->htLsigTXOPProtection;
	add_sta_params->maxAmsduSize = sta_ds->htMaxAmsduLength;
	add_sta_params->ch_width = sta_ds->htSupportedChannelWidthSet;
	add_sta_params->mimoPS = sta_ds->htMIMOPSState;

	pe_debug("greenFieldCapable: %d maxAmpduDensity: %d maxAmpduDensity: %d",
		add_sta_params->greenFieldCapable,
		add_sta_params->maxAmpduDensity, add_sta_params->maxAmpduSize);

	pe_debug("fDsssCckMode40Mhz: %d fShortGI20Mhz: %d fShortGI40Mhz: %d",
		add_sta_params->fDsssCckMode40Mhz,
		 add_sta_params->fShortGI20Mhz,	add_sta_params->fShortGI40Mhz);

	pe_debug("lsigTxopProtection: %d maxAmsduSize: %d txChannelWidth: %d mimoPS: %d",
		add_sta_params->lsigTxopProtection,
		add_sta_params->maxAmsduSize, add_sta_params->ch_width,
		add_sta_params->mimoPS);

	if (add_sta_params->vhtCapable) {
		if (sta_ds->vhtSupportedChannelWidthSet)
			add_sta_params->ch_width =
				sta_ds->vhtSupportedChannelWidthSet + 1;

		add_sta_params->vhtSupportedRxNss = sta_ds->vhtSupportedRxNss;
		if (LIM_IS_AP_ROLE(session_entry) ||
				LIM_IS_P2P_DEVICE_GO(session_entry))
			add_sta_params->vhtSupportedRxNss = QDF_MIN(
					add_sta_params->vhtSupportedRxNss,
					session_entry->nss);
		add_sta_params->vhtTxBFCapable =
#ifdef FEATURE_WLAN_TDLS
			((STA_ENTRY_PEER == sta_ds->staType)
			 || (STA_ENTRY_TDLS_PEER == sta_ds->staType)) ?
				 sta_ds->vhtBeamFormerCapable :
				 session_entry->vht_config.su_beam_formee;
#else
			(STA_ENTRY_PEER == sta_ds->staType) ?
				 sta_ds->vhtBeamFormerCapable :
				 session_entry->vht_config.su_beam_formee;
#endif
		add_sta_params->enable_su_tx_bformer =
			sta_ds->vht_su_bfee_capable;
	}

	pe_debug("TxChWidth %d vhtTxBFCap %d, su_bfer %d",
		add_sta_params->ch_width, add_sta_params->vhtTxBFCapable,
		add_sta_params->enable_su_tx_bformer);
#ifdef FEATURE_WLAN_TDLS
	if ((STA_ENTRY_PEER == sta_ds->staType) ||
		(STA_ENTRY_TDLS_PEER == sta_ds->staType))
#else
	if (STA_ENTRY_PEER == sta_ds->staType)
#endif
	{
		/*
		 * peer STA get the LDPC capability from sta_ds,
		 * which populated from
		 * HT/VHT capability
		 */
		if (add_sta_params->vhtTxBFCapable
		    && vht_cap_info->disable_ldpc_with_txbf_ap) {
			add_sta_params->htLdpcCapable = 0;
			add_sta_params->vhtLdpcCapable = 0;
		} else {
			if (session_entry->txLdpcIniFeatureEnabled & 0x1)
				add_sta_params->htLdpcCapable =
						sta_ds->htLdpcCapable;
			else
				add_sta_params->htLdpcCapable = 0;

			if (session_entry->txLdpcIniFeatureEnabled & 0x2)
				add_sta_params->vhtLdpcCapable =
						sta_ds->vhtLdpcCapable;
			else
				add_sta_params->vhtLdpcCapable = 0;
		}
	} else if (STA_ENTRY_SELF == sta_ds->staType) {
		/* For Self STA get the LDPC capability from config.ini */
		add_sta_params->htLdpcCapable =
			(session_entry->txLdpcIniFeatureEnabled & 0x01);
		add_sta_params->vhtLdpcCapable =
			((session_entry->txLdpcIniFeatureEnabled >> 1) & 0x01);
	}

	/* Update PE session ID */
	add_sta_params->sessionId = session_entry->peSessionId;

	/* Update SME session ID */
	add_sta_params->smesessionId = session_entry->smeSessionId;

	add_sta_params->maxTxPower = session_entry->maxTxPower;

	if (session_entry->parsedAssocReq) {
		uint16_t aid = sta_ds->assocId;
		/* Get a copy of the already parsed Assoc Request */
		assoc_req =
			(tpSirAssocReq) session_entry->parsedAssocReq[aid];
		if (assoc_req && assoc_req->addIEPresent
		    && assoc_req->addIE.length) {
			p2p_ie = limGetP2pIEPtr(mac_ctx,
					assoc_req->addIE.addIEdata,
					assoc_req->addIE.length);
		}

		add_sta_params->p2pCapableSta = (p2p_ie != NULL);
		if (assoc_req && add_sta_params->htCapable) {
			qdf_mem_copy(&add_sta_params->ht_caps,
				     ((uint8_t *) &assoc_req->HTCaps) + 1,
				     sizeof(add_sta_params->ht_caps));
		}

		if (assoc_req && add_sta_params->vhtCapable) {
			if (assoc_req->vendor_vht_ie.VHTCaps.present)
				vht_caps = assoc_req->vendor_vht_ie.VHTCaps;
			else
				vht_caps = assoc_req->VHTCaps;
			add_sta_params->vht_caps =
				lim_populate_vht_caps(vht_caps);
		}

		lim_add_he_cap(add_sta_params, assoc_req);

	} else if (LIM_IS_IBSS_ROLE(session_entry)) {

		/*
		 * in IBSS mode, use peer node as the source of ht_caps
		 * and vht_caps
		 */
		peer_node = lim_ibss_peer_find(mac_ctx, *sta_Addr);
		if (!peer_node) {
			pe_err("Can't find IBSS peer node for ADD_STA");
			return QDF_STATUS_E_NOENT;
		}

		if (peer_node->atimIePresent) {
			add_sta_params->atimIePresent =
				 peer_node->atimIePresent;
			add_sta_params->peerAtimWindowLength =
				peer_node->peerAtimWindowLength;
		}

		add_sta_params->ht_caps =
			(peer_node->htSupportedChannelWidthSet <<
			 SIR_MAC_HT_CAP_CHWIDTH40_S) |
			(peer_node->htGreenfield <<
			 SIR_MAC_HT_CAP_GREENFIELD_S) |
			(peer_node->htShortGI20Mhz <<
			 SIR_MAC_HT_CAP_SHORTGI20MHZ_S) |
			(peer_node->htShortGI40Mhz <<
			 SIR_MAC_HT_CAP_SHORTGI40MHZ_S) |
			(SIR_MAC_TXSTBC <<
			 SIR_MAC_HT_CAP_TXSTBC_S) |
			(SIR_MAC_RXSTBC <<
			 SIR_MAC_HT_CAP_RXSTBC_S) |
			(peer_node->htMaxAmsduLength <<
			 SIR_MAC_HT_CAP_MAXAMSDUSIZE_S) |
			(peer_node->htDsssCckRate40MHzSupport <<
			 SIR_MAC_HT_CAP_DSSSCCK40_S);

		add_sta_params->vht_caps =
			 lim_populate_vht_caps(peer_node->VHTCaps);
	}
#ifdef FEATURE_WLAN_TDLS
	if (STA_ENTRY_TDLS_PEER == sta_ds->staType) {
		add_sta_params->ht_caps = sta_ds->ht_caps;
		add_sta_params->vht_caps = sta_ds->vht_caps;
		if (add_sta_params->vhtCapable) {
			add_sta_params->maxAmpduSize =
				SIR_MAC_GET_VHT_MAX_AMPDU_EXPO(
						sta_ds->vht_caps);
		}
		pe_debug("Sta type is TDLS_PEER, ht_caps: 0x%x, vht_caps: 0x%x",
			  add_sta_params->ht_caps,
			  add_sta_params->vht_caps);
	}
#endif

#ifdef FEATURE_WLAN_TDLS
	if (sta_ds->wmeEnabled &&
	   (LIM_IS_AP_ROLE(session_entry) ||
	   (STA_ENTRY_TDLS_PEER == sta_ds->staType)))
#else
	if (sta_ds->wmeEnabled && LIM_IS_AP_ROLE(session_entry))
#endif
	{
		add_sta_params->uAPSD = 0;
		/*
		 * update UAPSD and send it to LIM to add STA
		 * bitmap MSB <- LSB MSB 4 bits are for
		 * trigger enabled AC setting and LSB 4 bits
		 * are for delivery enabled AC setting
		 * 7   6    5    4    3    2    1    0
		 * BE  BK   VI   VO   BE   BK   VI   VO
		 */
		add_sta_params->uAPSD |=
			sta_ds->qos.capability.qosInfo.acvo_uapsd;
		add_sta_params->uAPSD |=
			(sta_ds->qos.capability.qosInfo.acvi_uapsd << 1);
		add_sta_params->uAPSD |=
			(sta_ds->qos.capability.qosInfo.acbk_uapsd << 2);
		add_sta_params->uAPSD |=
			(sta_ds->qos.capability.qosInfo.acbe_uapsd << 3);
		/*
		 * making delivery enabled and
		 * trigger enabled setting the same.
		 */
		add_sta_params->uAPSD |= add_sta_params->uAPSD << 4;

		add_sta_params->maxSPLen =
			sta_ds->qos.capability.qosInfo.maxSpLen;
		pe_debug("uAPSD = 0x%x, maxSpLen = %d",
			add_sta_params->uAPSD, add_sta_params->maxSPLen);
	}
#ifdef WLAN_FEATURE_11W
	add_sta_params->rmfEnabled = sta_ds->rmfEnabled;
	pe_debug("PMF enabled %d", add_sta_params->rmfEnabled);
#endif

	pe_debug("htLdpcCapable: %d vhtLdpcCapable: %d "
			"p2pCapableSta: %d",
		add_sta_params->htLdpcCapable, add_sta_params->vhtLdpcCapable,
		add_sta_params->p2pCapableSta);

	if (!add_sta_params->htLdpcCapable)
		add_sta_params->ht_caps &= ~(1 << SIR_MAC_HT_CAP_ADVCODING_S);
	if (!add_sta_params->vhtLdpcCapable)
		add_sta_params->vht_caps &=
			~(1 << SIR_MAC_VHT_CAP_LDPC_CODING_CAP);

	/*
	 * we need to defer the message until we get the
	 * response back from HAL.
	 */
	if (add_sta_params->respReqd)
		SET_LIM_PROCESS_DEFD_MESGS(mac_ctx, false);

	add_sta_params->nwType = session_entry->nwType;

	if (!(add_sta_params->htCapable || add_sta_params->vhtCapable)) {
		nw_type_11b = 1;
		for (i = 0; i < SIR_NUM_11A_RATES; i++) {
			if (sirIsArate(sta_ds->supportedRates.llaRates[i] &
						0x7F)) {
				nw_type_11b = 0;
				break;
			}
		}
		if (nw_type_11b)
			add_sta_params->nwType = eSIR_11B_NW_TYPE;
	}

	if (add_sta_params->htCapable && session_entry->ht_config.ht_tx_stbc) {
		struct sDot11fIEHTCaps *ht_caps = (struct sDot11fIEHTCaps *)
			&add_sta_params->ht_caps;
		if (ht_caps->rxSTBC)
			add_sta_params->stbc_capable = 1;
		else
			add_sta_params->stbc_capable = 0;
	}

	if (add_sta_params->vhtCapable && add_sta_params->stbc_capable) {
		struct sDot11fIEVHTCaps *vht_caps = (struct sDot11fIEVHTCaps *)
			&add_sta_params->vht_caps;
		if (vht_caps->rxSTBC)
			add_sta_params->stbc_capable = 1;
		else
			add_sta_params->stbc_capable = 0;
	}

	lim_update_he_stbc_capable(add_sta_params);

	msg_q.type = WMA_ADD_STA_REQ;
	msg_q.reserved = 0;
	msg_q.bodyptr = add_sta_params;
	msg_q.bodyval = 0;

	pe_debug("Sending WMA_ADD_STA_REQ for assocId %d", sta_ds->assocId);
	MTRACE(mac_trace_msg_tx(mac_ctx, session_entry->peSessionId,
			 msg_q.type));

	ret_code = wma_post_ctrl_msg(mac_ctx, &msg_q);
	if (QDF_STATUS_SUCCESS != ret_code) {
		if (add_sta_params->respReqd)
			SET_LIM_PROCESS_DEFD_MESGS(mac_ctx, true);
		pe_err("ADD_STA_REQ for aId %d failed (reason %X)",
			sta_ds->assocId, ret_code);
		qdf_mem_free(add_sta_params);
	}

	return ret_code;
}

/**
 * lim_del_sta()
 *
 ***FUNCTION:
 * This function is called to delete an STA context at hardware
 * whenever a STA is disassociated
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  mac    - Pointer to Global MAC structure
 * @param  sta  - Pointer to the STA datastructure created by
 *                   LIM and maintained by DPH
 * @param  fRespReqd - flag to indicate whether the delete is synchronous (true)
 *                   or not (false)
 * @return retCode - Indicates success or failure return code
 */

QDF_STATUS
lim_del_sta(struct mac_context *mac,
	    tpDphHashNode sta, bool fRespReqd, struct pe_session *pe_session)
{
	tpDeleteStaParams pDelStaParams = NULL;
	struct scheduler_msg msgQ = {0};
	QDF_STATUS retCode = QDF_STATUS_SUCCESS;

	pDelStaParams = qdf_mem_malloc(sizeof(tDeleteStaParams));
	if (!pDelStaParams)
		return QDF_STATUS_E_NOMEM;

	/*
	 * 2G-AS platform: SAP associates with HT (11n)clients as 2x1 in 2G and
	 * 2X2 in 5G
	 * Non-2G-AS platform: SAP associates with HT (11n) clients as 2X2 in 2G
	 * and 5G; and enable async dbs scan when all HT clients are gone
	 * 5G-AS: Don't care
	 */
	if (!policy_mgr_is_hw_dbs_2x2_capable(mac->psoc) &&
		LIM_IS_AP_ROLE(pe_session) &&
		(sta->staType == STA_ENTRY_PEER) &&
		!sta->mlmStaContext.vhtCapability &&
		(pe_session->nss == 2)) {
		pe_session->ht_client_cnt--;
		if (pe_session->ht_client_cnt == 0) {
			pe_debug("clearing SMPS intolrent vdev_param");
			wma_cli_set_command(pe_session->smeSessionId,
				(int)WMI_VDEV_PARAM_SMPS_INTOLERANT,
				0, VDEV_CMD);
		}
	}
	/* */
	/* DPH contains the STA index only for "peer" STA entries. */
	/* LIM global contains "self" STA index */
	/* Thus, */
	/*    if( STA role ) */
	/*      get STA index from LIM global */
	/*    else */
	/*      get STA index from DPH */
	/* */

#ifdef FEATURE_WLAN_TDLS
	if (LIM_IS_STA_ROLE(pe_session) &&
	    (sta->staType != STA_ENTRY_TDLS_PEER))
#else
	if (LIM_IS_STA_ROLE(pe_session))
#endif
		pDelStaParams->staIdx = pe_session->staId;

	else
		pDelStaParams->staIdx = sta->staIndex;

	pDelStaParams->assocId = sta->assocId;
	sta->valid = 0;

	if (!fRespReqd)
		pDelStaParams->respReqd = 0;
	else {
		if (!(IS_TDLS_PEER(sta->staType))) {
			/* when lim_del_sta is called from processSmeAssocCnf
			 * then mlmState is already set properly. */
			if (eLIM_MLM_WT_ASSOC_DEL_STA_RSP_STATE !=
				GET_LIM_STA_CONTEXT_MLM_STATE(sta)) {
				MTRACE(mac_trace
					(mac, TRACE_CODE_MLM_STATE,
					 pe_session->peSessionId,
					 eLIM_MLM_WT_DEL_STA_RSP_STATE));
				SET_LIM_STA_CONTEXT_MLM_STATE(sta,
					eLIM_MLM_WT_DEL_STA_RSP_STATE);
			}
			if (LIM_IS_STA_ROLE(pe_session)) {
				MTRACE(mac_trace
					(mac, TRACE_CODE_MLM_STATE,
					 pe_session->peSessionId,
					 eLIM_MLM_WT_DEL_STA_RSP_STATE));

				pe_session->limMlmState =
					eLIM_MLM_WT_DEL_STA_RSP_STATE;

			}
		}

		/* we need to defer the message until we get the
		 * response back from HAL. */
		SET_LIM_PROCESS_DEFD_MESGS(mac, false);

		pDelStaParams->respReqd = 1;
	}

	/* Update PE session ID */
	pDelStaParams->sessionId = pe_session->peSessionId;
	pDelStaParams->smesessionId = pe_session->smeSessionId;

	pDelStaParams->staType = sta->staType;
	qdf_mem_copy((uint8_t *) pDelStaParams->staMac,
		     (uint8_t *) sta->staAddr, sizeof(tSirMacAddr));

	pDelStaParams->status = QDF_STATUS_SUCCESS;
	msgQ.type = WMA_DELETE_STA_REQ;
	msgQ.reserved = 0;
	msgQ.bodyptr = pDelStaParams;
	msgQ.bodyval = 0;

	pe_debug("Sessionid %d :Sending SIR_HAL_DELETE_STA_REQ "
		 "for STAID: %X and AssocID: %d MAC : "
		 QDF_MAC_ADDR_STR, pDelStaParams->sessionId,
		pDelStaParams->staIdx, pDelStaParams->assocId,
		QDF_MAC_ADDR_ARRAY(sta->staAddr));

	MTRACE(mac_trace_msg_tx(mac, pe_session->peSessionId, msgQ.type));
	retCode = wma_post_ctrl_msg(mac, &msgQ);
	if (QDF_STATUS_SUCCESS != retCode) {
		if (fRespReqd)
			SET_LIM_PROCESS_DEFD_MESGS(mac, true);
		pe_err("Posting DELETE_STA_REQ to HAL failed, reason=%X",
			retCode);
		qdf_mem_free(pDelStaParams);
	}

	return retCode;
}

/**
 * lim_set_mbssid_info() - Save mbssid info
 * @pe_session: pe session entry
 *
 * Return: None
 */
static void lim_set_mbssid_info(struct pe_session *pe_session)
{
	struct scan_mbssid_info *mbssid_info;

	mbssid_info = &pe_session->lim_join_req->bssDescription.mbssid_info;
	mlme_set_mbssid_info(pe_session->vdev, mbssid_info);
}

/**
 * lim_add_sta_self()
 *
 ***FUNCTION:
 * This function is called to add an STA context at hardware
 * whenever a STA is (Re) Associated.
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  mac    - Pointer to Global MAC structure
 * @param  sta  - Pointer to the STA datastructure created by
 *                   LIM and maintained by DPH
 * @return retCode - Indicates success or failure return code
 */

QDF_STATUS
lim_add_sta_self(struct mac_context *mac, uint16_t staIdx, uint8_t updateSta,
		 struct pe_session *pe_session)
{
	tpAddStaParams pAddStaParams = NULL;
	struct scheduler_msg msgQ = {0};
	QDF_STATUS retCode = QDF_STATUS_SUCCESS;
	tSirMacAddr staMac;
	uint32_t listenInterval = MLME_CFG_LISTEN_INTERVAL;
	/*This self Sta dot 11 mode comes from the cfg and the expectation here is
	 * that cfg carries the systemwide capability that device under
	 * consideration can support. This capability gets plumbed into the cfg
	 * cache at system initialization time via the .dat and .ini file override
	 * mechanisms and will not change. If it does change, it is the
	 * responsibility of SME to evict the selfSta and reissue a new AddStaSelf
	 * command.*/
	uint32_t selfStaDot11Mode = 0;

	selfStaDot11Mode = mac->mlme_cfg->dot11_mode.dot11_mode;
	pe_debug("cfgDot11Mode: %d", (int)selfStaDot11Mode);
	pe_debug("Roam Channel Bonding Mode %d",
		(int)mac->roam.configParam.uCfgDot11Mode);

	sir_copy_mac_addr(staMac, pe_session->self_mac_addr);
	pe_debug(QDF_MAC_ADDR_STR ": ", QDF_MAC_ADDR_ARRAY(staMac));
	pAddStaParams = qdf_mem_malloc(sizeof(tAddStaParams));
	if (!pAddStaParams)
		return QDF_STATUS_E_NOMEM;

	/* / Add STA context at MAC HW (BMU, RHP & TFP) */
	qdf_mem_copy((uint8_t *) pAddStaParams->staMac,
		     (uint8_t *) staMac, sizeof(tSirMacAddr));

	qdf_mem_copy((uint8_t *) pAddStaParams->bssId,
		     pe_session->bssId, sizeof(tSirMacAddr));

	pAddStaParams->assocId = pe_session->limAID;
	pAddStaParams->staType = STA_ENTRY_SELF;
	pAddStaParams->status = QDF_STATUS_SUCCESS;
	pAddStaParams->respReqd = 1;

	/* Update  PE session ID */
	pAddStaParams->sessionId = pe_session->peSessionId;

	/* Update SME session ID */
	pAddStaParams->smesessionId = pe_session->smeSessionId;

	pAddStaParams->maxTxPower = pe_session->maxTxPower;

	/* This will indicate HAL to "allocate" a new STA index */
	pAddStaParams->staIdx = staIdx;
	pAddStaParams->updateSta = updateSta;
	qdf_mem_copy(&pAddStaParams->mbssid_info,
		     &pe_session->lim_join_req->bssDescription.mbssid_info,
		     sizeof(struct scan_mbssid_info));

	lim_set_mbssid_info(pe_session);

	pAddStaParams->shortPreambleSupported =
					mac->mlme_cfg->ht_caps.short_preamble;

	lim_populate_own_rate_set(mac, &pAddStaParams->supportedRates,
				  NULL, false,
				  pe_session, NULL, NULL);
	if (IS_DOT11_MODE_HT(selfStaDot11Mode)) {
		pAddStaParams->htCapable = true;
		pAddStaParams->greenFieldCapable =
			lim_get_ht_capability(mac, eHT_GREENFIELD,
					      pe_session);
		pAddStaParams->ch_width =
			mac->roam.configParam.channelBondingMode5GHz;
		pAddStaParams->mimoPS =
			lim_get_ht_capability(mac, eHT_MIMO_POWER_SAVE,
					      pe_session);
		pAddStaParams->rifsMode =
			lim_get_ht_capability(mac, eHT_RIFS_MODE,
					      pe_session);
		pAddStaParams->lsigTxopProtection =
			lim_get_ht_capability(mac, eHT_LSIG_TXOP_PROTECTION,
					      pe_session);
		pAddStaParams->maxAmpduDensity =
			lim_get_ht_capability(mac, eHT_MPDU_DENSITY,
					      pe_session);
		pAddStaParams->maxAmpduSize =
			lim_get_ht_capability(mac, eHT_MAX_RX_AMPDU_FACTOR,
					      pe_session);
		pAddStaParams->maxAmsduSize =
			lim_get_ht_capability(mac, eHT_MAX_AMSDU_LENGTH,
					      pe_session);
		pAddStaParams->max_amsdu_num =
			lim_get_ht_capability(mac, eHT_MAX_AMSDU_NUM,
					      pe_session);
		pAddStaParams->fDsssCckMode40Mhz =
			lim_get_ht_capability(mac, eHT_DSSS_CCK_MODE_40MHZ,
					      pe_session);
		pAddStaParams->fShortGI20Mhz = pe_session->ht_config.ht_sgi20;
		pAddStaParams->fShortGI40Mhz = pe_session->ht_config.ht_sgi40;
		pe_debug("greenFieldCapable: %d maxAmpduDensity: %d maxAmpduSize: %d",
			 pAddStaParams->greenFieldCapable,
			 pAddStaParams->maxAmpduDensity,
			 pAddStaParams->maxAmpduSize);

		pe_debug("fDsssCckMode40Mhz: %d fShortGI20Mhz: %d fShortGI40Mhz: %d lsigTxopProtection: %d",
			 pAddStaParams->fDsssCckMode40Mhz,
			 pAddStaParams->fShortGI20Mhz,
			 pAddStaParams->fShortGI40Mhz,
			 pAddStaParams->lsigTxopProtection);

		pe_debug("maxAmsduSize: %d txChannelWidth: %d mimoPS: %d rifsMode %d",
			 pAddStaParams->maxAmsduSize,
			 pAddStaParams->ch_width,
			 pAddStaParams->mimoPS, pAddStaParams->rifsMode);
	}
	pAddStaParams->vhtCapable = IS_DOT11_MODE_VHT(selfStaDot11Mode);
	if (pAddStaParams->vhtCapable) {
		pAddStaParams->ch_width =
			pe_session->ch_width;
		pe_debug("VHT WIDTH SET %d", pAddStaParams->ch_width);
	}
	pAddStaParams->vhtTxBFCapable =
		pe_session->vht_config.su_beam_formee;
	pAddStaParams->enable_su_tx_bformer =
		pe_session->vht_config.su_beam_former;
	pe_debug("vhtCapable: %d vhtTxBFCapable %d, su_bfer %d",
		pAddStaParams->vhtCapable, pAddStaParams->vhtTxBFCapable,
		pAddStaParams->enable_su_tx_bformer);

	/* In 11ac mode, the hardware is capable of supporting 128K AMPDU size */
	if (IS_DOT11_MODE_VHT(selfStaDot11Mode))
		pAddStaParams->maxAmpduSize =
		mac->mlme_cfg->vht_caps.vht_cap_info.ampdu_len_exponent;

	pAddStaParams->vhtTxMUBformeeCapable =
				pe_session->vht_config.mu_beam_formee;
	pAddStaParams->enableVhtpAid = pe_session->enableVhtpAid;
	pAddStaParams->enableAmpduPs = pe_session->enableAmpduPs;
	pAddStaParams->enableHtSmps = (pe_session->enableHtSmps &&
				(!pe_session->supported_nss_1x1));
	pAddStaParams->htSmpsconfig = pe_session->htSmpsvalue;
	pAddStaParams->send_smps_action =
		pe_session->send_smps_action;

	/* For Self STA get the LDPC capability from session i.e config.ini */
	pAddStaParams->htLdpcCapable =
		(pe_session->txLdpcIniFeatureEnabled & 0x01);
	pAddStaParams->vhtLdpcCapable =
		((pe_session->txLdpcIniFeatureEnabled >> 1) & 0x01);

	listenInterval = mac->mlme_cfg->sap_cfg.listen_interval;
	pAddStaParams->listenInterval = (uint16_t) listenInterval;

	if (QDF_P2P_CLIENT_MODE == pe_session->opmode)
		pAddStaParams->p2pCapableSta = 1;

	pe_debug(" StaIdx: %d updateSta = %d htcapable = %d ",
		pAddStaParams->staIdx, pAddStaParams->updateSta,
		pAddStaParams->htCapable);

	pe_debug("htLdpcCapable: %d vhtLdpcCapable: %d "
			       "p2pCapableSta: %d",
		pAddStaParams->htLdpcCapable, pAddStaParams->vhtLdpcCapable,
		pAddStaParams->p2pCapableSta);

	if (pe_session->isNonRoamReassoc) {
		pAddStaParams->nonRoamReassoc = 1;
		pe_session->isNonRoamReassoc = 0;
	}
	pe_debug("sessionid: %d  Assoc ID: %d listenInterval = %d "
			       "shortPreambleSupported: %d",
		pe_session->smeSessionId, pAddStaParams->assocId,
		pAddStaParams->listenInterval,
		pAddStaParams->shortPreambleSupported);

	if (IS_DOT11_MODE_HE(selfStaDot11Mode))
		lim_add_self_he_cap(pAddStaParams, pe_session);

	if (lim_is_fils_connection(pe_session))
		pAddStaParams->no_ptk_4_way = true;

	msgQ.type = WMA_ADD_STA_REQ;
	msgQ.reserved = 0;
	msgQ.bodyptr = pAddStaParams;
	msgQ.bodyval = 0;

	pe_debug(QDF_MAC_ADDR_STR ":Sessionid %d : "
			       "Sending WMA_ADD_STA_REQ. (aid %d)",
		QDF_MAC_ADDR_ARRAY(pAddStaParams->staMac),
		pAddStaParams->sessionId, pAddStaParams->assocId);
	MTRACE(mac_trace_msg_tx(mac, pe_session->peSessionId, msgQ.type));

	retCode = wma_post_ctrl_msg(mac, &msgQ);
	if (QDF_STATUS_SUCCESS != retCode) {
		pe_err("Posting WMA_ADD_STA_REQ to HAL failed, reason=%X",
			retCode);
		qdf_mem_free(pAddStaParams);
	}
	return retCode;
}

/**
 * lim_handle_cnf_wait_timeout()
 *
 ***FUNCTION:
 * This function is called by limProcessMessageQueue to handle
 * various confirmation failure cases.
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 *
 ***NOTE:
 *
 * @param  mac - Pointer to Global MAC structure
 * @param  sta - Pointer to a sta descriptor
 * @return None
 */

void lim_handle_cnf_wait_timeout(struct mac_context *mac, uint16_t staId)
{
	tpDphHashNode sta;
	struct pe_session *pe_session = NULL;

	pe_session = pe_find_session_by_session_id(mac,
			mac->lim.limTimers.gpLimCnfWaitTimer[staId].sessionId);
	if (!pe_session) {
		pe_err("Session Does not exist for given sessionID");
		return;
	}
	sta = dph_get_hash_entry(mac, staId, &pe_session->dph.dphHashTable);

	if (!sta) {
		pe_err("No STA context in SIR_LIM_CNF_WAIT_TIMEOUT");
		return;
	}

	switch (sta->mlmStaContext.mlmState) {
	case eLIM_MLM_WT_ASSOC_CNF_STATE:
		pe_debug("Did not receive Assoc Cnf in eLIM_MLM_WT_ASSOC_CNF_STATE sta Assoc id %d",
				sta->assocId);
		lim_print_mac_addr(mac, sta->staAddr, LOGD);

		if (LIM_IS_AP_ROLE(pe_session)) {
			lim_reject_association(mac, sta->staAddr,
					       sta->mlmStaContext.subType,
					       true,
					       sta->mlmStaContext.authType,
					       sta->assocId, true,
					       eSIR_MAC_UNSPEC_FAILURE_STATUS,
					       pe_session);
		}
		break;

	default:
		pe_warn("Received CNF_WAIT_TIMEOUT in state %d",
			sta->mlmStaContext.mlmState);
	}
}

/**
 * lim_delete_dph_hash_entry()- function to delete dph hash entry
 * @mac_ctx: pointer to global mac structure
 * @sta_addr: peer station address
 * @sta_id: id assigned to peer station
 * @session_entry: pe session entry
 *
 * This function is called whenever we need to delete
 * the dph hash entry
 *
 * Return: none
 */

void
lim_delete_dph_hash_entry(struct mac_context *mac_ctx, tSirMacAddr sta_addr,
				 uint16_t sta_id, struct pe_session *session_entry)
{
	uint16_t aid;
	tpDphHashNode sta_ds;
	tUpdateBeaconParams beacon_params;

	qdf_mem_zero(&beacon_params, sizeof(tUpdateBeaconParams));
	beacon_params.paramChangeBitmap = 0;
	lim_deactivate_and_change_per_sta_id_timer(mac_ctx, eLIM_CNF_WAIT_TIMER,
		 sta_id);
	if (!session_entry) {
		pe_err("NULL session_entry");
		return;
	}

	beacon_params.bss_idx = session_entry->bss_idx;
	sta_ds = dph_lookup_hash_entry(mac_ctx, sta_addr, &aid,
			 &session_entry->dph.dphHashTable);

	if (!sta_ds) {
		pe_err("sta_ds is NULL");
		return;
	}

	pe_debug("Deleting DPH Hash entry for STAID: %X", sta_id);
	/*
	 * update the station count and perform associated actions
	 * do this before deleting the dph hash entry
	 */
	lim_util_count_sta_del(mac_ctx, sta_ds, session_entry);

	if (LIM_IS_AP_ROLE(session_entry) || LIM_IS_IBSS_ROLE(session_entry)) {
		if (LIM_IS_AP_ROLE(session_entry)) {
			if (session_entry->gLimProtectionControl !=
				MLME_FORCE_POLICY_PROTECTION_DISABLE)
				lim_decide_ap_protection_on_delete(mac_ctx,
					sta_ds, &beacon_params, session_entry);
		}

		if (sta_ds->non_ecsa_capable) {
			if (session_entry->lim_non_ecsa_cap_num == 0) {
				pe_debug("NonECSA sta 0, id %d is ecsa",
					 sta_id);
			} else {
				session_entry->lim_non_ecsa_cap_num--;
				pe_debug("reducing the non ECSA num to %d",
					 session_entry->lim_non_ecsa_cap_num);
			}
		}

		if (LIM_IS_IBSS_ROLE(session_entry))
			lim_ibss_decide_protection_on_delete(mac_ctx, sta_ds,
				     &beacon_params, session_entry);

		lim_decide_short_preamble(mac_ctx, sta_ds, &beacon_params,
					  session_entry);
		lim_decide_short_slot(mac_ctx, sta_ds, &beacon_params,
				      session_entry);

		/* Send message to HAL about beacon parameter change. */
		pe_debug("param bitmap: %d", beacon_params.paramChangeBitmap);
		if (beacon_params.paramChangeBitmap &&
			(false ==
			 mac_ctx->sap.SapDfsInfo.is_dfs_cac_timer_running)) {
			sch_set_fixed_beacon_fields(mac_ctx, session_entry);
			lim_send_beacon_params(mac_ctx, &beacon_params,
					       session_entry);
		}

		lim_obss_send_detection_cfg(mac_ctx, session_entry, false);

#ifdef WLAN_FEATURE_11W
		if (sta_ds->rmfEnabled) {
			pe_debug("delete pmf timer sta-idx:%d assoc-id:%d",
				 sta_ds->staIndex, sta_ds->assocId);
			tx_timer_delete(&sta_ds->pmfSaQueryTimer);
		}
#endif
	}

	if (dph_delete_hash_entry(mac_ctx, sta_addr, sta_id,
		 &session_entry->dph.dphHashTable) != QDF_STATUS_SUCCESS)
		pe_err("error deleting hash entry");
}

/**
 * lim_check_and_announce_join_success()- function to check if the received
 * Beacon/Probe Response is from the BSS that we're attempting to join.
 * @mac: pointer to global mac structure
 * @beacon_probe_rsp: pointer to reveived beacon/probe response frame
 * @header: pointer to received management frame header
 * @session_entry: pe session entry
 *
 * This function is called upon receiving Beacon/Probe Response
 * frame in WT_JOIN_BEACON_STATE to check if the received
 * Beacon/Probe Response is from the BSS that we're attempting
 * to join.
 * If the Beacon/Probe Response is indeed from the BSS we're
 * attempting to join, join success is sent to SME.
 *
 * Return: none
 */

void
lim_check_and_announce_join_success(struct mac_context *mac_ctx,
		tSirProbeRespBeacon *beacon_probe_rsp, tpSirMacMgmtHdr header,
		struct pe_session *session_entry)
{
	tSirMacSSid current_ssid;
	tLimMlmJoinCnf mlm_join_cnf;
	uint32_t val;
	uint32_t *noa_duration_from_beacon = NULL;
	uint32_t *noa2_duration_from_beacon = NULL;
	uint32_t noa;
	uint32_t total_num_noa_desc = 0;

	qdf_mem_copy(current_ssid.ssId,
		     session_entry->ssId.ssId, session_entry->ssId.length);

	current_ssid.length = (uint8_t) session_entry->ssId.length;

	/*
	 * Check for SSID only in probe response. Beacons may not carry
	 * SSID information in hidden SSID case
	 */
	if (((SIR_MAC_MGMT_FRAME == header->fc.type) &&
		(SIR_MAC_MGMT_PROBE_RSP == header->fc.subType)) &&
		current_ssid.length &&
		(qdf_mem_cmp((uint8_t *) &beacon_probe_rsp->ssId,
				  (uint8_t *) &current_ssid,
				  (uint8_t) (1 + current_ssid.length)))) {
		/*
		 * Received SSID does not match with the one we've.
		 * Ignore received Beacon frame
		 */
		pe_debug("SSID received in Beacon does not match");
#ifdef WLAN_DEBUG
		mac_ctx->lim.gLimBcnSSIDMismatchCnt++;
#endif
		return;
	}

	if (!LIM_IS_STA_ROLE(session_entry))
		return;

	pe_debug("Received Beacon/PR with matching BSSID:%pM PESessionID %d",
			session_entry->bssId, session_entry->peSessionId);

	/* Deactivate Join Failure timer */
	lim_deactivate_and_change_timer(mac_ctx, eLIM_JOIN_FAIL_TIMER);
	/* Deactivate Periodic Join timer */
	lim_deactivate_and_change_timer(mac_ctx,
		eLIM_PERIODIC_JOIN_PROBE_REQ_TIMER);

	if (QDF_P2P_CLIENT_MODE == session_entry->opmode &&
	    beacon_probe_rsp->P2PProbeRes.NoticeOfAbsence.present) {

		noa_duration_from_beacon = (uint32_t *)
		(beacon_probe_rsp->P2PProbeRes.NoticeOfAbsence.NoADesc + 1);

		if (beacon_probe_rsp->P2PProbeRes.NoticeOfAbsence.num_NoADesc)
			total_num_noa_desc =
				beacon_probe_rsp->P2PProbeRes.NoticeOfAbsence.
				num_NoADesc / SIZE_OF_NOA_DESCRIPTOR;

		noa = *noa_duration_from_beacon;

		if (total_num_noa_desc > 1) {
			noa2_duration_from_beacon = (uint32_t *)
			(beacon_probe_rsp->P2PProbeRes.NoticeOfAbsence.NoADesc +
				SIZE_OF_NOA_DESCRIPTOR + 1);
			noa += *noa2_duration_from_beacon;
		}

		/*
		 * If MAX Noa exceeds 3 secs we will consider only 3 secs to
		 * avoid arbitrary values in noa duration field
		 */
		noa = noa > MAX_NOA_PERIOD_IN_MICROSECS ?
				MAX_NOA_PERIOD_IN_MICROSECS : noa;
		noa = noa / 1000; /* Convert to ms */

		session_entry->defaultAuthFailureTimeout =
			mac_ctx->mlme_cfg->timeouts.auth_failure_timeout;
		val = mac_ctx->mlme_cfg->timeouts.auth_failure_timeout + noa;
		if (cfg_in_range(CFG_AUTH_FAILURE_TIMEOUT, val))
			mac_ctx->mlme_cfg->timeouts.auth_failure_timeout = val;
		else
			mac_ctx->mlme_cfg->timeouts.auth_failure_timeout =
				cfg_default(CFG_AUTH_FAILURE_TIMEOUT);
	} else {
		session_entry->defaultAuthFailureTimeout = 0;
	}


	/*
	 * Check if MBO Association disallowed subattr is present and post
	 * failure status to LIM if present
	 */
	if (!session_entry->ignore_assoc_disallowed &&
			beacon_probe_rsp->assoc_disallowed) {
		pe_err("Connection fails due to assoc disallowed reason(%d):%pM PESessionID %d",
				beacon_probe_rsp->assoc_disallowed_reason,
				session_entry->bssId,
				session_entry->peSessionId);
		mlm_join_cnf.resultCode = eSIR_SME_ASSOC_REFUSED;
		mlm_join_cnf.protStatusCode = eSIR_MAC_UNSPEC_FAILURE_STATUS;
		session_entry->limMlmState = eLIM_MLM_IDLE_STATE;
		mlm_join_cnf.sessionId = session_entry->peSessionId;
		if (session_entry->pLimMlmJoinReq) {
			qdf_mem_free(session_entry->pLimMlmJoinReq);
			session_entry->pLimMlmJoinReq = NULL;
		}
		lim_post_sme_message(mac_ctx, LIM_MLM_JOIN_CNF,
				(uint32_t *) &mlm_join_cnf);
		return;
	}

	/* Update Beacon Interval at CFG database */

	if (beacon_probe_rsp->HTCaps.present)
		lim_update_sta_run_time_ht_capability(mac_ctx,
			 &beacon_probe_rsp->HTCaps);
	if (beacon_probe_rsp->HTInfo.present)
		lim_update_sta_run_time_ht_info(mac_ctx,
			 &beacon_probe_rsp->HTInfo, session_entry);
	session_entry->limMlmState = eLIM_MLM_JOINED_STATE;
	MTRACE(mac_trace(mac_ctx, TRACE_CODE_MLM_STATE,
			 session_entry->peSessionId, eLIM_MLM_JOINED_STATE));

	/*
	 * update the capability info based on recently received beacon/probe
	 * response frame
	 */
	session_entry->limCurrentBssCaps =
		lim_get_u16((uint8_t *)&beacon_probe_rsp->capabilityInfo);

	/*
	 * Announce join success by sending
	 * Join confirm to SME.
	 */
	mlm_join_cnf.resultCode = eSIR_SME_SUCCESS;
	mlm_join_cnf.protStatusCode = eSIR_MAC_SUCCESS_STATUS;
	/* Update PE sessionId */
	mlm_join_cnf.sessionId = session_entry->peSessionId;
	lim_post_sme_message(mac_ctx, LIM_MLM_JOIN_CNF,
			     (uint32_t *) &mlm_join_cnf);

	if ((IS_DOT11_MODE_VHT(session_entry->dot11mode)) &&
		beacon_probe_rsp->vendor_vht_ie.VHTCaps.present) {
		session_entry->is_vendor_specific_vhtcaps = true;
		session_entry->vendor_specific_vht_ie_sub_type =
			beacon_probe_rsp->vendor_vht_ie.sub_type;
		pe_debug("VHT caps are present in vendor specific IE");
	}

	/* Update HS 2.0 Information Element */
	if (beacon_probe_rsp->hs20vendor_ie.present) {
		pe_debug("HS20 Indication Element Present, rel#:%u, id:%u",
			beacon_probe_rsp->hs20vendor_ie.release_num,
			beacon_probe_rsp->hs20vendor_ie.hs_id_present);
		qdf_mem_copy(&session_entry->hs20vendor_ie,
			&beacon_probe_rsp->hs20vendor_ie,
			sizeof(tDot11fIEhs20vendor_ie) -
			sizeof(beacon_probe_rsp->hs20vendor_ie.hs_id));
		if (beacon_probe_rsp->hs20vendor_ie.hs_id_present)
			qdf_mem_copy(&session_entry->hs20vendor_ie.hs_id,
				&beacon_probe_rsp->hs20vendor_ie.hs_id,
				sizeof(beacon_probe_rsp->hs20vendor_ie.hs_id));
	}
}

/**
 * lim_extract_ap_capabilities()
 *
 ***FUNCTION:
 * This function is called to extract all of the AP's capabilities
 * from the IEs received from it in Beacon/Probe Response frames
 *
 ***LOGIC:
 * This routine mimics the lim_extract_ap_capability() API. The difference here
 * is that this API returns the entire tSirProbeRespBeacon info as is. It is
 * left to the caller of this API to use this info as required
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 *
 * @param   mac         Pointer to Global MAC structure
 * @param   pIE          Pointer to starting IE in Beacon/Probe Response
 * @param   ieLen        Length of all IEs combined
 * @param   beaconStruct A pointer to tSirProbeRespBeacon that needs to be
 *                       populated
 * @return  status       A status reporting QDF_STATUS_SUCCESS or QDF_STATUS_E_FAILURE
 */
QDF_STATUS lim_extract_ap_capabilities(struct mac_context *mac,
					  uint8_t *pIE,
					  uint16_t ieLen,
					  tpSirProbeRespBeacon beaconStruct)
{
	qdf_mem_zero((uint8_t *) beaconStruct, sizeof(tSirProbeRespBeacon));

	pe_debug("The IE's being received are:");
	QDF_TRACE_HEX_DUMP(QDF_MODULE_ID_PE, QDF_TRACE_LEVEL_DEBUG,
				pIE, ieLen);
	/* Parse the Beacon IE's, Don't try to parse if we dont have anything in IE */
	if (ieLen > 0) {
		if (QDF_STATUS_SUCCESS !=
		    sir_parse_beacon_ie(mac, beaconStruct, pIE,
					(uint32_t) ieLen)) {
			pe_err("APCapExtract: Beacon parsing error!");
			return QDF_STATUS_E_FAILURE;
		}
	}

	return QDF_STATUS_SUCCESS;
}

/**
 * lim_del_bss()
 *
 ***FUNCTION:
 * This function is called to delete BSS context at hardware
 * whenever a STA is disassociated
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param  mac    - Pointer to Global MAC structure
 * @param  sta  - Pointer to the STA datastructure created by
 *                   LIM and maintained by DPH
 * @return retCode - Indicates success or failure return code
 */

QDF_STATUS
lim_del_bss(struct mac_context *mac, tpDphHashNode sta, uint16_t bss_idx,
	    struct pe_session *pe_session)
{
	tpDeleteBssParams pDelBssParams = NULL;
	struct scheduler_msg msgQ = {0};
	QDF_STATUS retCode = QDF_STATUS_SUCCESS;

	pDelBssParams = qdf_mem_malloc(sizeof(tDeleteBssParams));
	if (!pDelBssParams)
		return QDF_STATUS_E_NOMEM;

	pDelBssParams->sessionId = pe_session->peSessionId; /* update PE session Id */

	/* DPH was storing the AssocID in staID field, */
	/* staID is actually assigned by HAL when AddSTA message is sent. */
	if (sta) {
		pDelBssParams->bss_idx = sta->bssId;
		sta->valid = 0;
		sta->mlmStaContext.mlmState = eLIM_MLM_WT_DEL_BSS_RSP_STATE;
	} else
		pDelBssParams->bss_idx = bss_idx;
	pe_session->limMlmState = eLIM_MLM_WT_DEL_BSS_RSP_STATE;
	MTRACE(mac_trace
		       (mac, TRACE_CODE_MLM_STATE, pe_session->peSessionId,
		       eLIM_MLM_WT_DEL_BSS_RSP_STATE));

	if ((pe_session->peSessionId ==
	     mac->lim.limTimers.gLimJoinFailureTimer.sessionId)
	    && (true ==
		tx_timer_running(&mac->lim.limTimers.gLimJoinFailureTimer))) {
		lim_deactivate_and_change_timer(mac, eLIM_JOIN_FAIL_TIMER);
	}

	pDelBssParams->status = QDF_STATUS_SUCCESS;
	pDelBssParams->respReqd = 1;
	qdf_mem_copy(pDelBssParams->bssid, pe_session->bssId,
		     sizeof(tSirMacAddr));
	pDelBssParams->smesessionId = pe_session->smeSessionId;
	pe_debug("Sessionid %d : Sending HAL_DELETE_BSS_REQ "
			  "for bss idx: %X BSSID:" QDF_MAC_ADDR_STR,
		       pDelBssParams->sessionId, pDelBssParams->bss_idx,
		       QDF_MAC_ADDR_ARRAY(pe_session->bssId));
	/* we need to defer the message until we get the response back from HAL. */
	SET_LIM_PROCESS_DEFD_MESGS(mac, false);

	pe_debug("process_ho_fail = %d", pe_session->process_ho_fail);
	if (pe_session->process_ho_fail)
		msgQ.type = WMA_DELETE_BSS_HO_FAIL_REQ;
	else
		msgQ.type = WMA_DELETE_BSS_REQ;
	msgQ.reserved = 0;
	msgQ.bodyptr = pDelBssParams;
	msgQ.bodyval = 0;

	MTRACE(mac_trace_msg_tx(mac, pe_session->peSessionId, msgQ.type));

	retCode = wma_post_ctrl_msg(mac, &msgQ);
	if (QDF_STATUS_SUCCESS != retCode) {
		SET_LIM_PROCESS_DEFD_MESGS(mac, true);
		pe_err("Posting DELETE_BSS_REQ to HAL failed, reason=%X",
			retCode);
		qdf_mem_free(pDelBssParams);
	}

	return retCode;
}

/**
 * lim_update_vhtcaps_assoc_resp : Update VHT caps in assoc response.
 * @mac_ctx Pointer to Global MAC structure
 * @pAddBssParams: parameters required for add bss params.
 * @vht_caps: VHT capabilities.
 * @pe_session : session entry.
 *
 * Return : void
 */
static void lim_update_vhtcaps_assoc_resp(struct mac_context *mac_ctx,
		tpAddBssParams pAddBssParams,
		tDot11fIEVHTCaps *vht_caps, struct pe_session *pe_session)
{
	pAddBssParams->staContext.vht_caps =
		((vht_caps->maxMPDULen <<
		  SIR_MAC_VHT_CAP_MAX_MPDU_LEN) |
		 (vht_caps->supportedChannelWidthSet <<
		  SIR_MAC_VHT_CAP_SUPP_CH_WIDTH_SET) |
		 (vht_caps->ldpcCodingCap <<
		  SIR_MAC_VHT_CAP_LDPC_CODING_CAP) |
		 (vht_caps->shortGI80MHz <<
		  SIR_MAC_VHT_CAP_SHORTGI_80MHZ) |
		 (vht_caps->shortGI160and80plus80MHz <<
		  SIR_MAC_VHT_CAP_SHORTGI_160_80_80MHZ) |
		 (vht_caps->txSTBC <<
		  SIR_MAC_VHT_CAP_TXSTBC) |
		 (vht_caps->rxSTBC <<
		  SIR_MAC_VHT_CAP_RXSTBC) |
		 (vht_caps->suBeamFormerCap <<
		  SIR_MAC_VHT_CAP_SU_BEAMFORMER_CAP) |
		 (vht_caps->suBeamformeeCap <<
		  SIR_MAC_VHT_CAP_SU_BEAMFORMEE_CAP) |
		 (vht_caps->csnofBeamformerAntSup <<
		  SIR_MAC_VHT_CAP_CSN_BEAMORMER_ANT_SUP) |
		 (vht_caps->numSoundingDim <<
		  SIR_MAC_VHT_CAP_NUM_SOUNDING_DIM) |
		 (vht_caps->muBeamformerCap <<
		  SIR_MAC_VHT_CAP_NUM_BEAM_FORMER_CAP) |
		 (vht_caps->muBeamformeeCap <<
		  SIR_MAC_VHT_CAP_NUM_BEAM_FORMEE_CAP) |
		 (vht_caps->vhtTXOPPS <<
		  SIR_MAC_VHT_CAP_TXOPPS) |
		 (vht_caps->htcVHTCap <<
		  SIR_MAC_VHT_CAP_HTC_CAP) |
		 (vht_caps->maxAMPDULenExp <<
		  SIR_MAC_VHT_CAP_MAX_AMDU_LEN_EXPO) |
		 (vht_caps->vhtLinkAdaptCap <<
		  SIR_MAC_VHT_CAP_LINK_ADAPT_CAP) |
		 (vht_caps->rxAntPattern <<
		  SIR_MAC_VHT_CAP_RX_ANTENNA_PATTERN) |
		 (vht_caps->txAntPattern <<
		  SIR_MAC_VHT_CAP_TX_ANTENNA_PATTERN) |
		 (vht_caps->reserved1 <<
		  SIR_MAC_VHT_CAP_RESERVED2));

	pAddBssParams->staContext.maxAmpduSize =
		SIR_MAC_GET_VHT_MAX_AMPDU_EXPO(
				pAddBssParams->staContext.vht_caps);

	pe_debug("Updating VHT caps in assoc Response");
}

/**
 * lim_update_vht_oper_assoc_resp : Update VHT Operations in assoc response.
 * @mac_ctx Pointer to Global MAC structure
 * @pAddBssParams: parameters required for add bss params.
 * @vht_oper: VHT Operations to update.
 * @pe_session : session entry.
 *
 * Return : void
 */
static void lim_update_vht_oper_assoc_resp(struct mac_context *mac_ctx,
		tpAddBssParams pAddBssParams,
		tDot11fIEVHTOperation *vht_oper, struct pe_session *pe_session)
{
	if (vht_oper->chanWidth &&
			pe_session->ch_width) {
		pAddBssParams->ch_width = vht_oper->chanWidth + 1;

		pAddBssParams->ch_center_freq_seg0 =
			vht_oper->chanCenterFreqSeg1;

		pAddBssParams->ch_center_freq_seg1 =
			vht_oper->chanCenterFreqSeg2;
	}
	pe_debug("Updating VHT Operation in assoc Response");
}

#ifdef WLAN_SUPPORT_TWT
/**
 * lim_set_sta_ctx_twt() - Save the TWT settings in STA context
 * @sta_ctx: Pointer to Station Context
 * @session: Pointer to PE session
 *
 * Return: None
 */
static void lim_set_sta_ctx_twt(tAddStaParams *sta_ctx, struct pe_session *session)
{
	sta_ctx->twt_requestor = session->peer_twt_requestor;
	sta_ctx->twt_responder = session->peer_twt_responder;
}
#else
static inline void lim_set_sta_ctx_twt(tAddStaParams *sta_ctx,
				       struct pe_session *session)
{
}
#endif

/**
 * limSendAddBss()
 *
 ***FUNCTION:
 *
 ***LOGIC:
 * 1) LIM receives eWNI_SME_JOIN_REQ
 * 2) For a valid eWNI_SME_JOIN_REQ, LIM sends
 * SIR_HAL_ADD_BSS_REQ to HAL
 *
 ***ASSUMPTIONS:
 * JOIN REQ parameters are saved in mac->lim.gLimMlmJoinReq
 * ADD BSS parameters can be obtained from two sources:
 * 1) mac->lim.gLimMlmJoinReq
 * 2) beaconStruct, passed as parameter
 * So, if a reqd parameter is found in bssDescriptions
 * then it is given preference over beaconStruct
 *
 ***NOTE:
 *
 * @param  mac Pointer to Global MAC structure
 *              pAssocRsp    contains the structured assoc/reassoc Response got from AP
 *              beaconstruct        Has the ProbeRsp/Beacon structured details
 *              bssDescription      bssDescription passed to PE from the SME
 * @return None
 */

QDF_STATUS lim_sta_send_add_bss(struct mac_context *mac, tpSirAssocRsp pAssocRsp,
				   tpSchBeaconStruct pBeaconStruct,
				   struct bss_description *bssDescription,
				   uint8_t updateEntry, struct pe_session *pe_session)
{
	struct scheduler_msg msgQ = {0};
	tpAddBssParams pAddBssParams = NULL;
	uint32_t retCode;
	tpDphHashNode sta = NULL;
	uint8_t chanWidthSupp = 0;
	bool is_vht_cap_in_vendor_ie = false;
	tDot11fIEVHTCaps *vht_caps = NULL;
	tDot11fIEVHTOperation *vht_oper = NULL;
	tAddStaParams *sta_context;
	uint32_t listen_interval = MLME_CFG_LISTEN_INTERVAL;
	struct mlme_vht_capabilities_info *vht_cap_info;

	vht_cap_info = &mac->mlme_cfg->vht_caps.vht_cap_info;

	/* Package SIR_HAL_ADD_BSS_REQ message parameters */
	pAddBssParams = qdf_mem_malloc(sizeof(tAddBssParams));
	if (!pAddBssParams) {
		retCode = QDF_STATUS_E_NOMEM;
		goto returnFailure;
	}

	qdf_mem_copy(pAddBssParams->bssId, bssDescription->bssId,
		     sizeof(tSirMacAddr));
	/* Fill in tAddBssParams self_mac_addr */
	qdf_mem_copy(pAddBssParams->self_mac_addr,
		     pe_session->self_mac_addr, sizeof(tSirMacAddr));

	pe_debug("sessionid: %d updateEntry: %d limsystemrole: %d",
		pe_session->smeSessionId, updateEntry,
		GET_LIM_SYSTEM_ROLE(pe_session));

	pe_debug("BSSID: " QDF_MAC_ADDR_STR,
		 QDF_MAC_ADDR_ARRAY(pAddBssParams->bssId));

	pAddBssParams->bssType = eSIR_INFRASTRUCTURE_MODE;

	pAddBssParams->operMode = BSS_OPERATIONAL_MODE_STA;

	/* Update PE session ID */
	pAddBssParams->sessionId = pe_session->peSessionId;

	pAddBssParams->beaconInterval = bssDescription->beaconInterval;

	pAddBssParams->dtimPeriod = pBeaconStruct->tim.dtimPeriod;
	pAddBssParams->updateBss = updateEntry;

	pAddBssParams->cfParamSet.cfpCount = pBeaconStruct->cfParamSet.cfpCount;
	pAddBssParams->cfParamSet.cfpPeriod =
		pBeaconStruct->cfParamSet.cfpPeriod;
	pAddBssParams->cfParamSet.cfpMaxDuration =
		pBeaconStruct->cfParamSet.cfpMaxDuration;
	pAddBssParams->cfParamSet.cfpDurRemaining =
		pBeaconStruct->cfParamSet.cfpDurRemaining;

	pAddBssParams->rateSet.numRates = pAssocRsp->supportedRates.numRates;
	qdf_mem_copy(pAddBssParams->rateSet.rate,
		     pAssocRsp->supportedRates.rate,
		     pAssocRsp->supportedRates.numRates);

	if (IS_DOT11_MODE_11B(pe_session->dot11mode) &&
	    bssDescription->nwType != eSIR_11B_NW_TYPE) {
		pAddBssParams->nwType = eSIR_11B_NW_TYPE;
	} else {
		pAddBssParams->nwType = bssDescription->nwType;
	}

	pAddBssParams->shortSlotTimeSupported =
		(uint8_t) pAssocRsp->capabilityInfo.shortSlotTime;
	pAddBssParams->llaCoexist =
		(uint8_t) pe_session->beaconParams.llaCoexist;
	pAddBssParams->llbCoexist =
		(uint8_t) pe_session->beaconParams.llbCoexist;
	pAddBssParams->llgCoexist =
		(uint8_t) pe_session->beaconParams.llgCoexist;
	pAddBssParams->ht20Coexist =
		(uint8_t) pe_session->beaconParams.ht20Coexist;

	pe_debug("BSS Type %d Beacon Interval: %d dtimPeriod: %d "
		 "cfpCount: %d", pAddBssParams->bssType,
		pAddBssParams->beaconInterval, pAddBssParams->dtimPeriod,
		pAddBssParams->cfParamSet.cfpCount);

	pe_debug("cfpPeriod: %d cfpMaxDuration: %d cfpDurRemaining: %d "
		 "numRates: %d", pAddBssParams->cfParamSet.cfpPeriod,
		pAddBssParams->cfParamSet.cfpMaxDuration,
		pAddBssParams->cfParamSet.cfpDurRemaining,
		pAddBssParams->rateSet.numRates);

	pe_debug("nwType:%d shortSlotTimeSupported: %d llaCoexist: %d "
		"llbCoexist: %d llgCoexist: %d ht20Coexist: %d",
		pAddBssParams->nwType, pAddBssParams->shortSlotTimeSupported,
		pAddBssParams->llaCoexist, pAddBssParams->llbCoexist,
		pAddBssParams->llgCoexist, pAddBssParams->ht20Coexist);

	pAddBssParams->dot11_mode = pe_session->dot11mode;
	pe_debug("dot11_mode: %d", pAddBssParams->dot11_mode);

	if (IS_DOT11_MODE_HT(pe_session->dot11mode))
		chanWidthSupp = lim_get_ht_capability(mac,
						eHT_SUPPORTED_CHANNEL_WIDTH_SET,
						pe_session);

	/* Use the advertised capabilities from the received beacon/PR */
	if (IS_DOT11_MODE_HT(pe_session->dot11mode)
	    && (pAssocRsp->HTCaps.present)) {
		pAddBssParams->htCapable = pAssocRsp->HTCaps.present;
		pe_debug("htCapable: %d", pAddBssParams->htCapable);
		if (pBeaconStruct->HTInfo.present) {
			pAddBssParams->htOperMode =
				(tSirMacHTOperatingMode) pAssocRsp->HTInfo.opMode;
			pAddBssParams->dualCTSProtection =
				(uint8_t) pAssocRsp->HTInfo.dualCTSProtection;

			if ((pAssocRsp->HTCaps.supportedChannelWidthSet)
			    && (chanWidthSupp)) {
				pAddBssParams->ch_width = (uint8_t)
					pAssocRsp->HTInfo.recommendedTxWidthSet;
				if (pAssocRsp->HTInfo.secondaryChannelOffset ==
						PHY_DOUBLE_CHANNEL_LOW_PRIMARY)
					pAddBssParams->ch_center_freq_seg0 =
						bssDescription->channelId + 2;
				else if (pAssocRsp->HTInfo.secondaryChannelOffset ==
						PHY_DOUBLE_CHANNEL_HIGH_PRIMARY)
					pAddBssParams->ch_center_freq_seg0 =
						bssDescription->channelId - 2;
			} else {
				pAddBssParams->ch_width = CH_WIDTH_20MHZ;
				pAddBssParams->ch_center_freq_seg0 = 0;
			}
			pAddBssParams->llnNonGFCoexist =
				(uint8_t) pAssocRsp->HTInfo.nonGFDevicesPresent;
			pAddBssParams->fLsigTXOPProtectionFullSupport =
				(uint8_t) pAssocRsp->HTInfo.
				lsigTXOPProtectionFullSupport;
			pAddBssParams->fRIFSMode = pAssocRsp->HTInfo.rifsMode;

			pe_debug("htOperMode: %d dualCTSProtection: %d txChannelWidth: %d center_freq_0: %d",
				pAddBssParams->htOperMode,
				pAddBssParams->dualCTSProtection,
				pAddBssParams->ch_width,
				pAddBssParams->ch_center_freq_seg0);

			pe_debug("llnNonGFCoexist: %d "
					"fLsigTXOPProtectionFullSupport: %d fRIFSMode %d",
				pAddBssParams->llnNonGFCoexist,
				pAddBssParams->fLsigTXOPProtectionFullSupport,
				pAddBssParams->fRIFSMode);
		}
	}

	pAddBssParams->currentOperChannel = bssDescription->channelId;
	pe_debug("currentOperChannel %d", pAddBssParams->currentOperChannel);
	if (pe_session->vhtCapability && (pAssocRsp->VHTCaps.present)) {
		pAddBssParams->vhtCapable = pAssocRsp->VHTCaps.present;
		vht_caps =  &pAssocRsp->VHTCaps;
		vht_oper = &pAssocRsp->VHTOperation;
	} else if (pe_session->vhtCapability &&
			pAssocRsp->vendor_vht_ie.VHTCaps.present){
		pAddBssParams->vhtCapable =
			pAssocRsp->vendor_vht_ie.VHTCaps.present;
		pe_debug("VHT Caps and Operation are present in vendor Specific IE");
		vht_caps = &pAssocRsp->vendor_vht_ie.VHTCaps;
		vht_oper = &pAssocRsp->vendor_vht_ie.VHTOperation;
	} else {
		pAddBssParams->vhtCapable = 0;
	}
	if (pAddBssParams->vhtCapable) {
		if (vht_oper)
			lim_update_vht_oper_assoc_resp(mac, pAddBssParams,
					vht_oper, pe_session);
		if (vht_caps)
			lim_update_vhtcaps_assoc_resp(mac, pAddBssParams,
					vht_caps, pe_session);
	}

	pe_debug("vhtCapable %d TxChannelWidth %d center_freq_0 %d center_freq_1 %d",
			pAddBssParams->vhtCapable, pAddBssParams->ch_width,
			pAddBssParams->ch_center_freq_seg0,
			pAddBssParams->ch_center_freq_seg1);

	if (lim_is_session_he_capable(pe_session) &&
			(pAssocRsp->he_cap.present)) {
		lim_add_bss_he_cap(pAddBssParams, pAssocRsp);
		lim_add_bss_he_cfg(pAddBssParams, pe_session);
	}
	/*
	 * Populate the STA-related parameters here
	 * Note that the STA here refers to the AP
	 * staType = PEER
	 */
	sta_context = &pAddBssParams->staContext;
	/* Identifying AP as an STA */
	pAddBssParams->staContext.staType = STA_ENTRY_OTHER;

	qdf_mem_copy(pAddBssParams->staContext.bssId,
			bssDescription->bssId, sizeof(tSirMacAddr));

	listen_interval = mac->mlme_cfg->sap_cfg.listen_interval;
	pAddBssParams->staContext.listenInterval = listen_interval;

	/* Fill Assoc id from the dph table */
	sta = dph_lookup_hash_entry(mac, pAddBssParams->staContext.bssId,
				&pAddBssParams->staContext.assocId,
				&pe_session->dph.dphHashTable);
	if (!sta) {
		pe_err("Couldn't get assoc id for " "MAC ADDR: "
			QDF_MAC_ADDR_STR,
			QDF_MAC_ADDR_ARRAY(
				pAddBssParams->staContext.staMac));
			return QDF_STATUS_E_FAILURE;
	}

	pAddBssParams->staContext.uAPSD =
		pe_session->gUapsdPerAcBitmask;

	pAddBssParams->staContext.maxSPLen = 0;
	pAddBssParams->staContext.shortPreambleSupported =
		(uint8_t) pAssocRsp->capabilityInfo.shortPreamble;
	pAddBssParams->staContext.updateSta = updateEntry;

	pe_debug("StaContext: " QDF_MAC_ADDR_STR
			" shortPreambleSupported: %d",
			QDF_MAC_ADDR_ARRAY(pAddBssParams->staContext.staMac),
			pAddBssParams->staContext.shortPreambleSupported);

	if (IS_DOT11_MODE_HT(pe_session->dot11mode)
			&& pBeaconStruct->HTCaps.present) {
		pAddBssParams->staContext.us32MaxAmpduDuration = 0;
		pAddBssParams->staContext.htCapable = 1;
		pAddBssParams->staContext.greenFieldCapable =
			(uint8_t) pAssocRsp->HTCaps.greenField;
		pAddBssParams->staContext.lsigTxopProtection =
			(uint8_t) pAssocRsp->HTCaps.lsigTXOPProtection;
		pe_debug("StaCtx: htCap %d GFcap %d lsigTxopProtn %d",
				pAddBssParams->staContext.htCapable,
				pAddBssParams->staContext.greenFieldCapable,
				pAddBssParams->staContext.lsigTxopProtection);
		if (pe_session->ht_config.ht_tx_stbc)
			pAddBssParams->staContext.stbc_capable =
				pAssocRsp->HTCaps.rxSTBC;

		if (pe_session->vhtCapability &&
				(IS_BSS_VHT_CAPABLE(pBeaconStruct->VHTCaps) ||
				 IS_BSS_VHT_CAPABLE(pBeaconStruct->
						    vendor_vht_ie.VHTCaps))) {
			pAddBssParams->staContext.vhtCapable = 1;
			pAddBssParams->staContext.vhtSupportedRxNss =
				sta->vhtSupportedRxNss;
			if (pAssocRsp->VHTCaps.present)
				vht_caps = &pAssocRsp->VHTCaps;
			else if (pAssocRsp->vendor_vht_ie.VHTCaps.present) {
				vht_caps = &pAssocRsp->vendor_vht_ie.VHTCaps;
				pe_debug("VHT Caps are in vendor Specific IE");
				is_vht_cap_in_vendor_ie = true;
			}

			if ((vht_caps) && (vht_caps->suBeamFormerCap ||
				vht_caps->muBeamformerCap) &&
				pe_session->vht_config.su_beam_formee)
				sta_context->vhtTxBFCapable = 1;

			if ((vht_caps) && vht_caps->muBeamformerCap &&
				pe_session->vht_config.mu_beam_formee)
				sta_context->vhtTxMUBformeeCapable = 1;

			if ((vht_caps) && vht_caps->suBeamformeeCap &&
				pe_session->vht_config.su_beam_former)
				sta_context->enable_su_tx_bformer = 1;

			if (vht_caps && pAddBssParams->staContext.stbc_capable)
				pAddBssParams->staContext.stbc_capable =
					vht_caps->rxSTBC;
		}
		if (lim_is_session_he_capable(pe_session) &&
		    pAssocRsp->he_cap.present) {
			lim_intersect_ap_he_caps(pe_session,
						 pAddBssParams,
						 NULL,
						 pAssocRsp);
			lim_update_he_stbc_capable(&pAddBssParams->staContext);
		}

		/*
		 * in limExtractApCapability function intersection of FW
		 * advertised channel width and AP advertised channel
		 * width has been taken into account for calculating
		 * pe_session->ch_width
		 */
		if (chanWidthSupp &&
		    ((pAssocRsp->HTCaps.supportedChannelWidthSet) ||
		    (pBeaconStruct->HTCaps.supportedChannelWidthSet))) {
			pAddBssParams->staContext.ch_width =
					pe_session->ch_width;
		} else {
			sta_context->ch_width =	CH_WIDTH_20MHZ;
			if (!vht_cap_info->enable_txbf_20mhz)
				sta_context->vhtTxBFCapable = 0;
		}

		pe_debug("StaCtx: vhtCap %d ChBW %d TxBF %d",
			 pAddBssParams->staContext.vhtCapable,
			 pAddBssParams->staContext.ch_width,
			 sta_context->vhtTxBFCapable);
		pe_debug("StaContext su_tx_bfer %d",
			 sta_context->enable_su_tx_bformer);

		pAddBssParams->staContext.mimoPS =
			(tSirMacHTMIMOPowerSaveState)
			pAssocRsp->HTCaps.mimoPowerSave;
		pAddBssParams->staContext.maxAmsduSize =
			(uint8_t) pAssocRsp->HTCaps.maximalAMSDUsize;
		pAddBssParams->staContext.maxAmpduDensity =
			pAssocRsp->HTCaps.mpduDensity;
		pAddBssParams->staContext.fDsssCckMode40Mhz =
			(uint8_t) pAssocRsp->HTCaps.dsssCckMode40MHz;
		/*
		 * We will check gShortGI20Mhz and gShortGI40Mhz from
		 * session entry  if they are set then we will use what ever
		 * Assoc response coming from AP supports. If these
		 * values are set as 0 in session entry then we will
		 * hardcode this values to 0.
		 */
		if (pe_session->ht_config.ht_sgi20) {
			pAddBssParams->staContext.fShortGI20Mhz =
				(uint8_t)pAssocRsp->HTCaps.shortGI20MHz;
		} else {
			pAddBssParams->staContext.fShortGI20Mhz = false;
		}

		if (pe_session->ht_config.ht_sgi40) {
			pAddBssParams->staContext.fShortGI40Mhz =
				(uint8_t) pAssocRsp->HTCaps.shortGI40MHz;
		} else {
			pAddBssParams->staContext.fShortGI40Mhz = false;
		}

		if (!pAddBssParams->staContext.vhtCapable)
			/* Use max ampd factor advertised in
			 * HTCAP for non-vht connection */
		{
			pAddBssParams->staContext.maxAmpduSize =
				pAssocRsp->HTCaps.maxRxAMPDUFactor;
		} else if (pAddBssParams->staContext.maxAmpduSize <
				pAssocRsp->HTCaps.maxRxAMPDUFactor) {
			pAddBssParams->staContext.maxAmpduSize =
				pAssocRsp->HTCaps.maxRxAMPDUFactor;
		}
		if (pAddBssParams->staContext.vhtTxBFCapable
		    && vht_cap_info->disable_ldpc_with_txbf_ap) {
			pAddBssParams->staContext.htLdpcCapable = 0;
			pAddBssParams->staContext.vhtLdpcCapable = 0;
		} else {
			if (pe_session->txLdpcIniFeatureEnabled & 0x1)
				pAddBssParams->staContext.htLdpcCapable =
				    (uint8_t) pAssocRsp->HTCaps.advCodingCap;
			else
				pAddBssParams->staContext.htLdpcCapable = 0;

			if (pAssocRsp->VHTCaps.present)
				vht_caps = &pAssocRsp->VHTCaps;
			else if (pAssocRsp->vendor_vht_ie.VHTCaps.present) {
				vht_caps = &pAssocRsp->vendor_vht_ie.VHTCaps;
				pe_debug("VHT Caps is in vendor Specific IE");
			}
			if (vht_caps &&
				(pe_session->txLdpcIniFeatureEnabled & 0x2)) {
				if (!is_vht_cap_in_vendor_ie)
					pAddBssParams->staContext.vhtLdpcCapable =
					  (uint8_t) pAssocRsp->VHTCaps.ldpcCodingCap;
				else
					pAddBssParams->staContext.vhtLdpcCapable =
					    (uint8_t) vht_caps->ldpcCodingCap;
			} else {
				pAddBssParams->staContext.vhtLdpcCapable = 0;
			}
		}

		if (pBeaconStruct->HTInfo.present)
			pAddBssParams->staContext.rifsMode =
				pAssocRsp->HTInfo.rifsMode;

		pe_debug("StaCtx: ChBW %d mimoPS %d maxAmsduSize %d",
				pAddBssParams->staContext.ch_width,
				pAddBssParams->staContext.mimoPS,
				pAddBssParams->staContext.maxAmsduSize);

		pe_debug("maxAmpduDens %d CckMode40Mhz %d SGI20Mhz %d",
				pAddBssParams->staContext.maxAmpduDensity,
				pAddBssParams->staContext.fDsssCckMode40Mhz,
				pAddBssParams->staContext.fShortGI20Mhz);

		pe_debug("SGI40M %d maxAmpdu %d htLdpc %d vhtLdpc %d",
				pAddBssParams->staContext.fShortGI40Mhz,
				pAddBssParams->staContext.maxAmpduSize,
				pAddBssParams->staContext.htLdpcCapable,
				pAddBssParams->staContext.vhtLdpcCapable);
	}
	pAddBssParams->staContext.smesessionId =
		pe_session->smeSessionId;
	pAddBssParams->staContext.wpa_rsn = pBeaconStruct->rsnPresent;
	pAddBssParams->staContext.wpa_rsn |=
		(pBeaconStruct->wpaPresent << 1);
	/* For OSEN Connection AP does not advertise RSN or WPA IE
	 * so from the IEs we get from supplicant we get this info
	 * so for FW to transmit EAPOL message 4 we shall set
	 * wpa_rsn
	 */
	if ((!pAddBssParams->staContext.wpa_rsn)
			&& (pe_session->isOSENConnection))
		pAddBssParams->staContext.wpa_rsn = 1;
	qdf_mem_copy(&pAddBssParams->staContext.capab_info,
			&pAssocRsp->capabilityInfo,
			sizeof(pAddBssParams->staContext.capab_info));
	qdf_mem_copy(&pAddBssParams->staContext.ht_caps,
			(uint8_t *) &pAssocRsp->HTCaps + sizeof(uint8_t),
			sizeof(pAddBssParams->staContext.ht_caps));

	/* If WMM IE or 802.11E IE is present then enable WMM */
	if ((pe_session->limWmeEnabled && pAssocRsp->wmeEdcaPresent) ||
		(pe_session->limQosEnabled && pAssocRsp->edcaPresent))
		pAddBssParams->staContext.wmmEnabled = 1;
	else
		pAddBssParams->staContext.wmmEnabled = 0;

	/* Update the rates */
	sta = dph_get_hash_entry(mac, DPH_STA_HASH_INDEX_PEER,
				&pe_session->dph.dphHashTable);
	if (sta) {
		qdf_mem_copy(&pAddBssParams->staContext.supportedRates,
			     &sta->supportedRates,
			     sizeof(sta->supportedRates));
	} else
		pe_err("could not Update the supported rates");
	pAddBssParams->staContext.encryptType = pe_session->encryptType;

	pAddBssParams->maxTxPower = pe_session->maxTxPower;
	pe_debug("maxTxPower: %d", pAddBssParams->maxTxPower);
	/* FIXME_GEN4 - Any other value that can be used for initialization? */
	pAddBssParams->status = QDF_STATUS_SUCCESS;
	pAddBssParams->respReqd = true;
	/* update persona */
	pAddBssParams->halPersona = (uint8_t)pe_session->opmode;

	if (QDF_P2P_CLIENT_MODE == pe_session->opmode)
		pAddBssParams->staContext.p2pCapableSta = 1;

	pAddBssParams->bSpectrumMgtEnabled = pe_session->spectrumMgtEnabled;

	pAddBssParams->extSetStaKeyParamValid = 0;
	pe_debug("extSetStaKeyParamValid: %d",
		pAddBssParams->extSetStaKeyParamValid);

#ifdef WLAN_FEATURE_11W
	if (pe_session->limRmfEnabled) {
		pAddBssParams->rmfEnabled = 1;
		pAddBssParams->staContext.rmfEnabled = 1;
	}
#endif

	/* Set a new state for MLME */
	if (eLIM_MLM_WT_ASSOC_RSP_STATE == pe_session->limMlmState)
		pe_session->limMlmState =
			eLIM_MLM_WT_ADD_BSS_RSP_ASSOC_STATE;
	else
		pe_session->limMlmState =
			eLIM_MLM_WT_ADD_BSS_RSP_REASSOC_STATE;
	MTRACE(mac_trace
		       (mac, TRACE_CODE_MLM_STATE, pe_session->peSessionId,
		       pe_session->limMlmState));

	if (!pAddBssParams->staContext.htLdpcCapable)
		pAddBssParams->staContext.ht_caps &=
			~(1 << SIR_MAC_HT_CAP_ADVCODING_S);
	if (!pAddBssParams->staContext.vhtLdpcCapable)
		pAddBssParams->staContext.vht_caps &=
			~(1 << SIR_MAC_VHT_CAP_LDPC_CODING_CAP);

	pe_debug("staContext wmmEnabled: %d encryptType: %d "
		"p2pCapableSta: %d",
		pAddBssParams->staContext.wmmEnabled,
		pAddBssParams->staContext.encryptType,
		pAddBssParams->staContext.p2pCapableSta);

	pe_debug("bSpectrumMgtEnabled: %d halPersona: %d setting "
		"LimMlm state to %d",
		pAddBssParams->bSpectrumMgtEnabled, pAddBssParams->halPersona,
		pe_session->limMlmState);
	if (pe_session->isNonRoamReassoc)
		pAddBssParams->nonRoamReassoc = 1;
	pAddBssParams->nss = pe_session->nss;
	pe_debug("nss value: %d", pAddBssParams->nss);

	/* we need to defer the message until we get the response back from HAL. */
	SET_LIM_PROCESS_DEFD_MESGS(mac, false);

	if (cds_is_5_mhz_enabled()) {
		pAddBssParams->ch_width = CH_WIDTH_5MHZ;
		pAddBssParams->staContext.ch_width = CH_WIDTH_5MHZ;
	} else if (cds_is_10_mhz_enabled()) {
		pAddBssParams->ch_width = CH_WIDTH_10MHZ;
		pAddBssParams->staContext.ch_width = CH_WIDTH_10MHZ;
	}
	lim_set_sta_ctx_twt(&pAddBssParams->staContext, pe_session);

	if (lim_is_fils_connection(pe_session))
		pAddBssParams->no_ptk_4_way = true;

	msgQ.type = WMA_ADD_BSS_REQ;
	/** @ToDo : Update the Global counter to keeptrack of the PE <--> HAL messages*/
	msgQ.reserved = 0;
	msgQ.bodyptr = pAddBssParams;
	msgQ.bodyval = 0;

	pe_debug("SessionId: %d Sending WMA_ADD_BSS_REQ",
		pe_session->peSessionId);
	MTRACE(mac_trace_msg_tx(mac, pe_session->peSessionId, msgQ.type));

	retCode = wma_post_ctrl_msg(mac, &msgQ);
	if (QDF_STATUS_SUCCESS != retCode) {
		SET_LIM_PROCESS_DEFD_MESGS(mac, true);
		qdf_mem_free(pAddBssParams);
		pe_err("Posting ADD_BSS_REQ to HAL failed, reason=%X",
			retCode);
		goto returnFailure;

	} else
		return retCode;

returnFailure:
	/* Clean-up will be done by the caller... */
	return retCode;
}

QDF_STATUS lim_sta_send_add_bss_pre_assoc(struct mac_context *mac, uint8_t updateEntry,
					     struct pe_session *pe_session)
{
	struct scheduler_msg msgQ = {0};
	tpAddBssParams pAddBssParams = NULL;
	uint32_t retCode;
	tSchBeaconStruct *pBeaconStruct;
	uint8_t chanWidthSupp = 0;
	tDot11fIEVHTOperation *vht_oper = NULL;
	tDot11fIEVHTCaps *vht_caps = NULL;
	uint32_t listen_interval = MLME_CFG_LISTEN_INTERVAL;
	struct bss_description *bssDescription =
		&pe_session->lim_join_req->bssDescription;
	struct mlme_vht_capabilities_info *vht_cap_info;

	vht_cap_info = &mac->mlme_cfg->vht_caps.vht_cap_info;

	pBeaconStruct = qdf_mem_malloc(sizeof(tSchBeaconStruct));
	if (!pBeaconStruct)
		return QDF_STATUS_E_NOMEM;

	/* Package SIR_HAL_ADD_BSS_REQ message parameters */
	pAddBssParams = qdf_mem_malloc(sizeof(tAddBssParams));
	if (!pAddBssParams) {
		retCode = QDF_STATUS_E_NOMEM;
		goto returnFailure;
	}

	lim_extract_ap_capabilities(mac, (uint8_t *) bssDescription->ieFields,
			lim_get_ielen_from_bss_description(bssDescription),
			pBeaconStruct);

	if (mac->lim.gLimProtectionControl !=
	    MLME_FORCE_POLICY_PROTECTION_DISABLE)
		lim_decide_sta_protection_on_assoc(mac, pBeaconStruct,
						   pe_session);
	qdf_mem_copy(pAddBssParams->bssId, bssDescription->bssId,
		     sizeof(tSirMacAddr));

	/* Fill in tAddBssParams self_mac_addr */
	qdf_mem_copy(pAddBssParams->self_mac_addr,
		     pe_session->self_mac_addr, sizeof(tSirMacAddr));
	pe_debug("sessionid: %d updateEntry = %d limsystemrole = %d",
		pe_session->smeSessionId, updateEntry,
		GET_LIM_SYSTEM_ROLE(pe_session));

	pe_debug("BSSID: " QDF_MAC_ADDR_STR,
		QDF_MAC_ADDR_ARRAY(pAddBssParams->bssId));
	/* Incorrect BSS Type which caused UMA Descriptor to be overwritten on
	 * top of an already established Infra link. This lead to issues in
	 * concurrent data transfer.
	 */

	pAddBssParams->bssType = pe_session->bssType; /* eSIR_INFRASTRUCTURE_MODE; */
	pAddBssParams->operMode = BSS_OPERATIONAL_MODE_STA;

	pAddBssParams->beaconInterval = bssDescription->beaconInterval;

	pAddBssParams->dtimPeriod = pBeaconStruct->tim.dtimPeriod;
	pAddBssParams->updateBss = updateEntry;

	pAddBssParams->cfParamSet.cfpCount = pBeaconStruct->cfParamSet.cfpCount;
	pAddBssParams->cfParamSet.cfpPeriod =
		pBeaconStruct->cfParamSet.cfpPeriod;
	pAddBssParams->cfParamSet.cfpMaxDuration =
		pBeaconStruct->cfParamSet.cfpMaxDuration;
	pAddBssParams->cfParamSet.cfpDurRemaining =
		pBeaconStruct->cfParamSet.cfpDurRemaining;

	pAddBssParams->rateSet.numRates =
		pBeaconStruct->supportedRates.numRates;
	qdf_mem_copy(pAddBssParams->rateSet.rate,
		     pBeaconStruct->supportedRates.rate,
		     pBeaconStruct->supportedRates.numRates);

	pAddBssParams->nwType = bssDescription->nwType;

	pAddBssParams->shortSlotTimeSupported =
		(uint8_t) pBeaconStruct->capabilityInfo.shortSlotTime;
	pAddBssParams->llaCoexist =
		(uint8_t) pe_session->beaconParams.llaCoexist;
	pAddBssParams->llbCoexist =
		(uint8_t) pe_session->beaconParams.llbCoexist;
	pAddBssParams->llgCoexist =
		(uint8_t) pe_session->beaconParams.llgCoexist;
	pAddBssParams->ht20Coexist =
		(uint8_t) pe_session->beaconParams.ht20Coexist;

	pe_debug("BSS Type %d Beacon Interval: %d dtimPeriod: %d "
		"cfpCount: %d", pAddBssParams->bssType,
		pAddBssParams->beaconInterval, pAddBssParams->dtimPeriod,
		pAddBssParams->cfParamSet.cfpCount);

	pe_debug("cfpPeriod: %d cfpMaxDuration: %d cfpDurRemaining: %d"
		" numRates: %d", pAddBssParams->cfParamSet.cfpPeriod,
		pAddBssParams->cfParamSet.cfpMaxDuration,
		pAddBssParams->cfParamSet.cfpDurRemaining,
		pAddBssParams->rateSet.numRates);

	pe_debug("nwType:%d shortSlotTimeSupported: %d"
		"llaCoexist: %d llbCoexist: %d llgCoexist: %d ht20Coexist: %d",
		pAddBssParams->nwType, pAddBssParams->shortSlotTimeSupported,
		pAddBssParams->llaCoexist, pAddBssParams->llbCoexist,
		pAddBssParams->llgCoexist, pAddBssParams->ht20Coexist);
	/* Use the advertised capabilities from the received beacon/PR */
	if (IS_DOT11_MODE_HT(pe_session->dot11mode)
	    && (pBeaconStruct->HTCaps.present)) {
		pAddBssParams->htCapable = pBeaconStruct->HTCaps.present;
		pe_debug("htCapable: %d", pAddBssParams->htCapable);
		if (pBeaconStruct->HTInfo.present) {
			pAddBssParams->htOperMode =
				(tSirMacHTOperatingMode) pBeaconStruct->HTInfo.
				opMode;
			pAddBssParams->dualCTSProtection =
				(uint8_t) pBeaconStruct->HTInfo.dualCTSProtection;

			chanWidthSupp =
				lim_get_ht_capability(mac,
						      eHT_SUPPORTED_CHANNEL_WIDTH_SET,
						      pe_session);
			if ((pBeaconStruct->HTCaps.supportedChannelWidthSet)
			    && (chanWidthSupp)) {
				pAddBssParams->ch_width =
					(uint8_t) pBeaconStruct->HTInfo.
					recommendedTxWidthSet;
				if (pBeaconStruct->HTInfo.secondaryChannelOffset ==
						PHY_DOUBLE_CHANNEL_LOW_PRIMARY)
					pAddBssParams->ch_center_freq_seg0 =
						bssDescription->channelId + 2;

				if (pBeaconStruct->HTInfo.secondaryChannelOffset ==
						PHY_DOUBLE_CHANNEL_HIGH_PRIMARY)
					pAddBssParams->ch_center_freq_seg0 =
						bssDescription->channelId - 2;
			} else {
				pAddBssParams->ch_width = CH_WIDTH_20MHZ;
				pAddBssParams->ch_center_freq_seg0 = 0;
			}
			pAddBssParams->llnNonGFCoexist =
				(uint8_t) pBeaconStruct->HTInfo.nonGFDevicesPresent;
			pAddBssParams->fLsigTXOPProtectionFullSupport =
				(uint8_t) pBeaconStruct->HTInfo.
				lsigTXOPProtectionFullSupport;
			pAddBssParams->fRIFSMode =
				pBeaconStruct->HTInfo.rifsMode;

			pe_debug("htOperMode: %d dualCTSProtection: %d txChannelWidthSet: %d center_freq_seg0: %d",
				pAddBssParams->htOperMode,
				pAddBssParams->dualCTSProtection,
				pAddBssParams->txChannelWidthSet,
				pAddBssParams->ch_center_freq_seg0);

			pe_debug("llnNonGFCoexist: %d "
				"fLsigTXOPProtectionFullSupport: %d fRIFSMode %d",
				pAddBssParams->llnNonGFCoexist,
				pAddBssParams->fLsigTXOPProtectionFullSupport,
				pAddBssParams->fRIFSMode);
		}
	}

	pAddBssParams->currentOperChannel = bssDescription->channelId;
	pe_debug("currentOperChannel %d",
		pAddBssParams->currentOperChannel);

	if (pe_session->vhtCapability &&
		(IS_BSS_VHT_CAPABLE(pBeaconStruct->VHTCaps) ||
		 IS_BSS_VHT_CAPABLE(pBeaconStruct->vendor_vht_ie.VHTCaps))) {

		pAddBssParams->vhtCapable = 1;
		if (pBeaconStruct->VHTOperation.present)
			vht_oper = &pBeaconStruct->VHTOperation;
		else if (pBeaconStruct->vendor_vht_ie.VHTOperation.present) {
			vht_oper = &pBeaconStruct->vendor_vht_ie.VHTOperation;
			pe_debug("VHT Operation is present in vendor Specific IE");
		}


		if ((vht_oper) &&
			vht_oper->chanWidth &&
			chanWidthSupp) {
			pAddBssParams->ch_center_freq_seg0 =
				vht_oper->chanCenterFreqSeg1;
			pAddBssParams->ch_center_freq_seg1 =
				vht_oper->chanCenterFreqSeg2;
		}
		/*
		 * in limExtractApCapability function intersection of FW
		 * advertised channel width and AP advertised channel width has
		 * been taken into account for calculating
		 * pe_session->ch_width
		 */
		pAddBssParams->ch_width =
			pe_session->ch_width;
		pAddBssParams->staContext.maxAmpduSize =
			SIR_MAC_GET_VHT_MAX_AMPDU_EXPO(
					pAddBssParams->staContext.vht_caps);
	} else {
		pAddBssParams->vhtCapable = 0;
	}

	if (lim_is_session_he_capable(pe_session) &&
	    pBeaconStruct->he_cap.present) {
		lim_update_bss_he_capable(mac, pAddBssParams);
		lim_add_bss_he_cfg(pAddBssParams, pe_session);
	}
	pe_debug("vhtCapable %d vhtTxChannelWidthSet %d center_freq_seg0 - %d, center_freq_seg1 - %d",
		pAddBssParams->vhtCapable, pAddBssParams->ch_width,
		pAddBssParams->ch_center_freq_seg0,
		pAddBssParams->ch_center_freq_seg1);
	/*
	 * Populate the STA-related parameters here
	 * Note that the STA here refers to the AP
	 */
	/* Identifying AP as an STA */
	pAddBssParams->staContext.staType = STA_ENTRY_OTHER;

	qdf_mem_copy(pAddBssParams->staContext.bssId,
			bssDescription->bssId, sizeof(tSirMacAddr));

	listen_interval = mac->mlme_cfg->sap_cfg.listen_interval;
	pAddBssParams->staContext.listenInterval = listen_interval;
	pAddBssParams->staContext.assocId = 0;
	pAddBssParams->staContext.uAPSD = 0;
	pAddBssParams->staContext.maxSPLen = 0;
	pAddBssParams->staContext.shortPreambleSupported =
		(uint8_t) pBeaconStruct->capabilityInfo.shortPreamble;
	pAddBssParams->staContext.updateSta = updateEntry;

	pe_debug("StaCtx: " QDF_MAC_ADDR_STR " shortPreamble: %d",
			QDF_MAC_ADDR_ARRAY(pAddBssParams->staContext.staMac),
			pAddBssParams->staContext.shortPreambleSupported);

	pAddBssParams->dot11_mode = pe_session->dot11mode;
	pe_debug("dot11_mode:%d", pAddBssParams->dot11_mode);

	if (IS_DOT11_MODE_HT(pe_session->dot11mode)
			&& (pBeaconStruct->HTCaps.present)) {
		pAddBssParams->staContext.us32MaxAmpduDuration = 0;
		pAddBssParams->staContext.htCapable = 1;
		pAddBssParams->staContext.greenFieldCapable =
			(uint8_t) pBeaconStruct->HTCaps.greenField;
		pAddBssParams->staContext.lsigTxopProtection =
			(uint8_t) pBeaconStruct->HTCaps.lsigTXOPProtection;
		pe_debug("StaCtx: htCap %d GFCap %d lsigTxopProtn %d",
				pAddBssParams->staContext.htCapable,
				pAddBssParams->staContext.greenFieldCapable,
				pAddBssParams->staContext.lsigTxopProtection);
		if (pe_session->vhtCapability &&
			(IS_BSS_VHT_CAPABLE(pBeaconStruct->VHTCaps) ||
			 IS_BSS_VHT_CAPABLE(
				 pBeaconStruct->vendor_vht_ie.VHTCaps))) {
			pAddBssParams->staContext.vhtCapable = 1;
			if (pBeaconStruct->VHTCaps.present)
				vht_caps = &pBeaconStruct->VHTCaps;
			else if (pBeaconStruct->vendor_vht_ie.VHTCaps.present)
				vht_caps = &pBeaconStruct->
						vendor_vht_ie.VHTCaps;

			if ((vht_caps) && (vht_caps->suBeamFormerCap ||
				vht_caps->muBeamformerCap) &&
				pe_session->vht_config.su_beam_formee)
				pAddBssParams->staContext.vhtTxBFCapable = 1;

			if ((vht_caps) && vht_caps->muBeamformerCap &&
				pe_session->vht_config.mu_beam_formee)
				pAddBssParams->staContext.vhtTxMUBformeeCapable
						= 1;

			if ((vht_caps) && vht_caps->suBeamformeeCap &&
				pe_session->vht_config.su_beam_former)
				pAddBssParams->staContext.enable_su_tx_bformer
						= 1;

			pe_debug("StaContext: su_tx_bfer %d",
				pAddBssParams->staContext.enable_su_tx_bformer);
		}
		if (lim_is_session_he_capable(pe_session) &&
			pBeaconStruct->he_cap.present)
			lim_intersect_ap_he_caps(pe_session, pAddBssParams,
					      pBeaconStruct, NULL);

		if ((pBeaconStruct->HTCaps.supportedChannelWidthSet) &&
				(chanWidthSupp)) {
			pAddBssParams->staContext.ch_width =
				(uint8_t) pBeaconStruct->HTInfo.
				recommendedTxWidthSet;
			if ((vht_oper) &&
					pAddBssParams->staContext.vhtCapable &&
					vht_oper->chanWidth)
				pAddBssParams->staContext.ch_width =
					vht_oper->chanWidth + 1;
			pe_debug("StaCtx: vhtCap %d ch_bw %d TxBF %d",
				   pAddBssParams->staContext.vhtCapable,
				   pAddBssParams->staContext.ch_width,
				   pAddBssParams->staContext.vhtTxBFCapable);
		} else {
			pAddBssParams->staContext.ch_width =
				CH_WIDTH_20MHZ;
		}
		pAddBssParams->staContext.mimoPS =
			(tSirMacHTMIMOPowerSaveState) pBeaconStruct->HTCaps.
			mimoPowerSave;
		pAddBssParams->staContext.maxAmsduSize =
			(uint8_t) pBeaconStruct->HTCaps.maximalAMSDUsize;
		pAddBssParams->staContext.maxAmpduDensity =
			pBeaconStruct->HTCaps.mpduDensity;
		pAddBssParams->staContext.fDsssCckMode40Mhz =
			(uint8_t) pBeaconStruct->HTCaps.dsssCckMode40MHz;
		/*
		 * We will check gShortGI20Mhz and gShortGI40Mhz from ini file.
		 * if they are set then we will use what ever Beacon coming
		 * from AP supports. If these values are set as 0 in ini file
		 * then we will hardcode this values to 0.
		 */
		if (pe_session->ht_config.ht_sgi20)
			pAddBssParams->staContext.fShortGI20Mhz =
				(uint8_t)pBeaconStruct->HTCaps.shortGI20MHz;
		else
			pAddBssParams->staContext.fShortGI20Mhz = false;

		if (pe_session->ht_config.ht_sgi40)
			pAddBssParams->staContext.fShortGI40Mhz =
				(uint8_t) pBeaconStruct->HTCaps.shortGI40MHz;
		else
			pAddBssParams->staContext.fShortGI40Mhz = false;

		pAddBssParams->staContext.maxAmpduSize =
			pBeaconStruct->HTCaps.maxRxAMPDUFactor;
		if (pAddBssParams->staContext.vhtTxBFCapable
		    && vht_cap_info->disable_ldpc_with_txbf_ap) {
			pAddBssParams->staContext.htLdpcCapable = 0;
			pAddBssParams->staContext.vhtLdpcCapable = 0;
		} else {
			if (pe_session->txLdpcIniFeatureEnabled & 0x1)
				pAddBssParams->staContext.htLdpcCapable =
					(uint8_t) pBeaconStruct->HTCaps.
						advCodingCap;
			else
				pAddBssParams->staContext.htLdpcCapable = 0;

			if (pBeaconStruct->VHTCaps.present)
				vht_caps = &pBeaconStruct->VHTCaps;
			else if (pBeaconStruct->vendor_vht_ie.VHTCaps.present) {
				vht_caps =
					&pBeaconStruct->vendor_vht_ie.VHTCaps;
				pe_debug("VHT Caps are in vendor Specific IE");
			}
			if (vht_caps &&
				(pe_session->txLdpcIniFeatureEnabled & 0x2))
				pAddBssParams->staContext.vhtLdpcCapable =
					(uint8_t) vht_caps->ldpcCodingCap;
			else
				pAddBssParams->staContext.vhtLdpcCapable = 0;
		}

		if (pBeaconStruct->HTInfo.present)
			pAddBssParams->staContext.rifsMode =
				pBeaconStruct->HTInfo.rifsMode;
		pe_debug("StaContext ChannelWidth: %d mimoPS: %d maxAmsduSize: %d",
				pAddBssParams->staContext.ch_width,
				pAddBssParams->staContext.mimoPS,
				pAddBssParams->staContext.maxAmsduSize);

		pe_debug("maxAmpduDensity %d Cck40Mhz %d SGI20Mhz %d",
				pAddBssParams->staContext.maxAmpduDensity,
				pAddBssParams->staContext.fDsssCckMode40Mhz,
				pAddBssParams->staContext.fShortGI20Mhz);

		pe_debug("SGI40M %d maxAmpdu %d htLdpc %d vhtLdpc %d",
				pAddBssParams->staContext.fShortGI40Mhz,
				pAddBssParams->staContext.maxAmpduSize,
				pAddBssParams->staContext.htLdpcCapable,
				pAddBssParams->staContext.vhtLdpcCapable);
	}
	/*
	 * If WMM IE or 802.11E IE is not present
	 * and AP is HT AP then enable WMM
	 */
	if ((pe_session->limWmeEnabled && (pBeaconStruct->wmeEdcaPresent ||
			pAddBssParams->staContext.htCapable)) ||
			(pe_session->limQosEnabled &&
			 (pBeaconStruct->edcaPresent ||
			  pAddBssParams->staContext.htCapable)))
		pAddBssParams->staContext.wmmEnabled = 1;
	else
		pAddBssParams->staContext.wmmEnabled = 0;

	/* Update the rates */
	lim_populate_peer_rate_set(mac,
			&pAddBssParams->staContext.
			supportedRates,
			pBeaconStruct->HTCaps.supportedMCSSet,
			false, pe_session,
			&pBeaconStruct->VHTCaps,
			&pBeaconStruct->he_cap);

	pAddBssParams->staContext.encryptType = pe_session->encryptType;

	pAddBssParams->maxTxPower = pe_session->maxTxPower;
	pe_debug("maxTxPower: %d", pAddBssParams->maxTxPower);

	pAddBssParams->status = QDF_STATUS_SUCCESS;
	pAddBssParams->respReqd = true;

	pAddBssParams->staContext.smesessionId = pe_session->smeSessionId;
	pAddBssParams->staContext.sessionId = pe_session->peSessionId;
	pAddBssParams->sessionId = pe_session->peSessionId;

	pAddBssParams->halPersona = (uint8_t)pe_session->opmode;

	pAddBssParams->bSpectrumMgtEnabled = pe_session->spectrumMgtEnabled;

	pAddBssParams->extSetStaKeyParamValid = 0;
	pe_debug("extSetStaKeyParamValid: %d",
		pAddBssParams->extSetStaKeyParamValid);

#ifdef WLAN_FEATURE_11W
	if (pe_session->limRmfEnabled) {
		pAddBssParams->rmfEnabled = 1;
		pAddBssParams->staContext.rmfEnabled = 1;
	}
#endif

	pAddBssParams->nss = pe_session->nss;
	pe_debug("nss value: %d", pAddBssParams->nss);

	/* Set a new state for MLME */
	pe_session->limMlmState = eLIM_MLM_WT_ADD_BSS_RSP_PREASSOC_STATE;

	MTRACE(mac_trace
		       (mac, TRACE_CODE_MLM_STATE, pe_session->peSessionId,
		       pe_session->limMlmState));

	pe_debug("staContext wmmEnabled: %d encryptType: %d "
			       "p2pCapableSta: %d",
		pAddBssParams->staContext.wmmEnabled,
		pAddBssParams->staContext.encryptType,
		pAddBssParams->staContext.p2pCapableSta);

	pe_debug("bSpectrumMgtEnabled: %d halPersona: %d setting "
			       "LimMlm state to %d",
		pAddBssParams->bSpectrumMgtEnabled, pAddBssParams->halPersona,
		pe_session->limMlmState);

	/* we need to defer the message until we get the response back from HAL. */
	SET_LIM_PROCESS_DEFD_MESGS(mac, false);

	if (cds_is_5_mhz_enabled()) {
		pAddBssParams->ch_width = CH_WIDTH_5MHZ;
		pAddBssParams->staContext.ch_width = CH_WIDTH_5MHZ;
	} else if (cds_is_10_mhz_enabled()) {
		pAddBssParams->ch_width = CH_WIDTH_10MHZ;
		pAddBssParams->staContext.ch_width = CH_WIDTH_10MHZ;
	}

	if (lim_is_fils_connection(pe_session))
		pAddBssParams->no_ptk_4_way = true;

	msgQ.type = WMA_ADD_BSS_REQ;
	/** @ToDo : Update the Global counter to keeptrack of the PE <--> HAL messages*/
	msgQ.reserved = 0;
	msgQ.bodyptr = pAddBssParams;
	msgQ.bodyval = 0;

	pe_debug("SessionId:%d Sending WMA_ADD_BSS_REQ",
		pe_session->peSessionId);
	MTRACE(mac_trace_msg_tx(mac, pe_session->peSessionId, msgQ.type));

	retCode = wma_post_ctrl_msg(mac, &msgQ);
	if (QDF_STATUS_SUCCESS != retCode) {
		SET_LIM_PROCESS_DEFD_MESGS(mac, true);
		qdf_mem_free(pAddBssParams);
		pe_err("Posting ADD_BSS_REQ to HAL failed, reason=%X",
			retCode);
		goto returnFailure;

	} else {
		qdf_mem_free(pBeaconStruct);
		return retCode;
	}

returnFailure:
	/* Clean-up will be done by the caller... */
	qdf_mem_free(pBeaconStruct);
	return retCode;
}

/**
 * lim_prepare_and_send_del_sta_cnf() - prepares and send del sta cnf
 *
 * @mac:          mac global context
 * @sta:        sta dph node
 * @status_code:    status code
 * @pe_session: session context
 *
 * deletes DPH entry, changes the MLM mode for station, calls
 * lim_send_del_sta_cnf
 *
 * Return: void
 */
void
lim_prepare_and_send_del_sta_cnf(struct mac_context *mac, tpDphHashNode sta,
				 tSirResultCodes status_code,
				 struct pe_session *pe_session)
{
	uint16_t staDsAssocId = 0;
	struct qdf_mac_addr sta_dsaddr;
	tLimMlmStaContext mlmStaContext;

	if (!sta) {
		pe_err("sta is NULL");
		return;
	}
	staDsAssocId = sta->assocId;
	qdf_mem_copy((uint8_t *) sta_dsaddr.bytes,
		     sta->staAddr, QDF_MAC_ADDR_SIZE);

	mlmStaContext = sta->mlmStaContext;
	if (LIM_IS_AP_ROLE(pe_session))
		lim_release_peer_idx(mac, sta->assocId, pe_session);

	lim_delete_dph_hash_entry(mac, sta->staAddr, sta->assocId,
				  pe_session);

	if (LIM_IS_STA_ROLE(pe_session)) {
		pe_session->limMlmState = eLIM_MLM_IDLE_STATE;
		MTRACE(mac_trace(mac, TRACE_CODE_MLM_STATE,
				 pe_session->peSessionId,
				 pe_session->limMlmState));
	}
	lim_send_del_sta_cnf(mac, sta_dsaddr, staDsAssocId, mlmStaContext,
			     status_code, pe_session);
}

/** -------------------------------------------------------------
   \fn lim_init_pre_auth_timer_table
   \brief Initialize the Pre Auth Tanle and creates the timer for
       each node for the timeout value got from cfg.
   \param     struct mac_context *   mac
   \param     tpLimPreAuthTable pPreAuthTimerTable
   \return none
   -------------------------------------------------------------*/
void lim_init_pre_auth_timer_table(struct mac_context *mac,
				   tpLimPreAuthTable pPreAuthTimerTable)
{
	uint32_t cfgValue;
	uint32_t authNodeIdx;

	tLimPreAuthNode **pAuthNode = pPreAuthTimerTable->pTable;

	/* Get AUTH_RSP Timers value */
	cfgValue = SYS_MS_TO_TICKS(mac->mlme_cfg->timeouts.auth_rsp_timeout);
	for (authNodeIdx = 0; authNodeIdx < pPreAuthTimerTable->numEntry;
	     authNodeIdx++) {
		if (tx_timer_create(mac, &(pAuthNode[authNodeIdx]->timer),
			"AUTH RESPONSE TIMEOUT",
			lim_auth_response_timer_handler, authNodeIdx,
			cfgValue, 0, TX_NO_ACTIVATE) != TX_SUCCESS) {
			pe_err("Cannot create Auth Rsp timer of Index: %d",
				authNodeIdx);
			return;
		}
		pAuthNode[authNodeIdx]->authNodeIdx = (uint8_t) authNodeIdx;
		pAuthNode[authNodeIdx]->fFree = 1;
	}
}

/** -------------------------------------------------------------
   \fn lim_acquire_free_pre_auth_node
   \brief Retrives a free Pre Auth node from Pre Auth Table.
   \param     struct mac_context *   mac
   \param     tpLimPreAuthTable pPreAuthTimerTable
   \return none
   -------------------------------------------------------------*/
tLimPreAuthNode *lim_acquire_free_pre_auth_node(struct mac_context *mac,
						tpLimPreAuthTable pPreAuthTimerTable)
{
	uint32_t i;
	tLimPreAuthNode **pTempNode = pPreAuthTimerTable->pTable;

	for (i = 0; i < pPreAuthTimerTable->numEntry; i++) {
		if (pTempNode[i]->fFree == 1) {
			pTempNode[i]->fFree = 0;
			return pTempNode[i];
		}
	}

	return NULL;
}

/** -------------------------------------------------------------
   \fn lim_get_pre_auth_node_from_index
   \brief Depending on the Index this retrieves the pre auth node.
   \param     struct mac_context *   mac
   \param     tpLimPreAuthTable pAuthTable
   \param     uint32_t authNodeIdx
   \return none
   -------------------------------------------------------------*/
tLimPreAuthNode *lim_get_pre_auth_node_from_index(struct mac_context *mac,
						  tpLimPreAuthTable pAuthTable,
						  uint32_t authNodeIdx)
{
	if ((authNodeIdx >= pAuthTable->numEntry)
	    || (!pAuthTable->pTable)) {
		pe_err("Invalid Auth Timer Index: %d NumEntry: %d",
			authNodeIdx, pAuthTable->numEntry);
		return NULL;
	}

	return pAuthTable->pTable[authNodeIdx];
}

/* Util API to check if the channels supported by STA is within range */
QDF_STATUS lim_is_dot11h_supported_channels_valid(struct mac_context *mac,
						     tSirAssocReq *assoc)
{
	/*
	 * Allow all the stations to join with us.
	 * 802.11h-2003 11.6.1 => An AP may use the supported channels list for associated STAs
	 * as an input into an algorithm used to select a new channel for the BSS.
	 * The specification of the algorithm is beyond the scope of this amendment.
	 */

	return QDF_STATUS_SUCCESS;
}

/* Util API to check if the txpower supported by STA is within range */
QDF_STATUS lim_is_dot11h_power_capabilities_in_range(struct mac_context *mac,
							tSirAssocReq *assoc,
							struct pe_session *pe_session)
{
	int8_t localMaxTxPower;
	uint8_t local_pwr_constraint;

	localMaxTxPower = lim_get_regulatory_max_transmit_power(
					mac, pe_session->currentOperChannel);

	local_pwr_constraint = mac->mlme_cfg->power.local_power_constraint;
	localMaxTxPower -= (int8_t)local_pwr_constraint;

	/**
	 *  The min Tx Power of the associating station should not be greater than (regulatory
	 *  max tx power - local power constraint configured on AP).
	 */
	if (assoc->powerCapability.minTxPower > localMaxTxPower) {
		pe_warn("minTxPower (STA): %d, localMaxTxPower (AP): %d",
			assoc->powerCapability.minTxPower, localMaxTxPower);
		return QDF_STATUS_E_FAILURE;
	}

	return QDF_STATUS_SUCCESS;
}

void lim_fill_rx_highest_supported_rate(struct mac_context *mac,
					uint16_t *rxHighestRate,
					uint8_t *pSupportedMCSSet)
{
	tSirMacRxHighestSupportRate *pRxHighestRate;
	uint8_t *pBuf;
	uint16_t rate = 0;

	pBuf = pSupportedMCSSet + MCS_RX_HIGHEST_SUPPORTED_RATE_BYTE_OFFSET;
	rate = lim_get_u16(pBuf);

	pRxHighestRate = (tSirMacRxHighestSupportRate *) &rate;
	*rxHighestRate = pRxHighestRate->rate;

	return;
}

#ifdef WLAN_FEATURE_11W
/** -------------------------------------------------------------
   \fn     lim_send_sme_unprotected_mgmt_frame_ind
   \brief  Forwards the unprotected management frame to SME.
   \param  struct mac_context *   mac
   \param  frameType - 802.11 frame type
   \param  frame - frame buffer
   \param  sessionId - id for the current session
   \param  pe_session - PE session context
   \return none
   -------------------------------------------------------------*/
void lim_send_sme_unprotected_mgmt_frame_ind(struct mac_context *mac, uint8_t frameType,
					     uint8_t *frame, uint32_t frameLen,
					     uint16_t sessionId,
					     struct pe_session *pe_session)
{
	struct scheduler_msg mmhMsg = {0};
	tSirSmeUnprotMgmtFrameInd *pSirSmeMgmtFrame = NULL;
	uint16_t length;

	length = sizeof(tSirSmeUnprotMgmtFrameInd) + frameLen;

	pSirSmeMgmtFrame = qdf_mem_malloc(length);
	if (!pSirSmeMgmtFrame)
		return;

	pSirSmeMgmtFrame->sessionId = sessionId;
	pSirSmeMgmtFrame->frameType = frameType;

	qdf_mem_copy(pSirSmeMgmtFrame->frameBuf, frame, frameLen);
	pSirSmeMgmtFrame->frameLen = frameLen;

	mmhMsg.type = eWNI_SME_UNPROT_MGMT_FRM_IND;
	mmhMsg.bodyptr = pSirSmeMgmtFrame;
	mmhMsg.bodyval = 0;

	lim_sys_process_mmh_msg_api(mac, &mmhMsg);
	return;
}
#endif

#ifdef FEATURE_WLAN_ESE
void lim_send_sme_tsm_ie_ind(struct mac_context *mac,
			     struct pe_session *pe_session,
			     uint8_t tid, uint8_t state,
			     uint16_t measurement_interval)
{
	struct scheduler_msg msg = {0};
	struct tsm_ie_ind *tsm_ie_ind;

	if (!mac || !pe_session)
		return;

	tsm_ie_ind = qdf_mem_malloc(sizeof(*tsm_ie_ind));
	if (!tsm_ie_ind)
		return;

	tsm_ie_ind->sessionId = pe_session->smeSessionId;
	tsm_ie_ind->tsm_ie.tsid = tid;
	tsm_ie_ind->tsm_ie.state = state;
	tsm_ie_ind->tsm_ie.msmt_interval = measurement_interval;

	msg.type = eWNI_SME_TSM_IE_IND;
	msg.bodyptr = tsm_ie_ind;
	msg.bodyval = 0;

	lim_sys_process_mmh_msg_api(mac, &msg);
}
#endif /* FEATURE_WLAN_ESE */

void lim_extract_ies_from_deauth_disassoc(struct pe_session *session,
					  uint8_t *deauth_disassoc_frame,
					  uint16_t deauth_disassoc_frame_len)
{
	uint16_t reason_code, ie_offset;
	struct wlan_ies ie;

	if (!session) {
		pe_err("NULL session");
		return;
	}

	/* Get the offset of IEs */
	ie_offset = sizeof(struct wlan_frame_hdr) + sizeof(reason_code);

	if (!deauth_disassoc_frame || deauth_disassoc_frame_len <= ie_offset)
		return;

	ie.data = deauth_disassoc_frame + ie_offset;
	ie.len = deauth_disassoc_frame_len - ie_offset;

	mlme_set_peer_disconnect_ies(session->vdev, &ie);
}

