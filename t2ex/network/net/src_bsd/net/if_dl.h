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

/*	$NetBSD: if_dl.h,v 1.23 2008/02/20 17:18:11 matt Exp $	*/

/*
 * Copyright (c) 1990, 1993
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
 *	@(#)if_dl.h	8.1 (Berkeley) 6/10/93
 */

/*
 * A Link-Level Sockaddr may specify the interface in one of two
 * ways: either by means of a system-provided index number (computed
 * anew and possibly differently on every reboot), or by a human-readable
 * string such as "il0" (for managerial convenience).
 *
 * Census taking actions, such as something akin to SIOCGCONF would return
 * both the index and the human name.
 *
 * High volume transactions (such as giving a link-level ``from'' address
 * in a recvfrom or recvmsg call) may be likely only to provide the indexed
 * form, (which requires fewer copy operations and less space).
 *
 * The form and interpretation  of the link-level address is purely a matter
 * of convention between the device driver and its consumers; however, it is
 * expected that all drivers for an interface of a given if_type will agree.
 */

#ifndef _NET_IF_DL_H_
#define _NET_IF_DL_H_

#include <sys/ansi.h>

#ifndef sa_family_t
typedef __sa_family_t	sa_family_t;
#define sa_family_t	__sa_family_t
#endif

/*
 * Structure of a Link-Level sockaddr:
 */
struct sockaddr_dl {
	uint8_t	    sdl_len;	/* Total length of sockaddr */
	sa_family_t sdl_family;	/* AF_LINK */
	uint16_t   sdl_index;	/* if != 0, system given index for interface */
	uint8_t	    sdl_type;	/* interface type */
	uint8_t	    sdl_nlen;	/* interface name length, no trailing 0 reqd. */
	uint8_t	    sdl_alen;	/* link level address length */
	uint8_t	    sdl_slen;	/* link layer selector length */
	/* minimum work area, can be larger; contains both if name
	 * and ll address
	 */
	char	    sdl_data[12];
};

#define	satosdl(__sa)	((struct sockaddr_dl *)(__sa))
#define	satocsdl(__sa)	((const struct sockaddr_dl *)(__sa))

/* We do arithmetic directly with these, so keep them char instead of void */
#define LLADDR(s) ((char *)((s)->sdl_data + (s)->sdl_nlen))
#define CLLADDR(s) ((const char *)((s)->sdl_data + (s)->sdl_nlen))

#ifdef _KERNEL
uint8_t sockaddr_dl_measure(uint8_t, uint8_t);
struct sockaddr *sockaddr_dl_alloc(uint16_t, uint8_t,
    const void *, uint8_t, const void *, uint8_t, int);
struct sockaddr_dl *sockaddr_dl_init(struct sockaddr_dl *, socklen_t, uint16_t,
    uint8_t, const void *, uint8_t, const void *, uint8_t);
struct sockaddr_dl *sockaddr_dl_setaddr(struct sockaddr_dl *, socklen_t,
    const void *, uint8_t);
#else

#include <sys/cdefs.h>

__BEGIN_DECLS
void	link_addr(const char *, struct sockaddr_dl *);
char	*link_ntoa(const struct sockaddr_dl *);
__END_DECLS

#endif /* !_KERNEL */

#endif /* !_NET_IF_DL_H_ */
