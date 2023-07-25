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

/*	$NetBSD: subr_hash.c,v 1.3 2008/05/05 17:11:17 ad Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1991, 1993
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
 *	@(#)kern_subr.c	8.4 (Berkeley) 2/14/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: subr_hash.c,v 1.3 2008/05/05 17:11:17 ad Exp $");

#include <sys/param.h>
#include <sys/kmem.h>
#include <sys/systm.h>

/*
 * General routine to allocate a hash table.
 * Allocate enough memory to hold at least `elements' list-head pointers.
 * Return a pointer to the allocated space and set *hashmask to a pattern
 * suitable for masking a value to use as an index into the returned array.
 */
void *
hashinit(u_int elements, enum hashtype htype, bool waitok, u_long *hashmask)
{
	u_long hashsize, i;
	LIST_HEAD(, generic) *hashtbl_list;
	SLIST_HEAD(, generic) *hashtbl_slist;
	TAILQ_HEAD(, generic) *hashtbl_tailq;
	size_t esize;
	void *p;

	if (elements == 0)
		panic("hashinit: bad cnt");
	for (hashsize = 1; hashsize < elements; hashsize <<= 1)
		continue;

	switch (htype) {
	case HASH_LIST:
		esize = sizeof(*hashtbl_list);
		break;
	case HASH_SLIST:
		esize = sizeof(*hashtbl_slist);
		break;
	case HASH_TAILQ:
		esize = sizeof(*hashtbl_tailq);
		break;
	default:
		panic("hashinit: invalid table type");
	}

	p = kmem_alloc(hashsize * esize, (waitok ? KM_SLEEP : KM_NOSLEEP));
	if (p == NULL)
		return (NULL);

	switch (htype) {
	case HASH_LIST:
		hashtbl_list = p;
		for (i = 0; i < hashsize; i++)
			LIST_INIT(&hashtbl_list[i]);
		break;
	case HASH_SLIST:
		hashtbl_slist = p;
		for (i = 0; i < hashsize; i++)
			SLIST_INIT(&hashtbl_slist[i]);
		break;
	case HASH_TAILQ:
		hashtbl_tailq = p;
		for (i = 0; i < hashsize; i++)
			TAILQ_INIT(&hashtbl_tailq[i]);
		break;
	}
	*hashmask = hashsize - 1;
	return (p);
}

/*
 * Free memory from hash table previosly allocated via hashinit().
 */
void
hashdone(void *hashtbl, enum hashtype htype, u_long hashmask)
{
	LIST_HEAD(, generic) *hashtbl_list;
	SLIST_HEAD(, generic) *hashtbl_slist;
	TAILQ_HEAD(, generic) *hashtbl_tailq;
	size_t esize;

	switch (htype) {
	case HASH_LIST:
		esize = sizeof(*hashtbl_list);
		break;
	case HASH_SLIST:
		esize = sizeof(*hashtbl_slist);
		break;
	case HASH_TAILQ:
		esize = sizeof(*hashtbl_tailq);
		break;
	default:
		panic("hashdone: invalid table type");
	}

	kmem_free(hashtbl, esize * (hashmask + 1));
}
