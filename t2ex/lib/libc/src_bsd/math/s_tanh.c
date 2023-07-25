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

/* @(#)s_tanh.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* LINTLIBRARY */

/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *				       x    -x
 *				      e  - e
 *	0. tanh(x) is defined to be -----------
 *				       x    -x
 *				      e  + e
 *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *	2.  0      <= x <= 2**-55 : tanh(x) := x*(one+x)
 *					        -t
 *	    2**-55 <  x <=  1     : tanh(x) := -----; t = expm1(-2x)
 *					       t + 2
 *						     2
 *	    1      <= x <=  22.0  : tanh(x) := 1-  ----- ; t=expm1(2x)
 *						   t + 2
 *	    22.0   <  x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *	tanh(NaN) is NaN;
 *	only tanh(0)=0 is exact for finite argument.
 */

#include <sys/cdefs.h>
#include <float.h>
#include <math.h>

#include "math_private.h"

static const double one=1.0, two=2.0, tiny = 1.0e-300;

double
tanh(double x)
{
	double t,z;
	int32_t jx,ix,lx;

    /* High word of |x|. */
	EXTRACT_WORDS(jx,lx,x);
	ix = jx&0x7fffffff;

    /* x is INF or NaN */
	if(ix>=0x7ff00000) { 
	    if (jx>=0) return one/x+one;    /* tanh(+-inf)=+-1 */
	    else       return one/x-one;    /* tanh(NaN) = NaN */
	}

    /* |x| < 22 */
	if (ix < 0x40360000) {		/* |x|<22 */
	    if ((ix | lx) == 0)
		return x;		/* x == +-0 */
	    if (ix<0x3c800000) 		/* |x|<2**-55 */
		return x*(one+x);    	/* tanh(small) = small */
	    if (ix>=0x3ff00000) {	/* |x|>=1  */
		t = expm1(two*fabs(x));
		z = one - two/(t+two);
	    } else {
	        t = expm1(-two*fabs(x));
	        z= -t/(t+two);
	    }
    /* |x| > 22, return +-1 */
	} else {
	    z = one - tiny;		/* raised inexact flag */
	}
	return (jx>=0)? z: -z;
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double tanhl(long double);
#else	/* lint */
__weak_alias(tanhl, tanh);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
