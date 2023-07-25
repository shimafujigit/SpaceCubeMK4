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

/*	$NetBSD: if_altq.h,v 1.12 2007/03/04 05:59:03 christos Exp $	*/
/*	$KAME: if_altq.h,v 1.12 2005/04/13 03:44:25 suz Exp $	*/

/*
 * Copyright (C) 1997-2003
 *	Sony Computer Science Laboratories Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY SONY CSL AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL SONY CSL OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _ALTQ_IF_ALTQ_H_
#define	_ALTQ_IF_ALTQ_H_

#if defined(_KERNEL_OPT)
#include "opt_altq_enabled.h"
#endif

struct altq_pktattr; struct tb_regulator; struct top_cdnr;

/*
 * Structure defining a queue for a network interface.
 */
struct	ifaltq {
	/* fields compatible with struct ifqueue */
	struct	mbuf *ifq_head;
	struct	mbuf *ifq_tail;
	int	ifq_len;
	int	ifq_maxlen;
	int	ifq_drops;
#ifdef __FreeBSD__
	struct	mtx ifq_mtx;
#endif

	/* alternate queueing related fields */
	int	altq_type;		/* discipline type */
	int	altq_flags;		/* flags (e.g. ready, in-use) */
	void	*altq_disc;		/* for discipline-specific use */
	struct	ifnet *altq_ifp;	/* back pointer to interface */

	int	(*altq_enqueue)(struct ifaltq *, struct mbuf *,
				struct altq_pktattr *);
	struct	mbuf *(*altq_dequeue)(struct ifaltq *, int);
	int	(*altq_request)(struct ifaltq *, int, void *);

	/* classifier fields */
	void	*altq_clfier;		/* classifier-specific use */
	void	*(*altq_classify)(void *, struct mbuf *, int);

	/* token bucket regulator */
	struct	tb_regulator *altq_tbr;

	/* input traffic conditioner (doesn't belong to the output queue...) */
	struct top_cdnr *altq_cdnr;
};


#ifdef _KERNEL

/*
 * packet attributes used by queueing disciplines.
 * pattr_class is a discipline-dependent scheduling class that is
 * set by a classifier.
 * pattr_hdr and pattr_af may be used by a discipline to access
 * the header within a mbuf.  (e.g. ECN needs to update the CE bit)
 * note that pattr_hdr could be stale after m_pullup, though link
 * layer output routines usually don't use m_pullup.  link-level
 * compression also invalidates these fields.  thus, pattr_hdr needs
 * to be verified when a discipline touches the header.
 */
struct altq_pktattr {
	void	*pattr_class;		/* sched class set by classifier */
	int	pattr_af;		/* address family */
	void *	pattr_hdr;		/* saved header position in mbuf */
};

/*
 * mbuf tag to carry a queue id (and hints for ECN).
 */
struct altq_tag {
	u_int32_t	qid;		/* queue id */
	/* hints for ecn */
	int		af;		/* address family */
	void		*hdr;		/* saved header position in mbuf */
};

/*
 * a token-bucket regulator limits the rate that a network driver can
 * dequeue packets from the output queue.
 * modern cards are able to buffer a large amount of packets and dequeue
 * too many packets at a time.  this bursty dequeue behavior makes it
 * impossible to schedule packets by queueing disciplines.
 * a token-bucket is used to control the burst size in a device
 * independent manner.
 */
struct tb_regulator {
	int64_t		tbr_rate;	/* (scaled) token bucket rate */
	int64_t		tbr_depth;	/* (scaled) token bucket depth */

	int64_t		tbr_token;	/* (scaled) current token */
	int64_t		tbr_filluptime;	/* (scaled) time to fill up bucket */
	u_int64_t	tbr_last;	/* last time token was updated */

	int		tbr_lastop;	/* last dequeue operation type
					   needed for poll-and-dequeue */
};

/* if_altqflags */
#define	ALTQF_READY	 0x01	/* driver supports alternate queueing */
#define	ALTQF_ENABLED	 0x02	/* altq is in use */
#define	ALTQF_CLASSIFY	 0x04	/* classify packets */
#define	ALTQF_CNDTNING	 0x08	/* altq traffic conditioning is enabled */
#define	ALTQF_DRIVER1	 0x40	/* driver specific */

/* if_altqflags set internally only: */
#define	ALTQF_CANTCHANGE 	(ALTQF_READY)

/* altq_dequeue 2nd arg */
#define	ALTDQ_REMOVE		1	/* dequeue mbuf from the queue */
#define	ALTDQ_POLL		2	/* don't dequeue mbuf from the queue */

/* altq request types (currently only purge is defined) */
#define	ALTRQ_PURGE		1	/* purge all packets */

#define	ALTQ_IS_READY(ifq)		((ifq)->altq_flags & ALTQF_READY)
#define	ALTQ_IS_ENABLED(ifq)		((ifq)->altq_flags & ALTQF_ENABLED)
#define	ALTQ_NEEDS_CLASSIFY(ifq)	((ifq)->altq_flags & ALTQF_CLASSIFY)
#define	ALTQ_IS_CNDTNING(ifq)		((ifq)->altq_flags & ALTQF_CNDTNING)

#define	ALTQ_SET_CNDTNING(ifq)		((ifq)->altq_flags |= ALTQF_CNDTNING)
#define	ALTQ_CLEAR_CNDTNING(ifq)	((ifq)->altq_flags &= ~ALTQF_CNDTNING)
#define	ALTQ_IS_ATTACHED(ifq)		((ifq)->altq_disc != NULL)

#define	ALTQ_ENQUEUE(ifq, m, pa, err)					\
	(err) = (*(ifq)->altq_enqueue)((ifq),(m),(pa))
#define	ALTQ_DEQUEUE(ifq, m)						\
	(m) = (*(ifq)->altq_dequeue)((ifq), ALTDQ_REMOVE)
#define	ALTQ_POLL(ifq, m)						\
	(m) = (*(ifq)->altq_dequeue)((ifq), ALTDQ_POLL)
#define	ALTQ_PURGE(ifq)							\
	(void)(*(ifq)->altq_request)((ifq), ALTRQ_PURGE, (void *)0)
#define	ALTQ_IS_EMPTY(ifq)		((ifq)->ifq_len == 0)
#define	TBR_IS_ENABLED(ifq)		((ifq)->altq_tbr != NULL)

extern int altq_attach(struct ifaltq *, int, void *,
		       int (*)(struct ifaltq *, struct mbuf *,
			       struct altq_pktattr *),
		       struct mbuf *(*)(struct ifaltq *, int),
		       int (*)(struct ifaltq *, int, void *),
		       void *,
		       void *(*)(void *, struct mbuf *, int));
extern int altq_detach(struct ifaltq *);
extern int altq_enable(struct ifaltq *);
extern int altq_disable(struct ifaltq *);
extern struct mbuf *tbr_dequeue(struct ifaltq *, int);
extern int (*altq_input)(struct mbuf *, int);
#if 1 /* ALTQ3_CLFIER_COMPAT */
void altq_etherclassify(struct ifaltq *, struct mbuf *, struct altq_pktattr *);
#endif
#endif /* _KERNEL */

#endif /* _ALTQ_IF_ALTQ_H_ */
