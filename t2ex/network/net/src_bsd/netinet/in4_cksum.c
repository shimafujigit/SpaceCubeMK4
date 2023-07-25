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

/*	$NetBSD: in4_cksum.c,v 1.17 2008/02/12 13:05:55 joerg Exp $	*/

/*-
 * Copyright (c) 2008 Joerg Sonnenberger <joerg@NetBSD.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: in4_cksum.c,v 1.17 2008/02/12 13:05:55 joerg Exp $");

#include <sys/param.h>
#include <sys/mbuf.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

/*
 * Checksum of the IPv4 pseudo header.
 *
 * off is supposed to be the skipped IPv4 header, len is the payload size.
 */

int
in4_cksum(struct mbuf *m, u_int8_t nxt, int off, int len)
{
	uint32_t sum;
	uint16_t *w;

	if (__predict_false(off < sizeof(struct ip)))
		panic("in4_cksum: offset too short for IP header");
	if (__predict_false(m->m_len < sizeof(struct ip)))
		panic("in4_cksum: mbuf too short for IP header");

	if (nxt == 0)
		return cpu_in_cksum(m, len, off, 0);

	/*
	 * Compute the equivalent of:
	 * struct ipovly ip;
	 *
	 * bzero(sizeof(*ip));
	 * ip.ih_pr = nxt;
	 * ip.ip_len = htons(len);
	 * ip.ih_src = mtod(m, struct ip *)->ip_src;
	 * ip.ih_dst = mtod(m, struct ip *)->ip_dst;
	 * sum = one_add(&ip);
	 */

#if BYTE_ORDER == LITTLE_ENDIAN
	sum = ((len & 0xffff) + nxt) << 8;
#else
	sum = (len & 0xffff) + nxt;
#endif
	w = (uint16_t *)(mtod(m, char  *) + offsetof(struct ip, ip_src));
	if (__predict_true((uintptr_t)w % 2 == 0)) {
		sum += w[0];
		sum += w[1];
		sum += w[2];
		sum += w[3];
	} else {
		uint32_t partial;
		w = (void *)((uintptr_t)w - 1);
#if BYTE_ORDER == LITTLE_ENDIAN
		partial = w[0] & 0xff00;
#else
		partial = w[0] & 0x00ff;
#endif
		partial += w[1];
		partial += w[2];
		partial += w[3];
#if BYTE_ORDER == LITTLE_ENDIAN
		partial += w[4] & 0x00ff;
#else
		partial += w[4] & 0xff00;
#endif
		sum += partial << 8;
	}

	return cpu_in_cksum(m, len, off, sum);
}
