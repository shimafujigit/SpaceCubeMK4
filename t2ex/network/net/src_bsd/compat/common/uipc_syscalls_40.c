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

/*	$NetBSD: uipc_syscalls_40.c,v 1.6 2007/12/05 22:51:28 dyoung Exp $	*/

/* written by Pavel Cahyna, 2006. Public domain. */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: uipc_syscalls_40.c,v 1.6 2007/12/05 22:51:28 dyoung Exp $");

/*
 * System call interface to the socket abstraction.
 */

#include <sys/param.h>
#include <sys/kernel.h>
#ifndef T2EX
#include <sys/msg.h>
#endif
#include <sys/sysctl.h>
#ifndef T2EX
#include <sys/mount.h>
#endif
#include <sys/syscallargs.h>
#include <sys/errno.h>

#include <net/if.h>

#include <compat/sys/socket.h>
#include <compat/sys/sockio.h>

#ifdef COMPAT_OIFREQ
/*
 * Return interface configuration
 * of system.  List may be used
 * in later ioctl's (above) to get
 * other information.
 */
/*ARGSUSED*/
int
compat_ifconf(u_long cmd, void *data)
{
	struct oifconf *ifc = data;
	struct ifnet *ifp;
	struct ifaddr *ifa;
	struct oifreq ifr, *ifrp;
	int space, error = 0;
	const int sz = (int)sizeof(ifr);

	if ((ifrp = ifc->ifc_req) == NULL)
		space = 0;
	else
		space = ifc->ifc_len;
	IFNET_FOREACH(ifp) {
		(void)strncpy(ifr.ifr_name, ifp->if_xname,
		    sizeof(ifr.ifr_name));
		if (ifr.ifr_name[sizeof(ifr.ifr_name) - 1] != '\0')
			return ENAMETOOLONG;
		if (IFADDR_EMPTY(ifp)) {
			memset(&ifr.ifr_addr, 0, sizeof(ifr.ifr_addr));
			if (space >= sz) {
				error = copyout(&ifr, ifrp, sz);
				if (error != 0)
					return (error);
				ifrp++;
			}
			space -= sizeof(ifr);
			continue;
		}

		IFADDR_FOREACH(ifa, ifp) {
			struct sockaddr *sa = ifa->ifa_addr;
#ifdef COMPAT_OSOCK
			if (cmd == OOSIOCGIFCONF) {
				struct osockaddr *osa =
				    (struct osockaddr *)&ifr.ifr_addr;
				/*
				 * If it does not fit, we don't bother with it
				 */
				if (sa->sa_len > sizeof(*osa))
					continue;
				memcpy(&ifr.ifr_addr, sa, sa->sa_len);
				osa->sa_family = sa->sa_family;
				if (space >= sz) {
					error = copyout(&ifr, ifrp, sz);
					ifrp++;
				}
			} else
#endif
			if (sa->sa_len <= sizeof(*sa)) {
				memcpy(&ifr.ifr_addr, sa, sa->sa_len);
				if (space >= sz) {
					error = copyout(&ifr, ifrp, sz);
					ifrp++;
				}
			} else {
				space -= sa->sa_len - sizeof(*sa);
				if (space >= sz) {
					error = copyout(&ifr, ifrp,
					    sizeof(ifr.ifr_name));
					if (error == 0) {
						error = copyout(sa,
						    &ifrp->ifr_addr,
						    sa->sa_len);
					}
					ifrp = (struct oifreq *)
						(sa->sa_len +
						 (char *)&ifrp->ifr_addr);
				}
			}
			if (error != 0)
				return (error);
			space -= sz;
		}
	}
	if (ifrp != NULL)
		ifc->ifc_len -= space;
	else
		ifc->ifc_len = -space;
	return (0);
}
#endif
