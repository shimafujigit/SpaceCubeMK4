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

/*	$NetBSD: stat.h,v 1.57.4.1 2009/12/18 06:12:51 snj Exp $	*/

/*-
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
 *	@(#)stat.h	8.12 (Berkeley) 8/17/94
 */

#ifndef _SYS_STAT_H_
#define	_SYS_STAT_H_

#include <sys/featuretest.h>
#include <sys/types.h>		/* XXX */

#if defined(_NETBSD_SOURCE)
#include <sys/time.h>
#endif

/*
 * On systems with 8 byte longs and 4 byte time_ts, padding the time_ts
 * is required in order to have a consistent ABI.  This is because the
 * stat structure used to contain timespecs, which had different
 * alignment constraints than a time_t and a long alone.  The padding
 * should be removed the next time the stat structure ABI is changed.
 * (This will happen whever we change to 8 byte time_t.)
 */
#if defined(_LP64)	/* XXXX  && _BSD_TIME_T_ == int */
#define	__STATPAD(x)	int x;
#else
#define	__STATPAD(x)	/* nothing */
#endif

struct stat {
	dev_t	  st_dev;		/* inode's device */
	mode_t	  st_mode;		/* inode protection mode */
	ino_t	  st_ino;		/* inode's number */
	nlink_t	  st_nlink;		/* number of hard links */
	uid_t	  st_uid;		/* user ID of the file's owner */
	gid_t	  st_gid;		/* group ID of the file's group */
	dev_t	  st_rdev;		/* device type */
#if defined(_NETBSD_SOURCE)
	struct	  timespec st_atimespec;/* time of last access */
	struct	  timespec st_mtimespec;/* time of last data modification */
	struct	  timespec st_ctimespec;/* time of last file status change */
	struct 	  timespec st_birthtimespec; /* time of creation */
#else
	time_t	  st_atime;		/* time of last access */
	__STATPAD(__pad0)
	long	  st_atimensec;		/* nsec of last access */
	time_t	  st_mtime;		/* time of last data modification */
	__STATPAD(__pad1)
	long	  st_mtimensec;		/* nsec of last data modification */
	time_t	  st_ctime;		/* time of last file status change */
	__STATPAD(__pad2)
	long	  st_ctimensec;		/* nsec of last file status change */
	time_t	  st_birthtime;		/* time of creation */
	__STATPAD(__pad3)
	long	  st_birthtimensec;	/* nsec of time of creation */
#endif
	off_t	  st_size;		/* file size, in bytes */
	blkcnt_t  st_blocks;		/* blocks allocated for file */
	blksize_t st_blksize;		/* optimal blocksize for I/O */
	uint32_t  st_flags;		/* user defined flags for file */
	uint32_t  st_gen;		/* file generation number */
	uint32_t  st_spare[2];
};

#undef __STATPAD

#if defined(_NETBSD_SOURCE)
#define	st_atime		st_atimespec.tv_sec
#define	st_atimensec		st_atimespec.tv_nsec
#define	st_mtime		st_mtimespec.tv_sec
#define	st_mtimensec		st_mtimespec.tv_nsec
#define	st_ctime		st_ctimespec.tv_sec
#define	st_ctimensec		st_ctimespec.tv_nsec
#define st_birthtime		st_birthtimespec.tv_sec
#define st_birthtimensec	st_birthtimespec.tv_nsec
#endif

#define	S_ISUID	0004000			/* set user id on execution */
#define	S_ISGID	0002000			/* set group id on execution */
#if defined(_NETBSD_SOURCE)
#define	S_ISTXT	0001000			/* sticky bit */
#endif

#define	S_IRWXU	0000700			/* RWX mask for owner */
#define	S_IRUSR	0000400			/* R for owner */
#define	S_IWUSR	0000200			/* W for owner */
#define	S_IXUSR	0000100			/* X for owner */

#if defined(_NETBSD_SOURCE)
#define	S_IREAD		S_IRUSR
#define	S_IWRITE	S_IWUSR
#define	S_IEXEC		S_IXUSR
#endif

#define	S_IRWXG	0000070			/* RWX mask for group */
#define	S_IRGRP	0000040			/* R for group */
#define	S_IWGRP	0000020			/* W for group */
#define	S_IXGRP	0000010			/* X for group */

#define	S_IRWXO	0000007			/* RWX mask for other */
#define	S_IROTH	0000004			/* R for other */
#define	S_IWOTH	0000002			/* W for other */
#define	S_IXOTH	0000001			/* X for other */

#define	_S_IFMT	  0170000		/* type of file mask */
#define	_S_IFIFO  0010000		/* named pipe (fifo) */
#define	_S_IFCHR  0020000		/* character special */
#define	_S_IFDIR  0040000		/* directory */
#define	_S_IFBLK  0060000		/* block special */
#define	_S_IFREG  0100000		/* regular */
#define	_S_IFLNK  0120000		/* symbolic link */
#define	_S_ISVTX  0001000		/* save swapped text even after use */
#define	_S_IFSOCK 0140000		/* socket */
#define	_S_IFWHT  0160000		/* whiteout */
#define	_S_ARCH1  0200000		/* Archive state 1, ls -l shows 'a' */
#define	_S_ARCH2  0400000		/* Archive state 2, ls -l shows 'A' */

#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
#define	S_IFMT	 _S_IFMT
#define	S_IFIFO	 _S_IFIFO
#define	S_IFCHR	 _S_IFCHR
#define	S_IFDIR	 _S_IFDIR
#define	S_IFBLK	 _S_IFBLK
#define	S_IFREG	 _S_IFREG
#define	S_IFLNK	 _S_IFLNK
#define	S_ISVTX	 _S_ISVTX
#endif
#if ((_XOPEN_SOURCE - 0) >= 600) || defined(_NETBSD_SOURCE)
#define	S_IFSOCK _S_IFSOCK
#endif
#if defined(_NETBSD_SOURCE)
#define	S_IFWHT  _S_IFWHT

#define	S_ARCH1	_S_ARCH1
#define	S_ARCH2	_S_ARCH2
#endif

#define	S_ISDIR(m)	(((m) & _S_IFMT) == _S_IFDIR)	/* directory */
#define	S_ISCHR(m)	(((m) & _S_IFMT) == _S_IFCHR)	/* char special */
#define	S_ISBLK(m)	(((m) & _S_IFMT) == _S_IFBLK)	/* block special */
#define	S_ISREG(m)	(((m) & _S_IFMT) == _S_IFREG)	/* regular file */
#define	S_ISFIFO(m)	(((m) & _S_IFMT) == _S_IFIFO)	/* fifo */
#if ((_POSIX_C_SOURCE - 0) >= 200112L) || defined(_XOPEN_SOURCE) || \
    defined(_NETBSD_SOURCE)
#define	S_ISLNK(m)	(((m) & _S_IFMT) == _S_IFLNK)	/* symbolic link */
#endif
#if ((_POSIX_C_SOURCE - 0) >= 200112L) || ((_XOPEN_SOURCE - 0) >= 600) || \
    defined(_NETBSD_SOURCE)
#define	S_ISSOCK(m)	(((m) & _S_IFMT) == _S_IFSOCK)	/* socket */
#endif
#if defined(_NETBSD_SOURCE)
#define	S_ISWHT(m)	(((m) & _S_IFMT) == _S_IFWHT)	/* whiteout */
#endif

#if defined(_NETBSD_SOURCE)
#define	ACCESSPERMS	(S_IRWXU|S_IRWXG|S_IRWXO)	/* 0777 */
							/* 7777 */
#define	ALLPERMS	(S_ISUID|S_ISGID|S_ISTXT|S_IRWXU|S_IRWXG|S_IRWXO)
							/* 0666 */
#define	DEFFILEMODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#define S_BLKSIZE	512		/* block size used in the stat struct */

/*
 * Definitions of flags stored in file flags word.
 *
 * Super-user and owner changeable flags.
 */
#define	UF_SETTABLE	0x0000ffff	/* mask of owner changeable flags */
#define	UF_NODUMP	0x00000001	/* do not dump file */
#define	UF_IMMUTABLE	0x00000002	/* file may not be changed */
#define	UF_APPEND	0x00000004	/* writes to file may only append */
#define UF_OPAQUE	0x00000008	/* directory is opaque wrt. union */
/*	UF_NOUNLINK	0x00000010	   [NOT IMPLEMENTED] */
/*
 * Super-user changeable flags.
 */
#define	SF_SETTABLE	0xffff0000	/* mask of superuser changeable flags */
#define	SF_ARCHIVED	0x00010000	/* file is archived */
#define	SF_IMMUTABLE	0x00020000	/* file may not be changed */
#define	SF_APPEND	0x00040000	/* writes to file may only append */
/*	SF_NOUNLINK	0x00100000	   [NOT IMPLEMENTED] */
#define	SF_SNAPSHOT	0x00200000	/* snapshot inode */
#define	SF_LOG		0x00400000	/* WAPBL log file inode */

#ifdef _KERNEL
/*
 * Shorthand abbreviations of above.
 */
#define	OPAQUE		(UF_OPAQUE)
#define	APPEND		(UF_APPEND | SF_APPEND)
#define	IMMUTABLE	(UF_IMMUTABLE | SF_IMMUTABLE)
#endif /* _KERNEL */
#endif /* _NETBSD_SOURCE */

#if !defined(_KERNEL) && !defined(_STANDALONE)
#include <sys/cdefs.h>

__BEGIN_DECLS
int	chmod(const char *, mode_t);
int	mkdir(const char *, mode_t);
int	mkfifo(const char *, mode_t);
#ifndef __LIBC12_SOURCE__
int	stat(const char *, struct stat *) __RENAME(__stat30);
int	fstat(int, struct stat *) __RENAME(__fstat30);
#endif
mode_t	umask(mode_t);
#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
int	fchmod(int, mode_t);
#ifndef __LIBC12_SOURCE__
int	lstat(const char *, struct stat *) __RENAME(__lstat30);
#endif
int	mknod(const char *, mode_t, dev_t);
#endif /* defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE) */

#if defined(_NETBSD_SOURCE)
int	chflags(const char *, unsigned long);
int	fchflags(int, unsigned long);
int	lchflags(const char *, unsigned long);
int	lchmod(const char *, mode_t);
#endif /* defined(_NETBSD_SOURCE) */
__END_DECLS

#endif /* !_KERNEL && !_STANDALONE */
#endif /* !_SYS_STAT_H_ */
