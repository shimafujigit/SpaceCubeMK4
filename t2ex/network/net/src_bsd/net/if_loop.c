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

/*	$NetBSD: if_loop.c,v 1.69 2008/10/24 17:07:33 dyoung Exp $	*/

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
 *	@(#)if_loop.c	8.2 (Berkeley) 1/9/95
 */

/*
 * Loopback interface driver for protocol testing and timing.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: if_loop.c,v 1.69 2008/10/24 17:07:33 dyoung Exp $");

#include "opt_inet.h"
#ifndef T2EX
#include "opt_atalk.h"
#include "opt_iso.h"
#include "opt_ipx.h"
#endif
#include "opt_mbuftrace.h"

#include "bpfilter.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#ifndef T2EX
#include <sys/cpu.h>
#endif

#include <net/if.h>
#include <net/if_types.h>
#include <net/netisr.h>
#include <net/route.h>

#ifdef	INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#endif

#ifdef INET6
#ifndef INET
#include <netinet/in.h>
#endif
#include <netinet6/in6_var.h>
#include <netinet/ip6.h>
#endif


#ifdef IPX
#include <netipx/ipx.h>
#include <netipx/ipx_if.h>
#endif

#ifdef ISO
#include <netiso/iso.h>
#include <netiso/iso_var.h>
#endif

#ifdef NETATALK
#include <netatalk/at.h>
#include <netatalk/at_var.h>
#endif

#if NBPFILTER > 0
#include <net/bpf.h>
#endif

#if defined(LARGE_LOMTU)
#define LOMTU	(131072 +  MHLEN + MLEN)
#define LOMTU_MAX LOMTU
#else
#define	LOMTU	(32768 +  MHLEN + MLEN)
#define	LOMTU_MAX	(65536 +  MHLEN + MLEN)
#endif

#ifdef ALTQ
static void	lostart(struct ifnet *);
#endif

static int	loop_clone_create(struct if_clone *, int);
static int	loop_clone_destroy(struct ifnet *);
#ifdef T2EX
static int loop_clone_destroy_lo0(void);
#endif

static struct if_clone loop_cloner =
    IF_CLONE_INITIALIZER("lo", loop_clone_create, loop_clone_destroy);

#ifndef T2EX
void
loopattach(int n)
{

	(void)loop_clone_create(&loop_cloner, 0);	/* lo0 always exists */
	if_clone_attach(&loop_cloner);
}
#else
int
loopattach(int n)
{
	int error;

	error = loop_clone_create(&loop_cloner, 0);	/* lo0 always exists */
	if ( error != 0 ) {
		return error;
	}
	if_clone_attach(&loop_cloner);

	return 0;
}

int
loopdetach(int n)
{

	if_clone_detach(&loop_cloner);
	return loop_clone_destroy_lo0();
}
#endif

static int
loop_clone_create(struct if_clone *ifc, int unit)
{
	struct ifnet *ifp;
#ifdef T2EX
	int error;
#endif

	ifp = if_alloc(IFT_LOOP);
#ifdef T2EX
	if ( ifp == NULL ) {
		error = ENOMEM;
		goto err_ret0;
	}
#endif

	if_initname(ifp, ifc->ifc_name, unit);

	ifp->if_mtu = LOMTU;
	ifp->if_flags = IFF_LOOPBACK | IFF_MULTICAST | IFF_RUNNING;
	ifp->if_ioctl = loioctl;
	ifp->if_output = looutput;
#ifdef ALTQ
	ifp->if_start = lostart;
#endif
	ifp->if_type = IFT_LOOP;
	ifp->if_hdrlen = 0;
	ifp->if_addrlen = 0;
	ifp->if_dlt = DLT_NULL;
	IFQ_SET_READY(&ifp->if_snd);
	if (unit == 0)
		lo0ifp = ifp;
#ifndef T2EX
	if_attach(ifp);
	if_alloc_sadl(ifp);
#else
	error = if_attach(ifp);
	if ( error != 0 ) {
		goto err_ret1;
	}
	error = if_alloc_sadl(ifp);
	if ( error != 0 ) {
		goto err_ret2;
	}
#endif
#if NBPFILTER > 0
#ifndef T2EX
	bpfattach(ifp, DLT_NULL, sizeof(u_int));
#else
	error = bpfattach(ifp, DLT_NULL, sizeof(u_int));
	if ( error != 0 ) {
		goto err_ret3;
	}
#endif
#endif
#ifdef MBUFTRACE
	ifp->if_mowner = malloc(sizeof(struct mowner), M_DEVBUF,
	    M_WAITOK | M_ZERO);
#ifdef T2EX
	if ( ifp->if_mowner == NULL ) {
		error = ENOMEM;
		goto err_ret4;
	}
#endif
	strlcpy(ifp->if_mowner->mo_name, ifp->if_xname,
	    sizeof(ifp->if_mowner->mo_name));
#ifndef T2EX
	MOWNER_ATTACH(ifp->if_mowner);
#else
	error = MOWNER_ATTACH(ifp->if_mowner);
	if ( error != 0 ) {
		goto err_ret5;
	}
#endif
#endif

	return (0);
#ifdef T2EX
#ifdef MBUFTRACE
err_ret5:
	free(ifp->if_mowner, M_DEVBUF);
err_ret4:
	bpfdetach(ifp);
#endif
#if NBPFILTER > 0
err_ret3:
	if_free_sadl(ifp);
#endif
err_ret2:
	if_detach(ifp);
err_ret1:
	if_free(ifp);
err_ret0:
	return error;
#endif
}

static int
loop_clone_destroy(struct ifnet *ifp)
{

	if (ifp == lo0ifp)
		return (EPERM);

#ifdef MBUFTRACE
#ifdef T2EX
	if ( ifp->if_mowner != NULL ) {
#endif
	MOWNER_DETACH(ifp->if_mowner);
	free(ifp->if_mowner, M_DEVBUF);
#ifdef T2EX
	}
#endif
#endif

#if NBPFILTER > 0
	bpfdetach(ifp);
#endif
	if_detach(ifp);

	free(ifp, M_DEVBUF);

	return (0);
}

#ifdef T2EX
static int
loop_clone_destroy_lo0(void)
{
	struct ifnet *ifp = lo0ifp;

#ifdef MBUFTRACE
	if ( ifp->if_mowner != NULL ) {
		MOWNER_DETACH(ifp->if_mowner);
		free(ifp->if_mowner, M_DEVBUF);
	}
#endif

#if NBPFILTER > 0
	bpfdetach(ifp);
#endif
	if_detach(ifp);

	free(ifp, M_DEVBUF);

	return (0);
}
#endif

int
looutput(struct ifnet *ifp, struct mbuf *m, const struct sockaddr *dst,
    struct rtentry *rt)
{
	int s, isr;
	struct ifqueue *ifq = NULL;

	MCLAIM(m, ifp->if_mowner);
	if ((m->m_flags & M_PKTHDR) == 0)
		panic("looutput: no header mbuf");
#if NBPFILTER > 0
	if (ifp->if_bpf && (ifp->if_flags & IFF_LOOPBACK))
		bpf_mtap_af(ifp->if_bpf, dst->sa_family, m);
#endif
	m->m_pkthdr.rcvif = ifp;

	if (rt && rt->rt_flags & (RTF_REJECT|RTF_BLACKHOLE)) {
		m_freem(m);
		return (rt->rt_flags & RTF_BLACKHOLE ? 0 :
			rt->rt_flags & RTF_HOST ? EHOSTUNREACH : ENETUNREACH);
	}

	ifp->if_opackets++;
	ifp->if_obytes += m->m_pkthdr.len;

#ifdef ALTQ
	/*
	 * ALTQ on the loopback interface is just for debugging.  It's
	 * used only for loopback interfaces, not for a simplex interface.
	 */
	if ((ALTQ_IS_ENABLED(&ifp->if_snd) || TBR_IS_ENABLED(&ifp->if_snd)) &&
	    ifp->if_start == lostart) {
		struct altq_pktattr pktattr;
		int error;

		/*
		 * If the queueing discipline needs packet classification,
		 * do it before prepending the link headers.
		 */
		IFQ_CLASSIFY(&ifp->if_snd, m, dst->sa_family, &pktattr);

		M_PREPEND(m, sizeof(uint32_t), M_DONTWAIT);
		if (m == NULL)
			return (ENOBUFS);
		*(mtod(m, uint32_t *)) = dst->sa_family;

		s = splnet();
		IFQ_ENQUEUE(&ifp->if_snd, m, &pktattr, error);
		(*ifp->if_start)(ifp);
		splx(s);
		return (error);
	}
#endif /* ALTQ */

	m_tag_delete_nonpersistent(m);

	switch (dst->sa_family) {

#ifdef INET
	case AF_INET:
		ifq = &ipintrq;
		isr = NETISR_IP;
		break;
#endif
#ifdef INET6
	case AF_INET6:
		m->m_flags |= M_LOOP;
		ifq = &ip6intrq;
		isr = NETISR_IPV6;
		break;
#endif
#ifdef ISO
	case AF_ISO:
		ifq = &clnlintrq;
		isr = NETISR_ISO;
		break;
#endif
#ifdef IPX
	case AF_IPX:
		ifq = &ipxintrq;
		isr = NETISR_IPX;
		break;
#endif
#ifdef NETATALK
	case AF_APPLETALK:
	        ifq = &atintrq2;
		isr = NETISR_ATALK;
		break;
#endif
	default:
		printf("%s: can't handle af%d\n", ifp->if_xname,
		    dst->sa_family);
		m_freem(m);
		return (EAFNOSUPPORT);
	}
	s = splnet();
	if (IF_QFULL(ifq)) {
		IF_DROP(ifq);
		m_freem(m);
		splx(s);
		return (ENOBUFS);
	}
	IF_ENQUEUE(ifq, m);
	schednetisr(isr);
	ifp->if_ipackets++;
	ifp->if_ibytes += m->m_pkthdr.len;
	splx(s);
	return (0);
}

#ifdef ALTQ
static void
lostart(struct ifnet *ifp)
{
	struct ifqueue *ifq;
	struct mbuf *m;
	uint32_t af;
	int s, isr;

	for (;;) {
		IFQ_DEQUEUE(&ifp->if_snd, m);
		if (m == NULL)
			return;

		af = *(mtod(m, uint32_t *));
		m_adj(m, sizeof(uint32_t));

		switch (af) {
#ifdef INET
		case AF_INET:
			ifq = &ipintrq;
			isr = NETISR_IP;
			break;
#endif
#ifdef INET6
		case AF_INET6:
			m->m_flags |= M_LOOP;
			ifq = &ip6intrq;
			isr = NETISR_IPV6;
			break;
#endif
#ifdef IPX
		case AF_IPX:
			ifq = &ipxintrq;
			isr = NETISR_IPX;
			break;
#endif
#ifdef ISO
		case AF_ISO:
			ifq = &clnlintrq;
			isr = NETISR_ISO;
			break;
#endif
#ifdef NETATALK
		case AF_APPLETALK:
			ifq = &atintrq2;
			isr = NETISR_ATALK;
			break;
#endif
		default:
			printf("%s: can't handle af%d\n", ifp->if_xname, af);
			m_freem(m);
			return;
		}

		s = splnet();
		if (IF_QFULL(ifq)) {
			IF_DROP(ifq);
			splx(s);
			m_freem(m);
			return;
		}
		IF_ENQUEUE(ifq, m);
		schednetisr(isr);
		ifp->if_ipackets++;
		ifp->if_ibytes += m->m_pkthdr.len;
		splx(s);
	}
}
#endif /* ALTQ */

/* ARGSUSED */
void
lortrequest(int cmd, struct rtentry *rt,
    const struct rt_addrinfo *info)
{

	if (rt)
		rt->rt_rmx.rmx_mtu = lo0ifp->if_mtu;
}

/*
 * Process an ioctl request.
 */
/* ARGSUSED */
int
loioctl(struct ifnet *ifp, u_long cmd, void *data)
{
	struct ifaddr *ifa;
	struct ifreq *ifr = data;
	int error = 0;

	switch (cmd) {

	case SIOCSIFADDR:
		ifp->if_flags |= IFF_UP;
		ifa = (struct ifaddr *)data;
		if (ifa != NULL /*&& ifa->ifa_addr->sa_family == AF_ISO*/)
			ifa->ifa_rtrequest = lortrequest;
		/*
		 * Everything else is done at a higher level.
		 */
		break;

	case SIOCSIFMTU:
		if ((unsigned)ifr->ifr_mtu > LOMTU_MAX)
			error = EINVAL;
		else if ((error = ifioctl_common(ifp, cmd, data)) == ENETRESET){
			/* XXX update rt mtu for AF_ISO? */
			error = 0;
		}
		break;

	case SIOCADDMULTI:
	case SIOCDELMULTI:
		if (ifr == NULL) {
			error = EAFNOSUPPORT;		/* XXX */
			break;
		}
		switch (ifreq_getaddr(cmd, ifr)->sa_family) {

#ifdef INET
		case AF_INET:
			break;
#endif
#ifdef INET6
		case AF_INET6:
			break;
#endif

		default:
			error = EAFNOSUPPORT;
			break;
		}
		break;

	default:
		error = EINVAL;
	}
	return (error);
}
