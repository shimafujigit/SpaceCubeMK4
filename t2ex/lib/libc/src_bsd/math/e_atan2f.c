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

/* e_atan2f.c -- float version of e_atan2.c.
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
tiny  = 1.0e-30,
zero  = 0.0,
pi_o_4  = 7.8539818525e-01, /* 0x3f490fdb */
pi_o_2  = 1.5707963705e+00, /* 0x3fc90fdb */
pi      = 3.1415925026e+00, /* 0x40490fda */
pi_lo   = 1.5099578832e-07; /* 0x34222168 */

float
atan2f(float y, float x)
{  
	float z;
	int32_t k,m,hx,hy,ix,iy;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	GET_FLOAT_WORD(hy,y);
	iy = hy&0x7fffffff;
	if((ix>0x7f800000)||
	   (iy>0x7f800000))	/* x or y is NaN */
	   return x+y;
	if(hx==0x3f800000) return atanf(y);   /* x=1.0 */
	m = ((hy>>31)&1)|((hx>>30)&2);	/* 2*sign(x)+sign(y) */

    /* when y = 0 */
	if(iy==0) {
	    switch(m) {
		case 0: 
		case 1: return y; 	/* atan(+-0,+anything)=+-0 */
		case 2: return  pi+tiny;/* atan(+0,-anything) = pi */
		case 3: return -pi-tiny;/* atan(-0,-anything) =-pi */
	    }
	}
    /* when x = 0 */
	if(ix==0) return (hy<0)?  -pi_o_2-tiny: pi_o_2+tiny;
	    
    /* when x is INF */
	if(ix==0x7f800000) {
	    if(iy==0x7f800000) {
		switch(m) {
		    case 0: return  pi_o_4+tiny;/* atan(+INF,+INF) */
		    case 1: return -pi_o_4-tiny;/* atan(-INF,+INF) */
		    case 2: return  (float)3.0*pi_o_4+tiny;/*atan(+INF,-INF)*/
		    case 3: return (float)-3.0*pi_o_4-tiny;/*atan(-INF,-INF)*/
		}
	    } else {
		switch(m) {
		    case 0: return  zero  ;	/* atan(+...,+INF) */
		    case 1: return -zero  ;	/* atan(-...,+INF) */
		    case 2: return  pi+tiny  ;	/* atan(+...,-INF) */
		    case 3: return -pi-tiny  ;	/* atan(-...,-INF) */
		}
	    }
	}
    /* when y is INF */
	if(iy==0x7f800000) return (hy<0)? -pi_o_2-tiny: pi_o_2+tiny;

    /* compute y/x */
	k = (iy-ix)>>23;
	if(k > 60) z=pi_o_2+(float)0.5*pi_lo; 	/* |y/x| >  2**60 */
	else if(hx<0&&k<-60) z=0.0; 	/* |y|/x < -2**60 */
	else z=atanf(fabsf(y/x));	/* safe to do y/x */
	switch (m) {
	    case 0: return       z  ;	/* atan(+,+) */
	    case 1: {
	    	      u_int32_t zh;
		      GET_FLOAT_WORD(zh,z);
		      SET_FLOAT_WORD(z,zh ^ 0x80000000);
		    }
		    return       z  ;	/* atan(-,+) */
	    case 2: return  pi-(z-pi_lo);/* atan(+,-) */
	    default: /* case 3 */
	    	    return  (z-pi_lo)-pi;/* atan(-,-) */
	}
}
