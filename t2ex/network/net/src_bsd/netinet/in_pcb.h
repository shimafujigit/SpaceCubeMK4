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

/*	$NetBSD: in_pcb.h,v 1.45 2007/12/16 18:39:57 elad Exp $	*/

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
 * Copyright (c) 1982, 1986, 1990, 1993
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
 *	@(#)in_pcb.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_IN_PCB_H_
#define _NETINET_IN_PCB_H_

#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <netinet/in_pcb_hdr.h>

/*
 * Common structure pcb for internet protocol implementation.
 * Here are stored pointers to local and foreign host table
 * entries, local and foreign socket numbers, and pointers
 * up (to a socket structure) and down (to a protocol-specific)
 * control block.
 */
struct inpcb {
	struct inpcb_hdr inp_head;
#define inp_hash	inp_head.inph_hash
#define inp_queue	inp_head.inph_queue
#define inp_af		inp_head.inph_af
#define inp_ppcb	inp_head.inph_ppcb
#define inp_state	inp_head.inph_state
#define inp_socket	inp_head.inph_socket
#define inp_table	inp_head.inph_table
#define inp_sp		inp_head.inph_sp
	struct	  route inp_route;	/* placeholder for routing entry */
	u_int16_t inp_fport;		/* foreign port */
	u_int16_t inp_lport;		/* local port */
	int	  inp_flags;		/* generic IP/datagram flags */
	struct	  ip inp_ip;		/* header prototype; should have more */
	struct	  mbuf *inp_options;	/* IP options */
	struct	  ip_moptions *inp_moptions; /* IP multicast options */
	int	  inp_errormtu;		/* MTU of last xmit status = EMSGSIZE */
};

#define	inp_faddr	inp_ip.ip_dst
#define	inp_laddr	inp_ip.ip_src

/* flags in inp_flags: */
#define	INP_RECVOPTS		0x01	/* receive incoming IP options */
#define	INP_RECVRETOPTS		0x02	/* receive IP options for reply */
#define	INP_RECVDSTADDR		0x04	/* receive IP dst address */
#define	INP_HDRINCL		0x08	/* user supplies entire IP header */
#define	INP_HIGHPORT		0x10	/* (unused; FreeBSD compat) */
#define	INP_LOWPORT		0x20	/* user wants "low" port binding */
#define	INP_ANONPORT		0x40	/* port chosen for user */
#define	INP_RECVIF		0x80	/* receive incoming interface */
/* XXX should move to an UDP control block */
#define INP_ESPINUDP		0x100	/* ESP over UDP for NAT-T */
#define INP_ESPINUDP_NON_IKE	0x200	/* ESP over UDP for NAT-T */
#define	INP_CONTROLOPTS		(INP_RECVOPTS|INP_RECVRETOPTS|INP_RECVDSTADDR|\
				INP_RECVIF)
#define INP_ESPINUDP_ALL	(INP_ESPINUDP|INP_ESPINUDP_NON_IKE)
#define INP_NOHEADER		0x400	/* Kernel removes IP header
					 * before feeding a packet
					 * to the raw socket user.
					 * The socket user will
					 * not supply an IP header.
					 * Cancels INP_HDRINCL.
					 */

#define	sotoinpcb(so)		((struct inpcb *)(so)->so_pcb)

#ifdef _KERNEL
void	in_losing(struct inpcb *);
int	in_pcballoc(struct socket *, void *);
int	in_pcbbind(void *, struct mbuf *, struct lwp *);
int	in_pcbconnect(void *, struct mbuf *, struct lwp *);
void	in_pcbdetach(void *);
void	in_pcbdisconnect(void *);
#ifndef T2EX
void	in_pcbinit(struct inpcbtable *, int, int);
#else
int	in_pcbinit(struct inpcbtable *, int, int);
void	in_pcbfinish(struct inpcbtable *);
#endif
struct inpcb *
	in_pcblookup_port(struct inpcbtable *,
	    struct in_addr, u_int, int);
struct inpcb *
	in_pcblookup_bind(struct inpcbtable *,
	    struct in_addr, u_int);
struct inpcb *
	in_pcblookup_connect(struct inpcbtable *,
	    struct in_addr, u_int, struct in_addr, u_int);
int	in_pcbnotify(struct inpcbtable *, struct in_addr, u_int,
	    struct in_addr, u_int, int, void (*)(struct inpcb *, int));
void	in_pcbnotifyall(struct inpcbtable *, struct in_addr, int,
	    void (*)(struct inpcb *, int));
void	in_pcbpurgeif0(struct inpcbtable *, struct ifnet *);
void	in_pcbpurgeif(struct inpcbtable *, struct ifnet *);
void	in_pcbstate(struct inpcb *, int);
void	in_rtchange(struct inpcb *, int);
void	in_setpeeraddr(struct inpcb *, struct mbuf *);
void	in_setsockaddr(struct inpcb *, struct mbuf *);
struct rtentry *
	in_pcbrtentry(struct inpcb *);
extern struct sockaddr_in *in_selectsrc(struct sockaddr_in *,
	struct route *, int, struct ip_moptions *, int *);
#endif

#endif /* !_NETINET_IN_PCB_H_ */
