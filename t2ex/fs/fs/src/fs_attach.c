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
 *	@(#)fs_attach.c
 *
 */

#include "fsdefs.h"

/*
 *  Check connection point name : returns 0: valid, -1: invalid
 */
EXPORT	INT	fs_check_connm(const B *connm)
{
	INT	i;
	UB	c;

	if (connm != NULL) {
		/* Valid characters are 0-9 A-Z a-z _ */
		for (i = 0; (c = (UB)connm[i]) != '\0' && i < L_CONNM; i++) {
			if (	(c >= '0' && c <= '9') ||
				(c >= 'a' && c <= 'z') ||
				(c >= 'A' && c <= 'Z') ||
				(c == '_' ) )continue;
			break;
		}
		if (i > 0 && c == '\0') return 0;
		/* Special for "root" == "/" */
		if (connm[0] == '/' && connm[1] == '\0') return 0;
	}
	return -1;
}

/*
 *  Check device name : returns 0: valid, -1: invalid
 */
LOCAL	INT	fs_check_devnm(const B *devnm)
{
	/* devnm == NULL is OK (for "root" ) */
	return (devnm != NULL && strlen(devnm) > L_DEVNM) ? -1 : 0;
}

/*
 *  fs_attach() - Attach device as a file system
 */
EXPORT	INT	xfs_attach(fs_env_t *env,const B *devnm, const B *connm,
				const B *fimpnm, INT flags, void *exinf)
{
	INT		sts;
	fs_fimpd_t	*fimpd;
	fs_cond_t	*cond;

	/* Check parameters */
	if (fs_check_devnm(devnm) != 0 || fs_check_connm(connm) != 0 ||
					fs_check_fimpnm(fimpnm) != 0) {
		sts = EX_INVAL;
		goto exit0;
	}

	if (strcmp(fimpnm, FIMP_AUTODETECT) == 0) {
		/* FIMP_AUTODETECT isn't supported */
		sts = EX_NOTSUP;
		goto exit0;
	}

	/* Find & Get FIMP descriptor */
	sts = fs_fimp_find(fimpnm, &fimpd);
	if (sts != 0) goto exit0;

	/* Regist connection point name */
	sts = fs_con_regist(connm, &cond);
	if (sts != 0) goto exit1;

	/* Initialize connection descriptor */
	cond->c_fimpsd = fimpd;
	cond->c_coninf.fimpinf = &fimpd->p_fimpinf;
	cond->c_coninf.dflags = flags;
	cond->c_coninf.consd = NULL;
	if (devnm == NULL) {	/* for "root" */
		cond->c_coninf.devnm[0] = '\0';
	} else {
		strcpy(cond->c_coninf.devnm, devnm);
	}

	/* Call FIMP attach service */
	sts = fimpd->p_fimp.attachfn(&cond->c_coninf, exinf);

	/* End of registration */
	fs_con_end_regist(cond, sts);

	if (sts == 0) return 0;
exit1:
	/* Release FIMP descriptor */
	fs_fimp_release(fimpd);
exit0:
	env->t_errno = sts;
	return -1;
}

