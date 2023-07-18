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
; *    Changed by UC Technology at 2015/05/15.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	@(#)disint.S (libtk/SH7750R)
; *
; *	Enable/disable external interrupt.
; */

	.include "machine.inc"
	.include "tk/sysdef.inc"

;/*
; * Disable external interrupt 
; * UINT disint( void )
; *	SR.I = 15; the return value is SR prior to the change.
; */
	.section P,code
	.align	2
	.global	_disint
_disint:
	stc	sr, r0
	stc	sr, r1
	or	#(15<<4), r0
	ldc	r0, sr
	rts
	mov	r1, r0

;/*
; * Enable external interrupt (restore to original state)
; * UINT enaint( UINT intsts )
; *	SR.I = msk.I. Only  SR.I changes; all else remains unchanged.
; *	The return value is SR prior to the change.
; */
	.section P,code
	.align	2
	.global	_enaint
_enaint:
	stc	sr, r0
	stc	sr, r1
	xor	r4, r0
	and	#(15<<4), r0
	xor	r1, r0
	ldc	r0, sr
	rts
	mov	r1, r0

	.end
