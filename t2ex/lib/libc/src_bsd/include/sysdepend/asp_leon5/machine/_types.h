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

/*	$OpenBSD: _types.h,v 1.10 2011/11/15 17:12:13 deraadt Exp $	*/

/*-
 * Copyright (c) 1990, 1993
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
 *	@(#)types.h	8.3 (Berkeley) 1/5/94
 *	@(#)ansi.h	8.2 (Berkeley) 1/4/94
 */

#ifndef _SPARC__TYPES_H_
#define _SPARC__TYPES_H_

#if defined(_KERNEL)
typedef struct label_t {
	long val[11];
} label_t;
#endif

/*
 * _ALIGN(p) rounds p (pointer or byte index) up to a correctly-aligned
 * value for all data types (int, long, ...).   The result is an
 * unsigned long and must be cast to any desired pointer type.
 *
 * _ALIGNED_POINTER is a boolean macro that checks whether an address
 * is valid to fetch data elements of type t from on this architecture.
 * This does not reflect the optimal alignment, just the possibility
 * (within reasonable limits).
 */
#define	_ALIGNBYTES	(sizeof(int) - 1)
#define	_ALIGN(p)	(((unsigned long)(p) + _ALIGNBYTES) & ~_ALIGNBYTES)
#define	_ALIGNED_POINTER(p,t)	((((unsigned long)(p)) & (sizeof(t) - 1)) == 0)

/* 7.18.1.1 Exact-width integer types */
typedef	__signed char		__int8_t;
typedef	unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef	int			__int32_t;
typedef	unsigned int		__uint32_t;
/* LONGLONG */
typedef	long long		__int64_t;
/* LONGLONG */
typedef	unsigned long long	__uint64_t;

/* 7.18.1.2 Minimum-width integer types */
typedef	__int8_t		__int_least8_t;
typedef	__uint8_t		__uint_least8_t;
typedef	__int16_t		__int_least16_t;
typedef	__uint16_t		__uint_least16_t;
typedef	__int32_t		__int_least32_t;
typedef	__uint32_t		__uint_least32_t;
typedef	__int64_t		__int_least64_t;
typedef	__uint64_t		__uint_least64_t;

/* 7.18.1.3 Fastest minimum-width integer types */
typedef	__int32_t		__int_fast8_t;
typedef	__uint32_t		__uint_fast8_t;
typedef	__int32_t		__int_fast16_t;
typedef	__uint32_t		__uint_fast16_t;
typedef	__int32_t		__int_fast32_t;
typedef	__uint32_t		__uint_fast32_t;
typedef	__int64_t		__int_fast64_t;
typedef	__uint64_t		__uint_fast64_t;
#define	__INT_FAST8_MIN		INT32_MIN
#define	__INT_FAST16_MIN	INT32_MIN
#define	__INT_FAST32_MIN	INT32_MIN
#define	__INT_FAST64_MIN	INT64_MIN
#define	__INT_FAST8_MAX		INT32_MAX
#define	__INT_FAST16_MAX	INT32_MAX
#define	__INT_FAST32_MAX	INT32_MAX
#define	__INT_FAST64_MAX	INT64_MAX
#define	__UINT_FAST8_MAX	UINT32_MAX
#define	__UINT_FAST16_MAX	UINT32_MAX
#define	__UINT_FAST32_MAX	UINT32_MAX
#define	__UINT_FAST64_MAX	UINT64_MAX

/* 7.18.1.4 Integer types capable of holding object pointers */
typedef	long			__intptr_t;
typedef	unsigned long		__uintptr_t;

/* 7.18.1.5 Greatest-width integer types */
typedef	__int64_t		__intmax_t;
typedef	__uint64_t		__uintmax_t;

/* Register size */
typedef __int32_t		__register_t;

/* VM system types */
typedef unsigned long		__vaddr_t;
typedef unsigned long		__paddr_t;
typedef unsigned long		__vsize_t;
typedef unsigned long		__psize_t;

/* Standard system types */
typedef int			__clock_t;
typedef int			__clockid_t;
typedef double			__double_t;
typedef float			__float_t;
typedef long long		__off_t;
typedef long			__ptrdiff_t;
typedef	unsigned long		__size_t;
typedef	long			__ssize_t;
typedef	int			__time_t;
typedef int			__timer_t;
#if defined(__GNUC__) && __GNUC__ >= 3
typedef	__builtin_va_list	__va_list;
#else
typedef	char *			__va_list;
#endif

/* Wide character support types */
#ifndef __cplusplus
typedef	int			__wchar_t;
#endif
typedef int			__wint_t;
typedef	int			__rune_t;
typedef	void *			__wctrans_t;
typedef	void *			__wctype_t;

#endif	/* _SPARC__TYPES_H_ */
