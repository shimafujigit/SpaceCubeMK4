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

/*	$OpenBSD: s_fma.c,v 1.1 2011/07/06 00:02:42 martynas Exp $	*/

/*-
 * Copyright (c) 2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* LINTLIBRARY */

#include <sys/cdefs.h>
#if 0
__FBSDID("$FreeBSD: src/lib/msun/src/s_fma.c,v 1.5 2008/04/03 06:14:51 das Exp $");
#endif

#include <fenv.h>
#include <float.h>
#include <math.h>

/*
 * Fused multiply-add: Compute x * y + z with a single rounding error.
 *
 * We use scaling to avoid overflow/underflow, along with the
 * canonical precision-doubling technique adapted from:
 *
 *	Dekker, T.  A Floating-Point Technique for Extending the
 *	Available Precision.  Numer. Math. 18, 224-242 (1971).
 *
 * This algorithm is sensitive to the rounding precision.  FPUs such
 * as the i387 must be set in double-precision mode if variables are
 * to be stored in FP registers in order to avoid incorrect results.
 * This is the default on FreeBSD, but not on many other systems.
 *
 * Hardware instructions should be used on architectures that support it,
 * since this implementation will likely be several times slower.
 */
#if LDBL_MANT_DIG != 113
double
fma(double x, double y, double z)
{
	static const double split = 0x1p27 + 1.0;
	double xs, ys, zs;
	double c, cc, hx, hy, p, q, tx, ty;
	double r, rr, s;
	int oround;
	int ex, ey, ez;
	int spread;

	/*
	 * Handle special cases. The order of operations and the particular
	 * return values here are crucial in handling special cases involving
	 * infinities, NaNs, overflows, and signed zeroes correctly.
	 */
	if (x == 0.0 || y == 0.0)
		return (x * y + z);
	if (z == 0.0)
		return (x * y);
	if (!isfinite(x) || !isfinite(y))
		return (x * y + z);
	if (!isfinite(z))
		return (z);

	xs = frexp(x, &ex);
	ys = frexp(y, &ey);
	zs = frexp(z, &ez);
	oround = /*fegetround()*/FE_TONEAREST;
	spread = ex + ey - ez;

	/*
	 * If x * y and z are many orders of magnitude apart, the scaling
	 * will overflow, so we handle these cases specially.  Rounding
	 * modes other than FE_TONEAREST are painful.
	 */
	if (spread > DBL_MANT_DIG * 2) {
		fenv_t env;
		/*feraiseexcept(FE_INEXACT)*/;
		switch(oround) {
		case FE_TONEAREST:
			return (x * y);
		case FE_TOWARDZERO:
			if (x > 0.0 ^ y < 0.0 ^ z < 0.0)
				return (x * y);
			/*feholdexcept(&env)*/;
			r = x * y;
			if (!/*fetestexcept(FE_INEXACT)*/0)
				r = nextafter(r, 0);
			/*feupdateenv(&env)*/;
			return (r);
		case FE_DOWNWARD:
			if (z > 0.0)
				return (x * y);
			/*feholdexcept(&env)*/;
			r = x * y;
			if (!/*fetestexcept(FE_INEXACT)*/0)
				r = nextafter(r, -INFINITY);
			/*feupdateenv(&env)*/;
			return (r);
		default:	/* FE_UPWARD */
			if (z < 0.0)
				return (x * y);
			/*feholdexcept(&env)*/;
			r = x * y;
			if (!/*fetestexcept(FE_INEXACT)*/0)
				r = nextafter(r, INFINITY);
			/*feupdateenv(&env)*/;
			return (r);
		}
	}
	if (spread < -DBL_MANT_DIG) {
		/*feraiseexcept(FE_INEXACT)*/;
		if (!isnormal(z))
			/*feraiseexcept(FE_UNDERFLOW)*/;
		switch (oround) {
		case FE_TONEAREST:
			return (z);
		case FE_TOWARDZERO:
			if (x > 0.0 ^ y < 0.0 ^ z < 0.0)
				return (z);
			else
				return (nextafter(z, 0));
		case FE_DOWNWARD:
			if (x > 0.0 ^ y < 0.0)
				return (z);
			else
				return (nextafter(z, -INFINITY));
		default:	/* FE_UPWARD */
			if (x > 0.0 ^ y < 0.0)
				return (nextafter(z, INFINITY));
			else
				return (z);
		}
	}

	/*
	 * Use Dekker's algorithm to perform the multiplication and
	 * subsequent addition in twice the machine precision.
	 * Arrange so that x * y = c + cc, and x * y + z = r + rr.
	 */
	/*fesetround(FE_TONEAREST)*/;

	p = xs * split;
	hx = xs - p;
	hx += p;
	tx = xs - hx;

	p = ys * split;
	hy = ys - p;
	hy += p;
	ty = ys - hy;

	p = hx * hy;
	q = hx * ty + tx * hy;
	c = p + q;
	cc = p - c + q + tx * ty;

	zs = ldexp(zs, -spread);
	r = c + zs;
	s = r - c;
	rr = (c - (r - s)) + (zs - s) + cc;

	spread = ex + ey;
	if (spread + ilogb(r) > -1023) {
		/*fesetround(oround)*/;
		r = r + rr;
	} else {
		/*
		 * The result is subnormal, so we round before scaling to
		 * avoid double rounding.
		 */
		p = ldexp(copysign(0x1p-1022, r), -spread);
		c = r + p;
		s = c - r;
		cc = (r - (c - s)) + (p - s) + rr;
		/*fesetround(oround)*/;
		r = (c + cc) - p;
	}
	return (ldexp(r, spread));
}
#else	/* LDBL_MANT_DIG == 113 */
/*
 * 113 bits of precision is more than twice the precision of a double,
 * so it is enough to represent the intermediate product exactly.
 */
double
fma(double x, double y, double z)
{
	return ((long double)x * y + z);
}
#endif	/* LDBL_MANT_DIG != 113 */

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double fmal(long double, long double, long double);
#else	/* lint */
__weak_alias(fmal, fma);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
