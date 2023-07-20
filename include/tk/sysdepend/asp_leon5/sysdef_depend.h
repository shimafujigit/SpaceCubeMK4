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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysdef_depend.h (tk/ASP-LEON5)
 *
 *	Definition about LEON5
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * PSR Register
 */
#define	PSR_CWP		0x1f			/* current window pointer */
#define	PSR_CWPn(n)	((n) & 0x1f)
#define	PSR_ET		(1 << 5)		/* enable trap */
#define	PSR_PS		(1 << 6)		/* previous supervisor */
#define	PSR_S		(1 << 7)		/* supervisor */
#define	PSR_PIL_BIT	8
#define	PSR_PIL		(0xf << PSR_PIL_BIT)	/* interrupt level */
#define	PSR_PILn(n)	(((n) & 0xf) << PSR_PIL_BIT)
#define	PSR_EF		(1 << 12)		/* enable FPU */
#define	PSR_EC		(1 << 13)		/* enable coprocessor */
#define	PSR_ICC(n)	(((n) & 0xf) << 20)	/* integer condition code */

/*
 * FSR Register
 */
#define	FSR_CEXC(n)	(((n) & 0x1f) << 0)	/* current exception */
#define	FSR_AEXC(n)	(((n) & 0x1f) << 5)	/* accrued exception */
#define	FSR_FCC(n)	(((n) & 0x03) << 10)	/* condition code */
#define	FSR_QNE_BIT	13
#define	FSR_QNE		(1 << FSR_QNE_BIT)	/* trap queue not empty */
#define	FSR_FTT(n)	(((n) & 0x07) << 14)	/* trap type */
#define	FSR_NS		(1 << 22)		/* nonstandard */
#define	FSR_TEM(n)	(((n) & 0x1f) << 23)	/* trap enable mask */
#define	FSR_RD(n)	(((n) & 0x03) << 30)	/* rounding direction */

#define	FSR_INIT	0	/* initial setting */

/*
 * Trap number for T-Monitor
 */
#define	TRAP_MONITOR	16	/* Monitor service call */

/*
 * Trap/Interrupt number for T-Kernel
 */
#define	TRAP_SVC	17	/* System call, Extension SVC */
#define	TRAP_DEBUG	18	/* Debugger support function */
#define	TRAP_RETTEX	19	/* Return from task exception handler */
#define	INT_DISPATCH	1	/* Task dispatcher call */

#endif /* __TK_SYSDEF_DEPEND_H__ */
