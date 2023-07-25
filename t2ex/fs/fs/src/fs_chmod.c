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
 *	@(#)fs_chmod.c
 *
 */

#include "fsdefs.h"

/*
 *  fs_chmod() - Change file mode
 */
EXPORT	INT	xfs_chmod(fs_env_t *env, const B *name, mode_t mode)
{
	fs_path_t	path;
	INT		sts;

	/* Parse the pathname & get path structure */
	sts = fs_path_parse(env, name, &path, FALSE);
	if (sts != 0) goto exit0;

	/* Check mode parameter */
	if ((mode & ~(S_ISUID | S_ISGID | S_ISVTX | S_IRWXU |
						S_IRWXG | S_IRWXO)) != 0) {
		sts = EX_INVAL;
	} else {
		/* Call FIMP CHMODE service */
		env->t_cmd.r_chmod.path = path.p_name;
		env->t_cmd.r_chmod.mode = mode;
		sts = fs_callfimp(env, path.p_con, FIMP_CHMOD);
	}
	/* Release path structure */
	fs_path_release(env, &path);
	if (sts == 0) return 0; 
exit0:
	env->t_errno = sts;
	return -1;
}

