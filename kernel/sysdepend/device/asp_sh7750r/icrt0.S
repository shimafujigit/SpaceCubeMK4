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
; *	icrt0.S (SH7750R)
; *	System Startup
; */

	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/sysinfo.inc"

START:	.define		"_start"

	.import		_main
	.import		_ROM_startup
	.import		_DTBL
	.import		_BTBL

;	/* Low level memory manager information */
	.export _lowmem_top
	.export _lowmem_limit
	.section B,data
_lowmem_top:	.res.l	1		;// Head of area (Low address)
_lowmem_limit:	.res.l	1		;// End of area (High address)

_monitor_stacktop:	.res.l	1	;// Monitor stack pointer for re-startup

	.section P_start,code
	.align	2
	.global	START
START:
;	/* Use the stack set by the boot loader */

	mov.l	INIT_SR, r0		;// SR initial setting
	ldc	r0, sr

;	/* SR.MD=1, SR.BL=0, SR.RB=0, SR.I=15 */

	mov.l	CACHE_ON, r0		;// Cache on
	jsr	@r0
	nop

	mov.l	DATA_ORG, r1		;// Initialization of 'data' area (ROM startup)
	mov.l	@r1, r1
	mov.l	DATA_START, r2
	mov.l	@r2, r2
	mov.l	EDATA, r3
	mov.l	@r3, r3
	cmp/eq	r2, r3			;// If r2 != r3, start ROM
	bt/s	nocopy_data
	movt	r8			;// If start ROM, r8 = 0
  copy_data:
	cmp/hi	r2, r3
	bf	nocopy_data
	mov.l	@r1+, r0
	mov.l	r0, @r2
	bra	copy_data
	add	#4, r2
  nocopy_data:

	mov	#0, r0			;// Clear 'bss' area
	mov.l	BSS_TOP, r1
	mov.l	@r1, r1
	mov.l	BSS_END, r2
	mov.l	@r2, r2
	cmp/hi	r1, r2
	bf	nobss
	add	#4, r1
  clrbss:
	cmp/hi	r1, r2
	bt/s	clrbss
	mov.l	r0, @-r2
  nobss:

	mov.l	MON_STACKTOP, r0	;// Monitor stack pointer for re-startup
	mov.l	r15, @r0		;// Save

	mov.l	RAM_TOP, r1		;// Low level memory manager initial setting
	mov.l	@r1, r1
	mov.l	BSS_END, r0
	mov.l	@r0, r0
	cmp/hi	r0, r1			;// _end or RAM_TOP
	bt	l1			;// Either of High addresses
	mov	r0, r1
  l1:
	mov.l	RAM_END, r3
	mov.l	@r3, r3
	mov.l	LOWMEM_TOP, r2
	mov.l	LOWMEM_LIM, r4
	mov.l	r1, @r2			;// lowmem_top   = RAM_TOP or _end
	mov.l	r3, @r4			;// lowmem_limit = RAM_END

;;;;	tst	r8, r8			;// If r8 == 0, start Rom
;;;;	bf	l2
	mov.l	ROM_STARTUP, r0		;// Initialization at ROM startup
	jsr	@r0
	nop
  l2:

	mov.l	MAIN, r0		;// System start up
	jsr	@r0
	nop

  l_end:				;// Not suppose to return from 'main,'
	bra	l_end			;// but, just in case, prepare for out of control.
	nop

		.align	4
  INIT_SR:	.data.l	SR_MD | (15<<4)
  DATA_ORG:	.data.l	_DTBL
  DATA_START:	.data.l	_DTBL + 8
  EDATA:	.data.l	_DTBL + 12
  BSS_TOP:	.data.l	_BTBL
  BSS_END:	.data.l	_BTBL + 4
  LOWMEM_TOP:	.data.l	_lowmem_top
  LOWMEM_LIM:	.data.l	_lowmem_limit
  ROM_STARTUP:	.data.l	_ROM_startup
  MAIN:		.data.l	_main
  MON_STACKTOP:	.data.l	_monitor_stacktop

;	/* System shared information */
  RAM_TOP:	.data.l	SCINFO_RAMTOP
  RAM_END:	.data.l	SCINFO_RAMEND

;/*
; * When accessing the cache control register, the program itself
; * must be in the non-cache area. Therefore convert to the address
; * of the shadow image in P2 area and call.
; */
CACHE_ON:	.data.l	cache_on + H'20000000

;/* ------------------------------------------------------------------------ */
;/*
; *	Cache control
; */

CACHE_LINESZ:	.equ	H'20		;/* Cache line size */

;/*
; * Cache on
; */
	.section P,code
	.align	2
cache_on:
	sts.l	pr, @-SP

	bsr	cache_off		;// Disable cache once.
	nop

	mov.l	C_CCR, r0
	mov.l	C_CACHE_ON, r1
	mov.l	r1, @r0			;// Cache on (Writeback is enabled)

	nop;nop;nop;nop;nop;nop;nop

	lds.l	@SP+, pr
	rts
	nop

;/*
; * Cache off
; */
	.section P,code
	.align	2
cache_off:
	mov.l	C_CCR, r1		;// Disable cache and
	mov.l	C_CCR_MASK, r0		;// invalidate instruction cache

	mov.l	@r1, r2
	and	r0, r2
	mov.l	C_CACHE_OFF, r0
	or	r0, r2
	mov.l	r2, @r1

	mov	#0, r0			;// Flush cache
	mov.l	C_CACHE_TAGTOP, r1	;// (Writeback and disable)
	mov.l	C_CACHE_TAGEND, r2
  l_flush:
	mov.l	r0, @r1
	add	#CACHE_LINESZ, r1
	cmp/hi	r1, r2
	bt	l_flush

	rts
	nop

			.align	4
  C_CCR:		.data.l	CCR
  C_CACHE_ON:		.data.l	CCR_OCE|CCR_CB|CCR_OCI|CCR_ICE|CCR_ICI|CCR_EMODE
  C_CACHE_OFF:		.data.l	CCR_ICI			;// Invalidate instruction cache

  C_CCR_MASK:		.data.l	CCR_ORA|CCR_OIX|CCR_IIX|CCR_EMODE
  C_CACHE_TAGTOP:	.data.l	H'f4000000	;// Data cache only

  C_CACHE_TAGEND:	.data.l	H'f4008000

	.end
