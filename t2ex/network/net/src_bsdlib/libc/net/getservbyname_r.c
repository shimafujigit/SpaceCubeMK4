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

/*	$NetBSD: getservbyname_r.c,v 1.6.28.1 2011/04/05 06:23:25 riz Exp $	*/

/*
 * Copyright (c) 1983, 1993
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
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)getservbyname.c	8.1 (Berkeley) 6/4/93";
#else
__RCSID("$NetBSD: getservbyname_r.c,v 1.6.28.1 2011/04/05 06:23:25 riz Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

#include "namespace.h"
#include <assert.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <db.h>

#include "servent.h"

#ifdef __weak_alias
__weak_alias(getservbyname_r,_getservbyname_r)
#endif

static struct servent *
_servent_getbyname(struct servent_data *sd, struct servent *sp,
    const char *name, const char *proto)
{
	if (sd->db == NULL)
		return NULL;

#ifndef T2EX
	if (sd->flags & _SV_DB) {
		char buf[BUFSIZ];
		DBT key, data;
		DB *db = sd->db;
		key.data = buf;

		if (proto == NULL)
			key.size = snprintf(buf, sizeof(buf), "\376%s", name);
		else
			key.size = snprintf(buf, sizeof(buf), "\376%s/%s",
			    name, proto);
		key.size++;
		if (key.size > sizeof(buf))
			return NULL;
			
		if ((*db->get)(db, &key, &data, 0) != 0)
			return NULL;

		if ((*db->get)(db, &data, &key, 0) != 0)
			return NULL;

		if (sd->line)
			free(sd->line);

		sd->line = strdup(key.data);
		return _servent_parseline(sd, sp);
	} else {
#endif
		while (_servent_getline(sd) != -1) {
			char **cp;
			if (_servent_parseline(sd, sp) == NULL)
				continue;

			if (strcmp(name, sp->s_name) == 0)
				goto gotname;

			for (cp = sp->s_aliases; *cp; cp++)
				if (strcmp(name, *cp) == 0)
					goto gotname;
			continue;
gotname:
			if (proto == NULL || strcmp(sp->s_proto, proto) == 0)
				return sp;
		}
		return NULL;
#ifndef T2EX
	}
#endif
}

struct servent *
getservbyname_r(const char *name, const char *proto, struct servent *sp,
    struct servent_data *sd)
{
	_DIAGASSERT(name != NULL);
	/* proto may be NULL */

	setservent_r(sd->flags & _SV_STAYOPEN, sd);
	sp = _servent_getbyname(sd, sp, name, proto);
	if (!(sd->flags & _SV_STAYOPEN))
		_servent_close(sd);
	return sp;
}
