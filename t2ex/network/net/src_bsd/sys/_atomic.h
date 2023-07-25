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

/*	$NetBSD: atomic.h,v 1.10.10.1 2010/03/02 06:21:12 snj Exp $	*/

/*-
 * Copyright (c) 2007, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe.
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

#ifndef _SYS_ATOMIC_H_
#define	_SYS_ATOMIC_H_

#include <sys/types.h>
#if !defined(_KERNEL) && !defined(_STANDALONE)
#include <stdint.h>
#endif

__BEGIN_DECLS
/*
 * Atomic ADD
 */
void		atomic_add_32(volatile uint32_t *, int32_t);
void		atomic_add_int(volatile unsigned int *, int);
void		atomic_add_long(volatile unsigned long *, long);
void		atomic_add_ptr(volatile void *, ssize_t);
void		atomic_add_64(volatile uint64_t *, int64_t);

uint32_t	atomic_add_32_nv(volatile uint32_t *, int32_t);
unsigned int	atomic_add_int_nv(volatile unsigned int *, int);
unsigned long	atomic_add_long_nv(volatile unsigned long *, long);
void *		atomic_add_ptr_nv(volatile void *, ssize_t);
uint64_t	atomic_add_64_nv(volatile uint64_t *, int64_t);

/*
 * Atomic AND
 */
void		atomic_and_32(volatile uint32_t *, uint32_t);
void		atomic_and_uint(volatile unsigned int *, unsigned int);
void		atomic_and_ulong(volatile unsigned long *, unsigned long);
void		atomic_and_64(volatile uint64_t *, uint64_t);

uint32_t	atomic_and_32_nv(volatile uint32_t *, uint32_t);
unsigned int	atomic_and_uint_nv(volatile unsigned int *, unsigned int);
unsigned long	atomic_and_ulong_nv(volatile unsigned long *, unsigned long);
uint64_t	atomic_and_64_nv(volatile uint64_t *, uint64_t);

/*
 * Atomic OR
 */
void		atomic_or_32(volatile uint32_t *, uint32_t);
void		atomic_or_uint(volatile unsigned int *, unsigned int);
void		atomic_or_ulong(volatile unsigned long *, unsigned long);
void		atomic_or_64(volatile uint64_t *, uint64_t);

uint32_t	atomic_or_32_nv(volatile uint32_t *, uint32_t);
unsigned int	atomic_or_uint_nv(volatile unsigned int *, unsigned int);
unsigned long	atomic_or_ulong_nv(volatile unsigned long *, unsigned long);
uint64_t	atomic_or_64_nv(volatile uint64_t *, uint64_t);

/*
 * Atomic COMPARE-AND-SWAP
 */
uint32_t	atomic_cas_32(volatile uint32_t *, uint32_t, uint32_t);
unsigned int	atomic_cas_uint(volatile unsigned int *, unsigned int,
				unsigned int);
unsigned long	atomic_cas_ulong(volatile unsigned long *, unsigned long,
				 unsigned long);
void *		atomic_cas_ptr(volatile void *, void *, void *);
uint64_t	atomic_cas_64(volatile uint64_t *, uint64_t, uint64_t);

/*
 * Non-interlocked atomic COMPARE-AND-SWAP.
 */
uint32_t	atomic_cas_32_ni(volatile uint32_t *, uint32_t, uint32_t);
unsigned int	atomic_cas_uint_ni(volatile unsigned int *, unsigned int,
				   unsigned int);
unsigned long	atomic_cas_ulong_ni(volatile unsigned long *, unsigned long,
				    unsigned long);
void *		atomic_cas_ptr_ni(volatile void *, void *, void *);
uint64_t	atomic_cas_64_ni(volatile uint64_t *, uint64_t, uint64_t);

/*
 * Atomic SWAP
 */
uint32_t	atomic_swap_32(volatile uint32_t *, uint32_t);
unsigned int	atomic_swap_uint(volatile unsigned int *, unsigned int);
unsigned long	atomic_swap_ulong(volatile unsigned long *, unsigned long);
void *		atomic_swap_ptr(volatile void *, void *);
uint64_t	atomic_swap_64(volatile uint64_t *, uint64_t);

/*
 * Atomic DECREMENT
 */
void		atomic_dec_32(volatile uint32_t *);
void		atomic_dec_uint(volatile unsigned int *);
void		atomic_dec_ulong(volatile unsigned long *);
void		atomic_dec_ptr(volatile void *);
void		atomic_dec_64(volatile uint64_t *);

uint32_t	atomic_dec_32_nv(volatile uint32_t *);
unsigned int	atomic_dec_uint_nv(volatile unsigned int *);
unsigned long	atomic_dec_ulong_nv(volatile unsigned long *);
void *		atomic_dec_ptr_nv(volatile void *);
uint64_t	atomic_dec_64_nv(volatile uint64_t *);

/*
 * Atomic INCREMENT
 */
void		atomic_inc_32(volatile uint32_t *);
void		atomic_inc_uint(volatile unsigned int *);
void		atomic_inc_ulong(volatile unsigned long *);
void		atomic_inc_ptr(volatile void *);
void		atomic_inc_64(volatile uint64_t *);

uint32_t	atomic_inc_32_nv(volatile uint32_t *);
unsigned int	atomic_inc_uint_nv(volatile unsigned int *);
unsigned long	atomic_inc_ulong_nv(volatile unsigned long *);
void *		atomic_inc_ptr_nv(volatile void *);
uint64_t	atomic_inc_64_nv(volatile uint64_t *);

/*
 * Memory barrier operations
 */
void		membar_enter(void);
void		membar_exit(void);
void		membar_producer(void);
void		membar_consumer(void);
void		membar_sync(void);

__END_DECLS

#endif /* ! _SYS_ATOMIC_H_ */
