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

	.include "machine.inc"

	.import __so_initloading
	.import __so_init
	.import __so_fini

	.section D,data
	.global	__initloading
	.global	__callinit
	.global	__callfini

			.align	4
__initloading:	.data.l	__so_initloading
__callinit:	.data.l	__so_init
__callfini:	.data.l	__so_fini

	.end
