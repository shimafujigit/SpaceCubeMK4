;/*
; *----------------------------------------------------------------------
; *    T-Kernel 2.0 Software Package
; *
; *    Copyright 2011 by Ken Sakamura.
; *    This software is distributed under the T-License 2.0.
; *----------------------------------------------------------------------
; *
; *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
; *
; *----------------------------------------------------------------------
; *    Changes: Adapted to the ASP-SH7750R Board.
; *    Changed by UC Technology at 2012/12/25.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	T-Kernel extended SVC interface library (sh7750r) [HEW]
; *
; *	   created from <sys/segment.h>
; */

	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/svc/fnsegment.inc"

	.aifdef	SEG_READMEMSPACE_FN

	.section P,code
	.align	2
	.global	_ReadMemSpace
_ReadMemSpace:
	mov.l	r7, @-r15
	mov.l	r6, @-r15
	mov.l	r5, @-r15
	mov.l	r4, @-r15
	mov.l	fno, r0
	mov	r15, r4
	trapa	#TRAP_SVC
	rts
	add	#4*4, r15

	.align	4
fno:	.data.l	SEG_READMEMSPACE_FN

	.aendi

	.end

