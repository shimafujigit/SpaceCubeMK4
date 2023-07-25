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

/*	$NetBSD: pfil.h,v 1.29 2008/05/29 14:51:27 mrg Exp $	*/

/*
 * Copyright (c) 1996 Matthew R. Green
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NET_PFIL_H_
#define _NET_PFIL_H_

#ifdef _KERNEL_OPT
#ifndef T2EX
#include "opt_pfil_hooks.h"
#endif
#endif

#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <net/dlt.h>
#include <sys/null.h>

struct mbuf;
struct ifnet;

/*
 * The packet filter hooks are designed for anything to call them to
 * possibly intercept the packet.
 */
struct packet_filter_hook {
        TAILQ_ENTRY(packet_filter_hook) pfil_link;
	int	(*pfil_func)(void *, struct mbuf **, struct ifnet *, int);
	void	*pfil_arg;
	int	pfil_flags;
};

#define PFIL_IN		0x00000001
#define PFIL_OUT	0x00000002
#define PFIL_ALL	(PFIL_IN|PFIL_OUT)
#define PFIL_WAITOK	0x00000004
#define PFIL_IFADDR	0x00000008
#define PFIL_IFNET	0x00000010

/* events notified by PFIL_IFNET */
#define	PFIL_IFNET_ATTACH	0
#define	PFIL_IFNET_DETACH	1

typedef	TAILQ_HEAD(pfil_list, packet_filter_hook) pfil_list_t;

#define	PFIL_TYPE_AF		1	/* key is AF_* type */
#define	PFIL_TYPE_IFNET		2	/* key is ifnet pointer */

struct pfil_head {
	pfil_list_t	ph_in;
	pfil_list_t	ph_out;
	pfil_list_t	ph_ifaddr;
	pfil_list_t	ph_ifnetevent; /* XXX naming collision */
	int		ph_type;
	union {
		u_long		phu_val;
		void		*phu_ptr;
	} ph_un;
#define	ph_af		ph_un.phu_val
#define	ph_ifnet	ph_un.phu_ptr
	LIST_ENTRY(pfil_head) ph_list;
};
typedef struct pfil_head pfil_head_t;

#ifdef _KERNEL

int	pfil_run_hooks(struct pfil_head *, struct mbuf **, struct ifnet *,
	    int);

int	pfil_add_hook(int (*func)(void *, struct mbuf **,
	    struct ifnet *, int), void *, int, struct pfil_head *);
int	pfil_remove_hook(int (*func)(void *, struct mbuf **,
	    struct ifnet *, int), void *, int, struct pfil_head *);

int	pfil_head_register(struct pfil_head *);
int	pfil_head_unregister(struct pfil_head *);

struct pfil_head *pfil_head_get(int, u_long);

static __inline struct packet_filter_hook *
pfil_hook_get(int dir, struct pfil_head *ph)
{

	if (dir == PFIL_IN)
		return (TAILQ_FIRST(&ph->ph_in));
	else if (dir == PFIL_OUT)
		return (TAILQ_FIRST(&ph->ph_out));
	else if (dir == PFIL_IFADDR)
		return (TAILQ_FIRST(&ph->ph_ifaddr));
	else if (dir == PFIL_IFNET)
		return (TAILQ_FIRST(&ph->ph_ifnetevent));
	else
		return (NULL);
}

#endif /* _KERNEL */

/* XXX */
#if defined(_KERNEL_OPT)
#include "ipfilter.h"
#endif

#if NIPFILTER > 0
#ifdef PFIL_HOOKS
#undef PFIL_HOOKS
#endif
#define PFIL_HOOKS
#endif /* NIPFILTER */

#ifdef _KERNEL
/* in sys/net/if.c */
extern struct pfil_head if_pfil; /* packet filtering hook for interfaces */
#endif /* _KERNEL */

#endif /* !_NET_PFIL_H_ */
