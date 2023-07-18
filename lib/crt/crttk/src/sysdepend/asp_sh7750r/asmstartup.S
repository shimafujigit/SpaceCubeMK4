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
; *	@(#)asmstartup.S (crttk)
; *
; *	Driver/manager startup 
; */

	.include "machine.inc"
	.include "tk/asm.inc"

	.import __P_startup
	.import _DTBL
	.import _BTBL

	.section P,code
	.align	4
	.global	_C_startup
_C_startup:
	cmp/pz	r4
	bf	l_noinit	;// Initialize is performed only when ac >= 0

	mov.l	DATA_ORG, r1	;// When data area does not need to be moved,
	mov.l	@r1, r1
	mov.l	DATA_START, r2	;// OS loads
	mov.l	@r2, r2
        mov.l   DATA_END, r3
	mov.l	@r3, r3
	cmp/eq	r2, r3
	bt	l_noinit

;	bra     l1		;// initialize data area
;	nop
  copydata:
	mov.l	@r1+, r0
	mov.l	r0, @r2
	add	#4, r2
  l1:
	cmp/hi	r2, r3
	bt	copydata

	mov.l	BSS_START, r2	;// clear bss area
	mov.l	@r2, r2
	mov.l	BSS_END, r3
	mov.l	@r3, r3
	bra	l2
	mov	#0, r0
  clrbss:
	mov.l	r0, @-r3
  l2:
	cmp/hi	r2, r3
	bt	clrbss

  l_noinit:
	mov.l	L_Startup, r0
	jmp	@r0
	nop

		.align	4
DATA_ORG:	.data.l	_DTBL
DATA_START:	.data.l	_DTBL + 8
DATA_END:	.data.l	_DTBL + 12
BSS_START:	.data.l	_BTBL
BSS_END:	.data.l	_BTBL + 4
L_Startup:	.data.l	__P_startup

	.end
