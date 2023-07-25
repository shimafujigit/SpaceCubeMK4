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

/* $NetBSD: netisr_dispatch.h,v 1.14 2007/07/14 21:02:42 ad Exp $ */

#ifndef _NET_NETISR_DISPATCH_H_
#define _NET_NETISR_DISPATCH_H_

/*
 * netisr_dispatch: This file is included by the
 *	machine dependant softnet function.  The
 *	DONETISR macro should be set before including
 *	this file.  i.e.:
 *
 * softintr() {
 *	...do setup stuff...
 *	#define DONETISR(bit, fn) do { ... } while (0)
 *	#include <net/netisr_dispatch.h>
 *	#undef DONETISR
 *	...do cleanup stuff.
 * }
 */

#ifndef _NET_NETISR_H_
#error <net/netisr.h> must be included before <net/netisr_dispatch.h>
#endif

/*
 * When adding functions to this list, be sure to add headers to provide
 * their prototypes in <net/netisr.h> (if necessary).
 */

#ifdef INET
#if NARP > 0
	DONETISR(NETISR_ARP,arpintr);
#endif
	DONETISR(NETISR_IP,ipintr);
#endif
#ifdef INET6
	DONETISR(NETISR_IPV6,ip6intr);
#endif
#ifdef NETATALK
	DONETISR(NETISR_ATALK,atintr);
#endif
#ifdef ISO
	DONETISR(NETISR_ISO,clnlintr);
#endif
#ifdef NATM
	DONETISR(NETISR_NATM,natmintr);
#endif

#endif /* !_NET_NETISR_DISPATCH_H_ */
