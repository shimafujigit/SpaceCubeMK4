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
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	patch.c (SH7750R)
 *	System-dependent initialize process
 */

#include <basic.h>
#include <tk/tkernel.h>
#include "patch.h"

#if USE_SYSDEPEND_PATCH1
/*
 * System-dependent processes (before start_system)
 */
EXPORT void sysdepend_patch1( void )
{
	/* Do nothing */
}
#endif

#if USE_SYSDEPEND_PATCH2
/*
 * System-dependent processes (after start_system)
 */
EXPORT void sysdepend_patch2( void )
{
	/* Do nothing */
}
#endif
