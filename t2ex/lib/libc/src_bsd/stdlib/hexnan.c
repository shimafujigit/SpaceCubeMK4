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

Copyright (C) 2000 by Lucent Technologies
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

 static void
#ifdef KR_headers
L_shift(x, x1, i) ULong *x; ULong *x1; int i;
#else
L_shift(ULong *x, ULong *x1, int i)
#endif
{
	int j;

	i = 8 - i;
	i <<= 2;
	j = ULbits - i;
	do {
		*x |= x[1] << j;
		x[1] >>= i;
		} while(++x < x1);
	}

 int
#ifdef KR_headers
hexnan(sp, fpi, x0)
	CONST char **sp; FPI *fpi; ULong *x0;
#else
hexnan( CONST char **sp, FPI *fpi, ULong *x0)
#endif
{
	ULong c, h, *x, *x1, *xe;
	CONST char *s;
	int havedig, hd0, i, nbits;

	if (!hexdig['0'])
		hexdig_init_D2A();
	nbits = fpi->nbits;
	x = x0 + (nbits >> kshift);
	if (nbits & kmask)
		x++;
	*--x = 0;
	x1 = xe = x;
	havedig = hd0 = i = 0;
	s = *sp;
	/* allow optional initial 0x or 0X */
	while((c = *(CONST unsigned char*)(s+1)) && c <= ' ')
		++s;
	if (s[1] == '0' && (s[2] == 'x' || s[2] == 'X')
	 && *(CONST unsigned char*)(s+3) > ' ')
		s += 2;
	while((c = *(CONST unsigned char*)++s)) {
		if (!(h = hexdig[c])) {
			if (c <= ' ') {
				if (hd0 < havedig) {
					if (x < x1 && i < 8)
						L_shift(x, x1, i);
					if (x <= x0) {
						i = 8;
						continue;
						}
					hd0 = havedig;
					*--x = 0;
					x1 = x;
					i = 0;
					}
				while(*(CONST unsigned char*)(s+1) <= ' ')
					++s;
				if (s[1] == '0' && (s[2] == 'x' || s[2] == 'X')
				 && *(CONST unsigned char*)(s+3) > ' ')
					s += 2;
				continue;
				}
			if (/*(*/ c == ')' && havedig) {
				*sp = s + 1;
				break;
				}
#ifndef GDTOA_NON_PEDANTIC_NANCHECK
			do {
				if (/*(*/ c == ')') {
					*sp = s + 1;
					break;
					}
				} while((c = *++s));
#endif
			return STRTOG_NaN;
			}
		havedig++;
		if (++i > 8) {
			if (x <= x0)
				continue;
			i = 1;
			*--x = 0;
			}
		*x = (*x << 4) | (h & 0xf);
		}
	if (!havedig)
		return STRTOG_NaN;
	if (x < x1 && i < 8)
		L_shift(x, x1, i);
	if (x > x0) {
		x1 = x0;
		do *x1++ = *x++;
			while(x <= xe);
		do *x1++ = 0;
			while(x1 <= xe);
		}
	else {
		/* truncate high-order word if necessary */
		if ( (i = nbits & (ULbits-1)) !=0)
			*xe &= ((ULong)0xffffffff) >> (ULbits - i);
		}
	for(x1 = xe;; --x1) {
		if (*x1 != 0)
			break;
		if (x1 == x0) {
			*x1 = 1;
			break;
			}
		}
	return STRTOG_NaNbits;
	}
