/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*
 *	@(#)tkn_time.c
 *
 */

/* network time and clock management */

#include <tk/tkernel.h>

#include <sys/param.h>
#include <sys/types.h>

#include <tk/dbgspt.h>

void
tkn_microtime(struct timeval *tv)
{
	SYSTIM  systim;
	UINT    nsoff;
	int64_t msec, sec, usec;

	td_get_tim(&systim, &nsoff);
	msec = systim.hi;
	msec = (msec << 32) | systim.lo;
	sec  = msec / 1000;
	msec = msec - sec * 1000;
	usec = msec * 1000 + nsoff / 1000;

	tv->tv_sec  = sec;	/* TODO: time_t overflow. */
	tv->tv_usec = usec;
}

struct timeval startup_tv;

time_t tkn_time_second(void)
{
	SYSTIM tim;

	tk_get_tim(&tim);

	return ((((uint64_t)tim.hi) << 32) | tim.lo) / 1000;
}

time_t tkn_time_uptime(void)
{
	SYSTIM tim;

	tk_get_otm(&tim);

	return ((((uint64_t)tim.hi) << 32) | tim.lo) / 1000;
}

void tkn_tc_setclock(struct timespec *ts)
{
	SYSTIM tim;

	uint64_t hilo = ts->tv_sec * 1000 + ts->tv_nsec / 1000000;
	tim.hi = hilo >> 32;
	tim.lo = hilo & 0xffffffffU;

	tk_set_tim(&tim);
}

void tkn_getmicrotime(struct timeval *tv)
{
	tkn_microtime(tv);
}

void tkn_getmicrouptime(struct timeval *tv)
{
	SYSTIM  systim;
	UINT    nsoff;
	int64_t msec, sec, usec;

	td_get_otm(&systim, &nsoff);
	msec = systim.hi;
	msec = (msec << 32) | systim.lo;
	sec  = msec / 1000;
	msec = msec - sec * 1000;
	usec = msec * 1000 + nsoff / 1000;

	tv->tv_sec  = sec;	/* TODO: time_t overflow. */
	tv->tv_usec = usec;
}

void tkn_getnanotime(struct timespec *ts)
{
	SYSTIM tim;
	UINT ofs;

	td_get_tim(&tim, &ofs);

	ts->tv_sec = (((uint64_t)tim.hi << 32)  | tim.lo) / 1000;
	ts->tv_nsec = ofs;
}

