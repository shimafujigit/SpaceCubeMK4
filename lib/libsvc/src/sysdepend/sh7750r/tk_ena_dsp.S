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
; *	T-Kernel SVC interface library (sh7750r) [HEW]
; *
; *	   created from syscall.h
; */

	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/svc/tkfncd.inc"

	.aifdef	TFN_ENA_DSP

	.section P,code
	.align	2
	.global	_tk_ena_dsp
_tk_ena_dsp:
	mov.l	fno, r0
	trapa	#TRAP_SVC
	rts
	nop

	.align	4
fno:	.data.l	TFN_ENA_DSP

	.aendi

	.end

