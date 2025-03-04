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

/*	$NetBSD: sys_socket.c,v 1.58.10.2 2009/04/08 23:10:13 snj Exp $	*/

/*-
 * Copyright (c) 2008, 2009 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Andrew Doran.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 1982, 1986, 1990, 1993
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
 *	@(#)sys_socket.c	8.3 (Berkeley) 2/14/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sys_socket.c,v 1.58.10.2 2009/04/08 23:10:13 snj Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/systm.h>
#include <sys/file.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/proc.h>
#ifndef T2EX
#include <sys/kauth.h>
#endif

#include <net/if.h>
#include <net/route.h>

const struct fileops socketops = {
	.fo_read = soo_read,
	.fo_write = soo_write,
	.fo_ioctl = soo_ioctl,
	.fo_fcntl = soo_fcntl,
	.fo_poll = soo_poll,
	.fo_stat = soo_stat,
	.fo_close = soo_close,
	.fo_kqfilter = soo_kqfilter,
	.fo_drain = fnullop_drain, 	/* soo_drain, */
};

/* ARGSUSED */
int
soo_read(file_t *fp, off_t *offset, struct uio *uio, kauth_cred_t cred,
	 int flags)
{
	struct socket *so = fp->f_data;
	int error;

	error = (*so->so_receive)(so, (struct mbuf **)0,
	    uio, (struct mbuf **)0, (struct mbuf **)0, (int *)0);

	return error;
}

/* ARGSUSED */
int
soo_write(file_t *fp, off_t *offset, struct uio *uio, kauth_cred_t cred,
	  int flags)
{
	struct socket *so = fp->f_data;
	int error;

	error = (*so->so_send)(so, (struct mbuf *)0,
		uio, (struct mbuf *)0, (struct mbuf *)0, 0, curlwp);

	return error;
}

int
soo_ioctl(file_t *fp, u_long cmd, void *data)
{
	struct socket *so = fp->f_data;
	int error = 0;

	if (cmd == FIONBIO) {
		so->so_nbio = *(int *)data;
		return 0;
	}

	switch (cmd) {

	case FIOASYNC:
		solock(so);
		if (*(int *)data) {
			so->so_state |= SS_ASYNC;
			so->so_rcv.sb_flags |= SB_ASYNC;
			so->so_snd.sb_flags |= SB_ASYNC;
		} else {
			so->so_state &= ~SS_ASYNC;
			so->so_rcv.sb_flags &= ~SB_ASYNC;
			so->so_snd.sb_flags &= ~SB_ASYNC;
		}
		sounlock(so);
		break;

	case FIONREAD:
		*(int *)data = so->so_rcv.sb_cc;
		break;

	case FIONWRITE:
		*(int *)data = so->so_snd.sb_cc;
		break;

	case FIONSPACE:
		/*
		 * See the comment around sbspace()'s definition
		 * in sys/socketvar.h in face of counts about maximum
		 * to understand the following test. We detect overflow
		 * and return zero.
		 */
		solock(so);
		if ((so->so_snd.sb_hiwat < so->so_snd.sb_cc)
		    || (so->so_snd.sb_mbmax < so->so_snd.sb_mbcnt))
			*(int *)data = 0;
		else
			*(int *)data = sbspace(&so->so_snd);
		sounlock(so);
		break;

#ifndef T2EX /* ignore process group */
	case SIOCSPGRP:
	case FIOSETOWN:
	case TIOCSPGRP:
		error = fsetown(&so->so_pgid, cmd, data);
		break;

	case SIOCGPGRP:
	case FIOGETOWN:
	case TIOCGPGRP:
		error = fgetown(so->so_pgid, cmd, data);
		break;
#endif

	case SIOCATMARK:
		*(int *)data = (so->so_state&SS_RCVATMARK) != 0;
		break;

	default:
		/*
		 * Interface/routing/protocol specific ioctls:
		 * interface and routing ioctls should have a
		 * different entry since a socket's unnecessary
		 */
		KERNEL_LOCK(1, NULL);
		if (IOCGROUP(cmd) == 'i')
			error = ifioctl(so, cmd, data, curlwp);
		else if (IOCGROUP(cmd) == 'r')
			error = rtioctl(cmd, data, curlwp);
		else {
			error = (*so->so_proto->pr_usrreq)(so, PRU_CONTROL,
			    (struct mbuf *)cmd, (struct mbuf *)data, NULL,
			     curlwp);
		}
		KERNEL_UNLOCK_ONE(NULL);
		break;
	}


	return error;
}

int
soo_fcntl(file_t *fp, u_int cmd, void *data)
{

	if (cmd == F_SETFL)
		return 0;
	else
		return EOPNOTSUPP;
}

int
soo_poll(file_t *fp, int events)
{

	return sopoll(fp->f_data, events);
}

int
soo_stat(file_t *fp, struct stat *ub)
{
	struct socket *so = fp->f_data;
	int error;

	memset((void *)ub, 0, sizeof(*ub));
	ub->st_mode = S_IFSOCK;

	solock(so);
	error = (*so->so_proto->pr_usrreq)(so, PRU_SENSE,
	    (struct mbuf *)ub, (struct mbuf *)0, (struct mbuf *)0,
	    curlwp);
	sounlock(so);

	return error;
}

/* ARGSUSED */
int
soo_close(file_t *fp)
{
	int error = 0;

	if (fp->f_data)
		error = soclose(fp->f_data);
	fp->f_data = 0;

	return error;
}

#ifndef T2EX
void
soo_drain(file_t *fp)
{

	(void)sodrain(fp->f_data);
}
#endif

#ifdef T2EX
int
soo_kqfilter(struct file *fp, struct knote *knote)
{
	return EOPNOTSUPP;
}
#endif
