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

/*	$NetBSD: assert.h,v 1.19 2008/08/17 00:24:35 gmcgarry Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)assert.h	8.2 (Berkeley) 1/21/94
 */

/*
 * Unlike other ANSI header files, <assert.h> may usefully be included
 * multiple times, with and without NDEBUG defined.
 */

#include <sys/cdefs.h>
#include <sys/featuretest.h>

#undef assert

#ifdef NDEBUG
# ifndef __lint__
#  define assert(e)	(__static_cast(void,0))
# else /* !__lint__ */
#  define assert(e)
# endif /* __lint__ */
#else /* !NDEBUG */
# if __STDC__
#  define assert(e)							\
	((e) ? __static_cast(void,0) : __assert13(__FILE__, __LINE__,	\
	                                          __assert_function__, #e))
# else	/* PCC */
#  define assert(e)							\
	((e) ? __static_cast(void,0) : __assert13(__FILE__, __LINE__,	\
	                                          __assert_function__, "e"))
# endif /* !__STDC__ */
#endif /* NDEBUG */

#undef _DIAGASSERT
#if !defined(_DIAGNOSTIC)
# if !defined(__lint__)
#  define _DIAGASSERT(e) (__static_cast(void,0))
# else /* !__lint__ */
#  define _DIAGASSERT(e)
# endif /* __lint__ */
#else /* _DIAGNOSTIC */
# if __STDC__
#  define _DIAGASSERT(e)						\
	((e) ? __static_cast(void,0) : __diagassert13(__FILE__, __LINE__, \
	                                              __assert_function__, #e))
# else	/* !__STDC__ */
#  define _DIAGASSERT(e)	 					\
	((e) ? __static_cast(void,0) : __diagassert13(__FILE__, __LINE__, \
	                                              __assert_function__, "e"))
# endif
#endif /* _DIAGNOSTIC */


#if defined(__lint__)
#define	__assert_function__	(__static_cast(const void *,0))
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define	__assert_function__	__func__
#elif __GNUC_PREREQ__(2, 6)
#define	__assert_function__	__PRETTY_FUNCTION__
#else
#define	__assert_function__	(__static_cast(const void *,0))
#endif

#ifndef __ASSERT_DECLARED
#define __ASSERT_DECLARED
__BEGIN_DECLS
void __assert(const char *, int, const char *);
void __assert13(const char *, int, const char *, const char *);
void __diagassert(const char *, int, const char *);
void __diagassert13(const char *, int, const char *, const char *);
__END_DECLS
#endif /* __ASSERT_DECLARED */
