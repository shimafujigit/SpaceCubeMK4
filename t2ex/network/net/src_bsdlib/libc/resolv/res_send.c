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

/*	$NetBSD: res_send.c,v 1.15.12.1 2011/01/09 20:43:29 riz Exp $	*/

/*
 * Portions Copyright (C) 2004-2009  Internet Systems Consortium, Inc. ("ISC")
 * Portions Copyright (C) 1996-2003  Internet Software Consortium.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Copyright (c) 1985, 1989, 1993
 *    The Regents of the University of California.  All rights reserved.
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
 * 	This product includes software developed by the University of
 * 	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
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

/*
 * Portions Copyright (c) 1993 by Digital Equipment Corporation.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies, and that
 * the name of Digital Equipment Corporation not be used in advertising or
 * publicity pertaining to distribution of the document or software without
 * specific, written prior permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND DIGITAL EQUIPMENT CORP. DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL DIGITAL EQUIPMENT
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/*
 * Copyright (c) 2005 by Internet Systems Consortium, Inc. ("ISC")
 * Portions Copyright (c) 1996-1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
#ifdef notdef
static const char sccsid[] = "@(#)res_send.c	8.1 (Berkeley) 6/4/93";
static const char rcsid[] = "Id: res_send.c,v 1.22 2009/01/22 23:49:23 tbox Exp";
#else
__RCSID("$NetBSD: res_send.c,v 1.15.12.1 2011/01/09 20:43:29 riz Exp $");
#endif
#endif /* LIBC_SCCS and not lint */

/*! \file
 * \brief
 * Send query to name server and wait for reply.
 */

#ifdef T2EX
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <t2ex/socket.h>
#include <t2ex/errno.h>
#endif

#include "namespace.h"
#include "port_before.h"
#include "fd_setsize.h"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <resolv.h>
#ifndef T2EX
#include <signal.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <isc/eventlib.h>

#ifndef T2EX
#include "port_after.h"
#endif

#if 0
#ifdef __weak_alias
__weak_alias(res_ourserver_p,__res_ourserver_p)
__weak_alias(res_nameinquery,__res_nameinquery)
__weak_alias(res_queriesmatch,__res_queriesmatch)
__weak_alias(res_nsend,__res_nsend)
#endif
#endif


#ifdef USE_POLL
#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif
#include <poll.h>
#endif /* USE_POLL */

#ifndef T2EX
/* Options.  Leave them on. */
#ifndef DEBUG
#define DEBUG
#endif
#endif /* T2EX */
#include "res_debug.h"
#include "res_private.h"

#define EXT(res) ((res)->_u._ext)

#ifndef USE_POLL
static const int highestFD = FD_SETSIZE - 1;
#else
static int highestFD = 0;
#endif

/* Forward. */

static int		get_salen __P((const struct sockaddr *));
static struct sockaddr * get_nsaddr __P((res_state, size_t));
static int		send_vc(res_state, const u_char *, int,
				u_char *, int, int *, int);
static int		send_dg(res_state, const u_char *, int,
				u_char *, int, int *, int, int,
				int *, int *);
#ifndef T2EX
static void		Aerror(const res_state, FILE *, const char *, int,
			       const struct sockaddr *, int);
static void		Perror(const res_state, FILE *, const char *, int);
#endif
static int		sock_eq(struct sockaddr *, struct sockaddr *);
#if defined(NEED_PSELECT) && !defined(USE_POLL)
static int		pselect(int, void *, void *, void *,
				struct timespec *,
				const sigset_t *);
#endif
#ifndef T2EX
void res_pquery(const res_state, const u_char *, int, FILE *);
#endif

static const int niflags = NI_NUMERICHOST | NI_NUMERICSERV;

/* Public. */

/*%
 *	looks up "ina" in _res.ns_addr_list[]
 *
 * returns:
 *\li	0  : not found
 *\li	>0 : found
 *
 * author:
 *\li	paul vixie, 29may94
 */
int
res_ourserver_p(const res_state statp, const struct sockaddr *sa) {
	const struct sockaddr_in *inp, *srv;
	const struct sockaddr_in6 *in6p, *srv6;
	int ns;

	switch (sa->sa_family) {
	case AF_INET:
		inp = (const struct sockaddr_in *)(const void *)sa;
		for (ns = 0;  ns < statp->nscount;  ns++) {
			srv = (struct sockaddr_in *)(void *)get_nsaddr(statp, (size_t)ns);
			if (srv->sin_family == inp->sin_family &&
			    srv->sin_port == inp->sin_port &&
			    (srv->sin_addr.s_addr == INADDR_ANY ||
			     srv->sin_addr.s_addr == inp->sin_addr.s_addr))
				return (1);
		}
		break;
	case AF_INET6:
		if (EXT(statp).ext == NULL)
			break;
		in6p = (const struct sockaddr_in6 *)(const void *)sa;
		for (ns = 0;  ns < statp->nscount;  ns++) {
			srv6 = (struct sockaddr_in6 *)(void *)get_nsaddr(statp, (size_t)ns);
			if (srv6->sin6_family == in6p->sin6_family &&
			    srv6->sin6_port == in6p->sin6_port &&
#ifdef HAVE_SIN6_SCOPE_ID
			    (srv6->sin6_scope_id == 0 ||
			     srv6->sin6_scope_id == in6p->sin6_scope_id) &&
#endif
			    (IN6_IS_ADDR_UNSPECIFIED(&srv6->sin6_addr) ||
			     IN6_ARE_ADDR_EQUAL(&srv6->sin6_addr, &in6p->sin6_addr)))
				return (1);
		}
		break;
	default:
		break;
	}
	return (0);
}

/*%
 *	look for (name,type,class) in the query section of packet (buf,eom)
 *
 * requires:
 *\li	buf + HFIXEDSZ <= eom
 *
 * returns:
 *\li	-1 : format error
 *\li	0  : not found
 *\li	>0 : found
 *
 * author:
 *\li	paul vixie, 29may94
 */
int
res_nameinquery(const char *name, int type, int class,
		const u_char *buf, const u_char *eom)
{
	const u_char *cp = buf + HFIXEDSZ;
	int qdcount = ntohs(((const HEADER*)(const void *)buf)->qdcount);
#ifdef T2EX
	char *tname = NULL;
#endif

	while (qdcount-- > 0) {
#ifndef T2EX
		char tname[MAXDNAME+1];
#endif
		int n, ttype, tclass;

#ifndef T2EX
		n = dn_expand(buf, eom, cp, tname, sizeof tname);
		if (n < 0)
			return (-1);
#else
		if ( tname == NULL ) {
			tname = malloc(MAXDNAME+1);
			if ( tname == NULL ) {
				return -1;
			}
		}
		n = dn_expand(buf, eom, cp, tname, MAXDNAME+1);
		if (n < 0) {
			free(tname);
			return (-1);
		}
#endif
		cp += n;
#ifndef T2EX
		if (cp + 2 * INT16SZ > eom)
			return (-1);
#else
		if (cp + 2 * INT16SZ > eom) {
			free(tname);
			return (-1);
		}
#endif
		ttype = ns_get16(cp); cp += INT16SZ;
		tclass = ns_get16(cp); cp += INT16SZ;
#ifndef T2EX
		if (ttype == type && tclass == class &&
		    ns_samename(tname, name) == 1)
			return (1);
#else
		if (ttype == type && tclass == class &&
		    ns_samename(tname, name) == 1) {
			free(tname);
			return (1);
		}
#endif
	}
#ifdef T2EX
	if ( tname != NULL ) {
		free(tname);
	}
#endif
	return (0);
}

/*%
 *	is there a 1:1 mapping of (name,type,class)
 *	in (buf1,eom1) and (buf2,eom2)?
 *
 * returns:
 *\li	-1 : format error
 *\li	0  : not a 1:1 mapping
 *\li	>0 : is a 1:1 mapping
 *
 * author:
 *\li	paul vixie, 29may94
 */
int
res_queriesmatch(const u_char *buf1, const u_char *eom1,
		 const u_char *buf2, const u_char *eom2)
{
	const u_char *cp = buf1 + HFIXEDSZ;
	int qdcount = ntohs(((const HEADER*)(const void *)buf1)->qdcount);
#ifdef T2EX
	char *tname = NULL;
#endif

	if (buf1 + HFIXEDSZ > eom1 || buf2 + HFIXEDSZ > eom2)
		return (-1);

	/*
	 * Only header section present in replies to
	 * dynamic update packets.
	 */
	if ((((const HEADER *)(const void *)buf1)->opcode == ns_o_update) &&
	    (((const HEADER *)(const void *)buf2)->opcode == ns_o_update))
		return (1);

	if (qdcount != ntohs(((const HEADER*)(const void *)buf2)->qdcount))
		return (0);
	while (qdcount-- > 0) {
#ifndef T2EX
		char tname[MAXDNAME+1];
#endif
		int n, ttype, tclass;

#ifndef T2EX
		n = dn_expand(buf1, eom1, cp, tname, sizeof tname);
		if (n < 0)
			return (-1);
#else
		if ( tname == NULL ) {
			tname = malloc(MAXDNAME+1);
			if ( tname == NULL ) {
				return (-1);
			}
		}
		n = dn_expand(buf1, eom1, cp, tname, MAXDNAME+1);
		if (n < 0) {
			free(tname);
			return (-1);
		}
#endif
		cp += n;
#ifndef T2EX
		if (cp + 2 * INT16SZ > eom1)
			return (-1);
#else
		if (cp + 2 * INT16SZ > eom1) {
			free(tname);
			return (-1);
		}
#endif
		ttype = ns_get16(cp);	cp += INT16SZ;
		tclass = ns_get16(cp); cp += INT16SZ;
#ifndef T2EX
		if (!res_nameinquery(tname, ttype, tclass, buf2, eom2))
			return (0);
#else
		if (!res_nameinquery(tname, ttype, tclass, buf2, eom2)) {
			free(tname);
			return (0);
		}
#endif
	}
#ifdef T2EX
	if ( tname != NULL ) {
		free(tname);
	}
#endif
	return (1);
}

int
res_nsend(res_state statp,
	  const u_char *buf, int buflen, u_char *ans, int anssiz)
{
	int gotsomewhere, terrno, tries, v_circuit, resplen, ns, n;
#ifndef T2EX
	char abuf[NI_MAXHOST];
#endif

#ifdef USE_POLL
	highestFD = sysconf(_SC_OPEN_MAX) - 1;
#endif

	(void)res_check(statp, NULL);

	/* No name servers or res_init() failure */
	if (statp->nscount == 0 || EXT(statp).ext == NULL) {
#ifndef T2EX
		errno = ESRCH;
		return (-1);
#else
		return ERRNOtoER(ESRCH);
#endif
	}
	if (anssiz < HFIXEDSZ) {
#ifndef T2EX
		errno = EINVAL;
		return (-1);
#else
		return ERRNOtoER(EINVAL);
#endif
	}
	DprintQ((statp->options & RES_DEBUG) || (statp->pfcode & RES_PRF_QUERY),
		(stdout, ";; res_send()\n"), buf, buflen);
	v_circuit = (statp->options & RES_USEVC) || buflen > PACKETSZ;
	gotsomewhere = 0;
	terrno = ETIMEDOUT;

	/*
	 * If the ns_addr_list in the resolver context has changed, then
	 * invalidate our cached copy and the associated timing data.
	 */
	if (EXT(statp).nscount != 0) {
		int needclose = 0;
		struct sockaddr_storage peer;
		ISC_SOCKLEN_T peerlen;

		if (EXT(statp).nscount != statp->nscount)
			needclose++;
		else
			for (ns = 0; ns < statp->nscount; ns++) {
				if (statp->nsaddr_list[ns].sin_family &&
				    !sock_eq((struct sockaddr *)(void *)&statp->nsaddr_list[ns],
					     (struct sockaddr *)(void *)&EXT(statp).ext->nsaddrs[ns])) {
					needclose++;
					break;
				}

				if (EXT(statp).nssocks[ns] == -1)
					continue;
				peerlen = sizeof(peer);
#ifndef T2EX
				if (getpeername(EXT(statp).nssocks[ns],
				    (struct sockaddr *)(void *)&peer, &peerlen) < 0) {
#else
				if (so_getpeername(EXT(statp).nssocks[ns],
				    (struct sockaddr *)(void *)&peer, &peerlen) < 0) {
#endif
					needclose++;
					break;
				}
				if (!sock_eq((struct sockaddr *)(void *)&peer,
				    get_nsaddr(statp, (size_t)ns))) {
					needclose++;
					break;
				}
			}
		if (needclose) {
			res_nclose(statp);
			EXT(statp).nscount = 0;
		}
	}

	/*
	 * Maybe initialize our private copy of the ns_addr_list.
	 */
	if (EXT(statp).nscount == 0) {
		for (ns = 0; ns < statp->nscount; ns++) {
			EXT(statp).nstimes[ns] = RES_MAXTIME;
			EXT(statp).nssocks[ns] = -1;
			if (!statp->nsaddr_list[ns].sin_family)
				continue;
			EXT(statp).ext->nsaddrs[ns].sin =
				 statp->nsaddr_list[ns];
		}
		EXT(statp).nscount = statp->nscount;
	}

	/*
	 * Some resolvers want to even out the load on their nameservers.
	 * Note that RES_BLAST overrides RES_ROTATE.
	 */
	if ((statp->options & RES_ROTATE) != 0U &&
	    (statp->options & RES_BLAST) == 0U) {
		union res_sockaddr_union inu;
		struct sockaddr_in ina;
		int lastns = statp->nscount - 1;
		int fd;
		u_int16_t nstime;

		if (EXT(statp).ext != NULL)
			inu = EXT(statp).ext->nsaddrs[0];
		ina = statp->nsaddr_list[0];
		fd = EXT(statp).nssocks[0];
		nstime = EXT(statp).nstimes[0];
		for (ns = 0; ns < lastns; ns++) {
			if (EXT(statp).ext != NULL)
				EXT(statp).ext->nsaddrs[ns] =
					EXT(statp).ext->nsaddrs[ns + 1];
			statp->nsaddr_list[ns] = statp->nsaddr_list[ns + 1];
			EXT(statp).nssocks[ns] = EXT(statp).nssocks[ns + 1];
			EXT(statp).nstimes[ns] = EXT(statp).nstimes[ns + 1];
		}
		if (EXT(statp).ext != NULL)
			EXT(statp).ext->nsaddrs[lastns] = inu;
		statp->nsaddr_list[lastns] = ina;
		EXT(statp).nssocks[lastns] = fd;
		EXT(statp).nstimes[lastns] = nstime;
	}

	/*
	 * Send request, RETRY times, or until successful.
	 */
	for (tries = 0; tries < statp->retry; tries++) {
	    for (ns = 0; ns < statp->nscount; ns++) {
		struct sockaddr *nsap;
		int nsaplen;
		nsap = get_nsaddr(statp, (size_t)ns);
		nsaplen = get_salen(nsap);
		statp->_flags &= ~RES_F_LASTMASK;
		statp->_flags |= (ns << RES_F_LASTSHIFT);
 same_ns:
		if (statp->qhook) {
			int done = 0, loops = 0;

			do {
				res_sendhookact act;

				act = (*statp->qhook)(&nsap, &buf, &buflen,
						      ans, anssiz, &resplen);
				switch (act) {
				case res_goahead:
					done = 1;
					break;
				case res_nextns:
					res_nclose(statp);
					goto next_ns;
				case res_done:
					return (resplen);
				case res_modified:
					/* give the hook another try */
					if (++loops < 42) /*doug adams*/
						break;
					/*FALLTHROUGH*/
				case res_error:
					/*FALLTHROUGH*/
				default:
					goto fail;
				}
			} while (!done);
		}

		Dprint(((statp->options & RES_DEBUG) &&
			getnameinfo(nsap, (socklen_t)nsaplen, abuf, sizeof(abuf),
				    NULL, 0, niflags) == 0),
		       (stdout, ";; Querying server (# %d) address = %s\n",
			ns + 1, abuf));


		if (v_circuit) {
			/* Use VC; at most one attempt per server. */
			tries = statp->retry;
			n = send_vc(statp, buf, buflen, ans, anssiz, &terrno,
				    ns);
			if (n < 0)
				goto fail;
			if (n == 0)
				goto next_ns;
			resplen = n;
		} else {
			/* Use datagrams. */
			n = send_dg(statp, buf, buflen, ans, anssiz, &terrno,
				    ns, tries, &v_circuit, &gotsomewhere);
			if (n < 0)
				goto fail;
			if (n == 0)
				goto next_ns;
			if (v_circuit)
				goto same_ns;
			resplen = n;
		}

		Dprint((statp->options & RES_DEBUG) ||
		       ((statp->pfcode & RES_PRF_REPLY) &&
			(statp->pfcode & RES_PRF_HEAD1)),
		       (stdout, ";; got answer:\n"));

		DprintQ((statp->options & RES_DEBUG) ||
			(statp->pfcode & RES_PRF_REPLY),
			(stdout, "%s", ""),
			ans, (resplen > anssiz) ? anssiz : resplen);

		/*
		 * If we have temporarily opened a virtual circuit,
		 * or if we haven't been asked to keep a socket open,
		 * close the socket.
		 */
		if ((v_circuit && (statp->options & RES_USEVC) == 0U) ||
		    (statp->options & RES_STAYOPEN) == 0U) {
			res_nclose(statp);
		}
		if (statp->rhook) {
			int done = 0, loops = 0;

			do {
				res_sendhookact act;

				act = (*statp->rhook)(nsap, buf, buflen,
						      ans, anssiz, &resplen);
				switch (act) {
				case res_goahead:
				case res_done:
					done = 1;
					break;
				case res_nextns:
					res_nclose(statp);
					goto next_ns;
				case res_modified:
					/* give the hook another try */
					if (++loops < 42) /*doug adams*/
						break;
					/*FALLTHROUGH*/
				case res_error:
					/*FALLTHROUGH*/
				default:
					goto fail;
				}
			} while (!done);

		}
		return (resplen);
 next_ns: ;
	   } /*foreach ns*/
	} /*foreach retry*/
	res_nclose(statp);
#ifndef T2EX
	if (!v_circuit) {
		if (!gotsomewhere)
			errno = ECONNREFUSED;	/*%< no nameservers found */
		else
			errno = ETIMEDOUT;	/*%< no answer obtained */
	} else
		errno = terrno;
#endif
	return (-1);
 fail:
	res_nclose(statp);
	return (-1);
}

/* Private */

static int
get_salen(sa)
	const struct sockaddr *sa;
{

#ifdef HAVE_SA_LEN
	/* There are people do not set sa_len.  Be forgiving to them. */
	if (sa->sa_len)
		return (sa->sa_len);
#endif

	if (sa->sa_family == AF_INET)
		return (sizeof(struct sockaddr_in));
	else if (sa->sa_family == AF_INET6)
		return (sizeof(struct sockaddr_in6));
	else
		return (0);	/*%< unknown, die on connect */
}

/*%
 * pick appropriate nsaddr_list for use.  see res_init() for initialization.
 */
static struct sockaddr *
get_nsaddr(statp, n)
	res_state statp;
	size_t n;
{

	if (!statp->nsaddr_list[n].sin_family && EXT(statp).ext) {
		/*
		 * - EXT(statp).ext->nsaddrs[n] holds an address that is larger
		 *   than struct sockaddr, and
		 * - user code did not update statp->nsaddr_list[n].
		 */
		return (struct sockaddr *)(void *)&EXT(statp).ext->nsaddrs[n];
	} else {
		/*
		 * - user code updated statp->nsaddr_list[n], or
		 * - statp->nsaddr_list[n] has the same content as
		 *   EXT(statp).ext->nsaddrs[n].
		 */
		return (struct sockaddr *)(void *)&statp->nsaddr_list[n];
	}
}

static int
send_vc(res_state statp,
	const u_char *buf, int buflen, u_char *ans, int anssiz,
	int *terrno, int ns)
{
	const HEADER *hp = (const HEADER *)(const void *)buf;
	HEADER *anhp = (HEADER *)(void *)ans;
	struct sockaddr *nsap;
	int nsaplen;
	int truncating, connreset, resplen, n;
	struct iovec iov[2];
	u_short len;
	u_char *cp;
	void *tmp;
#ifdef SO_NOSIGPIPE
	int on = 1;
#endif
#ifdef T2EX
	struct msghdr mhdr;
#endif

	nsap = get_nsaddr(statp, (size_t)ns);
	nsaplen = get_salen(nsap);

	connreset = 0;
 same_ns:
	truncating = 0;

	/* Are we still talking to whom we want to talk to? */
	if (statp->_vcsock >= 0 && (statp->_flags & RES_F_VC) != 0) {
		struct sockaddr_storage peer;
		ISC_SOCKLEN_T size = sizeof peer;

#ifndef T2EX
		if (getpeername(statp->_vcsock,
				(struct sockaddr *)(void *)&peer, &size) < 0 ||
#else
		if (so_getpeername(statp->_vcsock,
				(struct sockaddr *)(void *)&peer, &size) < 0 ||
#endif
		    !sock_eq((struct sockaddr *)(void *)&peer, nsap)) {
			res_nclose(statp);
			statp->_flags &= ~RES_F_VC;
		}
	}

	if (statp->_vcsock < 0 || (statp->_flags & RES_F_VC) == 0) {
		if (statp->_vcsock >= 0)
			res_nclose(statp);

#ifndef T2EX
		statp->_vcsock = socket(nsap->sa_family, SOCK_STREAM, 0);
#else
		statp->_vcsock = so_socket(nsap->sa_family, SOCK_STREAM, 0);
#endif
		if (statp->_vcsock > highestFD) {
			res_nclose(statp);
#ifndef T2EX
			errno = ENOTSOCK;
#endif
		}
		if (statp->_vcsock < 0) {
#ifndef T2EX
			switch (errno) {
			case EPROTONOSUPPORT:
#ifdef EPFNOSUPPORT
			case EPFNOSUPPORT:
#endif
			case EAFNOSUPPORT:
				Perror(statp, stderr, "socket(vc)", errno);
				return (0);
			default:
				*terrno = errno;
				Perror(statp, stderr, "socket(vc)", errno);
				return (-1);
			}
#else
			return -1;
#endif
		}
#ifdef SO_NOSIGPIPE
		/*
		 * Disable generation of SIGPIPE when writing to a closed
		 * socket.  Write should return -1 and set errno to EPIPE
		 * instead.
		 *
		 * Push on even if setsockopt(SO_NOSIGPIPE) fails.
		 */
		(void)setsockopt(statp->_vcsock, SOL_SOCKET, SO_NOSIGPIPE, &on,
				 sizeof(on));
#endif
#ifndef T2EX
		errno = 0;
		if (connect(statp->_vcsock, nsap, (socklen_t)nsaplen) < 0) {
#else
		if (so_connect(statp->_vcsock, nsap, (socklen_t)nsaplen) < 0) {
#endif
#ifndef T2EX
			*terrno = errno;
			Aerror(statp, stderr, "connect/vc", errno, nsap,
			    nsaplen);
#endif
			res_nclose(statp);
			return (0);
		}
		statp->_flags |= RES_F_VC;
	}

	/*
	 * Send length & message
	 */
	ns_put16((u_short)buflen, (u_char*)(void *)&len);
	iov[0] = evConsIovec(&len, INT16SZ);
	DE_CONST(buf, tmp);
	iov[1] = evConsIovec(tmp, (size_t)buflen);
#ifndef T2EX
	if (writev(statp->_vcsock, iov, 2) != (INT16SZ + buflen)) {
		*terrno = errno;
		Perror(statp, stderr, "write failed", errno);
#else
	mhdr.msg_name = NULL;
	mhdr.msg_namelen = 0;
	mhdr.msg_iov = iov;
	mhdr.msg_iovlen = 2;
	mhdr.msg_control = NULL;
	mhdr.msg_controllen = 0;
	mhdr.msg_flags = 0;
	if (so_sendmsg(statp->_vcsock, &mhdr, 0) != (INT16SZ + buflen)) {
#endif
		res_nclose(statp);
		return (0);
	}
	/*
	 * Receive length & response
	 */
 read_len:
	cp = ans;
	len = INT16SZ;
#ifndef T2EX
	while ((n = read(statp->_vcsock, (char *)cp, (size_t)len)) > 0) {
#else
	while ((n = so_read(statp->_vcsock, (char *)cp, (size_t)len)) > 0) {
#endif
		cp += n;
		if ((len -= n) == 0)
			break;
	}
	if (n <= 0) {
#ifndef T2EX
		*terrno = errno;
		Perror(statp, stderr, "read failed", errno);
#endif
		res_nclose(statp);
		/*
		 * A long running process might get its TCP
		 * connection reset if the remote server was
		 * restarted.  Requery the server instead of
		 * trying a new one.  When there is only one
		 * server, this means that a query might work
		 * instead of failing.  We only allow one reset
		 * per query to prevent looping.
		 */
		if (*terrno == ECONNRESET && !connreset) {
			connreset = 1;
			res_nclose(statp);
			goto same_ns;
		}
		res_nclose(statp);
		return (0);
	}
	resplen = ns_get16(ans);
	if (resplen > anssiz) {
		Dprint(statp->options & RES_DEBUG,
		       (stdout, ";; response truncated\n")
		       );
		truncating = 1;
		len = anssiz;
	} else
		len = resplen;
	if (len < HFIXEDSZ) {
		/*
		 * Undersized message.
		 */
		Dprint(statp->options & RES_DEBUG,
		       (stdout, ";; undersized: %d\n", len));
		*terrno = EMSGSIZE;
		res_nclose(statp);
		return (0);
	}
	cp = ans;
#ifndef T2EX
	while (len != 0 && (n = read(statp->_vcsock, (char *)cp, (size_t)len)) > 0){
#else
	while (len != 0 && (n = so_read(statp->_vcsock, (char *)cp, (size_t)len)) > 0){
#endif
		cp += n;
		len -= n;
	}
	if (n <= 0) {
#ifndef T2EX
		*terrno = errno;
		Perror(statp, stderr, "read(vc)", errno);
#endif
		res_nclose(statp);
		return (0);
	}
	if (truncating) {
		/*
		 * Flush rest of answer so connection stays in synch.
		 */
		anhp->tc = 1;
		len = resplen - anssiz;
		while (len != 0) {
#ifndef T2EX
			char junk[PACKETSZ];

			n = read(statp->_vcsock, junk,
				 (len > sizeof junk) ? sizeof junk : len);
#else
			char junk[PACKETSZ/8];

			n = so_read(statp->_vcsock, junk,
				 (len > sizeof junk) ? sizeof junk : len);
#endif
			if (n > 0)
				len -= n;
			else
				break;
		}
	}
	/*
	 * If the calling applicating has bailed out of
	 * a previous call and failed to arrange to have
	 * the circuit closed or the server has got
	 * itself confused, then drop the packet and
	 * wait for the correct one.
	 */
	if (hp->id != anhp->id) {
		DprintQ((statp->options & RES_DEBUG) ||
			(statp->pfcode & RES_PRF_REPLY),
			(stdout, ";; old answer (unexpected):\n"),
			ans, (resplen > anssiz) ? anssiz: resplen);
		goto read_len;
	}

	/*
	 * All is well, or the error is fatal.  Signal that the
	 * next nameserver ought not be tried.
	 */
	return (resplen);
}

static int
send_dg(res_state statp, const u_char *buf, int buflen, u_char *ans,
	int anssiz, int *terrno, int ns, int tries, int *v_circuit,
	int *gotsomewhere)
{
	const HEADER *hp = (const HEADER *)(const void *)buf;
	HEADER *anhp = (HEADER *)(void *)ans;
	const struct sockaddr *nsap;
	int nsaplen;
	struct timespec now, timeout, finish;
	struct sockaddr_storage from;
	ISC_SOCKLEN_T fromlen;
	int resplen, seconds, n, s;
#ifdef USE_POLL
	int     polltimeout;
	struct pollfd   pollfd;
#else
	fd_set dsmask;
#endif

	nsap = get_nsaddr(statp, (size_t)ns);
	nsaplen = get_salen(nsap);
	if (EXT(statp).nssocks[ns] == -1) {
#ifndef T2EX
		EXT(statp).nssocks[ns] = socket(nsap->sa_family, SOCK_DGRAM, 0);
#else
		EXT(statp).nssocks[ns] = so_socket(nsap->sa_family, SOCK_DGRAM, 0);
#endif
		if (EXT(statp).nssocks[ns] > highestFD) {
			res_nclose(statp);
#ifndef T2EX
			errno = ENOTSOCK;
#endif
		}
		if (EXT(statp).nssocks[ns] < 0) {
#ifndef T2EX
			switch (errno) {
			case EPROTONOSUPPORT:
#ifdef EPFNOSUPPORT
			case EPFNOSUPPORT:
#endif
			case EAFNOSUPPORT:
				Perror(statp, stderr, "socket(dg)", errno);
				return (0);
			default:
				*terrno = errno;
				Perror(statp, stderr, "socket(dg)", errno);
				return (-1);
			}
#else
			return -1;
#endif
		}
#ifndef CANNOT_CONNECT_DGRAM
		/*
		 * On a 4.3BSD+ machine (client and server,
		 * actually), sending to a nameserver datagram
		 * port with no nameserver will cause an
		 * ICMP port unreachable message to be returned.
		 * If our datagram socket is "connected" to the
		 * server, we get an ECONNREFUSED error on the next
		 * socket operation, and select returns if the
		 * error message is received.  We can thus detect
		 * the absence of a nameserver without timing out.
		 */
#ifndef T2EX
		if (connect(EXT(statp).nssocks[ns], nsap, (socklen_t)nsaplen) < 0) {
			Aerror(statp, stderr, "connect(dg)", errno, nsap,
			    nsaplen);
#else
		if (so_connect(EXT(statp).nssocks[ns], nsap, (socklen_t)nsaplen) < 0) {
#endif
			res_nclose(statp);
			return (0);
		}
#endif /* !CANNOT_CONNECT_DGRAM */
		Dprint(statp->options & RES_DEBUG,
		       (stdout, ";; new DG socket\n"))
	}
	s = EXT(statp).nssocks[ns];
#ifndef CANNOT_CONNECT_DGRAM
#ifndef T2EX
	if (send(s, (const char*)buf, (size_t)buflen, 0) != buflen) {
		Perror(statp, stderr, "send", errno);
#else
	if (so_send(s, (const char*)buf, (size_t)buflen, 0) != buflen) {
#endif
		res_nclose(statp);
		return (0);
	}
#else /* !CANNOT_CONNECT_DGRAM */
	if (sendto(s, (const char*)buf, buflen, 0, nsap, nsaplen) != buflen)
	{
		Aerror(statp, stderr, "sendto", errno, nsap, nsaplen);
		res_nclose(statp);
		return (0);
	}
#endif /* !CANNOT_CONNECT_DGRAM */

	/*
	 * Wait for reply.
	 */
	seconds = (statp->retrans << tries);
	if (ns > 0)
		seconds /= statp->nscount;
	if (seconds <= 0)
		seconds = 1;
	now = evNowTime();
	timeout = evConsTime((long)seconds, 0L);
	finish = evAddTime(now, timeout);
	goto nonow;
 wait:
	now = evNowTime();
 nonow:
#ifndef USE_POLL
	FD_ZERO(&dsmask);
	FD_SET(s, &dsmask);
	if (evCmpTime(finish, now) > 0)
		timeout = evSubTime(finish, now);
	else
		timeout = evConsTime(0L, 0L);
#ifndef T2EX
	n = pselect(s + 1, &dsmask, NULL, NULL, &timeout, NULL);
#else
	{
		struct timeval tv = {
			.tv_sec = timeout.tv_sec,
			.tv_usec = timeout.tv_nsec / 1000,
		};
		n = so_select(s + 1, &dsmask, NULL, NULL, &tv);
	}
#endif
#else
	timeout = evSubTime(finish, now);
	if (timeout.tv_sec < 0)
		timeout = evConsTime(0L, 0L);
	polltimeout = 1000*(int)timeout.tv_sec +
		(int)timeout.tv_nsec/1000000;
	pollfd.fd = s;
	pollfd.events = POLLRDNORM;
	n = poll(&pollfd, 1, polltimeout);
#endif /* USE_POLL */

	if (n == 0) {
		Dprint(statp->options & RES_DEBUG, (stdout, ";; timeout\n"));
		*gotsomewhere = 1;
		return (0);
	}
	if (n < 0) {
#ifndef T2EX
		if (errno == EINTR)
#else
		if (ERRNO(n) == EINTR)
#endif
			goto wait;
#ifndef T2EX
#ifndef USE_POLL
		Perror(statp, stderr, "select", errno);
#else
		Perror(statp, stderr, "poll", errno);
#endif /* USE_POLL */
#endif
		res_nclose(statp);
		return (0);
	}
#ifndef T2EX
	errno = 0;
#endif
	fromlen = sizeof(from);
#ifndef T2EX
	resplen = recvfrom(s, (char*)ans, (size_t)anssiz,0,
			   (struct sockaddr *)(void *)&from, &fromlen);
#else
	resplen = so_recvfrom(s, (char*)ans, (size_t)anssiz,0,
			   (struct sockaddr *)(void *)&from, &fromlen);
#endif
	if (resplen <= 0) {
#ifndef T2EX
		Perror(statp, stderr, "recvfrom", errno);
#endif
		res_nclose(statp);
		return (0);
	}
	*gotsomewhere = 1;
	if (resplen < HFIXEDSZ) {
		/*
		 * Undersized message.
		 */
		Dprint(statp->options & RES_DEBUG,
		       (stdout, ";; undersized: %d\n",
			resplen));
		*terrno = EMSGSIZE;
		res_nclose(statp);
		return (0);
	}
	if (hp->id != anhp->id) {
		/*
		 * response from old query, ignore it.
		 * XXX - potential security hazard could
		 *	 be detected here.
		 */
		DprintQ((statp->options & RES_DEBUG) ||
			(statp->pfcode & RES_PRF_REPLY),
			(stdout, ";; old answer:\n"),
			ans, (resplen > anssiz) ? anssiz : resplen);
		goto wait;
	}
	if (!(statp->options & RES_INSECURE1) &&
	    !res_ourserver_p(statp, (struct sockaddr *)(void *)&from)) {
		/*
		 * response from wrong server? ignore it.
		 * XXX - potential security hazard could
		 *	 be detected here.
		 */
		DprintQ((statp->options & RES_DEBUG) ||
			(statp->pfcode & RES_PRF_REPLY),
			(stdout, ";; not our server:\n"),
			ans, (resplen > anssiz) ? anssiz : resplen);
		goto wait;
	}
#ifdef RES_USE_EDNS0
	if (anhp->rcode == FORMERR && (statp->options & RES_USE_EDNS0) != 0U) {
		/*
		 * Do not retry if the server do not understand EDNS0.
		 * The case has to be captured here, as FORMERR packet do not
		 * carry query section, hence res_queriesmatch() returns 0.
		 */
		DprintQ(statp->options & RES_DEBUG,
			(stdout, "server rejected query with EDNS0:\n"),
			ans, (resplen > anssiz) ? anssiz : resplen);
		/* record the error */
		statp->_flags |= RES_F_EDNS0ERR;
		res_nclose(statp);
		return (0);
	}
#endif
	if (!(statp->options & RES_INSECURE2) &&
	    !res_queriesmatch(buf, buf + buflen,
			      ans, ans + anssiz)) {
		/*
		 * response contains wrong query? ignore it.
		 * XXX - potential security hazard could
		 *	 be detected here.
		 */
		DprintQ((statp->options & RES_DEBUG) ||
			(statp->pfcode & RES_PRF_REPLY),
			(stdout, ";; wrong query name:\n"),
			ans, (resplen > anssiz) ? anssiz : resplen);
		goto wait;
	}
	if (anhp->rcode == SERVFAIL ||
	    anhp->rcode == NOTIMP ||
	    anhp->rcode == REFUSED) {
		DprintQ(statp->options & RES_DEBUG,
			(stdout, "server rejected query:\n"),
			ans, (resplen > anssiz) ? anssiz : resplen);
		res_nclose(statp);
		/* don't retry if called from dig */
		if (!statp->pfcode)
			return (0);
	}
	if (!(statp->options & RES_IGNTC) && anhp->tc) {
		/*
		 * To get the rest of answer,
		 * use TCP with same server.
		 */
		Dprint(statp->options & RES_DEBUG,
		       (stdout, ";; truncated answer\n"));
		*v_circuit = 1;
		res_nclose(statp);
		return (1);
	}
	/*
	 * All is well, or the error is fatal.  Signal that the
	 * next nameserver ought not be tried.
	 */
	return (resplen);
}

#ifndef T2EX
static void
Aerror(const res_state statp, FILE *file, const char *string, int error,
       const struct sockaddr *address, int alen)
{
	int save = errno;
	char hbuf[NI_MAXHOST];
	char sbuf[NI_MAXSERV];

	alen = alen;

	if ((statp->options & RES_DEBUG) != 0U) {
		if (getnameinfo(address, (socklen_t)alen, hbuf, sizeof(hbuf),
		    sbuf, sizeof(sbuf), niflags)) {
			strncpy(hbuf, "?", sizeof(hbuf) - 1);
			hbuf[sizeof(hbuf) - 1] = '\0';
			strncpy(sbuf, "?", sizeof(sbuf) - 1);
			sbuf[sizeof(sbuf) - 1] = '\0';
		}
		fprintf(file, "res_send: %s ([%s].%s): %s\n",
			string, hbuf, sbuf, strerror(error));
	}
	errno = save;
}

static void
Perror(const res_state statp, FILE *file, const char *string, int error) {
	int save = errno;

	if ((statp->options & RES_DEBUG) != 0U)
		fprintf(file, "res_send: %s: %s\n",
			string, strerror(error));
	errno = save;
}
#endif

static int
sock_eq(struct sockaddr *a, struct sockaddr *b) {
	struct sockaddr_in *a4, *b4;
	struct sockaddr_in6 *a6, *b6;

	if (a->sa_family != b->sa_family)
		return 0;
	switch (a->sa_family) {
	case AF_INET:
		a4 = (struct sockaddr_in *)(void *)a;
		b4 = (struct sockaddr_in *)(void *)b;
		return a4->sin_port == b4->sin_port &&
		    a4->sin_addr.s_addr == b4->sin_addr.s_addr;
	case AF_INET6:
		a6 = (struct sockaddr_in6 *)(void *)a;
		b6 = (struct sockaddr_in6 *)(void *)b;
		return a6->sin6_port == b6->sin6_port &&
#ifdef HAVE_SIN6_SCOPE_ID
		    a6->sin6_scope_id == b6->sin6_scope_id &&
#endif
		    IN6_ARE_ADDR_EQUAL(&a6->sin6_addr, &b6->sin6_addr);
	default:
		return 0;
	}
}

#if defined(NEED_PSELECT) && !defined(USE_POLL)
/* XXX needs to move to the porting library. */
static int
pselect(int nfds, void *rfds, void *wfds, void *efds,
	struct timespec *tsp, const sigset_t *sigmask)
{
	struct timeval tv, *tvp;
	sigset_t sigs;
	int n;

	if (tsp) {
		tvp = &tv;
		tv = evTimeVal(*tsp);
	} else
		tvp = NULL;
	if (sigmask)
		sigprocmask(SIG_SETMASK, sigmask, &sigs);
	n = select(nfds, rfds, wfds, efds, tvp);
	if (sigmask)
		sigprocmask(SIG_SETMASK, &sigs, NULL);
	if (tsp)
		*tsp = evTimeSpec(tv);
	return (n);
}
#endif
