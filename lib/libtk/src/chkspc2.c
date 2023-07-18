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
 *	@(#)chkspc2.c (libtk)
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
 * Check address space (TC string)
 *	Checks to see that the memory area from str through to
 *	either TNULL or the max character is valid.
 *	If max = 0, the number of characters (max) is ignored.
 * 	If the memory area is valid, the value returned is either
 *	the number of characters or the max value (through to the
 *	max character with max !=0 character and no TNULL).
 *	If the memory area is not valid, the error code is returned.
 */
#if defined(USE_FUNC_CHKSPACETSTRR)
EXPORT INT ChkSpaceTstrR( CONST TC *str, INT max )
{
	return ChkSpaceTstr(str, max, MA_READ, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACETSTRR */

#if defined(USE_FUNC_CHKSPACETSTRRW)
EXPORT INT ChkSpaceTstrRW( CONST TC *str, INT max )
{
	return ChkSpaceTstr(str, max, MA_READ|MA_WRITE, getsvcenv());
}
#endif /* USE_FUNC_CHKSPACETSTRRW */

#else /* VIRTUAL_ADDRESS */

#if defined(USE_FUNC_CHKSPACETSTRR)||defined(USE_FUNC_CHKSPACETSTRRW)
LOCAL INT chklen( CONST TC *p, INT max )
{
	INT	len = 0;

	while ( *p++ != TNULL ) {
		len++;
		if ( --max == 0 ) {
			break;
		}
	}
	return len;
}
#endif /* USE_FUNC_CHKSPACETSTRR or USE_FUNC_CHKSPACETSTRRW */

#if defined(USE_FUNC_CHKSPACETSTRR)
EXPORT INT ChkSpaceTstrR( CONST TC *str, INT max )
{
	return chklen(str, max);
}
#endif /* USE_FUNC_CHKSPACETSTRR */

#if defined(USE_FUNC_CHKSPACETSTRRW)
EXPORT INT ChkSpaceTstrRW( CONST TC *str, INT max )
{
	return chklen(str, max);
}
#endif /* USE_FUNC_CHKSPACETSTRRW */

#endif /* VIRTUAL_ADDRESS */
