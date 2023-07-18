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
 *	@(#)getsvcenv.c (libtk/SH7750R)
 *
 *	Get extended SVC call environment 
 */

#include <basic.h>
#include "getsvcenv.h"

#pragma inline_asm(getsvcenv)
static UW getsvcenv( void )
{
	stc		r2_bank, r0
}

#pragma inline_asm(setsvcenv)
static void setsvcenv( UW mdr )
{
	ldc		r4, r2_bank
}
