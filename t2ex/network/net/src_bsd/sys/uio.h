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

/*	$NetBSD: uio.h,v 1.34 2006/03/01 12:38:32 yamt Exp $	*/

/*
 * Copyright (c) 1982, 1986, 1993, 1994
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
 *	@(#)uio.h	8.5 (Berkeley) 2/22/94
 */

#ifndef _SYS_UIO_H_
#define	_SYS_UIO_H_

#ifdef _KERNEL
#ifndef __UIO_EXPOSE
#define __UIO_EXPOSE
#endif
#endif

#include <machine/ansi.h>
#include <sys/featuretest.h>

#ifndef T2EX
#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_	size_t;
#undef	_BSD_SIZE_T_
#endif

#ifdef	_BSD_SSIZE_T_
typedef	_BSD_SSIZE_T_	ssize_t;
#undef	_BSD_SSIZE_T_
#endif
#endif /* T2EX */

struct iovec {
	void	*iov_base;	/* Base address. */
	size_t	 iov_len;	/* Length. */
};

#if defined(_NETBSD_SOURCE)
#include <sys/ansi.h>

#ifndef T2EX
#ifndef	off_t
typedef	__off_t		off_t;	/* file offset */
#define	off_t		__off_t
#endif
#else
#ifdef	__off_t
typedef	__off_t		off_t;
#undef	__off_t
#endif
#endif

enum	uio_rw { UIO_READ, UIO_WRITE };

/* Segment flag values. */
enum uio_seg {
	UIO_USERSPACE,		/* from user data space */
	UIO_SYSSPACE		/* from system space */
};

#ifdef __UIO_EXPOSE

struct uio {
	struct	iovec *uio_iov;	/* pointer to array of iovecs */
	int	uio_iovcnt;	/* number of iovecs in array */
	off_t	uio_offset;	/* offset into file this uio corresponds to */
	size_t	uio_resid;	/* residual i/o count */
	enum	uio_rw uio_rw;	/* see above */
	struct	vmspace *uio_vmspace;
};
#define	UIO_SETUP_SYSSPACE(uio)	uio_setup_sysspace(uio)

#endif /* __UIO_EXPOSE */

/*
 * Limits
 */
/* Deprecated: use IOV_MAX from <limits.h> instead. */
#define UIO_MAXIOV	1024		/* max 1K of iov's */
#endif /* _NETBSD_SOURCE */

#ifdef _KERNEL
#include <sys/mallocvar.h>

MALLOC_DECLARE(M_IOV);

#define UIO_SMALLIOV	8		/* 8 on stack, else malloc */

void uio_setup_sysspace(struct uio *);
#endif

#ifndef T2EX
#ifndef	_KERNEL
#include <sys/cdefs.h>

__BEGIN_DECLS
#if defined(_NETBSD_SOURCE)
ssize_t preadv(int, const struct iovec *, int, off_t);
ssize_t pwritev(int, const struct iovec *, int, off_t);
#endif /* _NETBSD_SOURCE */
ssize_t	readv(int, const struct iovec *, int);
ssize_t	writev(int, const struct iovec *, int);
__END_DECLS
#else
int ureadc(int, struct uio *);
#endif /* !_KERNEL */
#endif /* T2EX */

#endif /* !_SYS_UIO_H_ */
