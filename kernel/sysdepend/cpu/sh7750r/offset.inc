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
; *	offset.inc (SH7750R)
; *	TCB Offset Definition
; */

	.aifdef _OFFSET_
	.aelse
_OFFSET_: .define " "

;/* Adjust offset of TCB member variables in offset.h for cpu_support.S */
	.include "config.inc"

;/*	TCB.wrdvno	*/
	.aifdef NUM_PORID
TCBSZ_POR	.equ	4	;/* = sizeof(RNO) */
	.aelse
TCBSZ_POR	.equ	0
	.aendi

;/*	TCB.mtxlist	*/
	.aifdef NUM_MTXID
TCBSZ_MTX	.equ	4	;/* = sizeof(MTXCB*) */
	.aelse
TCBSZ_MTX	.equ	0
	.aendi

;/*	TCB.winfo.xxx	*/
	.aifdef NUM_PORID
TCBSZ_WINFO	.equ	16
	.aelse
	.aifdef NUM_FLGID
TCBSZ_WINFO	.equ	12
	.aelse
	.aifdef NUM_MBFID
TCBSZ_WINFO	.equ	8
	.aelse
	.aifdef NUM_MPLID
TCBSZ_WINFO	.equ	8
	.aelse
	.aifdef NUM_SEMID
TCBSZ_WINFO	.equ	4
	.aelse
	.aifdef NUM_MBXID
TCBSZ_WINFO	.equ	4
	.aelse
	.aifdef NUM_MPFID
TCBSZ_WINFO	.equ	4
	.aelse
TCBSZ_WINFO	.equ	0
	.aendi
	.aendi
	.aendi
	.aendi
	.aendi
	.aendi
	.aendi

;/*----------------------------------------------------------------------*/
;/*	machine dependent data						*/
;/*----------------------------------------------------------------------*/
TCB_winfo		.equ	68		;/* tskque - wercd		*/
TCB_wtmeb		.equ	(((TCB_winfo+TCBSZ_WINFO)+3)&H'FFFFFFFC)
TCBsz_wtmeb2istack	.equ	28+TCBSZ_MTX+TCBSZ_POR+76
						;/* wtmeb - istack		*/
TCBSZ_GP		.equ	0		;/* No global pointer support	*/

;/*----------------------------------------------------------------------*/
;/*	offset data in TCB						*/
;/*----------------------------------------------------------------------*/
TCB_isstack	.equ	TCB_wtmeb+TCBsz_wtmeb2istack
TCB_tskctxb	.equ	(((TCB_isstack+4+TCBSZ_GP)+3)&H'FFFFFFFC)

TCB_tskid:	.equ	8
TCB_tskatr:	.equ	16
TCB_state:	.equ	43
TCB_reqdct:	.equ	36
CTXB_lsid:	.equ	8
TA_FPU:		.equ	4096
_TA_FPU_:	.assigna	4096
CTXB_ssp:	.equ	0
CTXB_uatb:	.equ	4
TA_COP0:	.equ	4096
COP0_REGSZ:	.equ	136

	.aendi ;/* _OFFSET_ */
