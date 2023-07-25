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

/* @(#)s_floor.c 5.1 93/09/24 */
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

#if 0
#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/lib/msun/src/s_trunc.c,v 1.1 2004/06/20 09:25:43 das Exp $");
#endif

/*
 * trunc(x)
 * Return x rounded toward 0 to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to trunc(x).
 */

#include <sys/cdefs.h>
#include <float.h>
#include <math.h>

#include "math_private.h"

static const double huge = 1.0e300;

double
trunc(double x)
{
	int32_t i0,i1,jj0;
	u_int32_t i;
	EXTRACT_WORDS(i0,i1,x);
	jj0 = ((i0>>20)&0x7ff)-0x3ff;
	if(jj0<20) {
	    if(jj0<0) { 	/* raise inexact if x != 0 */
		if(huge+x>0.0) {/* |x|<1, so return 0*sign(x) */
		    i0 &= 0x80000000U;
		    i1 = 0;
		}
	    } else {
		i = (0x000fffff)>>jj0;
		if(((i0&i)|i1)==0) return x; /* x is integral */
		if(huge+x>0.0) {	/* raise inexact flag */
		    i0 &= (~i); i1=0;
		}
	    }
	} else if (jj0>51) {
	    if(jj0==0x400) return x+x;	/* inf or NaN */
	    else return x;		/* x is integral */
	} else {
	    i = ((u_int32_t)(0xffffffff))>>(jj0-20);
	    if((i1&i)==0) return x;	/* x is integral */
	    if(huge+x>0.0)		/* raise inexact flag */
		i1 &= (~i);
	}
	INSERT_WORDS(x,i0,i1);
	return x;
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long double truncl(long double);
#else	/* lint */
__weak_alias(truncl, trunc);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
