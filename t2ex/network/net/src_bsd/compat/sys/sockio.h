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

/*	$NetBSD: sockio.h,v 1.4 2007/08/20 04:49:41 skd Exp $	*/

/*-
 * Copyright (c) 1982, 1986, 1990, 1993, 1994
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
 */

#ifndef _COMPAT_SYS_SOCKIO_H_
#define	_COMPAT_SYS_SOCKIO_H_

#ifdef _KERNEL_OPT

#include "opt_compat_netbsd.h"

#if defined(COMPAT_09) || defined(COMPAT_10) || defined(COMPAT_11) || \
    defined(COMPAT_12) || defined(COMPAT_13) || defined(COMPAT_14) || \
    defined(COMPAT_15) || defined(COMPAT_16) || defined(COMPAT_20) || \
    defined(COMPAT_30) || defined(COMPAT_40)
#define COMPAT_OIFREQ
#endif

#else
#define COMPAT_OIFREQ
#endif

struct oifreq {
	char	ifr_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		short	ifru_flags;
		int	ifru_metric;
		int	ifru_mtu;
		int	ifru_dlt;
		u_int	ifru_value;
		void *	ifru_data;
		struct {
			uint32_t	b_buflen;
			void		*b_buf;
		} ifru_b;
	} ifr_ifru;
};
struct	oifconf {
	int	ifc_len;		/* size of associated buffer */
	union {
		void *	ifcu_buf;
		struct	oifreq *ifcu_req;
	} ifc_ifcu;
#define	ifc_buf	ifc_ifcu.ifcu_buf	/* buffer address */
#define	ifc_req	ifc_ifcu.ifcu_req	/* array of structures returned */
};

#define	OSIOCSIFADDR	 _IOW('i', 12, struct oifreq)	/* set ifnet address */
#define	OOSIOCGIFADDR	 _IOWR('i', 13, struct oifreq)	/* get ifnet address */
#define	OSIOCSIFDSTADDR	 _IOW('i', 14, struct oifreq)	/* set p-p address */
#define	OOSIOCGIFDSTADDR _IOWR('i', 15, struct oifreq)	/* get p-p address */
#define OSIOCSIFFLAGS    _IOW('i', 16, struct oifreq)	/* set ifnet flags */
#define	OSIOCGIFFLAGS    _IOWR('i', 17, struct oifreq)	/* get ifnet flags */
#define	OOSIOCGIFBRDADDR _IOWR('i', 18, struct oifreq)	/* get bcast addr */
#define	OSIOCSIFBRDADDR	 _IOW('i', 19, struct oifreq)	/* set bcast addr */
#define	OOSIOCGIFCONF	 _IOWR('i', 20, struct ifconf)	/* get ifnet list */
#define	OOSIOCGIFNETMASK _IOWR('i', 21, struct oifreq)	/* get net addr mask */
#define	OSIOCSIFNETMASK	 _IOW('i', 22, struct oifreq)	/* set net addr mask */
#define	OSIOCGIFCONF	 _IOWR('i', 36, struct ifconf)	/* get ifnet list */
#define	OSIOCADDMULTI	 _IOW('i', 49, struct oifreq)	/* add m'cast addr */
#define	OSIOCDELMULTI	 _IOW('i', 50, struct oifreq)	/* del m'cast addr */
#define	OSIOCSIFMEDIA	 _IOWR('i', 53, struct oifreq)	/* set net media */




#define	OBIOCGETIF	 _IOR('B', 107, struct oifreq)
#define	OBIOCSETIF	 _IOW('B', 108, struct oifreq)
#define	OTAPGIFNAME	 _IOR('e', 0, struct oifreq)

#define ifreqn2o(oi, ni) \
	do { \
		(void)memcpy((oi)->ifr_name, (ni)->ifr_name, \
		    sizeof((oi)->ifr_name)); \
		(void)memcpy(&(oi)->ifr_ifru, &(ni)->ifr_ifru, \
		    sizeof((oi)->ifr_ifru)); \
	} while (/*CONSTCOND*/0)

#define ifreqo2n(oi, ni) \
	do { \
		(void)memcpy((ni)->ifr_name, (oi)->ifr_name, \
		    sizeof((oi)->ifr_name)); \
		(void)memcpy(&(ni)->ifr_ifru, &(oi)->ifr_ifru, \
		    sizeof((oi)->ifr_ifru)); \
	} while (/*CONSTCOND*/0)

#ifdef _KERNEL
__BEGIN_DECLS
int compat_ifconf(u_long, void *);
__END_DECLS
#endif
#endif /* _COMPAT_SYS_SOCKIO_H_ */
