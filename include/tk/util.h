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
 *	@(#)util.h (T-Kernel)
 *
 *	General Utilities
 */

#ifndef	__TK_UTIL_H__
#define __TK_UTIL_H__

#include <basic.h>
#include <tk/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Fast Lock
 */
typedef struct {
	W	cnt;
	ID	id;
} FastLock;

IMPORT ER CreateLock( FastLock *lock, CONST UB *name );
IMPORT void DeleteLock( FastLock *lock );
IMPORT void Lock( FastLock *lock );
IMPORT void Unlock( FastLock *lock );

/*
 * Multi Lock
 *	Can use the maximum of 32 independent locks with a single FastMLock.
 *	Divided by the lock number (no). Can specify 0-31 for 'no.'
 *	(Slightly less efficient than FastLock)
 */
typedef struct {
	UINT	flg;
	INT	wai;
	ID	id;
} FastMLock;

IMPORT ER CreateMLock( FastMLock *lock, CONST UB *name );
IMPORT ER DeleteMLock( FastMLock *lock );
IMPORT ER MLockTmo( FastMLock *lock, INT no, TMO tmout );
IMPORT ER MLockTmo_u( FastMLock *lock, INT no, TMO_U tmout_u );
IMPORT ER MLock( FastMLock *lock, INT no );
IMPORT ER MUnlock( FastMLock *lock, INT no );

/*
 * 4-character object name
 *	(Example)
 *	T_CTSK	ctsk;
 *	SetOBJNAME(ctsk.exinf, "TEST");
 */
union objname {
	char	s[4];
	void	*i;
};

#define SetOBJNAME(exinf, name)					\
	{							\
		const static union objname _nm = { name };	\
		exinf = _nm.i;					\
	}

/*
 * User-mode fast Lock
 */
typedef struct {
	UINT	misc;
	INT	cnt;
	ID	id;
} FastULock;

IMPORT ER CreateULock( FastULock *lock, CONST UB *name );
IMPORT void DeleteULock( FastULock *lock );
IMPORT void ULock( FastULock *lock );
IMPORT void UUnlock( FastULock *lock );

/*
 * User-mode multi Lock
 *	Can use the maximum of 32 independent locks with a single FastUMLock.
 *	Divided by the lock number (no). Can specify 0-31 for 'no.'
 *	(Slightly less efficient than FastULock)
 */
typedef struct {
	UINT	misc;
	UINT	flg;
	UINT	wai;
	ID	id;
} FastUMLock;

IMPORT ER CreateUMLock( FastUMLock *lock, CONST UB *name );
IMPORT ER DeleteUMLock( FastUMLock *lock );
IMPORT ER UMLockTmo( FastUMLock *lock, INT no, TMO tmout );
IMPORT ER UMLockTmo_u( FastUMLock *lock, INT no, TMO_U tmout_u );
IMPORT ER UMLock( FastUMLock *lock, INT no );
IMPORT ER UMUnlock( FastUMLock *lock, INT no );

#ifdef __cplusplus
}
#endif
#endif /* __TK_UTIL_H__ */
