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
 *    Changes: Adapted to the UCT T-Kernel 2.0
 *    Changed by UC Technology at 2013/01/29.
 *
 *    UCT T-Kernel 2.0 DevKit Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)bchkspc.c (libtk)
 *
 *	Address check
 */

#include <basic.h>
#include <sys/util.h>
#include <tk/syslib.h>

/*
 * Kernel configuration file
 */
#include "config/tk_config_func.h"

/*
 * Check address space
 *	Checks to see that the memory area from address to the
 *	len byte is valid.
 */
#if defined(USE_FUNC_CHKSPACER)
EXPORT ER CheckSpaceR( void *address, W len )
{
	return ChkSpaceR(address, len);
}
#endif /* USE_FUNC_CHKSPACER */

#if defined(USE_FUNC_CHKSPACERW)
EXPORT ER CheckSpaceRW( void *address, W len )
{
	return ChkSpaceRW(address, len);
}
#endif /* USE_FUNC_CHKSPACERW */

#if defined(USE_FUNC_CHKSPACERE)
EXPORT ER CheckSpaceRE( void *address, W len )
{
	return ChkSpaceRE(address, len);
}
#endif /* USE_FUNC_CHKSPACERE */
