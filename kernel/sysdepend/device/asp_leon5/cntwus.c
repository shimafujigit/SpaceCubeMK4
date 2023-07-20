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
 *	cntwus.c (ASP-LEON5)
 *	Loop Count Computation of micro Second Wait 
 */

#include <basic.h>
#include <sys/sysinfo.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include "tkdev_conf.h"

/*
 * Wait loop
 *	this codes should be the same as that in WaitNsec() and WaitUsec().
 */
LOCAL void WaitLoop( UW count )
{
#if 1
	SCInfo.loop64us = 64;
	WaitUsec(count);
#else
	Asm("	cmp	%0, 0	\n"
	    "	bgu,a	.	\n"
	    "	deccc	%0	"
	    : "+r"(count));
#endif
}

/*
 * WaitUsec()  Compute loop count of micro second wait
 *	Suppose that it is executed by interrupt disable.
 */
EXPORT void CountWaitUsec( void )
{
	volatile struct gptimer_timer_regs	*tmr;
	UW	cnt, sc, ec;

	if ( __bcc_timer_handle == 0 ) return;
	tmr = &((struct gptimer_regs*)__bcc_timer_handle)->timer[0];

	/* stop timer */
	tmr->ctrl = 0;

	/* Set timer counter */
	tmr->counter = 0;
	tmr->reload = ~0;

	/* Start counting */
	tmr->ctrl =
		GPTIMER_CTRL_IP |
		GPTIMER_CTRL_LD |
		GPTIMER_CTRL_RS |
		GPTIMER_CTRL_EN;
	WaitLoop(100);

	/* Measure time for 110000 loops  */
	sc = tmr->counter;
	WaitLoop(110000);
	ec = tmr->counter;
	cnt = sc - ec;

	/* Measure time for 10000 loops */
	sc = tmr->counter;
	WaitLoop(10000);
	ec = tmr->counter;

	/* Stop counting */
	tmr->ctrl = 0;

	/* Delete time for operation except for loops and
				compute the net time for 100000 loops */
	cnt -= sc - ec;

	/* Compute number of loops for 64 micro seconds
	 *		      100000 loops
	 *	loop64us = -------------------- * 64 usec
	 *		     cnt / GPTCLK_MHz
	 */
	SCInfo.loop64us = 100000 * 64 * GPTCLK_MHz / cnt;
}
