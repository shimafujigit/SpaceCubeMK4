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

/*      $NetBSD: bswap.h,v 1.13 2008/06/27 22:19:43 mlelstv Exp $      */

/* Written by Manuel Bouyer. Public domain */

#ifndef _SYS_BSWAP_H_
#define _SYS_BSWAP_H_

#ifndef _LOCORE
#include <sys/cdefs.h>
#include <sys/types.h>

#ifndef T2EX
#include <machine/bswap.h>
#endif

__BEGIN_DECLS
/* Always declare the functions in case their address is taken (etc) */
#if defined(_KERNEL) || defined(_STANDALONE) || !defined(__BSWAP_RENAME)
uint16_t bswap16(uint16_t) __attribute__((__const__));
uint32_t bswap32(uint32_t) __attribute__((__const__));
#else
uint16_t bswap16(uint16_t) __RENAME(__bswap16) __attribute__((__const__));
uint32_t bswap32(uint32_t) __RENAME(__bswap32) __attribute__((__const__));
#endif
uint64_t bswap64(uint64_t) __attribute__((__const__));
__END_DECLS

#if defined(__GNUC__) && defined(__OPTIMIZE__)

/* machine/byte_swap.h might have defined inline versions */
#ifndef __BYTE_SWAP_U64_VARIABLE
#define	__BYTE_SWAP_U64_VARIABLE bswap64
#endif

#ifndef __BYTE_SWAP_U32_VARIABLE
#define	__BYTE_SWAP_U32_VARIABLE bswap32
#endif

#ifndef __BYTE_SWAP_U16_VARIABLE
#define	__BYTE_SWAP_U16_VARIABLE bswap16
#endif

#define	__byte_swap_u64_constant(x) \
	((uint64_t) \
	 ((((x) & 0xff00000000000000ull) >> 56) | \
	  (((x) & 0x00ff000000000000ull) >> 40) | \
	  (((x) & 0x0000ff0000000000ull) >> 24) | \
	  (((x) & 0x000000ff00000000ull) >>  8) | \
	  (((x) & 0x00000000ff000000ull) <<  8) | \
	  (((x) & 0x0000000000ff0000ull) << 24) | \
	  (((x) & 0x000000000000ff00ull) << 40) | \
	  (((x) & 0x00000000000000ffull) << 56)))

#define	__byte_swap_u32_constant(x) \
	((uint32_t) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24)))

#define	__byte_swap_u16_constant(x) \
	((uint16_t) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8)))

#define	bswap64(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u64_constant(x) : __BYTE_SWAP_U64_VARIABLE(x))

#define	bswap32(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u32_constant(x) : __BYTE_SWAP_U32_VARIABLE(x))

#define	bswap16(x) \
	(__builtin_constant_p((x)) ? \
	 __byte_swap_u16_constant(x) : __BYTE_SWAP_U16_VARIABLE(x))

#endif /* __GNUC__ && __OPTIMIZE__ */
#endif /* !_LOCORE */

#endif /* !_SYS_BSWAP_H_ */
