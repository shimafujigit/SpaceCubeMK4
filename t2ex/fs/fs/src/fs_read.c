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
 *	@(#)fs_read.c
 *
 */

#include "fsdefs.h"

/*
 *  Read data from file
 */
LOCAL	INT	xfs_read_impl(fs_env_t *env, fs_cond_t *con, fs_file_t *file,
				void *buf, size_t num, off64_t off)
{
	fimp_t	*cmd;
	INT	sts;

	sts = 0;
	if ((INT)num < 0) {
		sts = EX_INVAL;
		goto exit0;
	}
	if (fs_lockspace(buf, num) != 0) {
		sts = EX_ACCES;
		goto exit0;
	}

	/* Check 64 bits offset */
	sts = fs_check_64bits(con, off);
	if (sts == 0) {
		/* Call FIMP READ64 service */
		env->t_misc.t_rw.t_len = num;
		env->t_misc.t_rw.t_off64 = off;

		cmd = &env->t_cmd;
		cmd->r_read64.fid = file->f.f_fid;
		cmd->r_read64.oflags = file->f_desc.d_flags;
		cmd->r_read64.buf = buf;
		cmd->r_read64.len = &env->t_misc.t_rw.t_len;
		cmd->r_read64.off = (off64_t *)&env->t_misc.t_rw.t_off64;
		cmd->r_read64.retoff = (off64_t *)&env->t_misc.t_rw.t_off64;
		sts = fs_callfimp(env, con, FIMP_READ64);
		FILE_OFFSET(file) = env->t_misc.t_rw.t_off64;
	}
	(void)fs_unlockspace(buf, num);
exit0:
	return sts;
}

/*
 *  fs_read() - Read data from file
 */
EXPORT	INT	xfs_read(fs_env_t * env, INT fno, void *buf, size_t num)
{
	fs_file_t	*file;
	fs_cond_t	*con;
	INT		sts;

	env->t_misc.t_rw.t_len = 0;

	/* Get file and connection descriptor */
	sts = fs_file_get(env, fno, &file, &con);
	if (sts != 0) goto exit0;

	/* Check open mode */
	if ((file->f_desc.d_flags & O_ACCMODE) == O_WRONLY) {
		sts = EX_BADF;
		goto exit1;
	}

	/* Read data */
	if (num != 0) {
		sts = xfs_read_impl(env, con, file,
					buf, num, FILE_OFFSET(file));
	}
exit1:
	/* Release file and connection descriptor */
	(void)fs_file_release(env, file, con);
	if (sts == 0) return (INT)env->t_misc.t_rw.t_len;
exit0:
	env->t_errno = sts;
	return -1;
}

