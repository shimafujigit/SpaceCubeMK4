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

/*	$OpenBSD: hcreate.c,v 1.4 2007/09/02 15:19:17 deraadt Exp $	*/
/*	$NetBSD: hcreate.c,v 1.5 2004/04/23 02:48:12 simonb Exp $	*/

/*
 * Copyright (c) 2001 Christopher G. Demetriou
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *          This product includes software developed for the
 *          NetBSD Project.  See http://www.NetBSD.org/ for
 *          information about NetBSD.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * <<Id: LICENSE,v 1.2 2000/06/14 15:57:33 cgd Exp>>
 */

/*
 * hcreate_r() / hsearch() / hdestroy(, struct hsearch_data *htab)
 *
 * SysV/XPG4 hash table functions.
 *
 * Implementation done based on NetBSD manual page and Solaris manual page,
 * plus my own personal experience about how they're supposed to work.
 *
 * I tried to look at Knuth (as cited by the Solaris manual page), but
 * nobody had a copy in the office, so...
 */

#include "namespace.h"
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#ifndef _DIAGASSERT
#define _DIAGASSERT(x)
#endif

/*
 * DO NOT MAKE THIS STRUCTURE LARGER THAN 32 BYTES (4 ptrs on 64-bit
 * ptr machine) without adjusting MAX_BUCKETS_LG2 below.
 */
struct internal_entry {
	SLIST_ENTRY(internal_entry) link;
	ENTRY ent;
};
SLIST_HEAD(internal_head, internal_entry);

#define	MIN_BUCKETS_LG2	4
#define	MIN_BUCKETS	(1 << MIN_BUCKETS_LG2)

/*
 * max * sizeof internal_entry must fit into size_t.
 * assumes internal_entry is <= 32 (2^5) bytes.
 */
#define	MAX_BUCKETS_LG2	(sizeof (size_t) * 8 - 1 - 5)
#define	MAX_BUCKETS	((size_t)1 << MAX_BUCKETS_LG2)

/* Default hash function, from db/hash/hash_func.c */
extern u_int32_t (*__default_hash)(const void *, size_t);

struct hsearch_data
{struct internal_head *table; size_t size;};

int
hcreate_r(size_t nel, struct hsearch_data *htab)
{
	size_t idx;
	unsigned int p2;

	/* Make sure this isn't called when a table already exists. */
	_DIAGASSERT(htab->table == NULL);
	if (htab->table != NULL) {
		/*errno = EINVAL*/;
		return 0;
	}

	/* If nel is too small, make it min sized. */
	if (nel < MIN_BUCKETS)
		nel = MIN_BUCKETS;

	/* If it's too large, cap it. */
	if (nel > MAX_BUCKETS)
		nel = MAX_BUCKETS;

	/* If it's is not a power of two in size, round up. */
	if ((nel & (nel - 1)) != 0) {
		for (p2 = 0; nel != 0; p2++)
			nel >>= 1;
		_DIAGASSERT(p2 <= MAX_BUCKETS_LG2);
		nel = 1 << p2;
	}
	
	/* Allocate the table. */
	htab->size = nel;
	htab->table = calloc(htab->size, sizeof htab->table[0]);
	if (htab->table == NULL) {
		/*errno = ENOMEM*/;
		return 0;
	}

	/* Initialize it. */
	for (idx = 0; idx < htab->size; idx++)
		SLIST_INIT(&htab->table[idx]);

	return 1;
}

void
hdestroy_r(struct hsearch_data *htab)
{
	struct internal_entry *ie;
	size_t idx;

	_DIAGASSERT(htab->table != NULL);
	if (htab->table == NULL)
		return;

	for (idx = 0; idx < htab->size; idx++) {
		while (!SLIST_EMPTY(&htab->table[idx])) {
			ie = SLIST_FIRST(&htab->table[idx]);
			SLIST_REMOVE_HEAD(&htab->table[idx], link);
			//free(ie->ent.key);
			free(ie);
		}
	}
	free(htab->table);
	htab->table = NULL;
}

int
hsearch_r(ENTRY item, ACTION action, ENTRY **result, struct hsearch_data *htab)
{
	struct internal_head *head;
	struct internal_entry *ie;
	uint32_t hashval;
	size_t len;

	_DIAGASSERT(htab->table != NULL);
	_DIAGASSERT(item.key != NULL);
	_DIAGASSERT(action == ENTER || action == FIND);

	len = strlen(item.key);
	hashval = (*__default_hash)(item.key, len);

	head = &htab->table[hashval & (htab->size - 1)];
	ie = SLIST_FIRST(head);
	while (ie != NULL) {
		if (strcmp(ie->ent.key, item.key) == 0)
			break;
		ie = SLIST_NEXT(ie, link);
	}

	if (ie != NULL)
		return (*result = &ie->ent), 1;
	else if (action == FIND)
		return 0;

	ie = malloc(sizeof *ie);
	if (ie == NULL)
		return 0;
	ie->ent.key = item.key;
	ie->ent.data = item.data;

	SLIST_INSERT_HEAD(head, ie, link);
	return (*result = &ie->ent), 1;
}
