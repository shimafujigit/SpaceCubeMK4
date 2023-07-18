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
 *	@(#)fastlock.c (libtk)
 *
 *	High-speed exclusive control lock
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/util.h>
#include <libstr.h>

/* ------------------------------------------------------------------------ */
/*
 *	Inc	Increment cnt, in result if cnt >  0, returns positive value.
 *		If cnt <= 0, returns 0 or negative value.
 *	Dec	Decrement cnt, in result if cnt >= 0, returns positive value.
 *		If cnt <  0, returns 0 or negative value.
 *	Increment/Decrement and evaluation of the associated result must
 *	be executed exclusively.
 */

#pragma inline(Inc)
static INT Inc( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = ++lock->cnt;
	EI(imask);
	return c;
}
#pragma inline(Dec)
static INT Dec( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = lock->cnt--;
	EI(imask);
	return c;
}

/* ------------------------------------------------------------------------ */

/*
 * Lock
 */
EXPORT void Lock( FastLock *lock )
{
	if ( Inc(lock) > 0 ) {
		tk_wai_sem(lock->id, 1, TMO_FEVR);
	}
}

/*
 * Lock release
 */
EXPORT void Unlock( FastLock *lock )
{
	if ( Dec(lock) > 0 ) {
		tk_sig_sem(lock->id, 1);
	}
}

/*
 * Create high-speed lock
 */
EXPORT ER CreateLock( FastLock *lock, CONST UB *name )
{
	T_CSEM	csem;
	ER	ercd;

	if ( name == NULL ) {
		csem.exinf = NULL;
	} else {
		strncpy((char*)&csem.exinf, (char*)name, sizeof(csem.exinf));
	}
	csem.sematr  = TA_TPRI | TA_NODISWAI;
	csem.isemcnt = 0;
	csem.maxsem  = 1;

	ercd = tk_cre_sem(&csem);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->id = ercd;
	lock->cnt = -1;

	return E_OK;
}

/*
 * Delete high-speed lock
 */
EXPORT void DeleteLock( FastLock *lock )
{
	if ( lock->id > 0 ) {
		tk_del_sem(lock->id);
	}
	lock->id = 0;
}
