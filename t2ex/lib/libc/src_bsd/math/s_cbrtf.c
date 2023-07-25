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

/* s_cbrtf.c -- float version of s_cbrt.c.
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

/* cbrtf(x)
 * Return cube root of x
 */
static const unsigned 
B1 = 709958130, /* B1 = (84+2/3-0.03306235651)*2**23 */
B2 = 642849266; /* B2 = (76+2/3-0.03306235651)*2**23 */

static const float
C =  5.4285717010e-01, /* 19/35     = 0x3f0af8b0 */
D = -7.0530611277e-01, /* -864/1225 = 0xbf348ef1 */
E =  1.4142856598e+00, /* 99/70     = 0x3fb50750 */
F =  1.6071428061e+00, /* 45/28     = 0x3fcdb6db */
G =  3.5714286566e-01; /* 5/14      = 0x3eb6db6e */

float
cbrtf(float x) 
{
	float r,s,t;
	int32_t hx;
	u_int32_t sign;
	u_int32_t high;

	GET_FLOAT_WORD(hx,x);
	sign=hx&0x80000000; 		/* sign= sign(x) */
	hx  ^=sign;
	if(hx>=0x7f800000) return(x+x); /* cbrt(NaN,INF) is itself */
	if(hx==0) 
	    return(x);		/* cbrt(0) is itself */

	SET_FLOAT_WORD(x,hx);	/* x <- |x| */
    /* rough cbrt to 5 bits */
	if(hx<0x00800000) 		/* subnormal number */
	  {SET_FLOAT_WORD(t,0x4b800000); /* set t= 2**24 */
	   t*=x; GET_FLOAT_WORD(high,t); SET_FLOAT_WORD(t,high/3+B2);
	  }
	else
	  SET_FLOAT_WORD(t,hx/3+B1);


    /* new cbrt to 23 bits */
	r=t*t/x;
	s=C+r*t;
	t*=G+F/(s+E+D/s);	

    /* retore the sign bit */
	GET_FLOAT_WORD(high,t);
	SET_FLOAT_WORD(t,high|sign);
	return(t);
}
