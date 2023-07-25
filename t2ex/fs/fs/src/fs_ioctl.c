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
 *	@(#)fs_ioctl.c
 *
 */

#include "fsdefs.h"

/*
 *  FIONBIO function
 */
LOCAL	INT	fionbio(fs_file_t *file, INT *arg)
{
	if (arg == NULL) return EX_INVAL;

	fs_lock_lock(LOCKNUM_FIL);

	if (*arg == 0) {	/* Set blocking mode */
		file->f_desc.d_flags &= ~O_NONBLOCK;
	} else {		/* Set non-blocking mode */
		file->f_desc.d_flags |= O_NONBLOCK;
	}

	fs_lock_unlock(LOCKNUM_FIL);

	return 0;
}

/*
 *  fs_ioctl() - I/O control
 */
EXPORT	INT	xfs_ioctl(fs_env_t *env, INT fno, INT request, void *arg)
{
	INT		sts, retv;
	fs_file_t	*file;
	fs_cond_t	*con;
	fimp_t		*cmd;

	retv = 0;
	if (request == FIONBIO) {
		/* Get file descriptor */
		sts = fs_file_get(env, fno, &file, NULL);
		if (sts != 0) goto exit0;

		/* Do FIONBIO function */
		sts = fionbio(file, arg);

		/* Relase file descriptor */
		(void)fs_file_release(env, file, NULL);
	} else {
		/* Get file & connection descriptor */
		sts = fs_file_get(env, fno, &file, &con);
		if (sts != 0) goto exit0;

		/* Call FIMP IOCTL service */
		cmd = &env->t_cmd;
		cmd->r_ioctl.dcmd = request;
		cmd->r_ioctl.arg = arg;
		cmd->r_ioctl.retval = &retv;
		cmd->r_ioctl.fid = file->f.f_fid;
		cmd->r_ioctl.oflags = file->f_desc.d_flags;
		sts = fs_callfimp(env, con, FIMP_IOCTL);

		/* Release file & connection descriptor */
		(void)fs_file_release(env, file, con);
	}
	if (sts == 0) return retv;
exit0:
	env->t_errno = sts;
	return -1;
}

