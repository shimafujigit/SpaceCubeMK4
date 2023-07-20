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
 *	patch.c (ASP-LEON5)
 *	System-dependent initialize process
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <sys/sysinfo.h>
#include "patch.h"

EXPORT	SysCommonArea	syscommonarea;

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
