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

/*	$NetBSD: domain.h,v 1.27 2007/09/19 04:33:45 dyoung Exp $	*/

/*
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
 *	@(#)domain.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _SYS_DOMAIN_H_
#define _SYS_DOMAIN_H_

/*
 * Structure per communications domain.
 */
#include <sys/mbuf.h>
#include <sys/socket.h>

/*
 * Forward structure declarations for function prototypes [sic].
 */
struct	lwp;
struct	mbuf;
struct	ifnet;
struct	ifqueue;
struct  route;
struct  sockaddr;

LIST_HEAD(dom_rtlist, route);

struct	domain {
	int	dom_family;		/* AF_xxx */
	const char *dom_name;
#ifndef T2EX
	void	(*dom_init)		/* initialize domain data structures */
			(void);
#else
	int	(*dom_init)		/* initialize domain data structures */
			(void);
	void	(*dom_finish)		/* finalize domain data structures */
			(void);
#endif
	int	(*dom_externalize)	/* externalize access rights */
			(struct mbuf *, struct lwp *);
	void	(*dom_dispose)		/* dispose of internalized rights */
			(struct mbuf *);
	const struct protosw *dom_protosw, *dom_protoswNPROTOSW;
	int	(*dom_rtattach)		/* initialize routing table */
			(void **, int);
#ifdef T2EX
	void	(*dom_rtdetach)		/* finalize routing table */
			(void **);
#endif
	int	dom_rtoffset;		/* an arg to rtattach, in bits */
	int	dom_maxrtkey;		/* for routing layer */
	void	*(*dom_ifattach)	/* attach af-dependent data on ifnet */
			(struct ifnet *);
	void	(*dom_ifdetach)		/* detach af-dependent data on ifnet */
			(struct ifnet *, void *);
	const void *(*dom_sockaddr_const_addr)(const struct sockaddr *,
					       socklen_t *);
	void	*(*dom_sockaddr_addr)(struct sockaddr *, socklen_t *);
	int	(*dom_sockaddr_cmp)(const struct sockaddr *,
	                            const struct sockaddr *);
	const struct sockaddr *dom_sa_any;
	struct ifqueue *dom_ifqueues[2]; /* ifqueue for domain */
	STAILQ_ENTRY(domain) dom_link;
	struct	mowner dom_mowner;
	uint_fast8_t	dom_sa_cmpofs;
	uint_fast8_t	dom_sa_cmplen;
	struct dom_rtlist dom_rtcache;
};

STAILQ_HEAD(domainhead,domain);

#ifdef _KERNEL
#ifndef T2EX
#define	DOMAIN_DEFINE(name)	\
	extern struct domain name; \
	__link_set_add_data(domains, name)
#else
#define	DOMAIN_DEFINE(name)	\
	extern struct domain name;
#endif

#define	DOMAIN_FOREACH(dom)	STAILQ_FOREACH(dom, &domains, dom_link)
extern struct domainhead domains;
#ifndef T2EX
void domain_attach(struct domain *);
void domaininit(void);
#else
int domain_attach(struct domain *);
int domain_detach(struct domain *);
int domaininit(void);
int domainfinish(void);
#endif
#endif

#endif /* !_SYS_DOMAIN_H_ */
