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
 *	cpu_task.h (ASP-LEON5)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

IMPORT TCB	*fpu_ctxtsk;

/*
 * System stack configuration at task startup
 */
typedef struct {
	VW	rsv1[16+7];
	UW	wim;
	UW	psr;
	UW	*pc;
	UW	*npc;
	VW	y;
	VW	o0,o1,o2,o3,o4,o5,o7;
	VW	g1,g2,g3,g4,g5,g6,g7;
	VW	l0,l1,l2,l3,l4,l5,l6,l7;
	VW	i0,i1,i2,i3,i4,i5,i6,i7;
	VW	rsv2[7+1];
} SStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 43 )	/* To 'wim' */

/*
 * FPU context
 */
typedef struct {
	UW	f[32];
	UW	fsr;
	UW	rsv;
} FPUContext;

/*
 * save FPU context
 */
IMPORT void save_fpuctx( FPUContext* );

/*
 * Size of area kept for special use from system stack
 */
#define	RESERVE_SSTACK(tskatr)	\
	( ( ((tskatr) & TA_FPU) != 0 )? sizeof(FPUContext): 0 )

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
IMPORT void setup_context( TCB *tcb );

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
IMPORT void setup_stacd( TCB *tcb, INT stacd );

/*
 * Delete task contexts
 */
IMPORT void cleanup_context( TCB *tcb );

#endif /* _CPU_TASK_ */
