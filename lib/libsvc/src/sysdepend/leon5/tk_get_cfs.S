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
 *	T-Kernel extended SVC interface library (leon5)
 *
 *	   created from <tk/sysmgr.h>
 */


#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/fnsysmgr.h>

#ifdef SYSTEM_TK_GET_CFS_FN

	.text
	.balign	4
	.globl	Csym(tk_get_cfs)
	.type	Csym(tk_get_cfs), #function
Csym(tk_get_cfs):
	st	%o0, [%sp+17*4]
	st	%o1, [%sp+18*4]
	st	%o2, [%sp+19*4]
	st	%o3, [%sp+20*4]
	st	%o4, [%sp+21*4]
	st	%o5, [%sp+22*4]
	add	%sp, 17*4, %o0
	set	SYSTEM_TK_GET_CFS_FN, %g2
	ta	TRAP_SVC
	retl
	nop

#endif

