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

Copyright (C) 1998, 2000 by Lucent Technologies
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

#include "gdtoaimp.h"

 float
#ifdef KR_headers
strtof(s, sp) CONST char *s; char **sp;
#else
strtof(CONST char *s, char **sp)
#endif
{
	static FPI fpi0 = { 24, 1-127-24+1,  254-127-24+1, 1, SI };
	ULong bits[1];
	Long exp;
	int k;
	union { ULong L[1]; float f; } u;
#ifdef Honor_FLT_ROUNDS
#include "gdtoa_fltrnds.h"
#else
#define fpi &fpi0
#endif

	k = strtodg(s, sp, fpi, &exp, bits);
	switch(k & STRTOG_Retmask) {
	  case STRTOG_NoNumber:
	  case STRTOG_Zero:
		u.L[0] = 0;
		break;

	  case STRTOG_Normal:
	  case STRTOG_NaNbits:
		u.L[0] = (bits[0] & 0x7fffff) | ((exp + 0x7f + 23) << 23);
		break;

	  case STRTOG_Denormal:
		u.L[0] = bits[0];
		break;

	  case STRTOG_NoMemory:
		/*errno = ERANGE*/;
		/* FALLTHROUGH */
	  case STRTOG_Infinite:
		u.L[0] = 0x7f800000;
		break;

	  case STRTOG_NaN:
		u.L[0] = f_QNAN;
	  }
	if (k & STRTOG_Neg)
		u.L[0] |= 0x80000000L;
	return u.f;
	}
