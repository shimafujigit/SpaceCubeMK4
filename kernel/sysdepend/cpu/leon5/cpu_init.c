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
 *	cpu_init.c (ASP-LEON5)
 *	CPU-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include "task.h"
#include "cpu_insn.h"

EXPORT ATR	available_cop;	/* Enabled coprocessor */
EXPORT TCB	*fpu_ctxtsk;	/* FPU context task */

#include <bcc/bcc_param.h>
#include <bcc/regs/irqmp.h>
extern int __bcc_cpuid;

EXPORT	_UW	*int_force_reg;

/*
 * Check existence of coprocessor
 */
LOCAL ATR check_cop( void )
{
	return TA_COP0;
}

/*
 * CPU-dependent initialization
 */
EXPORT ER cpu_initialize( void )
{
	/* No not save monitor exception handlers : may not require */

	IMPORT void dispatch_entry( void );	/* Dispatcher call */
	IMPORT void call_entry( void );		/* System call */
	IMPORT void rettex_entry( void );	/* Return task exception */
	IMPORT void call_dbgspt( void );	/* Debugger support call */
	IMPORT void unavailable_fpu( void );	/* Unavailable FPU */

	volatile struct irqmp_regs *regs =
		(struct irqmp_regs*)__bcc_int_handle;

	if ( __bcc_int_handle == 0 ) return E_SYS;

	if ( (regs->mpstat & IRQMP_MPSTAT_NCPU) == 0 ) {
		int_force_reg = &regs->iforce0;
	} else {
		int_force_reg = &regs->piforce[__bcc_cpuid];
	}

	/* Check enabled coprocessor */
	available_cop = check_cop();
	fpu_ctxtsk = NULL;

	/* Register exception handler used on OS */
	define_inthdr(EIT_TRAP(TRAP_SVC),    call_entry);
	define_inthdr(EIT_TRAP(TRAP_RETTEX), rettex_entry);
	define_inthdr(EIT_INT(INT_DISPATCH), dispatch_entry);
	define_inthdr(EIT_FP_DISABLED,       unavailable_fpu);
#if USE_DBGSPT
	define_inthdr(EIT_TRAP(TRAP_DEBUG),  call_dbgspt);
#endif

	EnableInt(INT_DISPATCH);

	return E_OK;
}

/*
 * CPU-dependent finalization
 */
EXPORT void cpu_shutdown( void )
{
	DisableInt(INT_DISPATCH);

	/* Restore saved monitor exception handler : may not require */
}

/* ------------------------------------------------------------------------- */

/*
 * Task exception handler startup reservation
 */
EXPORT void request_tex( TCB *tcb )
{
	/* Cannot set to the task operating at protected level 0 */
	if ( tcb->isysmode == 0 ) {
		tcb->reqdct = 0x80;
	}
}

/*
 * Task exception handler startup setting
 *
 *	texhdr -> return value
 *	texcd  -> *p_texcd
 */
EXPORT FP setup_texhdr( INT *p_texcd )
{
	FP	texhdr;
	INT	texcd;
	UINT	m;

	/* Called in interrupt disable state */

	ctxtsk->reqdct = 0;		/* Free DCT */

	/* Get exception code */
	m = 0x00000001;
	for ( texcd = 0; texcd <= 31; texcd++ ) {
		if ( (ctxtsk->exectex & m) != 0 ) {
			break;
		}
		m <<= 1;
	}

	/* Exception does not occur / Exception is freed */
	if ( texcd > 31 ) {
		return NULL;
	}

	ctxtsk->exectex = 0;
	ctxtsk->pendtex &= ~m;
	ctxtsk->texflg |= ( texcd == 0 )? TEX0_RUNNING: TEX1_RUNNING;
	texhdr = ctxtsk->texhdr;

	ENABLE_INTERRUPT;

	*p_texcd = texcd;
	return texhdr;
}
