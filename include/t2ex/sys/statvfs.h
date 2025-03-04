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
 *	@(#)statvfs.h
 *
 */

#ifndef _SYS_STATVFS_H_
#define _SYS_STATVFS_H_

#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* FS status structure */
struct statvfs {
	unsigned long f_bsize;	/* File system block size */
	unsigned long f_frsize;	/* Fundamental file system block size */
	fsblkcnt_t	f_blocks;	/* Total number of blocks on file system in units of f_frsize */
	fsblkcnt_t	f_bfree;	/* Total number of free blocks */
	fsblkcnt_t	f_bavail;	/* Number of free blocks available to non-privileged process */
	fsfilcnt_t	f_files;	/* Total number of file serial numbers */
	fsfilcnt_t	f_ffree;	/* Total number of free file serial numbers */
	fsfilcnt_t	f_favail;	/* Number of file serial numbers available to non-privileged process */
	unsigned long f_fsid;	/* File system ID */
	unsigned long f_flag;	/* Bit mask of f_flag values */
	unsigned long f_namemax;	/* Maximum filename length */
};

/* Values of f_flag (keep in sync with fs.h) */
#define ST_RDONLY	1	/* Readonly file system */
#define ST_READONLY	1	/* Readonly file system */
#define ST_REMOVABLE	2	/* Removable file system */
#define ST_MEMORY	4	/* Memory file system */
#define ST_NETWORK	8	/* Network file system */
#define ST_NOSUID     128	/* Does not support setuid()/ setgid() semantics */

#ifdef	__cplusplus
}
#endif
#endif	/* _SYS_STATVFS_H_ */

