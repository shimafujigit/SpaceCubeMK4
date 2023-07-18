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
; *    Changed by UC Technology at 2012/12/20.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

	.import _GLOBAL_OFFSET_TABLE_

	.section	P_init,code,align=2
	.global		__init
__init:
	mov.l	r12, @-r15
	mova	I_GOT, r0
	mov.l	I_GOT, r12
	sts.l	pr, @-r15
	bra	I_TOP
	add	r0, r12
I_GOT:	.data.l	_GLOBAL_OFFSET_TABLE_
I_TOP:

	.section	P_fini,code,align=2
	.global		__fini
__fini:
	mov.l	r12, @-r15
	mova	F_GOT, r0
	mov.l	F_GOT, r12
	sts.l	pr, @-r15
	bra	F_TOP
	add	r0, r12
F_GOT:	.data.l	_GLOBAL_OFFSET_TABLE_
F_TOP:

	.end
