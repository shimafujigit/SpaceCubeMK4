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

/*	$NetBSD: udp_var.h,v 1.36 2008/08/06 15:01:23 plunky Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)udp_var.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _NETINET_UDP_VAR_H_
#define _NETINET_UDP_VAR_H_

/*
 * UDP kernel structures and variables.
 */
struct	udpiphdr {
	struct 	ipovly ui_i;		/* overlaid ip structure */
	struct	udphdr ui_u;		/* udp header */
} __packed;
#define	ui_x1		ui_i.ih_x1
#define	ui_pr		ui_i.ih_pr
#define	ui_len		ui_i.ih_len
#define	ui_src		ui_i.ih_src
#define	ui_dst		ui_i.ih_dst
#define	ui_sport	ui_u.uh_sport
#define	ui_dport	ui_u.uh_dport
#define	ui_ulen		ui_u.uh_ulen
#define	ui_sum		ui_u.uh_sum

/*
 * UDP statistics.
 * Each counter is an unsigned 64-bit value.
 */
#define	UDP_STAT_IPACKETS	0	/* total input packets */
#define	UDP_STAT_HDROPS		1	/* packet shorter than header */
#define	UDP_STAT_BADSUM		2	/* checksum error */
#define	UDP_STAT_BADLEN		3	/* data length larger than packet */
#define	UDP_STAT_NOPORT		4	/* no socket on port */
#define	UDP_STAT_NOPORTBCAST	5	/* of above, arrived as broadcast */
#define	UDP_STAT_FULLSOCK	6	/* not delivered, input socket full */
#define	UDP_STAT_PCBHASHMISS	7	/* input packets missing PCB hash */
#define	UDP_STAT_OPACKETS	8	/* total output packets */

#define	UDP_NSTATS		9

/*
 * Names for UDP sysctl objects
 */
#define	UDPCTL_CHECKSUM		1	/* checksum UDP packets */
#define	UDPCTL_SENDSPACE	2	/* default send buffer */
#define	UDPCTL_RECVSPACE	3	/* default recv buffer */
#define	UDPCTL_LOOPBACKCKSUM	4	/* do UDP checksum on loopback */
#define	UDPCTL_STATS		5	/* UDP statistics */
#define	UDPCTL_MAXID		6

#define UDPCTL_NAMES { \
	{ 0, 0 }, \
	{ "checksum", CTLTYPE_INT }, \
	{ "sendspace", CTLTYPE_INT }, \
	{ "recvspace", CTLTYPE_INT }, \
	{ "do_loopback_cksum", CTLTYPE_INT }, \
	{ "stats", CTLTYPE_STRUCT }, \
}

#ifdef _KERNEL
extern	struct	inpcbtable udbtable;

void	 *udp_ctlinput(int, const struct sockaddr *, void *);
int	 udp_ctloutput(int, struct socket *, struct sockopt *);
#ifndef T2EX
void	 udp_init(void);
#else
int	 udp_init(void);
void	 udp_finish(void);
#endif
void	 udp_input(struct mbuf *, ...);
int	 udp_output(struct mbuf *, ...);
int	 udp_sysctl(int *, u_int, void *, size_t *, void *, size_t);
int	 udp_usrreq(struct socket *,
	    int, struct mbuf *, struct mbuf *, struct mbuf *, struct lwp *);

int	udp_input_checksum(int af, struct mbuf *, const struct udphdr *, int,
	    int);
void	udp_statinc(u_int);
#endif /* _KERNEL */

#endif /* !_NETINET_UDP_VAR_H_ */
