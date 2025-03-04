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

/* $NetBSD: netisr.h,v 1.38 2008/10/14 17:15:20 pooka Exp $ */

/*
 * Copyright (c) 1980, 1986, 1989, 1993
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
 *	@(#)netisr.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NET_NETISR_H_
#define _NET_NETISR_H_		/* checked by netisr_dispatch.h */

/*
 * The networking code runs off software interrupts.
 *
 * You can switch into the network by doing splsoftnet() and return by splx().
 * The software interrupt level for the network is higher than the software
 * level for the clock (so you can enter the network in routines called
 * at timeout time).
 *
 * The routine to request a network software interrupt, setsoftnet(),
 * is defined in the machine-specific include files.
 */

#if defined(_KERNEL)

#if !defined(_LKM)
#include "opt_inet.h"
#ifndef T2EX
#include "opt_atalk.h"
#include "opt_iso.h"
#include "opt_natm.h"
#endif
#include "arp.h"
#endif /* !defined(_LKM) */

#if !defined(_LOCORE)

/* XXX struct sockaddr defn for for if.h, if_arp.h */
#include <sys/socket.h>

/*
 * XXX IFNAMSIZE for if_ppp.h, natm.h; struct ifnet decl for in6.h, in.h;
 * XXX struct mbuf decl for in6.h, in.h, route.h (via in_var.h).
 */
#include <net/if.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/ip_var.h>
#if NARP > 0
#include <netinet/if_inarp.h>
#endif
#endif
#ifdef INET6
# ifndef INET
#  include <netinet/in.h>
# endif
#include <netinet/ip6.h>
#include <netinet6/ip6_var.h>
#endif
#ifdef ISO
#include <netiso/iso.h>
#include <netiso/clnp.h>
#endif
#ifdef NATM
#include <netnatm/natm.h>
#endif
#ifdef NETATALK
#include <netatalk/at_extern.h>
#endif

#endif /* !defined(_LOCORE) */
#endif /* defined(_KERNEL) */


/*
 * Each ``pup-level-1'' input queue has a bit in a ``netisr'' status
 * word which is used to de-multiplex a single software
 * interrupt used for scheduling the network code to calls
 * on the lowest level routine of each protocol.
 */
#define	NETISR_IP	2		/* same as AF_INET */
#define	NETISR_NS	6		/* same as AF_NS */
#define	NETISR_ISO	7		/* same as AF_ISO */
#define	NETISR_CCITT	10		/* same as AF_CCITT */
#define	NETISR_ATALK	16		/* same as AF_APPLETALK */
#define	NETISR_IPX	23		/* same as AF_IPX */
#define	NETISR_IPV6	24		/* same as AF_INET6 */
#define	NETISR_ISDN	26		/* same as AF_E164 */
#define	NETISR_NATM	27		/* same as AF_NATM */
#define	NETISR_ARP	28		/* same as AF_ARP */
#define	NETISR_MAX	AF_MAX

#if !defined(_LOCORE) && defined(_KERNEL)
/* XXX Legacy netisr support. */
void	schednetisr(int);
#endif

#endif /* !_NET_NETISR_H_ */
