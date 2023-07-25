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

/*	$OpenBSD: file.h,v 1.28 2011/06/28 10:15:38 thib Exp $	*/
/*	$NetBSD: file.h,v 1.11 1995/03/26 20:24:13 jtc Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1989, 1993
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
 *	@(#)file.h	8.2 (Berkeley) 8/20/94
 */

#include <sys/fcntl.h>
#include <sys/unistd.h>

#ifdef _KERNEL
#include <sys/queue.h>

struct proc;
struct uio;
struct knote;
struct stat;
struct file;

struct	fileops {
	int	(*fo_read)(struct file *, off_t *, struct uio *,
		    struct ucred *);
	int	(*fo_write)(struct file *, off_t *, struct uio *,
		    struct ucred *);
	int	(*fo_ioctl)(struct file *, u_long, caddr_t,
		    struct proc *);
	int	(*fo_poll)(struct file *, int, struct proc *);
	int	(*fo_kqfilter)(struct file *, struct knote *);
	int	(*fo_stat)(struct file *, struct stat *, struct proc *);
	int	(*fo_close)(struct file *, struct proc *);
};

/*
 * Kernel descriptor table.
 * One entry for each open kernel vnode and socket.
 */
struct file {
	LIST_ENTRY(file) f_list;/* list of active files */
	short	f_flag;		/* see fcntl.h */
#define	DTYPE_VNODE	1	/* file */
#define	DTYPE_SOCKET	2	/* communications endpoint */
#define	DTYPE_PIPE	3	/* pipe */
#define	DTYPE_KQUEUE	4	/* event queue */
#define	DTYPE_CRYPTO	5	/* crypto */
#define	DTYPE_SYSTRACE	6	/* system call tracing */
	short	f_type;		/* descriptor type */
	long	f_count;	/* reference count */
	long	f_msgcount;	/* references from message queue */
	struct	ucred *f_cred;	/* credentials associated with descriptor */
	struct	fileops *f_ops;
	off_t	f_offset;
	void 	*f_data;	/* private data */
	int	f_iflags;	/* internal flags */
	int	f_usecount;	/* number of users (temporary references). */
	u_int64_t f_rxfer;	/* total number of read transfers */
	u_int64_t f_wxfer;	/* total number of write transfers */
	u_int64_t f_seek;	/* total independent seek operations */
	u_int64_t f_rbytes;	/* total bytes read */
	u_int64_t f_wbytes;	/* total bytes written */
};

#define FIF_WANTCLOSE		0x01	/* a close is waiting for usecount */
#define FIF_LARVAL		0x02	/* not fully constructed, don't use */
#define FIF_MARK		0x04	/* mark during gc() */
#define FIF_DEFER		0x08	/* defer for next gc() pass */

#define FILE_IS_USABLE(fp) \
	(((fp)->f_iflags & (FIF_WANTCLOSE|FIF_LARVAL)) == 0)

#define FREF(fp) do { (fp)->f_usecount++; } while (0)
#define FRELE(fp) do {					\
	--(fp)->f_usecount;					\
	if (((fp)->f_iflags & FIF_WANTCLOSE) != 0)		\
		wakeup(&(fp)->f_usecount);			\
} while (0)

#define FILE_SET_MATURE(fp) do {				\
	(fp)->f_iflags &= ~FIF_LARVAL;				\
	FRELE(fp);						\
} while (0)

LIST_HEAD(filelist, file);
extern struct filelist filehead;	/* head of list of open files */
extern int maxfiles;			/* kernel limit on number of open files */
extern int nfiles;			/* actual number of open files */
extern struct fileops vnops;		/* vnode operations for files */

#endif /* _KERNEL */
