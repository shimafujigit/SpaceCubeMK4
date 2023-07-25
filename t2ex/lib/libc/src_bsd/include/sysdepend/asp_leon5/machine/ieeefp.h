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

/*	$OpenBSD: ieeefp.h,v 1.2 2011/03/23 16:54:34 pirofti Exp $	*/
/*	$NetBSD: ieeefp.h,v 1.1 2001/01/10 19:02:06 bjh21 Exp $	*/

/* 
 * Based on ieeefp.h written by J.T. Conklin, Apr 28, 1995
 * Public domain.
 */

#ifndef _SPARC_IEEEFP_H_
#define _SPARC_IEEEFP_H_

/* FP exception codes */

#define FP_EXCEPT_INV	0
#define FP_EXCEPT_DZ	1
#define FP_EXCEPT_OFL	2
#define FP_EXCEPT_UFL	3
#define FP_EXCEPT_IMP	4

/* Exception type (used by fpsetmask() et al.) */

typedef int fp_except;

/* Bit defines for fp_except */

#define	FP_X_INV	(1 << FP_EXCEPT_INV)	/* invalid operation exception */
#define	FP_X_DZ		(1 << FP_EXCEPT_DZ)	/* divide-by-zero exception */
#define	FP_X_OFL	(1 << FP_EXCEPT_OFL)	/* overflow exception */
#define	FP_X_UFL	(1 << FP_EXCEPT_UFL)	/* underflow exception */
#define	FP_X_IMP	(1 << FP_EXCEPT_IMP)	/* imprecise (loss of precision; "inexact") */

/* Rounding modes */

typedef enum {
    FP_RN=0,			/* round to nearest representable number */
    FP_RP=1,			/* round toward positive infinity */
    FP_RM=2,			/* round toward negative infinity */
    FP_RZ=3			/* round to zero (truncate) */
} fp_rnd;

#endif /* _SPARC_IEEEFP_H_ */
