/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*
 *	@(#)util.h (T2EX)
 *
 *	General Utilities
 */

#ifndef	__T2EX_UTIL_H__
#define __T2EX_UTIL_H__

#include <basic.h>
#include <tk/typedef.h>
#include <tk/util.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * User-mode fast Lock
 */
typedef struct {
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
#endif /* __T2EX_UTIL_H__ */
