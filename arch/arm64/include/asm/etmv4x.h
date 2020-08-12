/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2015-2017, The Linux Foundation. All rights reserved.
 */


#ifndef __ASM_ETMV4X_H
#define __ASM_ETMV4X_H

#include <linux/types.h>

/* 32 bit register reads for AArch64 */
#define trc_readl(reg)			RSYSL_##reg()
/* 64 bit register reads for AArch64 */
#define trc_readq(reg)			RSYSQ_##reg()
/* 32 and 64 bit register writes for AArch64 */
#define trc_write(val, reg)		WSYS_##reg(val)

#define MRSL(op0, op1, crn, crm, op2)					     \
({									     \
uint32_t val;								     \
asm volatile("mrs %0, S"#op0"_"#op1"_"#crn"_"#crm"_"#op2 : "=r" (val));      \
val;									     \
})

#define MRSQ(op0, op1, crn, crm, op2)					     \
({									     \
uint64_t val;								     \
asm volatile("mrs %0, S"#op0"_"#op1"_"#crn"_"#crm"_"#op2 : "=r" (val));      \
val;									     \
})

#define MSR(val, op0, op1, crn, crm, op2)				     \
({									     \
asm volatile("msr S"#op0"_"#op1"_"#crn"_"#crm"_"#op2", %0" : : "r" (val));   \
})

/* Clock and Power Management Register */
#define RSYSL_CPMR_EL1()		MRSL(3, 7, c15, c0, 5)
#define WSYS_CPMR_EL1(val)		MSR(val, 3, 7, c15, c0, 5)

/*
 * ETMv4 Registers
 *
 * Read only
 * ETMAUTHSTATUS, ETMDEVARCH, ETMDEVID, ETMIDRn[0-13], ETMOSLSR, ETMSTATR
 *
 * Write only
 * ETMOSLAR
 */
/* 32 bit registers */
#define RSYSL_ETMAUTHSTATUS()		MRSL(2, 1, c7, c14, 6)
#define RSYSL_ETMAUXCTLR()		MRSL(2, 1, c0, c6, 0)
#define RSYSL_ETMCCCTLR()		MRSL(2, 1, c0, c14, 0)
#define RSYSL_ETMCIDCCTLR0()		MRSL(2, 1, c3, c0, 2)
#define RSYSL_ETMCNTCTLR0()		MRSL(2, 1, c0, c4, 5)
#define RSYSL_ETMCNTCTLR1()		MRSL(2, 1, c0, c5, 5)
#define RSYSL_ETMCNTCTLR2()		MRSL(2, 1, c0, c6, 5)
#define RSYSL_ETMCNTCTLR3()		MRSL(2, 1, c0, c7, 5)
#define RSYSL_ETMCNTRLDVR0()		MRSL(2, 1, c0, c0, 5)
#define RSYSL_ETMCNTRLDVR1()		MRSL(2, 1, c0, c1, 5)
#define RSYSL_ETMCNTRLDVR2()		MRSL(2, 1, c0, c2, 5)
#define RSYSL_ETMCNTRLDVR3()		MRSL(2, 1, c0, c3, 5)
#define RSYSL_ETMCNTVR0()		MRSL(2, 1, c0, c8, 5)
#define RSYSL_ETMCNTVR1()		MRSL(2, 1, c0, c9, 5)
#define RSYSL_ETMCNTVR2()		MRSL(2, 1, c0, c10, 5)
#define RSYSL_ETMCNTVR3()		MRSL(2, 1, c0, c11, 5)
#define RSYSL_ETMCONFIGR()		MRSL(2, 1, c0, c4, 0)
#define RSYSL_ETMDEVARCH()		MRSL(2, 1, c7, c15, 6)
#define RSYSL_ETMDEVID()		MRSL(2, 1, c7, c2, 7)
#define RSYSL_ETMEVENTCTL0R()		MRSL(2, 1, c0, c8, 0)
#define RSYSL_ETMEVENTCTL1R()		MRSL(2, 1, c0, c9, 0)
#define RSYSL_ETMEXTINSELR()		MRSL(2, 1, c0, c8, 4)
#define RSYSL_ETMIDR0()			MRSL(2, 1, c0, c8, 7)
#define RSYSL_ETMIDR1()			MRSL(2, 1, c0, c9, 7)
#define RSYSL_ETMIDR10()		MRSL(2, 1, c0, c2, 6)
#define RSYSL_ETMIDR11()		MRSL(2, 1, c0, c3, 6)
#define RSYSL_ETMIDR12()		MRSL(2, 1, c0, c4, 6)
#define RSYSL_ETMIDR13()		MRSL(2, 1, c0, c5, 6)
#define RSYSL_ETMIDR2()			MRSL(2, 1, c0, c10, 7)
#define RSYSL_ETMIDR3()			MRSL(2, 1, c0, c11, 7)
#define RSYSL_ETMIDR4()			MRSL(2, 1, c0, c12, 7)
#define RSYSL_ETMIDR5()			MRSL(2, 1, c0, c13, 7)
#define RSYSL_ETMIDR6()			MRSL(2, 1, c0, c14, 7)
#define RSYSL_ETMIDR7()			MRSL(2, 1, c0, c15, 7)
#define RSYSL_ETMIDR8()			MRSL(2, 1, c0, c0, 6)
#define RSYSL_ETMIDR9()			MRSL(2, 1, c0, c1, 6)
#define RSYSL_ETMIMSPEC0()		MRSL(2, 1, c0, c0, 7)
#define RSYSL_ETMOSLSR()		MRSL(2, 1, c1, c1, 4)
#define RSYSL_ETMPRGCTLR()		MRSL(2, 1, c0, c1, 0)
#define RSYSL_ETMRSCTLR10()		MRSL(2, 1, c1, c10, 0)
#define RSYSL_ETMRSCTLR11()		MRSL(2, 1, c1, c11, 0)
#define RSYSL_ETMRSCTLR12()		MRSL(2, 1, c1, c12, 0)
#define RSYSL_ETMRSCTLR13()		MRSL(2, 1, c1, c13, 0)
#define RSYSL_ETMRSCTLR14()		MRSL(2, 1, c1, c14, 0)
#define RSYSL_ETMRSCTLR15()		MRSL(2, 1, c1, c15, 0)
#define RSYSL_ETMRSCTLR2()		MRSL(2, 1, c1, c2, 0)
#define RSYSL_ETMRSCTLR3()		MRSL(2, 1, c1, c3, 0)
#define RSYSL_ETMRSCTLR4()		MRSL(2, 1, c1, c4, 0)
#define RSYSL_ETMRSCTLR5()		MRSL(2, 1, c1, c5, 0)
#define RSYSL_ETMRSCTLR6()		MRSL(2, 1, c1, c6, 0)
#define RSYSL_ETMRSCTLR7()		MRSL(2, 1, c1, c7, 0)
#define RSYSL_ETMRSCTLR8()		MRSL(2, 1, c1, c8, 0)
#define RSYSL_ETMRSCTLR9()		MRSL(2, 1, c1, c9, 0)
#define RSYSL_ETMRSCTLR16()		MRSL(2, 1, c1, c0, 1)
#define RSYSL_ETMRSCTLR17()		MRSL(2, 1, c1, c1, 1)
#define RSYSL_ETMRSCTLR18()		MRSL(2, 1, c1, c2, 1)
#define RSYSL_ETMRSCTLR19()		MRSL(2, 1, c1, c3, 1)
#define RSYSL_ETMRSCTLR20()		MRSL(2, 1, c1, c4, 1)
#define RSYSL_ETMRSCTLR21()		MRSL(2, 1, c1, c5, 1)
#define RSYSL_ETMRSCTLR22()		MRSL(2, 1, c1, c6, 1)
#define RSYSL_ETMRSCTLR23()		MRSL(2, 1, c1, c7, 1)
#define RSYSL_ETMRSCTLR24()		MRSL(2, 1, c1, c8, 1)
#define RSYSL_ETMRSCTLR25()		MRSL(2, 1, c1, c9, 1)
#define RSYSL_ETMRSCTLR26()		MRSL(2, 1, c1, c10, 1)
#define RSYSL_ETMRSCTLR27()		MRSL(2, 1, c1, c11, 1)
#define RSYSL_ETMRSCTLR28()		MRSL(2, 1, c1, c12, 1)
#define RSYSL_ETMRSCTLR29()		MRSL(2, 1, c1, c13, 1)
#define RSYSL_ETMRSCTLR30()		MRSL(2, 1, c1, c14, 1)
#define RSYSL_ETMRSCTLR31()		MRSL(2, 1, c1, c15, 1)
#define RSYSL_ETMSEQEVR0()		MRSL(2, 1, c0, c0, 4)
#define RSYSL_ETMSEQEVR1()		MRSL(2, 1, c0, c1, 4)
#define RSYSL_ETMSEQEVR2()		MRSL(2, 1, c0, c2, 4)
#define RSYSL_ETMSEQRSTEVR()		MRSL(2, 1, c0, c6, 4)
#define RSYSL_ETMSEQSTR()		MRSL(2, 1, c0, c7, 4)
#define RSYSL_ETMSTALLCTLR()		MRSL(2, 1, c0, c11, 0)
#define RSYSL_ETMSTATR()		MRSL(2, 1, c0, c3, 0)
#define RSYSL_ETMSYNCPR()		MRSL(2, 1, c0, c13, 0)
#define RSYSL_ETMTRACEIDR()		MRSL(2, 1, c0, c0, 1)
#define RSYSL_ETMTSCTLR()		MRSL(2, 1, c0, c12, 0)
#define RSYSL_ETMVICTLR()		MRSL(2, 1, c0, c0, 2)
#define RSYSL_ETMVIIECTLR()		MRSL(2, 1, c0, c1, 2)
#define RSYSL_ETMVISSCTLR()		MRSL(2, 1, c0, c2, 2)
#define RSYSL_ETMSSCCR0()		MRSL(2, 1, c1, c0, 2)
#define RSYSL_ETMSSCCR1()		MRSL(2, 1, c1, c1, 2)
#define RSYSL_ETMSSCCR2()		MRSL(2, 1, c1, c2, 2)
#define RSYSL_ETMSSCCR3()		MRSL(2, 1, c1, c3, 2)
#define RSYSL_ETMSSCCR4()		MRSL(2, 1, c1, c4, 2)
#define RSYSL_ETMSSCCR5()		MRSL(2, 1, c1, c5, 2)
#define RSYSL_ETMSSCCR6()		MRSL(2, 1, c1, c6, 2)
#define RSYSL_ETMSSCCR7()		MRSL(2, 1, c1, c7, 2)
#define RSYSL_ETMSSCSR0()		MRSL(2, 1, c1, c8, 2)
#define RSYSL_ETMSSCSR1()		MRSL(2, 1, c1, c9, 2)
#define RSYSL_ETMSSCSR2()		MRSL(2, 1, c1, c10, 2)
#define RSYSL_ETMSSCSR3()		MRSL(2, 1, c1, c11, 2)
#define RSYSL_ETMSSCSR4()		MRSL(2, 1, c1, c12, 2)
#define RSYSL_ETMSSCSR5()		MRSL(2, 1, c1, c13, 2)
#define RSYSL_ETMSSCSR6()		MRSL(2, 1, c1, c14, 2)
#define RSYSL_ETMSSCSR7()		MRSL(2, 1, c1, c15, 2)
#define RSYSL_ETMSSPCICR0()		MRSL(2, 1, c1, c0, 3)
#define RSYSL_ETMSSPCICR1()		MRSL(2, 1, c1, c1, 3)
#define RSYSL_ETMSSPCICR2()		MRSL(2, 1, c1, c2, 3)
#define RSYSL_ETMSSPCICR3()		MRSL(2, 1, c1, c3, 3)
#define RSYSL_ETMSSPCICR4()		MRSL(2, 1, c1, c4, 3)
#define RSYSL_ETMSSPCICR5()		MRSL(2, 1, c1, c5, 3)
#define RSYSL_ETMSSPCICR6()		MRSL(2, 1, c1, c6, 3)
#define RSYSL_ETMSSPCICR7()		MRSL(2, 1, c1, c7, 3)

/* 64 bit registers */
#define RSYSQ_ETMACATR0()		MRSQ(2, 1, c2, c0, 2)
#define RSYSQ_ETMACATR1()		MRSQ(2, 1, c2, c2, 2)
#define RSYSQ_ETMACATR2()		MRSQ(2, 1, c2, c4, 2)
#define RSYSQ_ETMACATR3()		MRSQ(2, 1, c2, c6, 2)
#define RSYSQ_ETMACATR4()		MRSQ(2, 1, c2, c8, 2)
#define RSYSQ_ETMACATR5()		MRSQ(2, 1, c2, c10, 2)
#define RSYSQ_ETMACATR6()		MRSQ(2, 1, c2, c12, 2)
#define RSYSQ_ETMACATR7()		MRSQ(2, 1, c2, c14, 2)
#define RSYSQ_ETMACATR8()		MRSQ(2, 1, c2, c0, 3)
#define RSYSQ_ETMACATR9()		MRSQ(2, 1, c2, c2, 3)
#define RSYSQ_ETMACATR10()		MRSQ(2, 1, c2, c4, 3)
#define RSYSQ_ETMACATR11()		MRSQ(2, 1, c2, c6, 3)
#define RSYSQ_ETMACATR12()		MRSQ(2, 1, c2, c8, 3)
#define RSYSQ_ETMACATR13()		MRSQ(2, 1, c2, c10, 3)
#define RSYSQ_ETMACATR14()		MRSQ(2, 1, c2, c12, 3)
#define RSYSQ_ETMACATR15()		MRSQ(2, 1, c2, c14, 3)
#define RSYSQ_ETMCIDCVR0()		MRSQ(2, 1, c3, c0, 0)
#define RSYSQ_ETMCIDCVR1()		MRSQ(2, 1, c3, c2, 0)
#define RSYSQ_ETMCIDCVR2()		MRSQ(2, 1, c3, c4, 0)
#define RSYSQ_ETMCIDCVR3()		MRSQ(2, 1, c3, c6, 0)
#define RSYSQ_ETMCIDCVR4()		MRSQ(2, 1, c3, c8, 0)
#define RSYSQ_ETMCIDCVR5()		MRSQ(2, 1, c3, c10, 0)
#define RSYSQ_ETMCIDCVR6()		MRSQ(2, 1, c3, c12, 0)
#define RSYSQ_ETMCIDCVR7()		MRSQ(2, 1, c3, c14, 0)
#define RSYSQ_ETMACVR0()		MRSQ(2, 1, c2, c0, 0)
#define RSYSQ_ETMACVR1()		MRSQ(2, 1, c2, c2, 0)
#define RSYSQ_ETMACVR2()		MRSQ(2, 1, c2, c4, 0)
#define RSYSQ_ETMACVR3()		MRSQ(2, 1, c2, c6, 0)
#define RSYSQ_ETMACVR4()		MRSQ(2, 1, c2, c8, 0)
#define RSYSQ_ETMACVR5()		MRSQ(2, 1, c2, c10, 0)
#define RSYSQ_ETMACVR6()		MRSQ(2, 1, c2, c12, 0)
#define RSYSQ_ETMACVR7()		MRSQ(2, 1, c2, c14, 0)
#define RSYSQ_ETMACVR8()		MRSQ(2, 1, c2, c0, 1)
#define RSYSQ_ETMACVR9()		MRSQ(2, 1, c2, c2, 1)
#define RSYSQ_ETMACVR10()		MRSQ(2, 1, c2, c4, 1)
#define RSYSQ_ETMACVR11()		MRSQ(2, 1, c2, c6, 1)
#define RSYSQ_ETMACVR12()		MRSQ(2, 1, c2, c8, 1)
#define RSYSQ_ETMACVR13()		MRSQ(2, 1, c2, c10, 1)
#define RSYSQ_ETMACVR14()		MRSQ(2, 1, c2, c12, 1)
#define RSYSQ_ETMACVR15()		MRSQ(2, 1, c2, c14, 1)
#define RSYSQ_ETMVMIDCVR0()		MRSQ(2, 1, c3, c0, 1)
#define RSYSQ_ETMVMIDCVR1()		MRSQ(2, 1, c3, c2, 1)
#define RSYSQ_ETMVMIDCVR2()		MRSQ(2, 1, c3, c4, 1)
#define RSYSQ_ETMVMIDCVR3()		MRSQ(2, 1, c3, c6, 1)
#define RSYSQ_ETMVMIDCVR4()		MRSQ(2, 1, c3, c8, 1)
#define RSYSQ_ETMVMIDCVR5()		MRSQ(2, 1, c3, c10, 1)
#define RSYSQ_ETMVMIDCVR6()		MRSQ(2, 1, c3, c12, 1)
#define RSYSQ_ETMVMIDCVR7()		MRSQ(2, 1, c3, c14, 1)
#define RSYSQ_ETMDVCVR0()		MRSQ(2, 1, c2, c0, 4)
#define RSYSQ_ETMDVCVR1()		MRSQ(2, 1, c2, c4, 4)
#define RSYSQ_ETMDVCVR2()		MRSQ(2, 1, c2, c8, 4)
#define RSYSQ_ETMDVCVR3()		MRSQ(2, 1, c2, c12, 4)
#define RSYSQ_ETMDVCVR4()		MRSQ(2, 1, c2, c0, 5)
#define RSYSQ_ETMDVCVR5()		MRSQ(2, 1, c2, c4, 5)
#define RSYSQ_ETMDVCVR6()		MRSQ(2, 1, c2, c8, 5)
#define RSYSQ_ETMDVCVR7()		MRSQ(2, 1, c2, c12, 5)
#define RSYSQ_ETMDVCMR0()		MRSQ(2, 1, c2, c0, 6)
#define RSYSQ_ETMDVCMR1()		MRSQ(2, 1, c2, c4, 6)
#define RSYSQ_ETMDVCMR2()		MRSQ(2, 1, c2, c8, 6)
#define RSYSQ_ETMDVCMR3()		MRSQ(2, 1, c2, c12, 6)
#define RSYSQ_ETMDVCMR4()		MRSQ(2, 1, c2, c0, 7)
#define RSYSQ_ETMDVCMR5()		MRSQ(2, 1, c2, c4, 7)
#define RSYSQ_ETMDVCMR6()		MRSQ(2, 1, c2, c8, 7)
#define RSYSQ_ETMDVCMR7()		MRSQ(2, 1, c2, c12, 7)

/* 32 and 64 bit registers */
#define WSYS_ETMAUXCTLR(val)		MSR(val, 2, 1, c0, c6, 0)
#define WSYS_ETMACATR0(val)		MSR(val, 2, 1, c2, c0, 2)
#define WSYS_ETMACATR1(val)		MSR(val, 2, 1, c2, c2, 2)
#define WSYS_ETMACATR2(val)		MSR(val, 2, 1, c2, c4, 2)
#define WSYS_ETMACATR3(val)		MSR(val, 2, 1, c2, c6, 2)
#define WSYS_ETMACATR4(val)		MSR(val, 2, 1, c2, c8, 2)
#define WSYS_ETMACATR5(val)		MSR(val, 2, 1, c2, c10, 2)
#define WSYS_ETMACATR6(val)		MSR(val, 2, 1, c2, c12, 2)
#define WSYS_ETMACATR7(val)		MSR(val, 2, 1, c2, c14, 2)
#define WSYS_ETMACATR8(val)		MSR(val, 2, 1, c2, c0, 3)
#define WSYS_ETMACATR9(val)		MSR(val, 2, 1, c2, c2, 3)
#define WSYS_ETMACATR10(val)		MSR(val, 2, 1, c2, c4, 3)
#define WSYS_ETMACATR11(val)		MSR(val, 2, 1, c2, c6, 3)
#define WSYS_ETMACATR12(val)		MSR(val, 2, 1, c2, c8, 3)
#define WSYS_ETMACATR13(val)		MSR(val, 2, 1, c2, c10, 3)
#define WSYS_ETMACATR14(val)		MSR(val, 2, 1, c2, c12, 3)
#define WSYS_ETMACATR15(val)		MSR(val, 2, 1, c2, c14, 3)
#define WSYS_ETMACVR0(val)		MSR(val, 2, 1, c2, c0, 0)
#define WSYS_ETMACVR1(val)		MSR(val, 2, 1, c2, c2, 0)
#define WSYS_ETMACVR2(val)		MSR(val, 2, 1, c2, c4, 0)
#define WSYS_ETMACVR3(val)		MSR(val, 2, 1, c2, c6, 0)
#define WSYS_ETMACVR4(val)		MSR(val, 2, 1, c2, c8, 0)
#define WSYS_ETMACVR5(val)		MSR(val, 2, 1, c2, c10, 0)
#define WSYS_ETMACVR6(val)		MSR(val, 2, 1, c2, c12, 0)
#define WSYS_ETMACVR7(val)		MSR(val, 2, 1, c2, c14, 0)
#define WSYS_ETMACVR8(val)		MSR(val, 2, 1, c2, c0, 1)
#define WSYS_ETMACVR9(val)		MSR(val, 2, 1, c2, c2, 1)
#define WSYS_ETMACVR10(val)		MSR(val, 2, 1, c2, c4, 1)
#define WSYS_ETMACVR11(val)		MSR(val, 2, 1, c2, c6, 1)
#define WSYS_ETMACVR12(val)		MSR(val, 2, 1, c2, c8, 1)
#define WSYS_ETMACVR13(val)		MSR(val, 2, 1, c2, c10, 1)
#define WSYS_ETMACVR14(val)		MSR(val, 2, 1, c2, c12, 1)
#define WSYS_ETMACVR15(val)		MSR(val, 2, 1, c2, c14, 1)
#define WSYS_ETMCCCTLR(val)		MSR(val, 2, 1, c0, c14, 0)
#define WSYS_ETMCIDCCTLR0(val)		MSR(val, 2, 1, c3, c0, 2)
#define WSYS_ETMCIDCVR0(val)		MSR(val, 2, 1, c3, c0, 0)
#define WSYS_ETMCIDCVR1(val)		MSR(val, 2, 1, c3, c2, 0)
#define WSYS_ETMCIDCVR2(val)		MSR(val, 2, 1, c3, c4, 0)
#define WSYS_ETMCIDCVR3(val)		MSR(val, 2, 1, c3, c6, 0)
#define WSYS_ETMCIDCVR4(val)		MSR(val, 2, 1, c3, c8, 0)
#define WSYS_ETMCIDCVR5(val)		MSR(val, 2, 1, c3, c10, 0)
#define WSYS_ETMCIDCVR6(val)		MSR(val, 2, 1, c3, c12, 0)
#define WSYS_ETMCIDCVR7(val)		MSR(val, 2, 1, c3, c14, 0)
#define WSYS_ETMCNTCTLR0(val)		MSR(val, 2, 1, c0, c4, 5)
#define WSYS_ETMCNTCTLR1(val)		MSR(val, 2, 1, c0, c5, 5)
#define WSYS_ETMCNTCTLR2(val)		MSR(val, 2, 1, c0, c6, 5)
#define WSYS_ETMCNTCTLR3(val)		MSR(val, 2, 1, c0, c7, 5)
#define WSYS_ETMCNTRLDVR0(val)		MSR(val, 2, 1, c0, c0, 5)
#define WSYS_ETMCNTRLDVR1(val)		MSR(val, 2, 1, c0, c1, 5)
#define WSYS_ETMCNTRLDVR2(val)		MSR(val, 2, 1, c0, c2, 5)
#define WSYS_ETMCNTRLDVR3(val)		MSR(val, 2, 1, c0, c3, 5)
#define WSYS_ETMCNTVR0(val)		MSR(val, 2, 1, c0, c8, 5)
#define WSYS_ETMCNTVR1(val)		MSR(val, 2, 1, c0, c9, 5)
#define WSYS_ETMCNTVR2(val)		MSR(val, 2, 1, c0, c10, 5)
#define WSYS_ETMCNTVR3(val)		MSR(val, 2, 1, c0, c11, 5)
#define WSYS_ETMCONFIGR(val)		MSR(val, 2, 1, c0, c4, 0)
#define WSYS_ETMEVENTCTL0R(val)		MSR(val, 2, 1, c0, c8, 0)
#define WSYS_ETMEVENTCTL1R(val)		MSR(val, 2, 1, c0, c9, 0)
#define WSYS_ETMEXTINSELR(val)		MSR(val, 2, 1, c0, c8, 4)
#define WSYS_ETMIMSPEC0(val)		MSR(val, 2, 1, c0, c0, 7)
#define WSYS_ETMOSLAR(val)		MSR(val, 2, 1, c1, c0, 4)
#define WSYS_ETMPRGCTLR(val)		MSR(val, 2, 1, c0, c1, 0)
#define WSYS_ETMRSCTLR10(val)		MSR(val, 2, 1, c1, c10, 0)
#define WSYS_ETMRSCTLR11(val)		MSR(val, 2, 1, c1, c11, 0)
#define WSYS_ETMRSCTLR12(val)		MSR(val, 2, 1, c1, c12, 0)
#define WSYS_ETMRSCTLR13(val)		MSR(val, 2, 1, c1, c13, 0)
#define WSYS_ETMRSCTLR14(val)		MSR(val, 2, 1, c1, c14, 0)
#define WSYS_ETMRSCTLR15(val)		MSR(val, 2, 1, c1, c15, 0)
#define WSYS_ETMRSCTLR2(val)		MSR(val, 2, 1, c1, c2, 0)
#define WSYS_ETMRSCTLR3(val)		MSR(val, 2, 1, c1, c3, 0)
#define WSYS_ETMRSCTLR4(val)		MSR(val, 2, 1, c1, c4, 0)
#define WSYS_ETMRSCTLR5(val)		MSR(val, 2, 1, c1, c5, 0)
#define WSYS_ETMRSCTLR6(val)		MSR(val, 2, 1, c1, c6, 0)
#define WSYS_ETMRSCTLR7(val)		MSR(val, 2, 1, c1, c7, 0)
#define WSYS_ETMRSCTLR8(val)		MSR(val, 2, 1, c1, c8, 0)
#define WSYS_ETMRSCTLR9(val)		MSR(val, 2, 1, c1, c9, 0)
#define WSYS_ETMRSCTLR16(val)		MSR(val, 2, 1, c1, c0, 1)
#define WSYS_ETMRSCTLR17(val)		MSR(val, 2, 1, c1, c1, 1)
#define WSYS_ETMRSCTLR18(val)		MSR(val, 2, 1, c1, c2, 1)
#define WSYS_ETMRSCTLR19(val)		MSR(val, 2, 1, c1, c3, 1)
#define WSYS_ETMRSCTLR20(val)		MSR(val, 2, 1, c1, c4, 1)
#define WSYS_ETMRSCTLR21(val)		MSR(val, 2, 1, c1, c5, 1)
#define WSYS_ETMRSCTLR22(val)		MSR(val, 2, 1, c1, c6, 1)
#define WSYS_ETMRSCTLR23(val)		MSR(val, 2, 1, c1, c7, 1)
#define WSYS_ETMRSCTLR24(val)		MSR(val, 2, 1, c1, c8, 1)
#define WSYS_ETMRSCTLR25(val)		MSR(val, 2, 1, c1, c9, 1)
#define WSYS_ETMRSCTLR26(val)		MSR(val, 2, 1, c1, c10, 1)
#define WSYS_ETMRSCTLR27(val)		MSR(val, 2, 1, c1, c11, 1)
#define WSYS_ETMRSCTLR28(val)		MSR(val, 2, 1, c1, c12, 1)
#define WSYS_ETMRSCTLR29(val)		MSR(val, 2, 1, c1, c13, 1)
#define WSYS_ETMRSCTLR30(val)		MSR(val, 2, 1, c1, c14, 1)
#define WSYS_ETMRSCTLR31(val)		MSR(val, 2, 1, c1, c15, 1)
#define WSYS_ETMSEQEVR0(val)		MSR(val, 2, 1, c0, c0, 4)
#define WSYS_ETMSEQEVR1(val)		MSR(val, 2, 1, c0, c1, 4)
#define WSYS_ETMSEQEVR2(val)		MSR(val, 2, 1, c0, c2, 4)
#define WSYS_ETMSEQRSTEVR(val)		MSR(val, 2, 1, c0, c6, 4)
#define WSYS_ETMSEQSTR(val)		MSR(val, 2, 1, c0, c7, 4)
#define WSYS_ETMSTALLCTLR(val)		MSR(val, 2, 1, c0, c11, 0)
#define WSYS_ETMSYNCPR(val)		MSR(val, 2, 1, c0, c13, 0)
#define WSYS_ETMTRACEIDR(val)		MSR(val, 2, 1, c0, c0, 1)
#define WSYS_ETMTSCTLR(val)		MSR(val, 2, 1, c0, c12, 0)
#define WSYS_ETMVICTLR(val)		MSR(val, 2, 1, c0, c0, 2)
#define WSYS_ETMVIIECTLR(val)		MSR(val, 2, 1, c0, c1, 2)
#define WSYS_ETMVISSCTLR(val)		MSR(val, 2, 1, c0, c2, 2)
#define WSYS_ETMVMIDCVR0(val)		MSR(val, 2, 1, c3, c0, 1)
#define WSYS_ETMVMIDCVR1(val)		MSR(val, 2, 1, c3, c2, 1)
#define WSYS_ETMVMIDCVR2(val)		MSR(val, 2, 1, c3, c4, 1)
#define WSYS_ETMVMIDCVR3(val)		MSR(val, 2, 1, c3, c6, 1)
#define WSYS_ETMVMIDCVR4(val)		MSR(val, 2, 1, c3, c8, 1)
#define WSYS_ETMVMIDCVR5(val)		MSR(val, 2, 1, c3, c10, 1)
#define WSYS_ETMVMIDCVR6(val)		MSR(val, 2, 1, c3, c12, 1)
#define WSYS_ETMVMIDCVR7(val)		MSR(val, 2, 1, c3, c14, 1)
#define WSYS_ETMDVCVR0(val)		MSR(val, 2, 1, c2, c0, 4)
#define WSYS_ETMDVCVR1(val)		MSR(val, 2, 1, c2, c4, 4)
#define WSYS_ETMDVCVR2(val)		MSR(val, 2, 1, c2, c8, 4)
#define WSYS_ETMDVCVR3(val)		MSR(val, 2, 1, c2, c12, 4)
#define WSYS_ETMDVCVR4(val)		MSR(val, 2, 1, c2, c0, 5)
#define WSYS_ETMDVCVR5(val)		MSR(val, 2, 1, c2, c4, 5)
#define WSYS_ETMDVCVR6(val)		MSR(val, 2, 1, c2, c8, 5)
#define WSYS_ETMDVCVR7(val)		MSR(val, 2, 1, c2, c12, 5)
#define WSYS_ETMDVCMR0(val)		MSR(val, 2, 1, c2, c0, 6)
#define WSYS_ETMDVCMR1(val)		MSR(val, 2, 1, c2, c4, 6)
#define WSYS_ETMDVCMR2(val)		MSR(val, 2, 1, c2, c8, 6)
#define WSYS_ETMDVCMR3(val)		MSR(val, 2, 1, c2, c12, 6)
#define WSYS_ETMDVCMR4(val)		MSR(val, 2, 1, c2, c0, 7)
#define WSYS_ETMDVCMR5(val)		MSR(val, 2, 1, c2, c4, 7)
#define WSYS_ETMDVCMR6(val)		MSR(val, 2, 1, c2, c8, 7)
#define WSYS_ETMDVCMR7(val)		MSR(val, 2, 1, c2, c12, 7)
#define WSYS_ETMSSCCR0(val)		MSR(val, 2, 1, c1, c0, 2)
#define WSYS_ETMSSCCR1(val)		MSR(val, 2, 1, c1, c1, 2)
#define WSYS_ETMSSCCR2(val)		MSR(val, 2, 1, c1, c2, 2)
#define WSYS_ETMSSCCR3(val)		MSR(val, 2, 1, c1, c3, 2)
#define WSYS_ETMSSCCR4(val)		MSR(val, 2, 1, c1, c4, 2)
#define WSYS_ETMSSCCR5(val)		MSR(val, 2, 1, c1, c5, 2)
#define WSYS_ETMSSCCR6(val)		MSR(val, 2, 1, c1, c6, 2)
#define WSYS_ETMSSCCR7(val)		MSR(val, 2, 1, c1, c7, 2)
#define WSYS_ETMSSCSR0(val)		MSR(val, 2, 1, c1, c8, 2)
#define WSYS_ETMSSCSR1(val)		MSR(val, 2, 1, c1, c9, 2)
#define WSYS_ETMSSCSR2(val)		MSR(val, 2, 1, c1, c10, 2)
#define WSYS_ETMSSCSR3(val)		MSR(val, 2, 1, c1, c11, 2)
#define WSYS_ETMSSCSR4(val)		MSR(val, 2, 1, c1, c12, 2)
#define WSYS_ETMSSCSR5(val)		MSR(val, 2, 1, c1, c13, 2)
#define WSYS_ETMSSCSR6(val)		MSR(val, 2, 1, c1, c14, 2)
#define WSYS_ETMSSCSR7(val)		MSR(val, 2, 1, c1, c15, 2)
#define WSYS_ETMSSPCICR0(val)		MSR(val, 2, 1, c1, c0, 3)
#define WSYS_ETMSSPCICR1(val)		MSR(val, 2, 1, c1, c1, 3)
#define WSYS_ETMSSPCICR2(val)		MSR(val, 2, 1, c1, c2, 3)
#define WSYS_ETMSSPCICR3(val)		MSR(val, 2, 1, c1, c3, 3)
#define WSYS_ETMSSPCICR4(val)		MSR(val, 2, 1, c1, c4, 3)
#define WSYS_ETMSSPCICR5(val)		MSR(val, 2, 1, c1, c5, 3)
#define WSYS_ETMSSPCICR6(val)		MSR(val, 2, 1, c1, c6, 3)
#define WSYS_ETMSSPCICR7(val)		MSR(val, 2, 1, c1, c7, 3)

#endif
