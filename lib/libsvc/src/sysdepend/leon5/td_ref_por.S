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
 *	T-Kernel/DS SVC interface library (leon5)
 *
 *	   created from dbgspt.h
 */


#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tdfncd.h>

#ifdef TDFN_REF_POR

	.text
	.balign	4
	.globl	Csym(td_ref_por)
	.type	Csym(td_ref_por), #function
Csym(td_ref_por):
	set	TDFN_REF_POR, %g2
	ta	TRAP_DEBUG
	retl
	nop

#endif

