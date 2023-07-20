/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)asm_depend.inc (tk/ASP-LEON5)
 *
 *	Assembler Macro for ASP-LEON5
 */

#ifndef __TK_ASM_DEPEND_INC__
#define	__TK_ASM_DEPEND_INC__	1

#include "tk/sysdef.inc"
#include "cpuattr.inc"

/*
 * Task exception handler entry
 *
 *		+---------------+
 *	sp ->	| save area	| register window %i# %l#
 *		|     (17 word)	| hidden parameter
 *		+---------------+
 *	  +17*4	| texcd		|
 *		|      (6 word)	|
 *		+---------------+
 *	  +23*4	| psr		|
 *		| pc		|
 *		| npc		|
 *		|      (3 word)	|
 *		+---------------+
 *	  +26*4	| save area	| save registers %y %g# %f# %fsr
 *		|     (42 word)	|
 *		+---------------+
 *	fp ->	|		|
 */

 .macro TEXHDR_ENTRY texhdr
	mov	%y, %l0
	st	%l0,  [%sp + 26*4]
	st	%g1,  [%sp + 27*4]
	std	%g2,  [%sp + 28*4]
	std	%g4,  [%sp + 30*4]
	std	%g6,  [%sp + 32*4]
	std	%f0,  [%sp + 34*4]
	std	%f2,  [%sp + 36*4]
	std	%f4,  [%sp + 38*4]
	std	%f6,  [%sp + 40*4]
	std	%f8,  [%sp + 42*4]
	std	%f10, [%sp + 44*4]
	std	%f12, [%sp + 46*4]
	std	%f14, [%sp + 48*4]
	std	%f16, [%sp + 50*4]
	std	%f18, [%sp + 52*4]
	std	%f20, [%sp + 54*4]
	std	%f22, [%sp + 56*4]
	std	%f24, [%sp + 58*4]
	std	%f26, [%sp + 60*4]
	std	%f28, [%sp + 62*4]
	std	%f30, [%sp + 64*4]
	st	%fsr, [%sp + 66*4]

	set	FSR_INIT, %g1
	st	%g1,  [%sp + 67*4]
	ld	[%sp + 67*4], %fsr	// fsr initialize

	call	\texhdr			// call texhdr(texcd)
	 ld	[%sp + 17*4], %o0	// texcd

	ld	[%sp + 26*4], %l0
	mov	%l0, %y
	ld	[%sp + 27*4], %g1
	ldd	[%sp + 28*4], %g2
	ldd	[%sp + 30*4], %g4
	ldd	[%sp + 32*4], %g6
	ldd	[%sp + 34*4], %f0
	ldd	[%sp + 36*4], %f2
	ldd	[%sp + 38*4], %f4
	ldd	[%sp + 40*4], %f6
	ldd	[%sp + 42*4], %f8
	ldd	[%sp + 44*4], %f10
	ldd	[%sp + 46*4], %f12
	ldd	[%sp + 48*4], %f14
	ldd	[%sp + 50*4], %f16
	ldd	[%sp + 52*4], %f18
	ldd	[%sp + 54*4], %f20
	ldd	[%sp + 56*4], %f22
	ldd	[%sp + 58*4], %f24
	ldd	[%sp + 60*4], %f26
	ldd	[%sp + 62*4], %f28
	ldd	[%sp + 64*4], %f30
	ld	[%sp + 66*4], %fsr

	ta	TRAP_RETTEX
 .endm

#endif	/* __TK_ASM_DEPEND_INC__ */
