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
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	cpu_insn.c (SH7750R)
 *	CPU-Dependent Operation
 */


#include <basic.h>
#include "cpu_insn.h"

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */

/*
 * Set interrupt handler
 */
#pragma inline(define_inthdr)
static void define_inthdr( INT vecno, FP inthdr )
{
	SCArea->intvec[vecno] = inthdr;
}

/*
 * If it is the task-independent part, TRUE
 *	If EIT stack is consumed, the task independent part
 */
#pragma inline_asm(getISP)
static VW *getISP( void )
{
	stc		r6_bank, r0
}

#pragma inline(isTaskIndependent)
static BOOL isTaskIndependent( void )
{
	VW	*isp;

	isp = getISP();
	return ( isp != int_stack_top )? TRUE: FALSE;
}

/*
 * Move to/Restore task independent part
 */
#pragma inline_asm(EnterTaskIndependent)
static void EnterTaskIndependent( void )
{
	stc		r6_bank, r0
	add		#H'FE, r0
	ldc		r0, r6_bank
}
#pragma inline_asm(LeaveTaskIndependent)
static void LeaveTaskIndependent( void )
{
	stc		r6_bank, r0
	add		#2, r0
	ldc		r0, r6_bank
}


