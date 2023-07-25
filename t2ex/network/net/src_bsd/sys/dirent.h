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

/*	$NetBSD: dirent.h,v 1.24 2008/03/15 19:02:49 christos Exp $	*/

/*-
 * Copyright (c) 1989, 1993
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
 *	@(#)dirent.h	8.3 (Berkeley) 8/10/94
 */

#ifndef _SYS_DIRENT_H_
#define _SYS_DIRENT_H_

#include <sys/featuretest.h>

/*
 * The dirent structure defines the format of directory entries returned by
 * the getdents(2) system call.
 *
 * A directory entry has a struct dirent at the front of it, containing its
 * inode number, the length of the entry, and the length of the name
 * contained in the entry.  These are followed by the name padded to 
 * _DIRENT_ALIGN() byte boundary with null bytes.  All names are guaranteed
 * NUL terminated.  The maximum length of a name in a directory is MAXNAMLEN.
 */
struct dirent {
	ino_t d_fileno;			/* file number of entry */
	uint16_t d_reclen;		/* length of this record */
	uint16_t d_namlen;		/* length of string in d_name */
	uint8_t  d_type; 		/* file type, see below */
#if defined(_NETBSD_SOURCE)
#define	MAXNAMLEN	511
	char	d_name[MAXNAMLEN + 1];	/* name must be no longer than this */
#else
	char	d_name[511 + 1];	/* name must be no longer than this */
#endif
};

#if defined(_NETBSD_SOURCE)
/*
 * File types
 */
#define	DT_UNKNOWN	 0
#define	DT_FIFO		 1
#define	DT_CHR		 2
#define	DT_DIR		 4
#define	DT_BLK		 6
#define	DT_REG		 8
#define	DT_LNK		10
#define	DT_SOCK		12
#define	DT_WHT		14

/*
 * The _DIRENT_ALIGN macro returns the alignment of struct dirent.
 * struct direct and struct dirent12 used 4 byte alignment but
 * struct dirent uses 8.
 */
#define _DIRENT_ALIGN(dp) (sizeof((dp)->d_fileno) - 1)
/*
 * The _DIRENT_NAMEOFF macro returns the offset of the d_name field in 
 * struct dirent
 */
#define _DIRENT_NAMEOFF(dp) \
    ((char *)(void *)&(dp)->d_name - (char *)(void *)dp)
/*
 * The _DIRENT_RECLEN macro gives the minimum record length which will hold
 * a name of size "namlen".  This requires the amount of space in struct dirent
 * without the d_name field, plus enough space for the name with a terminating
 * null byte (namlen+1), rounded up to a the appropriate byte boundary.
 */
#define _DIRENT_RECLEN(dp, namlen) \
    ((_DIRENT_NAMEOFF(dp) + (namlen) + 1 + _DIRENT_ALIGN(dp)) & \
    ~_DIRENT_ALIGN(dp))
/*
 * The _DIRENT_SIZE macro returns the minimum record length required for
 * name name stored in the current record.
 */
#define	_DIRENT_SIZE(dp) _DIRENT_RECLEN(dp, (dp)->d_namlen)
/*
 * The _DIRENT_NEXT macro advances to the next dirent record.
 */
#define _DIRENT_NEXT(dp) ((void *)((char *)(void *)(dp) + (dp)->d_reclen))
/*
 * The _DIRENT_MINSIZE returns the size of an empty (invalid) record.
 */
#define _DIRENT_MINSIZE(dp) _DIRENT_RECLEN(dp, 0)

/*
 * Convert between stat structure types and directory types.
 */
#define	IFTODT(mode)	(((mode) & 0170000) >> 12)
#define	DTTOIF(dirtype)	((dirtype) << 12)
#endif

#endif	/* !_SYS_DIRENT_H_ */
