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

/*	$OpenBSD: wait.h,v 1.14 2006/04/27 02:17:21 tedu Exp $	*/
/*	$NetBSD: wait.h,v 1.11 1996/04/09 20:55:51 cgd Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1989, 1993, 1994
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
 *	@(#)wait.h	8.2 (Berkeley) 7/10/94
 */

#ifndef _SYS_WAIT_H_
#define _SYS_WAIT_H_

#include <sys/cdefs.h>

/*
 * This file holds definitions relevent to the wait4 system call
 * and the alternate interfaces that use it (wait, wait3, waitpid).
 */

/*
 * Macros to test the exit status returned by wait
 * and extract the relevant values.
 */
#if __BSD_VISIBLE
#define	_W_INT(w)	(*(int *)&(w))	/* convert union wait to int */
#else
#define	_W_INT(i)	(i)
#endif

#define	_WSTATUS(x)	(_W_INT(x) & 0177)
#define	_WSTOPPED	0177		/* _WSTATUS if process is stopped */
#define	_WCONTINUED	0177777		/* process has continued */
#define WIFSTOPPED(x)	((_W_INT(x) & 0xff) == _WSTOPPED)
#define WSTOPSIG(x)	(int)(((unsigned)_W_INT(x) >> 8) & 0xff)
#define WIFSIGNALED(x)	(_WSTATUS(x) != _WSTOPPED && _WSTATUS(x) != 0)
#define WTERMSIG(x)	(_WSTATUS(x))
#define WIFEXITED(x)	(_WSTATUS(x) == 0)
#define WEXITSTATUS(x)	(int)(((unsigned)_W_INT(x) >> 8) & 0xff)
#define WIFCONTINUED(x)	((_W_INT(x) & _WCONTINUED) == _WCONTINUED)
#if __XPG_VISIBLE
#define	WCOREFLAG	0200
#define WCOREDUMP(x)	(_W_INT(x) & WCOREFLAG)

#define	W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	W_STOPCODE(sig)		((sig) << 8 | _WSTOPPED)
#endif

/*
 * Option bits for the third argument of wait4.  WNOHANG causes the
 * wait to not hang if there are no stopped or terminated processes, rather
 * returning an error indication in this case (pid==0).  WUNTRACED
 * indicates that the caller should receive status about untraced children
 * which stop due to signals.  If children are stopped and a wait without
 * this option is done, it is as though they were still running... nothing
 * about them is returned.
 */
#define WNOHANG		1	/* don't hang in wait */
#define WUNTRACED	2	/* tell about stopped, untraced children */
#if __XPG_VISIBLE
#define	WALTSIG		4	/* wait for child with alternate exit signal */
#endif
#define	WCONTINUED	8	/* report a job control continued process */

#if __BSD_VISIBLE
/*
 * Tokens for special values of the "pid" parameter to wait4.
 */
#define	WAIT_ANY	(-1)	/* any process */
#define	WAIT_MYPGRP	0	/* any process in my process group */

#include <sys/types.h>

/*
 * Deprecated:
 * Structure of the information in the status word returned by wait4.
 * If w_stopval==WSTOPPED, then the second structure describes
 * the information returned, else the first.
 */
union wait {
	int	w_status;		/* used in syscall */
	/*
	 * Terminated process status.
	 */
	struct {
#if _BYTE_ORDER == _LITTLE_ENDIAN
		unsigned int	w_Termsig:7,	/* termination signal */
				w_Coredump:1,	/* core dump indicator */
				w_Retcode:8,	/* exit code if w_termsig==0 */
				w_Filler:16;	/* upper bits filler */
#endif
#if _BYTE_ORDER == _BIG_ENDIAN
		unsigned int	w_Filler:16,	/* upper bits filler */
				w_Retcode:8,	/* exit code if w_termsig==0 */
				w_Coredump:1,	/* core dump indicator */
				w_Termsig:7;	/* termination signal */
#endif
	} w_T;
	/*
	 * Stopped process status.  Returned
	 * only for traced children unless requested
	 * with the WUNTRACED option bit.
	 */
	struct {
#if _BYTE_ORDER == _LITTLE_ENDIAN
		unsigned int	w_Stopval:8,	/* == W_STOPPED if stopped */
				w_Stopsig:8,	/* signal that stopped us */
				w_Filler:16;	/* upper bits filler */
#endif
#if _BYTE_ORDER == _BIG_ENDIAN
		unsigned int	w_Filler:16,	/* upper bits filler */
				w_Stopsig:8,	/* signal that stopped us */
				w_Stopval:8;	/* == W_STOPPED if stopped */
#endif
	} w_S;
};
#define	w_termsig	w_T.w_Termsig
#define w_coredump	w_T.w_Coredump
#define w_retcode	w_T.w_Retcode
#define w_stopval	w_S.w_Stopval
#define w_stopsig	w_S.w_Stopsig

#define	WSTOPPED	_WSTOPPED
#endif /* __BSD_VISIBLE */

#ifndef _KERNEL
#include <sys/types.h>

__BEGIN_DECLS
struct rusage;	/* forward declaration */

pid_t	wait(int *);
pid_t	waitpid(pid_t, int *, int);
#if __BSD_VISIBLE
pid_t	wait3(int *, int, struct rusage *);
pid_t	wait4(pid_t, int *, int, struct rusage *);
#endif
__END_DECLS
#endif

#endif /* !_SYS_WAIT_H_ */
