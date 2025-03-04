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

/*	$OpenBSD: s_casinf.c,v 1.3 2011/07/20 19:28:33 martynas Exp $	*/
/*
 * Copyright (c) 2008 Stephen L. Moshier <steve@moshier.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*							casinf()
 *
 *	Complex circular arc sine
 *
 *
 *
 * SYNOPSIS:
 *
 * void casinf();
 * cmplxf z, w;
 *
 * casinf( &z, &w );
 *
 *
 *
 * DESCRIPTION:
 *
 * Inverse complex sine:
 *
 *                               2
 * w = -i clog( iz + csqrt( 1 - z ) ).
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -10,+10     30000       1.1e-5      1.5e-6
 * Larger relative error can be observed for z near zero.
 *
 */

#include <complex.h>
#include <math.h>

float complex
casinf(float complex z)
{
	float complex w;
	float x, y;
	static float complex ca, ct, zz, z2;
	/*
	float cn, n;
	static float a, b, s, t, u, v, y2;
	static cmplxf sum;
	*/

	x = crealf(z);
	y = cimagf(z);

	if(y == 0.0f) {
		if(fabsf(x) > 1.0f) {
			w = (float)M_PI_2 + 0.0f * I;
			/*mtherr( "casinf", DOMAIN );*/
		}
		else {
			w = asinf (x) + 0.0f * I;
		}
		return (w);
	}

	/* Power series expansion */
	/*
	b = cabsf(z);
	if(b < 0.125) {
		z2.r = (x - y) * (x + y);
		z2.i = 2.0 * x * y;

		cn = 1.0;
		n = 1.0;
		ca.r = x;
		ca.i = y;
		sum.r = x;
		sum.i = y;
		do {
			ct.r = z2.r * ca.r  -  z2.i * ca.i;
			ct.i = z2.r * ca.i  +  z2.i * ca.r;
			ca.r = ct.r;
			ca.i = ct.i;

			cn *= n;
			n += 1.0;
			cn /= n;
			n += 1.0;
			b = cn/n;

			ct.r *= b;
			ct.i *= b;
			sum.r += ct.r;
			sum.i += ct.i;
			b = fabsf(ct.r) + fabsf(ct.i);
		}
		while(b > MACHEPF);
		w->r = sum.r;
		w->i = sum.i;
		return;
	}
	*/


	ca = x + y * I;
	ct = ca * I;	/* iz */
	/* sqrt( 1 - z*z) */
	/* cmul( &ca, &ca, &zz ) */
	/*x * x  -  y * y */
	zz = (x - y) * (x + y) + (2.0f * x * y) * I;
	zz = 1.0f - crealf(zz) - cimagf(zz) * I;
	z2 = csqrtf (zz);

	zz = ct + z2;
	zz = clogf (zz);
	/* multiply by 1/i = -i */
	w = zz * (-1.0f * I);
	return (w);
}
