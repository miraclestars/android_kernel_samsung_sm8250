/*
 * Copyright (c) 2013-2019 The Linux Foundation. All rights reserved.
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

#include "bmi_msg.h"
#include "target_type.h"
#include "cepci.h"

#define MISSING 0
#include "regtable_sdio.h"
#include "targaddrs.h"
#include "if_sdio.h"
#include "ar9888def.h"
#include "ar6320def.h"
#include "ar6320v2def.h"

void target_register_tbl_attach(struct hif_softc *scn, u32 target_type)
{
	switch (target_type) {
	case TARGET_TYPE_AR9888:
		scn->targetdef = &ar9888_targetdef;
		break;
	case TARGET_TYPE_AR6320:
		scn->targetdef = &ar6320_targetdef;
		break;
	case TARGET_TYPE_AR6320V2:
		scn->targetdef = &ar6320v2_targetdef;
		break;
	default:
		break;
	}
}

void hif_register_tbl_attach(struct hif_softc *scn, u32 hif_type)
{
	if (!scn) {
		QDF_TRACE(QDF_MODULE_ID_HIF, QDF_TRACE_LEVEL_ERROR,
			  "%s: sc is NULL", __func__);
		return;
	}

	switch (hif_type) {
	case HIF_TYPE_AR9888:
		scn->hostdef = &ar9888_hostdef;
		break;
	case HIF_TYPE_AR6320:
		scn->hostdef = &ar6320_hostdef;
		break;
	case HIF_TYPE_AR6320V2:
		scn->hostdef = &ar6320v2_hostdef;
		break;
	default:
		break;
	}
}
