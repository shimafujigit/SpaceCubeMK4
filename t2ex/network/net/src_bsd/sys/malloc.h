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

/*	$NetBSD: malloc.h,v 1.99 2007/11/11 23:22:25 matt Exp $	*/

/*
 * Copyright (c) 1987, 1993
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
 *	@(#)malloc.h	8.5 (Berkeley) 5/3/95
 */

#ifndef _SYS_MALLOC_H_
#define	_SYS_MALLOC_H_

#if defined(_KERNEL_OPT)
#include "opt_kmemstats.h"
#include "opt_malloclog.h"
#include "opt_malloc_debug.h"
#endif

#ifdef T2EX
#include <netmain/tkn_malloc.h>
#endif

/*
 * flags to malloc
 */
#define	M_WAITOK	0x0000	/* can wait for resources */
#define	M_NOWAIT	0x0001	/* do not wait for resources */
#define	M_ZERO		0x0002	/* zero the allocation */
#define	M_CANFAIL	0x0004	/* can fail if requested memory can't ever
				 * be allocated */
#ifdef _KERNEL

#include <sys/mallocvar.h>
/*
 * The following are standard, built-in malloc types that are
 * not specific to any one subsystem.
 */
MALLOC_DECLARE(M_DEVBUF);
#ifndef T2EX
MALLOC_DECLARE(M_DMAMAP);
#endif
MALLOC_DECLARE(M_FREE);
MALLOC_DECLARE(M_PCB);
MALLOC_DECLARE(M_SOFTINTR);
MALLOC_DECLARE(M_TEMP);
#ifdef T2EX
MALLOC_DECLARE(M_KMEM);
#endif

/* XXX These should all be declared elsewhere. */
MALLOC_DECLARE(M_RTABLE);
MALLOC_DECLARE(M_FTABLE);
#ifndef T2EX
MALLOC_DECLARE(M_UFSMNT);
#endif
MALLOC_DECLARE(M_NETADDR);
MALLOC_DECLARE(M_IPMOPTS);
MALLOC_DECLARE(M_IPMADDR);
#ifndef T2EX
MALLOC_DECLARE(M_MRTABLE);
MALLOC_DECLARE(M_BWMETER);
MALLOC_DECLARE(M_1394DATA);
#endif
#endif /* _KERNEL */

/*
 * Set of buckets for each size of memory block that is retained
 */
struct kmembuckets {
	void *kb_next;	/* list of free blocks */
	void *kb_last;	/* last free block */
	long	kb_calls;	/* total calls to allocate this size */
	long	kb_total;	/* total number of blocks allocated */
	long	kb_totalfree;	/* # of free elements in this bucket */
	long	kb_elmpercl;	/* # of elements in this sized allocation */
	long	kb_highwat;	/* high water mark */
	long	kb_couldfree;	/* over high water mark and could free */
};

#ifdef _KERNEL
#define	MALLOC(space, cast, size, type, flags) \
	(space) = (cast)malloc((u_long)(size), (type), (flags))
#define	FREE(addr, type) free((void *)(addr), (type))

#ifdef MALLOCLOG
void	*_malloc(unsigned long, struct malloc_type *, int, const char *, long);
void	_free(void *, struct malloc_type *, const char *, long);
#define	malloc(size, type, flags) \
	    _malloc((size), (type), (flags), __FILE__, __LINE__)
#define	free(addr, type) \
	    _free((addr), (type), __FILE__, __LINE__)
#else
#ifdef T2EX
#define	malloc(size, type, flags) \
	    tkn_malloc((size), (type), (flags))
#define	free(addr, type) \
	    tkn_free((addr), (type))
#else
void	*malloc(unsigned long, struct malloc_type *, int);
void	free(void *, struct malloc_type *);
#endif /* T2EX */
#endif /* MALLOCLOG */

#ifdef MALLOC_DEBUG
int	debug_malloc(unsigned long, struct malloc_type *, int, void **);
int	debug_free(void *, struct malloc_type *);

void	debug_malloc_print(void);
void	debug_malloc_printit(void (*)(const char *, ...), vaddr_t);
#endif /* MALLOC_DEBUG */

#ifdef T2EX
#define	realloc(p, s, t, f)	tkn_realloc(p, s, t, f)	/* unsupported */
#define	malloc_roundup(s)	tkn_malloc_roundup(s)	/* unsupported */
#else
void	*realloc(void *, unsigned long, struct malloc_type *, int);
unsigned long
	malloc_roundup(unsigned long);
#endif /* T2EX */
#endif /* _KERNEL */
#endif /* !_SYS_MALLOC_H_ */
