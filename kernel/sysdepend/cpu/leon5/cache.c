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
 *    Copyright (c) 2017 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	cache.c (ASP-LEON5)
 *	Cache Operation
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

#include <bcc/bcc.h>

EXPORT INT GetCacheLineSize( void )
{
	return	32;
}

EXPORT void FlushCache( VP laddr, INT len )
{
	bcc_flush_cache();
}

/*
 * mode := (TCM_ICACHE | TCM_DCACHE)
 */
EXPORT void FlushCacheM( VP laddr, INT len, UINT mode )
{
	switch ( mode ) {
	  case TCM_ICACHE:	bcc_flush_icache();	break;
	  case TCM_DCACHE:	bcc_flush_dcache();	break;
	  default:		bcc_flush_cache();
	}
}

/*
 * mode := (CC_FLUSH | CC_INVALIDATE)
 */
EXPORT ER ControlCacheM( void *laddr, INT len, UINT mode )
{
	bcc_flush_cache();

	return E_OK;
}
