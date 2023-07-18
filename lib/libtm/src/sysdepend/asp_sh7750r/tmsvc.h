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
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)tmsvc.h (libtm/SH7750R)
 *
 *	ASP-SH7750R definitions
 *
 *	* Used by assembler 
 */

#include <tk/sysdef.h>

/*
 * T-Monitor service call 
 */
.macro _TMCALL func, fno
	.text
	.balign	2
	.globl	Csym(\func)
	.type	Csym(\func), @function
Csym(\func):
  .if \fno < 128
	mov	#\fno, r0
	trapa	#TRAP_MONITOR
	rts
	nop
  .else
	mov.l	fno_\func, r0
	trapa	#TRAP_MONITOR
	rts
	nop
		.balign	4
    fno_\func:	.long	\fno
  .endif
.endm

#define TMCALL(func, fno)	_TMCALL func, fno
