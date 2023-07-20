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
 *	@(#)cpudef.h  (tk/ASP-LEON5)
 *
 *	LEON5 dependent definition
 */

#ifndef __TK_CPUDEF_H__
#define __TK_CPUDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	g1,g2,g3,g4,g5,g6,g7;		/* %g */
	VW	o0,o1,o2,o3,o4,o5,o7;		/* %o */
	VW	l0,l1,l2,l3,l4,l5,l6,l7;	/* %l */
	VW	i0,i1,i2,i3,i4,i5,i6,i7;	/* %i */
	VW	y;		/* multiply/divide */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	VP	pc;		/* program counter */
	VP	npc;		/* next program counter */
	UW	psr;		/* processor state */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	VP	sp;		/* %o6 */
} T_CREGS;

/*
 * Coprocessor register			tk_get_cpr tk_set_cpr
 */
typedef struct t_cop0regs {
	UW	f[32];		/* f0¡Áf31 */
	UW	fsr;		/* floating-point state */
} T_COP0REGS;

typedef union {
	T_COP0REGS	cop0;	/* FPU */
#if 0
	T_COP1REGS	cop1;
	T_COP2REGS	cop2;
	T_COP3REGS	cop3;
#endif
} T_COPREGS;

#ifdef __cplusplus
}
#endif
#endif /* __TK_CPUDEF_H__ */
