;/*
; *----------------------------------------------------------------------
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R
; *
; *    Copyright (c) 2013 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; *
; *    Version:   1.00.00
; *    Released by UC Technology (http://www.uctec.com/) at 2013/01/28.
; *
; *    Changed by UC Technology at 2015/05/15.
; *----------------------------------------------------------------------
; */

;
; Minimum T-Monitor for ASP-SH7750R [HEW]
;

	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "tk/asm.inc"

	.global _exception_hdr
	.global _trapa_hdr
	.global _tlbmiss_hdr
	.global _interrupt_hdr
	.global _default_hdr
	.global _monitor_svc

	.global _reset_proc
	.global	_ent_monitor
	.global	_get_spc

;
; EIT Vector tables
;
VECTBL:		.equ	H'84001000
DEFAULTHDR:	.equ	H'84001200
TLBMISSHDR:	.equ	H'84001204

ISP_VAL:	.equ	H'84001500
SSP_VAL:	.equ	H'84001000
MSP_VAL:	.equ	H'84001000
VBR_VAL:	.equ	H'84000000

SR_FDv:		.equ	H'00000000	;// SR FD value = No Disable FPU

;--------------------------------------------------------------------
; Reset handler (0xA0000000)
;--------------------------------------------------------------------
	.section RESET,code
	.align	4
_reset_hdr:
	mov.l	P1_RESET, r0
	jmp	@r0		; P2 -> P1 space
	nop

reset_start:

	mov.l	SR_BL1RB1, r0	; init SR (BL=1.RB=1)
	ldc	r0, sr

	mov.l	INIT_ISP, r6	; init ISP R6 Bank 1
	mov.l	INIT_SSP, r3	; init SST R3 Bank 1
	mov.l	INIT_MSP, SP	; init Monitor Stack

	mov.l	INIT_VBR, r0
	ldc	r0, vbr		; VBR addr set
	ldc	r0, dbr		; DBR addr set

	mov.l	SR_BL0RB0, r0	; init SR (BL=0.RB=0)
	ldc	r0, sr

	; initialize basic hardware if required.

	mov.l	RESET_PROC, r0	; Reset process
	jsr	@r0		; return r0 = Kernel Start address
	nop

	jsr	@r0		; Jump to Kernel Entry
	nop

		.align 4

P1_RESET:	.data.l	reset_start
INIT_ISP:	.data.l	ISP_VAL
INIT_SSP:	.data.l	SSP_VAL
INIT_MSP:	.data.l	MSP_VAL
INIT_VBR:	.data.l VBR_VAL
RESET_PROC:	.data.l	_reset_proc
SR_BL1RB1:	.data.l	SR_MD | SR_FDv | (15<<4) | SR_RB | SR_BL
SR_BL0RB0:	.data.l	SR_MD | SR_FDv | (15<<4)

;--------------------------------------------------------------------
; Exception handler (VBR + 0x100)
;--------------------------------------------------------------------
	.org	H'100

_exception_hdr:

	INT_ENTER	_exception

	mov.l	L1_EXPEVT, r0	; Exception code
	mov.l	@r0, r0
	mov	#-3, r7
	shad	r7, r0		; EXPEVT / H'20 * 4

	mov.l	L1_VECTBL, r7	; Handler address
	mov.l	@(r0, r7), r7
	tst	r7, r7		; Undefined if NULL
	bf	l_exc1

	mov.l	L1_DEFAULT, r7	; Default handler
	mov.l	@r7, r7

l_exc1:
	jmp	@r7		; Jump to exception hdr
	nop			; r0 = Vector index + kind(0)

;------------------------------------
; TRAPA handler
;------------------------------------
_trapa_hdr:

	mov.l	L1_TRA, r0	; TRAPA index
	mov.l	@r0, r0

	mov.l	L1_MAXTRA, r7	; Index range check
	cmp/hs	r7, r0		; Useful range TRAPA 0〜0x7f
	bt	l_trap1

	mov.l	L1_VECTBL, r7	; Handler address
	mov.l	@(r0, r7), r7
	tst	r7, r7		; Undefined if NULL
	bf	l_trap2

l_trap1:
	mov.l	L1_DEFAULT, r7	; Default handler
	mov.l	@r7, r7

l_trap2:
	jmp	@r7		; Jump to TRAPA handler
	add	#1, r0		; r0 = Vector index + kind(1)

		.align	4

L1_MAXTRA:	.data.l	H'200	; TRAPA 0x80 is max
L1_EXPEVT:	.data.l	EXPEVT
L1_VECTBL:	.data.l	VECTBL
L1_DEFAULT:	.data.l	DEFAULTHDR
L1_TRA: 	.data.l	TRA

;------------------------------------
; Default  handler (r0: Vector index + kind)
;------------------------------------
_default_hdr:
	ldc	r0, r4_bank	; r0 -> r4_bank0
	mov	#127, r7
	ldc	r7, r0_bank	; 254 -> r0_bank0
	bra	_monitor_svc	; Jump to monitor svc entry
	nop

		.align	4

L2_MONITOR:	.data.l	_ent_monitor

	.align	2
_get_spc:
	stc	spc, r0
	rts
	nop

;--------------------------------------------------------------------
; TLB miss exception handler (VBR + 0x400)
;--------------------------------------------------------------------
	.org	H'400

_tlbmiss_hdr:
	mov.l	L3_TLBMISS, r7	; TLB miss exception handler
	mov.l	@r7, r7
	jmp	@r7		; Jump to TLB miss exception handler
	nop

		.align	4

L3_TLBMISS:	.data.l	TLBMISSHDR

;------------------------------------
; Monitor SVC handler
;------------------------------------

_monitor_svc:

	mov.l	L4_BL1RB1, r0	; BL=1, RB=1
	ldc	r0, sr

	mov.l	SP, r7		; save SP -> R7_BANK1

	mov.l	L4_MSP, SP	; montior stack

	mov.l	L4_BL0RB0, r0	; BL=0, RB=0
	ldc	r0, sr

	sts.l	pr, @-SP	; save pr
	mov.l	r14, @-SP	; save R14
	mov.l	r13, @-SP	; save R13
	mov.l	r12, @-SP	; save R12
	mov.l	r11, @-SP	; save R11
	mov.l	r10, @-SP	; save R10
	mov.l	r9, @-SP	; save R9
	mov.l	r8, @-SP	; save R8
	mov.l	r7, @-SP	; save R7_BANK0
	mov.l	r6, @-SP	; save R6_BANK0
	mov.l	r5, @-SP	; save R5_BANK0
	mov.l	r4, @-SP	; save R4_BANK0
	mov	SP, r5		; paramter pointer r4,r5,r6,r7
	mov.l	r3, @-SP	; save R3_BANK0
	mov.l	r2, @-SP	; save R2_BANK0
	mov.l	r1, @-SP	; save R1_BANK0

	mov	r0, r4		; ent_monitor(fn code:r4, *par:r5)
	mov.l	TM_ENTRY, r1
	jsr	@r1
	nop			; r0: return value

	mov.l	@SP+, r1	; restore R1_BANK0
	mov.l	@SP+, r2	; restore R2_BANK0
	mov.l	@SP+, r3	; restore R3_BANK0
	mov.l	@SP+, r4	; restore R4_BANK0
	mov.l	@SP+, r5	; restore R5_BANK0
	mov.l	@SP+, r6	; restore R6_BANK0
	mov.l	@SP+, r7	; restore R7_BANK0
	mov.l	@SP+, r8	; restore R8
	mov.l	@SP+, r9	; restore R9
	mov.l	@SP+, r10	; restore R10
	mov.l	@SP+, r11	; restore R11
	mov.l	@SP+, r12	; restore R12
	mov.l	@SP+, r13	; restore R13
	mov.l	@SP+, r14	; restore R14
	lds.l	@SP+, pr	; restore pr

	mov.l	L4_BL1RB1, r15	; BL=1, RB=1
	ldc	r15, sr

	mov	r7, SP		; restore SP

	INT_RETURN	monitor_svc

		.align	4

L4_MSP:		.data.l	MSP_VAL
L4_BL1RB1:	.data.l	SR_MD | SR_FDv | (15<<4) | SR_RB | SR_BL
L4_BL0RB0:	.data.l	SR_MD | SR_FDv | (15<<4)
TM_ENTRY:	.data.l	_ent_monitor

;--------------------------------------------------------------------
; Interrupt handler (VBR + 0x600)
;--------------------------------------------------------------------
	.org	H'600

_interrupt_hdr:

	INT_ENTER	_interrupt

	mov.l	L5_INTEVT, r0	; Interrupt code
	mov.l	@r0, r0
	mov	#-3, r7
	shad	r7, r0		; INTEVT / H'20 * 4

	mov.l	L5_VECTBL, r7	; Handler address
	mov.l	@(r0, r7), r7
	tst	r7, r7		; Undefined if NULL
	bf	l_int1

	mov.l	L5_DEFAULT, r7	; Default handler
	mov.l	@r7, r7

l_int1:
	jmp	@r7		; Jump to interrupt handler
	add	#2, r0		; r0 = Vector index + kind(2)

		.align 4

L5_INTEVT:	.data.l	INTEVT
L5_VECTBL:	.data.l	VECTBL
L5_DEFAULT:	.data.l	DEFAULTHDR

;------------------------------------------------------------------

	.end
