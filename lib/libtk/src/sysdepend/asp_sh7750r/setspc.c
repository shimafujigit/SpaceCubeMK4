/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2013/01/29.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)setspc.c (libtk/SH7750R)
 *
 *	Address space control 
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

#include "getsvcenv.h"

/*
 * Kernel configuration file
 */
#include "../../../../../config/tk_config_func.h"

#if defined(USE_FUNC_SETTASKSPACE)
/*
 * Set task address space 
 */
EXPORT ER SetTaskSpace( ID taskid )
{
	UW	mdr;
	ER	ercd;

	if ( taskid == TSK_SELF ) {
		/* Set current CPL in MDR.RPL */
		mdr = getsvcenv();
		mdr ^= (mdr ^ (mdr << 16)) & MDR_PPL(3);
		setsvcenv( mdr );
	} else {
		T_EIT		r_eit;
		T_CREGS		r_cregs;
		T_TSKSPC	tskspc;

		/* Get logical space/MDR register for taskid tasks */
		ercd = tk_get_reg(taskid, NULL, &r_eit, &r_cregs);
		if ( ercd < E_OK ) {
			goto err_ret;
		}

		/* Change to logical space for nominated tasks */
		tskspc.uatb = r_cregs.uatb;
		tskspc.lsid = r_cregs.lsid;
		ercd = tk_set_tsp(TSK_SELF, &tskspc);
		if ( ercd < E_OK ) {
			goto err_ret;
		}

		/* Change to MDR.PPL for nominated tasks */
		mdr = getsvcenv();
		mdr ^= (mdr ^ r_eit.mdr) & MDR_PPL(3);
		setsvcenv( mdr );
	}

	return E_OK;

err_ret:
	return ercd;
}
#endif /* USE_FUNC_SETTASKSPACE */
