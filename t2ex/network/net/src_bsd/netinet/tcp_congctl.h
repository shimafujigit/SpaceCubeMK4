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

/*	$NetBSD: tcp_congctl.h,v 1.5 2008/04/28 20:24:09 martin Exp $	*/

/*
 * Copyright (c) 2006 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Rui Paulo.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _NETINET_TCP_CONGCTL_H
#define _NETINET_TCP_CONGCTL_H

#define TCPCC_MAXLEN 12

struct tcp_congctlent {
	TAILQ_ENTRY(tcp_congctlent) congctl_ent;
	const struct tcp_congctl *congctl_ctl;
	unsigned int congctl_refcnt;
	char congctl_name[TCPCC_MAXLEN];
};
/*
 * Congestion control function table.
 */
struct tcp_congctl {
	int  (*fast_retransmit)(struct tcpcb *, const struct tcphdr *);
	void (*slow_retransmit)(struct tcpcb *);
	void (*fast_retransmit_newack)(struct tcpcb *, const struct tcphdr *);
	void (*newack)(struct tcpcb *, const struct tcphdr *);
	void (*cong_exp)(struct tcpcb *);
};

extern const struct tcp_congctl tcp_reno_ctl;
extern const struct tcp_congctl tcp_newreno_ctl;

extern struct simplelock tcp_congctl_slock;

/* currently selected global congestion control */
extern char tcp_congctl_global_name[TCPCC_MAXLEN];

/* available global congestion control algorithms */
extern char tcp_congctl_avail[10 * TCPCC_MAXLEN];

#ifndef T2EX
void   tcp_congctl_init(void);
#else
int    tcp_congctl_init(void);
void   tcp_congctl_finish(void);
#endif
int    tcp_congctl_register(const char *, const struct tcp_congctl *);
int    tcp_congctl_unregister(const char *);
int    tcp_congctl_select(struct tcpcb *, const char *);
void   tcp_congctl_release(struct tcpcb *);
const char *
       tcp_congctl_bystruct(const struct tcp_congctl *);

#endif /* _NETINET_TCP_CONGCTL_H */
