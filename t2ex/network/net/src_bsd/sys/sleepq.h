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

/*	$NetBSD: sleepq.h,v 1.15 2008/10/10 09:44:35 pooka Exp $	*/

/*-
 * Copyright (c) 2002, 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe and Andrew Doran.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	_SYS_SLEEPQ_H_
#define	_SYS_SLEEPQ_H_

#include <sys/lwp.h>
#include <sys/mutex.h>
#include <sys/pool.h>
#ifndef T2EX
#include <sys/queue.h>
#include <sys/sched.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/syncobj.h>

/*
 * Generic sleep queues.
 */

#ifndef T2EX
#define	SLEEPTAB_HASH_SHIFT	7
#else
#define	SLEEPTAB_HASH_SHIFT	0
#endif
#define	SLEEPTAB_HASH_SIZE	(1 << SLEEPTAB_HASH_SHIFT)
#define	SLEEPTAB_HASH_MASK	(SLEEPTAB_HASH_SIZE - 1)
#define	SLEEPTAB_HASH(wchan)	(((uintptr_t)(wchan) >> 8) & SLEEPTAB_HASH_MASK)

TAILQ_HEAD(sleepq, lwp);

typedef struct sleepq sleepq_t;

#ifdef _LP64
typedef struct sleeptab {
	struct {
		kmutex_t	st_mutex;
		sleepq_t	st_queue __aligned(32);
	} st_queues[SLEEPTAB_HASH_SIZE];
} __aligned(64) sleeptab_t;
#else	/* _LP64 */
typedef struct sleeptab {
	struct {
		kmutex_t	st_mutex;
		sleepq_t	st_queue;
	} st_queues[SLEEPTAB_HASH_SIZE];
} __aligned(32) sleeptab_t;
#endif	/* _LP64 */

#ifndef T2EX
void	sleepq_init(sleepq_t *);
#else
int	sleepq_init(sleepq_t *);
void	sleepq_finish(sleepq_t *);
#endif
int	sleepq_remove(sleepq_t *, lwp_t *);
void	sleepq_enqueue(sleepq_t *, wchan_t, const char *, syncobj_t *);
u_int	sleepq_unsleep(lwp_t *, bool);
void	sleepq_timeout(void *);
lwp_t	*sleepq_wake(sleepq_t *, wchan_t, u_int, kmutex_t *);
int	sleepq_abort(kmutex_t *, int);
void	sleepq_changepri(lwp_t *, pri_t);
void	sleepq_lendpri(lwp_t *, pri_t);
int	sleepq_block(int, bool);
void	sleepq_insert(sleepq_t *, lwp_t *, syncobj_t *);

#ifndef T2EX
void	sleeptab_init(sleeptab_t *);
#else
int	sleeptab_init(sleeptab_t *);
int	sleeptab_finish(sleeptab_t *);
#endif

extern sleeptab_t	sleeptab;

/*
 * Return non-zero if it is unsafe to sleep.
 *
 * XXX This only exists because panic() is broken.
 */
static inline bool
sleepq_dontsleep(lwp_t *l)
{
#ifndef T2EX
	extern int cold;

	return cold || (doing_shutdown && (panicstr || CURCPU_IDLE_P()));
#else
	(void)l;
	return (doing_shutdown && panicstr);
#endif
}

/*
 * Find the correct sleep queue for the the specified wait channel.  This
 * acquires and holds the per-queue interlock.
 */
static inline sleepq_t *
sleeptab_lookup(sleeptab_t *st, wchan_t wchan, kmutex_t **mp)
{
	sleepq_t *sq;

	sq = &st->st_queues[SLEEPTAB_HASH(wchan)].st_queue;
	*mp = &st->st_queues[SLEEPTAB_HASH(wchan)].st_mutex;
	mutex_spin_enter(*mp);
	return sq;
}

static inline kmutex_t *
sleepq_hashlock(wchan_t wchan)
{
	kmutex_t *mp;

	mp = &sleeptab.st_queues[SLEEPTAB_HASH(wchan)].st_mutex;
	mutex_spin_enter(mp);
	return mp;
}

/*
 * Prepare to block on a sleep queue, after which any interlock can be
 * safely released.
 */
static inline void
sleepq_enter(sleepq_t *sq, lwp_t *l, kmutex_t *mp)
{
	kmutex_t *omp;

#ifdef T2EX
	(void)sq;
#endif

	/*
	 * Acquire the per-LWP mutex and lend it ours (the sleep queue
	 * lock).  Once that's done we're interlocked, and so can release
	 * the kernel lock.
	 */
	omp = l->l_mutex;
	mutex_spin_enter(omp);
	if (__predict_false(l->l_mutex != omp)) {
		omp = lwp_lock_retry(l, omp);
	}
	l->l_mutex = mp;
	mutex_spin_exit(omp);
#ifndef T2EX
	KERNEL_UNLOCK_ALL(NULL, &l->l_biglocks);
#endif
}

/*
 * Turnstiles, specialized sleep queues for use by kernel locks.
 */

typedef struct turnstile {
	LIST_ENTRY(turnstile)	ts_chain;	/* link on hash chain */
	struct turnstile	*ts_free;	/* turnstile free list */
	wchan_t			ts_obj;		/* lock object */
	sleepq_t		ts_sleepq[2];	/* sleep queues */
	u_int			ts_waiters[2];	/* count of waiters */

	/* priority inheritance */
	pri_t			ts_eprio;
	lwp_t			*ts_inheritor;
	SLIST_ENTRY(turnstile)	ts_pichain;
} turnstile_t;

typedef struct tschain {
	kmutex_t		tc_mutex;	/* mutex on structs & queues */
	LIST_HEAD(, turnstile)	tc_chain;	/* turnstile chain */
} __aligned(32) tschain_t;

#define	TS_READER_Q	0		/* reader sleep queue */
#define	TS_WRITER_Q	1		/* writer sleep queue */

#define	TS_WAITERS(ts, q)						\
	(ts)->ts_waiters[(q)]

#define	TS_ALL_WAITERS(ts)						\
	((ts)->ts_waiters[TS_READER_Q] +				\
	 (ts)->ts_waiters[TS_WRITER_Q])

#define	TS_FIRST(ts, q)	(TAILQ_FIRST(&(ts)->ts_sleepq[(q)]))

#ifdef	_KERNEL

void	turnstile_init(void);
turnstile_t	*turnstile_lookup(wchan_t);
void	turnstile_exit(wchan_t);
void	turnstile_block(turnstile_t *, int, wchan_t, syncobj_t *);
void	turnstile_wakeup(turnstile_t *, int, int, lwp_t *);
void	turnstile_print(volatile void *, void (*)(const char *, ...));
u_int	turnstile_unsleep(lwp_t *, bool);
void	turnstile_changepri(lwp_t *, pri_t);

extern pool_cache_t turnstile_cache;
extern turnstile_t turnstile0;

#endif	/* _KERNEL */

#endif	/* _SYS_SLEEPQ_H_ */
