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

/*	$NetBSD: int_mwgwtypes.h,v 1.4 2008/04/28 20:23:14 martin Exp $	*/

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

#ifndef _ARM_INT_MWGWTYPES_H_
#define _ARM_INT_MWGWTYPES_H_

/*
 * 7.18.1 Integer types
 */

/* 7.18.1.2 Minimum-width integer types */

typedef	signed char		  int_least8_t;
typedef	unsigned char		 uint_least8_t;
typedef	short int		 int_least16_t;
typedef	unsigned short int	uint_least16_t;
typedef	int			 int_least32_t;
typedef	unsigned int		uint_least32_t;
#ifdef __COMPILER_INT64__
typedef	__COMPILER_INT64__	 int_least64_t;
typedef	__COMPILER_UINT64__	uint_least64_t;
#else
/* LONGLONG */
typedef	long long int		 int_least64_t;
/* LONGLONG */
typedef	unsigned long long int	uint_least64_t;
#endif

/* 7.18.1.3 Fastest minimum-width integer types */
typedef	int			   int_fast8_t;
typedef	unsigned int		  uint_fast8_t;
typedef	int			  int_fast16_t;
typedef	unsigned int		 uint_fast16_t;
typedef	int			  int_fast32_t;
typedef	unsigned int		 uint_fast32_t;
#ifdef __COMPILER_INT64__
typedef	__COMPILER_INT64__	  int_fast64_t;
typedef	__COMPILER_UINT64__	 uint_fast64_t;
#else
/* LONGLONG */
typedef	long long int		  int_fast64_t;
/* LONGLONG */
typedef	unsigned long long int	 uint_fast64_t;
#endif

/* 7.18.1.5 Greatest-width integer types */

#ifdef __COMPILER_INT64__
typedef	__COMPILER_INT64__	      intmax_t;
typedef	__COMPILER_UINT64__	     uintmax_t;
#else
/* LONGLONG */
typedef	long long int		      intmax_t;
/* LONGLONG */
typedef	unsigned long long int	     uintmax_t;
#endif

#endif /* !_ARM_INT_MWGWTYPES_H_ */
