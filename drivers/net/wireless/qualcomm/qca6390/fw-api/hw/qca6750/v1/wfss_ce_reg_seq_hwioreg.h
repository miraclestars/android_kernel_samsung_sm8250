/*
 * Copyright (c) 2020 The Linux Foundation. All rights reserved.
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

#ifndef __WFSS_CE_REG_SEQ_REG_H__
#define __WFSS_CE_REG_SEQ_REG_H__

#define HOST_SOC_WFSS_CE_REG_BASE    (0x01B80000)
#include "hwio.h"

#define HWIO_WFSS_CE_CHANNEL_DST_R0_DEST_CTRL_DEST_MAX_LENGTH_BMSK    \
HWIO_HOST_SOC_CE_0_DST_WFSS_CE_CHANNEL_DST_R0_DEST_CTRL_DEST_MAX_LENGTH_BMSK

#define HWIO_WFSS_CE_CHANNEL_DST_R0_DEST_RING_CONSUMER_PREFETCH_TIMER_RMSK    \
HWIO_HOST_SOC_CE_0_DST_WFSS_CE_CHANNEL_DST_R0_DEST_RING_CONSUMER_PREFETCH_TIMER_RMSK

#define HWIO_WFSS_CE_CHANNEL_DST_R0_DEST_CTRL_ADDR(x)    (x+0x000000b0)
#define HWIO_WFSS_CE_CHANNEL_DST_R0_DEST_RING_CONSUMER_PREFETCH_TIMER_ADDR(x) (x+0x00000040)

#endif
