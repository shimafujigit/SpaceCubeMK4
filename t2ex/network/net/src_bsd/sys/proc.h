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

/*	$NetBSD: proc.h,v 1.282 2008/10/22 11:14:33 ad Exp $	*/

/*-
 * Copyright (c) 2006, 2007, 2008 The NetBSD Foundation, Inc.
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

/*-
 * Copyright (c) 1986, 1989, 1991, 1993
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
 *	@(#)proc.h	8.15 (Berkeley) 5/19/95
 */

#ifndef _SYS_PROC_H_
#define	_SYS_PROC_H_

#if defined(_KERNEL_OPT)
#ifndef T2EX
#include "opt_multiprocessor.h"
#include "opt_kstack.h"
#include "opt_lockdebug.h"
#endif
#endif

#ifndef T2EX
#include <machine/proc.h>		/* Machine-dependent proc substruct */
#include <sys/aio.h>
#endif
#include <sys/rwlock.h>
#ifndef T2EX
#include <sys/mqueue.h>
#endif
#include <sys/mutex.h>
#include <sys/condvar.h>
#include <sys/lwp.h>
#ifndef T2EX
#include <sys/queue.h>
#include <sys/signalvar.h>
#include <sys/siginfo.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/event.h>
#include <sys/specificdata.h>
#ifdef T2EX
#include <sys/file.h>
#endif

#ifndef _KERNEL
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifndef T2EX
/*
 * One structure allocated per session.
 */
struct session {
	int		s_count;	/* Ref cnt; pgrps in session */
	u_int		s_flags;
#define	S_LOGIN_SET	1		/* s_login set in this session */
	struct proc	*s_leader;	/* Session leader */
	struct vnode	*s_ttyvp;	/* Vnode of controlling terminal */
	struct tty	*s_ttyp;	/* Controlling terminal */
	char		s_login[MAXLOGNAME]; /* Setlogin() name */
	pid_t		s_sid;		/* Session ID (pid of leader) */
};

/*
 * One structure allocated per process group.
 */
struct pgrp {
	LIST_HEAD(, proc) pg_members;	/* Pointer to pgrp members */
	struct session	*pg_session;	/* Pointer to session */
	pid_t		pg_id;		/* Pgrp id */
	int		pg_jobc;	/*
					 * Number of processes qualifying
					 * pgrp for job control
					 */
};

/*
 * One structure allocated per emulation.
 */
struct exec_package;
struct ps_strings;
struct ras;
struct kauth_cred;

struct emul {
	const char	*e_name;	/* Symbolic name */
	const char	*e_path;	/* Extra emulation path (NULL if none)*/
#ifndef __HAVE_MINIMAL_EMUL
	int		e_flags;	/* Miscellaneous flags, see above */
					/* Syscall handling function */
	const int	*e_errno;	/* Errno array */
	int		e_nosys;	/* Offset of the nosys() syscall */
	int		e_nsysent;	/* Number of system call entries */
#endif
	struct sysent	*e_sysent;	/* System call array */
	const char * const *e_syscallnames; /* System call name array */
					/* Signal sending function */
	void		(*e_sendsig)(const struct ksiginfo *,
					  const sigset_t *);
	void		(*e_trapsignal)(struct lwp *, struct ksiginfo *);
	int		(*e_tracesig)(struct proc *, int);
	char		*e_sigcode;	/* Start of sigcode */
	char		*e_esigcode;	/* End of sigcode */
					/* Set registers before execution */
	struct uvm_object **e_sigobject;/* shared sigcode object */
	void		(*e_setregs)(struct lwp *, struct exec_package *,
					  u_long);

					/* Per-process hooks */
	void		(*e_proc_exec)(struct proc *, struct exec_package *);
	void		(*e_proc_fork)(struct proc *, struct proc *, int);
	void		(*e_proc_exit)(struct proc *);
	void		(*e_lwp_fork)(struct lwp *, struct lwp *);
	void		(*e_lwp_exit)(struct lwp *);

#ifdef __HAVE_SYSCALL_INTERN
	void		(*e_syscall_intern)(struct proc *);
#else
	void		(*e_syscall)(void);
#endif
					/* Emulation specific sysctl data */
	struct sysctlnode *e_sysctlovly;
	int		(*e_fault)(struct proc *, vaddr_t, int);

	vaddr_t		(*e_vm_default_addr)(struct proc *, vaddr_t, vsize_t);

	/* Emulation-specific hook for userspace page faults */
	int		(*e_usertrap)(struct lwp *, vaddr_t, void *);

	/* SA-related information */
	const struct sa_emul *e_sa;

	size_t		e_ucsize;	/* size of ucontext_t */
	void		(*e_startlwp)(void *);
};

/*
 * Emulation miscelaneous flags
 */
#define	EMUL_HAS_SYS___syscall	0x001	/* Has SYS___syscall */
#endif /* T2EX */

/*
 * Description of a process.
 *
 * This structure contains the information needed to manage a thread of
 * control, known in UN*X as a process; it has references to substructures
 * containing descriptions of things that the process uses, but may share
 * with related processes.  The process structure and the substructures
 * are always addressible except for those marked "(PROC ONLY)" below,
 * which might be addressible only on a processor on which the process
 * is running.
 *
 * Field markings and the corresponding locks (not yet fully implemented,
 * more a statement of intent):
 *
 * a:	p_auxlock
 * k:	ktrace_mutex
 * l:	proc_lock
 * t:	p_stmutex
 * p:	p_lock
 * q:	mqlist_mtx
 * ::	unlocked, stable
 */
#ifdef T2EX
struct proc {
	struct filedesc	*p_fd;		/* Ptr to open files structure */
};
extern struct proc proc;
#else /* T2EX */
struct proc {
	LIST_ENTRY(proc) p_list;	/* l: List of all processes */

	kmutex_t	p_auxlock;	/* :: secondary, longer term lock */
	kmutex_t	*p_lock;	/* :: general mutex */
	kmutex_t	p_stmutex;	/* :: mutex on profiling state */
	krwlock_t	p_reflock;	/* p: lock for debugger, procfs */
	kcondvar_t	p_waitcv;	/* p: wait, stop CV on children */
	kcondvar_t	p_lwpcv;	/* p: wait, stop CV on LWPs */
      
	/* Substructures: */
	struct kauth_cred *p_cred;	/* p: Master copy of credentials */
	struct filedesc	*p_fd;		/* :: Ptr to open files structure */
	struct cwdinfo	*p_cwdi;	/* :: cdir/rdir/cmask info */
	struct pstats	*p_stats;	/* :: Accounting/stats (PROC ONLY) */
	struct plimit	*p_limit;	/* :: Process limits */
	struct vmspace	*p_vmspace;	/* :: Address space */
	struct sigacts	*p_sigacts;	/* :: Process sigactions */
	struct aioproc	*p_aio;		/* p: Asynchronous I/O data */
	u_int		p_mqueue_cnt;	/* q: Count of open mqueues */
	specificdata_reference
			p_specdataref;	/*    subsystem proc-specific data */

	int		p_exitsig;	/* l: signal to send to parent on exit */
	int		p_flag;		/* p: P_* flags */
	int		p_sflag;	/* p: PS_* flags */
	int		p_slflag;	/* s, l: PSL_* flags */
	int		p_lflag;	/* l: PL_* flags */
	int		p_stflag;	/* t: PST_* flags */
	char		p_stat;		/* p: S* process status. */
	char		p_trace_enabled;/* p: cached by syscall_intern() */
	char		p_pad1[2];	/*  unused */

	pid_t		p_pid;		/* :: Process identifier. */
	LIST_ENTRY(proc) p_pglist;	/* l: List of processes in pgrp. */
	struct proc 	*p_pptr;	/* l: Pointer to parent process. */
	LIST_ENTRY(proc) p_sibling;	/* l: List of sibling processes. */
	LIST_HEAD(, proc) p_children;	/* l: List of children. */
	LIST_HEAD(, lwp) p_lwps;	/* p: List of LWPs. */
	struct ras	*p_raslist;	/* a: List of RAS entries */

/* The following fields are all zeroed upon creation in fork. */
#define	p_startzero	p_nlwps

	int 		p_nlwps;	/* p: Number of LWPs */
	int 		p_nzlwps;	/* p: Number of zombie LWPs */
	int		p_nrlwps;	/* p: Number running/sleeping LWPs */
	int		p_nlwpwait;	/* p: Number of LWPs in lwp_wait1() */
	int		p_ndlwps;	/* p: Number of detached LWPs */
	int 		p_nlwpid;	/* p: Next LWP ID */
	u_int		p_nstopchild;	/* l: Count of stopped/dead children */
	u_int		p_waited;	/* l: parent has waited on child */
	struct lwp	*p_zomblwp;	/* p: detached LWP to be reaped */

	struct sadata	*p_sa;		/* p: Scheduler activation info */

	/* scheduling */
	void		*p_sched_info;	/* p: Scheduler-specific structure */
	fixpt_t		p_estcpu;	/* p: Time avg. value of p_cpticks */
	fixpt_t		p_estcpu_inherited; /* p: cpu inherited from children */
	unsigned int	p_forktime;
	fixpt_t         p_pctcpu;       /* p: %cpu from dead LWPs */

	struct proc	*p_opptr;	/* l: save parent during ptrace. */
	struct ptimers	*p_timers;	/*    Timers: real, virtual, profiling */
	struct bintime 	p_rtime;	/* p: real time */
	u_quad_t 	p_uticks;	/* t: Statclock hits in user mode */
	u_quad_t 	p_sticks;	/* t: Statclock hits in system mode */
	u_quad_t 	p_iticks;	/* t: Statclock hits processing intr */

	int		p_traceflag;	/* k: Kernel trace points */
	int		p_timerpend;	/* p: Pending itimer to run */
	void		*p_tracep;	/* k: Trace private data */
	struct vnode 	*p_textvp;	/* :: Vnode of executable */

	void	     (*p_userret)(void);/* p: return-to-user hook */
	const struct emul *p_emul;	/* :: emulation information */
	void		*p_emuldata;	/* :: per-proc emul data, or NULL */
	const struct execsw *p_execsw;	/* :: exec package information */
	struct klist	p_klist;	/* p: knotes attached to proc */

	LIST_HEAD(, lwp) p_sigwaiters;	/* p: LWPs waiting for signals */
	sigstore_t	p_sigstore;	/* p: process-wide signal state */
	sigpend_t	p_sigpend;	/* p: pending signals */
	struct lcproc	*p_lwpctl;	/* p, a: _lwp_ctl() information */
	pid_t		p_ppid;		/* :: cached parent pid */

/*
 * End area that is zeroed on creation
 */
#define	p_endzero	p_startcopy

/*
 * The following fields are all copied upon creation in fork.
 */
#define	p_startcopy	p_sigctx

	struct sigctx 	p_sigctx;	/* p: Shared signal state */

	u_char		p_nice;		/* p: Process "nice" value */
	char		p_comm[MAXCOMLEN+1];
					/* p: basename of last exec file */
	struct pgrp 	*p_pgrp;	/* l: Pointer to process group */

	struct ps_strings *p_psstr;	/* :: address of process's ps_strings */
	size_t 		p_psargv;	/* :: offset of ps_argvstr in above */
	size_t 		p_psnargv;	/* :: offset of ps_nargvstr in above */
	size_t 		p_psenv;	/* :: offset of ps_envstr in above */
	size_t 		p_psnenv;	/* :: offset of ps_nenvstr in above */
	u_int		p_pax;		/* :: PAX flags */

/*
 * End area that is copied on creation
 */
#define	p_endcopy	p_xstat

	u_short		p_xstat;	/* p: Exit status for wait; also stop signal */
	u_short		p_acflag;	/* p: Acc. flags; see struct lwp also */
	struct mdproc	p_md;		/* p: Any machine-dependent fields */
	vaddr_t		p_stackbase;	/* :: ASLR randomized stack base */
};

#define	p_rlimit	p_limit->pl_rlimit
#define	p_session	p_pgrp->pg_session
#define	p_pgid		p_pgrp->pg_id

/*
 * Status values.
 */
#define	SIDL		1		/* Process being created by fork */
#define	SACTIVE		2		/* Process is not stopped */
#define	SDYING		3		/* About to die */
#define	SSTOP		4		/* Process debugging or suspension */
#define	SZOMB		5		/* Awaiting collection by parent */
#define	SDEAD	 	6		/* Almost a zombie */

#define	P_ZOMBIE(p)	\
    ((p)->p_stat == SZOMB || (p)->p_stat == SDYING || (p)->p_stat == SDEAD)

/*
 * These flags are kept in p_flag and are protected by p_lock.  Access from
 * process context only.
 */
#define	PK_ADVLOCK	0x00000001 /* Process may hold a POSIX advisory lock */
#define	PK_SYSTEM	0x00000002 /* System process (kthread) */
#define	PK_SYSVSEM	0x00000004 /* Used SysV semaphores */
#define	PK_SUGID	0x00000100 /* Had set id privileges since last exec */
#define	PK_EXEC		0x00004000 /* Process called exec */
#define	PK_NOCLDWAIT	0x00020000 /* No zombies if child dies */
#define	PK_32		0x00040000 /* 32-bit process (used on 64-bit kernels) */
#define	PK_CLDSIGIGN	0x00080000 /* Process is ignoring SIGCHLD */
#define	PK_MARKER	0x80000000 /* Is a dummy marker process */

/*
 * These flags are kept in p_sflag and are protected by p_lock.  Access from
 * process context only.
 */
#define	PS_NOCLDSTOP	0x00000008 /* No SIGCHLD when children stop */
#define	PS_SA		0x00000400 /* Process using scheduler activations */
#define	PS_WCORE	0x00001000 /* Process needs to dump core */
#define	PS_WEXIT	0x00002000 /* Working on exiting */
#define	PS_STOPFORK	0x00800000 /* Child will be stopped on fork(2) */
#define	PS_STOPEXEC	0x01000000 /* Will be stopped on exec(2) */
#define	PS_STOPEXIT	0x02000000 /* Will be stopped at process exit */
#define	PS_NOTIFYSTOP	0x10000000 /* Notify parent of successful STOP */
#define	PS_NOSA 	0x40000000 /* Do not enable SA */
#define	PS_STOPPING	0x80000000 /* Transitioning SACTIVE -> SSTOP */

/*
 * These flags are kept in p_sflag and are protected by the proc_lock
 * and p_lock.  Access from process context only.
 */
#define	PSL_TRACED	0x00000800 /* Debugged process being traced */
#define	PSL_FSTRACE	0x00010000 /* Debugger process being traced by procfs */
#define	PSL_CHTRACED	0x00400000 /* Child has been traced & reparented */
#define	PSL_SYSCALL	0x04000000 /* process has PT_SYSCALL enabled */

/*
 * Kept in p_stflag and protected by p_stmutex.
 */
#define	PST_PROFIL	0x00000020 /* Has started profiling */

/*
 * The final set are protected by the proc_lock.  Access
 * from process context only.
 */
#define	PL_CONTROLT	0x00000002 /* Has a controlling terminal */
#define	PL_PPWAIT	0x00000010 /* Parent is waiting for child exec/exit */
#define	PL_ORPHANPG	0x20000000 /* Member of an orphaned pgrp */

/*
 * Macro to compute the exit signal to be delivered.
 */
#define	P_EXITSIG(p)	\
    (((p)->p_slflag & (PSL_TRACED|PSL_FSTRACE)) ? SIGCHLD : p->p_exitsig)
#endif /* T2EX */

#ifndef T2EX
LIST_HEAD(proclist, proc);		/* A list of processes */

/*
 * This structure associates a proclist with its lock.
 */
struct proclist_desc {
	struct proclist	*pd_list;	/* The list */
	/*
	 * XXX Add a pointer to the proclist's lock eventually.
	 */
};
#endif /* T2EX */

#ifdef _KERNEL
#ifndef T2EX
#include <sys/mallocvar.h>
MALLOC_DECLARE(M_EMULDATA);
MALLOC_DECLARE(M_PROC);
MALLOC_DECLARE(M_SESSION);
MALLOC_DECLARE(M_SUBPROC);	/* XXX - only used by sparc/sparc64 */

/*
 * We use process IDs <= PID_MAX until there are > 16k processes.
 * NO_PGID is used to represent "no process group" for a tty.
 */
#define	PID_MAX		30000
#define	NO_PGID		((pid_t)-1)

#define	SESS_LEADER(p)	((p)->p_session->s_leader == (p))
#define	SESSHOLD(s)	((s)->s_count++)
#define	SESSRELE(s)							\
do {									\
	if (--(s)->s_count == 0)					\
		sessdelete(s);						\
} while (/* CONSTCOND */ 0)


/*
 * Flags passed to fork1().
 */
#define	FORK_PPWAIT	0x0001		/* Block parent until child exit */
#define	FORK_SHAREVM	0x0002		/* Share vmspace with parent */
#define	FORK_SHARECWD	0x0004		/* Share cdir/rdir/cmask */
#define	FORK_SHAREFILES	0x0008		/* Share file descriptors */
#define	FORK_SHARESIGS	0x0010		/* Share signal actions */
#define	FORK_NOWAIT	0x0020		/* Make init the parent of the child */
#define	FORK_CLEANFILES	0x0040		/* Start with a clean descriptor set */
#define	FORK_SYSTEM	0x0080		/* Fork a kernel thread */
#define	FORK_SHARELIMIT	0x0100		/* Share rlimit values */
#endif /* T2EX */

#ifndef T2EX
extern struct proc	proc0;		/* Process slot for swapper */
extern u_int		nprocs;		/* Current number of procs */
extern int		maxproc;	/* Max number of procs */
#define	vmspace_kernel()	(proc0.p_vmspace)

extern kmutex_t		*proc_lock;
extern struct proclist	allproc;	/* List of all processes */
extern struct proclist	zombproc;	/* List of zombie processes */

extern SLIST_HEAD(deadprocs, proc) deadprocs;	/* List of dead processes */
extern struct simplelock deadproc_slock;

extern struct proc	*initproc;	/* Process slots for init, pager */

extern const struct proclist_desc proclists[];

extern struct pool	ptimer_pool;	/* Memory pool for ptimers */

struct proc *p_find(pid_t, uint);	/* Find process by id */
struct pgrp *pg_find(pid_t, uint);	/* Find process group by id */
/* Flags values for p_find() and pg_find(). */
#define PFIND_ZOMBIE		1	/* look for zombies as well */
#define PFIND_LOCKED		2	/* proclist locked on entry */
#define PFIND_UNLOCK_FAIL	4	/* unlock proclist on failure */
#define PFIND_UNLOCK_OK		8	/* unlock proclist on success */
#define PFIND_UNLOCK		(PFIND_UNLOCK_OK | PFIND_UNLOCK_FAIL)
/* For source compatibility. but UNLOCK_OK gives a stale answer... */
#define pfind(pid) p_find((pid), PFIND_UNLOCK)
#define pgfind(pgid) pg_find((pgid), PFIND_UNLOCK)
#endif /* T2EX */

struct simplelock;
#ifndef T2EX
int	enterpgrp(struct proc *, pid_t, pid_t, int);
void	leavepgrp(struct proc *);
void	fixjobc(struct proc *, struct pgrp *, int);
int	inferior(struct proc *, struct proc *);
void	sessdelete(struct session *);
void	yield(void);
void	pgdelete(struct pgrp *);
void	procinit(void);
void	suspendsched(void);
#endif /* T2EX */
int	ltsleep(wchan_t, pri_t, const char *, int, volatile struct simplelock *);
int	mtsleep(wchan_t, pri_t, const char *, int, kmutex_t *);
void	wakeup(wchan_t);
#ifndef T2EX
void	wakeup_one(wchan_t);
int	kpause(const char *, bool, int, kmutex_t *);
void	exit1(struct lwp *, int) __dead;
int	do_sys_wait(struct lwp *, int *, int *, int, struct rusage *, int *);
struct proc *proc_alloc(void);
void	proc0_init(void);
void	proc_free_pid(struct proc *);
void	proc_free_mem(struct proc *);
void	exit_lwps(struct lwp *l);
int	fork1(struct lwp *, int, int, void *, size_t,
	    void (*)(void *), void *, register_t *, struct proc **);
int	pgid_in_session(struct proc *, pid_t);
void	cpu_lwp_fork(struct lwp *, struct lwp *, void *, size_t,
	    void (*)(void *), void *);
#ifndef cpu_lwp_free
void	cpu_lwp_free(struct lwp *, int);
#ifndef cpu_lwp_free2
void	cpu_lwp_free2(struct lwp *);
#endif
#endif

#ifdef __HAVE_SYSCALL_INTERN
void	syscall_intern(struct proc *);
#endif

void	child_return(void *);

int	proc_isunder(struct proc *, struct lwp *);
void	proc_stop(struct proc *, int, int);

void	p_sugid(struct proc *);

int	proc_vmspace_getref(struct proc *, struct vmspace **);
void	proc_crmod_leave(kauth_cred_t, kauth_cred_t, bool);
void	proc_crmod_enter(void);
int	proc_addref(struct proc *);
void	proc_delref(struct proc *);
void	proc_drainrefs(struct proc *);

int	proc_specific_key_create(specificdata_key_t *, specificdata_dtor_t);
void	proc_specific_key_delete(specificdata_key_t);
void 	proc_initspecific(struct proc *);
void 	proc_finispecific(struct proc *);
void *	proc_getspecific(struct proc *, specificdata_key_t);
void	proc_setspecific(struct proc *, specificdata_key_t, void *);

int	proclist_foreach_call(struct proclist *,
    int (*)(struct proc *, void *arg), void *);
static __inline struct proc *_proclist_skipmarker(struct proc *);

static __inline struct proc *
_proclist_skipmarker(struct proc *p0)
{
	struct proc *p = p0;

	while (p != NULL && (p->p_flag & PK_MARKER))
		p = LIST_NEXT(p, p_list);

	return p;
}
#define	PROCLIST_FOREACH(var, head)					\
	for ((var) = LIST_FIRST(head);					\
		((var) = _proclist_skipmarker(var)) != NULL;		\
		(var) = LIST_NEXT(var, p_list))
#endif /* T2EX */

/* Compatibility with old, non-interlocked tsleep call */
#define	tsleep(chan, pri, wmesg, timo)					\
	ltsleep(chan, pri, wmesg, timo, NULL)

#ifndef T2EX
#ifdef KSTACK_CHECK_MAGIC
void kstack_setup_magic(const struct lwp *);
void kstack_check_magic(const struct lwp *);
#endif

/*
 * kernel stack paramaters
 * XXX require sizeof(struct user)
 */
/* the lowest address of kernel stack */
#ifndef KSTACK_LOWEST_ADDR
#define	KSTACK_LOWEST_ADDR(l)	((void *)ALIGN((l)->l_addr + 1))
#endif
/* size of kernel stack */
#ifndef KSTACK_SIZE
#define	KSTACK_SIZE	(USPACE - ALIGN(sizeof(struct user)))
#endif
#endif /* T2EX */

#endif	/* _KERNEL */
#endif	/* !_SYS_PROC_H_ */
