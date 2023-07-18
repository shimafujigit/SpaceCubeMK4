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
 *	tkdev_init.c (SH7750R)
 *	T-Kernel Device-Dependent Initialization/Finalization
 */

#include "kernel.h"
#include <tk/sysdef.h>
#include <tk/syslib.h>
#include <tm/tmonitor.h>

/*
 * Target system-dependent initialization
 */
EXPORT ER tkdev_initialize( void )
{
	/* Disable all interrupts */
	out_h(IPRA, 0);
	out_h(IPRB, 0);
	out_h(IPRC, 0);
	out_h(IPRD, 0);
//	out_w(INTPRI00, 0);
//	out_w(INTMSK00, 0x3ff);

	return E_OK;
}

/*
 Target system-dependent finalization
 *	Normally jump to ROM monitor.
 *	No return from this function.
 */
EXPORT void tkdev_exit( void )
{
	disint();
	tm_exit(0);	/* Turn off power and exit */

	/* Not suppose to return from 'tm_exit,' but just in case */
	for ( ;; ) {
		tm_monitor();  /* To T-Monitor */
	}
}
