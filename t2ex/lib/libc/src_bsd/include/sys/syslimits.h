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

/*	$OpenBSD: syslimits.h,v 1.11 2008/02/02 15:31:31 kettenis Exp $	*/
/*	$NetBSD: syslimits.h,v 1.12 1995/10/05 05:26:19 thorpej Exp $	*/

/*
 * Copyright (c) 1988, 1993
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
 *	@(#)syslimits.h	8.1 (Berkeley) 6/2/93
 */

#include <sys/cdefs.h>

#if __POSIX_VISIBLE || __XPG_VISIBLE || __BSD_VISIBLE
#define	ARG_MAX		 (256 * 1024)	/* max bytes for an exec function */
#define	CHILD_MAX		   80	/* max simultaneous processes */
#define	LINK_MAX		32767	/* max file link count */
#define	MAX_CANON		  255	/* max bytes in term canon input line */
#define	MAX_INPUT		  255	/* max bytes in terminal input */
#define	NAME_MAX		  255	/* max bytes in a file name */
#define	NGROUPS_MAX		   16	/* max supplemental group id's */
#define	OPEN_MAX		   64	/* max open files per process */
#define	PATH_MAX		 1024	/* max bytes in pathname */
#define	PIPE_BUF		  512	/* max bytes for atomic pipe writes */
#define	SYMLINK_MAX	     PATH_MAX	/* max bytes in a symbolic link */
#define	SYMLOOP_MAX		   32	/* max symlinks per path (for loops) */

#define	BC_BASE_MAX	      INT_MAX	/* max ibase/obase values in bc(1) */
#define	BC_DIM_MAX		65535	/* max array elements in bc(1) */
#define	BC_SCALE_MAX	      INT_MAX	/* max scale value in bc(1) */
#define	BC_STRING_MAX	      INT_MAX	/* max const string length in bc(1) */
#define	COLL_WEIGHTS_MAX	    2	/* max weights for order keyword */
#define	EXPR_NEST_MAX		   32	/* max expressions nested in expr(1) */
#define	LINE_MAX		 2048	/* max bytes in an input line */
#ifndef RE_DUP_MAX
#define	RE_DUP_MAX		  255	/* max RE's in interval notation */
#endif

#if __XPG_VISIBLE
#define	IOV_MAX			 1024	/* max # of iov's (readv,sendmsg,etc) */
#define	NZERO			   20	/* default "nice" */
#endif /* __XPG_VISIBLE */

#endif /* __POSIX_VISIBLE || __XPG_VISIBLE || __BSD_VISIBLE */

#if __XPG_VISIBLE >= 500 || __POSIX_VISIBLE >= 199506 || __BSD_VISIBLE
#define TTY_NAME_MAX		260	/* max tty device name length w/ NUL */
#define LOGIN_NAME_MAX          32	/* max login name length w/ NUL */
#endif /* __XPG_VISIBLE >= 500 || __POSIX_VISIBLE >= 199506 || __BSD_VISIBLE */

#if __POSIX_VISIBLE >= 200112
#define HOST_NAME_MAX		255
#endif
