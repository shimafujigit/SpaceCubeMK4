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

/*	$NetBSD: kern_uidinfo.c,v 1.3.6.1 2009/03/26 17:17:31 snj Exp $	*/

/*-
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
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: kern_uidinfo.c,v 1.3.6.1 2009/03/26 17:17:31 snj Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kmem.h>
#include <sys/proc.h>
#ifndef T2EX
#include <sys/atomic.h>
#else
#include <sys/_atomic.h>
#endif
#include <sys/uidinfo.h>
#ifndef T2EX
#include <sys/cpu.h>
#endif

#ifdef T2EX
static struct uidinfo uidinfo0;
#endif

#ifndef T2EX
static SLIST_HEAD(uihashhead, uidinfo) *uihashtbl;
static u_long 		uihash;
#endif

#define	UIHASH(uid)	(&uihashtbl[(uid) & uihash])

#ifndef T2EX
void
uid_init(void)
{
	/*
	 * In case of MP system, SLIST_FOREACH would force a cache line
	 * write-back for every modified 'uidinfo', thus we try to keep the
	 * lists short.
	 */
	const u_int uihash_sz = (maxcpus > 1 ? 1024 : 64);

	uihashtbl = hashinit(uihash_sz, HASH_SLIST, true, &uihash);

	/*
	 * Ensure that uid 0 is always in the user hash table, as
	 * sbreserve() expects it available from interrupt context.
	 */
	(void)uid_find(0);
}
#else
void
uid_init(void)
{

	bzero(&uidinfo0, sizeof uidinfo0);
}

void
uid_finish(void)
{

}
#endif

struct uidinfo *
uid_find(uid_t uid)
{
#ifndef T2EX
	struct uidinfo *uip, *uip_first, *newuip;
	struct uihashhead *uipp;

	uipp = UIHASH(uid);
	newuip = NULL;

	/*
	 * To make insertion atomic, abstraction of SLIST will be violated.
	 */
	uip_first = uipp->slh_first;
 again:
	SLIST_FOREACH(uip, uipp, ui_hash) {
		if (uip->ui_uid != uid)
			continue;
		if (newuip != NULL)
			kmem_free(newuip, sizeof(*newuip));
		return uip;
	}
	if (newuip == NULL)
		newuip = kmem_zalloc(sizeof(*newuip), KM_SLEEP);
	newuip->ui_uid = uid;

	/*
	 * If atomic insert is unsuccessful, another thread might be
	 * allocated this 'uid', thus full re-check is needed.
	 */
	newuip->ui_hash.sle_next = uip_first;
	membar_producer();
	uip = atomic_cas_ptr(&uipp->slh_first, uip_first, newuip);
	if (uip != uip_first) {
		uip_first = uip;
		goto again;
	}

	return newuip;
#else
	KASSERT(uid == 0);

	return &uidinfo0;
#endif
}

/*
 * Change the count associated with number of processes
 * a given user is using.
 */
#ifndef T2EX
int
chgproccnt(uid_t uid, int diff)
{
	struct uidinfo *uip;
	long proccnt;

	uip = uid_find(uid);
	proccnt = atomic_add_long_nv(&uip->ui_proccnt, diff);
	KASSERT(proccnt >= 0);
	return proccnt;
}
#endif

int
chgsbsize(struct uidinfo *uip, u_long *hiwat, u_long to, rlim_t xmax)
{
	rlim_t nsb;
	const long diff = to - *hiwat;

	nsb = (rlim_t)atomic_add_long_nv((long *)&uip->ui_sbsize, diff);
	if (diff > 0 && nsb > xmax) {
		atomic_add_long((long *)&uip->ui_sbsize, -diff);
		return 0;
	}
	*hiwat = to;
	KASSERT(nsb >= 0);
	return 1;
}
