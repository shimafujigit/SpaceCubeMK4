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

/*	$NetBSD: conf.h,v 1.131.8.1 2009/06/23 06:56:51 snj Exp $	*/

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 *	@(#)conf.h	8.5 (Berkeley) 1/9/95
 */

#ifndef _SYS_CONF_H_
#define _SYS_CONF_H_

/*
 * Definitions of device driver entry switches
 */

#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif

struct buf;
struct knote;
struct lwp;
struct tty;
struct uio;
struct vnode;

/*
 * Types for d_type
 */
#define D_OTHER		0x0000
#define	D_TAPE		0x0001
#define	D_DISK		0x0002
#define	D_TTY		0x0003
#define	D_TYPEMASK	0x00ff
#define	D_MPSAFE	0x0100
#define	D_NEGOFFSAFE	0x0200

/*
 * Block device switch table
 */
struct bdevsw {
	int		(*d_open)(dev_t, int, int, struct lwp *);
	int		(*d_close)(dev_t, int, int, struct lwp *);
	void		(*d_strategy)(struct buf *);
	int		(*d_ioctl)(dev_t, u_long, void *, int, struct lwp *);
	int		(*d_dump)(dev_t, daddr_t, void *, size_t);
	int		(*d_psize)(dev_t);
	int		d_flag;
};

/*
 * Character device switch table
 */
struct cdevsw {
	int		(*d_open)(dev_t, int, int, struct lwp *);
	int		(*d_close)(dev_t, int, int, struct lwp *);
	int		(*d_read)(dev_t, struct uio *, int);
	int		(*d_write)(dev_t, struct uio *, int);
	int		(*d_ioctl)(dev_t, u_long, void *, int, struct lwp *);
	void		(*d_stop)(struct tty *, int);
	struct tty *	(*d_tty)(dev_t);
	int		(*d_poll)(dev_t, int, struct lwp *);
	paddr_t		(*d_mmap)(dev_t, off_t, int);
	int		(*d_kqfilter)(dev_t, struct knote *);
	int		d_flag;
};

#ifdef _KERNEL

int devsw_attach(const char *, const struct bdevsw *, int *,
		 const struct cdevsw *, int *);
int devsw_detach(const struct bdevsw *, const struct cdevsw *);
const struct bdevsw *bdevsw_lookup(dev_t);
const struct cdevsw *cdevsw_lookup(dev_t);
int bdevsw_lookup_major(const struct bdevsw *);
int cdevsw_lookup_major(const struct cdevsw *);

#define	dev_type_open(n)	int n (dev_t, int, int, struct lwp *)
#define	dev_type_close(n)	int n (dev_t, int, int, struct lwp *)
#define	dev_type_read(n)	int n (dev_t, struct uio *, int)
#define	dev_type_write(n)	int n (dev_t, struct uio *, int)
#define	dev_type_ioctl(n) \
		int n (dev_t, u_long, void *, int, struct lwp *)
#define	dev_type_stop(n)	void n (struct tty *, int)
#define	dev_type_tty(n)		struct tty * n (dev_t)
#define	dev_type_poll(n)	int n (dev_t, int, struct lwp *)
#define	dev_type_mmap(n)	paddr_t n (dev_t, off_t, int)
#define	dev_type_strategy(n)	void n (struct buf *)
#define	dev_type_dump(n)	int n (dev_t, daddr_t, void *, size_t)
#define	dev_type_size(n)	int n (dev_t)
#define	dev_type_kqfilter(n)	int n (dev_t, struct knote *)

#define	noopen		((dev_type_open((*)))enodev)
#define	noclose		((dev_type_close((*)))enodev)
#define	noread		((dev_type_read((*)))enodev)
#define	nowrite		((dev_type_write((*)))enodev)
#define	noioctl		((dev_type_ioctl((*)))enodev)
#define	nostop		((dev_type_stop((*)))enodev)
#define	notty		NULL
#define	nopoll		seltrue
#define	nommap		((dev_type_mmap((*)))enodev)
#define	nodump		((dev_type_dump((*)))enodev)
#define	nosize		NULL
#define	nokqfilter	seltrue_kqfilter

#define	nullopen	((dev_type_open((*)))nullop)
#define	nullclose	((dev_type_close((*)))nullop)
#define	nullread	((dev_type_read((*)))nullop)
#define	nullwrite	((dev_type_write((*)))nullop)
#define	nullioctl	((dev_type_ioctl((*)))nullop)
#define	nullstop	((dev_type_stop((*)))nullop)
#define	nullpoll	((dev_type_poll((*)))nullop)
#define	nullmmap	((dev_type_mmap((*)))nullop)
#define	nulldump	((dev_type_dump((*)))nullop)
#define	nullkqfilter	((dev_type_kqfilter((*)))eopnotsupp)

/* device access wrappers. */

dev_type_open(bdev_open);
dev_type_close(bdev_close);
dev_type_strategy(bdev_strategy);
dev_type_ioctl(bdev_ioctl);
dev_type_dump(bdev_dump);

dev_type_open(cdev_open);
dev_type_close(cdev_close);
dev_type_read(cdev_read);
dev_type_write(cdev_write);
dev_type_ioctl(cdev_ioctl);
dev_type_stop(cdev_stop);
dev_type_tty(cdev_tty);
dev_type_poll(cdev_poll);
dev_type_mmap(cdev_mmap);
dev_type_kqfilter(cdev_kqfilter);

int	cdev_type(dev_t);
int	bdev_type(dev_t);

/* symbolic sleep message strings */
extern	const char devopn[], devio[], devwait[], devin[], devout[];
extern	const char devioc[], devcls[];

#endif /* _KERNEL */

/*
 * Line discipline switch table
 */
struct linesw {
	const char *l_name;	/* Linesw name */

	LIST_ENTRY(linesw) l_list;
	u_int	l_refcnt;	/* locked by ttyldisc_list_slock */
	int	l_no;		/* legacy discipline number (for TIOCGETD) */

	int	(*l_open)	(dev_t, struct tty *);
	int	(*l_close)	(struct tty *, int);
	int	(*l_read)	(struct tty *, struct uio *, int);
	int	(*l_write)	(struct tty *, struct uio *, int);
	int	(*l_ioctl)	(struct tty *, u_long, void *, int,
				    struct lwp *);
	int	(*l_rint)	(int, struct tty *);
	int	(*l_start)	(struct tty *);
	int	(*l_modem)	(struct tty *, int);
	int	(*l_poll)	(struct tty *, int, struct lwp *);
};

#ifdef _KERNEL
void	       ttyldisc_init(void);
int	       ttyldisc_attach(struct linesw *);
int	       ttyldisc_detach(struct linesw *);
struct linesw *ttyldisc_lookup(const char *);
struct linesw *ttyldisc_lookup_bynum(int);
struct linesw *ttyldisc_default(void);
void	       ttyldisc_release(struct linesw *);

/* For those defining their own line disciplines: */
#define	ttynodisc ((int (*)(dev_t, struct tty *))enodev)
#define	ttyerrclose ((int (*)(struct tty *, int))enodev)
#define	ttyerrio ((int (*)(struct tty *, struct uio *, int))enodev)
#define	ttyerrinput ((int (*)(int, struct tty *))enodev)
#define	ttyerrstart ((int (*)(struct tty *))enodev)

int	ttyerrpoll (struct tty *, int, struct lwp *);
int	ttynullioctl(struct tty *, u_long, void *, int, struct lwp *);

int	iskmemdev(dev_t);
int	seltrue_kqfilter(dev_t, struct knote *);
#endif

#ifdef _KERNEL

#define	DEV_MEM		0	/* minor device 0 is physical memory */
#define	DEV_KMEM	1	/* minor device 1 is kernel memory */
#define	DEV_NULL	2	/* minor device 2 is EOF/rathole */
#ifdef COMPAT_16
#define	_DEV_ZERO_oARM	3	/* reserved: old ARM /dev/zero minor */
#endif
#define	DEV_ZERO	12	/* minor device 12 is '\0'/rathole */

#endif /* _KERNEL */

struct devsw_conv {
	const char *d_name;
	int d_bmajor;
	int d_cmajor;
};

#ifdef _KERNEL
void devsw_init(void);
const char *devsw_blk2name(int);
int devsw_name2blk(const char *, char *, size_t);
int devsw_name2chr(const char *, char *, size_t);
dev_t devsw_chr2blk(dev_t);
dev_t devsw_blk2chr(dev_t);
#endif /* _KERNEL */

#ifdef _KERNEL
struct	device;
void	setroot(struct device *, int);
void	swapconf(void);
#endif /* _KERNEL */

#endif /* !_SYS_CONF_H_ */
