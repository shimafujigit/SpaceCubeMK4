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
 *	@(#)tkn_spl.c
 *
 */

/* System Priority Level(SPL) control */

/*
 * Alternative for SPL
 *
 * These function do not simulate SPL completely, just provide simple mutual
 * exclusion.
 */

#include <tk/tkernel.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/intr.h>

#include <tk/util.h>

#include "../tkn.h"

static ID	spl_mtxid;
static int	spl_level;

EXPORT INT	tkn_task_base_pri;

int
tkn_spl_lock(int level)
{
	int	oldlevel;

	if ( level < 0 ) {
		panic("tkn_spl_lock: invalid level %d.\n", level);
	}

	if ( level > 0 ) {
		tk_loc_mtx(spl_mtxid, TMO_FEVR);
	}

	LockSPL();

	oldlevel = spl_level;
	if ( level > spl_level ) {
		spl_level = level;
	}

	UnlockSPL();

	return oldlevel;
}

int
tkn_spl_unlock(int level)
{
	int oldlevel;

	if ( level < 0 ) {
		panic("tkn_spl_unlock: invalid level %d.\n", level);
	}

	LockSPL();

	oldlevel = spl_level;
	if ( level < spl_level ) {
		spl_level = level;
	}

	if ( spl_level == 0 ) {
		tk_unl_mtx(spl_mtxid);
	}

	UnlockSPL();

	return oldlevel;
}

/* initialize spl system */
EXPORT ER
tkn_spl_init(void)
{
	T_CMTX	cmtx;
	ER	err;

	SetOBJNAME(cmtx.exinf, "Nspl");
	cmtx.mtxatr = TA_INHERIT;
	err = tk_cre_mtx(&cmtx);
	if ( err < E_OK ) {
		goto err_ret;
	}

	spl_mtxid = err;
	spl_level = 0;

	return E_OK;

err_ret:
	return err;
}

EXPORT ER tkn_spl_finish(void)
{
	ER ercd = tk_del_mtx(spl_mtxid);
	spl_level = 0;
	spl_mtxid = 0;

	return ercd;
}

int
tkn_spl_priority(int level)
{
	return tkn_task_base_pri + (NIPL - 1) - level;
}
