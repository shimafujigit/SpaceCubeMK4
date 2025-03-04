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

/*	$NetBSD: condvar.h,v 1.10 2008/05/31 13:36:25 ad Exp $	*/

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

#ifndef _SYS_CONDVAR_H_
#define	_SYS_CONDVAR_H_

#include <sys/mutex.h>

/*
 * The condition variable implementation is private to kern_condvar.c but
 * the size of a kcondvar_t must remain constant.  cv_opaque is protected
 * both by the interlock passed to cv_wait() (enqueue only), and the sleep
 * queue lock acquired with sleeptab_lookup() (enqueue and dequeue). 
 * cv_wmesg is static and does not change throughout the life of the CV.
 */
typedef struct kcondvar {
#ifndef T2EX
	void		*cv_opaque[2];	/* sleep queue */
	const char	*cv_wmesg;	/* description for /bin/ps */
#else
	int	flgid;
	int signal;
	int waitcnt;
#endif
} kcondvar_t;

#ifdef _KERNEL

#ifdef T2EX
int	tkn_cv_init(kcondvar_t *, const char *);
void	tkn_cv_destroy(kcondvar_t *);

void	tkn_cv_wait(kcondvar_t *, kmutex_t *);
int	tkn_cv_wait_sig(kcondvar_t *, kmutex_t *);
int	tkn_cv_timedwait(kcondvar_t *, kmutex_t *, int);
int	tkn_cv_timedwait_sig(kcondvar_t *, kmutex_t *, int);

void	tkn_cv_signal(kcondvar_t *);
void	tkn_cv_broadcast(kcondvar_t *);

void	tkn_cv_wakeup(kcondvar_t *);

bool	tkn_cv_has_waiters(kcondvar_t *);
bool	tkn_cv_is_valid(kcondvar_t *);

#define	cv_init(a, b)	tkn_cv_init((a), (b))
#define	cv_destroy(a)	tkn_cv_destroy((a))
#define	cv_wait(a, b)	tkn_cv_wait((a), (b))
#define	cv_wait_sig(a, b)	tkn_cv_wait_sig((a), (b))
#define	cv_timedwait(a, b, c)	tkn_cv_timedwait((a), (b), (c))
#define	cv_timedwait_sig(a, b, c)	tkn_cv_timedwait_sig((a), (b), (c))
#define	cv_signal(a)	tkn_cv_signal((a))
#define	cv_broadcast(a)	tkn_cv_broadcast((a))
#define	cv_wakeup(a)	tkn_cv_wakeup((a))
#define	cv_has_waiters(a)	tkn_cv_has_waiters((a))
#define	cv_is_valid(a)	tkn_cv_is_valid((a))


#else
void	cv_init(kcondvar_t *, const char *);
void	cv_destroy(kcondvar_t *);

void	cv_wait(kcondvar_t *, kmutex_t *);
int	cv_wait_sig(kcondvar_t *, kmutex_t *);
int	cv_timedwait(kcondvar_t *, kmutex_t *, int);
int	cv_timedwait_sig(kcondvar_t *, kmutex_t *, int);

void	cv_signal(kcondvar_t *);
void	cv_broadcast(kcondvar_t *);

void	cv_wakeup(kcondvar_t *);

bool	cv_has_waiters(kcondvar_t *);
bool	cv_is_valid(kcondvar_t *);

/* The "lightning bolt", awoken once per second by the clock interrupt. */
extern kcondvar_t lbolt;
#endif

#endif	/* _KERNEL */

#endif /* _SYS_CONDVAR_H_ */
