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
 *	chkplv.c (ASP-LEON5)
 *	Call Protected Level Test
 */

#include "sysmgr.h"

IMPORT INT	svc_call_limit;	/* SVC protected level (T-Kernel/OS) */

#if 0
static INT PPL( void )
{
	return 0; 	/* Fixed */
}
#endif

/*
 * Check protected level of extended SVC caller
 */
EXPORT ER ChkCallPLevel( void )
{
	// return ( PPL() > svc_call_limit )? E_OACV: E_OK;
	return E_OK;	/* Because PPL() = always 0 */
}
