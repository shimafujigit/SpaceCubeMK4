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

/*	$OpenBSD: s_cpow.c,v 1.2 2011/07/08 19:25:31 martynas Exp $	*/
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

/*							cpow
 *
 *	Complex power function
 *
 *
 *
 * SYNOPSIS:
 *
 * double complex cpow();
 * double complex a, z, w;
 *
 * w = cpow (a, z);
 *
 *
 *
 * DESCRIPTION:
 *
 * Raises complex A to the complex Zth power.
 * Definition is per AMS55 # 4.2.8,
 * analytically equivalent to cpow(a,z) = cexp(z clog(a)).
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -10,+10     30000       9.4e-15     1.5e-15
 *
 */

#include <sys/cdefs.h>
#include <complex.h>
#include <float.h>
#include <math.h>

double complex
cpow(double complex a, double complex z)
{
	double complex w;
	double x, y, r, theta, absa, arga;

	x = creal (z);
	y = cimag (z);
	absa = cabs (a);
	if (absa == 0.0) {
		return (0.0 + 0.0 * I);
	}
	arga = carg (a);
	r = pow (absa, x);
	theta = x * arga;
	if (y != 0.0) {
		r = r * exp (-y * arga);
		theta = theta + y * log (absa);
	}
	w = r * cos (theta) + (r * sin (theta)) * I;
	return (w);
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double complex cpowl(long double complex, long double complex);
#else	/* lint */
__weak_alias(cpowl, cpow);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
