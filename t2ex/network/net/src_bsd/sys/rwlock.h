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

/*	$NetBSD: rwlock.h,v 1.6 2008/04/28 20:24:11 martin Exp $	*/

/*-
 * Copyright (c) 2002, 2006, 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe and Andrew Doran.
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

#ifndef _SYS_RWLOCK_H_
#define	_SYS_RWLOCK_H_

/*
 * The rwlock provides exclusive access when held as a "writer",
 * and shared access when held as a "reader".
 *
 * Architectures may optionally provide stubs for the following functions to
 * implement the easy (unlocked, no waiters) cases.  If these stubs are
 * provided, __HAVE_RW_STUBS should be defined.
 *
 *	rw_enter()
 *	rw_exit()
 *	rw_tryenter()
 */

#if defined(_KERNEL_OPT)
#ifndef T2EX
#include "opt_lockdebug.h"
#endif
#endif

#if !defined(_KERNEL)
#include <sys/types.h>
#include <sys/inttypes.h>
#endif

typedef enum krw_t {
	RW_READER = 0,
	RW_WRITER = 1
} krw_t;

#ifndef T2EX
typedef struct krwlock krwlock_t;
#else
typedef struct krwlock {
	int semid;
	int is_writer;
	int oldspl;
} krwlock_t;
#endif

#ifdef __RWLOCK_PRIVATE
/*
 * Bits in the owner field of the lock that indicate lock state.  If the
 * WRITE_LOCKED bit is clear, then the owner field is actually a count of
 * the number of readers.  The rw_owner field is laid out like so:
 *
 *	 N                    4       3        2        1      0
 *	+------------------------------------------------------+
 *	| owner or read count | debug | wrlock | wrwant | wait |
 *	+------------------------------------------------------+
 */
#define	RW_HAS_WAITERS		0x01UL	/* lock has waiters */
#define	RW_WRITE_WANTED		0x02UL	/* >= 1 waiter is a writer */
#define	RW_WRITE_LOCKED		0x04UL	/* lock is currently write locked */
#define	RW_DEBUG		0x08UL	/* LOCKDEBUG enabled */

#define	RW_READ_COUNT_SHIFT	4
#define	RW_READ_INCR		(1UL << RW_READ_COUNT_SHIFT)
#define	RW_THREAD		((uintptr_t)-RW_READ_INCR)
#define	RW_OWNER(rw)		((rw)->rw_owner & RW_THREAD)
#define	RW_COUNT(rw)		((rw)->rw_owner & RW_THREAD)
#define	RW_FLAGS(rw)		((rw)->rw_owner & ~RW_THREAD)

void	rw_vector_enter(krwlock_t *, const krw_t);
void	rw_vector_exit(krwlock_t *);
int	rw_vector_tryenter(krwlock_t *, const krw_t);
#endif	/* __RWLOCK_PRIVATE */

#ifndef T2EX
#include <machine/rwlock.h>
#endif

#ifdef _KERNEL

#ifdef T2EX
int	tkn_rw_init(krwlock_t *);
void	tkn_rw_destroy(krwlock_t *);

int	tkn_rw_tryenter(krwlock_t *, const krw_t);
int	tkn_rw_tryupgrade(krwlock_t *);
void	tkn_rw_downgrade(krwlock_t *);

int	tkn_rw_read_held(krwlock_t *);
int	tkn_rw_write_held(krwlock_t *);
int	tkn_rw_lock_held(krwlock_t *);

void	tkn_rw_enter(krwlock_t *, const krw_t);
void	tkn_rw_exit(krwlock_t *);

#define rw_init(a)	tkn_rw_init((a))
#define rw_destroy(a)	tkn_rw_destroy((a))
#define rw_tryenter(a, b)	tkn_rw_tryenter((a), (b))
#define rw_tryupgrade(a)	tkn_rw_tryupgrade((a))
#define rw_trydowngrade(a)	tkn_rw_trydowngrade((a))
#define rw_try_read_held(a)	tkn_rw_read_held((a))
#define rw_try_write_held(a)	tkn_rw_write_held((a))
#define rw_try_lock_held(a)	tkn_rw_lock_held((a))
#define rw_enter(a, b)	tkn_rw_enter((a), (b))
#define rw_exit(a)	tkn_rw_exit((a))

#else

void	rw_init(krwlock_t *);
void	rw_destroy(krwlock_t *);

int	rw_tryenter(krwlock_t *, const krw_t);
int	rw_tryupgrade(krwlock_t *);
void	rw_downgrade(krwlock_t *);

int	rw_read_held(krwlock_t *);
int	rw_write_held(krwlock_t *);
int	rw_lock_held(krwlock_t *);

void	rw_enter(krwlock_t *, const krw_t);
void	rw_exit(krwlock_t *);
#endif

#endif	/* _KERNEL */

#endif /* _SYS_RWLOCK_H_ */
