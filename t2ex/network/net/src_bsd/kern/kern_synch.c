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

/*	$NetBSD: kern_synch.c,v 1.254.2.6 2009/04/23 17:47:13 snj Exp $	*/

/*-
 * Copyright (c) 1999, 2000, 2004, 2006, 2007, 2008, 2009
 *    The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center, by Charles M. Hannum, Andrew Doran and
 * Daniel Sieger.
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

/*-
 * Copyright (c) 1982, 1986, 1990, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)kern_synch.c	8.9 (Berkeley) 5/19/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: kern_synch.c,v 1.254.2.6 2009/04/23 17:47:13 snj Exp $");

#ifndef T2EX
#include "opt_kstack.h"
#include "opt_perfctrs.h"
#include "opt_sa.h"
#endif

#define	__MUTEX_PRIVATE

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/kernel.h>
#if defined(PERFCTRS)
#include <sys/pmc.h>
#endif
#ifndef T2EX
#include <sys/cpu.h>
#include <sys/resourcevar.h>
#include <sys/sched.h>
#include <sys/sa.h>
#include <sys/savar.h>
#include <sys/syscall_stats.h>
#endif
#include <sys/sleepq.h>
#include <sys/lockdebug.h>
#include <sys/evcnt.h>
#include <sys/intr.h>
#ifndef T2EX
#include <sys/lwpctl.h>
#include <sys/atomic.h>
#else
#include <sys/_atomic.h>
#endif
#include <sys/simplelock.h>

#ifndef T2EX
#include <uvm/uvm_extern.h>

#include <dev/lockstat.h>

static u_int	sched_unsleep(struct lwp *, bool);
static void	sched_changepri(struct lwp *, pri_t);
static void	sched_lendpri(struct lwp *, pri_t);
static void	resched_cpu(struct lwp *);

syncobj_t sleep_syncobj = {
	SOBJ_SLEEPQ_SORTED,
	sleepq_unsleep,
	sleepq_changepri,
	sleepq_lendpri,
	syncobj_noowner,
};

syncobj_t sched_syncobj = {
	SOBJ_SLEEPQ_SORTED,
	sched_unsleep,
	sched_changepri,
	sched_lendpri,
	syncobj_noowner,
};

callout_t 	sched_pstats_ch;
unsigned	sched_pstats_ticks;
kcondvar_t	lbolt;			/* once a second sleep address */

/* Preemption event counters */
static struct evcnt kpreempt_ev_crit;
static struct evcnt kpreempt_ev_klock;
static struct evcnt kpreempt_ev_immed;

/*
 * During autoconfiguration or after a panic, a sleep will simply lower the
 * priority briefly to allow interrupts, then return.  The priority to be
 * used (safepri) is machine-dependent, thus this value is initialized and
 * maintained in the machine-dependent layers.  This priority will typically
 * be 0, or the lowest priority that is safe for use on the interrupt stack;
 * it can be made higher to block network software interrupts after panics.
 */
int	safepri;

void
sched_init(void)
{

	cv_init(&lbolt, "lbolt");
	callout_init(&sched_pstats_ch, CALLOUT_MPSAFE);
	callout_setfunc(&sched_pstats_ch, sched_pstats, NULL);

	evcnt_attach_dynamic(&kpreempt_ev_crit, EVCNT_TYPE_MISC, NULL,
	   "kpreempt", "defer: critical section");
	evcnt_attach_dynamic(&kpreempt_ev_klock, EVCNT_TYPE_MISC, NULL,
	   "kpreempt", "defer: kernel_lock");
	evcnt_attach_dynamic(&kpreempt_ev_immed, EVCNT_TYPE_MISC, NULL,
	   "kpreempt", "immediate");

	sched_pstats(NULL);
}
#endif

/*
 * OBSOLETE INTERFACE
 *
 * General sleep call.  Suspends the current process until a wakeup is
 * performed on the specified identifier.  The process will then be made
 * runnable with the specified priority.  Sleeps at most timo/hz seconds (0
 * means no timeout).  If pri includes PCATCH flag, signals are checked
 * before and after sleeping, else signals are not checked.  Returns 0 if
 * awakened, EWOULDBLOCK if the timeout expires.  If PCATCH is set and a
 * signal needs to be delivered, ERESTART is returned if the current system
 * call should be restarted if possible, and EINTR is returned if the system
 * call should be interrupted by the signal (return EINTR).
 *
 * The interlock is held until we are on a sleep queue. The interlock will
 * be locked before returning back to the caller unless the PNORELOCK flag
 * is specified, in which case the interlock will always be unlocked upon
 * return.
 */
int
ltsleep(wchan_t ident, pri_t priority, const char *wmesg, int timo,
	volatile struct simplelock *interlock)
{
	struct lwp *l = curlwp;
	sleepq_t *sq;
	kmutex_t *mp;
	int error;

	KASSERT((l->l_pflag & LP_INTR) == 0);

	if (sleepq_dontsleep(l)) {
		(void)sleepq_abort(NULL, 0);
		if ((priority & PNORELOCK) != 0)
			simple_unlock(interlock);
		return 0;
	}

#ifndef T2EX
	l->l_kpriority = true;
#endif
	sq = sleeptab_lookup(&sleeptab, ident, &mp);
	sleepq_enter(sq, l, mp);
	sleepq_enqueue(sq, ident, wmesg, &sleep_syncobj);

	if (interlock != NULL) {
		KASSERT(simple_lock_held(interlock));
		simple_unlock(interlock);
	}

	error = sleepq_block(timo, priority & PCATCH);

	if (interlock != NULL && (priority & PNORELOCK) == 0)
		simple_lock(interlock);
 
	return error;
}

#ifndef T2EX
int
mtsleep(wchan_t ident, pri_t priority, const char *wmesg, int timo,
	kmutex_t *mtx)
{
	struct lwp *l = curlwp;
	sleepq_t *sq;
	kmutex_t *mp;
	int error;

	KASSERT((l->l_pflag & LP_INTR) == 0);

	if (sleepq_dontsleep(l)) {
		(void)sleepq_abort(mtx, (priority & PNORELOCK) != 0);
		return 0;
	}

	l->l_kpriority = true;
	sq = sleeptab_lookup(&sleeptab, ident, &mp);
	sleepq_enter(sq, l, mp);
	sleepq_enqueue(sq, ident, wmesg, &sleep_syncobj);
	mutex_exit(mtx);
	error = sleepq_block(timo, priority & PCATCH);

	if ((priority & PNORELOCK) == 0)
		mutex_enter(mtx);
 
	return error;
}

/*
 * General sleep call for situations where a wake-up is not expected.
 */
int
kpause(const char *wmesg, bool intr, int timo, kmutex_t *mtx)
{
	struct lwp *l = curlwp;
	kmutex_t *mp;
	sleepq_t *sq;
	int error;

	if (sleepq_dontsleep(l))
		return sleepq_abort(NULL, 0);

	if (mtx != NULL)
		mutex_exit(mtx);
	l->l_kpriority = true;
	sq = sleeptab_lookup(&sleeptab, l, &mp);
	sleepq_enter(sq, l, mp);
	sleepq_enqueue(sq, l, wmesg, &sleep_syncobj);
	error = sleepq_block(timo, intr);
	if (mtx != NULL)
		mutex_enter(mtx);

	return error;
}

#ifdef KERN_SA
/*
 * sa_awaken:
 *
 *	We believe this lwp is an SA lwp. If it's yielding,
 * let it know it needs to wake up.
 *
 *	We are called and exit with the lwp locked. We are
 * called in the middle of wakeup operations, so we need
 * to not touch the locks at all.
 */
void
sa_awaken(struct lwp *l)
{
	/* LOCK_ASSERT(lwp_locked(l, NULL)); */

	if (l == l->l_savp->savp_lwp && l->l_flag & LW_SA_YIELD)
		l->l_flag &= ~LW_SA_IDLE;
}
#endif /* KERN_SA */
#endif

/*
 * OBSOLETE INTERFACE
 *
 * Make all processes sleeping on the specified identifier runnable.
 */
void
wakeup(wchan_t ident)
{
	sleepq_t *sq;
	kmutex_t *mp;

	if (cold)
		return;

	sq = sleeptab_lookup(&sleeptab, ident, &mp);
	sleepq_wake(sq, ident, (u_int)-1, mp);
}

#ifndef T2EX
/*
 * OBSOLETE INTERFACE
 *
 * Make the highest priority process first in line on the specified
 * identifier runnable.
 */
void 
wakeup_one(wchan_t ident)
{
	sleepq_t *sq;
	kmutex_t *mp;

	if (cold)
		return;

	sq = sleeptab_lookup(&sleeptab, ident, &mp);
	sleepq_wake(sq, ident, 1, mp);
}


/*
 * General yield call.  Puts the current process back on its run queue and
 * performs a voluntary context switch.  Should only be called when the
 * current process explicitly requests it (eg sched_yield(2)).
 */
void
yield(void)
{
	struct lwp *l = curlwp;

	KERNEL_UNLOCK_ALL(l, &l->l_biglocks);
	lwp_lock(l);
	KASSERT(lwp_locked(l, l->l_cpu->ci_schedstate.spc_lwplock));
	KASSERT(l->l_stat == LSONPROC);
	l->l_kpriority = false;
	(void)mi_switch(l);
	KERNEL_LOCK(l->l_biglocks, l);
}

/*
 * General preemption call.  Puts the current process back on its run queue
 * and performs an involuntary context switch.
 */
void
preempt(void)
{
	struct lwp *l = curlwp;

	KERNEL_UNLOCK_ALL(l, &l->l_biglocks);
	lwp_lock(l);
	KASSERT(lwp_locked(l, l->l_cpu->ci_schedstate.spc_lwplock));
	KASSERT(l->l_stat == LSONPROC);
	l->l_kpriority = false;
	l->l_nivcsw++;
	(void)mi_switch(l);
	KERNEL_LOCK(l->l_biglocks, l);
}

/*
 * Handle a request made by another agent to preempt the current LWP
 * in-kernel.  Usually called when l_dopreempt may be non-zero.
 *
 * Character addresses for lockstat only.
 */
static char	in_critical_section;
static char	kernel_lock_held;
static char	is_softint;
static char	cpu_kpreempt_enter_fail;

bool
kpreempt(uintptr_t where)
{
	uintptr_t failed;
	lwp_t *l;
	int s, dop;

	l = curlwp;
	failed = 0;
	while ((dop = l->l_dopreempt) != 0) {
		if (l->l_stat != LSONPROC) {
			/*
			 * About to block (or die), let it happen.
			 * Doesn't really count as "preemption has
			 * been blocked", since we're going to
			 * context switch.
			 */
			l->l_dopreempt = 0;
			return true;
		}
		if (__predict_false((l->l_flag & LW_IDLE) != 0)) {
			/* Can't preempt idle loop, don't count as failure. */
		    	l->l_dopreempt = 0;
		    	return true;
		}
		if (__predict_false(l->l_nopreempt != 0)) {
			/* LWP holds preemption disabled, explicitly. */
			if ((dop & DOPREEMPT_COUNTED) == 0) {
				kpreempt_ev_crit.ev_count++;
			}
			failed = (uintptr_t)&in_critical_section;
			break;
		}
		if (__predict_false((l->l_pflag & LP_INTR) != 0)) {
		    	/* Can't preempt soft interrupts yet. */
		    	l->l_dopreempt = 0;
		    	failed = (uintptr_t)&is_softint;
		    	break;
		}
		s = splsched();
		if (__predict_false(l->l_blcnt != 0 ||
		    curcpu()->ci_biglock_wanted != NULL)) {
			/* Hold or want kernel_lock, code is not MT safe. */
			splx(s);
			if ((dop & DOPREEMPT_COUNTED) == 0) {
				kpreempt_ev_klock.ev_count++;
			}
			failed = (uintptr_t)&kernel_lock_held;
			break;
		}
		if (__predict_false(!cpu_kpreempt_enter(where, s))) {
			/*
			 * It may be that the IPL is too high.
			 * kpreempt_enter() can schedule an
			 * interrupt to retry later.
			 */
			splx(s);
			failed = (uintptr_t)&cpu_kpreempt_enter_fail;
			break;
		}
		/* Do it! */
		if (__predict_true((dop & DOPREEMPT_COUNTED) == 0)) {
			kpreempt_ev_immed.ev_count++;
		}
		lwp_lock(l);
		mi_switch(l);
		l->l_nopreempt++;
		splx(s);

		/* Take care of any MD cleanup. */
		cpu_kpreempt_exit(where);
		l->l_nopreempt--;
	}

	/* Record preemption failure for reporting via lockstat. */
	if (__predict_false(failed)) {
		int lsflag = 0;
		atomic_or_uint(&l->l_dopreempt, DOPREEMPT_COUNTED);
		LOCKSTAT_ENTER(lsflag);
		/* Might recurse, make it atomic. */
		if (__predict_false(lsflag)) {
			if (where == 0) {
				where = (uintptr_t)__builtin_return_address(0);
			}
			if (atomic_cas_ptr_ni((void *)&l->l_pfailaddr,
			    NULL, (void *)where) == NULL) {
				LOCKSTAT_START_TIMER(lsflag, l->l_pfailtime);
				l->l_pfaillock = failed;
			}
		}
		LOCKSTAT_EXIT(lsflag);
	}

	return failed;
}

/*
 * Return true if preemption is explicitly disabled.
 */
bool
kpreempt_disabled(void)
{
	lwp_t *l;

	l = curlwp;

	return l->l_nopreempt != 0 || l->l_stat == LSZOMB ||
	    (l->l_flag & LW_IDLE) != 0 || cpu_kpreempt_disabled();
}

/*
 * Disable kernel preemption.
 */
void
kpreempt_disable(void)
{

	KPREEMPT_DISABLE(curlwp);
}

/*
 * Reenable kernel preemption.
 */
void
kpreempt_enable(void)
{

	KPREEMPT_ENABLE(curlwp);
}

/*
 * Compute the amount of time during which the current lwp was running.
 *
 * - update l_rtime unless it's an idle lwp.
 */

void
updatertime(lwp_t *l, const struct bintime *now)
{

	if ((l->l_flag & LW_IDLE) != 0)
		return;

	/* rtime += now - stime */
	bintime_add(&l->l_rtime, now);
	bintime_sub(&l->l_rtime, &l->l_stime);
}

/*
 * Select next LWP from the current CPU to run..
 */
static inline lwp_t *
nextlwp(struct cpu_info *ci, struct schedstate_percpu *spc)
{
	lwp_t *newl;

	/*
	 * Let sched_nextlwp() select the LWP to run the CPU next.
	 * If no LWP is runnable, select the idle LWP.
	 * 
	 * Note that spc_lwplock might not necessary be held, and
	 * new thread would be unlocked after setting the LWP-lock.
	 */
	newl = sched_nextlwp();
	if (newl != NULL) {
		sched_dequeue(newl);
		KASSERT(lwp_locked(newl, spc->spc_mutex));
		newl->l_stat = LSONPROC;
		newl->l_cpu = ci;
		newl->l_pflag |= LP_RUNNING;
		lwp_setlock(newl, spc->spc_lwplock);
	} else {
		newl = ci->ci_data.cpu_idlelwp;
		newl->l_stat = LSONPROC;
		newl->l_pflag |= LP_RUNNING;
	}
	
	/*
	 * Only clear want_resched if there are no pending (slow)
	 * software interrupts.
	 */
	ci->ci_want_resched = ci->ci_data.cpu_softints;
	spc->spc_flags &= ~SPCF_SWITCHCLEAR;
	spc->spc_curpriority = lwp_eprio(newl);

	return newl;
}

/*
 * The machine independent parts of context switch.
 *
 * Returns 1 if another LWP was actually run.
 */
int
mi_switch(lwp_t *l)
{
	struct cpu_info *ci;
	struct schedstate_percpu *spc;
	struct lwp *newl;
	int retval, oldspl;
	struct bintime bt;
	bool returning;

	KASSERT(lwp_locked(l, NULL));
	KASSERT(kpreempt_disabled());
	LOCKDEBUG_BARRIER(l->l_mutex, 1);

#ifdef KSTACK_CHECK_MAGIC
	kstack_check_magic(l);
#endif

	binuptime(&bt);

	KASSERT(l->l_cpu == curcpu());
	ci = l->l_cpu;
	spc = &ci->ci_schedstate;
	returning = false;
	newl = NULL;

	/*
	 * If we have been asked to switch to a specific LWP, then there
	 * is no need to inspect the run queues.  If a soft interrupt is
	 * blocking, then return to the interrupted thread without adjusting
	 * VM context or its start time: neither have been changed in order
	 * to take the interrupt.
	 */
	if (l->l_switchto != NULL) {
		if ((l->l_pflag & LP_INTR) != 0) {
			returning = true;
			softint_block(l);
			if ((l->l_pflag & LP_TIMEINTR) != 0)
				updatertime(l, &bt);
		}
		newl = l->l_switchto;
		l->l_switchto = NULL;
	}
#ifndef __HAVE_FAST_SOFTINTS
	else if (ci->ci_data.cpu_softints != 0) {
		/* There are pending soft interrupts, so pick one. */
		newl = softint_picklwp();
		newl->l_stat = LSONPROC;
		newl->l_pflag |= LP_RUNNING;
	}
#endif	/* !__HAVE_FAST_SOFTINTS */

	/* Count time spent in current system call */
	if (!returning) {
		SYSCALL_TIME_SLEEP(l);

		/*
		 * XXXSMP If we are using h/w performance counters,
		 * save context.
		 */
#if PERFCTRS
		if (PMC_ENABLED(l->l_proc)) {
			pmc_save_context(l->l_proc);
		}
#endif
		updatertime(l, &bt);
	}

	/* Lock the runqueue */
	KASSERT(l->l_stat != LSRUN);
	mutex_spin_enter(spc->spc_mutex);

	/*
	 * If on the CPU and we have gotten this far, then we must yield.
	 */
	if (l->l_stat == LSONPROC && l != newl) {
		KASSERT(lwp_locked(l, spc->spc_lwplock));
		if ((l->l_flag & LW_IDLE) == 0) {
			l->l_stat = LSRUN;
			lwp_setlock(l, spc->spc_mutex);
			sched_enqueue(l, true);
			/* Handle migration case */
			KASSERT(spc->spc_migrating == NULL);
			if (l->l_target_cpu !=  NULL) {
				spc->spc_migrating = l;
			}
		} else
			l->l_stat = LSIDL;
	}

	/* Pick new LWP to run. */
	if (newl == NULL) {
		newl = nextlwp(ci, spc);
	}

	/* Items that must be updated with the CPU locked. */
	if (!returning) {
		/* Update the new LWP's start time. */
		newl->l_stime = bt;

		/*
		 * ci_curlwp changes when a fast soft interrupt occurs.
		 * We use cpu_onproc to keep track of which kernel or
		 * user thread is running 'underneath' the software
		 * interrupt.  This is important for time accounting,
		 * itimers and forcing user threads to preempt (aston).
		 */
		ci->ci_data.cpu_onproc = newl;
	}

	/*
	 * Preemption related tasks.  Must be done with the current
	 * CPU locked.
	 */
	cpu_did_resched(l);
	l->l_dopreempt = 0;
	if (__predict_false(l->l_pfailaddr != 0)) {
		LOCKSTAT_FLAG(lsflag);
		LOCKSTAT_ENTER(lsflag);
		LOCKSTAT_STOP_TIMER(lsflag, l->l_pfailtime);
		LOCKSTAT_EVENT_RA(lsflag, l->l_pfaillock, LB_NOPREEMPT|LB_SPIN,
		    1, l->l_pfailtime, l->l_pfailaddr);
		LOCKSTAT_EXIT(lsflag);
		l->l_pfailtime = 0;
		l->l_pfaillock = 0;
		l->l_pfailaddr = 0;
	}

	if (l != newl) {
		struct lwp *prevlwp;

		/* Release all locks, but leave the current LWP locked */
		if (l->l_mutex == spc->spc_mutex) {
			/*
			 * Drop spc_lwplock, if the current LWP has been moved
			 * to the run queue (it is now locked by spc_mutex).
			 */
			mutex_spin_exit(spc->spc_lwplock);
		} else {
			/*
			 * Otherwise, drop the spc_mutex, we are done with the
			 * run queues.
			 */
			mutex_spin_exit(spc->spc_mutex);
		}

		/*
		 * Mark that context switch is going to be performed
		 * for this LWP, to protect it from being switched
		 * to on another CPU.
		 */
		KASSERT(l->l_ctxswtch == 0);
		l->l_ctxswtch = 1;
		l->l_ncsw++;
		l->l_pflag &= ~LP_RUNNING;

		/*
		 * Increase the count of spin-mutexes before the release
		 * of the last lock - we must remain at IPL_SCHED during
		 * the context switch.
		 */
		oldspl = MUTEX_SPIN_OLDSPL(ci);
		ci->ci_mtx_count--;
		lwp_unlock(l);

		/* Count the context switch on this CPU. */
		ci->ci_data.cpu_nswtch++;

		/* Update status for lwpctl, if present. */
		if (l->l_lwpctl != NULL)
			l->l_lwpctl->lc_curcpu = LWPCTL_CPU_NONE;

		/*
		 * Save old VM context, unless a soft interrupt
		 * handler is blocking.
		 */
		if (!returning)
			pmap_deactivate(l);

		/*
		 * We may need to spin-wait for if 'newl' is still
		 * context switching on another CPU.
		 */
		if (newl->l_ctxswtch != 0) {
			u_int count;
			count = SPINLOCK_BACKOFF_MIN;
			while (newl->l_ctxswtch)
				SPINLOCK_BACKOFF(count);
		}

		/* Switch to the new LWP.. */
		prevlwp = cpu_switchto(l, newl, returning);
		ci = curcpu();

		/*
		 * Switched away - we have new curlwp.
		 * Restore VM context and IPL.
		 */
		pmap_activate(l);
		if (prevlwp != NULL) {
			/* Normalize the count of the spin-mutexes */
			ci->ci_mtx_count++;
			/* Unmark the state of context switch */
			membar_exit();
			prevlwp->l_ctxswtch = 0;
		}

		/* Update status for lwpctl, if present. */
		if (l->l_lwpctl != NULL) {
			l->l_lwpctl->lc_curcpu = (int)cpu_index(ci);
			l->l_lwpctl->lc_pctr++;
		}

		KASSERT(l->l_cpu == ci);
		splx(oldspl);
		retval = 1;
	} else {
		/* Nothing to do - just unlock and return. */
		mutex_spin_exit(spc->spc_mutex);
		lwp_unlock(l);
		retval = 0;
	}

	KASSERT(l == curlwp);
	KASSERT(l->l_stat == LSONPROC);

	/*
	 * XXXSMP If we are using h/w performance counters, restore context.
	 * XXXSMP preemption problem.
	 */
#if PERFCTRS
	if (PMC_ENABLED(l->l_proc)) {
		pmc_restore_context(l->l_proc);
	}
#endif
	SYSCALL_TIME_WAKEUP(l);
	LOCKDEBUG_BARRIER(NULL, 1);

	return retval;
}

/*
 * The machine independent parts of context switch to oblivion.
 * Does not return.  Call with the LWP unlocked.
 */
void
lwp_exit_switchaway(lwp_t *l)
{
	struct cpu_info *ci;
	struct lwp *newl;
	struct bintime bt;

	ci = l->l_cpu;

	KASSERT(kpreempt_disabled());
	KASSERT(l->l_stat == LSZOMB || l->l_stat == LSIDL);
	KASSERT(ci == curcpu());
	LOCKDEBUG_BARRIER(NULL, 0);

#ifdef KSTACK_CHECK_MAGIC
	kstack_check_magic(l);
#endif

	/* Count time spent in current system call */
	SYSCALL_TIME_SLEEP(l);
	binuptime(&bt);
	updatertime(l, &bt);

	/* Must stay at IPL_SCHED even after releasing run queue lock. */
	(void)splsched();

	/*
	 * Let sched_nextlwp() select the LWP to run the CPU next.
	 * If no LWP is runnable, select the idle LWP.
	 * 
	 * Note that spc_lwplock might not necessary be held, and
	 * new thread would be unlocked after setting the LWP-lock.
	 */
	spc_lock(ci);
#ifndef __HAVE_FAST_SOFTINTS
	if (ci->ci_data.cpu_softints != 0) {
		/* There are pending soft interrupts, so pick one. */
		newl = softint_picklwp();
		newl->l_stat = LSONPROC;
		newl->l_pflag |= LP_RUNNING;
	} else 
#endif	/* !__HAVE_FAST_SOFTINTS */
	{
		newl = nextlwp(ci, &ci->ci_schedstate);
	}

	/* Update the new LWP's start time. */
	newl->l_stime = bt;
	l->l_pflag &= ~LP_RUNNING;

	/*
	 * ci_curlwp changes when a fast soft interrupt occurs.
	 * We use cpu_onproc to keep track of which kernel or
	 * user thread is running 'underneath' the software
	 * interrupt.  This is important for time accounting,
	 * itimers and forcing user threads to preempt (aston).
	 */
	ci->ci_data.cpu_onproc = newl;

	/*
	 * Preemption related tasks.  Must be done with the current
	 * CPU locked.
	 */
	cpu_did_resched(l);

	/* Unlock the run queue. */
	spc_unlock(ci);

	/* Count the context switch on this CPU. */
	ci->ci_data.cpu_nswtch++;

	/* Update status for lwpctl, if present. */
	if (l->l_lwpctl != NULL)
		l->l_lwpctl->lc_curcpu = LWPCTL_CPU_EXITED;

	/*
	 * We may need to spin-wait for if 'newl' is still
	 * context switching on another CPU.
	 */
	if (newl->l_ctxswtch != 0) {
		u_int count;
		count = SPINLOCK_BACKOFF_MIN;
		while (newl->l_ctxswtch)
			SPINLOCK_BACKOFF(count);
	}

	/* Switch to the new LWP.. */
	(void)cpu_switchto(NULL, newl, false);

	for (;;) continue;	/* XXX: convince gcc about "noreturn" */
	/* NOTREACHED */
}

/*
 * Change process state to be runnable, placing it on the run queue if it is
 * in memory, and awakening the swapper if it isn't in memory.
 *
 * Call with the process and LWP locked.  Will return with the LWP unlocked.
 */
void
setrunnable(struct lwp *l)
{
	struct proc *p = l->l_proc;
	struct cpu_info *ci;

	KASSERT((l->l_flag & LW_IDLE) == 0);
	KASSERT(mutex_owned(p->p_lock));
	KASSERT(lwp_locked(l, NULL));
	KASSERT(l->l_mutex != l->l_cpu->ci_schedstate.spc_mutex);

	switch (l->l_stat) {
	case LSSTOP:
		/*
		 * If we're being traced (possibly because someone attached us
		 * while we were stopped), check for a signal from the debugger.
		 */
		if ((p->p_slflag & PSL_TRACED) != 0 && p->p_xstat != 0)
			signotify(l);
		p->p_nrlwps++;
		break;
	case LSSUSPENDED:
		l->l_flag &= ~LW_WSUSPEND;
		p->p_nrlwps++;
		cv_broadcast(&p->p_lwpcv);
		break;
	case LSSLEEP:
		KASSERT(l->l_wchan != NULL);
		break;
	default:
		panic("setrunnable: lwp %p state was %d", l, l->l_stat);
	}

#ifdef KERN_SA
	if (l->l_proc->p_sa)
		sa_awaken(l);
#endif /* KERN_SA */

	/*
	 * If the LWP was sleeping interruptably, then it's OK to start it
	 * again.  If not, mark it as still sleeping.
	 */
	if (l->l_wchan != NULL) {
		l->l_stat = LSSLEEP;
		/* lwp_unsleep() will release the lock. */
		lwp_unsleep(l, true);
		return;
	}

	/*
	 * If the LWP is still on the CPU, mark it as LSONPROC.  It may be
	 * about to call mi_switch(), in which case it will yield.
	 */
	if ((l->l_pflag & LP_RUNNING) != 0) {
		l->l_stat = LSONPROC;
		l->l_slptime = 0;
		lwp_unlock(l);
		return;
	}

	/*
	 * Look for a CPU to run.
	 * Set the LWP runnable.
	 */
	ci = sched_takecpu(l);
	l->l_cpu = ci;
	spc_lock(ci);
	lwp_unlock_to(l, ci->ci_schedstate.spc_mutex);
	sched_setrunnable(l);
	l->l_stat = LSRUN;
	l->l_slptime = 0;

	/*
	 * If thread is swapped out - wake the swapper to bring it back in.
	 * Otherwise, enter it into a run queue.
	 */
	if (l->l_flag & LW_INMEM) {
		sched_enqueue(l, false);
		resched_cpu(l);
		lwp_unlock(l);
	} else {
		lwp_unlock(l);
		uvm_kick_scheduler();
	}
}

/*
 * suspendsched:
 *
 *	Convert all non-L_SYSTEM LSSLEEP or LSRUN LWPs to LSSUSPENDED. 
 */
void
suspendsched(void)
{
	CPU_INFO_ITERATOR cii;
	struct cpu_info *ci;
	struct lwp *l;
	struct proc *p;

	/*
	 * We do this by process in order not to violate the locking rules.
	 */
	mutex_enter(proc_lock);
	PROCLIST_FOREACH(p, &allproc) {
		if ((p->p_flag & PK_MARKER) != 0)
			continue;

		mutex_enter(p->p_lock);
		if ((p->p_flag & PK_SYSTEM) != 0) {
			mutex_exit(p->p_lock);
			continue;
		}

		p->p_stat = SSTOP;

		LIST_FOREACH(l, &p->p_lwps, l_sibling) {
			if (l == curlwp)
				continue;

			lwp_lock(l);

			/*
			 * Set L_WREBOOT so that the LWP will suspend itself
			 * when it tries to return to user mode.  We want to
			 * try and get to get as many LWPs as possible to
			 * the user / kernel boundary, so that they will
			 * release any locks that they hold.
			 */
			l->l_flag |= (LW_WREBOOT | LW_WSUSPEND);

			if (l->l_stat == LSSLEEP &&
			    (l->l_flag & LW_SINTR) != 0) {
				/* setrunnable() will release the lock. */
				setrunnable(l);
				continue;
			}

			lwp_unlock(l);
		}

		mutex_exit(p->p_lock);
	}
	mutex_exit(proc_lock);

	/*
	 * Kick all CPUs to make them preempt any LWPs running in user mode. 
	 * They'll trap into the kernel and suspend themselves in userret().
	 */
	for (CPU_INFO_FOREACH(cii, ci)) {
		spc_lock(ci);
		cpu_need_resched(ci, RESCHED_IMMED);
		spc_unlock(ci);
	}
}

/*
 * sched_unsleep:
 *
 *	The is called when the LWP has not been awoken normally but instead
 *	interrupted: for example, if the sleep timed out.  Because of this,
 *	it's not a valid action for running or idle LWPs.
 */
static u_int
sched_unsleep(struct lwp *l, bool cleanup)
{

	lwp_unlock(l);
	panic("sched_unsleep");
}

static void
resched_cpu(struct lwp *l)
{
	struct cpu_info *ci = ci = l->l_cpu;

	KASSERT(lwp_locked(l, NULL));
	if (lwp_eprio(l) > ci->ci_schedstate.spc_curpriority)
		cpu_need_resched(ci, 0);
}

static void
sched_changepri(struct lwp *l, pri_t pri)
{

	KASSERT(lwp_locked(l, NULL));

	if (l->l_stat == LSRUN && (l->l_flag & LW_INMEM) != 0) {
		KASSERT(lwp_locked(l, l->l_cpu->ci_schedstate.spc_mutex));
		sched_dequeue(l);
		l->l_priority = pri;
		sched_enqueue(l, false);
	} else {
		l->l_priority = pri;
	}
	resched_cpu(l);
}

static void
sched_lendpri(struct lwp *l, pri_t pri)
{

	KASSERT(lwp_locked(l, NULL));

	if (l->l_stat == LSRUN && (l->l_flag & LW_INMEM) != 0) {
		KASSERT(lwp_locked(l, l->l_cpu->ci_schedstate.spc_mutex));
		sched_dequeue(l);
		l->l_inheritedprio = pri;
		sched_enqueue(l, false);
	} else {
		l->l_inheritedprio = pri;
	}
	resched_cpu(l);
}

struct lwp *
syncobj_noowner(wchan_t wchan)
{

	return NULL;
}

/* Decay 95% of proc::p_pctcpu in 60 seconds, ccpu = exp(-1/20) */
const fixpt_t	ccpu = 0.95122942450071400909 * FSCALE;

/*
 * sched_pstats:
 *
 * Update process statistics and check CPU resource allocation.
 * Call scheduler-specific hook to eventually adjust process/LWP
 * priorities.
 */
/* ARGSUSED */
void
sched_pstats(void *arg)
{
	const int clkhz = (stathz != 0 ? stathz : hz);
	static bool backwards;
	struct rlimit *rlim;
	struct lwp *l;
	struct proc *p;
	long runtm;
	fixpt_t lpctcpu;
	u_int lcpticks;
	int sig;

	sched_pstats_ticks++;

	mutex_enter(proc_lock);
	PROCLIST_FOREACH(p, &allproc) {
		if (__predict_false((p->p_flag & PK_MARKER) != 0))
			continue;

		/*
		 * Increment time in/out of memory and sleep
		 * time (if sleeping), ignore overflow.
		 */
		mutex_enter(p->p_lock);
		runtm = p->p_rtime.sec;
		LIST_FOREACH(l, &p->p_lwps, l_sibling) {
			if (__predict_false((l->l_flag & LW_IDLE) != 0))
				continue;
			lwp_lock(l);
			runtm += l->l_rtime.sec;
			l->l_swtime++;
			sched_lwp_stats(l);
			lwp_unlock(l);

			l->l_pctcpu = (l->l_pctcpu * ccpu) >> FSHIFT;
			if (l->l_slptime != 0)
				continue;

			lpctcpu = l->l_pctcpu;
			lcpticks = atomic_swap_uint(&l->l_cpticks, 0);
			lpctcpu += ((FSCALE - ccpu) *
			    (lcpticks * FSCALE / clkhz)) >> FSHIFT;
			l->l_pctcpu = lpctcpu;
		}
		/* Calculating p_pctcpu only for ps(1) */
		p->p_pctcpu = (p->p_pctcpu * ccpu) >> FSHIFT;

		/*
		 * Check if the process exceeds its CPU resource allocation.
		 * If over max, kill it.
		 */
		rlim = &p->p_rlimit[RLIMIT_CPU];
		sig = 0;
		if (__predict_false(runtm >= rlim->rlim_cur)) {
			if (runtm >= rlim->rlim_max)
				sig = SIGKILL;
			else {
				sig = SIGXCPU;
				if (rlim->rlim_cur < rlim->rlim_max)
					rlim->rlim_cur += 5;
			}
		}
		mutex_exit(p->p_lock);
		if (__predict_false(runtm < 0)) {
			if (!backwards) {
				backwards = true;
				printf("WARNING: negative runtime; "
				    "monotonic clock has gone backwards\n");
			}
		} else if (__predict_false(sig)) {
			KASSERT((p->p_flag & PK_SYSTEM) == 0);
			psignal(p, sig);
		}
	}
	mutex_exit(proc_lock);
	uvm_meter();
	cv_wakeup(&lbolt);
	callout_schedule(&sched_pstats_ch, hz);
}
#endif
