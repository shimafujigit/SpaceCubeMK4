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
 *    Changed by UC Technology at 2013/01/28.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	cpu_task.c (SH7750R)
 *	CPU-Dependent Task Start Processing
 */

#include "kernel.h"
#include "task.h"
#include "cpu_task.h"

/*
 * Switch task eigenspace
 */
#pragma inline(change_space)
static void change_space( void *uatb, INT lsid )
{
//	*(_UW*)TTB  = (UW)uatb;
//	*(_UW*)PTEH = (UW)lsid;
}

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
#pragma inline(setup_context)
static void setup_context( TCB *tcb )
{
	SStackFrame	*ssp;
	T_COP0REGS	*cp0;	/* SH3:DSP */
	W		rng;
	UW		sr;

	rng = (tcb->tskatr & TA_RNG3) >> 8;
	cp0 = tcb->isstack;
	ssp = (SStackFrame*)((UB*)(tcb->isstack) - (UB*)((rng==0)? SizeofSS0:SizeofSS1_3));

	sr = INIT_SR(rng);

	if ( (tcb->tskatr & TA_COP0) != 0 ) {
#if USE_SH3_DSP
		/* DSP context initialization */
		cp0->dsr = INIT_DSP_DSR;
		sr |= SR_DSP;
#endif
#if TA_FPU
		/* FPU context initialization */
		cp0->fpscr = INIT_FPSCR;
#endif
	}

	/* CPU context initialization */
	ssp->rng0_3.mdr = INIT_MDR(rng);	/* Initial MDR */
	ssp->rng0_3.ssr = sr;			/* Initial SR */
	ssp->rng0_3.spc = tcb->task;		/* Task startup address */
	tcb->tskctxb.ssp = ssp;			/* System stack */

	if ( rng > 0 ) {
		ssp->rng1_3.usp = tcb->istack;	/* User stack */
	}
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
#pragma inline(setup_stacd)
static void setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->rng0_3.r[4] = stacd;
	ssp->rng0_3.r[5] = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
#pragma inline(cleanup_context)
static void cleanup_context( TCB *tcb )
{
#if USE_SH3_DSP
	/* If the current DSP context belongs to 'tcb,'
	   Disable the DSP context */
	if ( dsp_ctxtsk == tcb ) {
		dsp_ctxtsk = NULL;
	}
#endif
#if TA_FPU
	/* If the current FPU context belongs to 'tcb,'
	   Disable the FPU context */
	if ( fpu_ctxtsk == tcb ) {
		fpu_ctxtsk = NULL;
	}
#endif
}

