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

/*	$NetBSD: sys_select.c,v 1.10 2008/10/15 08:13:17 ad Exp $	*/

/*-
 * Copyright (c) 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
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

/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)sys_generic.c	8.9 (Berkeley) 2/14/95
 */

/*
 * System calls relating to files.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sys_select.c,v 1.10 2008/10/15 08:13:17 ad Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/proc.h>
#ifdef T2EX
#include <sys/socket.h>
#endif
#include <sys/socketvar.h>
#ifndef T2EX
#include <sys/signalvar.h>
#endif
#include <sys/uio.h>
#include <sys/kernel.h>
#include <sys/stat.h>
#include <sys/poll.h>
#ifndef T2EX
#include <sys/vnode.h>
#include <sys/mount.h>
#endif
#include <sys/syscallargs.h>
#ifndef T2EX
#include <sys/cpu.h>
#include <sys/atomic.h>
#else
#include <sys/_atomic.h>
#endif
#include <sys/socketvar.h>
#include <sys/sleepq.h>
#ifdef T2EX
#include <sys/kmem.h>
#endif

/* Flags for lwp::l_selflag. */
#define	SEL_RESET	0	/* awoken, interrupted, or not yet polling */
#define	SEL_SCANNING	1	/* polling descriptors */
#define	SEL_BLOCKING	2	/* about to block on select_cv */

/* Per-CPU state for select()/poll(). */
#if MAXCPUS > 32
#error adjust this code
#endif
typedef struct selcpu {
	kmutex_t	sc_lock;
	sleepq_t	sc_sleepq;
	int		sc_ncoll;
	uint32_t	sc_mask;
} selcpu_t;

#ifdef T2EX
static selcpu_t selcpu0;
#endif

static int	selscan(lwp_t *, fd_mask *, fd_mask *, int, register_t *);
#ifndef T2EX
static int	pollscan(lwp_t *, struct pollfd *, int, register_t *);
#endif
static void	selclear(void);

#ifndef T2EX
static syncobj_t select_sobj = {
	SOBJ_SLEEPQ_FIFO,
	sleepq_unsleep,
	sleepq_changepri,
	sleepq_lendpri,
	syncobj_noowner,
};
#endif

#ifndef T2EX
/*
 * Select system call.
 */
int
sys_pselect(struct lwp *l, const struct sys_pselect_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)				nd;
		syscallarg(fd_set *)			in;
		syscallarg(fd_set *)			ou;
		syscallarg(fd_set *)			ex;
		syscallarg(const struct timespec *)	ts;
		syscallarg(sigset_t *)			mask;
	} */
	struct timespec	ats;
	struct timeval	atv, *tv = NULL;
	sigset_t	amask, *mask = NULL;
	int		error;

	if (SCARG(uap, ts)) {
		error = copyin(SCARG(uap, ts), &ats, sizeof(ats));
		if (error)
			return error;
		atv.tv_sec = ats.tv_sec;
		atv.tv_usec = ats.tv_nsec / 1000;
		tv = &atv;
	}
	if (SCARG(uap, mask) != NULL) {
		error = copyin(SCARG(uap, mask), &amask, sizeof(amask));
		if (error)
			return error;
		mask = &amask;
	}

	return selcommon(l, retval, SCARG(uap, nd), SCARG(uap, in),
	    SCARG(uap, ou), SCARG(uap, ex), tv, mask);
}
#endif

int
inittimeleft(struct timeval *tv, struct timeval *sleeptv)
{
	if (itimerfix(tv))
		return -1;
	getmicrouptime(sleeptv);
	return 0;
}

int
gettimeleft(struct timeval *tv, struct timeval *sleeptv)
{
	/*
	 * We have to recalculate the timeout on every retry.
	 */
	struct timeval slepttv;
	/*
	 * reduce tv by elapsed time
	 * based on monotonic time scale
	 */
	getmicrouptime(&slepttv);
	timeradd(tv, sleeptv, tv);
	timersub(tv, &slepttv, tv);
	*sleeptv = slepttv;
	return tvtohz(tv);
}

int
sys_select(struct lwp *l, const struct sys_select_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)			nd;
		syscallarg(fd_set *)		in;
		syscallarg(fd_set *)		ou;
		syscallarg(fd_set *)		ex;
		syscallarg(struct timeval *)	tv;
	} */
#ifdef T2EX
#define	nd	nfds
#define	in	readfds
#define	ou	writefds
#define	ex	exceptfds
#endif

	struct timeval atv, *tv = NULL;
	int error;

	if (SCARG(uap, tv)) {
		error = copyin(SCARG(uap, tv), (void *)&atv,
			sizeof(atv));
		if (error)
			return error;
		tv = &atv;
	}

#ifndef T2EX
	return selcommon(l, retval, SCARG(uap, nd), SCARG(uap, in),
	    SCARG(uap, ou), SCARG(uap, ex), tv, NULL);
#else
	return selcommon(l, retval, SCARG(uap, nd), SCARG(uap, in),
	    SCARG(uap, ou), SCARG(uap, ex), tv);
#endif

#ifdef T2EX
#undef	nd
#undef	in
#undef	ou
#undef	ex
#endif
}

#ifndef T2EX
int
selcommon(lwp_t *l, register_t *retval, int nd, fd_set *u_in,
	  fd_set *u_ou, fd_set *u_ex, struct timeval *tv, sigset_t *mask)
#else
int
selcommon(lwp_t *l, register_t *retval, int nd, fd_set *u_in,
	  fd_set *u_ou, fd_set *u_ex, struct timeval *tv)
#endif
{
	char		smallbits[howmany(FD_SETSIZE, NFDBITS) *
			    sizeof(fd_mask) * 6];
	proc_t		* const p = l->l_proc;
	char 		*bits;
	int		ncoll, error, timo;
	size_t		ni;
#ifndef T2EX
	sigset_t	oldmask;
#endif
	struct timeval  sleeptv;
	selcpu_t	*sc;

	error = 0;
	if (nd < 0)
		return (EINVAL);
	if (nd > p->p_fd->fd_nfiles) {
		/* forgiving; slightly wrong */
		nd = p->p_fd->fd_nfiles;
	}
	ni = howmany(nd, NFDBITS) * sizeof(fd_mask);
	if (ni * 6 > sizeof(smallbits)) {
		bits = kmem_alloc(ni * 6, KM_SLEEP);
		if (bits == NULL)
			return ENOMEM;
	} else
		bits = smallbits;

#define	getbits(name, x)						\
	if (u_ ## name) {						\
		error = copyin(u_ ## name, bits + ni * x, ni);		\
		if (error)						\
			goto done;					\
	} else								\
		memset(bits + ni * x, 0, ni);
	getbits(in, 0);
	getbits(ou, 1);
	getbits(ex, 2);
#undef	getbits

	timo = 0;
	if (tv && inittimeleft(tv, &sleeptv) == -1) {
		error = EINVAL;
		goto done;
	}

#ifndef T2EX
	if (mask) {
		sigminusset(&sigcantmask, mask);
		mutex_enter(p->p_lock);
		oldmask = l->l_sigmask;
		l->l_sigmask = *mask;
		mutex_exit(p->p_lock);
	} else
		oldmask = l->l_sigmask;	/* XXXgcc */
#endif

#ifndef T2EX
	sc = curcpu()->ci_data.cpu_selcpu;
#else
	sc = &selcpu0;
#endif
	l->l_selcpu = sc;
	SLIST_INIT(&l->l_selwait);
	for (;;) {
		/*
		 * No need to lock.  If this is overwritten by another
		 * value while scanning, we will retry below.  We only
		 * need to see exact state from the descriptors that
		 * we are about to poll, and lock activity resulting
		 * from fo_poll is enough to provide an up to date value
		 * for new polling activity.
		 */
	 	l->l_selflag = SEL_SCANNING;
		ncoll = sc->sc_ncoll;

		error = selscan(l, (fd_mask *)(bits + ni * 0),
		    (fd_mask *)(bits + ni * 3), nd, retval);

		if (error || *retval)
			break;
		if (tv && (timo = gettimeleft(tv, &sleeptv)) <= 0)
			break;
		mutex_spin_enter(&sc->sc_lock);
		if (l->l_selflag != SEL_SCANNING || sc->sc_ncoll != ncoll) {
			mutex_spin_exit(&sc->sc_lock);
			continue;
		}
		l->l_selflag = SEL_BLOCKING;
#ifndef T2EX
		l->l_kpriority = true;
#endif
		sleepq_enter(&sc->sc_sleepq, l, &sc->sc_lock);
#ifndef T2EX
		sleepq_enqueue(&sc->sc_sleepq, sc, "select", &select_sobj);
#else
		sleepq_enqueue(&sc->sc_sleepq, sc, "select", &sleep_syncobj);
#endif
		error = sleepq_block(timo, true);
		if (error != 0)
			break;
	}
	selclear();

#ifndef T2EX
	if (mask) {
		mutex_enter(p->p_lock);
		l->l_sigmask = oldmask;
		mutex_exit(p->p_lock);
	}
#endif

 done:
	/* select is not restarted after signals... */
	if (error == ERESTART)
		error = EINTR;
	if (error == EWOULDBLOCK)
		error = 0;
	if (error == 0 && u_in != NULL)
		error = copyout(bits + ni * 3, u_in, ni);
	if (error == 0 && u_ou != NULL)
		error = copyout(bits + ni * 4, u_ou, ni);
	if (error == 0 && u_ex != NULL)
		error = copyout(bits + ni * 5, u_ex, ni);
	if (bits != smallbits)
		kmem_free(bits, ni * 6);
	return (error);
}

int
selscan(lwp_t *l, fd_mask *ibitp, fd_mask *obitp, int nfd,
	register_t *retval)
{
	static const int flag[3] = { POLLRDNORM | POLLHUP | POLLERR,
			       POLLWRNORM | POLLHUP | POLLERR,
			       POLLRDBAND };
	int msk, i, j, fd, n;
	fd_mask ibits, obits;
	file_t *fp;

	n = 0;
	for (msk = 0; msk < 3; msk++) {
		for (i = 0; i < nfd; i += NFDBITS) {
			ibits = *ibitp++;
			obits = 0;
			while ((j = ffs(ibits)) && (fd = i + --j) < nfd) {
				ibits &= ~(1 << j);
				if ((fp = fd_getfile(fd)) == NULL)
					return (EBADF);
				if ((*fp->f_ops->fo_poll)(fp, flag[msk])) {
					obits |= (1 << j);
					n++;
				}
				fd_putfile(fd);
			}
			*obitp++ = obits;
		}
	}
	*retval = n;
	return (0);
}

#ifndef T2EX
/*
 * Poll system call.
 */
int
sys_poll(struct lwp *l, const struct sys_poll_args *uap, register_t *retval)
{
	/* {
		syscallarg(struct pollfd *)	fds;
		syscallarg(u_int)		nfds;
		syscallarg(int)			timeout;
	} */
	struct timeval	atv, *tv = NULL;

	if (SCARG(uap, timeout) != INFTIM) {
		atv.tv_sec = SCARG(uap, timeout) / 1000;
		atv.tv_usec = (SCARG(uap, timeout) % 1000) * 1000;
		tv = &atv;
	}

	return pollcommon(l, retval, SCARG(uap, fds), SCARG(uap, nfds),
		tv, NULL);
}

/*
 * Poll system call.
 */
int
sys_pollts(struct lwp *l, const struct sys_pollts_args *uap, register_t *retval)
{
	/* {
		syscallarg(struct pollfd *)		fds;
		syscallarg(u_int)			nfds;
		syscallarg(const struct timespec *)	ts;
		syscallarg(const sigset_t *)		mask;
	} */
	struct timespec	ats;
	struct timeval	atv, *tv = NULL;
	sigset_t	amask, *mask = NULL;
	int		error;

	if (SCARG(uap, ts)) {
		error = copyin(SCARG(uap, ts), &ats, sizeof(ats));
		if (error)
			return error;
		atv.tv_sec = ats.tv_sec;
		atv.tv_usec = ats.tv_nsec / 1000;
		tv = &atv;
	}
	if (SCARG(uap, mask)) {
		error = copyin(SCARG(uap, mask), &amask, sizeof(amask));
		if (error)
			return error;
		mask = &amask;
	}

	return pollcommon(l, retval, SCARG(uap, fds), SCARG(uap, nfds),
		tv, mask);
}

int
pollcommon(lwp_t *l, register_t *retval,
	struct pollfd *u_fds, u_int nfds,
	struct timeval *tv, sigset_t *mask)
{
	char		smallbits[32 * sizeof(struct pollfd)];
	proc_t		* const p = l->l_proc;
	void *		bits;
	sigset_t	oldmask;
	int		ncoll, error, timo;
	size_t		ni;
	struct timeval	sleeptv;
	selcpu_t	*sc;

	if (nfds > p->p_fd->fd_nfiles) {
		/* forgiving; slightly wrong */
		nfds = p->p_fd->fd_nfiles;
	}
	ni = nfds * sizeof(struct pollfd);
	if (ni > sizeof(smallbits)) {
		bits = kmem_alloc(ni, KM_SLEEP);
		if (bits == NULL)
			return ENOMEM;
	} else
		bits = smallbits;

	error = copyin(u_fds, bits, ni);
	if (error)
		goto done;

	timo = 0;
	if (tv && inittimeleft(tv, &sleeptv) == -1) {
		error = EINVAL;
		goto done;
	}

	if (mask) {
		sigminusset(&sigcantmask, mask);
		mutex_enter(p->p_lock);
		oldmask = l->l_sigmask;
		l->l_sigmask = *mask;
		mutex_exit(p->p_lock);
	} else
		oldmask = l->l_sigmask;	/* XXXgcc */

	sc = curcpu()->ci_data.cpu_selcpu;
	l->l_selcpu = sc;
	SLIST_INIT(&l->l_selwait);
	for (;;) {
		/*
		 * No need to lock.  If this is overwritten by another
		 * value while scanning, we will retry below.  We only
		 * need to see exact state from the descriptors that
		 * we are about to poll, and lock activity resulting
		 * from fo_poll is enough to provide an up to date value
		 * for new polling activity.
		 */
		ncoll = sc->sc_ncoll;
		l->l_selflag = SEL_SCANNING;

		error = pollscan(l, (struct pollfd *)bits, nfds, retval);

		if (error || *retval)
			break;
		if (tv && (timo = gettimeleft(tv, &sleeptv)) <= 0)
			break;
		mutex_spin_enter(&sc->sc_lock);
		if (l->l_selflag != SEL_SCANNING || sc->sc_ncoll != ncoll) {
			mutex_spin_exit(&sc->sc_lock);
			continue;
		}
		l->l_selflag = SEL_BLOCKING;
		l->l_kpriority = true;
		sleepq_enter(&sc->sc_sleepq, l, &sc->sc_lock);
		sleepq_enqueue(&sc->sc_sleepq, sc, "select", &select_sobj);
		error = sleepq_block(timo, true);
		if (error != 0)
			break;
	}
	selclear();

	if (mask) {
		mutex_enter(p->p_lock);
		l->l_sigmask = oldmask;
		mutex_exit(p->p_lock);
	}
 done:
	/* poll is not restarted after signals... */
	if (error == ERESTART)
		error = EINTR;
	if (error == EWOULDBLOCK)
		error = 0;
	if (error == 0)
		error = copyout(bits, u_fds, ni);
	if (bits != smallbits)
		kmem_free(bits, ni);
	return (error);
}

int
pollscan(lwp_t *l, struct pollfd *fds, int nfd, register_t *retval)
{
	int i, n;
	file_t *fp;

	n = 0;
	for (i = 0; i < nfd; i++, fds++) {
		if (fds->fd < 0) {
			fds->revents = 0;
		} else if ((fp = fd_getfile(fds->fd)) == NULL) {
			fds->revents = POLLNVAL;
			n++;
		} else {
			fds->revents = (*fp->f_ops->fo_poll)(fp,
			    fds->events | POLLERR | POLLHUP);
			if (fds->revents != 0)
				n++;
			fd_putfile(fds->fd);
		}
	}
	*retval = n;
	return (0);
}
#endif

/*ARGSUSED*/
int
seltrue(dev_t dev, int events, lwp_t *l)
{

	return (events & (POLLIN | POLLOUT | POLLRDNORM | POLLWRNORM));
}

#ifdef T2EX
int
seltrue_kqfilter(dev_t dev, int events, lwp_t *l)
{
	/* kqfilter is not supported. */
	panic("%s", __func__);
	return 0;
}
#endif

/*
 * Record a select request.  Concurrency issues:
 *
 * The caller holds the same lock across calls to selrecord() and
 * selnotify(), so we don't need to consider a concurrent wakeup
 * while in this routine.
 *
 * The only activity we need to guard against is selclear(), called by
 * another thread that is exiting selcommon() or pollcommon().
 * `sel_lwp' can only become non-NULL while the caller's lock is held,
 * so it cannot become non-NULL due to a change made by another thread
 * while we are in this routine.  It can only become _NULL_ due to a
 * call to selclear().
 *
 * If it is non-NULL and != selector there is the potential for
 * selclear() to be called by another thread.  If either of those
 * conditions are true, we're not interested in touching the `named
 * waiter' part of the selinfo record because we need to record a
 * collision.  Hence there is no need for additional locking in this
 * routine.
 */
void
selrecord(lwp_t *selector, struct selinfo *sip)
{
	selcpu_t *sc;
	lwp_t *other;

	KASSERT(selector == curlwp);

	sc = selector->l_selcpu;
	other = sip->sel_lwp;

	if (other == selector) {
		/* `selector' has already claimed it. */
		KASSERT(sip->sel_cpu = sc);
	} else if (other == NULL) {
		/*
		 * First named waiter, although there may be unnamed
		 * waiters (collisions).  Issue a memory barrier to
		 * ensure that we access sel_lwp (above) before other
		 * fields - this guards against a call to selclear().
		 */
		membar_enter();
		sip->sel_lwp = selector;
		SLIST_INSERT_HEAD(&selector->l_selwait, sip, sel_chain);
		/* Replace selinfo's lock with our chosen CPU's lock. */
		sip->sel_cpu = sc;
	} else {
		/* Multiple waiters: record a collision. */
		sip->sel_collision |= sc->sc_mask;
		KASSERT(sip->sel_cpu != NULL);
	}
}

/*
 * Do a wakeup when a selectable event occurs.  Concurrency issues:
 *
 * As per selrecord(), the caller's object lock is held.  If there
 * is a named waiter, we must acquire the associated selcpu's lock
 * in order to synchronize with selclear() and pollers going to sleep
 * in selcommon() and/or pollcommon().
 *
 * sip->sel_cpu cannot change at this point, as it is only changed
 * in selrecord(), and concurrent calls to selrecord() are locked
 * out by the caller.
 */
void
selnotify(struct selinfo *sip, int events, long knhint)
{
	selcpu_t *sc;
#ifndef T2EX
	uint32_t mask;
	int index, oflag, swapin;
#else
	int oflag, swapin;
#endif
	lwp_t *l;

#ifndef T2EX
	KNOTE(&sip->sel_klist, knhint);
#endif

	if (sip->sel_lwp != NULL) {
		/* One named LWP is waiting. */
		swapin = 0;
		sc = sip->sel_cpu;
		mutex_spin_enter(&sc->sc_lock);
		/* Still there? */
		if (sip->sel_lwp != NULL) {
			l = sip->sel_lwp;
			/*
			 * If thread is sleeping, wake it up.  If it's not
			 * yet asleep, it will notice the change in state
			 * and will re-poll the descriptors.
			 */
			oflag = l->l_selflag;
			l->l_selflag = SEL_RESET;
			if (oflag == SEL_BLOCKING &&
			    l->l_mutex == &sc->sc_lock) {
				KASSERT(l->l_wchan == sc);
				swapin = sleepq_unsleep(l, false);
			}
		}
		mutex_spin_exit(&sc->sc_lock);
#ifndef T2EX
		if (swapin)
			uvm_kick_scheduler();
#endif
	}

#ifndef T2EX
	if ((mask = sip->sel_collision) != 0) {
		/*
		 * There was a collision (multiple waiters): we must
		 * inform all potentially interested waiters.
		 */
		sip->sel_collision = 0;
		do {
			index = ffs(mask) - 1;
			mask &= ~(1 << index);
			sc = cpu_lookup(index)->ci_data.cpu_selcpu;
			mutex_spin_enter(&sc->sc_lock);
			sc->sc_ncoll++;
			sleepq_wake(&sc->sc_sleepq, sc, (u_int)-1,
			    &sc->sc_lock);
		} while (__predict_false(mask != 0));
	}
#endif
}

/*
 * Remove an LWP from all objects that it is waiting for.  Concurrency
 * issues:
 *
 * The object owner's (e.g. device driver) lock is not held here.  Calls
 * can be made to selrecord() and we do not synchronize against those
 * directly using locks.  However, we use `sel_lwp' to lock out changes.
 * Before clearing it we must use memory barriers to ensure that we can
 * safely traverse the list of selinfo records.
 */
static void
selclear(void)
{
	struct selinfo *sip, *next;
	selcpu_t *sc;
	lwp_t *l;

	l = curlwp;
	sc = l->l_selcpu;

	mutex_spin_enter(&sc->sc_lock);
	for (sip = SLIST_FIRST(&l->l_selwait); sip != NULL; sip = next) {
		KASSERT(sip->sel_lwp == l);
		KASSERT(sip->sel_cpu == l->l_selcpu);
		/*
		 * Read link to next selinfo record, if any.
		 * It's no longer safe to touch `sip' after clearing
		 * `sel_lwp', so ensure that the read of `sel_chain'
		 * completes before the clearing of sel_lwp becomes
		 * globally visible.
		 */
		next = SLIST_NEXT(sip, sel_chain);
		membar_exit();
		/* Release the record for another named waiter to use. */
		sip->sel_lwp = NULL;
	}
	mutex_spin_exit(&sc->sc_lock);
}

/*
 * Initialize the select/poll system calls.  Called once for each
 * CPU in the system, as they are attached.
 */
#ifndef T2EX
void
selsysinit(struct cpu_info *ci)
{
	selcpu_t *sc;

	sc = kmem_alloc(roundup2(sizeof(selcpu_t), coherency_unit) +
	    coherency_unit, KM_SLEEP);
	sc = (void *)roundup2((uintptr_t)sc, coherency_unit);
	mutex_init(&sc->sc_lock, MUTEX_DEFAULT, IPL_SCHED);
	sleepq_init(&sc->sc_sleepq);
	sc->sc_ncoll = 0;
	sc->sc_mask = (1 << cpu_index(ci));
	ci->ci_data.cpu_selcpu = sc;
}
#else
int
selsysinit(struct cpu_info *ci)
{
	selcpu_t *sc;
	int error;

	sc = &selcpu0;
	error = mutex_init(&sc->sc_lock, MUTEX_DEFAULT, IPL_SCHED);
	if ( error != 0 ) {
		return error;
	}
	error = sleepq_init(&sc->sc_sleepq);
	if ( error != 0 ) {
		mutex_destroy(&sc->sc_lock);
		return error;
	}
	sc->sc_ncoll = 0;
	sc->sc_mask = (1 << 0);

	return 0;
}

int
selsysfinish(struct cpu_info *ci)
{
	selcpu_t *sc;

	sc = &selcpu0;

	sleepq_finish(&sc->sc_sleepq);
	mutex_destroy(&sc->sc_lock);

	return 0;
}
#endif

/*
 * Initialize a selinfo record.
 */
void
selinit(struct selinfo *sip)
{

	memset(sip, 0, sizeof(*sip));
}

/*
 * Destroy a selinfo record.  The owning object must not gain new
 * references while this is in progress: all activity on the record
 * must be stopped.
 *
 * Concurrency issues: we only need guard against a call to selclear()
 * by a thread exiting selcommon() and/or pollcommon().  The caller has
 * prevented further references being made to the selinfo record via
 * selrecord(), and it won't call selwakeup() again.
 */
void
seldestroy(struct selinfo *sip)
{
	selcpu_t *sc;
	lwp_t *l;

	if (sip->sel_lwp == NULL)
		return;

	/*
	 * Lock out selclear().  The selcpu pointer can't change while
	 * we are here since it is only ever changed in selrecord(),
	 * and that will not be entered again for this record because
	 * it is dying.
	 */
	KASSERT(sip->sel_cpu != NULL);
	sc = sip->sel_cpu;
	mutex_spin_enter(&sc->sc_lock);
	if ((l = sip->sel_lwp) != NULL) {
		/*
		 * This should rarely happen, so although SLIST_REMOVE()
		 * is slow, using it here is not a problem.
		 */
		KASSERT(l->l_selcpu == sc);
		SLIST_REMOVE(&l->l_selwait, sip, selinfo, sel_chain);
		sip->sel_lwp = NULL;
	}
	mutex_spin_exit(&sc->sc_lock);
}

#ifndef T2EX
int
pollsock(struct socket *so, const struct timeval *tvp, int events)
{
	int		ncoll, error, timo;
	struct timeval	sleeptv, tv;
	selcpu_t	*sc;
	lwp_t		*l;

	timo = 0;
	if (tvp != NULL) {
		tv = *tvp;
		if (inittimeleft(&tv, &sleeptv) == -1)
			return EINVAL;
	}

	l = curlwp;
	sc = l->l_cpu->ci_data.cpu_selcpu;
	l->l_selcpu = sc;
	SLIST_INIT(&l->l_selwait);
	error = 0;
	for (;;) {
		/*
		 * No need to lock.  If this is overwritten by another
		 * value while scanning, we will retry below.  We only
		 * need to see exact state from the descriptors that
		 * we are about to poll, and lock activity resulting
		 * from fo_poll is enough to provide an up to date value
		 * for new polling activity.
		 */
		ncoll = sc->sc_ncoll;
		l->l_selflag = SEL_SCANNING;
		if (sopoll(so, events) != 0)
			break;
		if (tvp && (timo = gettimeleft(&tv, &sleeptv)) <= 0)
			break;
		mutex_spin_enter(&sc->sc_lock);
		if (l->l_selflag != SEL_SCANNING || sc->sc_ncoll != ncoll) {
			mutex_spin_exit(&sc->sc_lock);
			continue;
		}
		l->l_selflag = SEL_BLOCKING;
		sleepq_enter(&sc->sc_sleepq, l, &sc->sc_lock);
		sleepq_enqueue(&sc->sc_sleepq, sc, "pollsock", &select_sobj);
		error = sleepq_block(timo, true);
		if (error != 0)
			break;
	}
	selclear();
	/* poll is not restarted after signals... */
	if (error == ERESTART)
		error = EINTR;
	if (error == EWOULDBLOCK)
		error = 0;
	return (error);
}
#endif
