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

/*	$NetBSD: sockio.h,v 1.27 2008/10/24 22:32:53 dyoung Exp $	*/

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
 *
 *	@(#)sockio.h	8.1 (Berkeley) 3/28/94
 */

#ifndef	_SYS_SOCKIO_H_
#define	_SYS_SOCKIO_H_

#include <sys/ioccom.h>

/* Socket ioctl's. */
#define	SIOCSHIWAT	 _IOW('s',  0, int)		/* set high watermark */
#define	SIOCGHIWAT	 _IOR('s',  1, int)		/* get high watermark */
#define	SIOCSLOWAT	 _IOW('s',  2, int)		/* set low watermark */
#define	SIOCGLOWAT	 _IOR('s',  3, int)		/* get low watermark */
#define	SIOCATMARK	 _IOR('s',  7, int)		/* at oob mark? */
#define	SIOCSPGRP	 _IOW('s',  8, int)		/* set process group */
#define	SIOCGPGRP	 _IOR('s',  9, int)		/* get process group */

#define	SIOCADDRT	 _IOW('r', 10, struct ortentry)	/* add route */
#define	SIOCDELRT	 _IOW('r', 11, struct ortentry)	/* delete route */

#define	SIOCSIFADDR	 _IOW('i', 12, struct ifreq)	/* set ifnet address */
#define	SIOCGIFADDR	_IOWR('i', 33, struct ifreq)	/* get ifnet address */

#define	SIOCSIFDSTADDR	 _IOW('i', 14, struct ifreq)	/* set p-p address */
#define	SIOCGIFDSTADDR	_IOWR('i', 34, struct ifreq)	/* get p-p address */

#define	SIOCSIFFLAGS	 _IOW('i', 16, struct ifreq)	/* set ifnet flags */
#define	SIOCGIFFLAGS	_IOWR('i', 17, struct ifreq)	/* get ifnet flags */

#define	SIOCGIFBRDADDR	_IOWR('i', 35, struct ifreq)	/* get broadcast addr */
#define	SIOCSIFBRDADDR	 _IOW('i', 19, struct ifreq)	/* set broadcast addr */

#define	SIOCGIFCONF	_IOWR('i', 38, struct ifconf)	/* get ifnet list */

#define	SIOCGIFNETMASK	_IOWR('i', 37, struct ifreq)	/* get net addr mask */
#define	SIOCSIFNETMASK	 _IOW('i', 22, struct ifreq)	/* set net addr mask */

#define	SIOCGIFMETRIC	_IOWR('i', 23, struct ifreq)	/* get IF metric */
#define	SIOCSIFMETRIC	 _IOW('i', 24, struct ifreq)	/* set IF metric */

#define	SIOCDIFADDR	 _IOW('i', 25, struct ifreq)	/* delete IF addr */

#define	SIOCAIFADDR	 _IOW('i', 26, struct ifaliasreq)/* add/chg IF alias */
#define	SIOCGIFALIAS	_IOWR('i', 27, struct ifaliasreq)/* get IF alias */

#define	SIOCALIFADDR	 _IOW('i', 28, struct if_laddrreq) /* add IF addr */
#define	SIOCGLIFADDR	_IOWR('i', 29, struct if_laddrreq) /* get IF addr */
#define	SIOCDLIFADDR	 _IOW('i', 30, struct if_laddrreq) /* delete IF addr */
/* get/set IF addr preference */
#define	SIOCSIFADDRPREF	 _IOW('i', 31, struct if_addrprefreq)
#define	SIOCGIFADDRPREF	_IOWR('i', 32, struct if_addrprefreq)

#define	SIOCADDMULTI	 _IOW('i', 49, struct ifreq)	/* add m'cast addr */
#define	SIOCDELMULTI	 _IOW('i', 50, struct ifreq)	/* del m'cast addr */

#define	SIOCGETVIFCNT	_IOWR('u', 51, struct sioc_vif_req)/* vif pkt cnt */
#define	SIOCGETSGCNT	_IOWR('u', 52, struct sioc_sg_req) /* sg pkt cnt */

#define	SIOCSIFMEDIA	_IOWR('i', 53, struct ifreq)	/* set net media */
#define	SIOCGIFMEDIA	_IOWR('i', 54, struct ifmediareq) /* get net media */

#define	SIOCSIFGENERIC	 _IOW('i', 57, struct ifreq)	/* generic IF set op */
#define	SIOCGIFGENERIC	_IOWR('i', 58, struct ifreq)	/* generic IF get op */

#define	SIOCSIFPHYADDR	 _IOW('i', 70, struct ifaliasreq) /* set gif addres */
#define	SIOCGIFPSRCADDR	_IOWR('i', 71, struct ifreq)	/* get gif psrc addr */
#define	SIOCGIFPDSTADDR	_IOWR('i', 72, struct ifreq)	/* get gif pdst addr */
#define	SIOCDIFPHYADDR	 _IOW('i', 73, struct ifreq)	/* delete gif addrs */
#define	SIOCSLIFPHYADDR	 _IOW('i', 74, struct if_laddrreq) /* set gif addrs */
#define	SIOCGLIFPHYADDR	_IOWR('i', 75, struct if_laddrreq) /* get gif addrs */

#define	SIOCZIFDATA	_IOWR('i', 129, struct ifdatareq) /* get if_data then
							     zero ctrs*/
#define	SIOCGIFDATA	_IOWR('i', 128, struct ifdatareq) /* get if_data */

#define	SIOCSIFMTU	 _IOW('i', 127, struct ifreq)	/* set ifnet mtu */
#define	SIOCGIFMTU	_IOWR('i', 126, struct ifreq)	/* get ifnet mtu */

/* 125 was SIOCSIFASYNCMAP */
/* 124 was SIOCGIFASYNCMAP */

#define	SIOCSDRVSPEC     _IOW('i', 123, struct ifdrv)   /* set driver-specific
							   parameters */
#define	SIOCGDRVSPEC    _IOWR('i', 123, struct ifdrv)   /* get driver-specific
							   parameters */

#define	SIOCIFCREATE	 _IOW('i', 122, struct ifreq)	/* create clone if */
#define	SIOCIFDESTROY	 _IOW('i', 121, struct ifreq)	/* destroy clone if */
#define	SIOCIFGCLONERS	_IOWR('i', 120, struct if_clonereq) /* get cloners */

#define	SIOCGIFDLT	_IOWR('i', 119, struct ifreq)	/* get DLT */
#define	SIOCGIFCAP	_IOWR('i', 118, struct ifcapreq)/* get capabilities */
#define	SIOCSIFCAP	 _IOW('i', 117, struct ifcapreq)/* set capabilities */

#define	SIOCSVH		_IOWR('i', 130, struct ifreq)	/* set carp param */
#define	SIOCGVH		_IOWR('i', 131, struct ifreq)	/* get carp param */
#define	SIOCINITIFADDR	_IOWR('i', 132, struct ifaddr)

#endif /* !_SYS_SOCKIO_H_ */
