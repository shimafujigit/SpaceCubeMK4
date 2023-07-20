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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)setspc.c (libtk/ASP-LEON5)
 *
 *	Address space control 
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

/*
 * Kernel configuration file
 */
#include "config/tk_config_func.h"

#if defined(USE_FUNC_SETTASKSPACE)
/*
 * Set task address space 
 */
EXPORT ER SetTaskSpace( ID taskid )
{
	/* No task space, dummy */
	return E_OK;
}
#endif /* USE_FUNC_SETTASKSPACE */
