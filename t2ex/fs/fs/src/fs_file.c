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
 *	@(#)fs_file.c
 *
 */

#include "fsdefs.h"

LOCAL	fs_descmgr_t	file_descmgr;		/* file descriptor manager	*/

/*
 *  Initialize File descriptor manager
 */
EXPORT	INT	fs_file_init(INT max)
{
	return fs_descmgr_init(&file_descmgr, max,
					sizeof(fs_file_t), DESC_FILE);
}

/*
 *  Finalize File descriptor manager
 */
EXPORT	INT	fs_file_fini(void)
{
	return fs_descmgr_fini(&file_descmgr);
}

/*
 *  Allocate new file descriptor
 */
EXPORT	fs_file_t	*fs_file_alloc(void)
{
	return (fs_file_t *)fs_descmgr_alloc(&file_descmgr);
}

/*
 *  Free file descriptor
 */
EXPORT	void	fs_file_free(fs_file_t *file)
{
	fs_descmgr_free(&file_descmgr, &file->f_desc);
}

/*
 *  Get file descriptor by file number
 */
EXPORT	INT	fs_file_get(fs_env_t *env, INT fn,
					fs_file_t **fp, fs_cond_t **conp)
{
	fs_file_t	*file;
	INT		sts;

	/* Get new file descriptor */
	file = NULL;
	if ((UINT)fn < (UINT)env->t_ctx->x_maxfile) {
		file = (fs_file_t *)fs_descmgr_get(&file_descmgr,
				env->t_ctx->x_fileids[fn] | FD_CLOEXEC);
	}
	if (file == NULL) {
		sts = EX_BADF;
	} else {
		sts = 0;
		if (conp != NULL) {
			/* Get connection descriptor of the file */
			sts = fs_con_get(file->f.f_cid, conp);
			if (sts != 0) {
				(void)fs_file_release(env, file, NULL);
				file = NULL;
			}
		}
	}
	*fp = file;
	return sts;
}

/*
 *  Release file descriptor obtained by fs_file_get()
 */
EXPORT	INT	fs_file_release(fs_env_t *env, fs_file_t *file, fs_cond_t *con)
{
	fs_cond_t	*cond;
	UINT		cid;
	INT		sts;

	sts = 0;
	cid = 0;

	fs_lock_lock(LOCKNUM_FIL);

	if (file->f_desc.d_refc == 1) {
		/* Last, close file */
		fs_descmgr_invalidate(&file_descmgr, &file->f_desc);
		cid = file->f.f_cid;
		file->f.f_cid = 0;
	}

	fs_lock_unlock(LOCKNUM_FIL);

	if (cid != 0) {
		/* Get connection descriptor */
		sts = fs_con_get(cid, &cond);
		if (sts == 0) {
			/* Call FIMP CLOSE service */
			env->t_cmd.r_close.fid = file->f.f_fid;
			env->t_cmd.r_close.oflags = file->f_desc.d_flags;
			sts = fs_callfimp(env, cond, FIMP_CLOSE);
			/* Release connection descriptor */
			fs_con_release(cond);
			/* Again, release connection desc got by fs_open */
			fs_con_release(cond);
		}
	}

	/* Release file descriptor */
	fs_descmgr_release(&file_descmgr, &file->f_desc);

	/* Release connection descriptor */
	if (con != NULL) {
		fs_con_release(con);
	}

	return sts;
}

/*
 *  Setup file descriptor
 */
EXPORT	void	fs_file_setup(fs_file_t * const file, INT fid,
							INT cid, INT flags)
{
	(void)fs_descmgr_validate(&file_descmgr, &file->f_desc);
	file->f_desc.d_refc = 1;
	file->f_desc.d_flags = (UH)flags;
	file->f.f_cid = cid;
	file->f.f_fid = fid;
}

