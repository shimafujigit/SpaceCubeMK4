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

/*	$OpenBSD: s_csqrtf.c,v 1.2 2010/07/18 18:42:26 guenther Exp $	*/
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

/*							csqrtf()
 *
 *	Complex square root
 *
 *
 *
 * SYNOPSIS:
 *
 * float complex csqrtf();
 * float complex z, w;
 *
 * w = csqrtf( z );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 * If z = x + iy,  r = |z|, then
 *
 *                       1/2
 * Re w  =  [ (r + x)/2 ]   ,
 *
 *                       1/2
 * Im w  =  [ (r - x)/2 ]   .
 *
 * Cancellation error in r-x or r+x is avoided by using the
 * identity  2 Re w Im w  =  y.
 *
 * Note that -w is also a square root of z.  The root chosen
 * is always in the right half plane and Im w has the same sign as y.
 *
 *
 *
 * ACCURACY:
 *
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -10,+10    1,000,000    1.8e-7       3.5e-8
 *
 */

#include <complex.h>
#include <math.h>

float complex
csqrtf(float complex z)
{
	float complex w;
	float x, y, r, t, scale;

	x = crealf(z);
	y = cimagf(z);

	if(y == 0.0f) {
		if (x < 0.0f) {
			w = 0.0f + sqrtf(-x) * I;
			return (w);
		}
		else if (x == 0.0f) {
			return (0.0f + y * I);
		}
		else {
			w = sqrtf(x) + y * I;
			return (w);
		}
	}

	if (x == 0.0f) {
		r = fabsf(y);
		r = sqrtf(0.5f*r);
		if(y > 0)
			w = r + r * I;
		else
			w = r - r * I;
		return (w);
	}

	/* Rescale to avoid internal overflow or underflow.  */
	if ((fabsf(x) > 4.0f) || (fabsf(y) > 4.0f)) {
		x *= 0.25f;
		y *= 0.25f;
		scale = 2.0f;
	}
	else {
		x *= 6.7108864e7f; /* 2^26 */
		y *= 6.7108864e7f;
		scale = 1.220703125e-4f; /* 2^-13 */
#if 0
		x *= 4.0f;
		y *= 4.0f;
		scale = 0.5f;
#endif
	}
	w = x + y * I;
	r = cabsf(w);
	if (x > 0) {
		t = sqrtf( 0.5f * r + 0.5f * x );
		r = scale * fabsf((0.5f * y) / t);
		t *= scale;
	}
	else {
		r = sqrtf(0.5f * r - 0.5f * x);
		t = scale * fabsf((0.5f * y) / r);
		r *= scale;
	}

	if (y < 0)
		w = t - r * I;
	else
		w = t + r * I;
	return (w);
}
