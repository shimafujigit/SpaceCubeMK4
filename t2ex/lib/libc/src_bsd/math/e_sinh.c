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

/* @(#)e_sinh.c 5.1 93/09/24 */
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

/* sinh(x)
 * Method : 
 * mathematically sinh(x) if defined to be (exp(x)-exp(-x))/2
 *	1. Replace x by |x| (sinh(-x) = -sinh(x)). 
 *	2. 
 *		                                    E + E/(E+1)
 *	    0        <= x <= 22     :  sinh(x) := --------------, E=expm1(x)
 *			       			        2
 *
 *	    22       <= x <= lnovft :  sinh(x) := exp(x)/2 
 *	    lnovft   <= x <= ln2ovft:  sinh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  sinh(x) := x*shuge (overflow)
 *
 * Special cases:
 *	sinh(x) is |x| if x is +INF, -INF, or NaN.
 *	only sinh(0)=0 is exact for finite x.
 */

#include <sys/cdefs.h>
#include <float.h>
#include <math.h>

#include "math_private.h"

static const double one = 1.0, shuge = 1.0e307;

double
sinh(double x)
{	
	double t,w,h;
	int32_t ix,jx;
	u_int32_t lx;

    /* High word of |x|. */
	GET_HIGH_WORD(jx,x);
	ix = jx&0x7fffffff;

    /* x is INF or NaN */
	if(ix>=0x7ff00000) return x+x;	

	h = 0.5;
	if (jx<0) h = -h;
    /* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < 0x40360000) {		/* |x|<22 */
	    if (ix<0x3e300000) 		/* |x|<2**-28 */
		if(shuge+x>one) return x;/* sinh(tiny) = tiny with inexact */
	    t = expm1(fabs(x));
	    if(ix<0x3ff00000) return h*(2.0*t-t*t/(t+one));
	    return h*(t+t/(t+one));
	}

    /* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x40862E42)  return h*exp(fabs(x));

    /* |x| in [log(maxdouble), overflowthresold] */
	GET_LOW_WORD(lx,x);
	if (ix<0x408633CE || ((ix==0x408633ce)&&(lx<=(u_int32_t)0x8fb9f87d))) {
	    w = exp(0.5*fabs(x));
	    t = h*w;
	    return t*w;
	}

    /* |x| > overflowthresold, sinh(x) overflow */
	return x*shuge;
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double sinhl(long double);
#else	/* lint */
__weak_alias(sinhl, sinh);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
