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

/*	$NetBSD: sys_descrip.c,v 1.7.4.1 2009/02/02 02:53:51 snj Exp $	*/

/*-
 * Copyright (c) 2008 The NetBSD Foundation, Inc.
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
 * Copyright (c) 1982, 1986, 1989, 1991, 1993
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
 *	@(#)kern_descrip.c	8.8 (Berkeley) 2/14/95
 */

/*
 * System calls on descriptors.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: sys_descrip.c,v 1.7.4.1 2009/02/02 02:53:51 snj Exp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#ifndef T2EX
#include <sys/vnode.h>
#endif
#include <sys/proc.h>
#include <sys/file.h>
#ifndef T2EX
#include <sys/namei.h>
#endif
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/malloc.h>
#include <sys/pool.h>
#include <sys/syslog.h>
#include <sys/unistd.h>
#ifndef T2EX
#include <sys/resourcevar.h>
#endif
#include <sys/conf.h>
#include <sys/event.h>
#ifndef T2EX
#include <sys/kauth.h>
#include <sys/atomic.h>
#include <sys/mount.h>
#else
#include <sys/_atomic.h>
#include <sys/kmem.h>
#endif
#include <sys/syscallargs.h>

#ifndef T2EX
/*
 * Duplicate a file descriptor.
 */
int
sys_dup(struct lwp *l, const struct sys_dup_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)	fd;
	} */
	int new, error, old;
	file_t *fp;

	old = SCARG(uap, fd);

	if ((fp = fd_getfile(old)) == NULL) {
		return EBADF;
	}
	error = fd_dup(fp, 0, &new, false);
	fd_putfile(old);
	*retval = new;
	return error;
}

/*
 * Duplicate a file descriptor to a particular value.
 */
int
sys_dup2(struct lwp *l, const struct sys_dup2_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)	from;
		syscallarg(int)	to;
	} */
	int old, new, error;
	file_t *fp;

	old = SCARG(uap, from);
	new = SCARG(uap, to);

	if ((fp = fd_getfile(old)) == NULL) {
		return EBADF;
	}
	mutex_enter(&fp->f_lock);
	fp->f_count++;
	mutex_exit(&fp->f_lock);
	fd_putfile(old);

	if ((u_int)new >= curproc->p_rlimit[RLIMIT_NOFILE].rlim_cur ||
	    (u_int)new >= maxfiles) {
		error = EBADF;
	} else if (old == new) {
		error = 0;
	} else {
		error = fd_dup2(fp, new);
	}
	closef(fp);
	*retval = new;

	return error;
}

/*
 * fcntl call which is being passed to the file's fs.
 */
static int
fcntl_forfs(int fd, file_t *fp, int cmd, void *arg)
{
	int		error;
	u_int		size;
	void		*data, *memp;
#define STK_PARAMS	128
	char		stkbuf[STK_PARAMS];

	if ((fp->f_flag & (FREAD | FWRITE)) == 0)
		return (EBADF);

	/*
	 * Interpret high order word to find amount of data to be
	 * copied to/from the user's address space.
	 */
	size = (size_t)F_PARAM_LEN(cmd);
	if (size > F_PARAM_MAX)
		return (EINVAL);
	memp = NULL;
	if (size > sizeof(stkbuf)) {
		memp = kmem_alloc(size, KM_SLEEP);
		data = memp;
	} else
		data = stkbuf;
	if (cmd & F_FSIN) {
		if (size) {
			error = copyin(arg, data, size);
			if (error) {
				if (memp)
					kmem_free(memp, size);
				return (error);
			}
		} else
			*(void **)data = arg;
	} else if ((cmd & F_FSOUT) != 0 && size != 0) {
		/*
		 * Zero the buffer so the user always
		 * gets back something deterministic.
		 */
		memset(data, 0, size);
	} else if (cmd & F_FSVOID)
		*(void **)data = arg;


	error = (*fp->f_ops->fo_fcntl)(fp, cmd, data);

	/*
	 * Copy any data to user, size was
	 * already set and checked above.
	 */
	if (error == 0 && (cmd & F_FSOUT) && size)
		error = copyout(data, arg, size);
	if (memp)
		kmem_free(memp, size);
	return (error);
}

int
do_fcntl_lock(int fd, int cmd, struct flock *fl)
{
	file_t *fp;
	vnode_t *vp;
	proc_t *p;
	int error, flg;

	if ((fp = fd_getfile(fd)) == NULL)
		return EBADF;
	if (fp->f_type != DTYPE_VNODE) {
		fd_putfile(fd);
		return EINVAL;
	}
	vp = fp->f_data;
	if (fl->l_whence == SEEK_CUR)
		fl->l_start += fp->f_offset;

	flg = F_POSIX;
	p = curproc;

	switch (cmd) {
	case F_SETLKW:
		flg |= F_WAIT;
		/* Fall into F_SETLK */

	case F_SETLK:
		switch (fl->l_type) {
		case F_RDLCK:
			if ((fp->f_flag & FREAD) == 0) {
				error = EBADF;
				break;
			}
			if ((p->p_flag & PK_ADVLOCK) == 0) {
				mutex_enter(p->p_lock);
				p->p_flag |= PK_ADVLOCK;
				mutex_exit(p->p_lock);
			}
			error = VOP_ADVLOCK(vp, p, F_SETLK, fl, flg);
			break;

		case F_WRLCK:
			if ((fp->f_flag & FWRITE) == 0) {
				error = EBADF;
				break;
			}
			if ((p->p_flag & PK_ADVLOCK) == 0) {
				mutex_enter(p->p_lock);
				p->p_flag |= PK_ADVLOCK;
				mutex_exit(p->p_lock);
			}
			error = VOP_ADVLOCK(vp, p, F_SETLK, fl, flg);
			break;

		case F_UNLCK:
			error = VOP_ADVLOCK(vp, p, F_UNLCK, fl, F_POSIX);
			break;

		default:
			error = EINVAL;
			break;
		}
		break;

	case F_GETLK:
		if (fl->l_type != F_RDLCK &&
		    fl->l_type != F_WRLCK &&
		    fl->l_type != F_UNLCK) {
			error = EINVAL;
			break;
		}
		error = VOP_ADVLOCK(vp, p, F_GETLK, fl, F_POSIX);
		break;

	default:
		error = EINVAL;
		break;
	}

	fd_putfile(fd);
	return error;
}
#endif

/*
 * The file control system call.
 */
int
sys_fcntl(struct lwp *l, const struct sys_fcntl_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)		fd;
		syscallarg(int)		cmd;
		syscallarg(void *)	arg;
	} */
#ifdef T2EX
#define	fd	sd
#endif

#ifndef T2EX
	int fd, i, tmp, error, cmd, newmin;
#else
	int fd, i, tmp, error, cmd;
#endif
	filedesc_t *fdp;
	file_t *fp;
	fdfile_t *ff;
#ifndef T2EX
	struct flock fl;
#endif

	fd = SCARG(uap, fd);
	cmd = SCARG(uap, cmd);
	fdp = l->l_fd;
	error = 0;

	switch (cmd) {
	case F_CLOSEM:
		if (fd < 0)
			return EBADF;
		while ((i = fdp->fd_lastfile) >= fd) {
			if (fd_getfile(i) == NULL) {
				/* Another thread has updated. */
				continue;
			}
			fd_close(i);
		}
		return 0;

	case F_MAXFD:
		*retval = fdp->fd_lastfile;
		return 0;

#ifndef T2EX
	case F_SETLKW:
	case F_SETLK:
	case F_GETLK:
		error = copyin((void*)SCARG(uap, arg), &fl, sizeof(fl));
		if (error)
			return error;
		error = do_fcntl_lock(fd, cmd, &fl);
		if (cmd == F_GETLK && error == 0)
			error = copyout(&fl, (void*)SCARG(uap, arg), sizeof(fl));
		return error;
#endif

	default:
		/* Handled below */
		break;
	}

	if ((fp = fd_getfile(fd)) == NULL)
		return (EBADF);
	ff = fdp->fd_ofiles[fd];

#ifndef T2EX
	if ((cmd & F_FSCTL)) {
		error = fcntl_forfs(fd, fp, cmd, SCARG(uap, arg));
		fd_putfile(fd);
		return error;
	}
#endif

	switch (cmd) {
#ifndef T2EX
	case F_DUPFD:
		newmin = (long)SCARG(uap, arg);
		if ((u_int)newmin >=
		    l->l_proc->p_rlimit[RLIMIT_NOFILE].rlim_cur ||
		    (u_int)newmin >= maxfiles) {
			fd_putfile(fd);
			return EINVAL;
		}
		error = fd_dup(fp, newmin, &i, false);
		*retval = i;
		break;
#endif

	case F_GETFD:
		*retval = ff->ff_exclose;
		break;

	case F_SETFD:
		if ((long)SCARG(uap, arg) & 1) {
			ff->ff_exclose = true;
			fdp->fd_exclose = true;
		} else {
			ff->ff_exclose = false;
		}
		break;

	case F_GETFL:
		*retval = OFLAGS(fp->f_flag);
		break;

	case F_SETFL:
		/* XXX not guaranteed to be atomic. */
		tmp = FFLAGS((long)SCARG(uap, arg)) & FCNTLFLAGS;
		error = (*fp->f_ops->fo_fcntl)(fp, F_SETFL, &tmp);
		if (error)
			break;
		i = tmp ^ fp->f_flag;
		if (i & FNONBLOCK) {
			int flgs = tmp & FNONBLOCK;
			error = (*fp->f_ops->fo_ioctl)(fp, FIONBIO, &flgs);
			if (error) {
				(*fp->f_ops->fo_fcntl)(fp, F_SETFL,
				    &fp->f_flag);
				break;
			}
		}
		if (i & FASYNC) {
			int flgs = tmp & FASYNC;
			error = (*fp->f_ops->fo_ioctl)(fp, FIOASYNC, &flgs);
			if (error) {
				if (i & FNONBLOCK) {
					tmp = fp->f_flag & FNONBLOCK;
					(void)(*fp->f_ops->fo_ioctl)(fp,
						FIONBIO, &tmp);
				}
				(*fp->f_ops->fo_fcntl)(fp, F_SETFL,
				    &fp->f_flag);
				break;
			}
		}
		fp->f_flag = (fp->f_flag & ~FCNTLFLAGS) | tmp;
		break;

	case F_GETOWN:
		error = (*fp->f_ops->fo_ioctl)(fp, FIOGETOWN, &tmp);
		*retval = tmp;
		break;

	case F_SETOWN:
		tmp = (int)(uintptr_t) SCARG(uap, arg);
		error = (*fp->f_ops->fo_ioctl)(fp, FIOSETOWN, &tmp);
		break;

	default:
		error = EINVAL;
	}

	fd_putfile(fd);
	return (error);

#ifdef T2EX
#undef	fd
#endif
}

/*
 * Close a file descriptor.
 */
int
sys_close(struct lwp *l, const struct sys_close_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)	fd;
	} */

#ifdef T2EX
#define	fd	sd
#endif

	if (fd_getfile(SCARG(uap, fd)) == NULL) {
		return EBADF;
	}
	return fd_close(SCARG(uap, fd));

#ifdef T2EX
#undef	fd
#endif
}

#ifndef T2EX
/*
 * Return status information about a file descriptor.
 * Common function for compat code.
 */
int
do_sys_fstat(int fd, struct stat *sb)
{
	file_t *fp;
	int error;

	if ((fp = fd_getfile(fd)) == NULL) {
		return EBADF;
	}
	error = (*fp->f_ops->fo_stat)(fp, sb);
	fd_putfile(fd);

	return error;
}

/*
 * Return status information about a file descriptor.
 */
int
sys___fstat30(struct lwp *l, const struct sys___fstat30_args *uap,
	      register_t *retval)
{
	/* {
		syscallarg(int)			fd;
		syscallarg(struct stat *)	sb;
	} */
	struct stat sb;
	int error;

	error = do_sys_fstat(SCARG(uap, fd), &sb);
	if (error == 0) {
		error = copyout(&sb, SCARG(uap, sb), sizeof(sb));
	}
	return error;
}

/*
 * Return pathconf information about a file descriptor.
 */
int
sys_fpathconf(struct lwp *l, const struct sys_fpathconf_args *uap,
	      register_t *retval)
{
	/* {
		syscallarg(int)	fd;
		syscallarg(int)	name;
	} */
	int fd, error;
	file_t *fp;

	fd = SCARG(uap, fd);
	error = 0;

	if ((fp = fd_getfile(fd)) == NULL) {
		return (EBADF);
	}
	switch (fp->f_type) {
	case DTYPE_SOCKET:
	case DTYPE_PIPE:
		if (SCARG(uap, name) != _PC_PIPE_BUF)
			error = EINVAL;
		else
			*retval = PIPE_BUF;
		break;

	case DTYPE_VNODE:
		error = VOP_PATHCONF(fp->f_data, SCARG(uap, name), retval);
		break;

	case DTYPE_KQUEUE:
		error = EINVAL;
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	fd_putfile(fd);
	return (error);
}

/*
 * Apply an advisory lock on a file descriptor.
 *
 * Just attempt to get a record lock of the requested type on
 * the entire file (l_whence = SEEK_SET, l_start = 0, l_len = 0).
 */
/* ARGSUSED */
int
sys_flock(struct lwp *l, const struct sys_flock_args *uap, register_t *retval)
{
	/* {
		syscallarg(int)	fd;
		syscallarg(int)	how;
	} */
	int fd, how, error;
	file_t *fp;
	vnode_t	*vp;
	struct flock lf;
	proc_t *p;

	fd = SCARG(uap, fd);
	how = SCARG(uap, how);
	error = 0;

	if ((fp = fd_getfile(fd)) == NULL) {
		return EBADF;
	}
	if (fp->f_type != DTYPE_VNODE) {
		fd_putfile(fd);
		return EOPNOTSUPP;
	}

	vp = fp->f_data;
	lf.l_whence = SEEK_SET;
	lf.l_start = 0;
	lf.l_len = 0;
	if (how & LOCK_UN) {
		lf.l_type = F_UNLCK;
		atomic_and_uint(&fp->f_flag, ~FHASLOCK);
		error = VOP_ADVLOCK(vp, fp, F_UNLCK, &lf, F_FLOCK);
		fd_putfile(fd);
		return error;
	}
	if (how & LOCK_EX) {
		lf.l_type = F_WRLCK;
	} else if (how & LOCK_SH) {
		lf.l_type = F_RDLCK;
	} else {
		fd_putfile(fd);
		return EINVAL;
	}
	atomic_or_uint(&fp->f_flag, FHASLOCK);
	p = curproc;
	if (how & LOCK_NB) {
		error = VOP_ADVLOCK(vp, fp, F_SETLK, &lf, F_FLOCK);
	} else {
		error = VOP_ADVLOCK(vp, fp, F_SETLK, &lf, F_FLOCK|F_WAIT);
	}
	fd_putfile(fd);
	return error;
}

int
do_posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
	file_t *fp;
	int error;

	if ((fp = fd_getfile(fd)) == NULL) {
		return EBADF;
	}
	if (fp->f_type != DTYPE_VNODE) {
		if (fp->f_type == DTYPE_PIPE || fp->f_type == DTYPE_SOCKET) {
			error = ESPIPE;
		} else {
			error = EOPNOTSUPP;
		}
		fd_putfile(fd);
		return error;
	}

	switch (advice) {
	case POSIX_FADV_NORMAL:
	case POSIX_FADV_RANDOM:
	case POSIX_FADV_SEQUENTIAL:
		KASSERT(POSIX_FADV_NORMAL == UVM_ADV_NORMAL);
		KASSERT(POSIX_FADV_RANDOM == UVM_ADV_RANDOM);
		KASSERT(POSIX_FADV_SEQUENTIAL == UVM_ADV_SEQUENTIAL);

		/*
		 * We ignore offset and size.  must lock the file to
		 * do this, as f_advice is sub-word sized.
		 */
		mutex_enter(&fp->f_lock);
		fp->f_advice = (u_char)advice;
		mutex_exit(&fp->f_lock);
		error = 0;
		break;

	case POSIX_FADV_WILLNEED:
	case POSIX_FADV_DONTNEED:
	case POSIX_FADV_NOREUSE:
		/* Not implemented yet. */
		error = 0;
		break;
	default:
		error = EINVAL;
		break;
	}

	fd_putfile(fd);
	return error;
}

int
sys___posix_fadvise50(struct lwp *l,
		      const struct sys___posix_fadvise50_args *uap,
		      register_t *retval)
{
	/* {
		syscallarg(int) fd;
		syscallarg(int) pad;
		syscallarg(off_t) offset;
		syscallarg(off_t) len;
		syscallarg(int) advice;
	} */

	return do_posix_fadvise(SCARG(uap, fd), SCARG(uap, offset),
	    SCARG(uap, len), SCARG(uap, advice));
}
#endif /* T2EX */
