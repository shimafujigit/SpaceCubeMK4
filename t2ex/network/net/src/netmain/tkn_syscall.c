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

/*
 *	@(#)tkn_syscall.c
 *
 */

/* Network System Call management */

#include <sys/tkn_syscall.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/lwp.h>
#include <sys/proc.h>
#include <sys/syscallargs.h>

#include <tk/tkernel.h>
#include "../tkn.h"

#undef SYS_
#define SYS_(f)		[SYS_##f] = (sy_call_t *)sys_##f
#define COMPAT_43_SYS_(f)		[compat_43_SYS_##f] = (sy_call_t *)compat_43_sys_##f

static sy_call_t *sy_call[] = {
	SYS_(accept),
	SYS_(bind),
	SYS_(close),
	SYS_(connect),
	SYS_(getpeername),
	SYS_(getsockname),
	SYS_(getsockopt),
	SYS_(ioctl),
	SYS_(listen),
	COMPAT_43_SYS_(recv),
	SYS_(recvfrom),
	SYS_(recvmsg),
	SYS_(select),
	SYS_(select_us),
	SYS_(select_ms),
	COMPAT_43_SYS_(send),
	SYS_(sendmsg),
	SYS_(sendto),
	SYS_(setsockopt),
	SYS_(shutdown),
	SYS_(__socket30),
	SYS_(read),
	SYS_(write),
	SYS_(fcntl),

	SYS_(gethostname),
	SYS_(sethostname),
	SYS_(getaddrinfo),
	SYS_(getaddrinfo_us),
	SYS_(getaddrinfo_ms),
	SYS_(getnameinfo),
	SYS_(getnameinfo_us),
	SYS_(getnameinfo_ms),
	SYS_(sockatmark),
	SYS_(rtlist),
	SYS_(getifaddrs),
	SYS_(ifattach),
	SYS_(ifdetach),
	SYS_(ifindextoname),
	SYS_(ifnametoindex),
	SYS_(resctl),
	SYS_(break),

	SYS_(bpfopen),
	SYS_(tunopen),
};
#define	SYS_MAXSYSCALL	( (int)( sizeof(sy_call) / sizeof(sy_call_t*) ) )

static int nosys(struct lwp *l, void *v, register_t *retval)
{
	(void)l;
	(void)v;
	(void)retval;

	return ENOSYS;
}

inline static sy_call_t *
tkn_sysfunctbl_search(int sys_no)
{
	sy_call_t	*fp;

	if (sys_no > 0 && sys_no < SYS_MAXSYSCALL) {
		if ((fp = sy_call[sys_no]) == NULL) fp = (sy_call_t *)nosys;
	} else {
		fp = (sy_call_t *)nosys;
	}
	return fp;
}

int
tkn_syscall(int sys_no, void *v, register_t *retval)
{
	struct lwp *l = curlwp;
	int error;

	/* call system call entry */
	error = tkn_sysfunctbl_search(sys_no)(l, v, retval);

	return error;
}
