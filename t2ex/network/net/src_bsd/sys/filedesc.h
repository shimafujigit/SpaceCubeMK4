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

/*	$NetBSD: filedesc.h,v 1.52 2008/07/02 17:06:12 ad Exp $	*/

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
 * Copyright (c) 1990, 1993
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
 *	@(#)filedesc.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _SYS_FILEDESC_H_
#define	_SYS_FILEDESC_H_

#include <sys/param.h>
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/mutex.h>
#include <sys/rwlock.h>
#include <sys/condvar.h>

/*
 * This structure is used for the management of descriptors.  It may be
 * shared by multiple processes.
 *
 * A process is initially started out with NDFILE descriptors stored within
 * this structure, selected to be enough for typical applications based on
 * the historical limit of 20 open files (and the usage of descriptors by
 * shells).  If these descriptors are exhausted, a larger descriptor table
 * may be allocated, up to a process' resource limit; the internal arrays
 * are then unused.  The initial expansion is set to NDEXTENT; each time
 * it runs out, it is doubled until the resource limit is reached. NDEXTENT
 * should be selected to be the biggest multiple of OFILESIZE (see below)
 * that will fit in a power-of-two sized piece of memory.
 */
#ifndef T2EX
#define	NDFILE		20
#define	NDEXTENT	50		/* 250 bytes in 256-byte alloc */
#endif
#define	NDENTRIES	32		/* 32 fds per entry */
#define	NDENTRYMASK	(NDENTRIES - 1)
#define	NDENTRYSHIFT	5		/* bits per entry */
#define	NDLOSLOTS(x)	(((x) + NDENTRIES - 1) >> NDENTRYSHIFT)
#define	NDHISLOTS(x)	((NDLOSLOTS(x) + NDENTRIES - 1) >> NDENTRYSHIFT)
#define	NDFDFILE	3		/* first 3 descriptors are free */

/*
 * Process-private descriptor reference, one for each descriptor slot
 * in use.  Locks:
 *
 * :	unlocked
 * d	filedesc_t::fd_lock
 * f	fdfile_t::ff_lock, may be stable if reference held
 *
 * Note that ff_exclose and ff_allocated are likely to be byte sized
 * (bool).  In general adjacent sub-word sized fields must be locked
 * the same way, but in this case it's ok: ff_exclose can only be
 * modified while the descriptor slot is live, and ff_allocated when
 * it's invalid.
 */
typedef struct fdfile {
	kmutex_t	ff_lock;	/* :: lock on structure */
	bool		ff_exclose;	/* :: close on exec flag */
	bool		ff_allocated;	/* d: descriptor slot is allocated */
	u_int		ff_refcnt;	/* f: reference count on structure */
	struct file	*ff_file;	/* f: pointer to file if open */
	SLIST_HEAD(,knote) ff_knlist;	/* f: knotes attached to this fd */
	kcondvar_t	ff_closing;	/* f: notifier for close */
} fdfile_t;

/* Reference count */
#define	FR_CLOSING	(0x80000000)	/* closing: must interlock */
#define	FR_MASK		(~FR_CLOSING)	/* reference count */

typedef struct filedesc {
	/*
	 * Built-in fdfile_t records first, since they have strict
	 * alignment requirements.
	 */
	uint8_t		fd_dfdfile[NDFDFILE][CACHE_LINE_SIZE];
	/*
	 * All of the remaining fields are locked by fd_lock.
	 */
	kmutex_t	fd_lock;	/* lock on structure */
	fdfile_t	**fd_ofiles;	/* file structures for open files */
	uint32_t	*fd_himap;	/* each bit points to 32 fds */
	uint32_t	*fd_lomap;	/* bitmap of free fds */
	void		*fd_discard;	/* old fd_ofiles tables to discard */
	struct klist	*fd_knhash;	/* hash of attached non-fd knotes */
	int		fd_lastkqfile;	/* max descriptor for kqueue */
	int		fd_lastfile;	/* high-water mark of fd_ofiles */
	int		fd_refcnt;	/* reference count */
	int		fd_nfiles;	/* number of open files allocated */
	u_long		fd_knhashmask;	/* size of fd_knhash */
#define fd_startzero	fd_freefile	/* area to zero on return to cache */
	int		fd_freefile;	/* approx. next free file */
	int		fd_nused;	/* number of slots in use */
	bool		fd_exclose;	/* non-zero if >0 fd with EXCLOSE */
#ifndef T2EX
	/*
	 * These arrays are used when the number of open files is
	 * <= NDFILE, and are then pointed to by the pointers above.
	 */
	fdfile_t	*fd_dfiles[NDFILE];
#endif
	/*
	 * These arrays are used when the number of open files is
	 * <= 1024, and are then pointed to by the pointers above.
	 */
	uint32_t	fd_dhimap[NDENTRIES >> NDENTRYSHIFT];
	uint32_t	fd_dlomap[NDENTRIES];
} filedesc_t;

#ifndef T2EX
typedef struct cwdinfo {
	struct vnode	*cwdi_cdir;	/* current directory */
	struct vnode	*cwdi_rdir;	/* root directory */
	struct vnode	*cwdi_edir;	/* emulation root (if known) */
	krwlock_t	cwdi_lock;	/* lock on entire struct */
	u_short		cwdi_cmask;	/* mask for file creation */
	u_int		cwdi_refcnt;	/* reference count */
} cwdinfo_t;
#endif /* T2EX */

#ifdef _KERNEL

struct fileops;
struct socket;
struct proc;

/*
 * Kernel global variables and routines.
 */
#ifndef T2EX
void	fd_sys_init(void);
#else
int	fd_sys_init(void);
int	fd_sys_finish(void);
#endif
int	fd_dupopen(int, int *, int, int);
int	fd_alloc(struct proc *, int, int *);
void	fd_tryexpand(struct proc *);
int	fd_allocfile(file_t **, int *);
void	fd_affix(struct proc *, file_t *, unsigned);
void	fd_abort(struct proc *, file_t *, unsigned);
filedesc_t *fd_copy(void);
filedesc_t *fd_init(filedesc_t *);
void	fd_share(proc_t *);
void	fd_clear(void);
void	fd_free(void);
void	fd_remove(filedesc_t *, unsigned);
void	fd_closeexec(void);
int	fd_checkstd(void);
file_t	*fd_getfile(unsigned);
file_t	*fd_getfile2(proc_t *, unsigned);
void	fd_putfile(unsigned);
int	fd_getvnode(unsigned, file_t **);
int	fd_getsock(unsigned, struct socket **);
void	fd_putvnode(unsigned);
void	fd_putsock(unsigned);
int	fd_close(unsigned);
void	fd_used(filedesc_t *, unsigned);
void	fd_unused(filedesc_t *, unsigned);
bool	fd_isused(filedesc_t *, unsigned);
int	fd_dup(file_t *, int, int *, bool);
int	fd_dup2(file_t *, unsigned);
int	fd_clone(file_t *, unsigned, int, const struct fileops *, void *);

#ifndef T2EX
struct cwdinfo *cwdinit(void);
void	cwdshare(proc_t *);
void	cwdfree(struct cwdinfo *);
#define GETCWD_CHECK_ACCESS 0x0001
int	getcwd_common(struct vnode *, struct vnode *, char **, char *, int,
    int, struct lwp *);
int	vnode_to_path(char *, size_t, struct vnode *, struct lwp *,
    struct proc *);
#endif

void	ffree(file_t *);
int	closef(file_t *);
file_t *fgetdummy(void);
void	fputdummy(file_t *);

struct stat;
int	do_sys_fstat(int, struct stat *);
struct flock;
int	do_fcntl_lock(int, int, struct flock *);
int	do_posix_fadvise(int, off_t, off_t, int);

extern kmutex_t filelist_lock;

#endif /* _KERNEL */

#endif /* !_SYS_FILEDESC_H_ */
