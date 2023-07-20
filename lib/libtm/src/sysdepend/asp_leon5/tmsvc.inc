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
 *	@(#)tmsvc.inc (libtm/ASP-LEON5)
 *
 *	ASP-LEON5 definitions
 *
 *	* Used by assembler 
 */

#include "tk/sysdef.inc"

/*
 * T-Monitor service call 
 */
.macro _TMCALL func, fno
	.text
	.balign	4
	.globl	\func
	.type	\func, #function
\func:
	set	\fno, %g2
	ta	TRAP_MONITOR
	retl
	nop
.endm
