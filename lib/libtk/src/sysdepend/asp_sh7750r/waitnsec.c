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
 *	@(#)waitnsec.c (libtk/SH7750R)
 *
 *	Busy loop wait time in nanoseconds 
 */

#include <basic.h>
#include <sys/sysinfo.h>

/*
 * Wait loop
 */
extern void WaitLoop(UW count);

#pragma inline_asm(WaitLoop)
static void WaitLoop( UW count )
{
?_loop:
		tst		r4, r4
		bf/s	?_loop
		add		#-1, r4
}

EXPORT void WaitNsec( UINT nsec )
{
	UW	count = nsec * SCInfo.loop64us / 64000U;

	WaitLoop(count);
}
