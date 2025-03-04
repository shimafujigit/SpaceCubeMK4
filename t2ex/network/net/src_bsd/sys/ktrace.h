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

/*	$NetBSD: ktrace.h,v 1.55 2008/10/20 11:36:39 ad Exp $	*/

/*
 * Copyright (c) 1988, 1993
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
 *	@(#)ktrace.h	8.2 (Berkeley) 2/19/95
 */

#ifndef _SYS_KTRACE_H_
#define _SYS_KTRACE_H_

#include <sys/mutex.h>

/*
 * operations to ktrace system call  (KTROP(op))
 */
#define KTROP_SET		0	/* set trace points */
#define KTROP_CLEAR		1	/* clear trace points */
#define KTROP_CLEARFILE		2	/* stop all tracing to file */
#define	KTROP_MASK		0x3
#define	KTROP(o)		((o)&KTROP_MASK) /* macro to extract operation */
/*
 * flags (ORed in with operation)
 */
#define KTRFLAG_DESCEND		4	/* perform op on all children too */

/*
 * ktrace record header
 */
struct ktr_header {
	int	ktr_len;		/* length of record minus length of old header */
#if BYTE_ORDER == LITTLE_ENDIAN
	short	ktr_type;		/* trace record type */
	short	ktr_version;		/* trace record version */
#else
	short	ktr_version;		/* trace record version */
	short	ktr_type;		/* trace record type */
#endif
	pid_t	ktr_pid;		/* process id */
	char	ktr_comm[MAXCOMLEN+1];	/* command name */
	union {
		struct timeval _tv;	/* v0 timestamp */
		struct timespec _ts;	/* v1 timespec */
	} _ktr_time;
	union {
		const void *_buf;	/* v0 unused */
		lwpid_t _lid;		/* v1 lwp id */
	} _ktr_id;
};

#define ktr_lid	_ktr_id._lid
#define ktr_time _ktr_time._ts
#define ktr_tv _ktr_time._tv
#define ktr_ts _ktr_time._ts
#define ktr_unused _ktr_id._buf

#define	KTR_SHIMLEN	offsetof(struct ktr_header, ktr_pid)

/*
 * Test for kernel trace point
 */
#if !defined(T2EX) || defined(KTRACE)
#define KTRPOINT(p, type)	\
	(((p)->p_traceflag & (1<<(type))) != 0)
#else
#define KTRPOINT(p, type)	0
#endif /* !defined(T2EX) || defined(KTRACE) */


/*
 * ktrace record types
 */

/*
 * KTR_SYSCALL - system call record
 */
#define KTR_SYSCALL	1
struct ktr_syscall {
	int	ktr_code;		/* syscall number */
	int	ktr_argsize;		/* size of arguments */
	/*
	 * followed by ktr_argsize/sizeof(register_t) "register_t"s
	 */
};

/*
 * KTR_SYSRET - return from system call record
 */
#define KTR_SYSRET	2
struct ktr_sysret {
	short	ktr_code;
	short	ktr_eosys;		/* XXX unused */
	int	ktr_error;
	register_t ktr_retval;
	register_t ktr_retval_1;
};

/*
 * KTR_NAMEI - namei record
 */
#define KTR_NAMEI	3
	/* record contains pathname */

/*
 * KTR_GENIO - trace generic process i/o
 */
#define KTR_GENIO	4
struct ktr_genio {
	int	ktr_fd;
	enum	uio_rw ktr_rw;
	/*
	 * followed by data successfully read/written
	 */
};

#ifndef T2EX
/*
 * KTR_PSIG - trace processed signal
 */
#define	KTR_PSIG	5
struct ktr_psig {
	int	signo;
	sig_t	action;
	sigset_t mask;
	int	code;
	/*
	 * followed by optional siginfo_t
	 */
};
#endif

/*
 * KTR_CSW - trace context switches
 */
#define KTR_CSW		6
struct ktr_csw {
	int	out;	/* 1 if switch out, 0 if switch in */
	int	user;	/* 1 if usermode (ivcsw), 0 if kernel (vcsw) */
};

/*
 * KTR_EMUL - emulation change
 */
#define KTR_EMUL	7
	/* record contains emulation name */

/*
 * KTR_USER - user record
 */
#define	KTR_USER	8
#define KTR_USER_MAXIDLEN	20
#define KTR_USER_MAXLEN		2048	/* maximum length of passed data */
struct ktr_user {
	char 	ktr_id[KTR_USER_MAXIDLEN];	/* string id of caller */
	/*
	 * Followed by ktr_len - sizeof(struct ktr_user) of user data.
	 */
};

/*
 * KTR_MMSG - Mach message
 */
#define KTR_MMSG		9
struct ktr_mmsg {
	/*
	 * This is a Mach message header
	 */
	int	ktr_bits;
	int	ktr_size;
	int	ktr_remote_port;
	int	ktr_local_port;
	int	ktr_reserved;
	int	ktr_id;
	/*
	 * Followed by ktr_size - sizeof(mach_msg_header_t) of message payload
	 */
};

/*
 * KTR_EXEC_ARG, KTR_EXEC_ENV - Arguments and environment from exec
 */
#define KTR_EXEC_ARG		10
#define KTR_EXEC_ENV		11
	/* record contains arg/env string */

/*
 * KTR_MOOL - Mach Out Of Line data
 */
#define KTR_MOOL		12
struct ktr_mool {
	const void 	*uaddr;	/* User address */
	size_t		size;	/* Data len */
	/* Followed by size bytes of data */
};

/*
 * KTR_SAUPCALL - scheduler activated upcall.
 */
#define	KTR_SAUPCALL	13
struct ktr_saupcall {
	int ktr_type;
	int ktr_nevent;
	int ktr_nint;
	void *ktr_sas;
	void *ktr_ap;
	/*
	 * followed by nevent sa_t's from sas[]
	 */
};

/*
 * KTR_MIB - MIB name and data
 */
#define KTR_MIB		14
	/* Record contains MIB name */


/*
 * kernel trace points (in p_traceflag)
 */
#define KTRFAC_MASK	0x00ffffff
#define KTRFAC_SYSCALL	(1<<KTR_SYSCALL)
#define KTRFAC_SYSRET	(1<<KTR_SYSRET)
#define KTRFAC_NAMEI	(1<<KTR_NAMEI)
#define KTRFAC_GENIO	(1<<KTR_GENIO)
#define	KTRFAC_PSIG	(1<<KTR_PSIG)
#define KTRFAC_CSW	(1<<KTR_CSW)
#define KTRFAC_EMUL	(1<<KTR_EMUL)
#define	KTRFAC_USER	(1<<KTR_USER)
#define KTRFAC_MMSG	(1<<KTR_MMSG)
#define KTRFAC_EXEC_ARG	(1<<KTR_EXEC_ARG)
#define KTRFAC_EXEC_ENV	(1<<KTR_EXEC_ENV)
#define KTRFAC_MOOL	(1<<KTR_MOOL)
#define	KTRFAC_SAUPCALL	(1<<KTR_SAUPCALL)
#define	KTRFAC_MIB	(1<<KTR_MIB)
/*
 * trace flags (also in p_traceflags)
 */
#define KTRFAC_PERSISTENT	0x80000000	/* persistent trace across sugid
						   exec (exclusive) */
#define KTRFAC_INHERIT	0x40000000	/* pass trace flags to children */
#define KTRFAC_TRC_EMUL	0x10000000	/* ktrace KTR_EMUL before next trace */
#define	KTRFAC_VER_MASK	0x0f000000	/* record version mask */
#define	KTRFAC_VER_SHIFT	24	/* record version shift */

#define	KTRFAC_VERSION(tf)	(((tf) & KTRFAC_VER_MASK) >> KTRFAC_VER_SHIFT)

#define	KTRFACv0	(0 << KTRFAC_VER_SHIFT)
#define	KTRFACv1	(1 << KTRFAC_VER_SHIFT)

#ifndef	_KERNEL

#include <sys/cdefs.h>

__BEGIN_DECLS
int	ktrace(const char *, int, int, pid_t);
int	fktrace(int, int, int, pid_t);
int	utrace(const char *, void *, size_t);
__END_DECLS

#else

#if !defined(T2EX) || defined(KTRACE)

void ktrinit(void);
void ktrderef(struct proc *);
void ktradref(struct proc *);

extern kmutex_t ktrace_lock;
extern int ktrace_on;

int ktruser(const char *, void *, size_t, int);
bool ktr_point(int);

void ktr_csw(int, int);
void ktr_emul(void);
void ktr_geniov(int, enum uio_rw, struct iovec *, size_t, int);
void ktr_genio(int, enum uio_rw, const void *, size_t, int);
void ktr_mibio(int, enum uio_rw, const void *, size_t, int);
void ktr_namei(const char *, size_t);
void ktr_namei2(const char *, size_t, const char *, size_t);
void ktr_psig(int, sig_t, const sigset_t *, const ksiginfo_t *);
void ktr_syscall(register_t, const register_t [], int);
void ktr_sysret(register_t, int, register_t *);
void ktr_kuser(const char *, void *, size_t);
void ktr_mmsg(const void *, size_t);
void ktr_mib(const int *a , u_int b);
void ktr_mool(const void *, size_t, const void *);
void ktr_execarg(const void *, size_t);
void ktr_execenv(const void *, size_t);
void ktr_saupcall(struct lwp *, int, int, int, void *, void *, void *);

static inline bool
ktrpoint(int fac)
{
    return __predict_false(ktrace_on) && __predict_false(ktr_point(1 << fac));
}

static inline void
ktrcsw(int a, int b)
{
	if (__predict_false(ktrace_on))
		ktr_csw(a, b);
}

static inline void
ktremul(void)
{
	if (__predict_false(ktrace_on))
		ktr_emul();
}

static inline void
ktrgenio(int a, enum uio_rw b, const void *c, size_t d, int e)
{
	if (__predict_false(ktrace_on))
		ktr_genio(a, b, c, d, e);
}

static inline void
ktrgeniov(int a, enum uio_rw b, struct iovec *c, int d, int e)
{
	if (__predict_false(ktrace_on))
		ktr_geniov(a, b, c, d, e);
}

static inline void
ktrmibio(int a, enum uio_rw b, const void *c, size_t d, int e)
{
	if (__predict_false(ktrace_on))
		ktr_mibio(a, b, c, d, e);
}

static inline void
ktrnamei(const char *a, size_t b)
{
	if (__predict_false(ktrace_on))
		ktr_namei(a, b);
}

static inline void
ktrnamei2(const char *a, size_t b, const char *c, size_t d)
{
	if (__predict_false(ktrace_on))
		ktr_namei2(a, b, c, d);
}

static inline void
ktrpsig(int a, sig_t b, const sigset_t *c, const ksiginfo_t * d)
{
	if (__predict_false(ktrace_on))
		ktr_psig(a, b, c, d);
}

static inline void
ktrsyscall(register_t code, const register_t args[], int narg)
{
	if (__predict_false(ktrace_on))
		ktr_syscall(code, args, narg);
}

static inline void
ktrsysret(register_t a, int b, register_t *c)
{
	if (__predict_false(ktrace_on))
		ktr_sysret(a, b, c);
}

static inline void
ktrkuser(const char *a, void *b, size_t c)
{
	if (__predict_false(ktrace_on))
		ktr_kuser(a, b, c);
}

static inline void
ktrmmsg(const void *a, size_t b)
{
	if (__predict_false(ktrace_on))
		ktr_mmsg(a, b);
}

static inline void
ktrmib(const int *a , u_int b)
{
	if (__predict_false(ktrace_on))
		ktr_mib(a, b);
}

static inline void
ktrmool(const void *a, size_t b, const void *c)
{
	if (__predict_false(ktrace_on))
		ktr_mool(a, b, c);
}

static inline void
ktrexecarg(const void *a, size_t b)
{
	if (__predict_false(ktrace_on))
		ktr_execarg(a, b);
}

static inline void
ktrexecenv(const void *a, size_t b)
{
	if (__predict_false(ktrace_on))
		ktr_execenv(a, b);
}

static inline void
ktrsaupcall(struct lwp *a, int b, int c, int d, void *e, void *f, void *g)
{
	if (__predict_false(ktrace_on))
		ktr_saupcall(a, b, c, d, e, f, g);
}

#else

#define ktrinit()
#define ktrderef(a)
#define ktradref(a)

#define ktruser(a, b, c, d)
#define ktr_point(a)

#define ktr_csw(a, b)
#define ktr_emul()
#define ktr_geniov(a, b, c, d, e)
#define ktr_genio(a, b, c, d, e)
#define ktr_mibio(a, b, c, d, e)
#define ktr_namei(a, b)
#define ktr_namei2(a, b, c, d)
#define ktr_psig(a, b, c, d)
#define ktr_syscall(a, b, c)
#define ktr_sysret(a, b, c)
#define ktr_kuser(a, b, c)
#define ktr_mmsg(a, b)
#define ktr_mib(a, b)
#define ktr_mool(a, b, c)
#define ktr_execarg(a, b)
#define ktr_execenv(a, b)
#define ktr_saupcall(a, b, c, d, e, f, g)

#define ktrpoint(a)	0
#define ktrcsw(a, b)
#define ktremul()
#define ktrgenio(a, b, c, d, e)
#define ktrgeniov(a, b, c, d, e)
#define ktrmibio(a, b, c, d, e)
#define ktrnamei(a, b)
#define ktrnamei2(a, b, c, d)
#define ktrpsig(a, b, c, d)
#define ktrsyscall(a, b, c)
#define ktrsysret(a, b, c)
#define ktrkuser(a, b, c)
#define ktrmmsg(a, b)
#define ktrmib(a, b)
#define ktrmool(a, b, c)
#define ktrexecarg(a, b)
#define ktrexecenv(a, b)
#define ktrsaupcall(a, b, c, d, e, f, g)

#endif /* !defined(T2EX) || defined(KTRACE) */

#endif	/* !_KERNEL */

#endif /* _SYS_KTRACE_H_ */
