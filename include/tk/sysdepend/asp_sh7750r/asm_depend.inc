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
; *	@(#)asm_depend.inc (tk/ASP-SH7750R)
; *
; *	Assembler Macro for SH7750R
; */

	.aifdef __TK_ASM_DEPEND_H__
	.aelse
__TK_ASM_DEPEND_H__: .define " "

;/*
; * Stack state at Exception/Interrupt
; *
; *		+-------+
; *   new ISP ->| R0	|
; *		| SPC	|
; *		| SSR	|
; *		| MDR	|
; *		| USP	| Save USP only when switching to the system stack
; *		+-------+
; *   old ISP ->
; *
; * Macro
; *	INT_ENTER	* EIT entry
; *			Save to the interrupt stack as shown above.
; *	INT_RETURN	* EIT return
; *			Restore from the interrupt stack shown above
; *			and return (RTE).
; *
; *	If it is not the task independent part like extension SVC,
; *	need to move to the system stack before dispatch is enabled
; *	(interrupt enable) becauaes the interrupt stack can not be
; *	used continuously.
; *
; *	MOVE_ISP_SSP	Move from the interrupt stack to the system stack
; *	MOVE_SSP_ISP	Move from the system stack to the interrupt stack
; */

;/*
; * Exception/Interrupt entry common processing
; */
 .macro	INT_ENTER name

	mov	#3, r7		;// If CPL > 0, switch to system stack
	tst	r7, MDR
	bt	l_nochg_\name

	mov.l	SP, @-ISP	;// User stack save
	mov	SST, SP		;// Switch to system stack

  l_nochg_\name:
	mov.l	MDR, @-ISP	;// Operation mode save 
	shll16	MDR		;// Operation mode update 

	stc.l	ssr, @-ISP	;// SSR save
	stc.l	spc, @-ISP	;// SPC save 
	mov.l	r0,  @-ISP	;// R0 save

	stc	sr, r0		;// Interrupt disable SR.I = 15
	or	#(15<<4), r0
	ldc	r0, sr

 .endm

;/*
; * Exception/Interrupt return common processing
; *	Execute by setting SR.BL=1 and SR.RB=1
; */
 .macro	INT_RETURN name

	mov.l	@ISP+, r0	;// R0 restore
	ldc.l	@ISP+, spc	;// SPC restore
	ldc.l	@ISP+, ssr	;// SSR restore
	mov.l	@ISP+, MDR	;// MDR restore

	mov	#3, r7		;// If PPL > 0, switch to user stack
	tst	r7, MDR
	bt	l_norst_\name

	mov.l	@ISP+, SP	;// Switch to user stack

  l_norst_\name:
	rte
	nop

 .endm

;/*
; * TRAPA return common processing
; *	Execute by setting SR.BL=1 and SR.RB=1
; */
 .macro	CALL_RETURN name

	mov.l	@(2*4, ISP), r0	;// SSR
	ldc	r0, ssr		;// SSR restore

	mov.l	@ISP+, r0	;// R0 restore
	ldc.l	@ISP+, spc	;// SPC restore
	add	#4, ISP		;// skip SSR size(4byte)
	mov.l	@ISP+, MDR	;// MDR restore

	mov	#3, r7		;// If PPL > 0, switch to user stack
	tst	r7, MDR
	bt	l_norst_\name

	mov.l	@ISP+, SP	;// Switch to user stack

  l_norst_\name:
	rte
	nop

 .endm

;/*
; * ISP -> SSP move
; *	Execute by setting SR.BL=0 and SR.RB=1
; */
 .macro	MOVE_ISP_SSP name

	mov.l	@(3*4,ISP), r0	;// MDR
	tst	#3, r0		;// Check whether USP is saved
	bt	l_nousp1_\name	;// If PPL > 0, USP is saved

	mov.l	@(4*4,ISP), r0	;// USP
	mov.l	r0, @-SP
  l_nousp1_\name:
	mov.l	@(3*4,ISP), r0	;// MDR
	mov.l	r0, @-SP
	mov.l	@(2*4,ISP), r0	;// SSR
	mov.l	r0, @-SP
	mov.l	@(1*4,ISP), r0	;// SPC
	mov.l	r0, @-SP
	mov.l	@(0*4,ISP), r0	;// R0
	mov.l	r0, @-SP

	bt	l_nousp2_\name	;// Waste from interrupt stack
	add	#1*4, ISP
  l_nousp2_\name:
	add	#4*4, ISP

 .endm

;/*
; * SSP -> ISP move
; *	Execute by setting SR.BL=0 and SR.RB=1
; */
 .macro	MOVE_SSP_ISP name

	mov.l	@(3*4,SP), r0	;// MDR
	tst	#3, r0		;// Check whether USP is saved
	bt	l_nousp3_\name	;// If PPL > 0, USP is saved

	mov.l	@(4*4,SP), r0	;// USP
	mov.l	r0, @-ISP
  l_nousp3_\name:
	mov.l	@(3*4,SP), r0	;// MDR
	mov.l	r0, @-ISP
	mov.l	@(2*4,SP), r0	;// SSR
	mov.l	r0, @-ISP
	mov.l	@(1*4,SP), r0	;// SPC
	mov.l	r0, @-ISP
	mov.l	@(0*4,SP), r0	;// R0
	mov.l	r0, @-ISP

	bt	l_nousp4_\name	;// Waste from system stack
	add	#1*4, SP
  l_nousp4_\name:
	add	#4*4, SP

 .endm

;/*
; * Task exception handler entry
; *
; *		+---------------+
; *  USP(R15) ->| save SR	| Restore when returning from handler SR
; *	+4	| retadr	| Return address from handler
; *	+8	| texcd		| Exception code
; *		+---------------+
; *
; *	Note that the privilege instruction is not available
; *	because it may be executed by the user mode.
; *	Since stack is alignmented by 4 bytes, it cannot be accessed
; *	by double(8bytes).
; */
 .macro TEXHDR_ENTRY texhdr

	mov.l	r4, @-SP		;// R4 save
	mov.l	@(3*4, SP), r4		;// R4 = texcd
	mov.l	r0, @(3*4, SP)		;// R0 save

	stc.l	gbr,  @-SP		;// Save other register
	sts.l	pr,   @-SP
	sts.l	mach, @-SP
	sts.l	macl, @-SP
	mov.l	r1,   @-SP
	mov.l	r2,   @-SP
	mov.l	r3,   @-SP
	mov.l	r5,   @-SP
	mov.l	r6,   @-SP
	mov.l	r7,   @-SP

	sts.l	fpscr, @-SP		;// Save FPU register
	sts.l	fpul,  @-SP
	mov	#0, r0
	lds	r0, fpscr
	frchg				;// FPSCR.FR=1 SZ=0
	fmov.s	fr15, @-SP
	fmov.s	fr14, @-SP
	fmov.s	fr13, @-SP
	fmov.s	fr12, @-SP
	fmov.s	fr11, @-SP
	fmov.s	fr10, @-SP
	fmov.s	fr9,  @-SP
	fmov.s	fr8,  @-SP
	fmov.s	fr7,  @-SP
	fmov.s	fr6,  @-SP
	fmov.s	fr5,  @-SP
	fmov.s	fr4,  @-SP
	fmov.s	fr3,  @-SP
	fmov.s	fr2,  @-SP
	fmov.s	fr1,  @-SP
	fmov.s	fr0,  @-SP
	frchg				;// FPSCR.FR=0
	fmov.s	fr11, @-SP
	fmov.s	fr10, @-SP
	fmov.s	fr9,  @-SP
	fmov.s	fr8,  @-SP
	fmov.s	fr7,  @-SP
	fmov.s	fr6,  @-SP
	fmov.s	fr5,  @-SP
	fmov.s	fr4,  @-SP
	fmov.s	fr3,  @-SP
	fmov.s	fr2,  @-SP
	fmov.s	fr1,  @-SP
	fmov.s	fr0,  @-SP

	mov.l	l_fpscr_\texhdr, r0
	lds.l	@r0+, fpscr		;// FPSCR initialize
	mov.l	l_\texhdr, r0
	jsr	@r0			;// call texhdr(texcd)
	nop

	mov	#0, r0
	lds	r0, fpscr		;// FPSCR.FR=0 SZ=0
	fmov.s	@SP+, fr0
	fmov.s	@SP+, fr1
	fmov.s	@SP+, fr2
	fmov.s	@SP+, fr3
	fmov.s	@SP+, fr4
	fmov.s	@SP+, fr5
	fmov.s	@SP+, fr6
	fmov.s	@SP+, fr7
	fmov.s	@SP+, fr8
	fmov.s	@SP+, fr9
	fmov.s	@SP+, fr10
	fmov.s	@SP+, fr11
	frchg				;// FPSCR.FR=1
	fmov.s	@SP+, fr0
	fmov.s	@SP+, fr1
	fmov.s	@SP+, fr2
	fmov.s	@SP+, fr3
	fmov.s	@SP+, fr4
	fmov.s	@SP+, fr5
	fmov.s	@SP+, fr6
	fmov.s	@SP+, fr7
	fmov.s	@SP+, fr8
	fmov.s	@SP+, fr9
	fmov.s	@SP+, fr10
	fmov.s	@SP+, fr11
	fmov.s	@SP+, fr12
	fmov.s	@SP+, fr13
	fmov.s	@SP+, fr14
	fmov.s	@SP+, fr15
	lds.l	@SP+, fpul
	lds.l	@SP+, fpscr

	mov.l	@SP+, r7		;// Restore registers
	mov.l	@SP+, r6
	mov.l	@SP+, r5
	mov.l	@SP+, r3
	mov.l	@SP+, r2
	mov.l	@SP+, r1
	lds.l	@SP+, macl
	lds.l	@SP+, mach
	lds.l	@SP+, pr
	ldc.l	@SP+, gbr
	mov.l	@SP+, r4

	mov.l	@SP+, r0		;// SR restore
	trapa	#H'75			;// R0 -> SR

	mov.l	@SP+, r0		;// Return address from handler
	jmp	@r0
	mov.l	@SP+, r0		;// R0 restore

	.align	4
__fpscr_values_\texhdr:
	.data.l	H'40000			; PR=1
	.data.l	H'c0000			; PR=1, DN=1

			.align	4
  l_\texhdr:		.data.l	\texhdr	;// handler address
  l_fpscr_\texhdr:	.data.l	__fpscr_values_\texhdr + 4

 .endm

	.aendi ;/* __TK_ASM_DEPEND_H__ */
