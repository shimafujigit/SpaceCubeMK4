/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysdef_depend.h (tk/ASP-SH7750R)
 *
 *	Definition about SH7750R
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * Use specify register
 */
#define SP	r15	/* Stack pointer */

/*
 * BANK1 register
 *	R0	For work				With task
 *	R1	Reservation				With task
 *	R2 MDR	Operation mode register		  	With task
 *	R3 SST	System stack top		  	With task
 *	R4	For work				Task independent
 *	R5	Reservation				Task independent
 *	R6 ISP	EIT stack pointer	  		Task independent
 *	R7	For EIT work				Task independent
 *		(Available only at SR.BL=1)
 */
#define MDR	r2	/* Operation mode register */
#define SST	r3	/* System stack top */
#define ISP	r6	/* EIT stack pointer */

/*
 * MDR register
 */
#define MDR_DCT		0x80000000	/* Delayed context trap request */
#define MDR_PPL(n)	( (n) << 16 )	/* Previous protection level (0-3) */
#define MDR_CPL(n)	( (n) )		/* Current protection level (0-3) */

/*
 * SR register
 */
#define SR_MD		0x40000000	/* Privilege mode */
#define SR_RB		0x20000000	/* Register bank */
#define SR_BL		0x10000000	/* Interrupt block */
#if	0	/* IMPLICIT FPU PROBLEM */
#define SR_FD		0x00000000	/* FPU disable (SH4) */
#else
#define SR_FD		0x00008000	/* FPU disable (SH4) */
#endif
#define SR_I(n)		( (n) << 4 )	/* Interrupt mask register (0-15) */

/*
 * FPU status/control register (SH4)
 */
#define FPSCR_FR	0x00200000	/* Register bank */
#define FPSCR_SZ	0x00100000	/* Transfer size mode */
#define FPSCR_PR	0x00080000	/* Precision mode */
#define FPSCR_DN	0x00040000	/* Denormalization mode */
#define FPSCR_C(n)	((n) << 12)	/* Exception cause  (6bit) */
#define FPSCR_E(n)	((n) << 7)	/* Exception enable (5bit) */
#define FPSCR_F(n)	((n) << 2)	/* Exception flag   (5bit) */
#define FPSCR_RM(n)	(n)		/* Rounding mode    (2bit) */

					/* FPU exception */
#define FPE_I		0x01		/*   Inexact */
#define FPE_U		0x02		/*   Underflow */
#define FPE_O		0x04		/*   Overflow */
#define FPE_Z		0x08		/*   Division by zero */
#define FPE_V		0x10		/*   Invalid */
#define FPE_E		0x20		/*   FPU error */

/*
 * EIT register
 */
#define TRA		0xff000020	/* W:TRAPA exception */
#define EXPEVT		0xff000024	/* W:Exception event */
#define INTEVT		0xff000028	/* W:Interrupt event */

/*
 * MMU register
 */
#define PTEH		0xff000000	/* W:Page table entry high */
#define PTEL		0xff000004	/* W:Page table entry low */
#define PTEA		0xff000034	/* W:Page table entry assistance */
#define TTB		0xff000008	/* W:Page table base */
#define TEA		0xff00000c	/* W:TLB exception address */
#define MMUCR		0xff000010	/* W:MMU control */

#define MMU_AT		0x00000001	/* MMU enable */
#define MMU_TF		0x00000004	/* TLB flush */
#define MMU_SV		0x00000100	/* Single virtual memory mode */
#define MMU_SQMD	0x00000200	/* Store queue mode */

/*
 * Cache register
 */
#define CCR		0xff00001c	/* W:Cache control */
#define QACR0		0xff000038	/* W:Queue address control 0 */
#define QACR1		0xff00003c	/* W:Queue address control 1 */

#define CCR_OCE		0x00000001	/* OC enable */
#define CCR_WT		0x00000002	/* Write through enable (U0 P0 P3) */
#define CCR_CB		0x00000004	/* Copy back enable (P1) */
#define CCR_OCI		0x00000008	/* OC invalidation */
#define CCR_ORA		0x00000020	/* OC RAM enable */
#define CCR_OIX		0x00000080	/* OC index enable */
#define CCR_ICE		0x00000100	/* IC enable */
#define CCR_ICI		0x00000800	/* IC invalidation */
#define CCR_IIX		0x00008000	/* IC index enable */
#define CCR_EMODE	0x80000000	/* Cache double mode */

/*
 * Interrupt controller
 */
#define ICR		0xffd00000	/* H:Interrupt control */
#define IPRA		0xffd00004	/* H:Interrupt priority A */
#define IPRB		0xffd00008	/* H:Interrupt priority B */
#define IPRC		0xffd0000c	/* H:Interrupt priority C */
#define IPRD		0xffd00010	/* H:Interrupt priority D */
#define INTPRI00	0xfe080000	/* W:Interrupt priority level 00 */
#define INTREQ00	0xfe080020	/* W:Interrupt factor 00 */
#define INTMSK00	0xfe080040	/* W:Interrupt mask 00 */
#define INTMSKCLR00	0xfe080060	/* W:Interrupt mask clear 00 */

/*
 * Interrupt vector number for T-Monitor
 */
#define TRAP_MONITOR	0x70	/* Monitor service call */

/*
 * Interrupt vector number for T-Kernel
 */
#define TRAP_SVC	0x71	/* System call, Extension SVC */
#define TRAP_RETINT	0x72	/* tk_ret_int() system call */
#define TRAP_DISPATCH	0x73	/* Task dispatcher call */
#define TRAP_DCT	0x74	/* Delayed context trap */
#define TRAP_LOADSR	0x75	/* SR register load function */
#define TRAP_DEBUG	0x76	/* Debugger support function */

/*
 * Interrupt vector number for Extension
 */
#define TRAP_KILLPROC	0x77	/* Force process termination request */

#endif /* __TK_SYSDEF_DEPEND_H__ */
