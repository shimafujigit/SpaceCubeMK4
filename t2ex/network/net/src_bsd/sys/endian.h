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

/*	$NetBSD: endian.h,v 1.26 2007/07/20 15:07:15 christos Exp $	*/

/*
 * Copyright (c) 1987, 1991, 1993
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
 *	@(#)endian.h	8.1 (Berkeley) 6/11/93
 */

#ifndef _SYS_ENDIAN_H_
#define _SYS_ENDIAN_H_

#include <sys/featuretest.h>

/*
 * Definitions for byte order, according to byte significance from low
 * address to high.
 */
#define	_LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	_BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#define	_PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */


#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
#ifndef _LOCORE

/* C-family endian-ness definitions */

#include <sys/ansi.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#ifndef in_addr_t
typedef __in_addr_t	in_addr_t;
#define	in_addr_t	__in_addr_t
#endif

#ifndef in_port_t
typedef __in_port_t	in_port_t;
#define	in_port_t	__in_port_t
#endif

__BEGIN_DECLS
uint32_t htonl(uint32_t) __attribute__((__const__));
uint16_t htons(uint16_t) __attribute__((__const__));
uint32_t ntohl(uint32_t) __attribute__((__const__));
uint16_t ntohs(uint16_t) __attribute__((__const__));
__END_DECLS

#endif /* !_LOCORE */
#endif /* _XOPEN_SOURCE || _NETBSD_SOURCE */


#ifndef T2EX
#include <machine/endian_machdep.h>
#else
#include <sys/sysdepend/machine_common.h>
#if BIGENDIAN
#define _BYTE_ORDER _BIG_ENDIAN
#else
#define _BYTE_ORDER _LITTLE_ENDIAN
#endif
#endif

/*
 * Define the order of 32-bit words in 64-bit words.
 */
#if _BYTE_ORDER == _LITTLE_ENDIAN
#define _QUAD_HIGHWORD 1
#define _QUAD_LOWWORD 0
#endif

#if _BYTE_ORDER == _BIG_ENDIAN
#define _QUAD_HIGHWORD 0
#define _QUAD_LOWWORD 1
#endif


#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
/*
 *  Traditional names for byteorder.  These are defined as the numeric
 *  sequences so that third party code can "#define XXX_ENDIAN" and not
 *  cause errors.
 */
#define	LITTLE_ENDIAN	1234		/* LSB first: i386, vax */
#define	BIG_ENDIAN	4321		/* MSB first: 68000, ibm, net */
#define	PDP_ENDIAN	3412		/* LSB first in word, MSW first in long */
#define BYTE_ORDER	_BYTE_ORDER

#ifndef _LOCORE

#ifndef T2EX
#include <machine/bswap.h>
#else
#include <sys/bswap.h>
#endif

/*
 * Macros for network/external number representation conversion.
 */
#if BYTE_ORDER == BIG_ENDIAN && !defined(__lint__)
#define	ntohl(x)	(x)
#define	ntohs(x)	(x)
#define	htonl(x)	(x)
#define	htons(x)	(x)

#define	NTOHL(x)	(void) (x)
#define	NTOHS(x)	(void) (x)
#define	HTONL(x)	(void) (x)
#define	HTONS(x)	(void) (x)

#else	/* LITTLE_ENDIAN || !defined(__lint__) */

#define	ntohl(x)	bswap32((uint32_t)(x))
#define	ntohs(x)	bswap16((uint16_t)(x))
#define	htonl(x)	bswap32((uint32_t)(x))
#define	htons(x)	bswap16((uint16_t)(x))

#define	NTOHL(x)	(x) = ntohl((uint32_t)(x))
#define	NTOHS(x)	(x) = ntohs((uint16_t)(x))
#define	HTONL(x)	(x) = htonl((uint32_t)(x))
#define	HTONS(x)	(x) = htons((uint16_t)(x))
#endif	/* LITTLE_ENDIAN || !defined(__lint__) */

/*
 * Macros to convert to a specific endianness.
 */

#if BYTE_ORDER == BIG_ENDIAN

#define htobe16(x)	(x)
#define htobe32(x)	(x)
#define htobe64(x)	(x)
#define htole16(x)	bswap16((uint16_t)(x))
#define htole32(x)	bswap32((uint32_t)(x))
#define htole64(x)	bswap64((uint64_t)(x))

#define HTOBE16(x)	(void) (x)
#define HTOBE32(x)	(void) (x)
#define HTOBE64(x)	(void) (x)
#define HTOLE16(x)	(x) = bswap16((uint16_t)(x))
#define HTOLE32(x)	(x) = bswap32((uint32_t)(x))
#define HTOLE64(x)	(x) = bswap64((uint64_t)(x))

#else	/* LITTLE_ENDIAN */

#define htobe16(x)	bswap16((uint16_t)(x))
#define htobe32(x)	bswap32((uint32_t)(x))
#define htobe64(x)	bswap64((uint64_t)(x))
#define htole16(x)	(x)
#define htole32(x)	(x)
#define htole64(x)	(x)

#define HTOBE16(x)	(x) = bswap16((uint16_t)(x))
#define HTOBE32(x)	(x) = bswap32((uint32_t)(x))
#define HTOBE64(x)	(x) = bswap64((uint64_t)(x))
#define HTOLE16(x)	(void) (x)
#define HTOLE32(x)	(void) (x)
#define HTOLE64(x)	(void) (x)

#endif	/* LITTLE_ENDIAN */

#define be16toh(x)	htobe16(x)
#define be32toh(x)	htobe32(x)
#define be64toh(x)	htobe64(x)
#define le16toh(x)	htole16(x)
#define le32toh(x)	htole32(x)
#define le64toh(x)	htole64(x)

#define BE16TOH(x)	HTOBE16(x)
#define BE32TOH(x)	HTOBE32(x)
#define BE64TOH(x)	HTOBE64(x)
#define LE16TOH(x)	HTOLE16(x)
#define LE32TOH(x)	HTOLE32(x)
#define LE64TOH(x)	HTOLE64(x)

/*
 * Routines to encode/decode big- and little-endian multi-octet values
 * to/from an octet stream.
 */

#if __GNUC_PREREQ__(2, 95)

#define __GEN_ENDIAN_ENC(bits, endian) \
static __inline __unused void \
endian ## bits ## enc(void *dst, uint ## bits ## _t u) \
{ \
	u = hto ## endian ## bits (u); \
	__builtin_memcpy(dst, &u, sizeof(u)); \
}

__GEN_ENDIAN_ENC(16, be)
__GEN_ENDIAN_ENC(32, be)
__GEN_ENDIAN_ENC(64, be)
__GEN_ENDIAN_ENC(16, le)
__GEN_ENDIAN_ENC(32, le)
__GEN_ENDIAN_ENC(64, le)
#undef __GEN_ENDIAN_ENC

#define __GEN_ENDIAN_DEC(bits, endian) \
static __inline __unused uint ## bits ## _t \
endian ## bits ## dec(const void *buf) \
{ \
	uint ## bits ## _t u; \
	__builtin_memcpy(&u, buf, sizeof(u)); \
	return endian ## bits ## toh (u); \
}

__GEN_ENDIAN_DEC(16, be)
__GEN_ENDIAN_DEC(32, be)
__GEN_ENDIAN_DEC(64, be)
__GEN_ENDIAN_DEC(16, le)
__GEN_ENDIAN_DEC(32, le)
__GEN_ENDIAN_DEC(64, le)
#undef __GEN_ENDIAN_DEC

#else	/* !(GCC >= 2.95) */

static __inline void __unused
be16enc(void *buf, uint16_t u)
{
	uint8_t *p = (uint8_t *)buf;

	p[0] = (uint8_t)(((unsigned)u >> 8) & 0xff);
	p[1] = (uint8_t)(u & 0xff);
}

static __inline void __unused
le16enc(void *buf, uint16_t u)
{
	uint8_t *p = (uint8_t *)buf;

	p[0] = (uint8_t)(u & 0xff);
	p[1] = (uint8_t)(((unsigned)u >> 8) & 0xff);
}

static __inline uint16_t __unused
be16dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return (uint16_t)((p[0] << 8) | p[1]);
}

static __inline uint16_t __unused
le16dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return (uint16_t)((p[1] << 8) | p[0]);
}

static __inline void __unused
be32enc(void *buf, uint32_t u)
{
	uint8_t *p = (uint8_t *)buf;

	p[0] = (uint8_t)((u >> 24) & 0xff);
	p[1] = (uint8_t)((u >> 16) & 0xff);
	p[2] = (uint8_t)((u >> 8) & 0xff);
	p[3] = (uint8_t)(u & 0xff);
}

static __inline void __unused
le32enc(void *buf, uint32_t u)
{
	uint8_t *p = (uint8_t *)buf;

	p[0] = (uint8_t)(u & 0xff);
	p[1] = (uint8_t)((u >> 8) & 0xff);
	p[2] = (uint8_t)((u >> 16) & 0xff);
	p[3] = (uint8_t)((u >> 24) & 0xff);
}

static __inline uint32_t __unused
be32dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return ((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

static __inline uint32_t __unused
le32dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
}

static __inline void __unused
be64enc(void *buf, uint64_t u)
{
	uint8_t *p = (uint8_t *)buf;

	be32enc(p, (uint32_t)(u >> 32));
	be32enc(p + 4, (uint32_t)(u & 0xffffffffULL));
}

static __inline void __unused
le64enc(void *buf, uint64_t u)
{
	uint8_t *p = (uint8_t *)buf;

	le32enc(p, (uint32_t)(u & 0xffffffffULL));
	le32enc(p + 4, (uint32_t)(u >> 32));
}

static __inline uint64_t __unused
be64dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return (((uint64_t)be32dec(p) << 32) | be32dec(p + 4));
}

static __inline uint64_t __unused
le64dec(const void *buf)
{
	const uint8_t *p = (const uint8_t *)buf;

	return (le32dec(p) | ((uint64_t)le32dec(p + 4) << 32));
}

#endif	/* GCC >= 2.95 */

#endif /* !_LOCORE */
#endif /* _XOPEN_SOURCE || _NETBSD_SOURCE */
#endif /* !_SYS_ENDIAN_H_ */
