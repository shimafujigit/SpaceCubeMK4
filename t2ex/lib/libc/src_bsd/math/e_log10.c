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

/* @(#)e_log10.c 5.1 93/09/24 */
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

/* log10(x)
 * Return the base 10 logarithm of x
 * 
 * Method :
 *	Let log10_2hi = leading 40 bits of log10(2) and
 *	    log10_2lo = log10(2) - log10_2hi,
 *	    ivln10   = 1/log(10) rounded.
 *	Then
 *		n = ilogb(x), 
 *		if(n<0)  n = n+1;
 *		x = scalbn(x,-n);
 *		log10(x) := n*log10_2hi + (n*log10_2lo + ivln10*log(x))
 *
 * Note 1:
 *	To guarantee log10(10**n)=n, where 10**n is normal, the rounding 
 *	mode must set to Round-to-Nearest.
 * Note 2:
 *	[1/log(10)] rounded to 53 bits has error  .198   ulps;
 *	log10 is monotonic at all binary break points.
 *
 * Special cases:
 *	log10(x) is NaN with signal if x < 0; 
 *	log10(+INF) is +INF with no signal; log10(0) is -INF with signal;
 *	log10(NaN) is that NaN with no signal;
 *	log10(10**N) = N  for N=0,1,...,22.
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following constants.
 * The decimal values may be used, provided that the compiler will convert
 * from decimal to binary accurately enough to produce the hexadecimal values
 * shown.
 */

#include <sys/cdefs.h>
#include <float.h>
#include <math.h>

#include "math_private.h"

static const double
two54      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10     =  4.34294481903251816668e-01, /* 0x3FDBCB7B, 0x1526E50E */
log10_2hi  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

static const double zero   =  0.0;

double
log10(double x)
{
	double y,z;
	int32_t i,k,hx;
	u_int32_t lx;

	EXTRACT_WORDS(hx,lx,x);

        k=0;
        if (hx < 0x00100000) {                  /* x < 2**-1022  */
            if (((hx&0x7fffffff)|lx)==0)
                return -two54/zero;             /* log(+-0)=-inf */
            if (hx<0) return (x-x)/zero;        /* log(-#) = NaN */
            k -= 54; x *= two54; /* subnormal number, scale up x */
	    GET_HIGH_WORD(hx,x);
        }
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	i  = ((u_int32_t)k&0x80000000)>>31;
        hx = (hx&0x000fffff)|((0x3ff-i)<<20);
        y  = (double)(k+i);
	SET_HIGH_WORD(x,hx);
	z  = y*log10_2lo + ivln10*log(x);
	return  z+y*log10_2hi;
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double log10l(long double);
#else	/* lint */
__weak_alias(log10l, log10);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
