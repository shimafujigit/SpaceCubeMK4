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
 *	service.c
 *
 *       T2EX: program load functions
 *       manager initialization
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <t2ex/load.h>
#include "ifload.h"
#include "service.h"
#include "pminfo.h"

IMPORT ER ChkT2EXLevel( void );

/* Manager lock */
EXPORT	FastLock	pmLock;

/*
 * Service call entries
 */
LOCAL	ER	svcEntry(void* para, W fn)
{
	ER	er;

	er = ChkT2EXLevel();
	if (er < E_OK) {
		return er;
	}

	switch (fn) {
	case PM_PM_LOAD_FN: {
		PM_PM_LOAD_PARA* p = (PM_PM_LOAD_PARA*)para;
		er = _pm_load(p->prog, p->attr, p->entry);
		break;
	}
	case PM_PM_LOADSPG_FN: {
		PM_PM_LOADSPG_PARA* p = (PM_PM_LOADSPG_PARA*)para;
		er = _pm_loadspg(p->prog, p->ac, p->av);
		break;
	}
	case PM_PM_STATUS_FN: {
		PM_PM_STATUS_PARA* p = (PM_PM_STATUS_PARA*)para;
		er = _pm_status(p->progid, p->status);
		break;
	}
	case PM_PM_UNLOAD_FN: {
		PM_PM_UNLOAD_PARA* p = (PM_PM_UNLOAD_PARA*)para;
		er = _pm_unload(p->progid);
		break;
	}
	default:
		er = E_RSFN;
		break;
	}

	return er;
}

/*
 * Program load functions entry
 */
EXPORT	ER	pm_main(INT ac, UB* av[])
{
	ER	er;
	T_DSSY	dssy;
	INT	maxprog;

	if (ac < 0) {
		/* Stop other tasks from executing SVC */
		Lock(&pmLock);

		/* Unregister subsystem */
		tk_def_ssy(PM_SVC, NULL);

		/* Cleanup control block */
		pmCleanupInfo();

		/* Delete lock */
		DeleteLock(&pmLock);
		return E_OK;
	}

	/* Create lock */
	er = CreateLock(&pmLock, "pmLk");
	if ( er < E_OK ) {
		goto err_ret0;
	}

	/* Initialize control block */
	er = tk_get_cfn((UB*)"PmMaxProg", &maxprog, 1);
	if ( er <= 0 || maxprog < 0 ) maxprog = DEFAULT_MAX_PROG;

	er = pmInitInfo(maxprog);
	if ( er < E_OK ) {
		goto err_ret1;
	}

	/* Register subsystem */
	dssy.ssyatr	= TA_NULL;
	dssy.ssypri	= PM_PRI;
	dssy.svchdr	= (FP)svcEntry;
	dssy.breakfn	= NULL;
	dssy.startupfn	= NULL;
	dssy.cleanupfn	= NULL;
	dssy.eventfn	= NULL;
	dssy.resblksz	= 0;
	er = tk_def_ssy(PM_SVC, &dssy);
	if ( er < E_OK ) {
		goto err_ret2;
	}

	return E_OK;

err_ret2:
	pmCleanupInfo();
err_ret1:
	DeleteLock(&pmLock);
err_ret0:
	return er;
}
