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
 *	@(#)disint.S (libtk/ASP-LEON5)
 *
 *	Enable/disable external interrupt.
 */

#include <machine.h>
#include <tk/sysdef.h>

/*
 * Disable external interrupt
 * UINT disint( void )
 */
	.text
	.balign	4
	.globl	Csym(disint)
	.type	Csym(disint), #function
Csym(disint):
	rd	%psr, %o0
	set	PSR_PIL|PSR_ET|PSR_EF, %g1
	andn	%o0, %g1, %g1
	wr	%g1, PSR_PIL, %psr		// PSR.ET=0 PIL=15
	wr	%g1, PSR_PIL|PSR_ET, %psr	// PSR.ET=1
	nop
	retl
	 nop

/*
 * Enable external interrupt (restore to original state)
 * UINT enaint( UINT intsts )
 */
	.text
	.balign	4
	.globl	Csym(enaint)
	.type	Csym(enaint), #function
Csym(enaint):
	and	%o0, PSR_PIL, %g1
	rd	%psr, %o0
	set	PSR_PIL|PSR_ET|PSR_EF, %g2
	andn	%o0, %g2, %g2
	wr	%g2, %g1, %psr			// PSR.ET=0 PIL=intsts
	or	%g1, PSR_ET, %g1
	wr	%g2, %g1, %psr			// PSR.ET=1
	nop
	retl
	 nop
