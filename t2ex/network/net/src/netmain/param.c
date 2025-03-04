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

/*	$NetBSD: param.c,v 1.58 2008/07/12 11:50:07 gmcgarry Exp $	*/

/*
 * Copyright (c) 1980, 1986, 1989 Regents of the University of California.
 * All rights reserved.
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
 *	@(#)param.c	7.20 (Berkeley) 6/27/91
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: param.c,v 1.58 2008/07/12 11:50:07 gmcgarry Exp $");

#ifndef T2EX
#include "opt_hz.h"
#include "opt_rtc_offset.h"
#include "opt_sysv.h"
#include "opt_sysvparam.h"
#include "opt_nmbclusters.h"
#include "opt_multiprocessor.h"
#endif

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/proc.h>
#ifndef T2EX
#include <sys/vnode.h>
#include <sys/file.h>
#endif
#include <sys/callout.h>
#include <sys/mbuf.h>
#ifndef T2EX
#include <ufs/ufs/quota.h>
#endif
#include <sys/kernel.h>
#ifndef T2EX
#include <sys/utsname.h>
#endif
#ifdef SYSVSHM
#include <machine/vmparam.h>
#include <sys/shm.h>
#endif
#ifdef SYSVSEM
#include <sys/sem.h>
#endif
#ifdef SYSVMSG
#include <sys/msg.h>
#endif

#ifndef T2EX
/*
 * PCC cannot handle the 80KB string literal.
 */
#if !defined(__PCC__)
#define CONFIG_FILE
#include "config_file.h"
#endif
#endif

/*
 * System parameter formulae.
 *
 * This file is copied into each directory where we compile
 * the kernel; it should be modified there to suit local taste
 * if necessary.
 *
 * Compiled with -DHZ=xx -DRTC_OFFSET=x -DMAXUSERS=xx
 */

#ifndef T2EX
#ifdef TIMEZONE
#error TIMEZONE is an obsolete kernel option.
#endif

#ifdef DST
#error DST is an obsolete kernel option.
#endif

#ifndef RTC_OFFSET
#define RTC_OFFSET 0
#endif
#endif /* !T2EX */

#ifndef HZ
#define	HZ 100
#endif

#ifndef T2EX
#ifndef MAXFILES
#define	MAXFILES	(3 * (NPROC + MAXUSERS) + 80)
#endif
#else
/* maxfiles is initialized to SoMaxSocket at read_sysconf(). */
u_int	maxfiles;
#endif /* !T2EX */

#ifndef MAXEXEC
#define	MAXEXEC		16
#endif

int	hz = HZ;
int	tick = 1000000 / HZ;
#ifndef T2EX
/* can adjust 240ms in 60s */
int	tickadj = (240000 / (60 * HZ)) ? (240000 / (60 * HZ)) : 1;
int	rtc_offset = RTC_OFFSET;
int	maxproc = NPROC;
int	desiredvnodes = NVNODE;
u_int	maxfiles = MAXFILES;
int	fscale = FSCALE;	/* kernel uses `FSCALE', user uses `fscale' */
int	maxexec = MAXEXEC;	/* max number of concurrent exec() calls */
#endif /* !T2EX */

#ifdef MULTIPROCESSOR
u_int	maxcpus = MAXCPUS;
size_t	coherency_unit = COHERENCY_UNIT;
#else
u_int	maxcpus = 1;
size_t	coherency_unit = ALIGNBYTES + 1;
#endif

/*
 * Various mbuf-related parameters.  These can also be changed at run-time
 * with sysctl.
 */
int	nmbclusters = NMBCLUSTERS;

#ifndef MBLOWAT
#define	MBLOWAT		16
#endif
int	mblowat = MBLOWAT;

#ifndef MCLLOWAT
#define	MCLLOWAT	8
#endif
int	mcllowat = MCLLOWAT;

#ifndef T2EX
/*
 * Values in support of System V compatible shared memory.	XXX
 */
#ifdef SYSVSHM
#ifndef	SHMMAX
#define	SHMMAX	SHMMAXPGS	/* shminit() performs a `*= PAGE_SIZE' */
#endif
#ifndef	SHMMIN
#define	SHMMIN	1
#endif
#ifndef	SHMMNI
#define	SHMMNI	128		/* <64k, see IPCID_TO_IX in ipc.h */
#endif
#ifndef	SHMSEG
#define	SHMSEG	128
#endif
#define	SHMALL	SHMMAXPGS

struct	shminfo shminfo = {
	SHMMAX,
	SHMMIN,
	SHMMNI,
	SHMSEG,
	SHMALL
};
#endif

/*
 * Values in support of System V compatible semaphores.
 */
#ifdef SYSVSEM
struct	seminfo seminfo = {
	SEMMAP,		/* # of entries in semaphore map */
	SEMMNI,		/* # of semaphore identifiers */
	SEMMNS,		/* # of semaphores in system */
	SEMMNU,		/* # of undo structures in system */
	SEMMSL,		/* max # of semaphores per id */
	SEMOPM,		/* max # of operations per semop call */
	SEMUME,		/* max # of undo entries per process */
	SEMUSZ,		/* size in bytes of undo structure */
	SEMVMX,		/* semaphore maximum value */
	SEMAEM		/* adjust on exit max value */
};
#endif

/*
 * Values in support of System V compatible messages.
 */
#ifdef SYSVMSG
struct	msginfo msginfo = {
	MSGMAX,		/* max chars in a message */
	MSGMNI,		/* # of message queue identifiers */
	MSGMNB,		/* max chars in a queue */
	MSGTQL,		/* max messages in system */
	MSGSSZ,		/* size of a message segment */
			/* (must be small power of 2 greater than 4) */
	MSGSEG		/* number of message segments */
};
#endif
#endif  /* !T2EX */

/*
 * Actual network mbuf sizes (read-only), for netstat.
 */
const	int msize = MSIZE;
const	int mclbytes = MCLBYTES;
