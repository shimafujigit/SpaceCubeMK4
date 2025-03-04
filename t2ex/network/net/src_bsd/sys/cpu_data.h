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

/*	$NetBSD: cpu_data.h,v 1.27 2008/06/03 15:50:22 ad Exp $	*/

/*-
 * Copyright (c) 2004, 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
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
 * based on arch/i386/include/cpu.h:
 *	NetBSD: cpu.h,v 1.115 2004/05/16 12:32:53 yamt Exp
 */

#ifndef _SYS_CPU_DATA_H_
#define	_SYS_CPU_DATA_H_

struct callout;
struct lwp;

#ifndef T2EX
#include <sys/sched.h>	/* for schedstate_percpu */
#endif
#include <sys/condvar.h>
#include <sys/percpu_types.h>
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif

/*
 * MI per-cpu data
 *
 * this structure is intended to be included in MD cpu_info structure.
 *	struct cpu_info {
 *		struct cpu_data ci_data;
 *	}
 *
 * note that cpu_data is not expected to contain much data,
 * as cpu_info is size-limited on most ports.
 */

struct lockdebug;

struct cpu_data {
	/*
	 * The first section is likely to be touched by other CPUs -
	 * it is cache hot.
	 */
	lwp_t		*cpu_biglock_wanted;	/* LWP spinning on biglock */
	void		*cpu_callout;		/* per-CPU callout state */
	void		*cpu_unused1;		/* unused */
	u_int		cpu_unused2;		/* unused */
#ifndef T2EX
	struct schedstate_percpu cpu_schedstate; /* scheduler state */
#endif
	kcondvar_t	cpu_xcall;		/* cross-call support */
	int		cpu_xcall_pending;	/* cross-call support */
	lwp_t		*cpu_onproc;		/* bottom level LWP */
	CIRCLEQ_ENTRY(cpu_info) cpu_qchain;	/* circleq of all CPUs */
	
	/*
	 * This section is mostly CPU-private.
	 */
	lwp_t		*cpu_idlelwp;		/* idle lwp */
	void		*cpu_lockstat;		/* lockstat private tables */
	u_int		cpu_index;		/* CPU index */
	u_int		cpu_biglock_count;	/* # recursive holds */
	u_int		cpu_spin_locks;		/* # of spinlockmgr locks */
	u_int		cpu_simple_locks;	/* # of simple locks held */
	u_int		cpu_spin_locks2;	/* # of spin locks held XXX */
	u_int		cpu_lkdebug_recurse;	/* LOCKDEBUG recursion */
	u_int		cpu_softints;		/* pending (slow) softints */
	u_int		cpu_nsyscall;		/* syscall counter */
	u_int		cpu_ntrap;		/* trap counter */
	u_int		cpu_nswtch;		/* context switch counter */
	void		*cpu_uvm;		/* uvm per-cpu data */
	void		*cpu_softcpu;		/* soft interrupt table */
	TAILQ_HEAD(,buf) cpu_biodone;		/* finished block xfers */
	percpu_cpu_t	cpu_percpu;		/* per-cpu data */
	struct selcpu	*cpu_selcpu;		/* per-CPU select() info */
	void		*cpu_nch;		/* per-cpu vfs_cache data */
	_TAILQ_HEAD(,struct lockdebug,volatile) cpu_ld_locks;/* !: lockdebug */
	__cpu_simple_lock_t cpu_ld_lock;	/* lockdebug */
	uint64_t	cpu_cc_freq;		/* cycle counter frequency */
	int64_t		cpu_cc_skew;		/* counter skew vs cpu0 */
};

/* compat definitions */
#define	ci_schedstate		ci_data.cpu_schedstate
#define	ci_index		ci_data.cpu_index
#define	ci_biglock_count	ci_data.cpu_biglock_count
#define	ci_biglock_wanted	ci_data.cpu_biglock_wanted
#define	ci_spin_locks		ci_data.cpu_spin_locks
#define	ci_simple_locks		ci_data.cpu_simple_locks
#define	ci_lockstat		ci_data.cpu_lockstat
#define	ci_spin_locks2		ci_data.cpu_spin_locks2
#define	ci_lkdebug_recurse	ci_data.cpu_lkdebug_recurse

int mi_cpu_attach(struct cpu_info *ci);

#endif /* _SYS_CPU_DATA_H_ */
