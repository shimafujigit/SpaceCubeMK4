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

/*
 *	@(#)inet.h
 *
 */

#ifndef _ARPA_INET_H_
#define	_ARPA_INET_H_

#include <basic.h>
#include <stdint.h>
#include <netinet/in.h>

typedef	uint32_t	socklen_t;

#ifdef __cplusplus
extern "C" {
#endif

IMPORT	uint32_t	htonl(uint32_t hostlong);
IMPORT	uint16_t	htons(uint16_t hostshort);
IMPORT	uint32_t	ntohl(uint32_t netlong);
IMPORT	uint16_t	ntohs(uint16_t netshort);

IMPORT	in_addr_t	inet_addr(const char *cp);
IMPORT	const char	*inet_ntop(int af, const void *src, char *dst, socklen_t size);
IMPORT	int	inet_pton(int af, const char *src, void *dst);

#ifdef __cplusplus
}
#endif
#endif /* _ARPA_INET_H_ */

