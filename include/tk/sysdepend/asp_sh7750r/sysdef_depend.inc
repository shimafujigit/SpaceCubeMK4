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
; *	@(#)sysdef_depend.inc (tk/ASP-SH7750R)
; *
; *	Definition about SH7750R
; *
; *	Included also from assembler program.
; */

	.aifdef __TK_SYSDEF_DEPEND_H__
	.aelse
__TK_SYSDEF_DEPEND_H__: .define " "

;/*
; * Use specify register
; */
SP:	.define		"r15"	;/* Stack pointer */

;/*
; * BANK1 register
; *	R0	For work				With task
; *	R1	Reservation				With task
; *	R2 MDR	Operation mode register		  	With task
; *	R3 SST	System stack top		  	With task
; *	R4	For work				Task independent
; *	R5	Reservation				Task independent
; *	R6 ISP	EIT stack pointer	  		Task independent
; *	R7	For EIT work				Task independent
; *		(Available only at SR.BL=1)
; */
MDR:	.define		"r2"	;/* Operation mode register */
SST:	.define		"r3"	;/* System stack top */
ISP:	.define		"r6"	;/* EIT stack pointer */

;/*
; * MDR register
; */
MDR_DCT:	.equ	H'80000000	;/* Delayed context trap request */

;/*
; * SR register
; */
SR_MD:		.equ	H'40000000	;/* Privilege mode */
SR_RB:		.equ	H'20000000	;/* Register bank */
SR_BL:		.equ	H'10000000	;/* Interrupt block */
;;; IMPLICIT FPU PROBLEM ;;;
SR_FD:		.equ	H'00008000	;/* FPU disable (SH4) */
;;;SR_FD:	.equ	H'00000000	;/* FPU disable (SH4) */

;/*
; * FPU status/control register (SH4)
; */
FPSCR_FR:	.equ	H'00200000	;/* Register bank */
FPSCR_SZ:	.equ	H'00100000	;/* Transfer size mode */
FPSCR_PR:	.equ	H'00080000	;/* Precision mode */
FPSCR_DN:	.equ	H'00040000	;/* Denormalization mode */

					;/* FPU exception */
FPE_I:		.equ	H'01		;/*   Inexact */
FPE_U:		.equ	H'02		;/*   Underflow */
FPE_O:		.equ	H'04		;/*   Overflow */
FPE_Z:		.equ	H'08		;/*   Division by zero */
FPE_V:		.equ	H'10		;/*   Invalid */
FPE_E:		.equ	H'20		;/*   FPU error */

;/*
; * EIT register
; */
TRA:		.equ	H'ff000020	;/* W:TRAPA exception */
EXPEVT:		.equ	H'ff000024	;/* W:Exception event */
INTEVT:		.equ	H'ff000028	;/* W:Interrupt event */

;/*
; * MMU register
; */
PTEH:		.equ	H'ff000000	;/* W:Page table entry high */
PTEL:		.equ	H'ff000004	;/* W:Page table entry low */
PTEA:		.equ	H'ff000034	;/* W:Page table entry assistance */
TTB:		.equ	H'ff000008	;/* W:Page table base */
TEA:		.equ	H'ff00000c	;/* W:TLB exception address */
MMUCR:		.equ	H'ff000010	;/* W:MMU control */

MMU_AT:		.equ	H'00000001	;/* MMU enable */
MMU_TF:		.equ	H'00000004	;/* TLB flush */
MMU_SV:		.equ	H'00000100	;/* Single virtual memory mode */
MMU_SQMD:	.equ	H'00000200	;/* Store queue mode */

;/*
; * Cache register
; */
CCR:		.equ	H'ff00001c	;/* W:Cache control */
QACR0:		.equ	H'ff000038	;/* W:Queue address control 0 */
QACR1:		.equ	H'ff00003c	;/* W:Queue address control 1 */

CCR_OCE:	.equ	H'00000001	;/* OC enable */
CCR_WT:		.equ	H'00000002	;/* Write through enable (U0 P0 P3) */
CCR_CB:		.equ	H'00000004	;/* Copy back enable (P1) */
CCR_OCI:	.equ	H'00000008	;/* OC invalidation */
CCR_ORA:	.equ	H'00000020	;/* OC RAM enable */
CCR_OIX:	.equ	H'00000080	;/* OC index enable */
CCR_ICE:	.equ	H'00000100	;/* IC enable */
CCR_ICI:	.equ	H'00000800	;/* IC invalidation */
CCR_IIX:	.equ	H'00008000	;/* IC index enable */
CCR_EMODE:	.equ	H'80000000	;/* Cache double mode */

;/*
; * Interrupt controller
; */
ICR:		.equ	H'ffd00000	;/* H:Interrupt control */
IPRA:		.equ	H'ffd00004	;/* H:Interrupt priority A */
IPRB:		.equ	H'ffd00008	;/* H:Interrupt priority B */
IPRC:		.equ	H'ffd0000c	;/* H:Interrupt priority C */
IPRD:		.equ	H'ffd00010	;/* H:Interrupt priority D */
INTPRI00:	.equ	H'fe080000	;/* W:Interrupt priority level 00 */
INTREQ00:	.equ	H'fe080020	;/* W:Interrupt factor 00 */
INTMSK00:	.equ	H'fe080040	;/* W:Interrupt mask 00 */
INTMSKCLR00:	.equ	H'fe080060	;/* W:Interrupt mask clear 00 */

;/*
; * Interrupt vector number for T-Monitor
; */
TRAP_MONITOR:	.equ	H'70	;/* Monitor service call */

;/*
; * Interrupt vector number for T-Kernel
; */
TRAP_SVC:	.equ	H'71	;/* System call, Extension SVC */
TRAP_RETINT:	.equ	H'72	;/* tk_ret_int() system call */
TRAP_DISPATCH:	.equ	H'73	;/* Task dispatcher call */
TRAP_DCT:	.equ	H'74	;/* Delayed context trap */
TRAP_LOADSR:	.equ	H'75	;/* SR register load function */
TRAP_DEBUG:	.equ	H'76	;/* Debugger support function */

;/*
; * Interrupt vector number for Extension
; */
TRAP_KILLPROC:	.equ	H'77	;/* Force process termination request */

	.aendi ;/* __TK_SYSDEF_DEPEND_H__ */
