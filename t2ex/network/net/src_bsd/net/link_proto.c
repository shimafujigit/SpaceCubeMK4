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

/*	$NetBSD: link_proto.c,v 1.4 2008/05/13 18:09:22 dyoung Exp $	*/

/*-
 * Copyright (c) 1982, 1986, 1993
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
 *	@(#)uipc_proto.c	8.2 (Berkeley) 2/14/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: link_proto.c,v 1.4 2008/05/13 18:09:22 dyoung Exp $");

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/protosw.h>
#include <sys/domain.h>
#include <sys/mbuf.h>
#include <sys/un.h>
#include <sys/socketvar.h>

#include <net/if.h>
#include <net/if_dl.h>
#ifndef T2EX
#include <net/raw_cb.h>
#endif
#include <net/route.h>

static int sockaddr_dl_cmp(const struct sockaddr *, const struct sockaddr *);
static int link_usrreq(struct socket *, int, struct mbuf *, struct mbuf *,
    struct mbuf *, struct lwp *);
#ifndef T2EX
static void link_init(void);
#else
static int link_init(void);
static void link_finish(void);
#endif

/*
 * Definitions of protocols supported in the link-layer domain.
 */

DOMAIN_DEFINE(linkdomain);	/* forward define and add to link set */

const struct protosw linksw[] = {
	{	.pr_type = SOCK_DGRAM,
		.pr_domain = &linkdomain,
		.pr_protocol = 0,	/* XXX */
		.pr_flags = PR_ATOMIC|PR_ADDR|PR_PURGEIF,
		.pr_input = NULL,
		.pr_ctlinput = NULL,
		.pr_ctloutput = NULL,
		.pr_usrreq = link_usrreq,
		.pr_init = link_init,
#ifdef T2EX
		.pr_finish = link_finish,
#endif
	},
};

struct domain linkdomain = {
	.dom_family = AF_LINK,
	.dom_name = "link",
	.dom_externalize = NULL,
	.dom_dispose = NULL,
	.dom_protosw = linksw,
	.dom_protoswNPROTOSW = &linksw[__arraycount(linksw)],
	.dom_sockaddr_cmp = sockaddr_dl_cmp
};

#ifndef T2EX
static void
link_init(void)
{
	return;
}
#else
static int
link_init(void)
{
	return 0;
}

static void
link_finish(void)
{

}
#endif

static int
link_control(struct socket *so, unsigned long cmd, void *data,
    struct ifnet *ifp, struct lwp *l)
{
	int error, s;
	bool isactive, mkactive;
	struct if_laddrreq *iflr;
	union {
		struct sockaddr sa;
		struct sockaddr_dl sdl;
		struct sockaddr_storage ss;
	} u;
	struct ifaddr *ifa;
	const struct sockaddr_dl *asdl, *nsdl;

	switch (cmd) {
	case SIOCALIFADDR:
	case SIOCDLIFADDR:
	case SIOCGLIFADDR:
		iflr = data;

		if (iflr->addr.ss_family != AF_LINK)
			return EINVAL;

		asdl = satocsdl(sstocsa(&iflr->addr));

		if (asdl->sdl_alen != ifp->if_addrlen)
			return EINVAL;

		if (sockaddr_dl_init(&u.sdl, sizeof(u.ss), ifp->if_index,
		    ifp->if_type, ifp->if_xname, strlen(ifp->if_xname),
		    CLLADDR(asdl), asdl->sdl_alen) == NULL)
			return EINVAL;

		if ((iflr->flags & IFLR_PREFIX) == 0)
			;
		else if (iflr->prefixlen != NBBY * ifp->if_addrlen)
			return EINVAL;	/* XXX match with prefix */

		error = 0;

		s = splnet();

		IFADDR_FOREACH(ifa, ifp) {
			if (sockaddr_cmp(&u.sa, ifa->ifa_addr) == 0)
				break;
		}

		switch (cmd) {
		case SIOCGLIFADDR:
			if ((iflr->flags & IFLR_PREFIX) == 0) {
				IFADDR_FOREACH(ifa, ifp) {
					if (ifa->ifa_addr->sa_family == AF_LINK)
						break;
				}
			}
			if (ifa == NULL) {
				error = EADDRNOTAVAIL; 
				break;
			}

			if (ifa == ifp->if_dl)
				iflr->flags = IFLR_ACTIVE;
			else
				iflr->flags = 0;

			sockaddr_copy(sstosa(&iflr->addr), sizeof(iflr->addr),
			    ifa->ifa_addr);

			break;
		case SIOCDLIFADDR:
			if (ifa == NULL)
				error = EADDRNOTAVAIL;
			else if (ifa == ifp->if_dl)
				error = EBUSY;
			else {
				/* TBD routing socket */
				rt_newaddrmsg(RTM_DELETE, ifa, 0, NULL);
				ifa_remove(ifp, ifa);
			}
			break;
		case SIOCALIFADDR:
			if (ifa != NULL)
				;
			else if ((ifa = if_dl_create(ifp, &nsdl)) == NULL) {
				error = ENOMEM;
				break;
			} else {
				sockaddr_copy(ifa->ifa_addr,
				    ifa->ifa_addr->sa_len, &u.sa);
				ifa_insert(ifp, ifa);
				rt_newaddrmsg(RTM_ADD, ifa, 0, NULL);
			}

			mkactive = (iflr->flags & IFLR_ACTIVE) != 0;
			isactive = (ifa == ifp->if_dl);

			if (!isactive && mkactive) {
				if_activate_sadl(ifp, ifa, nsdl);
				error = ENETRESET;
			}
			break;
		}
		splx(s);
		if (error != ENETRESET)
			return error;
		else if ((ifp->if_flags & IFF_RUNNING) != 0)
			return (*ifp->if_init)(ifp);
		else
			return 0;
	default:
		return ENOTTY;
	}
}

static int
link_usrreq(struct socket *so, int req, struct mbuf *m, struct mbuf *nam,
	struct mbuf *control, struct lwp *l)
{
	switch (req) {
	case PRU_ATTACH:
		sosetlock(so);
		return 0;
	case PRU_DETACH:
		sofree(so);
		return 0;
	case PRU_CONTROL:
		return link_control(so, (unsigned long)m, nam,
		    (struct ifnet *)control, l);
	default:
		return EOPNOTSUPP;
	}
}

/* Compare the field at byte offsets [fieldstart, fieldend) in
 * two memory regions, [l, l + llen) and [r, r + llen).
 */
static inline int
submemcmp(const void *l, const void *r,
    const uint_fast8_t llen, const uint_fast8_t rlen,
    const uint_fast8_t fieldstart, const uint_fast8_t fieldend)
{
	uint_fast8_t cmpend, minlen;
	const uint8_t *lb = l, *rb = r;
	int rc;

	minlen = MIN(llen, rlen);

	/* The field is missing from one region.  The shorter region is the
	 * lesser region.
	 */
	if (fieldstart >= minlen)
		return llen - rlen;

	/* Two empty, present fields are always equal. */
	if (fieldstart > fieldend)
		return 0;

	cmpend = MIN(fieldend, minlen);

	rc = memcmp(&lb[fieldstart], &rb[fieldstart], cmpend - fieldstart);

	if (rc != 0)
		return rc;
	/* If one or both fields are truncated, then the shorter is the lesser
	 * field.
	 */
	if (minlen < fieldend)
		return llen - rlen;
	/* Fields are full-length and equal.  The fields are equal. */
	return 0;
}

uint8_t
sockaddr_dl_measure(uint8_t namelen, uint8_t addrlen)
{
	return offsetof(struct sockaddr_dl, sdl_data[namelen + addrlen]);
}

#ifndef T2EX
struct sockaddr *
sockaddr_dl_alloc(uint16_t ifindex, uint8_t type,
    const void *name, uint8_t namelen, const void *addr, uint8_t addrlen,
    int flags)
{
	struct sockaddr *sa;
	socklen_t len;

	len = sockaddr_dl_measure(namelen, addrlen);
	sa = sockaddr_alloc(AF_LINK, len, flags);

	if (sa == NULL)
		return NULL;

	if (sockaddr_dl_init(satosdl(sa), len, ifindex, type, name, namelen,
	    addr, addrlen) == NULL) {
		sockaddr_free(sa);
		return NULL;
	}

	return sa;
}
#endif

struct sockaddr_dl *
sockaddr_dl_init(struct sockaddr_dl *sdl, socklen_t socklen, uint16_t ifindex,
    uint8_t type, const void *name, uint8_t namelen, const void *addr,
    uint8_t addrlen)
{
	socklen_t len;

	sdl->sdl_family = AF_LINK;
	sdl->sdl_slen = 0;
	len = sockaddr_dl_measure(namelen, addrlen);
	if (len > socklen) {
		sdl->sdl_len = socklen;
#ifdef DIAGNOSTIC
		printf("%s: too long: %" PRIu8 " > %" PRIu8 "\n", __func__, len,
		    socklen);
#endif
		return NULL;
	}
	sdl->sdl_len = len;
	sdl->sdl_index = ifindex;
	sdl->sdl_type = type;
	memset(&sdl->sdl_data[0], 0, namelen + addrlen);
	if (name != NULL) {
		memcpy(&sdl->sdl_data[0], name, namelen);
		sdl->sdl_nlen = namelen;
	} else
		sdl->sdl_nlen = 0;
	if (addr != NULL) {
		memcpy(&sdl->sdl_data[sdl->sdl_nlen], addr, addrlen);
		sdl->sdl_alen = addrlen;
	} else
		sdl->sdl_alen = 0;
	return sdl;
}

static int
sockaddr_dl_cmp(const struct sockaddr *sa1, const struct sockaddr *sa2)
{
	int rc;
	const uint_fast8_t indexofs = offsetof(struct sockaddr_dl, sdl_index);
	const uint_fast8_t nlenofs = offsetof(struct sockaddr_dl, sdl_nlen);
	uint_fast8_t dataofs = offsetof(struct sockaddr_dl, sdl_data[0]);
	const struct sockaddr_dl *sdl1, *sdl2;

	sdl1 = satocsdl(sa1);
	sdl2 = satocsdl(sa2);

	rc = submemcmp(sdl1, sdl2, sdl1->sdl_len, sdl2->sdl_len,
	    indexofs, nlenofs);

	if (rc != 0)
		return rc;

	rc = submemcmp(sdl1, sdl2, sdl1->sdl_len, sdl2->sdl_len,
	    dataofs, dataofs + MIN(sdl1->sdl_nlen, sdl2->sdl_nlen));

	if (rc != 0)
		return rc;

	if (sdl1->sdl_nlen != sdl2->sdl_nlen)
		return sdl1->sdl_nlen - sdl2->sdl_nlen;

	dataofs += sdl1->sdl_nlen;

	rc = submemcmp(sdl1, sdl2, sdl1->sdl_len, sdl2->sdl_len,
	    dataofs, dataofs + MIN(sdl1->sdl_alen, sdl2->sdl_alen));

	if (rc != 0)
		return rc;

	if (sdl1->sdl_alen != sdl2->sdl_alen)
		return sdl1->sdl_alen - sdl2->sdl_alen;

	dataofs += sdl1->sdl_alen;

	rc = submemcmp(sdl1, sdl2, sdl1->sdl_len, sdl2->sdl_len,
	    dataofs, dataofs + MIN(sdl1->sdl_slen, sdl2->sdl_slen));

	if (sdl1->sdl_slen != sdl2->sdl_slen)
		return sdl1->sdl_slen - sdl2->sdl_slen;

	return sdl1->sdl_len - sdl2->sdl_len;
}

struct sockaddr_dl *
sockaddr_dl_setaddr(struct sockaddr_dl *sdl, socklen_t socklen,
    const void *addr, uint8_t addrlen)
{
	socklen_t len;

	len = sockaddr_dl_measure(sdl->sdl_nlen, addrlen);
	if (len > socklen) {
#ifdef DIAGNOSTIC
		printf("%s: too long: %" PRIu8 " > %" PRIu8 "\n", __func__, len,
		    socklen);
#endif
		return NULL;
	}
	memcpy(&sdl->sdl_data[sdl->sdl_nlen], addr, addrlen);
	sdl->sdl_alen = addrlen;
	sdl->sdl_len = len;
	return sdl;
}
