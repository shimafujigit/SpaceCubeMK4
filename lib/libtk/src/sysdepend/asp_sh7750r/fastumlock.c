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
 *	@(#)fastumlock.c (libtk)
 *
 *	High-speed exclusive control multi-lock in user-mode
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/util.h>
#include <libstr.h>

/* ------------------------------------------------------------------------ */
/*
 *	void INC( INT *val )		increment
 *	void DEC( INT *val )		decrement
 *	BOOL BTS( UINT *val, INT no )	bit test and set
 *	void BR( UINT *val, INT no )	bit reset
 *
 *	The above must be operated exclusively.
 */

#pragma inline_asm(INC)
static void INC( FastUMLock *lock )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(2*4, r4), r0		; wai
	add	#1, r0			; wai++
	mov.l	r0, @(2*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

#pragma inline_asm(DEC)
static void DEC( FastUMLock *lock )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(2*4, r4), r0		; wai
	add	#-1, r0			; wai--
	mov.l	r0, @(2*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

#pragma inline_asm(BTS)
static BOOL BTS( FastUMLock *lock, INT no )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(1*4, r4), r0		; flg
	mov	#1, r1
	shld	r5, r1			; 1 << no
	mov	r0, r2
	and	r1, r0			; return flg & (1 << no)
	or	r1, r2			; flg |= (1 << no)
	mov.l	r2, @(1*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

#pragma inline_asm(BR)
static void BR( FastUMLock *lock, INT no )
{
?_loop:
	tas.b	@r4			; test & set misc
	bf	?_loop
;
	mov.l	@(1*4, r4), r0		; flg
	mov	#1, r1
	shld	r5, r1			; 1 << no
	not	r1, r1
	and	r1, r0			; flg &= ~(1 << (no))
	mov.l	r0, @(1*4, r4)
;
	mov	#0, r1			; clear misc
	mov.b	r1, @r4
}

/* ------------------------------------------------------------------------ */

/*
 * Lock with wait time designation
 *	no	lock number 0 - 31
 */
EXPORT ER UMLockTmo( FastUMLock *lock, INT no, TMO tmo )
{
	UINT	ptn = (UINT)(1 << no);
	UINT	flg;
	ER	ercd;

	INC(lock);
	for ( ;; ) {
		if ( !BTS(lock, no) ) {
			ercd = E_OK;
			break;
		}

		ercd = tk_wai_flg(lock->id, ptn, TWF_ORW|TWF_BITCLR, &flg, tmo);
		if ( ercd < E_OK ) {
			break;
		}
	}
	DEC(lock);

	return ercd;
}
EXPORT ER UMLockTmo_u( FastUMLock *lock, INT no, TMO_U tmo )
{
	UINT	ptn = (UINT)(1 << no);
	UINT	flg;
	ER	ercd;

	INC(lock);
	for ( ;; ) {
		if ( !BTS(lock, no) ) {
			ercd = E_OK;
			break;
		}

		ercd = tk_wai_flg_u(lock->id, ptn, TWF_ORW|TWF_BITCLR, &flg, tmo);
		if ( ercd < E_OK ) {
			break;
		}
	}
	DEC(lock);

	return ercd;
}

/*
 * Lock
 *	no	Lock number 0 - 31
 */
EXPORT ER UMLock( FastUMLock *lock, INT no )
{
	return UMLockTmo(lock, no, TMO_FEVR);
}

/*
 * Lock release
 *	no	Lock number 0 - 31
 */
EXPORT ER UMUnlock( FastUMLock *lock, INT no )
{
	UINT	ptn = (UINT)(1 << no);
	ER	ercd;

	BR(lock, no);
	ercd = ( lock->wai == 0 )? E_OK: tk_set_flg(lock->id, ptn);

	return ercd;
}

/*
 * Create multi-lock
 */
EXPORT ER CreateUMLock( FastUMLock *lock, CONST UB *name )
{
	T_CFLG	cflg;
	ER	ercd;

	if ( name == NULL ) {
		cflg.exinf = NULL;
	} else {
		strncpy((char*)&cflg.exinf, (char*)name, sizeof(cflg.exinf));
	}
	cflg.flgatr  = TA_TPRI | TA_WMUL | TA_NODISWAI;
	cflg.iflgptn = 0;

	lock->id = ercd = tk_cre_flg(&cflg);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->wai = 0;
	lock->flg = 0;
	lock->misc = 0;

	return E_OK;
}

/*
 * Delete multi-lock
 */
EXPORT ER DeleteUMLock( FastUMLock *lock )
{
	ER	ercd;

	if ( lock->id <= 0 ) {
		return E_PAR;
	}

	ercd = tk_del_flg(lock->id);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->id = 0;

	return E_OK;
}
