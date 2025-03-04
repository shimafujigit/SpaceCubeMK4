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

/*	$NetBSD: protosw.h,v 1.44 2008/08/06 15:01:24 plunky Exp $	*/

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
 *	@(#)protosw.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _SYS_PROTOSW_H_
#define _SYS_PROTOSW_H_

/*
 * Protocol switch table.
 *
 * Each protocol has a handle initializing one of these structures,
 * which is used for protocol-protocol and system-protocol communication.
 *
 * A protocol is called through the pr_init entry before any other.
 * Thereafter it is called every 200ms through the pr_fasttimo entry and
 * every 500ms through the pr_slowtimo for timer based actions.
 * The system will call the pr_drain entry if it is low on space and
 * this should throw away any non-critical data.
 *
 * Protocols pass data between themselves as chains of mbufs using
 * the pr_input and pr_output hooks.  Pr_input passes data up (towards
 * UNIX) and pr_output passes it down (towards the imps); control
 * information passes up and down on pr_ctlinput and pr_ctloutput.
 * The protocol is responsible for the space occupied by any the
 * arguments to these entries and must dispose it.
 *
 * The userreq routine interfaces protocols to the system and is
 * described below.
 */

struct mbuf;
struct sockaddr;
struct socket;
struct sockopt;
struct domain;
struct proc;
struct lwp;

struct protosw {
	int 	pr_type;		/* socket type used for */
	struct	domain *pr_domain;	/* domain protocol a member of */
	short	pr_protocol;		/* protocol number */
	short	pr_flags;		/* see below */

/* protocol-protocol hooks */
	void	(*pr_input)		/* input to protocol (from below) */
			(struct mbuf *, ...);
	int	(*pr_output)		/* output to protocol (from above) */
			(struct mbuf *, ...);
	void	*(*pr_ctlinput)		/* control input (from below) */
			(int, const struct sockaddr *, void *);
	int	(*pr_ctloutput)		/* control output (from above) */
			(int, struct socket *, struct sockopt *);

/* user-protocol hook */
	int	(*pr_usrreq)		/* user request: see list below */
			(struct socket *, int, struct mbuf *,
			     struct mbuf *, struct mbuf *, struct lwp *);

/* utility hooks */
#ifndef T2EX
	void	(*pr_init)		/* initialization hook */
			(void);
#else
	int	(*pr_init)		/* initialization hook */
			(void);
	void	(*pr_finish)		/* finalization hook */
			(void);
#endif

	void	(*pr_fasttimo)		/* fast timeout (200ms) */
			(void);
	void	(*pr_slowtimo)		/* slow timeout (500ms) */
			(void);
	void	(*pr_drain)		/* flush any excess space possible */
			(void);
};

#define	PR_SLOWHZ	2		/* 2 slow timeouts per second */
#define	PR_FASTHZ	5		/* 5 fast timeouts per second */

/*
 * Values for pr_flags.
 * PR_ADDR requires PR_ATOMIC;
 * PR_ADDR and PR_CONNREQUIRED are mutually exclusive.
 */
#define	PR_ATOMIC	0x01		/* exchange atomic messages only */
#define	PR_ADDR		0x02		/* addresses given with messages */
#define	PR_CONNREQUIRED	0x04		/* connection required by protocol */
#define	PR_WANTRCVD	0x08		/* want PRU_RCVD calls */
#define	PR_RIGHTS	0x10		/* passes capabilities */
#define	PR_LISTEN	0x20		/* supports listen(2) and accept(2) */
#define	PR_LASTHDR	0x40		/* enforce ipsec policy; last header */
#define	PR_ABRTACPTDIS	0x80		/* abort on accept(2) to disconnected
					   socket */
#define PR_PURGEIF	0x100		/* might store struct ifnet pointer;
					   PRU_PURGEIF must be called on ifnet
					   deletion */

/*
 * The arguments to usrreq are:
 *	(*protosw[].pr_usrreq)(up, req, m, nam, opt, p);
 * where up is a (struct socket *), req is one of these requests,
 * m is a optional mbuf chain containing a message,
 * nam is an optional mbuf chain containing an address,
 * opt is an optional mbuf containing socket options,
 * and p is a pointer to the process requesting the action (if any).
 * The protocol is responsible for disposal of the mbuf chains m and opt,
 * the caller is responsible for any space held by nam.
 * A non-zero return from usrreq gives an
 * UNIX error number which should be passed to higher level software.
 */
#define	PRU_ATTACH		0	/* attach protocol to up */
#define	PRU_DETACH		1	/* detach protocol from up */
#define	PRU_BIND		2	/* bind socket to address */
#define	PRU_LISTEN		3	/* listen for connection */
#define	PRU_CONNECT		4	/* establish connection to peer */
#define	PRU_ACCEPT		5	/* accept connection from peer */
#define	PRU_DISCONNECT		6	/* disconnect from peer */
#define	PRU_SHUTDOWN		7	/* won't send any more data */
#define	PRU_RCVD		8	/* have taken data; more room now */
#define	PRU_SEND		9	/* send this data */
#define	PRU_ABORT		10	/* abort (fast DISCONNECT, DETACH) */
#define	PRU_CONTROL		11	/* control operations on protocol */
#define	PRU_SENSE		12	/* return status into m */
#define	PRU_RCVOOB		13	/* retrieve out of band data */
#define	PRU_SENDOOB		14	/* send out of band data */
#define	PRU_SOCKADDR		15	/* fetch socket's address */
#define	PRU_PEERADDR		16	/* fetch peer's address */
#define	PRU_CONNECT2		17	/* connect two sockets */
/* begin for protocols internal use */
#define	PRU_FASTTIMO		18	/* 200ms timeout */
#define	PRU_SLOWTIMO		19	/* 500ms timeout */
#define	PRU_PROTORCV		20	/* receive from below */
#define	PRU_PROTOSEND		21	/* send to below */
#define	PRU_PURGEIF		22	/* purge specified if */

#define	PRU_NREQ		23

#ifdef PRUREQUESTS
static const char * const prurequests[] = {
	"ATTACH",	"DETACH",	"BIND",		"LISTEN",
	"CONNECT",	"ACCEPT",	"DISCONNECT",	"SHUTDOWN",
	"RCVD",		"SEND",		"ABORT",	"CONTROL",
	"SENSE",	"RCVOOB",	"SENDOOB",	"SOCKADDR",
	"PEERADDR",	"CONNECT2",	"FASTTIMO",	"SLOWTIMO",
	"PROTORCV",	"PROTOSEND",	"PURGEIF",
};
#endif

/*
 * The arguments to the ctlinput routine are
 *	(*protosw[].pr_ctlinput)(cmd, sa, arg);
 * where cmd is one of the commands below, sa is a pointer to a sockaddr,
 * and arg is an optional void *argument used within a protocol family.
 */
#define	PRC_IFDOWN		0	/* interface transition */
#define	PRC_ROUTEDEAD		1	/* select new route if possible ??? */
#define	PRC_QUENCH2		3	/* DEC congestion bit says slow down */
#define	PRC_QUENCH		4	/* some one said to slow down */
#define	PRC_MSGSIZE		5	/* message size forced drop */
#define	PRC_HOSTDEAD		6	/* host appears to be down */
#define	PRC_HOSTUNREACH		7	/* deprecated (use PRC_UNREACH_HOST) */
#define	PRC_UNREACH_NET		8	/* no route to network */
#define	PRC_UNREACH_HOST	9	/* no route to host */
#define	PRC_UNREACH_PROTOCOL	10	/* dst says bad protocol */
#define	PRC_UNREACH_PORT	11	/* bad port # */
/* was	PRC_UNREACH_NEEDFRAG	12	   (use PRC_MSGSIZE) */
#define	PRC_UNREACH_SRCFAIL	13	/* source route failed */
#define	PRC_REDIRECT_NET	14	/* net routing redirect */
#define	PRC_REDIRECT_HOST	15	/* host routing redirect */
#define	PRC_REDIRECT_TOSNET	16	/* redirect for type of service & net */
#define	PRC_REDIRECT_TOSHOST	17	/* redirect for tos & host */
#define	PRC_TIMXCEED_INTRANS	18	/* packet lifetime expired in transit */
#define	PRC_TIMXCEED_REASS	19	/* lifetime expired on reass q */
#define	PRC_PARAMPROB		20	/* header incorrect */

#define	PRC_NCMDS		21

#define	PRC_IS_REDIRECT(cmd)	\
	((cmd) >= PRC_REDIRECT_NET && (cmd) <= PRC_REDIRECT_TOSHOST)

#ifdef PRCREQUESTS
static const char * const prcrequests[] = {
	"IFDOWN", "ROUTEDEAD", "#2", "DEC-BIT-QUENCH2",
	"QUENCH", "MSGSIZE", "HOSTDEAD", "#7",
	"NET-UNREACH", "HOST-UNREACH", "PROTO-UNREACH", "PORT-UNREACH",
	"#12", "SRCFAIL-UNREACH", "NET-REDIRECT", "HOST-REDIRECT",
	"TOSNET-REDIRECT", "TOSHOST-REDIRECT", "TX-INTRANS", "TX-REASS",
	"PARAMPROB"
};
#endif

/*
 * The arguments to ctloutput are:
 *	(*protosw[].pr_ctloutput)(req, so, sopt);
 * req is one of the actions listed below, so is a (struct socket *),
 * sopt is a (struct sockopt *)
 * A non-zero return from usrreq gives an
 * UNIX error number which should be passed to higher level software.
 */
#define	PRCO_GETOPT	0
#define	PRCO_SETOPT	1

#define	PRCO_NCMDS	2

#ifdef PRCOREQUESTS
static const char * const prcorequests[] = {
	"GETOPT", "SETOPT",
};
#endif

#ifdef _KERNEL
/*
 * Monotonically increasing time values for slow and fast timers.
 */
extern	u_int pfslowtimo_now;
extern	u_int pffasttimo_now;

#define	PRT_SLOW_ARM(t, nticks)	(t) = (pfslowtimo_now + (nticks))
#define	PRT_FAST_ARM(t, nticks)	(t) = (pffasttimo_now + (nticks))

#define	PRT_SLOW_DISARM(t)	(t) = 0
#define	PRT_FAST_DISARM(t)	(t) = 0

#define	PRT_SLOW_ISARMED(t)	((t) != 0)
#define	PRT_FAST_ISARMED(t)	((t) != 0)

#define	PRT_SLOW_ISEXPIRED(t)	(PRT_SLOW_ISARMED((t)) && (t) <= pfslowtimo_now)
#define	PRT_FAST_ISEXPIRED(t)	(PRT_FAST_ISARMED((t)) && (t) <= pffasttimo_now)

struct sockaddr;
const struct protosw *pffindproto(int, int, int);
const struct protosw *pffindtype(int, int);
struct domain *pffinddomain(int);
void pfctlinput(int, const struct sockaddr *);
void pfctlinput2(int, const struct sockaddr *, void *);

/*
 * Wrappers for non-MPSAFE protocols
 */
#include <sys/systm.h>	/* kernel_lock */

#define	PR_WRAP_USRREQ(name)				\
static int						\
name##_wrapper(struct socket *a, int b, struct mbuf *c,	\
     struct mbuf *d, struct mbuf *e, struct lwp *f)	\
{							\
	int rv;						\
	KERNEL_LOCK(1, NULL);				\
	rv = name(a, b, c, d, e, f);			\
	KERNEL_UNLOCK_ONE(NULL);			\
	return rv;					\
}

#define	PR_WRAP_CTLOUTPUT(name)				\
static int						\
name##_wrapper(int a, struct socket *b,			\
    struct sockopt *c)					\
{							\
	int rv;						\
	KERNEL_LOCK(1, NULL);				\
	rv = name(a, b, c);				\
	KERNEL_UNLOCK_ONE(NULL);			\
	return rv;					\
}

#define	PR_WRAP_CTLINPUT(name)				\
static void *						\
name##_wrapper(int a, const struct sockaddr *b, void *c)\
{							\
	void *rv;					\
	KERNEL_LOCK(1, NULL);				\
	rv = name(a, b, c);				\
	KERNEL_UNLOCK_ONE(NULL);			\
	return rv;					\
}

#endif /* _KERNEL */

#endif /* !_SYS_PROTOSW_H_ */
