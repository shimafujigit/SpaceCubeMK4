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
 *	@(#)chkspc.c (libtk)
 *
 *	Address check
 */

#include <basic.h>
#include <tk/syslib.h>
#include <sys/segment.h>

/*
 * Kernel configuration file
 */
#include "../../../../../config/tk_config_func.h"

#if VIRTUAL_ADDRESS

#include "getsvcenv.h"

/*
 * Check address space
 *	Checks to see that the memory space from address to the
 *	len byte is valid.
 */
#if defined(USE_FUNC_CHKSPACER)
EXPORT ER ChkSpaceR( CONST void *addr, INT len )
{
	return ChkSpace(addr, len, MA_READ, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACER */

#if defined(USE_FUNC_CHKSPACERW)
EXPORT ER ChkSpaceRW( CONST void *addr, INT len )
{
	return ChkSpace(addr, len, MA_READ|MA_WRITE, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACERW */

#if defined(USE_FUNC_CHKSPACERE)
EXPORT ER ChkSpaceRE( CONST void *addr, INT len )
{
	return ChkSpace(addr, len, MA_READ|MA_EXECUTE, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACERE */

#else /* VIRTUAL_ADDRESS */

#if defined(USE_FUNC_CHKSPACER)
EXPORT ER ChkSpaceR( CONST void *addr, INT len )
{
	return E_OK;
}
#endif /* USE_FUNC_CHKSPACER */

#if defined(USE_FUNC_CHKSPACERW)
EXPORT ER ChkSpaceRW( CONST void *addr, INT len )
{
	return E_OK;
}
#endif /* USE_FUNC_CHKSPACERW */

#if defined(USE_FUNC_CHKSPACERE)
EXPORT ER ChkSpaceRE( CONST void *addr, INT len )
{
	return E_OK;
}
#endif /* USE_FUNC_CHKSPACERE */

#endif /* VIRTUAL_ADDRESS */
