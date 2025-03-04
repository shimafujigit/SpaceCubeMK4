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

/*	$OpenBSD: s_csqrt.c,v 1.2 2011/07/08 19:25:31 martynas Exp $	*/
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

/* LINTLIBRARY */

/*							csqrt()
 *
 *	Complex square root
 *
 *
 *
 * SYNOPSIS:
 *
 * double complex csqrt();
 * double complex z, w;
 *
 * w = csqrt (z);
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
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       -10,+10     25000       3.2e-17     9.6e-18
 *    IEEE      -10,+10   1,000,000     2.9e-16     6.1e-17
 *
 */

#include <sys/cdefs.h>
#include <complex.h>
#include <float.h>
#include <math.h>

double complex
csqrt(double complex z)
{
	double complex w;
	double x, y, r, t, scale;

	x = creal (z);
	y = cimag (z);

	if (y == 0.0) {
		if (x == 0.0) {
			w = 0.0 + y * I;
		}
		else {
			r = fabs (x);
			r = sqrt (r);
			if (x < 0.0) {
				w = 0.0 + r * I;
			}
			else {
				w = r + y * I;
			}
		}
		return (w);
	}
	if (x == 0.0) {
		r = fabs (y);
		r = sqrt (0.5*r);
		if (y > 0)
			w = r + r * I;
		else
			w = r - r * I;
		return (w);
	}
	/* Rescale to avoid internal overflow or underflow.  */
	if ((fabs(x) > 4.0) || (fabs(y) > 4.0)) {
		x *= 0.25;
		y *= 0.25;
		scale = 2.0;
	}
	else {
		x *= 1.8014398509481984e16;  /* 2^54 */
		y *= 1.8014398509481984e16;
		scale = 7.450580596923828125e-9; /* 2^-27 */
#if 0
		x *= 4.0;
		y *= 4.0;
		scale = 0.5;
#endif
	}
	w = x + y * I;
	r = cabs(w);
	if (x > 0) {
		t = sqrt(0.5 * r + 0.5 * x);
		r = scale * fabs((0.5 * y) / t);
		t *= scale;
	}
	else {
		r = sqrt( 0.5 * r - 0.5 * x );
		t = scale * fabs( (0.5 * y) / r );
		r *= scale;
	}
	if (y < 0)
		w = t - r * I;
	else
		w = t + r * I;
	return (w);
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double complex csqrtl(long double complex);
#else	/* lint */
__weak_alias(csqrtl, csqrt);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
