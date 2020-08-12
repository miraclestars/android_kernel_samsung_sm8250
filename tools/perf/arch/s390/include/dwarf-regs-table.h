/* SPDX-License-Identifier: GPL-2.0 */
#ifndef S390_DWARF_REGS_TABLE_H
#define S390_DWARF_REGS_TABLE_H

#define REG_DWARFNUM_NAME(reg, idx)	[idx] = "%" #reg

/*
 * For reference, see DWARF register mapping:
 * http://refspecs.linuxfoundation.org/ELF/zSeries/lzsabi0_s390/x1542.html
 */
static const char * const s390_dwarf_regs[] = {
	"%r0", "%r1",  "%r2",  "%r3",  "%r4",  "%r5",  "%r6",  "%r7",
	"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
	REG_DWARFNUM_NAME(f0, 16),
	REG_DWARFNUM_NAME(f1, 20),
	REG_DWARFNUM_NAME(f2, 17),
	REG_DWARFNUM_NAME(f3, 21),
	REG_DWARFNUM_NAME(f4, 18),
	REG_DWARFNUM_NAME(f5, 22),
	REG_DWARFNUM_NAME(f6, 19),
	REG_DWARFNUM_NAME(f7, 23),
	REG_DWARFNUM_NAME(f8, 24),
	REG_DWARFNUM_NAME(f9, 28),
	REG_DWARFNUM_NAME(f10, 25),
	REG_DWARFNUM_NAME(f11, 29),
	REG_DWARFNUM_NAME(f12, 26),
	REG_DWARFNUM_NAME(f13, 30),
	REG_DWARFNUM_NAME(f14, 27),
	REG_DWARFNUM_NAME(f15, 31),
	REG_DWARFNUM_NAME(c0, 32),
	REG_DWARFNUM_NAME(c1, 33),
	REG_DWARFNUM_NAME(c2, 34),
	REG_DWARFNUM_NAME(c3, 35),
	REG_DWARFNUM_NAME(c4, 36),
	REG_DWARFNUM_NAME(c5, 37),
	REG_DWARFNUM_NAME(c6, 38),
	REG_DWARFNUM_NAME(c7, 39),
	REG_DWARFNUM_NAME(c8, 40),
	REG_DWARFNUM_NAME(c9, 41),
	REG_DWARFNUM_NAME(c10, 42),
	REG_DWARFNUM_NAME(c11, 43),
	REG_DWARFNUM_NAME(c12, 44),
	REG_DWARFNUM_NAME(c13, 45),
	REG_DWARFNUM_NAME(c14, 46),
	REG_DWARFNUM_NAME(c15, 47),
	REG_DWARFNUM_NAME(a0, 48),
	REG_DWARFNUM_NAME(a1, 49),
	REG_DWARFNUM_NAME(a2, 50),
	REG_DWARFNUM_NAME(a3, 51),
	REG_DWARFNUM_NAME(a4, 52),
	REG_DWARFNUM_NAME(a5, 53),
	REG_DWARFNUM_NAME(a6, 54),
	REG_DWARFNUM_NAME(a7, 55),
	REG_DWARFNUM_NAME(a8, 56),
	REG_DWARFNUM_NAME(a9, 57),
	REG_DWARFNUM_NAME(a10, 58),
	REG_DWARFNUM_NAME(a11, 59),
	REG_DWARFNUM_NAME(a12, 60),
	REG_DWARFNUM_NAME(a13, 61),
	REG_DWARFNUM_NAME(a14, 62),
	REG_DWARFNUM_NAME(a15, 63),
	REG_DWARFNUM_NAME(pswm, 64),
	REG_DWARFNUM_NAME(pswa, 65),
};

#ifdef DEFINE_DWARF_REGSTR_TABLE
/* This is included in perf/util/dwarf-regs.c */

#define s390_regstr_tbl s390_dwarf_regs

#endif	/* DEFINE_DWARF_REGSTR_TABLE */
#endif	/* S390_DWARF_REGS_TABLE_H */
