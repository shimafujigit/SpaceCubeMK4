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

/* $NetBSD: subr_evcnt.c,v 1.4 2005/12/11 12:24:30 christos Exp $ */

/*
 * Copyright (c) 1996, 2000 Christopher G. Demetriou
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
 * --(license Id: LICENSE.proto,v 1.1 2000/06/13 21:40:26 cgd Exp )--
 */

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Berkeley Laboratories.
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
 * from: Header: subr_autoconf.c,v 1.12 93/02/01 19:31:48 torek Exp  (LBL)
 *
 *	@(#)subr_autoconf.c	8.3 (Berkeley) 5/17/94
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: subr_evcnt.c,v 1.4 2005/12/11 12:24:30 christos Exp $");

#include "opt_ddb.h"

#include <sys/param.h>
#include <sys/device.h>
#include <sys/systm.h>

/* list of all events */
struct evcntlist allevents = TAILQ_HEAD_INITIALIZER(allevents);

#ifndef T2EX
/*
 * We need a dummy object to stuff into the evcnt link set to
 * ensure that there always is at least one object in the set.
 */
static struct evcnt dummy_static_evcnt;
__link_set_add_bss(evcnts, dummy_static_evcnt);

/*
 * Initialize event counters.  This does the attach procedure for
 * each of the static event counters in the "evcnts" link set.
 */
void
evcnt_init(void)
{
#ifndef T2EX
	__link_set_decl(evcnts, struct evcnt);
	struct evcnt * const *evp;

	__link_set_foreach(evp, evcnts) {
		if (*evp == &dummy_static_evcnt)
			continue;
		evcnt_attach_static(*evp);
	}
#endif
}
#endif

/*
 * Attach a statically-initialized event.  The type and string pointers
 * are already set up.
 */
void
evcnt_attach_static(struct evcnt *ev)
{
	int len;

	len = strlen(ev->ev_group);
#ifdef DIAGNOSTIC
	if (len >= EVCNT_STRING_MAX)		/* ..._MAX includes NUL */
		panic("evcnt_attach_static: group length (%s)", ev->ev_group);
#endif
	ev->ev_grouplen = len;

	len = strlen(ev->ev_name);
#ifdef DIAGNOSTIC
	if (len >= EVCNT_STRING_MAX)		/* ..._MAX includes NUL */
		panic("evcnt_attach_static: name length (%s)", ev->ev_name);
#endif
	ev->ev_namelen = len;

	TAILQ_INSERT_TAIL(&allevents, ev, ev_list);
}

/*
 * Attach a dynamically-initialized event.  Zero it, set up the type
 * and string pointers and then act like it was statically initialized.
 */
void
evcnt_attach_dynamic(struct evcnt *ev, int type, const struct evcnt *parent,
    const char *group, const char *name)
{

	memset(ev, 0, sizeof *ev);
	ev->ev_type = type;
	ev->ev_parent = parent;
	ev->ev_group = group;
	ev->ev_name = name;
	evcnt_attach_static(ev);
}

/*
 * Detach an event.
 */
void
evcnt_detach(struct evcnt *ev)
{

	TAILQ_REMOVE(&allevents, ev, ev_list);
}

#ifdef DDB
void
event_print(int full, void (*pr)(const char *, ...))
{
	struct evcnt *evp;

	TAILQ_FOREACH(evp, &allevents, ev_list) {
		if (evp->ev_count == 0 && !full)
			continue;

		(*pr)("evcnt type %d: %s %s = %lld\n", evp->ev_type,
		    evp->ev_group, evp->ev_name, evp->ev_count);
	}
}
#endif /* DDB */
