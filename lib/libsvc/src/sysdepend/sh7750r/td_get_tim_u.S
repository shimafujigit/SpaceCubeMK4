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
; *	T-Kernel/DS SVC interface library (sh7750r) [HEW]
; *
; *	   created from dbgspt.h
; */

	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/svc/tdfncd.inc"

	.aifdef	TDFN_GET_TIM_U

	.section P,code
	.align	2
	.global	_td_get_tim_u
_td_get_tim_u:
	mov.l	fno, r0
	trapa	#TRAP_DEBUG
	rts
	nop

	.align	4
fno:	.data.l	TDFN_GET_TIM_U

	.aendi

	.end

