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
 *	@(#)fs_regist.c
 *
 */

#include "fsdefs.h"

/*
 *  Check FIMP name : returns 0: valid, -1: invalid
 */
EXPORT	INT	fs_check_fimpnm(const B *fimpnm)
{
	INT	i;
	B	c;

	if (fimpnm != NULL) {
		/* valid characters are 0-9 A-Z a-z _ */
		for (i = 0; (c = fimpnm[i]) != '\0' && i < L_FIMPNM; i++) {
			if (	(c >= '0' && c <= '9') ||
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c == '_' ) )continue;
			break;
		}
		if (i > 0 && c == '\0') return 0;
	}
	return -1;
}

/*
 *  fs_regist() - FIMP registration
 */
EXPORT	INT	xfs_regist(fs_env_t *env, const B *fimpnm, 
			const fs_fimp_t *fimp, void *exinf, void *tsd)
{
	INT		sts;
	fs_fimpd_t	*fimpd;

	/* Check FIMP name */
	if (fs_check_fimpnm(fimpnm) != 0) {
		sts = EX_INVAL;
		goto exit0;
	}

	/* Regist FIMP */
	sts = fs_fimp_regist(fimpnm, &fimpd);
	if (sts != 0) goto exit0;

	/* Initialize FIMP descriptor */
	(void)memcpy(&fimpd->p_fimp, fimp, sizeof(fs_fimp_t));

	/* Call FIMP registration service */
	fimpd->p_fimpinf.fimpsd = tsd;
	sts = fimp->registfn(&fimpd->p_fimpinf, exinf);

	/* End of registration */
	fs_fimp_end_regist(fimpd, sts);
	if (sts == 0) return 0;
exit0:
	env->t_errno = sts;
	return -1;
}

