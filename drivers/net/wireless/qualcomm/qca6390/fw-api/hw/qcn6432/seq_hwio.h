/*
 * Copyright (c) 2023 Qualcomm Innovation Center, Inc. All rights reserved.
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

#ifndef __SEQ_H__
#define __SEQ_H__

#include "HALhwio.h"



/**** Register Ref Read ****/
#define SEQ_INH(base, regtype, reg) \
        SEQ_##regtype##_INH(base, reg)

/**** Masked Register Read ****/
#define SEQ_INMH(base, regtype, reg, mask) \
        SEQ_##regtype##_INMH(base, reg, mask)


/**** Ref Reg Field Read ****/
#define SEQ_INFH(base, regtype, reg, fld) \
        (SEQ_##regtype##_INMH(base, reg, HWIO_FMSK(regtype, fld)) >> HWIO_SHFT(regtype, fld))


/**** Ref Register  Write ****/
#define SEQ_OUTH(base, regtype, reg, val) \
        SEQ_##regtype##_OUTH(base, reg, val)

/**** Ref Register Masked Write ****/
#define SEQ_OUTMH(base, regtype, reg, mask, val) \
        SEQ_##regtype##_OUTMH(base, reg, mask, val)


/**** Ref Register Field Write ****/
#define SEQ_OUTFH(base, regtype, reg, fld, val) \
        SEQ_##regtype##_OUTMH(base, reg, HWIO_FMSK(regtype, fld), val << HWIO_SHFT(regtype, fld))


/**** seq_msg() ****

typedef enum {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL
} SeverityLevel ;

void seq_msg(SeverityLevel severity, unsigned int msg_id, const char *format_str, ... );

*/

/************ seq_wait() ************/

typedef enum {
    SEC,
    MS,
    US,
    NS
} SEQ_TimeUnit;

extern void seq_wait(uint32 time_value, SEQ_TimeUnit time_unit);


/************ seq_poll() ************/
extern uint32 seq_poll(uint32 reg_offset, uint32 expect_value, uint32 value_mask, uint32 value_shift, uint32 max_poll_cnt);

#endif /* __SEQ_H__ */



















