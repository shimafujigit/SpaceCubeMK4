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

/* s_scalbnf.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "math.h"
#include "math_private.h"

static const float
two25   =  3.355443200e+07,	/* 0x4c000000 */
twom25  =  2.9802322388e-08,	/* 0x33000000 */
huge   = 1.0e+30,
tiny   = 1.0e-30;

float
scalbnf(float x, int n)
{
	int32_t k,ix;
	GET_FLOAT_WORD(ix,x);
        k = (ix&0x7f800000)>>23;		/* extract exponent */
        if (k==0) {				/* 0 or subnormal x */
            if ((ix&0x7fffffff)==0) return x; /* +-0 */
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000)>>23) - 25; 
            if (n< -50000) return tiny*x; 	/*underflow*/
	    }
        if (k==0xff) return x+x;		/* NaN or Inf */
        k = k+n; 
        if (k >  0xfe) return huge*copysignf(huge,x); /* overflow  */
        if (k > 0) 				/* normal result */
	    {SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23)); return x;}
        if (k <= -25)
            if (n > 50000) 	/* in case integer overflow in n+k */
		return huge*copysignf(huge,x);	/*overflow*/
	    else return tiny*copysignf(tiny,x);	/*underflow*/
        k += 25;				/* subnormal result */
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
        return x*twom25;
}

float
ldexpf(float x, int n)
{
	return scalbnf(x, n);
}
