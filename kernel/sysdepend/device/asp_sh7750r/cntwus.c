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
 *	cntwus.c (SH7750R)
 *	Loop Count Computation of micro Second Wait 
 */

#include <basic.h>
#include <sys/sysinfo.h>
#include <tk/syslib.h>
#include "tkdev_conf.h"

/*
 * Wait loop
 */
#pragma inline_asm(WaitLoop)
static void WaitLoop( UW count )
{
?_loop:
		tst		r4, r4
		bf/s	?_loop
		add		#-1, r4
}

/*
 * WaitUsec()  Compute loop count of micro second wait
 *	Suppose that it is executed by interrupt disable.
 */
EXPORT void CountWaitUsec( void )
{
	UW	cnt, sc, ec;
	UB	tstr;

	/* Stop TMU0 */
	tstr = in_b(TSTR) & ~TSTR_STR0;
	out_b(TSTR, tstr);

	/* Set TMU0 */
	out_h(TCR0, TCR_TPSC_P64);	/* Select clock/Disable interrupt */
	out_w(TCOR0, 0xffffffff);	/* Counter initial value */
	out_w(TCNT0, 0xffffffff);	/* Counter initial value */

	/* Start TMU0 count */
	out_b(TSTR, tstr | TSTR_STR0);

	/* Measure time for 110000 loops */
	sc = in_w(TCNT0);
	WaitLoop(110000);
	ec = in_w(TCNT0);
	cnt = sc - ec;

	/* Measure time for 10000 loops */
	sc = in_w(TCNT0);
	WaitLoop(10000);
	ec = in_w(TCNT0);

	/* Delete time for operation except for loops and 
	   compute the net time for 100000 loops */
	cnt -= sc - ec;

	/* Stop TMU0 */
	out_b(TSTR, tstr);

	/* Compute number of loops for 64 micro seconds
	 *			100000 loops
	 *	loop64us = ----------------------- * 64usec
	 *		    cnt * (64 dividing / Pclk)
	 *	Pclk = Peripheral clock (MHz)
	 */
	SCInfo.loop64us = SCInfo.Pclk * 100000 / cnt;
}

