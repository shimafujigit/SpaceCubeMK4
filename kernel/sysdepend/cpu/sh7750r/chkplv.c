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
 *	chkplv.c (SH7750R)
 *	Call Protected Level Test
 */

#include "sysmgr.h"

IMPORT INT	svc_call_limit;	/* SVC protected level (T-Kernel/OS) */

#pragma inline_asm(PPL)
static INT PPL( void )
{
	stc		r2_bank, r0
	shlr16	r0
	and		#3, r0
}

/*
 * Check protected level of extended SVC caller
 */
EXPORT ER ChkCallPLevel( void )
{
	return ( PPL() > svc_call_limit )? E_OACV: E_OK;
}
