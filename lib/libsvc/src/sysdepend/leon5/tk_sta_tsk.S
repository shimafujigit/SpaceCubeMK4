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

#ifdef TFN_STA_TSK

	.text
	.balign	4
	.globl	Csym(tk_sta_tsk)
	.type	Csym(tk_sta_tsk), #function
Csym(tk_sta_tsk):
	set	TFN_STA_TSK, %g2
	ta	TRAP_SVC
	retl
	nop

#endif

