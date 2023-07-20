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
 *	cpu_task.c (ASP-LEON5)
 *	CPU-Dependent Task Start Processing
 */
#include <basic.h>
#include <kernel.h>
#include "task.h"
#include "cpu_task.h"

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
EXPORT	void setup_context( TCB *tcb )
{
	SStackFrame	*ssp;
	UW		rng;
	const UW	cwp = 1;

	rng = tcb->tskatr & TA_RNG3;
	ssp = tcb->isstack;
	--ssp;

	if ( (tcb->tskatr & TA_FPU) != 0 ) {
		/* setup fpu context */
		FPUContext	*fpu = tcb->isstack;
		fpu->fsr = FSR_INIT;
	}

#ifndef _FLAT
	ssp->wim = 1 << (cwp + 1);
#else
	ssp->wim = ~0 << (cwp + 1);
#endif
	ssp->psr = PSR_CWPn(cwp - 1)|PSR_PILn(0)|PSR_S;
	if ( rng != TA_RNG3 ) ssp->psr |= PSR_PS;
	ssp->pc = (UW*)tcb->task;	/* Task startup address */
	ssp->npc = ssp->pc + 1;
	ssp->i6 = (UW)&ssp->l0;		/* fp */
	tcb->tskctxb.ssp = ssp;		/* System stack */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
EXPORT	void setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->o0 = stacd;
	ssp->o1 = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
EXPORT	void cleanup_context( TCB *tcb )
{
	if ( fpu_ctxtsk == tcb ) fpu_ctxtsk = NULL;
}
