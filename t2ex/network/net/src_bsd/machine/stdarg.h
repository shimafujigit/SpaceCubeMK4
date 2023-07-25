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

/*	$NetBSD: stdarg.h,v 1.10 2008/06/21 00:56:39 gmcgarry Exp $	*/

/*
 * Copyright (c) 1991, 1993
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
 *	from: @(#)stdarg.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _ARM32_STDARG_H_
#define	_ARM32_STDARG_H_

#include <machine/ansi.h>
#include <sys/featuretest.h>

typedef _BSD_VA_LIST_	va_list;
#ifdef __lint__
#define __builtin_next_arg(t)		((t) ? 0 : 0)
#define	__builtin_stdarg_start(a, l)	((a) = ((l) ? 0 : 0))
#define	__builtin_va_arg(a, t)		((t)((a) ? 0 : 0))
#define	__builtin_va_end		/* nothing */
#define	__builtin_va_copy(d, s)		((d) = (s))
#endif

#if __GNUC_PREREQ__(2, 96)
/*#define	va_start(ap, last)	__builtin_stdarg_start((ap), (last))*/
#define	va_start(ap, last)	__builtin_va_start((ap), (last))
#define	va_arg			__builtin_va_arg
#define	va_end			__builtin_va_end
#define	__va_copy(dest, src)	__builtin_va_copy((dest), (src))
#elif defined(__PCC__)
#define	va_start(ap, last)	__builtin_stdarg_start((ap), (last))
#define	va_arg			__builtin_va_arg
#define	va_end			__builtin_va_end
#define	__va_copy(dest, src)	__builtin_va_copy((dest), (src))
#else
#define	__va_size(type) \
	(((sizeof(type) + sizeof(long) - 1) / sizeof(long)) * sizeof(long))

#define	va_start(ap, last) \
	((ap) = (va_list)__builtin_next_arg(last))

#define	va_arg(ap, type) \
	((type *)(ap += sizeof(type)))[-1]

#define	va_end(ap)

#define	__va_copy(dest, src)	((dest) = (src))

#endif /* __GNUC_PREREQ__(2, 96) */

#if !defined(_ANSI_SOURCE) &&						\
    (defined(_ISOC99_SOURCE) || (__STDC_VERSION__ - 0) >= 199901L ||	\
     defined(_NETBSD_SOURCE))
#define	va_copy(dest, src)	__va_copy((dest), (src))
#endif

#endif /* !_ARM32_STDARG_H_ */
