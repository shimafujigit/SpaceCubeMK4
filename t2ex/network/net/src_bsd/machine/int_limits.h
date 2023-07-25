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

/*	$NetBSD: int_limits.h,v 1.9 2008/08/29 19:08:29 matt Exp $	*/

/*-
 * Copyright (c) 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein.
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

#ifndef _ARM_INT_LIMITS_H_
#define _ARM_INT_LIMITS_H_

/*
 * 7.18.2 Limits of specified-width integer types
 */

/* 7.18.2.1 Limits of exact-width integer types */

/* minimum values of exact-width signed integer types */
#define	INT8_MIN	(-0x7f-1)			/* int8_t	  */
#define	INT16_MIN	(-0x7fff-1)			/* int16_t	  */
#define	INT32_MIN	(-0x7fffffff-1)			/* int32_t	  */
#define	INT64_MIN	(-0x7fffffffffffffffLL-1)	/* int64_t	  */

/* maximum values of exact-width signed integer types */
#define	INT8_MAX	0x7f				/* int8_t	  */
#define	INT16_MAX	0x7fff				/* int16_t	  */
#define	INT32_MAX	0x7fffffff			/* int32_t	  */
#define	INT64_MAX	0x7fffffffffffffffLL		/* int64_t	  */

/* maximum values of exact-width unsigned integer types */
#define	UINT8_MAX	0xff				/* uint8_t	  */
#define	UINT16_MAX	0xffff				/* uint16_t	  */
#define	UINT32_MAX	0xffffffffU			/* uint32_t	  */
#define	UINT64_MAX	0xffffffffffffffffULL		/* uint64_t	  */

/* 7.18.2.2 Limits of minimum-width integer types */

/* minimum values of minimum-width signed integer types */
#define	INT_LEAST8_MIN	(-0x7f-1)			/* int_least8_t	  */
#define	INT_LEAST16_MIN	(-0x7fff-1)			/* int_least16_t  */
#define	INT_LEAST32_MIN	(-0x7fffffff-1)			/* int_least32_t  */
#define	INT_LEAST64_MIN	(-0x7fffffffffffffffLL-1)	/* int_least64_t  */

/* maximum values of minimum-width signed integer types */
#define	INT_LEAST8_MAX	0x7f				/* int_least8_t	  */
#define	INT_LEAST16_MAX	0x7fff				/* int_least16_t  */
#define	INT_LEAST32_MAX	0x7fffffff			/* int_least32_t  */
#define	INT_LEAST64_MAX	0x7fffffffffffffffLL		/* int_least64_t  */

/* maximum values of minimum-width unsigned integer types */
#define	UINT_LEAST8_MAX	 0xff				/* uint_least8_t  */
#define	UINT_LEAST16_MAX 0xffff				/* uint_least16_t */
#define	UINT_LEAST32_MAX 0xffffffffU			/* uint_least32_t */
#define	UINT_LEAST64_MAX 0xffffffffffffffffULL		/* uint_least64_t */

/* 7.18.2.3 Limits of fastest minimum-width integer types */
 
/* minimum values of fastest minimum-width signed integer types */
#define	INT_FAST8_MIN	(-0x7fffffff-1)			/* int_fast8_t	  */
#define	INT_FAST16_MIN	(-0x7fffffff-1)			/* int_fast16_t	  */
#define	INT_FAST32_MIN	(-0x7fffffff-1)			/* int_fast32_t	  */
#define	INT_FAST64_MIN	(-0x7fffffffffffffffLL-1)	/* int_fast64_t	  */

/* maximum values of fastest minimum-width signed integer types */
#define	INT_FAST8_MAX	0x7fffffff			/* int_fast8_t	  */
#define	INT_FAST16_MAX	0x7fffffff			/* int_fast16_t	  */
#define	INT_FAST32_MAX	0x7fffffff			/* int_fast32_t	  */
#define	INT_FAST64_MAX	0x7fffffffffffffffLL		/* int_fast64_t	  */

/* maximum values of fastest minimum-width unsigned integer types */
#define	UINT_FAST8_MAX	0xffffffffU			/* uint_fast8_t	  */
#define	UINT_FAST16_MAX	0xffffffffU			/* uint_fast16_t  */
#define	UINT_FAST32_MAX	0xffffffffU			/* uint_fast32_t  */
#define	UINT_FAST64_MAX	0xffffffffffffffffULL		/* uint_fast64_t  */

/* 7.18.2.4 Limits of integer types capable of holding object pointers */

#define	INTPTR_MIN	(-0x7fffffffL-1)		/* intptr_t	  */
#define	INTPTR_MAX	0x7fffffffL			/* intptr_t	  */
#define	UINTPTR_MAX	0xffffffffUL			/* uintptr_t	  */

/* 7.18.2.5 Limits of greatest-width integer types */

#define	INTMAX_MIN	(-0x7fffffffffffffffLL-1)	/* intmax_t	  */
#define	INTMAX_MAX	0x7fffffffffffffffLL		/* intmax_t	  */
#define	UINTMAX_MAX	0xffffffffffffffffULL		/* uintmax_t	  */


/*
 * 7.18.3 Limits of other integer types
 */

/* limits of ptrdiff_t */
#define	PTRDIFF_MIN	(-0x7fffffffL-1)		/* ptrdiff_t	  */
#define	PTRDIFF_MAX	0x7fffffffL			/* ptrdiff_t	  */

/* limits of sig_atomic_t */
#define	SIG_ATOMIC_MIN	(-0x7fffffff-1)			/* sig_atomic_t	  */
#define	SIG_ATOMIC_MAX	0x7fffffff			/* sig_atomic_t	  */

/* limit of size_t */
#define	SIZE_MAX	0xffffffffUL			/* size_t	  */

#endif /* !_ARM_INT_LIMITS_H_ */
