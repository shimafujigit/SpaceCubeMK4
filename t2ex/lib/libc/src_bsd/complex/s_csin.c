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

/*	$OpenBSD: s_csin.c,v 1.2 2011/07/08 19:25:31 martynas Exp $	*/
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

/*							csin()
 *
 *	Complex circular sine
 *
 *
 *
 * SYNOPSIS:
 *
 * double complex csin();
 * double complex z, w;
 *
 * w = csin (z);
 *
 *
 *
 * DESCRIPTION:
 *
 * If
 *     z = x + iy,
 *
 * then
 *
 *     w = sin x  cosh y  +  i cos x sinh y.
 *
 * csin(z) = -i csinh(iz).
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       -10,+10      8400       5.3e-17     1.3e-17
 *    IEEE      -10,+10     30000       3.8e-16     1.0e-16
 * Also tested by csin(casin(z)) = z.
 *
 */

#include <sys/cdefs.h>
#include <complex.h>
#include <float.h>
#include <math.h>

/* calculate cosh and sinh */

static void
cchsh(double x, double *c, double *s)
{
	double e, ei;

	if (fabs(x) <= 0.5) {
		*c = cosh(x);
		*s = sinh(x);
	}
	else {
		e = exp(x);
		ei = 0.5/e;
		e = 0.5 * e;
		*s = e - ei;
		*c = e + ei;
	}
}

double complex
csin(double complex z)
{
	double complex w;
	double ch, sh;

	cchsh( cimag (z), &ch, &sh );
	w = sin (creal(z)) * ch + (cos (creal(z)) * sh) * I;
	return (w);
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double complex csinl(long double complex);
#else	/* lint */
__weak_alias(csinl, csin);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
