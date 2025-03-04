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

/*	$NetBSD: raw_usrreq.c,v 1.35.22.1 2011/01/16 13:04:45 bouyer Exp $	*/

/*
 * Copyright (c) 1980, 1986, 1993
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
 *	@(#)raw_usrreq.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: raw_usrreq.c,v 1.35.22.1 2011/01/16 13:04:45 bouyer Exp $");

#include <sys/param.h>
#include <sys/mbuf.h>
#include <sys/domain.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/errno.h>
#include <sys/systm.h>
#include <sys/proc.h>
#ifndef T2EX
#include <sys/kauth.h>
#endif

#include <net/if.h>
#include <net/route.h>
#include <net/netisr.h>
#include <net/raw_cb.h>

#include <machine/stdarg.h>

/*
 * Initialize raw connection block q.
 */
#ifndef T2EX
void
raw_init(void)
{

	LIST_INIT(&rawcb);
}
#else
int
raw_init(void)
{

	LIST_INIT(&rawcb);

	return 0;
}

void
raw_finish(void)
{

}
#endif

static inline int
equal(const struct sockaddr *a1, const struct sockaddr *a2)
{
	return memcmp(a1, a2, a1->sa_len) == 0;
}

/*
 * Raw protocol input routine.  Find the socket
 * associated with the packet(s) and move them over.  If
 * nothing exists for this packet, drop it.
 */
/*
 * Raw protocol interface.
 */
void
raw_input(struct mbuf *m0, ...)
{
	struct rawcb *rp;
	struct mbuf *m = m0;
	struct socket *last;
	va_list ap;
	struct sockproto *proto;
	struct sockaddr *src, *dst;

	KASSERT(mutex_owned(softnet_lock));

	va_start(ap, m0);
	proto = va_arg(ap, struct sockproto *);
	src = va_arg(ap, struct sockaddr *);
	dst = va_arg(ap, struct sockaddr *);
	va_end(ap);

	last = NULL;
	LIST_FOREACH(rp, &rawcb, rcb_list) {
		if (rp->rcb_proto.sp_family != proto->sp_family)
			continue;
		if (rp->rcb_proto.sp_protocol  &&
		    rp->rcb_proto.sp_protocol != proto->sp_protocol)
			continue;
		/*
		 * We assume the lower level routines have
		 * placed the address in a canonical format
		 * suitable for a structure comparison.
		 *
		 * Note that if the lengths are not the same
		 * the comparison will fail at the first byte.
		 */
		if (rp->rcb_laddr && !equal(rp->rcb_laddr, dst))
			continue;
		if (rp->rcb_faddr && !equal(rp->rcb_faddr, src))
			continue;
		if (last != NULL) {
			struct mbuf *n;
			if ((n = m_copy(m, 0, M_COPYALL)) == NULL)
				;
			else if (sbappendaddr(&last->so_rcv, src, n, NULL) == 0)
				/* should notify about lost packet */
				m_freem(n);
			else {
				sorwakeup(last);
			}
		}
		last = rp->rcb_socket;
	}
	if (last == NULL || sbappendaddr(&last->so_rcv, src, m, NULL) == 0)
		m_freem(m);
	else {
		sorwakeup(last);
	}
}

/*ARGSUSED*/
void *
raw_ctlinput(int cmd, const struct sockaddr *arg, void *d)
{

	if ((unsigned)cmd >= PRC_NCMDS)
		return NULL;
	return NULL;
	/* INCOMPLETE */
}

void
raw_setsockaddr(struct rawcb *rp, struct mbuf *nam)
{

	nam->m_len = rp->rcb_laddr->sa_len;
	memcpy(mtod(nam, void *), rp->rcb_laddr, (size_t)nam->m_len);
}

void
raw_setpeeraddr(struct rawcb *rp, struct mbuf *nam)
{

	nam->m_len = rp->rcb_faddr->sa_len;
	memcpy(mtod(nam, void *), rp->rcb_faddr, (size_t)nam->m_len);
}

/*ARGSUSED*/
int
raw_usrreq(struct socket *so, int req, struct mbuf *m, struct mbuf *nam,
    struct mbuf *control, struct lwp *l)
{
	struct rawcb *rp;
	int s;
	int error = 0;

	if (req == PRU_CONTROL)
		return (EOPNOTSUPP);

	s = splsoftnet();
	KERNEL_LOCK(1, NULL);
	rp = sotorawcb(so);
#ifdef DIAGNOSTIC
	if (req != PRU_SEND && req != PRU_SENDOOB && control)
		panic("raw_usrreq: unexpected control mbuf");
#endif
	if (rp == NULL && req != PRU_ATTACH) {
		error = EINVAL;
		goto release;
	}

	switch (req) {

	/*
	 * Allocate a raw control block and fill in the
	 * necessary info to allow packets to be routed to
	 * the appropriate raw interface routine.
	 */
	case PRU_ATTACH:
		sosetlock(so);
		if (l == NULL)
			break;

		/* XXX: raw socket permissions are checked in socreate() */

		error = raw_attach(so, (int)(long)nam);
		break;

	/*
	 * Destroy state just before socket deallocation.
	 * Flush data or not depending on the options.
	 */
	case PRU_DETACH:
		raw_detach(rp);
		break;

	/*
	 * If a socket isn't bound to a single address,
	 * the raw input routine will hand it anything
	 * within that protocol family (assuming there's
	 * nothing else around it should go to).
	 */
	case PRU_BIND:
	case PRU_LISTEN:
	case PRU_CONNECT:
	case PRU_CONNECT2:
		error = EOPNOTSUPP;
		break;

	case PRU_DISCONNECT:
		soisdisconnected(so);
		raw_disconnect(rp);
		break;

	/*
	 * Mark the connection as being incapable of further input.
	 */
	case PRU_SHUTDOWN:
		socantsendmore(so);
		break;

	case PRU_RCVD:
		error = EOPNOTSUPP;
		break;

	/*
	 * Ship a packet out.  The appropriate raw output
	 * routine handles any massaging necessary.
	 */
	case PRU_SEND:
		if (control && control->m_len) {
			m_freem(control);
			m_freem(m);
			error = EINVAL;
			break;
		}
		if (nam) {
			if ((so->so_state & SS_ISCONNECTED) != 0) {
				error = EISCONN;
				goto die;
			}
			error = (*so->so_proto->pr_usrreq)(so, PRU_CONNECT,
			    NULL, nam, NULL, l);
			if (error) {
			die:
				m_freem(m);
				break;
			}
		} else {
			if ((so->so_state & SS_ISCONNECTED) == 0) {
				error = ENOTCONN;
				goto die;
			}
		}
		error = (*so->so_proto->pr_output)(m, so);
		if (nam)
			raw_disconnect(rp);
		break;

	case PRU_SENSE:
		/*
		 * stat: don't bother with a blocksize.
		 */
		error = 0;
		break;

	/*
	 * Not supported.
	 */
	case PRU_RCVOOB:
		error = EOPNOTSUPP;
		break;

	case PRU_SENDOOB:
		m_freem(control);
		m_freem(m);
		error = EOPNOTSUPP;
		break;

	case PRU_SOCKADDR:
		if (rp->rcb_laddr == NULL) {
			error = EINVAL;
			break;
		}
		raw_setsockaddr(rp, nam);
		break;

	case PRU_PEERADDR:
		if (rp->rcb_faddr == NULL) {
			error = ENOTCONN;
			break;
		}
		raw_setpeeraddr(rp, nam);
		break;

	default:
		panic("raw_usrreq");
	}

release:
	KERNEL_UNLOCK_ONE(NULL);
	splx(s);
	return (error);
}
