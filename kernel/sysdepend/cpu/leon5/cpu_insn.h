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
 *	cpu_insn.h (ASP-LEON5)
 *	CPU-Dependent Operation
 */

#ifndef _CPU_INSN_
#define _CPU_INSN_

#include <sys/sysinfo.h>
#include <tk/sysdef.h>
#include <tk/syslib.h>

#include <bcc/bcc.h>

/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

/*
 * get PSR register
 */
Inline UINT getPSR( void )
{
	UINT	psr;

	Asm("mov %%psr, %0": "=r"(psr));

	return psr;
}

/*
 * set/get PIL (interrupt level)
 */
Inline void setPIL( UINT pil )
{
	enaint(PSR_PILn(pil));
}
Inline UINT getPIL( void )
{
	return (getPSR() & PSR_PIL) >> PSR_PIL_BIT;
}

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */

/*
 * Set interrupt handler
 */
Inline void define_inthdr( INT vecno, FP inthdr )
{
	bcc_set_trap(vecno, inthdr);
}

/*
 * If it is the task-independent part, TRUE
 */
Inline BOOL isTaskIndependent( void )
{
	return ( SCInfo.taskindp > 0 )? TRUE: FALSE;
}

/*
 * Move to/Restore task independent part
 */
Inline void EnterTaskIndependent( void )
{
	SCInfo.taskindp++;
}
Inline void LeaveTaskIndependent( void )
{
	SCInfo.taskindp--;
}

/* ------------------------------------------------------------------------ */

#endif /* _CPU_INSN_ */
