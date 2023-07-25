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

/*	$NetBSD: dlfcn.h,v 1.19.8.1 2009/10/14 09:31:28 sborrill Exp $	*/

/*-
 * Copyright (c) 1998 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Paul Kranenburg.
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

#ifndef _DLFCN_H_
#define _DLFCN_H_

#include <sys/featuretest.h>
#include <sys/cdefs.h>

#if defined(_NETBSD_SOURCE)
typedef struct _dl_info {
	const char	*dli_fname;	/* File defining the symbol */
	void		*dli_fbase;	/* Base address */
	const char	*dli_sname;	/* Symbol name */
	const void	*dli_saddr;	/* Symbol address */
} Dl_info;
#endif /* defined(_NETBSD_SOURCE) */

/*
 * User interface to the run-time linker.
 */
__BEGIN_DECLS
void	*dlopen(const char *, int);
int	dlclose(void *);
void	*dlsym(void * __restrict, const char * __restrict);
#if defined(_NETBSD_SOURCE)
int	dladdr(const void * __restrict, Dl_info * __restrict);
int	dlctl(void *, int, void *);
int	dlinfo(void *, int, void *);
#endif
__aconst char *dlerror(void);
__END_DECLS

/* Values for dlopen `mode'. */
#define RTLD_LAZY	1
#define RTLD_NOW	2
#define RTLD_GLOBAL	0x100		/* Allow global searches in object */
#define RTLD_LOCAL	0x200
#if defined(_NETBSD_SOURCE)
#define DL_LAZY		RTLD_LAZY	/* Compat */
#endif

/* 
 * Special handle arguments for dlsym().
 */   
#define	RTLD_NEXT	((void *) -1)	/* Search subsequent objects. */
#define	RTLD_DEFAULT	((void *) -2)	/* Use default search algorithm. */
#define	RTLD_SELF	((void *) -3)	/* Search the caller itself. */

/*
 * dlctl() commands
 */
#if defined(_NETBSD_SOURCE)
#define DL_GETERRNO	1
#define DL_GETSYMBOL	2
#if 0
#define DL_SETSRCHPATH	x
#define DL_GETLIST	x
#define DL_GETREFCNT	x
#define DL_GETLOADADDR	x
#endif /* 0 */
#endif /* defined(_NETBSD_SOURCE) */

/*
 * dlinfo() commands
 *
 * From Solarisa: http://docs.sun.com/app/docs/doc/816-5168/dlinfo-3c?a=view
 */
#if defined(_NETBSD_SOURCE)
#define RTLD_DI_LINKMAP		3
#if 0
#define RTLD_DI_ARGSINFO	1
#define RTLD_DI_CONFIGADDR	2
#define RTLD_DI_LMID		4
#define RTLD_DI_SERINFO		5
#define RTLD_DI_SERINFOSIZE	6
#define RTLD_DI_ORIGIN		7
#define RTLD_DI_GETSIGNAL	8
#define RTLD_DI_SETSIGNAL	9
#endif
#endif /* _NETBSD_SOURCE */

#endif /* !defined(_DLFCN_H_) */
