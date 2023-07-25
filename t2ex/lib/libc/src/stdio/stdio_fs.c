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
 *	@(#)stdio_fs.c
 *
 */

#include <basic.h>
#include <t2ex/fs.h>

struct __libc_stat {
	unsigned int	st_mode;
	unsigned int	st_blksize;
	off64_t	st_size;
};

EXPORT	int	__libc_open( const char *path, int oflags, mode_t mode )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_open( path, oflags, mode );
}

EXPORT	int	__libc_close( int fd )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_close( fd );
}

EXPORT	int	__libc_read( int fd, void *buf, size_t count )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_read( fd, buf, count );
}

EXPORT	int	__libc_write( int fd, const void *buf, size_t count )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_write( fd, buf, count );
}

EXPORT	off64_t	__libc_lseek( int fd, off64_t offset64, int whence )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_lseek64( fd, offset64, whence );
}

EXPORT	ER	__libc_fcntl(int fd, int cmd, int arg)
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	return fs_fcntl( fd, cmd, arg );
}

EXPORT	int	__libc_fstat( int fd, struct __libc_stat *buf  )
{
#ifndef	USE_T2EX_FS
	/* If there are not any filesystems, this function always fail.	*/
	return EX_NOTSUP;	/* Not Support	*/
#endif
	int	r;
	struct stat64	s;

	r = fs_fstat64( fd, &s );
	if (r < 0) return -1;

	buf->st_mode = s.st_mode;
	buf->st_size = s.st_size;
	buf->st_blksize = s.st_blksize;

	return 0;
}

