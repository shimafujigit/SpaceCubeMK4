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
 *	@(#)int.c (libtk/ASP-LEON5)
 *
 *	Interrupt controller  
 */

#include <basic.h>
#include <tk/syslib.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/irqmp.h>

/*
 * Kernel configuration file
 */
#include "config/tk_config_func.h"

#if defined(USE_FUNC_ENABLEINT)
/*
 * Enable interrupt
 */
EXPORT void EnableInt( INTVEC intvec )
{
	bcc_int_unmask(intvec);
}
#endif /* USE_FUNC_ENABLEINT */


#if defined(USE_FUNC_DISABLEINT)
/*
 * Disable interrupt
 */
EXPORT void DisableInt( INTVEC intvec )
{
	bcc_int_mask(intvec);
}
#endif /* USE_FUNC_DISABLEINT */


#if defined(USE_FUNC_CLEARINT)
/*
 * Clear interrupt request
 */
EXPORT void ClearInt( INTVEC intvec )
{
	bcc_int_clear(intvec);
}
#endif /* USE_FUNC_CLEARINT */


#if defined(USE_FUNC_CHECKINT)
/*
 * Check the existence of interrupt request
 */
EXPORT BOOL CheckInt( INTVEC intvec )
{
	volatile struct irqmp_regs *regs =
		(struct irqmp_regs*)__bcc_int_handle;

	if ( __bcc_int_handle == 0 ) return 0;

	return (regs->ipend >> intvec) & 1;
}
#endif /* USE_FUNC_CHECKINT */
