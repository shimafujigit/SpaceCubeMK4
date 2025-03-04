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

/*	$NetBSD: assertions.h,v 1.2.14.1 2011/01/09 20:43:28 riz Exp $	*/

/*
 * Copyright (C) 2004, 2005, 2008  Internet Systems Consortium, Inc. ("ISC")
 * Copyright (C) 1997-2001  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Id: assertions.h,v 1.5 2008/11/14 02:36:51 marka Exp
 */

#ifndef ASSERTIONS_H
#define ASSERTIONS_H		1

typedef enum {
	assert_require, assert_ensure, assert_insist, assert_invariant
} assertion_type;

typedef void (*assertion_failure_callback)(const char *, int, assertion_type,
					   const char *, int);

/* coverity[+kill] */
extern assertion_failure_callback __assertion_failed;
void set_assertion_failure_callback(assertion_failure_callback f);
const char *assertion_type_to_text(assertion_type type);

#if defined(CHECK_ALL) || defined(__COVERITY__)
#define CHECK_REQUIRE		1
#define CHECK_ENSURE		1
#define CHECK_INSIST		1
#define CHECK_INVARIANT		1
#endif

#if defined(CHECK_NONE) && !defined(__COVERITY__)
#define CHECK_REQUIRE		0
#define CHECK_ENSURE		0
#define CHECK_INSIST		0
#define CHECK_INVARIANT		0
#endif

#ifdef _DIAGNOSTIC
#ifndef CHECK_REQUIRE
#define CHECK_REQUIRE		1
#endif

#ifndef CHECK_ENSURE
#define CHECK_ENSURE		1
#endif

#ifndef CHECK_INSIST
#define CHECK_INSIST		1
#endif

#ifndef CHECK_INVARIANT
#define CHECK_INVARIANT		1
#endif
#endif	/* _DIAGNOSTIC */

#if CHECK_REQUIRE != 0
#define REQUIRE(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_require, \
				       #cond, 0), 0)))
#define REQUIRE_ERR(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_require, \
				       #cond, 1), 0)))
#else
#define REQUIRE(cond)		((void) (cond))
#define REQUIRE_ERR(cond)	((void) (cond))
#endif /* CHECK_REQUIRE */

#if CHECK_ENSURE != 0
#define ENSURE(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_ensure, \
				       #cond, 0), 0)))
#define ENSURE_ERR(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_ensure, \
				       #cond, 1), 0)))
#else
#define ENSURE(cond)		((void) (cond))
#define ENSURE_ERR(cond)	((void) (cond))
#endif /* CHECK_ENSURE */

#if CHECK_INSIST != 0
#define INSIST(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_insist, \
				       #cond, 0), 0)))
#define INSIST_ERR(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_insist, \
				       #cond, 1), 0)))
#else
#if !defined(__lint__)
#define INSIST(cond)		((void) (cond))
#define INSIST_ERR(cond)	((void) (cond))
#else /* !__lint__ */
#define INSIST(cond)
#define INSIST_ERR(cond)
#endif /* !__lint__ */
#endif /* CHECK_INSIST */

#if CHECK_INVARIANT != 0
#define INVARIANT(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_invariant, \
				       #cond, 0), 0)))
#define INVARIANT_ERR(cond) \
	((void) ((cond) || \
		 ((__assertion_failed)(__FILE__, __LINE__, assert_invariant, \
				       #cond, 1), 0)))
#else
#define INVARIANT(cond)		((void) (cond))
#define INVARIANT_ERR(cond)	((void) (cond))
#endif /* CHECK_INVARIANT */
#endif /* ASSERTIONS_H */
/*! \file */
