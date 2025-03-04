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

/*	$NetBSD: ip_var.h,v 1.90 2008/10/12 11:15:54 plunky Exp $	*/

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
 *	@(#)ip_var.h	8.2 (Berkeley) 1/9/95
 */

#ifndef _NETINET_IP_VAR_H_
#define _NETINET_IP_VAR_H_

#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <net/route.h>

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
	u_int8_t  ih_x1[9];		/* (unused) */
	u_int8_t  ih_pr;		/* protocol */
	u_int16_t ih_len;		/* protocol length */
	struct	  in_addr ih_src;	/* source internet address */
	struct	  in_addr ih_dst;	/* destination internet address */
} __packed;

/*
 * Ip (reassembly or sequence) queue structures.
 *
 * XXX -- The following explains why the ipqe_m field is here, for TCP's use:
 * We want to avoid doing m_pullup on incoming packets but that
 * means avoiding dtom on the tcp reassembly code.  That in turn means
 * keeping an mbuf pointer in the reassembly queue (since we might
 * have a cluster).  As a quick hack, the source & destination
 * port numbers (which are no longer needed once we've located the
 * tcpcb) are overlayed with an mbuf pointer.
 */
TAILQ_HEAD(ipqehead, ipqent);
struct ipqent {
	TAILQ_ENTRY(ipqent) ipqe_q;
	union {
		struct ip	*_ip;
		struct tcpiphdr *_tcp;
	} _ipqe_u1;
	struct mbuf	*ipqe_m;	/* point to first mbuf */
	struct mbuf	*ipre_mlast;	/* point to last mbuf */
	u_int8_t	ipqe_mff;	/* for IP fragmentation */
	/*
	 * The following are used in TCP reassembly
	 */
	TAILQ_ENTRY(ipqent) ipqe_timeq;
	u_int32_t ipqe_seq;
	u_int32_t ipqe_len;
	u_int32_t ipqe_flags;
};
#define	ipqe_ip		_ipqe_u1._ip
#define	ipqe_tcp	_ipqe_u1._tcp

/*
 * Ip reassembly queue structure.  Each fragment
 * being reassembled is attached to one of these structures.
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
struct ipq {
	LIST_ENTRY(ipq) ipq_q;		/* to other reass headers */
	u_int8_t  ipq_ttl;		/* time for reass q to live */
	u_int8_t  ipq_p;		/* protocol of this fragment */
	u_int16_t ipq_id;		/* sequence id for reassembly */
	struct	  ipqehead ipq_fragq;	/* to ip fragment queue */
	struct	  in_addr ipq_src, ipq_dst;
	u_int16_t ipq_nfrags;		/* frags in this queue entry */
	u_int8_t  ipq_tos;		/* TOS of this fragment */
};

/*
 * Structure stored in mbuf in inpcb.ip_options
 * and passed to ip_output when ip options are in use.
 * The actual length of the options (including ipopt_dst)
 * is in m_len.
 */
#define	MAX_IPOPTLEN	40

struct ipoption {
	struct	in_addr ipopt_dst;	/* first-hop dst if source routed */
	int8_t	ipopt_list[MAX_IPOPTLEN];	/* options proper */
};

/*
 * Structure attached to inpcb.ip_moptions and
 * passed to ip_output when IP multicast options are in use.
 */
struct ip_moptions {
	struct	  ifnet *imo_multicast_ifp; /* ifp for outgoing multicasts */
	struct in_addr imo_multicast_addr; /* ifindex/addr on MULTICAST_IF */
	u_int8_t  imo_multicast_ttl;	/* TTL for outgoing multicasts */
	u_int8_t  imo_multicast_loop;	/* 1 => hear sends if a member */
	u_int16_t imo_num_memberships;	/* no. memberships this socket */
	struct	  in_multi *imo_membership[IP_MAX_MEMBERSHIPS];
};

/*
 * IP statistics.
 * Each counter is an unsigned 64-bit value.
 */
#define	IP_STAT_TOTAL		0	/* total packets received */
#define	IP_STAT_BADSUM		1	/* checksum bad */
#define	IP_STAT_TOOSHORT	2	/* packet too short */
#define	IP_STAT_TOOSMALL	3	/* not enough data */
#define	IP_STAT_BADHLEN		4	/* ip header length < data size */
#define	IP_STAT_BADLEN		5	/* ip length < ip header length */
#define	IP_STAT_FRAGMENTS	6	/* fragments received */
#define	IP_STAT_FRAGDROPPED	7	/* frags dropped (dups, out of space) */
#define	IP_STAT_FRAGTIMEOUT	8	/* fragments timed out */
#define	IP_STAT_FORWARD		9	/* packets forwarded */
#define	IP_STAT_FASTFORWARD	10	/* packets fast forwarded */
#define	IP_STAT_CANTFORWARD	11	/* packets rcvd for unreachable dest */
#define	IP_STAT_REDIRECTSENT	12	/* packets forwareded on same net */
#define	IP_STAT_NOPROTO		13	/* unknown or unsupported protocol */
#define	IP_STAT_DELIVERED	14	/* datagrams delivered to upper level */
#define	IP_STAT_LOCALOUT	15	/* total ip packets generated here */
#define	IP_STAT_ODROPPED	16	/* lost packets due to nobufs, etc. */
#define	IP_STAT_REASSEMBLED	17	/* total packets reassembled ok */
#define	IP_STAT_FRAGMENTED	18	/* datagrams successfully fragmented */
#define	IP_STAT_OFRAGMENTS	19	/* output fragments created */
#define	IP_STAT_CANTFRAG	20	/* don't fragment flag was set, etc. */
#define	IP_STAT_BADOPTIONS	21	/* error in option processing */
#define	IP_STAT_NOROUTE		22	/* packets discarded due to no route */
#define	IP_STAT_BADVERS		23	/* ip version != 4 */
#define	IP_STAT_RAWOUT		24	/* total raw ip packets generated */
#define	IP_STAT_BADFRAGS	25	/* malformed fragments (bad length) */
#define	IP_STAT_RCVMEMDROP	26	/* frags dropped for lack of memory */
#define	IP_STAT_TOOLONG		27	/* ip length > max ip packet size */
#define	IP_STAT_NOGIF		28	/* no match gif found */
#define	IP_STAT_BADADDR		29	/* invalid address on header */

#define	IP_NSTATS		30

#ifdef _KERNEL

#ifdef _KERNEL_OPT
#include "opt_gateway.h"
#include "opt_mbuftrace.h"
#endif

/* flags passed to ip_output as last parameter */
#define	IP_FORWARDING		0x1		/* most of ip header exists */
#define	IP_RAWOUTPUT		0x2		/* raw ip header exists */
#define	IP_RETURNMTU		0x4		/* pass back mtu on EMSGSIZE */
#define	IP_NOIPNEWID		0x8		/* don't fill in ip_id */
#define	IP_ROUTETOIF		SO_DONTROUTE	/* bypass routing tables */
#define	IP_ALLOWBROADCAST	SO_BROADCAST	/* can send broadcast packets */
#define	IP_MTUDISC		0x0400		/* Path MTU Discovery; set DF */

extern struct domain inetdomain;

extern LIST_HEAD(ipqhead, ipq) ipq[];	/* ip reass. queue */
extern int   ip_defttl;			/* default IP ttl */
extern int   ipforwarding;		/* ip forwarding */
extern int   ip_mtudisc;		/* mtu discovery */
extern int   ip_mtudisc_timeout;	/* seconds to timeout mtu discovery */
extern int   anonportmin;		/* minimum ephemeral port */
extern int   anonportmax;		/* maximum ephemeral port */
extern int   lowportmin;		/* minimum reserved port */
extern int   lowportmax;		/* maximum reserved port */
extern int   ip_do_loopback_cksum;	/* do IP checksum on loopback? */
extern struct rttimer_queue *ip_mtudisc_timeout_q;
#ifdef MBUFTRACE
extern struct mowner ip_rx_mowner;
extern struct mowner ip_tx_mowner;
#endif
#ifdef GATEWAY
extern int ip_maxflows;
extern int ip_hashsize;
#endif
extern struct pool inmulti_pool;
extern struct pool ipqent_pool;
struct	 inpcb;
struct   sockopt;

int	 ip_ctloutput(int, struct socket *, struct sockopt *);
int	 ip_dooptions(struct mbuf *);
void	 ip_drain(void);
void	 ip_forward(struct mbuf *, int);
void	 ip_freef(struct ipq *);
void	 ip_freemoptions(struct ip_moptions *);
int	 ip_getmoptions(struct ip_moptions *, struct sockopt *);
#ifndef T2EX
void	 ip_init(void);
#else
int	 ip_init(void);
void	 ip_finish(void);
#endif
int	 ip_optcopy(struct ip *, struct ip *);
u_int	 ip_optlen(struct inpcb *);
int	 ip_output(struct mbuf *, ...);
int	 ip_fragment(struct mbuf *, struct ifnet *, u_long);
int	 ip_pcbopts(struct mbuf **, const struct sockopt *);
struct mbuf *
	 ip_reass(struct ipqent *, struct ipq *, struct ipqhead *);
struct in_ifaddr *
	 ip_rtaddr(struct in_addr);
void	 ip_savecontrol(struct inpcb *, struct mbuf **, struct ip *,
	   struct mbuf *);
int	 ip_setmoptions(struct ip_moptions **, const struct sockopt *);
void	 ip_slowtimo(void);
struct mbuf *
	 ip_srcroute(void);
int	 ip_sysctl(int *, u_int, void *, size_t *, void *, size_t);
void	 ip_statinc(u_int);
void	 ipintr(void);
void *	 rip_ctlinput(int, const struct sockaddr *, void *);
int	 rip_ctloutput(int, struct socket *, struct sockopt *);
#ifndef T2EX
void	 rip_init(void);
#else
int	 rip_init(void);
void	 rip_finish(void);
#endif
void	 rip_input(struct mbuf *, ...);
int	 rip_output(struct mbuf *, ...);
int	 rip_usrreq(struct socket *,
	    int, struct mbuf *, struct mbuf *, struct mbuf *, struct lwp *);
int	ipflow_init(int);
#ifdef T2EX
void	ipflow_finish(void);
#endif
void	ipflow_prune(void);
void	ipflow_create(const struct route *, struct mbuf *);
void	ipflow_slowtimo(void);
int	ipflow_invalidate_all(int);

#endif  /* _KERNEL */

#endif /* !_NETINET_IP_VAR_H_ */
