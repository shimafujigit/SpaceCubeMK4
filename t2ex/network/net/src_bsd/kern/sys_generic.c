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

/*	$NetBSD: sys_generic.c,v 1.120.6.1 2009/06/17 20:49:00 bouyer Exp $	*/

/*-
 * Copyright (c) 2007, 2008, 2009 The NetBSD Foundation, Inc.
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
 *	@(#)sys_generic.c	8.9 (Berkeley) 2/14/95
 */

/*
 * System calls relating to files.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sys_generic.c,v 1.120.6.1 2009/06/17 20:49:00 bouyer Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/proc.h>
#include <sys/socketvar.h>
#ifndef T2EX
#include <sys/signalvar.h>
#endif
#include <sys/uio.h>
#include <sys/kernel.h>
#include <sys/stat.h>
#include <sys/kmem.h>
#include <sys/poll.h>
#ifndef T2EX
#include <sys/vnode.h>
#include <sys/mount.h>
#else
#include <sys/filio.h>
#include <sys/fcntl.h>
#endif
#include <sys/syscallargs.h>
#include <sys/ktrace.h>
#ifndef T2EX
#include <sys/atomic.h>
#else
#include <sys/_atomic.h>
#endif

#ifndef T2EX
#include <uvm/uvm_extern.h>
#endif

/*
 * Read system call.
 */
/* ARGSUSED */
int
sys_read(struct lwp *l, const struct sys_read_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)		fd;
		syscallarg(void *)	buf;
		syscallarg(size_t)	nbyte;
	} */
#ifdef T2EX
#define	fd	sd
#define	nbyte	count
#endif

	file_t *fp;
	int fd;

	fd = SCARG(uap, fd);

	if ((fp = fd_getfile(fd)) == NULL)
		return (EBADF);

	if ((fp->f_flag & FREAD) == 0) {
		fd_putfile(fd);
		return (EBADF);
	}

	/* dofileread() will unuse the descriptor for us */
	return (dofileread(fd, fp, SCARG(uap, buf), SCARG(uap, nbyte),
	    &fp->f_offset, FOF_UPDATE_OFFSET, retval));

#ifdef T2EX
#undef	fd
#undef	nbyte
#endif
}

int
dofileread(int fd, struct file *fp, void *buf, size_t nbyte,
	off_t *offset, int flags, register_t *retval)
{
	struct iovec aiov;
	struct uio auio;
	size_t cnt;
	int error;
#ifndef T2EX
	lwp_t *l;

	l = curlwp;
#endif

	aiov.iov_base = (void *)buf;
	aiov.iov_len = nbyte;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_resid = nbyte;
	auio.uio_rw = UIO_READ;
#ifndef T2EX
	auio.uio_vmspace = l->l_proc->p_vmspace;
#endif

	/*
	 * Reads return ssize_t because -1 is returned on error.  Therefore
	 * we must restrict the length to SSIZE_MAX to avoid garbage return
	 * values.
	 */
	if (auio.uio_resid > SSIZE_MAX) {
		error = EINVAL;
		goto out;
	}

	cnt = auio.uio_resid;
#ifndef T2EX
	error = (*fp->f_ops->fo_read)(fp, offset, &auio, fp->f_cred, flags);
#else
	error = (*fp->f_ops->fo_read)(fp, offset, &auio, NULL, flags);
#endif
	if (error)
		if (auio.uio_resid != cnt && (error == ERESTART ||
		    error == EINTR || error == EWOULDBLOCK))
			error = 0;
	cnt -= auio.uio_resid;
	ktrgenio(fd, UIO_READ, buf, cnt, error);
	*retval = cnt;
 out:
	fd_putfile(fd);
	return (error);
}

#ifndef T2EX
/*
 * Scatter read system call.
 */
int
sys_readv(struct lwp *l, const struct sys_readv_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)				fd;
		syscallarg(const struct iovec *)	iovp;
		syscallarg(int)				iovcnt;
	} */

	return do_filereadv(SCARG(uap, fd), SCARG(uap, iovp),
	    SCARG(uap, iovcnt), NULL, FOF_UPDATE_OFFSET, retval);
}

int
do_filereadv(int fd, const struct iovec *iovp, int iovcnt,
    off_t *offset, int flags, register_t *retval)
{
	struct uio	auio;
	struct iovec	*iov, *needfree = NULL, aiov[UIO_SMALLIOV];
	int		i, error;
	size_t		cnt;
	u_int		iovlen;
	struct file	*fp;
	struct iovec	*ktriov = NULL;

	if (iovcnt == 0)
		return EINVAL;

	if ((fp = fd_getfile(fd)) == NULL)
		return EBADF;

	if ((fp->f_flag & FREAD) == 0) {
		fd_putfile(fd);
		return EBADF;
	}

	if (offset == NULL)
		offset = &fp->f_offset;
	else {
		struct vnode *vp = fp->f_data;
		if (fp->f_type != DTYPE_VNODE || vp->v_type == VFIFO) {
			error = ESPIPE;
			goto out;
		}
		/*
		 * Test that the device is seekable ?
		 * XXX This works because no file systems actually
		 * XXX take any action on the seek operation.
		 */
		error = VOP_SEEK(vp, fp->f_offset, *offset, fp->f_cred);
		if (error != 0)
			goto out;
	}

	iovlen = iovcnt * sizeof(struct iovec);
	if (flags & FOF_IOV_SYSSPACE)
		iov = __UNCONST(iovp);
	else {
		iov = aiov;
		if ((u_int)iovcnt > UIO_SMALLIOV) {
			if ((u_int)iovcnt > IOV_MAX) {
				error = EINVAL;
				goto out;
			}
			iov = kmem_alloc(iovlen, KM_SLEEP);
			if (iov == NULL) {
				error = ENOMEM;
				goto out;
			}
			needfree = iov;
		}
		error = copyin(iovp, iov, iovlen);
		if (error)
			goto done;
	}

	auio.uio_iov = iov;
	auio.uio_iovcnt = iovcnt;
	auio.uio_rw = UIO_READ;
	auio.uio_vmspace = curproc->p_vmspace;

	auio.uio_resid = 0;
	for (i = 0; i < iovcnt; i++, iov++) {
		auio.uio_resid += iov->iov_len;
		/*
		 * Reads return ssize_t because -1 is returned on error.
		 * Therefore we must restrict the length to SSIZE_MAX to
		 * avoid garbage return values.
		 */
		if (iov->iov_len > SSIZE_MAX || auio.uio_resid > SSIZE_MAX) {
			error = EINVAL;
			goto done;
		}
	}

	/*
	 * if tracing, save a copy of iovec
	 */
	if (ktrpoint(KTR_GENIO))  {
		ktriov = kmem_alloc(iovlen, KM_SLEEP);
		if (ktriov != NULL)
			memcpy(ktriov, auio.uio_iov, iovlen);
	}

	cnt = auio.uio_resid;
	error = (*fp->f_ops->fo_read)(fp, offset, &auio, fp->f_cred, flags);
	if (error)
		if (auio.uio_resid != cnt && (error == ERESTART ||
		    error == EINTR || error == EWOULDBLOCK))
			error = 0;
	cnt -= auio.uio_resid;
	*retval = cnt;

	if (ktriov != NULL) {
		ktrgeniov(fd, UIO_READ, ktriov, cnt, error);
		kmem_free(ktriov, iovlen);
	}

 done:
	if (needfree)
		kmem_free(needfree, iovlen);
 out:
	fd_putfile(fd);
	return (error);
}
#endif

/*
 * Write system call
 */
int
sys_write(struct lwp *l, const struct sys_write_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)			fd;
		syscallarg(const void *)	buf;
		syscallarg(size_t)		nbyte;
	} */
#ifdef T2EX
#define	fd	sd
#define	nbyte	count
#endif

	file_t *fp;
	int fd;

	fd = SCARG(uap, fd);

	if ((fp = fd_getfile(fd)) == NULL)
		return (EBADF);

	if ((fp->f_flag & FWRITE) == 0) {
		fd_putfile(fd);
		return (EBADF);
	}

	/* dofilewrite() will unuse the descriptor for us */
	return (dofilewrite(fd, fp, SCARG(uap, buf), SCARG(uap, nbyte),
	    &fp->f_offset, FOF_UPDATE_OFFSET, retval));

#ifdef T2EX
#undef	fd
#undef	nbyte
#endif
}

int
dofilewrite(int fd, struct file *fp, const void *buf,
	size_t nbyte, off_t *offset, int flags, register_t *retval)
{
	struct iovec aiov;
	struct uio auio;
	size_t cnt;
	int error;

	aiov.iov_base = __UNCONST(buf);		/* XXXUNCONST kills const */
	aiov.iov_len = nbyte;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_resid = nbyte;
	auio.uio_rw = UIO_WRITE;
#ifndef T2EX
	auio.uio_vmspace = curproc->p_vmspace;
#endif

	/*
	 * Writes return ssize_t because -1 is returned on error.  Therefore
	 * we must restrict the length to SSIZE_MAX to avoid garbage return
	 * values.
	 */
	if (auio.uio_resid > SSIZE_MAX) {
		error = EINVAL;
		goto out;
	}

	cnt = auio.uio_resid;
#ifndef T2EX
	error = (*fp->f_ops->fo_write)(fp, offset, &auio, fp->f_cred, flags);
#else
	error = (*fp->f_ops->fo_write)(fp, offset, &auio, NULL, flags);
#endif
	if (error) {
		if (auio.uio_resid != cnt && (error == ERESTART ||
		    error == EINTR || error == EWOULDBLOCK))
			error = 0;
#ifndef T2EX
		if (error == EPIPE) {
			mutex_enter(proc_lock);
			psignal(curproc, SIGPIPE);
			mutex_exit(proc_lock);
		}
#endif
	}
	cnt -= auio.uio_resid;
	ktrgenio(fd, UIO_WRITE, buf, cnt, error);
	*retval = cnt;
 out:
	fd_putfile(fd);
	return (error);
}

#ifndef T2EX
/*
 * Gather write system call
 */
int
sys_writev(struct lwp *l, const struct sys_writev_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)				fd;
		syscallarg(const struct iovec *)	iovp;
		syscallarg(int)				iovcnt;
	} */

	return do_filewritev(SCARG(uap, fd), SCARG(uap, iovp),
	    SCARG(uap, iovcnt), NULL, FOF_UPDATE_OFFSET, retval);
}

int
do_filewritev(int fd, const struct iovec *iovp, int iovcnt,
    off_t *offset, int flags, register_t *retval)
{
	struct uio	auio;
	struct iovec	*iov, *needfree = NULL, aiov[UIO_SMALLIOV];
	int		i, error;
	size_t		cnt;
	u_int		iovlen;
	struct file	*fp;
	struct iovec	*ktriov = NULL;

	if (iovcnt == 0)
		return EINVAL;

	if ((fp = fd_getfile(fd)) == NULL)
		return EBADF;

	if ((fp->f_flag & FWRITE) == 0) {
		fd_putfile(fd);
		return EBADF;
	}

	if (offset == NULL)
		offset = &fp->f_offset;
	else {
		struct vnode *vp = fp->f_data;
		if (fp->f_type != DTYPE_VNODE || vp->v_type == VFIFO) {
			error = ESPIPE;
			goto out;
		}
		/*
		 * Test that the device is seekable ?
		 * XXX This works because no file systems actually
		 * XXX take any action on the seek operation.
		 */
#ifndef T2EX
		error = VOP_SEEK(vp, fp->f_offset, *offset, fp->f_cred);
#else
		error = VOP_SEEK(vp, fp->f_offset, *offset, NULL);
#endif
		if (error != 0)
			goto out;
	}

	iovlen = iovcnt * sizeof(struct iovec);
	if (flags & FOF_IOV_SYSSPACE)
		iov = __UNCONST(iovp);
	else {
		iov = aiov;
		if ((u_int)iovcnt > UIO_SMALLIOV) {
			if ((u_int)iovcnt > IOV_MAX) {
				error = EINVAL;
				goto out;
			}
			iov = kmem_alloc(iovlen, KM_SLEEP);
			if (iov == NULL) {
				error = ENOMEM;
				goto out;
			}
			needfree = iov;
		}
		error = copyin(iovp, iov, iovlen);
		if (error)
			goto done;
	}

	auio.uio_iov = iov;
	auio.uio_iovcnt = iovcnt;
	auio.uio_rw = UIO_WRITE;
#ifndef T2EX
	auio.uio_vmspace = curproc->p_vmspace;
#endif

	auio.uio_resid = 0;
	for (i = 0; i < iovcnt; i++, iov++) {
		auio.uio_resid += iov->iov_len;
		/*
		 * Writes return ssize_t because -1 is returned on error.
		 * Therefore we must restrict the length to SSIZE_MAX to
		 * avoid garbage return values.
		 */
		if (iov->iov_len > SSIZE_MAX || auio.uio_resid > SSIZE_MAX) {
			error = EINVAL;
			goto done;
		}
	}

	/*
	 * if tracing, save a copy of iovec
	 */
	if (ktrpoint(KTR_GENIO))  {
		ktriov = kmem_alloc(iovlen, KM_SLEEP);
		if (ktriov != NULL)
			memcpy(ktriov, auio.uio_iov, iovlen);
	}

	cnt = auio.uio_resid;
	error = (*fp->f_ops->fo_write)(fp, offset, &auio, fp->f_cred, flags);
	if (error) {
		if (auio.uio_resid != cnt && (error == ERESTART ||
		    error == EINTR || error == EWOULDBLOCK))
			error = 0;
		if (error == EPIPE) {
			mutex_enter(proc_lock);
			psignal(curproc, SIGPIPE);
			mutex_exit(proc_lock);
		}
	}
	cnt -= auio.uio_resid;
	*retval = cnt;

	if (ktriov != NULL) {
		ktrgeniov(fd, UIO_WRITE, ktriov, cnt, error);
		kmem_free(ktriov, iovlen);
	}

 done:
	if (needfree)
		kmem_free(needfree, iovlen);
 out:
	fd_putfile(fd);
	return (error);
}
#endif

/*
 * Ioctl system call
 */
/* ARGSUSED */
int
sys_ioctl(struct lwp *l, const struct sys_ioctl_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)		fd;
		syscallarg(u_long)	com;
		syscallarg(void *)	data;
	} */
#ifdef T2EX
#define	fd	sd
#define	com	request
#endif

	struct file	*fp;
	proc_t		*p;
	struct filedesc	*fdp;
	u_long		com;
	int		error;
	u_int		size;
	void 		*data, *memp;
#define	STK_PARAMS	128
	u_long		stkbuf[STK_PARAMS/sizeof(u_long)];
	fdfile_t	*ff;

	error = 0;
	p = l->l_proc;
	fdp = p->p_fd;

	if ((fp = fd_getfile(SCARG(uap, fd))) == NULL)
		return (EBADF);

	if ((fp->f_flag & (FREAD | FWRITE)) == 0) {
		error = EBADF;
		com = 0;
		goto out;
	}

	ff = fdp->fd_ofiles[SCARG(uap, fd)];
	switch (com = SCARG(uap, com)) {
	case FIONCLEX:
		ff->ff_exclose = false;
		goto out;

	case FIOCLEX:
		ff->ff_exclose = true;
		fdp->fd_exclose = true;
		goto out;
	}

	/*
	 * Interpret high order word to find amount of data to be
	 * copied to/from the user's address space.
	 */
	size = IOCPARM_LEN(com);
	if (size > IOCPARM_MAX) {
		error = ENOTTY;
		goto out;
	}
	memp = NULL;
	if (size > sizeof(stkbuf)) {
		memp = kmem_alloc(size, KM_SLEEP);
#ifdef T2EX
		if ( memp == NULL ) {
			error = ENOMEM;
			goto out;
		}
#endif
		data = memp;
	} else
		data = (void *)stkbuf;
	if (com&IOC_IN) {
		if (size) {
			error = copyin(SCARG(uap, data), data, size);
			if (error) {
				if (memp)
					kmem_free(memp, size);
				goto out;
			}
			ktrgenio(SCARG(uap, fd), UIO_WRITE, SCARG(uap, data),
			    size, 0);
		} else
			*(void **)data = SCARG(uap, data);
	} else if ((com&IOC_OUT) && size)
		/*
		 * Zero the buffer so the user always
		 * gets back something deterministic.
		 */
		memset(data, 0, size);
	else if (com&IOC_VOID)
		*(void **)data = SCARG(uap, data);

	switch (com) {

	case FIONBIO:
		/* XXX Code block is not atomic */
		if (*(int *)data != 0)
			atomic_or_uint(&fp->f_flag, FNONBLOCK);
		else
			atomic_and_uint(&fp->f_flag, ~FNONBLOCK);
		error = (*fp->f_ops->fo_ioctl)(fp, FIONBIO, data);
		break;

	case FIOASYNC:
		/* XXX Code block is not atomic */
		if (*(int *)data != 0)
			atomic_or_uint(&fp->f_flag, FASYNC);
		else
			atomic_and_uint(&fp->f_flag, ~FASYNC);
		error = (*fp->f_ops->fo_ioctl)(fp, FIOASYNC, data);
		break;

	default:
		error = (*fp->f_ops->fo_ioctl)(fp, com, data);
		/*
		 * Copy any data to user, size was
		 * already set and checked above.
		 */
		if (error == 0 && (com&IOC_OUT) && size) {
			error = copyout(data, SCARG(uap, data), size);
			ktrgenio(SCARG(uap, fd), UIO_READ, SCARG(uap, data),
			    size, error);
		}
		break;
	}
	if (memp)
		kmem_free(memp, size);
 out:
	fd_putfile(SCARG(uap, fd));
	switch (error) {
	case -1:
		printf("sys_ioctl: _IO%s%s('%c', %lu, %lu) returned -1: "
		    "pid=%d comm=%s\n",
		    (com & IOC_IN) ? "W" : "", (com & IOC_OUT) ? "R" : "",
		    (char)IOCGROUP(com), (com & 0xff), IOCPARM_LEN(com),
#ifndef T2EX
		    p->p_pid, p->p_comm);
#else
			0, "N/A");
#endif
		/* FALLTHROUGH */
	case EPASSTHROUGH:
		error = ENOTTY;
		/* FALLTHROUGH */
	default:
		return (error);
	}

#ifdef T2EX
#undef	fd
#undef	com
#endif
}
