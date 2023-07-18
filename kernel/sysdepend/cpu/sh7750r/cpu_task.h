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
 *	cpu_task.h (SH7750R)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _CPU_TASK_
#define _CPU_TASK_

IMPORT TCB	*dsp_ctxtsk;		/* Task including DSP context */
IMPORT TCB	*fpu_ctxtsk;		/* Task including FPU context */

/*
 * System stack configuration at task startup
 */
typedef struct {
	struct _rng0_3 {		/* RNG 0-3 common */
		VW	mach;
		VW	macl;
		void	*pr;
		void	*gbr;
		VW	r[15];		/* bank0 R0-R7, R8-R14 */
		VW	bank1_r1;
		VW	bank1_r0;
		void	*spc;
		UW	ssr;
		UW	mdr;		/* bank1 R2 */
	} rng0_3;
	struct _rng1_3 {	/* Only RNG 1-3 */
		void	*usp;
	} rng1_3;
} SStackFrame;

#define SizeofSS0	( sizeof(SStackFrame) - sizeof(void*) )	/* RNG 0 */
#define SizeofSS1_3	( sizeof(SStackFrame) )			/* RNG 1-3 */

/*
 * User stack configuration at task startup (only RNG 1-3)
 */
/*typedef struct {*/
	/* Empty */
/*} UStackFrame;*/

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 4 )	/* To 'spc' */

/*
 * Size of area kept for special use from system stack
 */
#if USE_SH3_DSP
#define RESERVE_SSTACK(tskatr)	\
		( ( ((tskatr) & TA_COP0) != 0 )? sizeof(T_COP0REGS): 0 )
#elif TA_FPU
#define RESERVE_SSTACK(tskatr)	\
		( ( ((tskatr) & TA_FPU) != 0 )? sizeof(T_COP0REGS): 0 )
#else
#define RESERVE_SSTACK(tskatr)	0
#endif

/*
 * Initial value for task startup
 */
#if	USE_MEM_PROTECT
#define INIT_SR(rng)	( ( (rng) >= 2 )? 0: SR_MD )
#elif USE_MMU
#define INIT_SR(rng)	( ( (rng) == 3 )? 0: SR_MD )
#else
#define INIT_SR(rng)	( SR_MD )
#endif

#define INIT_MDR(rng)	( MDR_PPL(rng) | MDR_CPL(rng) )

#define INIT_DSP_DSR	0		/* DSP register initial value */
#define INIT_FPSCR	0x000c0000	/* FPU register initial value */

/*
 * Switch task eigenspace
 */
IMPORT void change_space( void *uatb, INT lsid );

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
