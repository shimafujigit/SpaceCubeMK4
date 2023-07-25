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

/*	$NetBSD: timevar.h,v 1.23 2008/07/15 16:18:09 christos Exp $	*/

/*
 *  Copyright (c) 2005, 2008 The NetBSD Foundation.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)time.h	8.5 (Berkeley) 5/4/95
 */

#ifndef _SYS_TIMEVAR_H_
#define _SYS_TIMEVAR_H_

#include <sys/callout.h>
#ifndef T2EX
#include <sys/queue.h>
#include <sys/signal.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/systm.h>

#ifndef T2EX
/*
 * Structure used to manage timers in a process.
 */
struct 	ptimer {
	union {
		callout_t	pt_ch;
		struct {
			LIST_ENTRY(ptimer)	pt_list;
			int	pt_active;
		} pt_nonreal;
	} pt_data;
	struct	sigevent pt_ev;
	struct	itimerspec pt_time;
	struct	ksiginfo pt_info;
	int	pt_overruns;	/* Overruns currently accumulating */
	int	pt_poverruns;	/* Overruns associated w/ a delivery */
	int	pt_type;
	int	pt_entry;
	int	pt_queued;
	struct proc *pt_proc;
	TAILQ_ENTRY(ptimer) pt_chain;
};

#define pt_ch	pt_data.pt_ch
#define pt_list	pt_data.pt_nonreal.pt_list
#define pt_active	pt_data.pt_nonreal.pt_active

#define	TIMER_MAX	32	/* See ptimers->pts_fired if you enlarge this */
#define	TIMERS_ALL	0
#define	TIMERS_POSIX	1

LIST_HEAD(ptlist, ptimer);

struct	ptimers {
	struct ptlist pts_virtual;
	struct ptlist pts_prof;
	struct ptimer *pts_timers[TIMER_MAX];
	int pts_fired;
};
#endif /* T2EX */

/*
 * Functions for looking at our clock: [get]{bin,nano,micro}[up]time()
 *
 * Functions without the "get" prefix returns the best timestamp
 * we can produce in the given format.
 *
 * "bin"   == struct bintime  == seconds + 64 bit fraction of seconds.
 * "nano"  == struct timespec == seconds + nanoseconds.
 * "micro" == struct timeval  == seconds + microseconds.
 *              
 * Functions containing "up" returns time relative to boot and
 * should be used for calculating time intervals.
 *
 * Functions without "up" returns GMT time.
 *
 * Functions with the "get" prefix returns a less precise result
 * much faster than the functions without "get" prefix and should
 * be used where a precision of 1/HZ (eg 10 msec on a 100HZ machine)
 * is acceptable or where performance is priority.
 * (NB: "precision", _not_ "resolution" !) 
 * 
 */

#ifndef T2EX
void	binuptime(struct bintime *);
void	nanouptime(struct timespec *);
void	microuptime(struct timeval *);

void	bintime(struct bintime *);
void	nanotime(struct timespec *);
void	microtime(struct timeval *);

void	getbinuptime(struct bintime *);
void	getnanouptime(struct timespec *);
void	getmicrouptime(struct timeval *);

void	getbintime(struct bintime *);
void	getnanotime(struct timespec *);
void	getmicrotime(struct timeval *);
#else
void	tkn_microtime(struct timeval *);

void	tkn_getmicrouptime(struct timeval *);

void	tkn_getnanotime(struct timespec *);
void	tkn_getmicrotime(struct timeval *);

#define microtime	tkn_microtime
#define getmicrotime	tkn_getmicrotime
#define getmicrouptime	tkn_getmicrouptime
#define getnanotime	tkn_getnanotime
#endif

/* Other functions */
int	adjtime1(const struct timeval *, struct timeval *, struct proc *);
int	clock_settime1(struct proc *, clockid_t, const struct timespec *, bool);
int	dogetitimer(struct proc *, int, struct itimerval *);
int	dosetitimer(struct proc *, int, struct itimerval *);
int	dotimer_gettime(int, struct proc *, struct itimerspec *);
int	dotimer_settime(int, struct itimerspec *, struct itimerspec *, int,
	    struct proc *);
int	tshzto(const struct timespec *);
int	tvhzto(const struct timeval *);
void	inittimecounter(void);
int	itimerfix(struct timeval *);
int	itimespecfix(struct timespec *);
int	ppsratecheck(struct timeval *, int *, int);
int	ratecheck(struct timeval *, const struct timeval *);
void	realtimerexpire(void *);
int	settime(struct proc *p, struct timespec *);
int	nanosleep1(struct lwp *l, struct timespec *, struct timespec *);
int	settimeofday1(const struct timeval *, bool,
	    const void *, struct lwp *, bool);
#ifndef T2EX
int	timer_create1(timer_t *, clockid_t, struct sigevent *, copyin_t,
	    struct lwp *);
void	timer_gettime(struct ptimer *, struct itimerspec *);
void	timer_settime(struct ptimer *);
struct	ptimers *timers_alloc(struct proc *);
void	timers_free(struct proc *, int);
#endif /* T2EX */
void	timer_tick(struct lwp *, bool);
int	tstohz(const struct timespec *);
int	tvtohz(const struct timeval *);
int	inittimeleft(struct timeval *, struct timeval *);
int	gettimeleft(struct timeval *, struct timeval *);
void	timerupcall(struct lwp *);
void	time_init(void);
void	time_init2(void);

#ifndef T2EX
extern time_t time_second;	/* current second in the epoch */
extern time_t time_uptime;	/* system uptime in seconds */
#else
time_t tkn_time_second(void);
time_t tkn_time_uptime(void);
#define	time_second	tkn_time_second()
#define	time_uptime	tkn_time_uptime()
#endif

#endif /* !_SYS_TIMEVAR_H_ */
