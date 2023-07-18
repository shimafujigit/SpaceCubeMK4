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

;/*
; *	@(#)crt1f.S (SH)
; *
; *	C startup 
; *
; *	For static links 
; *	shared library loader is not linked.
; */

	.include "machine.inc"

	.import _init
	.import _fini

	.section D,data
	.global	__initloading
	.global	__callinit
	.global	__callfini

			.align	4
__initloading:	.data.l	initloading
__callinit:	.data.l	_init
__callfini:	.data.l	_fini

	.section P,code
	.align	2
initloading:
;	/* No action */
	rts
	nop

	.end
