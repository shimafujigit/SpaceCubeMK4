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

/*	$NetBSD: poll.h,v 1.12 2008/04/28 20:24:11 martin Exp $	*/

/*-
 * Copyright (c) 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Charles M. Hannum.
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

#ifndef _SYS_POLL_H_
#define	_SYS_POLL_H_

#include <sys/featuretest.h>

#ifndef T2EX
typedef unsigned int	nfds_t;
#else
#ifndef nfds_t
typedef unsigned int    nfds_t;
#define	nfds_t		nfds_t
#endif
#endif

#ifndef T2EX
struct pollfd {
	int	fd;		/* file descriptor */
	short	events;		/* events to look for */
	short	revents;	/* events returned */
};
#endif

/*
 * Testable events (may be specified in events field).
 */
#define	POLLIN		0x0001
#define	POLLPRI		0x0002
#define	POLLOUT		0x0004
#define	POLLRDNORM	0x0040
#define	POLLWRNORM	POLLOUT
#define	POLLRDBAND	0x0080
#define	POLLWRBAND	0x0100

/*
 * Non-testable events (may not be specified in events field).
 */
#define	POLLERR		0x0008
#define	POLLHUP		0x0010
#define	POLLNVAL	0x0020

#if defined(_NETBSD_SOURCE)
/*
 * Infinite timeout value.
 */
#define	INFTIM		-1
#endif

#ifdef _KERNEL
#ifndef T2EX
#include <sys/signal.h>		/* for sigset_t */
#endif

struct lwp;
struct timeval;

#ifndef T2EX
int	pollcommon(struct lwp *, register_t *, struct pollfd *, u_int,
	    struct timeval *, sigset_t *);
#endif
#else
#include <sys/cdefs.h>

__BEGIN_DECLS
int	poll(struct pollfd *, nfds_t, int);
__END_DECLS

#ifdef _NETBSD_SOURCE
#include <sys/sigtypes.h>	/* for sigset_t */
struct timespec;

__BEGIN_DECLS
int	pollts(struct pollfd * __restrict, nfds_t,
	    const struct timespec * __restrict, const sigset_t * __restrict);
__END_DECLS
#endif /* _NETBSD_SOURCE */

#endif /* _KERNEL */

#endif /* !_SYS_POLL_H_ */
