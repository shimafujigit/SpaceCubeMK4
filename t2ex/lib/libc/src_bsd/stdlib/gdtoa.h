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

/****************************************************************

The author of this software is David M. Gay.

Copyright (C) 1998 by Lucent Technologies
All Rights Reserved

Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appear in all
copies and that both that the copyright notice and this
permission notice and warranty disclaimer appear in supporting
documentation, and that the name of Lucent or any of its entities
not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

LUCENT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
IN NO EVENT SHALL LUCENT OR ANY OF ITS ENTITIES BE LIABLE FOR ANY
SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

****************************************************************/

/* Please send bug reports to David M. Gay (dmg at acm dot org,
 * with " at " changed at "@" and " dot " changed to ".").	*/

#ifndef GDTOA_H_INCLUDED
#define GDTOA_H_INCLUDED

#include "arith.h"
#include <stddef.h> /* for size_t */

#ifndef Long
#define Long int
#endif
#ifndef ULong
typedef unsigned Long ULong;
#endif
#ifndef UShort
typedef unsigned short UShort;
#endif

#ifndef ANSI
#ifdef KR_headers
#define ANSI(x) ()
#define Void /*nothing*/
#else
#define ANSI(x) x
#define Void void
#endif
#endif /* ANSI */

#ifndef CONST
#ifdef KR_headers
#define CONST /* blank */
#else
#define CONST const
#endif
#endif /* CONST */

 enum {	/* return values from strtodg */
	STRTOG_Zero	= 0x000,
	STRTOG_Normal	= 0x001,
	STRTOG_Denormal	= 0x002,
	STRTOG_Infinite	= 0x003,
	STRTOG_NaN	= 0x004,
	STRTOG_NaNbits	= 0x005,
	STRTOG_NoNumber	= 0x006,
	STRTOG_NoMemory = 0x007,
	STRTOG_Retmask	= 0x00f,

	/* The following may be or-ed into one of the above values. */

	STRTOG_Inexlo	= 0x010, /* returned result rounded toward zero */
	STRTOG_Inexhi	= 0x020, /* returned result rounded away from zero */
	STRTOG_Inexact	= 0x030,
	STRTOG_Underflow= 0x040,
	STRTOG_Overflow	= 0x080,
	STRTOG_Neg	= 0x100 /* does not affect STRTOG_Inexlo or STRTOG_Inexhi */
	};

 typedef struct
FPI {
	int nbits;
	int emin;
	int emax;
	int rounding;
	int sudden_underflow;
	} FPI;

enum {	/* FPI.rounding values: same as FLT_ROUNDS */
	FPI_Round_zero = 0,
	FPI_Round_near = 1,
	FPI_Round_up = 2,
	FPI_Round_down = 3
	};

#ifdef __cplusplus
extern "C" {
#endif

extern char* __dtoa  ANSI((double d, int mode, int ndigits, int *decpt,
			int *sign, char **rve));
extern char* __gdtoa ANSI((FPI *fpi, int be, ULong *bits, int *kindp,
			int mode, int ndigits, int *decpt, char **rve));
extern void __freedtoa ANSI((char*));
extern float  strtof ANSI((CONST char *, char **));
extern double strtod ANSI((CONST char *, char **));
extern int __strtodg ANSI((CONST char*, char**, FPI*, Long*, ULong*));

extern char*	__g_ddfmt  ANSI((char*, double*, int, size_t));
extern char*	__g_dfmt   ANSI((char*, double*, int, size_t));
extern char*	__g_ffmt   ANSI((char*, float*,  int, size_t));
extern char*	__g_Qfmt   ANSI((char*, void*,   int, size_t));
extern char*	__g_xfmt   ANSI((char*, void*,   int, size_t));
extern char*	__g_xLfmt  ANSI((char*, void*,   int, size_t));

extern int	__strtoId  ANSI((CONST char*, char**, double*, double*));
extern int	__strtoIdd ANSI((CONST char*, char**, double*, double*));
extern int	__strtoIf  ANSI((CONST char*, char**, float*, float*));
extern int	__strtoIQ  ANSI((CONST char*, char**, void*, void*));
extern int	__strtoIx  ANSI((CONST char*, char**, void*, void*));
extern int	__strtoIxL ANSI((CONST char*, char**, void*, void*));
extern int	__strtord  ANSI((CONST char*, char**, int, double*));
extern int	__strtordd ANSI((CONST char*, char**, int, double*));
extern int	__strtorf  ANSI((CONST char*, char**, int, float*));
extern int	__strtorQ  ANSI((CONST char*, char**, int, void*));
extern int	__strtorx  ANSI((CONST char*, char**, int, void*));
extern int	__strtorxL ANSI((CONST char*, char**, int, void*));
#if 1
extern int	__strtodI  ANSI((CONST char*, char**, double*));
extern int	__strtopd  ANSI((CONST char*, char**, double*));
extern int	__strtopdd ANSI((CONST char*, char**, double*));
extern int	__strtopf  ANSI((CONST char*, char**, float*));
extern int	__strtopQ  ANSI((CONST char*, char**, void*));
extern int	__strtopx  ANSI((CONST char*, char**, void*));
extern int	__strtopxL ANSI((CONST char*, char**, void*));
#else
#define __strtopd(s,se,x) strtord(s,se,1,x)
#define __strtopdd(s,se,x) strtordd(s,se,1,x)
#define __strtopf(s,se,x) strtorf(s,se,1,x)
#define __strtopQ(s,se,x) strtorQ(s,se,1,x)
#define __strtopx(s,se,x) strtorx(s,se,1,x)
#define __strtopxL(s,se,x) strtorxL(s,se,1,x)
#endif

#ifdef __cplusplus
}
#endif
#endif /* GDTOA_H_INCLUDED */
