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

/*	$NetBSD: if_nametoindex.c,v 1.4 2000/11/24 08:21:12 itojun Exp $	*/
/*	$KAME: if_nametoindex.c,v 1.6 2000/11/24 08:18:54 itojun Exp $	*/

/*-
 * Copyright (c) 1997, 2000
 *	Berkeley Software Design, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY Berkeley Software Design, Inc. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Berkeley Software Design, Inc. BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	BSDI Id: if_nametoindex.c,v 2.3 2000/04/17 22:38:05 dab Exp
 */

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: if_nametoindex.c,v 1.4 2000/11/24 08:21:12 itojun Exp $");
#endif /* LIBC_SCCS and not lint */

#include "namespace.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __weak_alias
__weak_alias(if_nametoindex,_if_nametoindex)
#endif

/*
 * From RFC 2553:
 *
 * 4.1 Name-to-Index
 *
 *
 *    The first function maps an interface name into its corresponding
 *    index.
 *
 *       #include <net/if.h>
 *
 *       unsigned int  if_nametoindex(const char *ifname);
 *
 *    If the specified interface name does not exist, the return value is
 *    0, and errno is set to ENXIO.  If there was a system error (such as
 *    running out of memory), the return value is 0 and errno is set to the
 *    proper value (e.g., ENOMEM).
 */

#ifndef T2EX
unsigned int
if_nametoindex(const char *ifname)
#else
int
if_nametoindex(const char *ifname, unsigned int *ifindex)
#endif
{
	struct ifaddrs *ifaddrs, *ifa;
	unsigned int ni;

#ifndef T2EX
	if (getifaddrs(&ifaddrs) < 0)
		return(0);
#else
	int error;
	void *buf;
	size_t len;
	error = getifaddrs(&ifaddrs, NULL, 0, &len);
	if (error != 0) {
		return error;
	}
	buf = malloc(len);
	if ((error = getifaddrs(&ifaddrs, buf, len, &len)) < 0) {
		free(buf);
		return error;
	}
#endif

	ni = 0;

	for (ifa = ifaddrs; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr &&
		    ifa->ifa_addr->sa_family == AF_LINK &&
		    strcmp(ifa->ifa_name, ifname) == 0) {
			ni = ((struct sockaddr_dl*)
			    (void *)ifa->ifa_addr)->sdl_index;
			break;
		}
	}

#ifndef T2EX
	freeifaddrs(ifaddrs);
	if (!ni)
		errno = ENXIO;
	return(ni);
#else
	free(buf);
	if (ni == 0) {
		return ENXIO;
	}
	*ifindex = ni;
	return 0;
#endif
}
