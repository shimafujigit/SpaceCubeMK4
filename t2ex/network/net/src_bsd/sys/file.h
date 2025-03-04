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

/*	$NetBSD: file.h,v 1.65.6.2 2009/04/04 23:36:28 snj Exp $	*/

/*-
 * Copyright (c) 2009 The NetBSD Foundation, Inc.
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
 *	@(#)file.h	8.3 (Berkeley) 1/9/95
 */

#ifndef _SYS_FILE_H_
#define	_SYS_FILE_H_

#include <sys/fcntl.h>
#ifndef T2EX
#include <sys/unistd.h>
#endif

#ifdef _KERNEL
#include <sys/mallocvar.h>
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/mutex.h>
#include <sys/condvar.h>

struct proc;
struct lwp;
struct uio;
struct iovec;
struct stat;
struct knote;

/*
 * Kernel file descriptor.  One entry for each open kernel vnode and
 * socket.
 *
 * This structure is exported via the KERN_FILE and KERN_FILE2 sysctl
 * calls.  Only add members to the end, do not delete them.
 */
struct file {
	off_t		f_offset;	/* first, is 64-bit */
	kauth_cred_t 	f_cred;		/* creds associated with descriptor */
	const struct fileops {
		int	(*fo_read)	(struct file *, off_t *, struct uio *,
					    kauth_cred_t, int);
		int	(*fo_write)	(struct file *, off_t *, struct uio *,
					    kauth_cred_t, int);
		int	(*fo_ioctl)	(struct file *, u_long, void *);
		int	(*fo_fcntl)	(struct file *, u_int, void *);
		int	(*fo_poll)	(struct file *, int);
		int	(*fo_stat)	(struct file *, struct stat *);
		int	(*fo_close)	(struct file *);
		int	(*fo_kqfilter)	(struct file *, struct knote *);
		void	(*fo_drain)	(struct file *);
		void	(*fo_spare1)	(void);
		void	(*fo_spare2)	(void);
	} *f_ops;
	void		*f_data;	/* descriptor data, e.g. vnode/socket */
	LIST_ENTRY(file) f_list;	/* list of active files */
	kmutex_t	f_lock;		/* lock on structure */
	int		f_flag;		/* see fcntl.h */
	u_int		f_unused1;	/* unused; was internal flags; FIF_* */
#define	DTYPE_VNODE	1		/* file */
#define	DTYPE_SOCKET	2		/* communications endpoint */
#define	DTYPE_PIPE	3		/* pipe */
#define	DTYPE_KQUEUE	4		/* event queue */
#define	DTYPE_MISC	5		/* misc file descriptor type */
#define	DTYPE_CRYPTO	6		/* crypto */
#define	DTYPE_MQUEUE	7		/* message queue */
#define DTYPE_NAMES \
    "0", "file", "socket", "pipe", "kqueue", "misc", "crypto", "mqueue"
	u_int		f_type;		/* descriptor type */
	u_int		f_advice;	/* access pattern hint; UVM_ADV_* */
	u_int		f_count;	/* reference count */
	u_int		f_msgcount;	/* references from message queue */
	u_int		f_unpcount;	/* deferred close: see uipc_usrreq.c */
	SLIST_ENTRY(file) f_unplist;	/* deferred close: see uipc_usrreq.c */
};

#define FILE_LOCK(fp)	mutex_enter(&(fp)->f_lock)
#define FILE_UNLOCK(fp)	mutex_exit(&(fp)->f_lock)

/*
 * Flags for fo_read and fo_write and do_fileread/write/v
 */
#define	FOF_UPDATE_OFFSET	0x0001	/* update the file offset */
#define	FOF_IOV_SYSSPACE	0x0100	/* iov structure in kernel memory */

LIST_HEAD(filelist, file);
extern struct filelist	filehead;	/* head of list of open files */
extern u_int		maxfiles;	/* kernel limit on # of open files */
extern u_int		nfiles;		/* actual number of open files */

extern const struct fileops vnops;	/* vnode operations for files */

int	dofileread(int, struct file *, void *, size_t,
	    off_t *, int, register_t *);
int	dofilewrite(int, struct file *, const void *,
	    size_t, off_t *, int, register_t *);

int	do_filereadv(int, const struct iovec *, int, off_t *,
	    int, register_t *);
int	do_filewritev(int, const struct iovec *, int, off_t *,
	    int, register_t *);

#ifndef T2EX
int	fsetown(pid_t *, u_long, const void *);
int	fgetown(pid_t, u_long, void *);
void	fownsignal(pid_t, int, int, int, void *);
#endif /* T2EX */

/* Commonly used fileops */
int	fnullop_fcntl(struct file *, u_int, void *);
int	fnullop_poll(struct file *, int);
int	fnullop_kqfilter(struct file *, struct knote *);
int	fbadop_read(struct file *, off_t *, struct uio *, kauth_cred_t, int);
int	fbadop_write(struct file *, off_t *, struct uio *, kauth_cred_t, int);
int	fbadop_ioctl(struct file *, u_long, void *);
int	fbadop_close(struct file *);
int	fbadop_stat(struct file *, struct stat *);
void	fnullop_drain(struct file *);

#endif /* _KERNEL */

#endif /* _SYS_FILE_H_ */
