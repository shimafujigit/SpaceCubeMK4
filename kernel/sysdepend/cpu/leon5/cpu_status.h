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
 *	cpu_status.h (ASP-LEON5)
 *	CPU-Dependent Definition
 */

#ifndef _CPU_STATUS_
#define _CPU_STATUS_

#include <tk/syslib.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"

#include <bcc/bcc.h>

/*
 * Start/End critical section
 */
#define	BEGIN_CRITICAL_SECTION	{ UINT _psr_ = disint();
#define END_CRITICAL_SECTION	if ( schedtsk != ctxtsk ) dispatch_request(); \
				enaint(_psr_); \
				if ( (_psr_ & PSR_PIL) == 0 ) wait_delayint(); }

/*
 * Start/End interrupt disable section
 */
#define	BEGIN_DISABLE_INTERRUPT	{ UINT _psr_ = disint();
#define	END_DISABLE_INTERRUPT	enaint(_psr_); }

/*
 * Interrupt enable/disable
 */
#define ENABLE_INTERRUPT	{ enaint(0); }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 * Enable interrupt nesting
 *	Enable the interrupt that has a higher priority than 'level.'
 */
#define	ENABLE_INTERRUPT_UPTO(level)	{ setPIL(level); }

/*
 * Move to/Restore task independent part
 */
#define ENTER_TASK_INDEPENDENT	{ EnterTaskIndependent(); }
#define LEAVE_TASK_INDEPENDENT	{ LeaveTaskIndependent(); }

/* ----------------------------------------------------------------------- */
/*
 *	Check system state
 */

/*
 * When a system call is called from the task independent part, TRUE
 */
#define in_indp()	( isTaskIndependent() || ctxtsk == NULL )

/*
 * When a system call is called during dispatch disable, TRUE
 * Also include the task independent part as during dispatch disable.
 */
#define	in_ddsp()	( getPIL() > 0 || in_indp() )

/*
 * When a system call is called during CPU lock (interrupt disable), TRUE
 * Also include the task independent part as during CPU lock.
 */
#define	in_loc()	( getPIL() > 1 || in_indp() )

/*
 * When a system call is called during executing the quasi task part, TRUE
 * Valid only when in_indp() == FALSE because it is not discriminated from
 * the task independent part.
 */
#define in_qtsk()	( ctxtsk->sysmode > ctxtsk->isysmode )

/* ----------------------------------------------------------------------- */
/*
 *	Task dispatcher startup routine
 */

IMPORT	_UW	*int_force_reg;
#define	delayint_bit		(1 << 1)
#define	signal_delayint()	(*int_force_reg |= delayint_bit)
#define	clear_delayint()	(*int_force_reg &= ~delayint_bit)
#define	wait_delayint()		while ( (*int_force_reg & delayint_bit) != 0 )

/*
 * Request for task dispatcher startup
 */
#define dispatch_request()	signal_delayint()

/*
 * Throw away the current task context
 * and forcibly dispatch to the task that should be performed next.
 *	Use at system startup and 'tk_ext_tsk, tk_exd_tsk.'
 */
Inline void force_dispatch( void )
{
	clear_delayint();
	Asm("ba,a dispatch_to_schedtsk");
}

/*
 * Start task dispatcher
 */
#define	dispatch()	/* nop */

/* ----------------------------------------------------------------------- */
/*
 *	Task exception
 */

/*
 * Task exception handler startup reservation
 */
IMPORT void request_tex( TCB *tcb );

/* ----------------------------------------------------------------------- */

/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */
	void	*uatb;		/* Task space page table */
	INT	lsid;		/* Task space ID */
} CTXB;

/*
 * CPU information
 */
IMPORT ATR	available_cop;	/* Enabled coprocessor (TA_COPn) */

#endif /* _CPU_STATUS_ */
