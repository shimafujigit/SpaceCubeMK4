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
 */

/*
 *	bitop.h (tstdlib)
 *
 *	T-Kernel common standard library
 *
 */

#ifndef	_BITOP_
#define _BITOP_

#include <stdtype.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef	__size_t
typedef __size_t	size_t;
#undef	__size_t
#endif

#ifdef	__wchar_t
typedef __wchar_t	wchar_t;
#undef	__wchar_t
#endif

#define NULL		0

extern void tstdlib_bitclr( void *base, W offset );
extern void tstdlib_bitset( void *base, W offset );
extern BOOL tstdlib_bittest( void *base, W offset );
 
extern W tstdlib_bitsearch0( void *base, W offset, W width );
extern W tstdlib_bitsearch1( void *base, W offset, W width );
extern W tstdlib_bitsearch1_binsearch( UW* base, W offset, W width );

#ifdef __cplusplus
}
#endif
#endif /* _BITOP_ */
