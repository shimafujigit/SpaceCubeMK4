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

/*	$NetBSD: limits.h,v 1.28 2008/08/27 08:53:55 christos Exp $	*/

/*
 * Copyright (c) 1988, 1993
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
 *	@(#)limits.h	8.2 (Berkeley) 1/4/94
 */

#ifndef _LIMITS_H_
#define	_LIMITS_H_

#include <sys/featuretest.h>

#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) || \
    defined(_NETBSD_SOURCE)
#define	_POSIX_AIO_LISTIO_MAX	2
#define	_POSIX_AIO_MAX		1
#define	_POSIX_ARG_MAX		4096
#define	_POSIX_CHILD_MAX	25
#define	_POSIX_HOST_NAME_MAX	255
#define	_POSIX_LINK_MAX		8
#define	_POSIX_LOGIN_NAME_MAX	9
#define	_POSIX_MAX_CANON	255
#define	_POSIX_MAX_INPUT	255
#define	_POSIX_MQ_OPEN_MAX	8
#define	_POSIX_MQ_PRIO_MAX	32
#define	_POSIX_NAME_MAX		14
#define	_POSIX_NGROUPS_MAX	8
#define	_POSIX_OPEN_MAX		20
#define	_POSIX_PATH_MAX		256
#define	_POSIX_PIPE_BUF		512
#define	_POSIX_RE_DUP_MAX	255
#define	_POSIX_SSIZE_MAX	32767
#define	_POSIX_STREAM_MAX	8
#define	_POSIX_SYMLINK_MAX	255
#define	_POSIX_SYMLOOP_MAX	8
#define	_POSIX_THREAD_DESTRUCTOR_ITERATIONS	4
#define	_POSIX_THREAD_KEYS_MAX	128
#define	_POSIX_THREAD_THREADS_MAX		64
#define	_POSIX_TIMER_MAX	32
#define	_POSIX_TTY_NAME_MAX	9
#define	_POSIX_TZNAME_MAX	6

#define	_POSIX2_BC_BASE_MAX	99
#define	_POSIX2_BC_DIM_MAX	2048
#define	_POSIX2_BC_SCALE_MAX	99
#define	_POSIX2_BC_STRING_MAX	1000
#define	_POSIX2_CHARCLASS_NAME_MAX	14
#define	_POSIX2_COLL_WEIGHTS_MAX	2
#define	_POSIX2_EXPR_NEST_MAX	32
#define	_POSIX2_LINE_MAX	2048
#define	_POSIX2_RE_DUP_MAX	255

/*
 * X/Open CAE Specifications,
 * adopted in IEEE Std 1003.1-2001 XSI.
 */
#if (_POSIX_C_SOURCE - 0) >= 200112L || defined(_XOPEN_SOURCE) || \
    defined(_NETBSD_SOURCE)
#define	_XOPEN_IOV_MAX		16
#define	_XOPEN_NAME_MAX		256
#define	_XOPEN_PATH_MAX		1024

#define PASS_MAX		128		/* Legacy */

#define CHARCLASS_NAME_MAX	14
#define NL_ARGMAX		9
#define NL_LANGMAX		14
#define NL_MSGMAX		32767
#define NL_NMAX			1
#define NL_SETMAX		255
#define NL_TEXTMAX		2048

	/* IEEE Std 1003.1-2001 TSF */
#define	_GETGR_R_SIZE_MAX	1024
#define	_GETPW_R_SIZE_MAX	1024

/* Always ensure that this is consistent with <stdio.h> */
#ifndef TMP_MAX
#define TMP_MAX			308915776	/* Legacy */
#endif
#endif /* _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE || _NETBSD_SOURCE */

#endif /* _POSIX_C_SOURCE || _XOPEN_SOURCE || _NETBSD_SOURCE */

#include <machine/limits.h>
#include <sys/syslimits.h>

#endif /* !_LIMITS_H_ */
