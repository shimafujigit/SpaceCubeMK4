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

/*	$NetBSD: systm.h,v 1.228.4.2 2009/03/15 19:43:48 snj Exp $	*/

/*-
 * Copyright (c) 1982, 1988, 1991, 1993
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
 *	@(#)systm.h	8.7 (Berkeley) 3/29/95
 */

#ifndef _SYS_SYSTM_H_
#define _SYS_SYSTM_H_

#if defined(_KERNEL_OPT)
#include "opt_ddb.h"
#ifndef T2EX
#include "opt_multiprocessor.h"
#endif
#endif

#ifndef T2EX
#include <machine/endian.h>
#else
#include <sys/endian.h>
#endif

#ifdef _KERNEL
#include <sys/types.h>
#endif

struct clockframe;
#ifndef T2EX
struct device;
#endif
struct lwp;
struct proc;
struct timeval;
struct tty;
struct uio;
#ifndef T2EX
struct vnode;
struct vmspace;
struct vm_map;
#endif

#ifndef T2EX
extern const char *panicstr;	/* panic message */
extern int doing_shutdown;	/* shutting down */
#else
#define panicstr (NULL)
#define doing_shutdown (0)
#endif

extern const char copyright[];	/* system copyright */
extern char cpu_model[];	/* machine/cpu model name */
extern char machine[];		/* machine type */
extern char machine_arch[];	/* machine architecture */
extern const char osrelease[];	/* short system version */
extern const char ostype[];	/* system type */
extern const char kernel_ident[];/* kernel configuration ID */
extern const char version[];	/* system version */

extern int autonicetime;        /* time (in seconds) before autoniceval */
extern int autoniceval;         /* proc priority after autonicetime */

extern int selwait;		/* select timeout address */

#ifndef T2EX
extern int maxmem;		/* max memory per process */
extern int physmem;		/* physical memory */

extern dev_t dumpdev;		/* dump device */
extern dev_t dumpcdev;		/* dump device (character equivalent) */
extern long dumplo;		/* offset into dumpdev */
extern int dumpsize;		/* size of dump in pages */
extern const char *dumpspec;	/* how dump device was specified */

extern dev_t rootdev;		/* root device */
extern struct vnode *rootvp;	/* vnode equivalent to above */
extern struct device *root_device; /* device equivalent to above */
extern const char *rootspec;	/* how root device was specified */
#endif

extern int ncpu;		/* number of CPUs configured */
extern int ncpuonline;		/* number of CPUs online */
#if defined(_KERNEL)
extern bool mp_online;		/* secondary processors are started */
#endif /* defined(_KERNEL) */

extern const char hexdigits[];	/* "0123456789abcdef" in subr_prf2.c */
extern const char HEXDIGITS[];	/* "0123456789ABCDEF" in subr_prf2.c */

#ifndef T2EX
/*
 * These represent the swap pseudo-device (`sw').  This device
 * is used by the swap pager to indirect through the routines
 * in sys/vm/vm_swap.c.
 */
extern const dev_t swapdev;	/* swapping device */
extern struct vnode *swapdev_vp;/* vnode equivalent to above */

extern const dev_t zerodev;	/* /dev/zero */
#endif

typedef int	sy_call_t(struct lwp *, const void *, register_t *);

extern struct sysent {		/* system call table */
	short	sy_narg;	/* number of args */
	short	sy_argsize;	/* total size of arguments */
	int	sy_flags;	/* flags. see below */
	sy_call_t *sy_call;     /* implementing function */
} sysent[];
extern int nsysent;
#ifdef T2EX
#define SCARG(p, k)	((p)->a.k)
#else /* !T2EX */
#if	BYTE_ORDER == BIG_ENDIAN
#define	SCARG(p,k)	((p)->k.be.datum)	/* get arg from args pointer */
#elif	BYTE_ORDER == LITTLE_ENDIAN
#define	SCARG(p,k)	((p)->k.le.datum)	/* get arg from args pointer */
#else
#error	"what byte order is this machine?"
#endif
#endif /*T2EX*/

#ifndef T2EX
#define	SYCALL_INDIRECT	0x0002	/* indirect (ie syscall() or __syscall()) */

extern int boothowto;		/* reboot flags, from console subsystem */
#define	bootverbose	(boothowto & AB_VERBOSE)
#define	bootquiet	(boothowto & AB_QUIET)

extern void (*v_putc)(int); /* Virtual console putc routine */
#endif

/*
 * General function declarations.
 */
int	nullop(void *);
int	enodev(void);
int	enosys(void);
int	enoioctl(void);
int	enxio(void);
int	eopnotsupp(void);

enum hashtype {
	HASH_LIST,
	HASH_SLIST,
	HASH_TAILQ
};

#ifdef _KERNEL
void	*hashinit(u_int, enum hashtype, bool, u_long *);
void	hashdone(void *, enum hashtype, u_long);
int	seltrue(dev_t, int, struct lwp *);
int	sys_nosys(struct lwp *, const void *, register_t *);

#ifndef T2EX
void	aprint_normal(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	aprint_error(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	aprint_naive(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	aprint_verbose(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	aprint_debug(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));

struct device;

void	aprint_normal_dev(struct device *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_error_dev(struct device *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_naive_dev(struct device *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_verbose_dev(struct device *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_debug_dev(struct device *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));

struct ifnet;

void	aprint_normal_ifnet(struct ifnet *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_error_ifnet(struct ifnet *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_naive_ifnet(struct ifnet *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_verbose_ifnet(struct ifnet *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
void	aprint_debug_ifnet(struct ifnet *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));

int	aprint_get_error_count(void);

void	printf_tolog(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));

void	printf_nolog(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
#endif

void	printf(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
int	sprintf(char *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));
int	snprintf(char *, size_t, const char *, ...)
    __attribute__((__format__(__printf__,3,4)));
void	vprintf(const char *, _BSD_VA_LIST_);
int	vsprintf(char *, const char *, _BSD_VA_LIST_);
int	vsnprintf(char *, size_t, const char *, _BSD_VA_LIST_);
int	humanize_number(char *, size_t, uint64_t, const char *, int);

void	twiddle(void);
#endif /* _KERNEL */

#ifdef T2EX
void	tkn_panic(const char *, ...)
    __dead __attribute__((__format__(__printf__,1,2)));
#define panic	tkn_panic
#else
void	panic(const char *, ...)
    __dead __attribute__((__format__(__printf__,1,2)));
#endif
void	uprintf(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	uprintf_locked(const char *, ...)
    __attribute__((__format__(__printf__,1,2)));
void	ttyprintf(struct tty *, const char *, ...)
    __attribute__((__format__(__printf__,2,3)));

char	*bitmask_snprintf(u_quad_t, const char *, char *, size_t);

int	format_bytes(char *, size_t, uint64_t);

void	tablefull(const char *, const char *);

int	kcopy(const void *, void *, size_t);

#ifdef _KERNEL
#define bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define bzero(src, len)		memset((src), 0, (len))
#define bcmp(a, b, len)		memcmp((a), (b), (len))
#endif /* KERNEL */

int	copystr(const void *, void *, size_t, size_t *);
int	copyinstr(const void *, void *, size_t, size_t *);
int	copyoutstr(const void *, void *, size_t, size_t *);
int	copyin(const void *, void *, size_t);
int	copyout(const void *, void *, size_t);

#ifdef _KERNEL
typedef	int	(*copyin_t)(const void *, void *, size_t);
typedef int	(*copyout_t)(const void *, void *, size_t);
#endif

int	copyin_proc(struct proc *, const void *, void *, size_t);
int	copyout_proc(struct proc *, const void *, void *, size_t);
#ifndef T2EX
int	copyin_vmspace(struct vmspace *, const void *, void *, size_t);
int	copyout_vmspace(struct vmspace *, const void *, void *, size_t);
#endif

int	ioctl_copyin(int ioctlflags, const void *src, void *dst, size_t len);
int	ioctl_copyout(int ioctlflags, const void *src, void *dst, size_t len);

int	ucas_ptr(volatile void *, void *, void *, void *);
int	ucas_int(volatile int *, int, int, int *);

#ifndef T2EX
int	subyte(void *, int);
int	suibyte(void *, int);
int	susword(void *, short);
int	suisword(void *, short);
int	suswintr(void *, short);
int	suword(void *, long);
int	suiword(void *, long);

int	fubyte(const void *);
int	fuibyte(const void *);
int	fusword(const void *);
int	fuisword(const void *);
int	fuswintr(const void *);
long	fuword(const void *);
long	fuiword(const void *);
#endif

void	hardclock(struct clockframe *);
void	softclock(void *);
void	statclock(struct clockframe *);

#ifdef NTP
void	ntp_init(void);
#ifdef PPS_SYNC
void	hardpps(struct timespec *, long);
#endif /* PPS_SYNC */
#else
void	ntp_init(void);	/* also provides adjtime() functionality */
#endif /* NTP */

void	ssp_init(void);

#ifndef T2EX
void	initclocks(void);
void	inittodr(time_t);
void	resettodr(void);
void	cpu_initclocks(void);
void	setrootfstime(time_t);

void	startprofclock(struct proc *);
void	stopprofclock(struct proc *);
void	proftick(struct clockframe *);
void	setstatclockrate(int);

/*
 * Shutdown hooks.  Functions to be run with all interrupts disabled
 * immediately before the system is halted or rebooted.
 */
void	*shutdownhook_establish(void (*)(void *), void *);
void	shutdownhook_disestablish(void *);
void	doshutdownhooks(void);

/*
 * Power management hooks.
 */
void	*powerhook_establish(const char *, void (*)(int, void *), void *);
void	powerhook_disestablish(void *);
void	dopowerhooks(int);
#define PWR_RESUME	0
#define PWR_SUSPEND	1
#define PWR_STANDBY	2
#define PWR_SOFTRESUME	3
#define PWR_SOFTSUSPEND	4
#define PWR_SOFTSTANDBY	5
#define PWR_NAMES \
	"resume",	/* 0 */ \
	"suspend",	/* 1 */ \
	"standby",	/* 2 */ \
	"softresume",	/* 3 */ \
	"softsuspend",	/* 4 */ \
	"softstandby"	/* 5 */

/*
 * Mountroot hooks (and mountroot declaration).  Device drivers establish
 * these to be executed just before (*mountroot)() if the passed device is
 * selected as the root device.
 */
extern int (*mountroot)(void);
void	*mountroothook_establish(void (*)(struct device *), struct device *);
void	mountroothook_disestablish(void *);
void	mountroothook_destroy(void);
void	domountroothook(void);

/*
 * Exec hooks. Subsystems may want to do cleanup when a process
 * execs.
 */
void	*exechook_establish(void (*)(struct proc *, void *), void *);
void	exechook_disestablish(void *);
void	doexechooks(struct proc *);

/*
 * Exit hooks. Subsystems may want to do cleanup when a process exits.
 */
void	*exithook_establish(void (*)(struct proc *, void *), void *);
void	exithook_disestablish(void *);
void	doexithooks(struct proc *);

/*
 * Fork hooks.  Subsystems may want to do special processing when a process
 * forks.
 */
void	*forkhook_establish(void (*)(struct proc *, struct proc *));
void	forkhook_disestablish(void *);
void	doforkhooks(struct proc *, struct proc *);

/*
 * kernel syscall tracing/debugging hooks.
 */
#ifdef _KERNEL
bool	trace_is_enabled(struct proc *);
int	trace_enter(register_t, const register_t *, int);
void	trace_exit(register_t, register_t [], int);
#endif
#endif

int	uiomove(void *, size_t, struct uio *);
int	uiomove_frombuf(void *, size_t, struct uio *);

#ifndef T2EX
#ifdef _KERNEL
int	setjmp(label_t *);
void	longjmp(label_t *) __dead;
#endif

void	consinit(void);

void	cpu_startup(void);
void	cpu_configure(void);
void	cpu_rootconf(void);
void	cpu_dumpconf(void);

#ifdef GPROF
void	kmstartup(void);
#endif
#endif

#ifdef _KERNEL
#include <lib/libkern/libkern.h>

#ifndef T2EX
/*
 * Stuff to handle debugger magic key sequences.
 */
#define CNS_LEN			128
#define CNS_MAGIC_VAL(x)	((x)&0x1ff)
#define CNS_MAGIC_NEXT(x)	(((x)>>9)&0x7f)
#define CNS_TERM		0x7f	/* End of sequence */

typedef struct cnm_state {
	int	cnm_state;
	u_short	*cnm_magic;
} cnm_state_t;

/* Override db_console() in MD headers */
#ifndef cn_trap
#define cn_trap()	console_debugger()
#endif
#ifndef cn_isconsole
#define cn_isconsole(d)	(cn_tab != NULL && (d) == cn_tab->cn_dev)
#endif

void cn_init_magic(cnm_state_t *);
void cn_destroy_magic(cnm_state_t *);
int cn_set_magic(const char *);
int cn_get_magic(char *, size_t);
/* This should be called for each byte read */
#ifndef cn_check_magic
#define cn_check_magic(d, k, s)						\
	do {								\
		if (cn_isconsole(d)) {					\
			int _v = (s).cnm_magic[(s).cnm_state];		\
			if ((k) == CNS_MAGIC_VAL(_v)) {			\
				(s).cnm_state = CNS_MAGIC_NEXT(_v);	\
				if ((s).cnm_state == CNS_TERM) {	\
					cn_trap();			\
					(s).cnm_state = 0;		\
				}					\
			} else {					\
				(s).cnm_state = 0;			\
			}						\
		}							\
	} while (/* CONSTCOND */ 0)
#endif

/* Encode out-of-band events this way when passing to cn_check_magic() */
#define	CNC_BREAK		0x100

#if defined(DDB) || defined(sun3) || defined(sun2)
/* note that cpu_Debugger() is always available on sun[23] */
void	cpu_Debugger(void);
#define Debugger	cpu_Debugger
#endif

#ifdef DDB
/*
 * Enter debugger(s) from console attention if enabled
 */
extern int db_fromconsole; /* XXX ddb/ddbvar.h */
#define console_debugger() if (db_fromconsole) Debugger()
#elif defined(Debugger)
#define console_debugger() Debugger()
#else
#define console_debugger() do {} while (/* CONSTCOND */ 0) /* NOP */
#endif
#endif
#endif /* _KERNEL */

#ifndef T2EX
/* For SYSCALL_DEBUG */
void scdebug_call(register_t, const register_t[]);
void scdebug_ret(register_t, int, const register_t[]);
#endif

void	kernel_lock_init(void);
void	_kernel_lock(int);
void	_kernel_unlock(int, int *);

#if defined(MULTIPROCESSOR) || defined(_LKM)
#define	KERNEL_LOCK(count, lwp)			\
do {						\
	if ((count) != 0)			\
		_kernel_lock((count));	\
} while (/* CONSTCOND */ 0)
#define	KERNEL_UNLOCK(all, lwp, p)	_kernel_unlock((all), (p))
#else
#define	KERNEL_LOCK(count, lwp)		do {(void)(count); (void)(lwp);} while (/* CONSTCOND */ 0) /*NOP*/
#define	KERNEL_UNLOCK(all, lwp, ptr)	do {(void)(all); (void)(lwp); (void)(ptr);} while (/* CONSTCOND */ 0) /*NOP*/
#endif

#define	KERNEL_UNLOCK_LAST(l)		KERNEL_UNLOCK(-1, (l), NULL)
#define	KERNEL_UNLOCK_ALL(l, p)		KERNEL_UNLOCK(0, (l), (p))
#define	KERNEL_UNLOCK_ONE(l)		KERNEL_UNLOCK(1, (l), NULL)

/* Preemption control. */
#ifdef _KERNEL
void	kpreempt_disable(void);
void	kpreempt_enable(void);
bool	kpreempt_disabled(void);
#endif

#ifndef T2EX
void assert_sleepable(void);
#if defined(DEBUG)
#define	ASSERT_SLEEPABLE()	assert_sleepable()
#else /* defined(DEBUG) */
#define	ASSERT_SLEEPABLE()	/* nothing */
#endif /* defined(DEBUG) */
#else
#define	ASSERT_SLEEPABLE()	/* nothing */
#endif

#ifndef T2EX
vaddr_t calc_cache_size(struct vm_map *, int, int);
#endif

#endif	/* !_SYS_SYSTM_H_ */
