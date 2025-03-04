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
 *	@(#)fs_readd.c
 *
 */

#include "fsdefs.h"

/*
 *  fs_getdents() - Read directory entries
 */
EXPORT	INT	xfs_getdents(fs_env_t *env, INT fno, struct dirent *buf,
					INT bufsz)
{
	fs_file_t	*file;
	fs_cond_t	*con;
	fimp_t		*cmd;
	INT		sts;

	/* Get file & connection descriptor */
	sts = fs_file_get(env, fno, &file, &con);
	if (sts != 0) goto exit0;

	/* Check if file is a directory */
	sts = fs_is_fdir(env, con, file);
	if (sts != 1) {
		if (sts == 0) sts = EX_NOTDIR;
		goto exit1;
	}

	/* Lock buffer space */
	if (fs_lockspace(buf, bufsz) != 0) {
		sts = EX_ACCES;
		goto exit1;
	}

	/* Call FIMP GETDENTS service */
	env->t_misc.t_rw.t_len = bufsz;
	env->t_misc.t_rw.t_off64 = FILE_OFFSET(file);

	cmd = &env->t_cmd;
	cmd->r_getdents.fid = file->f.f_fid;
	cmd->r_getdents.oflags = file->f_desc.d_flags;
	cmd->r_getdents.buf = buf;
	cmd->r_getdents.len = &env->t_misc.t_rw.t_len;
	cmd->r_getdents.off = &env->t_misc.t_rw.t_off64;
	cmd->r_getdents.retoff = &env->t_misc.t_rw.t_off64;
	sts = fs_callfimp(env, con, FIMP_GETDENTS);

	/* Unlock buffer space */
	(void)fs_unlockspace(buf, bufsz);

	if (sts == 0) {
		FILE_OFFSET(file) = env->t_misc.t_rw.t_off64;
		sts = env->t_misc.t_rw.t_len;
	}
exit1:
	/* Release file & connection descriptor */
	(void)fs_file_release(env, file, con);
	if (sts >= 0) return sts;
exit0:
	env->t_errno = sts;
	return -1;
}

