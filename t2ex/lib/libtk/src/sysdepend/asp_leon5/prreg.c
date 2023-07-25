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
 *	@(#)prreg.c (libtk/ASP-LEON5)
 *
 *	Display task register 
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <sys/misc.h>

/*
 * Uses prfn to display the contents of gr, er, and cr.
 * prfn must be a printf compatible function.
 */
EXPORT W PrintTaskRegister( int (*prfn)( const char *format, ... ),
				T_REGS *gr, T_EIT *er, T_CREGS *cr )
{
/*
 *	PC: 12345678 nPC:12345678 PSR:12345678 SP: 12345678
 *	Y:  12345678 g1: 12345678 g2: 12345678 g3: 12345678
 *	g4: 12345678 g5: 12345678 g6: 12345678 g7: 12345678
 *	o0: 12345678 o1: 12345678 o2: 12345678 o3: 12345678
 *	o4: 12345678 o5: 12345678              o7: 12345678
 *	l0: 12345678 l1: 12345678 l2: 12345678 l3: 12345678
 *	l4: 12345678 l5: 12345678 l6: 12345678 l7: 12345678
 *	i0: 12345678 i1: 12345678 i2: 12345678 i3: 12345678
 *	i4: 12345678 i5: 12345678 i6: 12345678 i7: 12345678
 */
	(*prfn)("PC: %08x nPC:%08x PSR:%08x SP: %08x\n",
		(UW)er->pc, (UW)er->npc, er->psr, (UW)cr->sp);
	(*prfn)("Y:  %08x g1: %08x g2: %08x g3: %08x\n",
		gr->y, gr->g1, gr->g2, gr->g3);
	(*prfn)("g4: %08x g5: %08x g6: %08x g7: %08x\n",
		gr->g4, gr->g5, gr->g6, gr->g7);
	(*prfn)("o0: %08x o1: %08x o2: %08x o3: %08x\n",
		gr->o0, gr->o1, gr->o2, gr->o3);
	(*prfn)("o4: %08x o5: %08x              o7: %08x\n",
		gr->o4, gr->o5,         gr->o7);
	(*prfn)("l0: %08x l1: %08x l2: %08x l3: %08x\n",
		gr->l0, gr->l1, gr->l2, gr->l3);
	(*prfn)("l4: %08x l5: %08x l6: %08x l7: %08x\n",
		gr->l4, gr->l5, gr->l6, gr->l7);
	(*prfn)("i0: %08x i1: %08x i2: %08x i3: %08x\n",
		gr->i0, gr->i1, gr->i2, gr->i3);
	(*prfn)("i4: %08x i5: %08x i6: %08x i7: %08x\n",
		gr->i4, gr->i5, gr->i6, gr->i7);
	return 9;  /* Number of display rows */
}
