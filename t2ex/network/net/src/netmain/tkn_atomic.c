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

/*
 *	@(#)tkn_atomic.c
 *
 */

#include <sys/atomic.h>

void
atomic_and_uint(volatile unsigned int * target, unsigned int bits)
{
	atomic_bitclr(target, bits);
}

unsigned int
atomic_cas_uint(volatile unsigned int * target, unsigned int cmp, unsigned int newval)
{
	return atomic_cmpxchg(target, newval, cmp);
}

void*
atomic_cas_ptr(volatile void* target, void* cmp, void* newval)
{
	return (void*)atomic_cmpxchg((UINT*)target, (UINT)newval, (UINT)cmp);
}

void
atomic_dec_uint(volatile unsigned int * target)
{
	atomic_dec(target);
}

unsigned int
atomic_dec_uint_nv(volatile unsigned int * target)
{
	return atomic_dec(target);
}

void
atomic_inc_uint(volatile unsigned int * target)
{
	atomic_inc(target);
}

unsigned int
atomic_inc_uint_nv(volatile unsigned int * target)
{
	return atomic_inc(target);
}

void
atomic_or_uint(volatile unsigned int * target, unsigned int bits)
{
	atomic_bitset(target, bits);
}

void
atomic_add_long(volatile unsigned long * target, long val)
{
	atomic_add((UINT*)target, val);
}

unsigned long
atomic_add_long_nv(volatile unsigned long * target, long val)
{
	return atomic_add((UINT*)target, val);
}

void
membar_enter(void)
{
	// TODO membar
}

void
membar_exit(void)
{
	// TODO membar
}

void
membar_producer(void)
{
	// TODO membar
}

void
membar_consumer(void)
{
	// TODO membar
}

void
membar_sync(void)
{
	// TODO membar
}
