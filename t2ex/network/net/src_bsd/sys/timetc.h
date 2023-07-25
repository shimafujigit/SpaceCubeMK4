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

/* $NetBSD: timetc.h,v 1.5 2008/05/08 18:56:58 ad Exp $ */

/*-
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 *
 * $FreeBSD: src/sys/sys/timetc.h,v 1.58 2003/08/16 08:23:52 phk Exp $
 */

#ifndef _SYS_TIMETC_H_
#define	_SYS_TIMETC_H_

#ifndef _KERNEL
#error "no user-serviceable parts inside"
#endif

/*
 * max recommended timecounter name length
 *
 * it is not a functional limit but names longer
 * then that will not be controllable via
 * sysctl. see kern/kern_tc.c for the sysctl
 * implementation.
 */
#define MAX_TCNAMELEN	64

/*-
 * `struct timecounter' is the interface between the hardware which implements
 * a timecounter and the MI code which uses this to keep track of time.
 *
 * A timecounter is a binary counter which has two properties:
 *    * it runs at a fixed, known frequency.
 *    * it has sufficient bits to not roll over in less than approximately
 *      max(2 msec, 2/HZ seconds).  (The value 2 here is really 1 + delta,
 *      for some indeterminate value of delta.)
 */

struct timecounter;
typedef u_int timecounter_get_t(struct timecounter *);
typedef void timecounter_pps_t(struct timecounter *);

struct timecounter {
	timecounter_get_t	*tc_get_timecount;
		/*
		 * This function reads the counter.  It is not required to
		 * mask any unimplemented bits out, as long as they are
		 * constant.
		 */
	timecounter_pps_t	*tc_poll_pps;
		/*
		 * This function is optional.  It will be called whenever the
		 * timecounter is rewound, and is intended to check for PPS
		 * events.  Normal hardware does not need it but timecounters
		 * which latch PPS in hardware (like sys/pci/xrpu.c) do.
		 */
	u_int 			tc_counter_mask;
		/* This mask should mask off any unimplemented bits. */
	u_int64_t		tc_frequency;
		/* Frequency of the counter in Hz. */
	const char		*tc_name;
		/* Name of the timecounter. */
	int			tc_quality;
		/*
		 * Used to determine if this timecounter is better than
		 * another timecounter higher means better.  Negative
		 * means "only use at explicit request".
		 */

	void			*tc_priv;
		/* Pointer to the timecounter's private parts. */
	struct timecounter	*tc_next;
		/* Pointer to the next timecounter. */
};

extern struct timecounter *timecounter;

u_int64_t tc_getfrequency(void);
void	tc_init(struct timecounter *tc);
int	tc_detach(struct timecounter *);
#ifndef T2EX
void	tc_setclock(struct timespec *ts);
#else
void	tkn_tc_setclock(struct timespec *ts);
#define	tc_setclock	tkn_tc_setclock
#endif
void	tc_ticktock(void);
void	tc_gonebad(struct timecounter *);

#ifdef SYSCTL_DECL
SYSCTL_DECL(_kern_timecounter);
#endif

#endif /* !_SYS_TIMETC_H_ */
