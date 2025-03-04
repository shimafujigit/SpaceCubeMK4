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

/*	$NetBSD: unistd.h,v 1.47 2008/08/27 08:58:16 christos Exp $	*/

/*
 * Copyright (c) 1989, 1993
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
 *	@(#)unistd.h	8.2 (Berkeley) 1/7/94
 */

#ifndef _SYS_UNISTD_H_
#define	_SYS_UNISTD_H_

#include <sys/featuretest.h>

/* compile-time symbolic constants */
#define	_POSIX_JOB_CONTROL	1
				/* implementation supports job control */

/*
 * According to POSIX 1003.1:
 * "The saved set-user-ID capability allows a program to regain the
 * effective user ID established at the last exec call."
 * However, the setuid/setgid function as specified by POSIX 1003.1 does
 * not allow changing the effective ID from the super-user without also
 * changed the saved ID, so it is impossible to get super-user privileges
 * back later.  Instead we provide this feature independent of the current
 * effective ID through the seteuid/setegid function.  In addition, we do
 * not use the saved ID as specified by POSIX 1003.1 in setuid/setgid,
 * because this would make it impossible for a set-user-ID executable
 * owned by a user other than the super-user to permanently revoke its
 * extra privileges.
 */
#ifdef	_NOT_AVAILABLE
#define	_POSIX_SAVED_IDS	1
				/* saved set-user-ID and set-group-ID */
#endif

#define	_POSIX_VERSION		199009L
#define	_POSIX2_VERSION		199212L

/* execution-time symbolic constants */
				/* asynchronous I/O is available */
#define	_POSIX_ASYNCHRONOUS_IO	200112L
				/* chown requires appropriate privileges */
#define	_POSIX_CHOWN_RESTRICTED	1
				/* clock selection */
#define	_POSIX_CLOCK_SELECTION	-1
				/* too-long path components generate errors */
#define	_POSIX_NO_TRUNC		1
				/* may disable terminal special characters */
#define	_POSIX_VDISABLE		((unsigned char)'\377')
				/* file synchronization is available */
#define	_POSIX_FSYNC		1
				/* synchronized I/O is available */
#define	_POSIX_SYNCHRONIZED_IO	1
				/* memory mapped files */
#define	_POSIX_MAPPED_FILES	1
				/* memory locking of whole address space */
#define	_POSIX_MEMLOCK		1
				/* memory locking address ranges */
#define	_POSIX_MEMLOCK_RANGE	1
				/* memory access protections */
#define	_POSIX_MEMORY_PROTECTION 1
				/* message passing is available */
#define	_POSIX_MESSAGE_PASSING	200112L
				/* monotonic clock */
#define	_POSIX_MONOTONIC_CLOCK	200112L
				/* priority scheduling */
#define	_POSIX_PRIORITY_SCHEDULING	200112L
				/* threads */
#define	_POSIX_THREADS		200112L
				/* _r functions */
#define	_POSIX_THREAD_SAFE_FUNCTIONS	200112L
				/* pthread_attr functions for stack size */
#define	_POSIX_THREAD_ATTR_STACKSIZE	200112L
				/* pthread_attr functions for stack address */
#define	_POSIX_THREAD_ATTR_STACKADDR	200112L
				/* semaphores */
#define	_POSIX_SEMAPHORES	0
				/* barriers */
#define	_POSIX_BARRIERS		200112L
				/* timers */
#define	_POSIX_TIMERS		200112L
				/* spin locks */
#define	_POSIX_SPIN_LOCKS	200112L
				/* read/write locks */
#define	_POSIX_READER_WRITER_LOCKS	200112L
				/* XPG4.2 shared memory */
#define	_XOPEN_SHM		0
				/* shell */
#define	_POSIX_SHELL		1
				/* regular expressions */
#define	_POSIX_REGEXP		1
				/* C binding */
#define	_POSIX2_C_BIND		200112L

/* access function */
#define	F_OK		0	/* test for existence of file */
#define	X_OK		0x01	/* test for execute or search permission */
#define	W_OK		0x02	/* test for write permission */
#define	R_OK		0x04	/* test for read permission */

/* whence values for lseek(2) */
#define	SEEK_SET	0	/* set file offset to offset */
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#define	SEEK_END	2	/* set file offset to EOF plus offset */

#if defined(_NETBSD_SOURCE)
/* whence values for lseek(2); renamed by POSIX 1003.1 */
#define	L_SET		SEEK_SET
#define	L_INCR		SEEK_CUR
#define	L_XTND		SEEK_END

/*
 * fsync_range values.
 *
 * Note the following flag values were chosen to not overlap
 * values for SEEK_XXX flags.  While not currently implemented,
 * it is possible to extend this call to respect SEEK_CUR and
 * SEEK_END offset addressing modes.
 */
#define	FDATASYNC	0x0010	/* sync data and minimal metadata */
#define	FFILESYNC	0x0020	/* sync data and metadata */
#define	FDISKSYNC	0x0040	/* flush disk caches after sync */
#endif

/* configurable pathname variables; use as argument to pathconf(3) */
#define	_PC_LINK_MAX		 1
#define	_PC_MAX_CANON		 2
#define	_PC_MAX_INPUT		 3
#define	_PC_NAME_MAX		 4
#define	_PC_PATH_MAX		 5
#define	_PC_PIPE_BUF		 6
#define	_PC_CHOWN_RESTRICTED	 7
#define	_PC_NO_TRUNC		 8
#define	_PC_VDISABLE		 9
#define	_PC_SYNC_IO		10
#define	_PC_FILESIZEBITS	11
#define	_PC_SYMLINK_MAX		12
#define	_PC_2_SYMLINKS		13

/* configurable system variables; use as argument to sysconf(3) */
/*
 * XXX The value of _SC_CLK_TCK is embedded in <time.h>.
 * XXX The value of _SC_PAGESIZE is embedded in <sys/shm.h>.
 */
#define	_SC_ARG_MAX		 1
#define	_SC_CHILD_MAX		 2
#define	_O_SC_CLK_TCK		 3 /* Old version, always 100 */
#define	_SC_NGROUPS_MAX		 4
#define	_SC_OPEN_MAX		 5
#define	_SC_JOB_CONTROL		 6
#define	_SC_SAVED_IDS		 7
#define	_SC_VERSION		 8
#define	_SC_BC_BASE_MAX		 9
#define	_SC_BC_DIM_MAX		10
#define	_SC_BC_SCALE_MAX	11
#define	_SC_BC_STRING_MAX	12
#define	_SC_COLL_WEIGHTS_MAX	13
#define	_SC_EXPR_NEST_MAX	14
#define	_SC_LINE_MAX		15
#define	_SC_RE_DUP_MAX		16
#define	_SC_2_VERSION		17
#define	_SC_2_C_BIND		18
#define	_SC_2_C_DEV		19
#define	_SC_2_CHAR_TERM		20
#define	_SC_2_FORT_DEV		21
#define	_SC_2_FORT_RUN		22
#define	_SC_2_LOCALEDEF		23
#define	_SC_2_SW_DEV		24
#define	_SC_2_UPE		25
#define	_SC_STREAM_MAX		26
#define	_SC_TZNAME_MAX		27
#define	_SC_PAGESIZE		28
#define	_SC_PAGE_SIZE		_SC_PAGESIZE	/* 1170 compatibility */
#define	_SC_FSYNC		29
#define	_SC_XOPEN_SHM		30
#define	_SC_SYNCHRONIZED_IO	31
#define	_SC_IOV_MAX		32
#define	_SC_MAPPED_FILES	33
#define	_SC_MEMLOCK		34
#define	_SC_MEMLOCK_RANGE	35
#define	_SC_MEMORY_PROTECTION	36
#define	_SC_LOGIN_NAME_MAX	37
#define	_SC_MONOTONIC_CLOCK	38
#define	_SC_CLK_TCK		39 /* New, variable version */
#define	_SC_ATEXIT_MAX		40
#define	_SC_THREADS		41
#define	_SC_SEMAPHORES		42
#define	_SC_BARRIERS		43
#define	_SC_TIMERS		44
#define	_SC_SPIN_LOCKS		45
#define	_SC_READER_WRITER_LOCKS	46
#define	_SC_GETGR_R_SIZE_MAX	47
#define	_SC_GETPW_R_SIZE_MAX	48
#define	_SC_CLOCK_SELECTION	49
#define	_SC_ASYNCHRONOUS_IO	50
#define	_SC_AIO_LISTIO_MAX	51
#define	_SC_AIO_MAX		52
#define	_SC_MESSAGE_PASSING	53
#define	_SC_MQ_OPEN_MAX		54
#define	_SC_MQ_PRIO_MAX		55
#define	_SC_PRIORITY_SCHEDULING	56
#define	_SC_THREAD_DESTRUCTOR_ITERATIONS 57
#define	_SC_THREAD_KEYS_MAX		58
#define	_SC_THREAD_STACK_MIN		59
#define	_SC_THREAD_THREADS_MAX		60
#define	_SC_THREAD_ATTR_STACKADDR	61
#define	_SC_THREAD_ATTR_STACKSIZE 	62
#define	_SC_THREAD_PRIORITY_SCHEDULING	63
#define	_SC_THREAD_PRIO_INHERIT 	64
#define	_SC_THREAD_PRIO_PROTECT		65
#define	_SC_THREAD_PROCESS_SHARED	66
#define	_SC_THREAD_SAFE_FUNCTIONS	67
#define	_SC_TTY_NAME_MAX		68
#define	_SC_HOST_NAME_MAX		69
#define	_SC_PASS_MAX			70
#define	_SC_REGEXP			71
#define	_SC_SHELL			72
#define	_SC_SYMLOOP_MAX			73

/* Actually, they are not supported or implemented yet */
#define	_SC_V6_ILP32_OFF32		74
#define	_SC_V6_ILP32_OFFBIG		75
#define	_SC_V6_LP64_OFF64		76
#define	_SC_V6_LPBIG_OFFBIG		77
#define	_SC_2_PBS			80
#define	_SC_2_PBS_ACCOUNTING		81
#define	_SC_2_PBS_CHECKPOINT		82
#define	_SC_2_PBS_LOCATE		83
#define	_SC_2_PBS_MESSAGE		84
#define	_SC_2_PBS_TRACK			85

#ifdef _NETBSD_SOURCE
/* Commonly provided sysconf() extensions */
#define	_SC_NPROCESSORS_CONF	1001
#define	_SC_NPROCESSORS_ONLN	1002
/* Native variables */
#define	_SC_SCHED_RT_TS		2001
#define	_SC_SCHED_PRI_MIN	2002
#define	_SC_SCHED_PRI_MAX	2003
#endif	/* _NETBSD_SOURCE */

/* configurable system strings */
#define	_CS_PATH		 1

#endif /* !_SYS_UNISTD_H_ */
