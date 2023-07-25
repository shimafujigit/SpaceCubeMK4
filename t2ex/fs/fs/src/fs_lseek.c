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
 *	@(#)fs_lseek.c
 *
 */

#include "fsdefs.h"

/*
 *  Move the read/write file offset
 */
LOCAL	INT	lseek64(fs_env_t *env, INT fno, off64_t off,
				INT whence, off64_t *newoff, BOOL off64)
{
	fs_file_t	*file;
	fs_cond_t	*con;
	INT		sts;

	/* Get file & connection descriptor */
	sts = fs_file_get(env, fno, &file, &con);
	if (sts != 0) goto exit0;

	/* Check file type & get file size */
	sts = xfs_fstat64_us_impl(env, con, file);
	if (sts != 0) goto exit1;

	if (S_ISCHR(env->t_misc.t_stat64_u.st_mode)
		|| S_ISFIFO(env->t_misc.t_stat64_u.st_mode)
		|| S_ISSOCK(env->t_misc.t_stat64_u.st_mode)) {
		sts = EX_SPIPE;
		goto exit1;
	}

	/* Move offset depends on whence */
	switch (whence) {
	case SEEK_SET:
		// off = off;
		break;
	case SEEK_CUR:
		off += FILE_OFFSET(file);
		break;
	case SEEK_END:
		off += env->t_misc.t_stat64_u.st_size;
		break;
	default:
		sts = EX_INVAL;
		goto exit1;
	}

	/* Check result offset */
	if (off < 0
#if	LSEEK_WITHIN_FILE_SIZE
		|| off > env->t_misc.t_stat64_u.st_size
#endif
							) {
		sts = EX_INVAL;
		goto exit1;
	}
	/* Check over 32 bits */
	if (off64 == FALSE) {
		if (off > ULONG_MAX) {
			sts = EX_OVERFLOW;
			goto exit1;
		}
#if	SIGNED_OFFSET
		if (off > LONG_MAX) {
			sts = EX_INVAL;
			goto exit1;
		}
#endif
	}

	/* Set new offset */
	FILE_OFFSET(file) = off;
	*newoff = off;
exit1:
	/* Release file & connection descriptor */
	(void)fs_file_release(env, file, con);
	if (sts == 0) return 0;
exit0:
	env->t_errno = sts;
	return sts;
}

/*
 *  fs_lseek() - Move the read/write file offset
 */
EXPORT	INT	xfs_lseek(fs_env_t *env, INT fno, off_t off, INT whence,
					off_t *newoff)
{
	off64_t	newoff64;
	INT	sts;

	sts = lseek64(env, fno, off, whence, &newoff64, FALSE);
	if (sts == 0) *newoff = (off_t)newoff64;
	return sts;
}

/*
 *  fs_lseek64() - Move the read/write file offset
 */
EXPORT	INT	xfs_lseek64(fs_env_t *env, INT fno, off64_t off,
					INT whence, off64_t *newoff)
{
	return lseek64(env, fno, off, whence, newoff, TRUE);
}

