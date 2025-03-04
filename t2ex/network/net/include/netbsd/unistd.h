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

/*	$NetBSD: unistd.h,v 1.118.4.1 2009/01/16 01:04:29 snj Exp $	*/

/*-
 * Copyright (c) 1998, 1999, 2008 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein.
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
 * Copyright (c) 1991, 1993, 1994
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
 *	@(#)unistd.h	8.12 (Berkeley) 4/27/95
 */

#ifndef _UNISTD_H_
#define	_UNISTD_H_

#include <machine/ansi.h>
#include <machine/int_types.h>
#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <sys/types.h>
#include <sys/unistd.h>


/*
 * IEEE Std 1003.1-90
 */
#define	STDIN_FILENO	0	/* standard input file descriptor */
#define	STDOUT_FILENO	1	/* standard output file descriptor */
#define	STDERR_FILENO	2	/* standard error file descriptor */

#include <sys/null.h>

__BEGIN_DECLS
__dead	 void _exit(int);
int	 access(const char *, int);
unsigned int alarm(unsigned int);
int	 chdir(const char *);
#if defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)
int	chown(const char *, uid_t, gid_t) __RENAME(__posix_chown);
#else
int	chown(const char *, uid_t, gid_t);
#endif /* defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE) */
int	 close(int);
size_t	 confstr(int, char *, size_t);
#ifndef __CUSERID_DECLARED
#define __CUSERID_DECLARED
/* also declared in stdio.h */
char	*cuserid(char *);	/* obsolete */
#endif /* __CUSERID_DECLARED */
int	 dup(int);
int	 dup2(int, int);
int	 execl(const char *, const char *, ...);
int	 execle(const char *, const char *, ...);
int	 execlp(const char *, const char *, ...);
int	 execv(const char *, char * const *);
int	 execve(const char *, char * const *, char * const *);
int	 execvp(const char *, char * const *);
pid_t	 fork(void);
long	 fpathconf(int, int);
char	*getcwd(char *, size_t);
gid_t	 getegid(void);
uid_t	 geteuid(void);
gid_t	 getgid(void);
int	 getgroups(int, gid_t []);
__aconst char *getlogin(void);
int	 getlogin_r(char *, size_t);
pid_t	 getpgrp(void);
pid_t	 getpid(void);
pid_t	 getppid(void);
uid_t	 getuid(void);
int	 isatty(int);
int	 link(const char *, const char *);
long	 pathconf(const char *, int);
int	 pause(void);
int	 pipe(int *);
ssize_t	 read(int, void *, size_t);
int	 rmdir(const char *);
int	 setgid(gid_t);
int	 setpgid(pid_t, pid_t);
pid_t	 setsid(void);
int	 setuid(uid_t);
unsigned int	 sleep(unsigned int);
long	 sysconf(int);
pid_t	 tcgetpgrp(int);
int	 tcsetpgrp(int, pid_t);
__aconst char *ttyname(int);
int	 unlink(const char *);
ssize_t	 write(int, const void *, size_t);


/*
 * IEEE Std 1003.2-92, adopted in X/Open Portability Guide Issue 4 and later
 */
#if (_POSIX_C_SOURCE - 0) >= 2 || defined(_XOPEN_SOURCE) || \
    defined(_NETBSD_SOURCE)
int	 getopt(int, char * const [], const char *);

extern	 char *optarg;			/* getopt(3) external variables */
extern	 int opterr;
extern	 int optind;
extern	 int optopt;
#endif

/*
 * The Open Group Base Specifications, Issue 6; IEEE Std 1003.1-2001 (POSIX)
 */
#if (_POSIX_C_SOURCE - 0) >= 200112L || (_XOPEN_SOURCE - 0) >= 600 || \
    defined(_NETBSD_SOURCE)
int	 setegid(gid_t);
int	 seteuid(uid_t);
#endif

/*
 * The following three syscalls are also defined in <sys/types.h>
 * We protect them against double declarations.
 */
#ifndef __OFF_T_SYSCALLS_DECLARED
#define __OFF_T_SYSCALLS_DECLARED
off_t	 lseek(int, off_t, int);
int	 truncate(const char *, off_t);
/*
 * IEEE Std 1003.1b-93,
 * also found in X/Open Portability Guide >= Issue 4 Verion 2
 */
#if (_POSIX_C_SOURCE - 0) >= 199309L || \
    (defined(_XOPEN_SOURCE) && defined(_XOPEN_SOURCE_EXTENDED)) || \
    (_XOPEN_SOURCE - 0) >= 500 || defined(_NETBSD_SOURCE)
int	 ftruncate(int, off_t);
#endif
#endif /* __OFF_T_SYSCALLS_DECLARED */


/*
 * IEEE Std 1003.1b-93, adopted in X/Open CAE Specification Issue 5 Version 2
 */
#if (_POSIX_C_SOURCE - 0) >= 199309L || (_XOPEN_SOURCE - 0) >= 500 || \
    defined(_NETBSD_SOURCE)
int	 fdatasync(int);
int	 fsync(int);
#endif


/*
 * IEEE Std 1003.1c-95, also adopted by X/Open CAE Spec Issue 5 Version 2
 */
#if (_POSIX_C_SOURCE - 0) >= 199506L || (_XOPEN_SOURCE - 0) >= 500 || \
    defined(_REENTRANT) || defined(_NETBSD_SOURCE)
int	 ttyname_r(int, char *, size_t);
int	 pthread_atfork(void (*)(void), void (*)(void), void (*)(void));
#endif

/*
 * X/Open Portability Guide, all issues
 */
#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
int	 chroot(const char *);
int	 nice(int);
#endif


/*
 * X/Open Portability Guide >= Issue 4
 */
#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
__aconst char *crypt(const char *, const char *);
int	 encrypt(char *, int);
char	*getpass(const char *);
pid_t	 getsid(pid_t);
#endif


/*
 * X/Open Portability Guide >= Issue 4 Version 2
 */
#if (defined(_XOPEN_SOURCE) && defined(_XOPEN_SOURCE_EXTENDED)) || \
    (_XOPEN_SOURCE - 0) >= 500 || defined(_NETBSD_SOURCE)
#ifndef	intptr_t
typedef	__intptr_t	intptr_t;
#define	intptr_t	__intptr_t
#endif

#define F_ULOCK		0
#define F_LOCK		1
#define F_TLOCK		2
#define F_TEST		3

int	 brk(void *);
int	 fchdir(int);
#if defined(_XOPEN_SOURCE)
int	 fchown(int, uid_t, gid_t) __RENAME(__posix_fchown);
#else
int	 fchown(int, uid_t, gid_t);
#endif
int	 getdtablesize(void);
long	 gethostid(void);
int	 gethostname(char *, size_t);
__pure int
	 getpagesize(void);		/* legacy */
pid_t	 getpgid(pid_t);
#if defined(_XOPEN_SOURCE)
int	 lchown(const char *, uid_t, gid_t) __RENAME(__posix_lchown);
#else
int	 lchown(const char *, uid_t, gid_t);
#endif
int	 lockf(int, int, off_t);
ssize_t	 readlink(const char * __restrict, char * __restrict, size_t);
void	*sbrk(intptr_t);
/* XXX prototype wrong! */
int	 setpgrp(pid_t, pid_t);			/* obsoleted by setpgid() */
int	 setregid(gid_t, gid_t);
int	 setreuid(uid_t, uid_t);
void	 swab(const void *, void *, size_t);
int	 symlink(const char *, const char *);
void	 sync(void);
useconds_t ualarm(useconds_t, useconds_t);
int	 usleep(useconds_t);
#ifndef __LIBC12_SOURCE__
pid_t	 vfork(void) __RENAME(__vfork14);
#endif

#ifndef __AUDIT__
char	*getwd(char *);				/* obsoleted by getcwd() */
#endif
#endif /* _XOPEN_SOURCE_EXTENDED || _XOPEN_SOURCE >= 500 || _NETBSD_SOURCE */


/*
 * X/Open CAE Specification Issue 5 Version 2
 */
#if (_XOPEN_SOURCE - 0) >= 500 || defined(_NETBSD_SOURCE)
ssize_t	 pread(int, void *, size_t, off_t);
ssize_t	 pwrite(int, const void *, size_t, off_t);
#endif


/*
 * Implementation-defined extensions
 */
#if defined(_NETBSD_SOURCE)
int	 acct(const char *);
int	 closefrom(int);
int	 des_cipher(const char *, char *, long, int);
int	 des_setkey(const char *);
void	 endusershell(void);
int	 exect(const char *, char * const *, char * const *);
int	 fchroot(int);
int	 fsync_range(int, int, off_t, off_t);
int	 getdomainname(char *, size_t);
int	 getgrouplist(const char *, gid_t, gid_t *, int *);
int	 getgroupmembership(const char *, gid_t, gid_t *, int, int *);
mode_t	 getmode(const void *, mode_t);
int	 getpeereid(int, uid_t *, gid_t *);
int	 getsubopt(char **, char * const *, char **);
__aconst char *getusershell(void);
int	 initgroups(const char *, gid_t);
int	 iruserok(uint32_t, int, const char *, const char *);
int      issetugid(void);
int	 nfssvc(int, void *);
int	 profil(char *, size_t, u_long, u_int);
#ifndef __PSIGNAL_DECLARED
#define __PSIGNAL_DECLARED
/* also in signal.h */
void	psignal(unsigned int, const char *);
#endif /* __PSIGNAL_DECLARED */
int	 rcmd(char **, int, const char *, const char *, const char *, int *);
int	 reboot(int, char *);
int	 revoke(const char *);
int	 rresvport(int *);
int	 ruserok(const char *, int, const char *, const char *);
int	 setdomainname(const char *, size_t);
int	 setgroups(int, const gid_t *);
int	 sethostid(long);
int	 sethostname(const char *, size_t);
int	 setlogin(const char *);
void	*setmode(const char *);
int	 setrgid(gid_t);
int	 setruid(uid_t);
void	 setusershell(void);
void	 strmode(mode_t, char *);
__aconst char *strsignal(int);
int	 swapctl(int, void *, int);
int	 swapon(const char *);			/* obsoleted by swapctl() */
int	 syscall(int, ...);
quad_t	 __syscall(quad_t, ...);
int	 undelete(const char *);

#if 1 /*INET6*/
int	 rcmd_af(char **, int, const char *,
	    const char *, const char *, int *, int);
int	 rresvport_af(int *, int);
int	 iruserok_sa(const void *, int, int, const char *, const char *);
#endif

#ifndef __SYS_SIGLIST_DECLARED
#define __SYS_SIGLIST_DECLARED
/* also in signal.h */
extern const char *const *sys_siglist __RENAME(__sys_siglist14);
#endif /* __SYS_SIGLIST_DECLARED */
extern	 int optreset;		/* getopt(3) external variable */
extern	 char *suboptarg;	/* getsubopt(3) external variable */
#endif

__END_DECLS

#if _FORTIFY_SOURCE > 0
#include <ssp/unistd.h>
#endif
#endif /* !_UNISTD_H_ */
