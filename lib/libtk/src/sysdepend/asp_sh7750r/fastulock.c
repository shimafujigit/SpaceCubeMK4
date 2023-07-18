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
 *    Changed by UC Technology at 2013/01/28.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)fastulock.c (libtk)
 *
 *	High-speed exclusive control lock in user-mode
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

#pragma inline_asm(INC)
static int INC( FastULock *lock )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(1*4, r4), r0		; cnt
	add	#1, r0			; return ++cnt
	mov.l	r0, @(1*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

#pragma inline_asm(DEC)
static int DEC( FastULock *lock )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(1*4, r4), r0		; cnt
	mov	r0, r1			; return cnt--
	add	#-1, r1
	mov.l	r1, @(1*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

/* ------------------------------------------------------------------------ */

/*
 * Lock
 */
EXPORT void ULock( FastULock *lock )
{
	if ( INC(lock) > 0 ) {
		tk_wai_sem(lock->id, 1, TMO_FEVR);
	}
}

/*
 * Lock release
 */
EXPORT void UUnlock( FastULock *lock )
{
	if ( DEC(lock) > 0 ) {
		tk_sig_sem(lock->id, 1);
	}
}

/*
 * Create high-speed lock
 */
EXPORT ER CreateULock( FastULock *lock, CONST UB *name )
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
	lock->misc = 0;

	return E_OK;
}

/*
 * Delete high-speed lock
 */
EXPORT void DeleteULock( FastULock *lock )
{
	if ( lock->id > 0 ) {
		tk_del_sem(lock->id);
	}
	lock->id = 0;
}
