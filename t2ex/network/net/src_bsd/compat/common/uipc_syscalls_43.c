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

/*	$NetBSD: uipc_syscalls_43.c,v 1.43 2008/04/29 19:02:14 ad Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1989, 1990, 1993
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
 *	@(#)uipc_syscalls.c	8.4 (Berkeley) 2/21/94
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: uipc_syscalls_43.c,v 1.43 2008/04/29 19:02:14 ad Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/malloc.h>
#include <sys/syslog.h>
#include <sys/unistd.h>
#ifndef T2EX
#include <sys/resourcevar.h>
#include <sys/mbuf.h>		/* for MLEN */
#endif
#include <sys/protosw.h>

#ifndef T2EX
#include <sys/mount.h>
#endif
#include <sys/syscallargs.h>

#include <net/if.h>
#include <net/bpf.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#ifndef T2EX
#include <net/if_gre.h>
#include <net/if_atm.h>
#include <net/if_tap.h>
#endif
#include <net80211/ieee80211_ioctl.h>
#include <netinet6/in6_var.h>
#ifndef T2EX
#include <netinet6/nd6.h>
#endif
#include <compat/sys/socket.h>
#include <compat/sys/sockio.h>

#ifndef T2EX
#include <compat/common/compat_util.h>

#include <uvm/uvm_extern.h>
#endif

#ifndef T2EX
/*
 * Following 4.3 syscalls were not versioned, even through they should
 * have been:
 * connect(2), bind(2), sendto(2)
 */

static int compat_43_sa_put(void *);

int
compat_43_sys_accept(struct lwp *l, const struct compat_43_sys_accept_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(void *) name;
		syscallarg(int *) anamelen;
	} */
	int error;

	if ((error = sys_accept(l, (const void *)uap, retval)) != 0)
		return error;

	if (SCARG(uap, name)
	    && (error = compat_43_sa_put(SCARG(uap, name))))
		return (error);

	return 0;
}

int
compat_43_sys_getpeername(struct lwp *l, const struct compat_43_sys_getpeername_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) fdes;
		syscallarg(void *) asa;
		syscallarg(int *) alen;
	} */

	int error;

	if ((error = sys_getpeername(l, (const void *)uap, retval)) != 0)
		return error;

	if ((error = compat_43_sa_put(SCARG(uap, asa))))
		return (error);

	return 0;
}

int
compat_43_sys_getsockname(struct lwp *l, const struct compat_43_sys_getsockname_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) fdes;
		syscallarg(void *) asa;
		syscallarg(int *) alen;
	} */
	int error;

	if ((error = sys_getsockname(l, (const void *)uap, retval)) != 0)
		return error;

	if ((error = compat_43_sa_put(SCARG(uap, asa))))
		return (error);

	return 0;
}
#endif

int
compat_43_sys_recv(struct lwp *l, const struct compat_43_sys_recv_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(void *) buf;
		syscallarg(int) len;
		syscallarg(int) flags;
	} */
#ifdef T2EX
#define	s		sd
#define	from		src_addr
#define	fromlenaddr	addrlen
#endif

	struct sys_recvfrom_args bra;

	SCARG(&bra, s) = SCARG(uap, s);
	SCARG(&bra, buf) = SCARG(uap, buf);
	SCARG(&bra, len) = (size_t) SCARG(uap, len);
	SCARG(&bra, flags) = SCARG(uap, flags);
	SCARG(&bra, from) = NULL;
	SCARG(&bra, fromlenaddr) = NULL;

	return (sys_recvfrom(l, &bra, retval));

#ifdef T2EX
#undef	s
#undef	from
#undef	fromlenaddr
#endif
}

#ifndef T2EX
int
compat_43_sys_recvfrom(struct lwp *l, const struct compat_43_sys_recvfrom_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(void *) buf;
		syscallarg(size_t) len;
		syscallarg(int) flags;
		syscallarg(void *) from;
		syscallarg(int *) fromlenaddr;
	} */
	int error;

	if ((error = sys_recvfrom(l, (const void *)uap, retval)))
		return (error);

	if (SCARG(uap, from) && (error = compat_43_sa_put(SCARG(uap, from))))
		return (error);

	return (0);
}

/*
 * Old recvmsg. Arrange necessary structures, calls generic code and
 * adjusts results accordingly.
 */
int
compat_43_sys_recvmsg(struct lwp *l, const struct compat_43_sys_recvmsg_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(struct omsghdr *) msg;
		syscallarg(int) flags;
	} */
	struct omsghdr omsg;
	struct msghdr msg;
	struct mbuf *from, *control;
	int error;

	error = copyin(SCARG(uap, msg), &omsg, sizeof (struct omsghdr));
	if (error)
		return (error);

	if (omsg.msg_accrights == NULL)
		omsg.msg_accrightslen = 0;
	/* it was this way in 4.4BSD */
	if (omsg.msg_accrightslen > MLEN)
		return EINVAL;

	msg.msg_name	= omsg.msg_name;
	msg.msg_namelen = omsg.msg_namelen;
	msg.msg_iovlen	= omsg.msg_iovlen;
	msg.msg_iov	= omsg.msg_iov;
	msg.msg_flags	= (SCARG(uap, flags) & MSG_USERFLAGS) | MSG_IOVUSRSPACE;

	error = do_sys_recvmsg(l, SCARG(uap, s), &msg, &from,
	    omsg.msg_accrights != NULL ? &control : NULL, retval);
	if (error != 0)
		return error;

	/*
	 * If there is any control information and it's SCM_RIGHTS,
	 * pass it back to the program.
	 * XXX: maybe there can be more than one chunk of control data?
	 */
	if (omsg.msg_accrights && control != NULL) {
		struct cmsghdr *cmsg = mtod(control, void *);

		if (cmsg->cmsg_level == SOL_SOCKET
		    && cmsg->cmsg_type == SCM_RIGHTS
		    && cmsg->cmsg_len < omsg.msg_accrightslen
		    && copyout(CMSG_DATA(cmsg), omsg.msg_accrights,
			    cmsg->cmsg_len) == 0) {
			omsg.msg_accrightslen = cmsg->cmsg_len;
			free_control_mbuf(l, control, control->m_next);
		} else {
			omsg.msg_accrightslen = 0;
			free_control_mbuf(l, control, control);
		}
	} else
		omsg.msg_accrightslen = 0;

	if (from != NULL)
		/* convert from sockaddr sa_family to osockaddr one here */
		mtod(from, struct osockaddr *)->sa_family =
				    mtod(from, struct sockaddr *)->sa_family;

	error = copyout_sockname(omsg.msg_name, &omsg.msg_namelen, 0, from);
	if (from != NULL)
		m_free(from);

	if (error != 0)
		 error = copyout(&omsg, SCARG(uap, msg), sizeof(omsg));

	return error;
}
#endif

int
compat_43_sys_send(struct lwp *l, const struct compat_43_sys_send_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(void *) buf;
		syscallarg(int) len;
		syscallarg(int) flags;
	} */
#ifdef T2EX
#define	s	sd
#define	to	dest_addr
#define	tolen	addrlen
#endif

	struct sys_sendto_args bsa;

	SCARG(&bsa, s)		= SCARG(uap, s);
	SCARG(&bsa, buf)	= SCARG(uap, buf);
	SCARG(&bsa, len)	= SCARG(uap, len);
	SCARG(&bsa, flags)	= SCARG(uap, flags);
	SCARG(&bsa, to)		= NULL;
	SCARG(&bsa, tolen)	= 0;

	return (sys_sendto(l, &bsa, retval));

#ifdef T2EX
#undef	s
#undef	to
#undef	tolen
#endif
}

#ifndef T2EX
int
compat43_set_accrights(struct msghdr *msg, void *accrights, int accrightslen)
{
	struct cmsghdr *cmsg;
	int error;
	struct mbuf *ctl;
	u_int clen;

	if (accrights == NULL || accrightslen == 0) {
		msg->msg_control = NULL;
		msg->msg_controllen = 0;
		return 0;
	}

	clen = CMSG_SPACE(accrightslen);
	/* it was (almost) this way in 4.4BSD */
	if (accrightslen < 0 || clen > MLEN)
		return EINVAL;

	ctl = m_get(M_WAIT, MT_CONTROL);
	ctl->m_len = clen;
	cmsg = mtod(ctl, void *);
	cmsg->cmsg_len		= CMSG_SPACE(accrightslen);
	cmsg->cmsg_level	= SOL_SOCKET;
	cmsg->cmsg_type 	= SCM_RIGHTS;

	error = copyin(accrights, CMSG_DATA(cmsg), accrightslen);
	if (error) {
		m_free(ctl);
		return error;
	}

	msg->msg_control = ctl;
	msg->msg_controllen = clen;
	msg->msg_flags |= MSG_CONTROLMBUF;
	return 0;
}

/*
 * Old sendmsg. Arrange necessary structures, call generic code and
 * adjust the results accordingly for old code.
 */
int
compat_43_sys_sendmsg(struct lwp *l, const struct compat_43_sys_sendmsg_args *uap, register_t *retval)
{
	/* {
		syscallarg(int) s;
		syscallarg(void *) msg;
		syscallarg(int) flags;
	} */
	struct omsghdr omsg;
	struct msghdr msg;
	int error;
	struct mbuf *nam;
	struct osockaddr *osa;
	struct sockaddr *sa;

	error = copyin(SCARG(uap, msg), &omsg, sizeof (struct omsghdr));
	if (error != 0)
		return (error);

	msg.msg_iovlen = omsg.msg_iovlen;
	msg.msg_iov = omsg.msg_iov;

	error = sockargs(&nam, omsg.msg_name, omsg.msg_namelen, MT_SONAME);
	if (error != 0)
		return (error);

	sa = mtod(nam, void *);
	osa = mtod(nam, void *);
	sa->sa_family = osa->sa_family;
	sa->sa_len = omsg.msg_namelen;

	msg.msg_flags = MSG_IOVUSRSPACE | MSG_NAMEMBUF;

	msg.msg_name = nam;
	msg.msg_namelen = omsg.msg_namelen;
	error = compat43_set_accrights(&msg, omsg.msg_accrights,
	    omsg.msg_accrightslen);
	if (error != 0)
		goto bad;

	return do_sys_sendmsg(l, SCARG(uap, s), &msg, SCARG(uap, flags), retval);

    bad:
	if (nam != NULL)
		m_free(nam);

	return (error);
}

static int
compat_43_sa_put(void *from)
{
	struct osockaddr *osa = (struct osockaddr *) from;
	struct sockaddr sa;
	struct osockaddr *kosa;
	int error, len;

	/*
	 * Only read/write the sockaddr family and length, the rest is
	 * not changed.
	 */
	len = sizeof(sa.sa_len) + sizeof(sa.sa_family);

	error = copyin((void *) osa, (void *) &sa, len);
	if (error)
		return (error);

	/* Note: we convert from sockaddr sa_family to osockaddr one here */
	kosa = (struct osockaddr *) &sa;
	kosa->sa_family = sa.sa_family;
	error = copyout(kosa, osa, len);
	if (error)
		return (error);

	return (0);
}
#endif

u_long 
compat_cvtcmd(u_long cmd)
{ 
	u_long ncmd;

	if (IOCPARM_LEN(cmd) != sizeof(struct oifreq))
		return cmd;

	ncmd = ((cmd) & ~(IOCPARM_MASK << IOCPARM_SHIFT)) | 
		(sizeof(struct ifreq) << IOCPARM_SHIFT);

	switch (ncmd) {
	case BIOCGETIF:
	case BIOCSETIF:
#ifndef T2EX
	case GREDSOCK:
	case GREGADDRD:
	case GREGADDRS:
	case GREGPROTO:
	case GRESADDRD:
	case GRESADDRS:
	case GRESPROTO:
	case GRESSOCK:
#endif
#ifdef COMPAT_20
	case OSIOCG80211STATS:
	case OSIOCG80211ZSTATS:
#endif /* COMPAT_20 */
	case SIOCADDMULTI:
	case SIOCDELMULTI:
	case SIOCDIFADDR:
	case SIOCDIFADDR_IN6:
	case SIOCDIFPHYADDR:
#ifndef T2EX
	case SIOCGDEFIFACE_IN6:
#endif
	case SIOCG80211NWID:
	case SIOCG80211STATS:
#ifndef T2EX
	case SIOCG80211ZSTATS:
#endif
	case SIOCGIFADDR:
	case SIOCGIFADDR_IN6:
	case SIOCGIFAFLAG_IN6:
	case SIOCGIFALIFETIME_IN6:
	case SIOCGIFBRDADDR:
	case SIOCGIFDLT:
	case SIOCGIFDSTADDR:
	case SIOCGIFDSTADDR_IN6:
	case SIOCGIFFLAGS:
	case SIOCGIFGENERIC:
	case SIOCGIFMETRIC:
	case SIOCGIFMTU:
	case SIOCGIFNETMASK:
	case SIOCGIFNETMASK_IN6:
	case SIOCGIFPDSTADDR:
	case SIOCGIFPDSTADDR_IN6:
	case SIOCGIFPSRCADDR:
	case SIOCGIFPSRCADDR_IN6:
	case SIOCGIFSTAT_ICMP6:
	case SIOCGIFSTAT_IN6:
#ifndef T2EX
	case SIOCGPVCSIF:
#endif
	case SIOCGVH:
	case SIOCIFCREATE:
	case SIOCIFDESTROY:
	case SIOCS80211NWID:
#ifndef T2EX
	case SIOCSDEFIFACE_IN6:
#endif
	case SIOCSIFADDR:
	case SIOCSIFADDR_IN6:
	case SIOCSIFBRDADDR:
	case SIOCSIFDSTADDR:
	case SIOCSIFDSTADDR_IN6:
	case SIOCSIFFLAGS:
	case SIOCSIFGENERIC:
	case SIOCSIFMEDIA:
	case SIOCSIFMETRIC:
	case SIOCSIFMTU:
	case SIOCSIFNETMASK:
	case SIOCSIFNETMASK_IN6:
	case SIOCSNDFLUSH_IN6:
	case SIOCSPFXFLUSH_IN6:
#ifndef T2EX
	case SIOCSPVCSIF:
#endif
	case SIOCSRTRFLUSH_IN6:
	case SIOCSVH:
#ifndef T2EX
	case TAPGIFNAME:
#endif
		return ncmd;
	}
	return cmd;
}

int
compat_ifioctl(struct socket *so, u_long ocmd, u_long cmd, void *data,
    struct lwp *l)
{
	int error;
	struct ifreq *ifr = data;
	struct ifnet *ifp = ifunit(ifr->ifr_name);
	struct sockaddr *sa;

	if (ifp == NULL)
		return ENXIO;

	switch (ocmd) {
	case OSIOCSIFADDR:
	case OSIOCSIFDSTADDR:
	case OSIOCSIFBRDADDR:
	case OSIOCSIFNETMASK:
		sa = &ifr->ifr_addr;
#if BYTE_ORDER != BIG_ENDIAN
		if (sa->sa_family == 0 && sa->sa_len < 16) {
			sa->sa_family = sa->sa_len;
			sa->sa_len = 16;
		}
#else
		if (sa->sa_len == 0)
			sa->sa_len = 16;
#endif
		break;

	case OOSIOCGIFADDR:
		cmd = SIOCGIFADDR;
		break;

	case OOSIOCGIFDSTADDR:
		cmd = SIOCGIFDSTADDR;
		break;

	case OOSIOCGIFBRDADDR:
		cmd = SIOCGIFBRDADDR;
		break;

	case OOSIOCGIFNETMASK:
		cmd = SIOCGIFNETMASK;
	}

	error = (*so->so_proto->pr_usrreq)(so, PRU_CONTROL,
	    (struct mbuf *)cmd, (struct mbuf *)ifr, (struct mbuf *)ifp, l);

	switch (ocmd) {
	case OOSIOCGIFADDR:
	case OOSIOCGIFDSTADDR:
	case OOSIOCGIFBRDADDR:
	case OOSIOCGIFNETMASK:
		*(u_int16_t *)&ifr->ifr_addr = 
		    ((struct sockaddr *)&ifr->ifr_addr)->sa_family;
	}
	return error;
}
