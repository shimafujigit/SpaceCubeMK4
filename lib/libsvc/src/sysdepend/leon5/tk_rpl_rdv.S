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
 *	T-Kernel SVC interface library (leon5)
 *
 *	   created from syscall.h
 */


#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tkfncd.h>

#ifdef TFN_RPL_RDV

	.text
	.balign	4
	.globl	Csym(tk_rpl_rdv)
	.type	Csym(tk_rpl_rdv), #function
Csym(tk_rpl_rdv):
	set	TFN_RPL_RDV, %g2
	ta	TRAP_SVC
	retl
	nop

#endif

