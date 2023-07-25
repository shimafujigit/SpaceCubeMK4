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

/*	$NetBSD: ip_encap.h,v 1.12 2008/04/24 11:38:37 ad Exp $	*/
/*	$KAME: ip_encap.h,v 1.7 2000/03/25 07:23:37 sumikawa Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
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
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NETINET_IP_ENCAP_H_
#define _NETINET_IP_ENCAP_H_

#ifdef _KERNEL

#ifndef RNF_NORMAL
#include <net/radix.h>
#endif

struct encaptab {
	struct radix_node nodes[2];
	LIST_ENTRY(encaptab) chain;
	int af;
	int proto;			/* -1: don't care, I'll check myself */
	struct sockaddr *addrpack;	/* malloc'ed, for radix lookup */
	struct sockaddr *maskpack;	/* ditto */
	struct sockaddr *src;		/* my addr */
	struct sockaddr *srcmask;
	struct sockaddr *dst;		/* remote addr */
	struct sockaddr *dstmask;
	int (*func) (struct mbuf *, int, int, void *);
	const struct protosw *psw;	/* only pr_input will be used */
	void *arg;			/* passed via PACKET_TAG_ENCAP */
};

void	encap_setkeylen(void);
#ifndef T2EX
void	encap_init(void);
#else
int	encap_init(void);
void	encap_finish(void);
#endif
void	encap4_input(struct mbuf *, ...);
int	encap6_input(struct mbuf **, int *, int);
const struct encaptab *encap_attach(int, int, const struct sockaddr *,
	const struct sockaddr *, const struct sockaddr *,
	const struct sockaddr *, const struct protosw *, void *);
const struct encaptab *encap_attach_func(int, int,
	int (*)(struct mbuf *, int, int, void *),
	const struct protosw *, void *);
void	*encap6_ctlinput(int, const struct sockaddr *, void *);
int	encap_detach(const struct encaptab *);
void	*encap_getarg(struct mbuf *);
#endif

#endif /* !_NETINET_IP_ENCAP_H_ */
