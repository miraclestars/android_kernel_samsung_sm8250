/*
 * Copyright (c) 2011-2018 The Linux Foundation. All rights reserved.
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

/**
 * DOC: This file contains centralized definitions of converged configuration.
 */

#ifndef __CFG_MLME_TWT_H
#define __CFG_MLME_TWT_H

/*
 * <ini>
 * twt_requestor - twt requestor.
 * @Min: 0
 * @Max: 1
 * @Default: 0
 *
 * This cfg is used to store twt requestor config.
 *
 * Related: NA
 *
 * Supported Feature: 11AX
 *
 * Usage: Internal
 *
 * </ini>
 */
#define CFG_TWT_REQUESTOR CFG_INI_BOOL( \
		"twt_requestor", \
		0, \
		"TWT requestor")
/*
 * <ini>
 * twt_responder - twt responder.
 * @Min: 0
 * @Max: 1
 * @Default: 0
 *
 * This cfg is used to store twt responder config.
 *
 * Related: NA
 *
 * Supported Feature: 11AX
 *
 * Usage: Internal
 *
 * </ini>
 */
#define CFG_TWT_RESPONDER CFG_INI_BOOL( \
		"twt_responder", \
		0, \
		"TWT responder")

/*
 * <ini>
 * bcast_twt - to bcast twt capability.
 * @Min: 0
 * @Max: 1
 * @Default: 0
 *
 * This cfg is used to bcast twt capability.
 *
 * Related: NA
 *
 * Supported Feature: 11AX
 *
 * Usage: Internal
 *
 * </ini>
 */
#define CFG_BCAST_TWT CFG_INI_BOOL( \
		"bcast_twt", \
		0, \
		"Bcast TWT")

/*
 * <ini>
 * enable_twt - Enable Target Wake Time support.
 * @Min: 0
 * @Max: 1
 * @Default: 1
 *
 * This ini is used to enable or disable TWT support.
 *
 * Related: NA
 *
 * Supported Feature: 11AX
 *
 * Usage: External
 *
 * </ini>
 */
#define CFG_ENABLE_TWT CFG_INI_BOOL( \
		"enable_twt", \
		1, \
		"TWT support")

/*
 * <ini>
 * twt_congestion_timeout - Target wake time congestion timeout.
 * @Min: 0
 * @Max: 10000
 * @Default: 100
 *
 * STA uses this timer to continuously monitor channel congestion levels to
 * decide whether to start or stop TWT. This ini is used to configure the
 * target wake time congestion timeout value in the units of milliseconds.
 * A value of Zero indicates that this is a host triggered TWT and all the
 * necessary configuration for TWT will be directed from the host.
 *
 * Related: NA
 *
 * Supported Feature: 11AX
 *
 * Usage: External
 *
 * </ini>
 */
#define CFG_TWT_CONGESTION_TIMEOUT CFG_INI_UINT( \
		"twt_congestion_timeout", \
		0, \
		10000, \
		100, \
		CFG_VALUE_OR_DEFAULT, \
		"twt congestion timeout")

#define CFG_TWT_ALL \
	CFG(CFG_BCAST_TWT) \
	CFG(CFG_ENABLE_TWT) \
	CFG(CFG_TWT_REQUESTOR) \
	CFG(CFG_TWT_RESPONDER) \
	CFG(CFG_TWT_CONGESTION_TIMEOUT)

#endif /* __CFG_MLME_TWT_H */
