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

/* e_asinf.c -- float version of e_asin.c.
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
one =  1.0000000000e+00, /* 0x3F800000 */
huge =  1.000e+30,
pio2_hi =  1.5707962513e+00, /* 0x3fc90fda */
pio2_lo =  7.5497894159e-08, /* 0x33a22168 */
pio4_hi =  7.8539818525e-01, /* 0x3f490fdb */
	/* coefficient for R(x^2) */
pS0 =  1.6666667163e-01, /* 0x3e2aaaab */
pS1 = -3.2556581497e-01, /* 0xbea6b090 */
pS2 =  2.0121252537e-01, /* 0x3e4e0aa8 */
pS3 = -4.0055535734e-02, /* 0xbd241146 */
pS4 =  7.9153501429e-04, /* 0x3a4f7f04 */
pS5 =  3.4793309169e-05, /* 0x3811ef08 */
qS1 = -2.4033949375e+00, /* 0xc019d139 */
qS2 =  2.0209457874e+00, /* 0x4001572d */
qS3 = -6.8828397989e-01, /* 0xbf303361 */
qS4 =  7.7038154006e-02; /* 0x3d9dc62e */

float
asinf(float x)
{
	float t,w,p,q,c,r,s;
	int32_t hx,ix;
	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix==0x3f800000) {
		/* asin(1)=+-pi/2 with inexact */
	    return x*pio2_hi+x*pio2_lo;	
	} else if(ix> 0x3f800000) {	/* |x|>= 1 */
	    return (x-x)/(x-x);		/* asin(|x|>1) is NaN */   
	} else if (ix<0x3f000000) {	/* |x|<0.5 */
	    if(ix<0x32000000) {		/* if |x| < 2**-27 */
		if(huge+x>one) return x;/* return x with inexact if x!=0*/
	    } else 
		t = x*x;
		p = t*(pS0+t*(pS1+t*(pS2+t*(pS3+t*(pS4+t*pS5)))));
		q = one+t*(qS1+t*(qS2+t*(qS3+t*qS4)));
		w = p/q;
		return x+x*w;
	}
	/* 1> |x|>= 0.5 */
	w = one-fabsf(x);
	t = w*(float)0.5;
	p = t*(pS0+t*(pS1+t*(pS2+t*(pS3+t*(pS4+t*pS5)))));
	q = one+t*(qS1+t*(qS2+t*(qS3+t*qS4)));
	s = sqrtf(t);
	if(ix>=0x3F79999A) { 	/* if |x| > 0.975 */
	    w = p/q;
	    t = pio2_hi-((float)2.0*(s+s*w)-pio2_lo);
	} else {
	    int32_t iw;
	    w  = s;
	    GET_FLOAT_WORD(iw,w);
	    SET_FLOAT_WORD(w,iw&0xfffff000);
	    c  = (t-w*w)/(s+w);
	    r  = p/q;
	    p  = (float)2.0*s*r-(pio2_lo-(float)2.0*c);
	    q  = pio4_hi-(float)2.0*w;
	    t  = pio4_hi-(p-q);
	}    
	if(hx>0) return t; else return -t;    
}
