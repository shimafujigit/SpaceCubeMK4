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
 *	@(#)chkspc3.c (libtk)
 *
 *	Address check
 */

#include <basic.h>
#include <tk/syslib.h>
#include <sys/segment.h>

/*
 * Kernel configuration file
 */
#include "config/tk_config_func.h"

#if VIRTUAL_ADDRESS

#include "getsvcenv.h"

/*
 * Check address space (B string)
 *	Checks to see that the memory area from str through to either
 *	'\0' or the max byte is valid.
 *	If max = 0, the number of bytes (max) is ignored.
 *	If the memory area is valid, the value returned is either the
 *	number of bytes or the max value (through to the max byte with
 *	max !=0 and no '\0').
 *	If the memory area is not valid, the error code is returned.
 */
#if defined(USE_FUNC_CHKSPACEBSTRR)
EXPORT INT ChkSpaceBstrR( CONST UB *str, INT max )
{
	return ChkSpaceBstr(str, max, MA_READ, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACEBSTRR */

#if defined(USE_FUNC_CHKSPACEBSTRRW)
EXPORT INT ChkSpaceBstrRW( CONST UB *str, INT max )
{
	return ChkSpaceBstr(str, max, MA_READ|MA_WRITE, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACEBSTRRW */

#else /* VIRTUAL_ADDRESS */

#if defined(USE_FUNC_CHKSPACEBSTRR)||defined(USE_FUNC_CHKSPACEBSTRRW)
LOCAL INT chklen( CONST UB *p, INT max )
{
	INT	len = 0;

	while ( *p++ != '\0' ) {
		len++;
		if ( --max == 0 ) {
			break;
		}
	}
	return len;
}
#endif /* USE_FUNC_CHKSPACEBSTRR or USE_FUNC_CHKSPACEBSTRRW */

#if defined(USE_FUNC_CHKSPACEBSTRR)
EXPORT INT ChkSpaceBstrR( CONST UB *str, INT max )
{
	return chklen(str, max);
}
#endif /* USE_FUNC_CHKSPACEBSTRR */

#if defined(USE_FUNC_CHKSPACEBSTRRW)
EXPORT INT ChkSpaceBstrRW( CONST UB *str, INT max )
{
	return chklen(str, max);
}
#endif /* USE_FUNC_CHKSPACEBSTRRW */

#endif /* VIRTUAL_ADDRESS */
