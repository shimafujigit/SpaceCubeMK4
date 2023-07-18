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
; *----------------------------------------------------------------------
; */

;
;	tlb.S
;

	.include "machine.inc"
	.include "tk/sysdef.inc"

  .aif \&USE_MEM_PROTECT NE 0

	.global	_tlb_miss_hdr
	.global	_purge_tlb_page

;-----------------------------------------------------------------------------

;;
;; TLB miss exception handler
;;
	.section P,code
	.align	2

_tlb_miss_hdr:
	mov.l	r0, @-ISP
	mov.l	r8, @-ISP

	mov.l	T_PTEH, r7		; PTEH
	mov.l	@r7, r8
	mov	#-12, r0		; 4KB page
	shld	r0, r8			; r8 : VPN

	mov.l	T_TTB, r7		; TTB
	mov.l	@r7, r7			; P_TAB
	mov.l	@(1*4, r7), r0		; VPN-HI
	cmp/hs	r0, r8			; VPN >= VPN-HI
	bt	l_notpresent

	mov.l	@r7, r0			; VPN-LO
	cmp/hi	r8, r0			; VPN < VPN-LO
	bt	l_notpresent

	add	#8, r7
	sub	r0, r7
	mov	r8, r0
	mov.b	@(r0, r7), r0		; PDE lo byte
	cmp/pl	r0
	bf	l_notpresent

	mov.l	T_PPNMSK, r7		; PPN = PPN
	and	r7, r8			; Clear MSB 3 bits for PPN
	shll2	r8
	shll2	r8
	add	#1, r8			; set V bit
	shll8	r8
	or	r8, r0			; PTEL entry

	mov.l	T_PTEL, r7
	mov.l	r0, @r7
	ldtlb				; Load TLB
	nop

	mov.l	@ISP+, r8
	mov.l	@ISP+, r0
	rte
	nop

  l_notpresent:
	stc	vbr, r7			; VBR
	mov	#1, r0
	shll8	r0
	add	r0, r7			; VBR + H'100 : Exception entry 
	mov.l	@ISP+, r8
	mov.l	@ISP+, r0
	jmp	@r7			; Jump to exeption entry 
	nop

;-----------------------------------------------------------------------------
;
; Purge Page TLB
;

	.section P,code
	.align	2

_purge_tlb_page:
	mov.l	T_PTEH, r1		; Save PTEH
	mov.l	@r1, r2	

	mov.l	r4, @r1			; laddr -> PTEH

	mov.l	T_GOTO_P2, r0
	braf	r0			; Jump to P2
	nop

	mov.l	T_UTLB_ADR_A, r0
	mov.l	r4, @r0			; Purge TLB (V=0 D=0)

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	mov.l	r2, @r1			; Restore PTEH

	rts
	nop

		.align	4

  T_TTB:	.data.l	TTB
  T_PTEH:	.data.l	PTEH
  T_PTEL:	.data.l	PTEL
  T_MMUCR:	.data.l	MMUCR
  T_UTLB_ADR_A:	.data.l	H'F6000080
  T_PPNMSK:	.data.l	H'1FFFF		; 4KB Page
  T_GOTO_P2:	.data.l	H'20000000

;-----------------------------------------------------------------------------

  .aendi	; USE_MEM_PROTECT

	.end


