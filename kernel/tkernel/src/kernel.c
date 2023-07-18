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
 *	kernel.c (T-Kernel/OS)
 *	T-Kernel Common Definition
 */

#include <basic.h>
#include "kernel.h"


/*
 * Object lock
 *	Locked task is the highest run priority.
 *	Unable to nest lock.
 */
#pragma inline(InitOBJLOCK)
static void InitOBJLOCK( OBJLOCK *loc )
{
	loc->wtskq.next = NULL;
}

#pragma inline(isLockedOBJ)
static BOOL isLockedOBJ( OBJLOCK *loc )
{
	return ( loc->wtskq.next != NULL )? TRUE: FALSE;
}

/*
 * Extended SVC lock
 *	The priority does not change during lock.
 *	Able to nest lock.
 */
#pragma inline(InitSVCLOCK)
static void InitSVCLOCK( SVCLOCK *loc )
{
	loc->wtskq.next = NULL;
}

