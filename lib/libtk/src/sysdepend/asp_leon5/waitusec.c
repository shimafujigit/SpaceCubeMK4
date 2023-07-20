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
 *	@(#)waitusec.c (libtk/ASP-LEON5)
 *
 *	Busy loop wait time in micro-sec 
 */

#include <basic.h>
#include <sys/sysinfo.h>

/*
 * Wait loop
 */
EXPORT void WaitUsec( UINT usec )
{
	UW	count = usec * SCInfo.loop64us / 64U;

	Asm("	cmp	%0, 0	\n"
	    "	bgu,a	.	\n"
	    "	deccc	%0	"
	    : "+r"(count));
}
