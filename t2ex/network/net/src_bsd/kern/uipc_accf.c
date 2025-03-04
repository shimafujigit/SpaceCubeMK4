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

/*	$NetBSD: uipc_accf.c,v 1.6 2008/10/15 08:25:28 ad Exp $	*/

/*-
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software developed for The NetBSD Foundation
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

/*-
 * Copyright (c) 2000 Paycounter, Inc.
 * Copyright (c) 2005 Robert N. M. Watson
 * Author: Alfred Perlstein <alfred@paycounter.com>, <alfred@FreeBSD.org>
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: uipc_accf.c,v 1.6 2008/10/15 08:25:28 ad Exp $");

#define ACCEPT_FILTER_MOD

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/domain.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/kmem.h>
#include <sys/mbuf.h>
#ifndef T2EX
#include <sys/lkm.h>
#endif
#include <sys/rwlock.h>
#include <sys/protosw.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#ifndef T2EX
#include <sys/queue.h>
#include <sys/once.h>
#include <sys/atomic.h>
#else
#include <sys/_queue.h>
#include <sys/_atomic.h>
#endif

static krwlock_t accept_filter_lock;

static LIST_HEAD(, accept_filter) accept_filtlsthd =
    LIST_HEAD_INITIALIZER(&accept_filtlsthd);

#ifndef T2EX
/*
 * Names of Accept filter sysctl objects
 */
SYSCTL_SETUP(sysctl_net_inet_accf_setup, "sysctl net.inet.accf subtree setup")
{

	sysctl_createv(clog, 0, NULL, NULL,
		       CTLFLAG_PERMANENT,
		       CTLTYPE_NODE, "net", NULL,
		       NULL, 0, NULL, 0,
		       CTL_NET, CTL_EOL);
	sysctl_createv(clog, 0, NULL, NULL,
		       CTLFLAG_PERMANENT,
		       CTLTYPE_NODE, "inet", NULL,
		       NULL, 0, NULL, 0,
		       CTL_NET, PF_INET, CTL_EOL);
	sysctl_createv(clog, 0, NULL, NULL,
		       CTLFLAG_PERMANENT,
		       CTLTYPE_NODE, "accf",
		       SYSCTL_DESCR("Accept filters"),
		       NULL, 0, NULL, 0,
		       CTL_NET, PF_INET, SO_ACCEPTFILTER, CTL_EOL);
}
#endif

int
accept_filt_add(struct accept_filter *filt)
{
	struct accept_filter *p;

	rw_enter(&accept_filter_lock, RW_WRITER);
	LIST_FOREACH(p, &accept_filtlsthd, accf_next) {
		if (strcmp(p->accf_name, filt->accf_name) == 0)  {
			rw_exit(&accept_filter_lock);
			return EEXIST;
		}
	}				
	LIST_INSERT_HEAD(&accept_filtlsthd, filt, accf_next);
	rw_exit(&accept_filter_lock);

	return 0;
}

int
accept_filt_del(struct accept_filter *p)
{

	rw_enter(&accept_filter_lock, RW_WRITER);
	if (p->accf_refcnt != 0) {
		rw_exit(&accept_filter_lock);
		return EBUSY;
	}
	LIST_REMOVE(p, accf_next);
	rw_exit(&accept_filter_lock);

	return 0;
}

struct accept_filter *
accept_filt_get(char *name)
{
	struct accept_filter *p;

	rw_enter(&accept_filter_lock, RW_READER);
	LIST_FOREACH(p, &accept_filtlsthd, accf_next) {
		if (strcmp(p->accf_name, name) == 0) {
			atomic_inc_uint(&p->accf_refcnt);
			break;
		}
	}
	rw_exit(&accept_filter_lock);

	return p;
}

#ifndef T2EX
/*
 * Accept filter initialization routine.
 * This should be called only once.
 */

static int
accept_filter_init0(void)
{

	rw_init(&accept_filter_lock);

	return 0;
}

/*
 * Initialization routine: This can also be replaced with 
 * accept_filt_generic_mod_event for attaching new accept filter.
 */

void
accept_filter_init(void)
{
	static ONCE_DECL(accept_filter_init_once);

	RUN_ONCE(&accept_filter_init_once, accept_filter_init0);
}

int
accept_filt_generic_mod_event(struct lkm_table *lkmtp, int event, void *data)
{
	struct accept_filter *accfp = (struct accept_filter *) data;
	int error;

	switch (event) {
	case LKM_E_LOAD:
		accept_filter_init();
		error = accept_filt_add(accfp);
		break;

	case LKM_E_UNLOAD:
		error = accept_filt_del(accfp);
		break;

	case LKM_E_STAT:
		error = 0;
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	return error;
}
#endif

int
accept_filt_getopt(struct socket *so, struct sockopt *sopt)
{
	struct accept_filter_arg afa;
	int error;

	KASSERT(solocked(so));

	if ((so->so_options & SO_ACCEPTCONN) == 0) {
		error = EINVAL;
		goto out;
	}
	if ((so->so_options & SO_ACCEPTFILTER) == 0) {
		error = EINVAL;
		goto out;
	}

	memset(&afa, 0, sizeof(afa));
	strcpy(afa.af_name, so->so_accf->so_accept_filter->accf_name);
	if (so->so_accf->so_accept_filter_str != NULL)
		strcpy(afa.af_arg, so->so_accf->so_accept_filter_str);
	error = sockopt_set(sopt, &afa, sizeof(afa));
out:
	return error;
}

/*
 * Simple delete case, with socket locked.
 */
int
accept_filt_clear(struct socket *so)
{
	struct accept_filter_arg afa;
	struct accept_filter *afp;
	struct socket *so2, *next;
	struct so_accf *af;

	KASSERT(solocked(so));

	if ((so->so_options & SO_ACCEPTCONN) == 0) {
		return EINVAL;
	}
	if (so->so_accf != NULL) {
		/* Break in-flight processing. */
		for (so2 = TAILQ_FIRST(&so->so_q0); so2 != NULL; so2 = next) {
			next = TAILQ_NEXT(so2, so_qe);
			if (so2->so_upcall == NULL) {
				continue;
			}
			so2->so_upcall = NULL;
			so2->so_upcallarg = NULL;
			so2->so_options &= ~SO_ACCEPTFILTER;
			so2->so_rcv.sb_flags &= ~SB_UPCALL;
			soisconnected(so2);
		}
		af = so->so_accf;
		afp = af->so_accept_filter;
		if (afp != NULL && afp->accf_destroy != NULL) {
			(*afp->accf_destroy)(so);
		}
		if (af->so_accept_filter_str != NULL) {
			kmem_free(af->so_accept_filter_str,
			    sizeof(afa.af_name));
		}
		kmem_free(af, sizeof(*af));
		so->so_accf = NULL;
		atomic_dec_uint(&afp->accf_refcnt);
	}
	so->so_options &= ~SO_ACCEPTFILTER;
	return 0;
}

/*
 * setsockopt() for accept filters.  Called with the socket unlocked,
 * will always return it locked.
 */
int
accept_filt_setopt(struct socket *so, const struct sockopt *sopt)
{
	struct accept_filter_arg afa;
	struct accept_filter *afp;
	struct so_accf *newaf;
	int error;

	if (sopt == NULL || sopt->sopt_size == 0) {
		solock(so);
		return accept_filt_clear(so);
	}

	/*
	 * Pre-allocate any memory we may need later to avoid blocking at
	 * untimely moments.  This does not optimize for invalid arguments.
	 */
	error = sockopt_get(sopt, &afa, sizeof(afa));
	if (error) {
		solock(so);
		return error;
	}
	afa.af_name[sizeof(afa.af_name)-1] = '\0';
	afa.af_arg[sizeof(afa.af_arg)-1] = '\0';
	afp = accept_filt_get(afa.af_name);
	if (afp == NULL) {
		solock(so);
		return ENOENT;
	}
	/*
	 * Allocate the new accept filter instance storage.  We may
	 * have to free it again later if we fail to attach it.  If
	 * attached properly, 'newaf' is NULLed to avoid a free()
	 * while in use.
	 */
	newaf = kmem_zalloc(sizeof(*newaf), KM_SLEEP);
#ifdef T2EX
	if ( newaf == NULL ) {
		error = ENOMEM;
		goto out;
	}
#endif
	if (afp->accf_create != NULL && afa.af_name[0] != '\0') {
		/*
		 * FreeBSD did a variable-size allocation here
		 * with the actual string length from afa.af_name
		 * but it is so short, why bother tracking it?
		 * XXX as others have noted, this is an API mistake;
		 * XXX accept_filter_arg should have a mandatory namelen.
		 * XXX (but it's a bit too late to fix that now)
		 */
		newaf->so_accept_filter_str =
		    kmem_alloc(sizeof(afa.af_name), KM_SLEEP);
#ifdef T2EX
		if ( newaf->so_accept_filter_str == NULL ) {
			error = ENOMEM;
			goto out;
		}
#endif
		strcpy(newaf->so_accept_filter_str, afa.af_name);
	}

	/*
	 * Require a listen socket; don't try to replace an existing filter
	 * without first removing it.
	 */
	solock(so);
	if ((so->so_options & SO_ACCEPTCONN) == 0 || so->so_accf != NULL) {
		error = EINVAL;
		goto out;
	}

	/*
	 * Invoke the accf_create() method of the filter if required.  The
	 * socket lock is held over this call, so create methods for filters
	 * shouldn't block.
	 */
	if (afp->accf_create != NULL) {
		newaf->so_accept_filter_arg =
		    (*afp->accf_create)(so, afa.af_arg);
		if (newaf->so_accept_filter_arg == NULL) {
			error = EINVAL;
			goto out;
		}
	}
	newaf->so_accept_filter = afp;
	so->so_accf = newaf;
	so->so_options |= SO_ACCEPTFILTER;
	newaf = NULL;
out:
	if (newaf != NULL) {
		if (newaf->so_accept_filter_str != NULL)
			kmem_free(newaf->so_accept_filter_str,
			    sizeof(afa.af_name));
		kmem_free(newaf, sizeof(*newaf));
		atomic_dec_uint(&afp->accf_refcnt);
	}
	return error;
}
