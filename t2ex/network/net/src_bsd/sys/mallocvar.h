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

/*	$NetBSD: mallocvar.h,v 1.7 2007/11/07 16:12:25 matt Exp $	*/

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

#ifndef _SYS_MALLOCVAR_H_
#define	_SYS_MALLOCVAR_H_

#include <sys/types.h>

#define	M_MAGIC		877983977

/*
 * This structure describes a type of malloc'd memory and carries
 * allocation statistics for that memory.
 */
struct malloc_type {
	struct malloc_type *ks_next;	/* next in list */
	u_long	ks_magic;	 /* indicates valid structure */
	const char *ks_shortdesc;/* short description */

	/* Statistics */
	u_long	ks_inuse;	/* # of packets of this type currently in use */
	u_long	ks_calls;	/* total packets of this type ever allocated */
	u_long 	ks_memuse;	/* total memory held in bytes */
	u_short	ks_limblocks;	/* number of times blocked for hitting limit */
	u_short	ks_mapblocks;	/* number of times blocked for kernel map */
	u_long	ks_maxused;	/* maximum number ever used */
	u_long	ks_limit;	/* most that are allowed to exist */
	u_long	ks_size;	/* sizes of this thing that are allocated */
	u_long	ks_spare;
};

#ifdef _KERNEL
#define	MALLOC_JUSTDEFINE_LIMIT(type, shortdesc, longdesc, limit)	\
struct malloc_type type[1] = {						\
	[0] = {								\
		.ks_magic = M_MAGIC,					\
		.ks_shortdesc = shortdesc,				\
		.ks_limit = limit,					\
	},								\
};

#define	MALLOC_JUSTDEFINE(type, shortdesc, longdesc)			\
	MALLOC_JUSTDEFINE_LIMIT(type, shortdesc, longdesc, 0)

#ifndef T2EX
#define	MALLOC_DEFINE_LIMIT(type, shortdesc, longdesc, limit)		\
	MALLOC_JUSTDEFINE_LIMIT(type, shortdesc, longdesc, limit)	\
	__link_set_add_data(malloc_types, type)
#else
#define	MALLOC_DEFINE_LIMIT(type, shortdesc, longdesc, limit)		\
	MALLOC_JUSTDEFINE_LIMIT(type, shortdesc, longdesc, limit)
#endif

#define	MALLOC_DEFINE(type, shortdesc, longdesc)			\
	MALLOC_DEFINE_LIMIT(type, shortdesc, longdesc, 0)

#define	MALLOC_DECLARE(type)						\
	extern struct malloc_type type[1]

void	malloc_type_attach(struct malloc_type *);
void	malloc_type_detach(struct malloc_type *);

void	malloc_type_setlimit(struct malloc_type *, u_long);
#endif /* _KERNEL */

#endif /* _SYS_MALLOCVAR_H_ */
