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

/*
 *	@(#)float.h
 *
 */

#ifndef _FLOAT_H_
#define	_FLOAT_H_

#include <basic.h>

#define	FLT_ROUNDS	(__flt_rounds())
#define	FLT_RADIX	__FLT_RADIX__
#define	FLT_EVAL_METHOD	__FLT_EVAL_METHOD__
#define	DECIMAL_DIG	__DECIMAL_DIG__

#define	FLT_DIG	__FLT_DIG__
#define	FLT_EPSILON	__FLT_EPSILON__
#define	FLT_MANT_DIG	__FLT_MANT_DIG__
#define	FLT_MAX	__FLT_MAX__
#define	FLT_MAX_10_EXP	__FLT_MAX_10_EXP__
#define	FLT_MAX_EXP	__FLT_MAX_EXP__
#define	FLT_MIN	__FLT_MIN__
#define	FLT_MIN_10_EXP	__FLT_MIN_10_EXP__
#define	FLT_MIN_EXP	__FLT_MIN_EXP__

#define	DBL_DIG	__DBL_DIG__
#define	DBL_EPSILON	__DBL_EPSILON__
#define	DBL_MANT_DIG	__DBL_MANT_DIG__
#define	DBL_MAX	__DBL_MAX__
#define	DBL_MAX_10_EXP	__DBL_MAX_10_EXP__
#define	DBL_MAX_EXP	__DBL_MAX_EXP__
#define	DBL_MIN	__DBL_MIN__
#define	DBL_MIN_10_EXP	__DBL_MIN_10_EXP__
#define	DBL_MIN_EXP	__DBL_MIN_EXP__

#define	LDBL_DIG	__LDBL_DIG__
#define	LDBL_EPSILON	__LDBL_EPSILON__
#define	LDBL_MANT_DIG	__LDBL_MANT_DIG__
#define	LDBL_MAX	__LDBL_MAX__
#define	LDBL_MAX_10_EXP	__LDBL_MAX_10_EXP__
#define	LDBL_MAX_EXP	__LDBL_MAX_EXP__
#define	LDBL_MIN	__LDBL_MIN__
#define	LDBL_MIN_10_EXP	__LDBL_MIN_10_EXP__
#define	LDBL_MIN_EXP	__LDBL_MIN_EXP__

#ifdef __cplusplus
extern "C" {
#endif

IMPORT	int	__flt_rounds(void);

#ifdef __cplusplus
}
#endif

#endif /* _FLOAT_H_ */

