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

/*	$NetBSD: select.h,v 1.33 2008/03/22 18:04:42 ad Exp $	*/

/*-
 * Copyright (c) 1992, 1993
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
 *	@(#)select.h	8.2 (Berkeley) 1/4/94
 */

#ifndef _SYS_SELECT_H_
#define	_SYS_SELECT_H_

#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <sys/fd_set.h>

#ifdef _KERNEL
#include <sys/selinfo.h>		/* for struct selinfo */
#ifndef T2EX
#include <sys/signal.h>			/* for sigset_t */
#endif

struct lwp;
struct proc;
struct timeval;
struct cpu_info;
struct socket;

#ifndef T2EX
int	selcommon(struct lwp *, register_t *, int, fd_set *, fd_set *,
	    fd_set *, struct timeval *, sigset_t *);
#else
int	selcommon(struct lwp *, register_t *, int, fd_set *, fd_set *,
	    fd_set *, struct timeval *);
#endif
void	selrecord(struct lwp *selector, struct selinfo *);
void	selnotify(struct selinfo *, int, long);
#ifndef T2EX
void	selsysinit(struct cpu_info *);
#else
int	selsysinit(struct cpu_info *);
int	selsysfinish(struct cpu_info *);
#endif
void	selinit(struct selinfo *);
void	seldestroy(struct selinfo *);
int	pollsock(struct socket *, const struct timeval *, int);

#else /* _KERNEL */

#ifndef T2EX
#include <sys/sigtypes.h>
#include <time.h>
#endif

__BEGIN_DECLS
#ifndef T2EX
int	pselect(int, fd_set * __restrict, fd_set * __restrict,
	    fd_set * __restrict, const struct timespec * __restrict,
	    const sigset_t * __restrict);
#endif
int	select(int, fd_set * __restrict, fd_set * __restrict,
	    fd_set * __restrict, struct timeval * __restrict);
__END_DECLS
#endif /* _KERNEL */

#endif /* !_SYS_SELECT_H_ */
