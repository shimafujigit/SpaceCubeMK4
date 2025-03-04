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

/*	$NetBSD: types.h,v 1.80 2008/02/26 13:37:09 simonb Exp $	*/

/*-
 * Copyright (c) 1982, 1986, 1991, 1993, 1994
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
 *	@(#)types.h	8.4 (Berkeley) 1/21/94
 */

#ifndef _SYS_TYPES_H_
#define	_SYS_TYPES_H_

#include <sys/featuretest.h>

/* Machine type dependent parameters. */
#include <machine/types.h>

#include <machine/ansi.h>
#include <machine/int_types.h>


#include <sys/ansi.h>

#ifndef	int8_t
typedef	__int8_t	int8_t;
#define	int8_t		__int8_t
#endif

#ifndef	uint8_t
typedef	__uint8_t	uint8_t;
#define	uint8_t		__uint8_t
#endif

#ifndef	int16_t
typedef	__int16_t	int16_t;
#define	int16_t		__int16_t
#endif

#ifndef	uint16_t
typedef	__uint16_t	uint16_t;
#define	uint16_t	__uint16_t
#endif

#ifndef	int32_t
typedef	__int32_t	int32_t;
#define	int32_t		__int32_t
#endif

#ifndef	uint32_t
typedef	__uint32_t	uint32_t;
#define	uint32_t	__uint32_t
#endif

#ifndef	int64_t
typedef	__int64_t	int64_t;
#define	int64_t		__int64_t
#endif

#ifndef	uint64_t
typedef	__uint64_t	uint64_t;
#define	uint64_t	__uint64_t
#endif

typedef	uint8_t		u_int8_t;
typedef	uint16_t	u_int16_t;
typedef	uint32_t	u_int32_t;
typedef	uint64_t	u_int64_t;

#ifndef T2EX
#include <machine/endian.h>
#else
#include <sys/endian.h>
#endif

#if defined(_NETBSD_SOURCE)
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef unsigned char	unchar;		/* Sys V compatibility */
typedef	unsigned short	ushort;		/* Sys V compatibility */
typedef	unsigned int	uint;		/* Sys V compatibility */
typedef unsigned long	ulong;		/* Sys V compatibility */
#endif

typedef	uint64_t	u_quad_t;	/* quads */
typedef	int64_t		quad_t;
typedef	quad_t *	qaddr_t;

#ifdef T2EX
#include_next <sys/types.h>
#endif

/*
 * The types longlong_t and u_longlong_t exist for use with the
 * Sun-derived XDR routines involving these types, and their usage
 * in other contexts is discouraged.  Further note that these types
 * may not be equivalent to "long long" and "unsigned long long",
 * they are only guaranteed to be signed and unsigned 64-bit types
 * respectively.  Portable programs that need 64-bit types should use
 * the C99 types int64_t and uint64_t instead.
 */

typedef	int64_t		longlong_t;	/* for XDR */
typedef	uint64_t	u_longlong_t;	/* for XDR */

#ifndef T2EX
typedef	int64_t		blkcnt_t;	/* fs block count */
typedef	uint32_t	blksize_t;	/* fs optimal block size */
#endif

#ifndef T2EX
#ifndef	fsblkcnt_t
typedef	__fsblkcnt_t	fsblkcnt_t;	/* fs block count (statvfs) */
#define fsblkcnt_t	__fsblkcnt_t
#endif

#ifndef	fsfilcnt_t
typedef	__fsfilcnt_t	fsfilcnt_t;	/* fs file count */
#define fsfilcnt_t	__fsfilcnt_t
#endif
#endif

#if !defined(_KERNEL) && !defined(_STANDALONE)
/* We don't and shouldn't use caddr_t in the kernel anymore */
#ifndef	caddr_t
typedef	__caddr_t	caddr_t;	/* core address */
#define	caddr_t		__caddr_t
#endif
#else
#ifndef	caddr_t
typedef	__caddr_t	caddr_t;	/* core address */
#define	caddr_t		__caddr_t
#endif
#endif

#ifdef __daddr_t
typedef	__daddr_t	daddr_t;	/* disk address */
#undef __daddr_t
#else
typedef	int64_t		daddr_t;	/* disk address */
#endif

#ifndef T2EX
typedef	uint32_t	dev_t;		/* device number */
#else
#ifdef	__dev_t
typedef __dev_t		dev_t;
#undef	__dev_t
#endif
#endif

typedef	uint32_t	fixpt_t;	/* fixed point number */

#ifndef T2EX
#ifndef	gid_t
typedef	__gid_t		gid_t;		/* group id */
#define	gid_t		__gid_t
#endif
#else
#ifdef	__gid_t
typedef	__gid_t		gid_t;
#undef	__gid_t
#endif
#endif

typedef	int		idtype_t;	/* type of the id */
typedef	uint32_t	id_t;		/* group id, process id or user id */
#ifndef T2EX
typedef	uint32_t	ino_t;		/* inode number */
#else
#ifdef	__ino_t
typedef	__ino_t		ino_t;
#undef	__ino_t
#endif
#endif
typedef	long		key_t;		/* IPC key (for Sys V IPC) */

#ifndef T2EX
#ifndef	mode_t
typedef	__mode_t	mode_t;		/* permissions */
#define	mode_t		__mode_t
#endif
#else
#ifdef	__mode_t
typedef	__mode_t	mode_t;
#undef	__mode_t
#endif
#endif

#ifndef T2EX
typedef	uint32_t	nlink_t;	/* link count */
#else
#ifdef	__nlink_t
typedef __nlink_t	nlink_t;
#undef	__nlink_t
#endif
#endif

#ifndef T2EX
#ifndef	off_t
typedef	__off_t		off_t;		/* file offset */
#define	off_t		__off_t
#endif
#else
#ifdef	__off_t
typedef	__off_t		off_t;
#undef	__off_t
#endif
#endif

#ifndef	pid_t
typedef	__pid_t		pid_t;		/* process id */
#define	pid_t		__pid_t
#endif

typedef int32_t		lwpid_t;	/* LWP id */
typedef quad_t		rlim_t;		/* resource limit */
typedef	int32_t		segsz_t;	/* segment size */
typedef	int32_t		swblk_t;	/* swap offset */

#ifndef T2EX
#ifndef	uid_t
typedef	__uid_t		uid_t;		/* user id */
#define	uid_t		__uid_t
#endif
#else
#ifdef	__uid_t
typedef	__uid_t		uid_t;
#undef	__uid_t
#endif
#endif

typedef int		mqd_t;

typedef	unsigned long	cpuid_t;

typedef	int		psetid_t;

#if defined(_KERNEL) || defined(_STANDALONE)
/*
 * Boolean type definitions for the kernel environment.  User-space
 * boolean definitions are found in <stdbool.h>.
 */
#define bool	_Bool
#define true	1
#define false	0

/*
 * Deprecated Mach-style boolean_t type.  Should not be used by new code.
 */
typedef int	boolean_t;
#ifndef TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif

#endif /* _KERNEL || _STANDALONE */

#if defined(_KERNEL) || defined(_LIBC)
/*
 * semctl(2)'s argument structure.  This is here for the benefit of
 * <sys/syscallargs.h>.  It is not in the user's namespace in SUSv2.
 * The SUSv2 semctl(2) takes variable arguments.
 */
union __semun {
	int		val;		/* value for SETVAL */
	struct semid_ds	*buf;		/* buffer for IPC_STAT & IPC_SET */
	unsigned short	*array;		/* array for GETALL & SETALL */
};
/* For the same reason as above */
#include <sys/stdint.h>
typedef intptr_t semid_t;
#endif /* _KERNEL || _LIBC */

#ifndef T2EX
/*
 * These belong in unistd.h, but are placed here too to ensure that
 * long arguments will be promoted to off_t if the program fails to
 * include that header or explicitly cast them to off_t.
 */
#if defined(_NETBSD_SOURCE)
#ifndef __OFF_T_SYSCALLS_DECLARED
#define __OFF_T_SYSCALLS_DECLARED
#ifndef _KERNEL
#include <sys/cdefs.h>
__BEGIN_DECLS
off_t	 lseek(int, off_t, int);
int	 ftruncate(int, off_t);
int	 truncate(const char *, off_t);
__END_DECLS
#endif /* !_KERNEL */
#endif /* __OFF_T_SYSCALLS_DECLARED */
#endif /* defined(_NETBSD_SOURCE) */
#endif /* T2EX */

#if defined(_NETBSD_SOURCE)
/* Major, minor numbers, dev_t's. */
#define	major(x)	((int32_t)((((x) & 0x000fff00) >>  8)))
#define	minor(x)	((int32_t)((((x) & 0xfff00000) >> 12) | \
				   (((x) & 0x000000ff) >>  0)))
#define	makedev(x,y)	((dev_t)((((x) <<  8) & 0x000fff00) | \
				 (((y) << 12) & 0xfff00000) | \
				 (((y) <<  0) & 0x000000ff)))
#endif

#ifndef T2EX
#ifdef	_BSD_CLOCK_T_
typedef	_BSD_CLOCK_T_		clock_t;
#undef	_BSD_CLOCK_T_
#endif

#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_		size_t;
#define _SIZE_T
#undef	_BSD_SIZE_T_
#endif

#ifdef	_BSD_SSIZE_T_
typedef	_BSD_SSIZE_T_		ssize_t;
#undef	_BSD_SSIZE_T_
#endif

#ifdef	_BSD_TIME_T_
typedef	_BSD_TIME_T_		time_t;
#undef	_BSD_TIME_T_
#endif

#else /* T2EX */

#ifdef	__clock_t
typedef	__clock_t	clock_t;
#undef	__clock_t
#endif

#ifdef	__size_t
typedef __size_t	size_t;
#undef	__size_t
#endif

#ifdef	__ssize_t
typedef	__ssize_t	ssize_t;
#undef	__ssize_t
#endif

#ifdef	__time_t
typedef	__time_t	time_t;
#undef	__time_t
#endif
#endif /* T2EX */

#ifdef	_BSD_CLOCKID_T_
typedef	_BSD_CLOCKID_T_		clockid_t;
#undef	_BSD_CLOCKID_T_
#endif

#ifdef	_BSD_TIMER_T_
typedef	_BSD_TIMER_T_		timer_t;
#undef	_BSD_TIMER_T_
#endif

#ifdef	_BSD_SUSECONDS_T_
typedef	_BSD_SUSECONDS_T_	suseconds_t;
#undef	_BSD_SUSECONDS_T_
#endif

#ifdef	_BSD_USECONDS_T_
typedef	_BSD_USECONDS_T_	useconds_t;
#undef	_BSD_USECONDS_T_
#endif

#ifdef _NETBSD_SOURCE
#include <sys/fd_set.h>
#define	NBBY	__NBBY

typedef struct kauth_cred *kauth_cred_t;

typedef int pri_t;

#endif

#if defined(__STDC__) && defined(_KERNEL)
/*
 * Forward structure declarations for function prototypes.  We include the
 * common structures that cross subsystem boundaries here; others are mostly
 * used in the same place that the structure is defined.
 */
struct	lwp;
typedef struct lwp lwp_t;
struct	user;
struct	__ucontext;
struct	proc;
typedef struct proc proc_t;
struct	pgrp;
struct	rusage;
struct	file;
typedef struct file file_t;
struct	buf;
typedef struct buf buf_t;
struct	tty;
struct	uio;
#endif

#ifdef _KERNEL
#define SET(t, f)	((t) |= (f))
#define	ISSET(t, f)	((t) & (f))
#define	CLR(t, f)	((t) &= ~(f))
#endif

#ifndef T2EX
#if !defined(_KERNEL) && !defined(_STANDALONE)
#if (_POSIX_C_SOURCE - 0L) >= 199506L || (_XOPEN_SOURCE - 0) >= 500 || \
    defined(_NETBSD_SOURCE)
#include <pthread_types.h>
#endif
#endif
#endif /* T2EX */

#endif /* !_SYS_TYPES_H_ */
