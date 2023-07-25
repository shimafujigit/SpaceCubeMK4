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
 *	@(#)complex.h
 *
 */

#ifndef _COMPLEX_H_
#define	_COMPLEX_H_

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define complex	_Complex
#define	_Complex_I	1.0fi
#define I	_Complex_I

IMPORT	double	cabs(double complex z);
IMPORT	float	cabsf(float complex z);
IMPORT	long double	cabsl(long double complex z);
IMPORT	double complex	cacos(double complex z);
IMPORT	float complex	cacosf(float complex z);
IMPORT	long double complex	cacosl(long double complex z);
IMPORT	double complex	cacosh(double complex z);
IMPORT	float complex	cacoshf(float complex z);
IMPORT	long double complex	cacoshl(long double complex z);
IMPORT	double	carg(double complex);
IMPORT	float	cargf(float complex);
IMPORT	long double	cargl(long double complex);
IMPORT	double complex	casin(double complex z);
IMPORT	float complex	casinf(float complex z);
IMPORT	long double complex	casinl(long double complex z);
IMPORT	double complex	casinh(double complex z);
IMPORT	float complex	casinhf(float complex z);
IMPORT	long double complex	casinhl(long double complex z);
IMPORT	double complex	catan(double complex z);
IMPORT	float complex	catanf(float complex z);
IMPORT	long double complex	catanl(long double complex z);
IMPORT	double complex	catanh(double complex z);
IMPORT	float complex	catanhf(float complex z);
IMPORT	long double complex	catanhl(long double complex z);
IMPORT	double complex	ccos(double complex z);
IMPORT	float complex	ccosf(float complex z);
IMPORT	long double complex	ccosl(long double complex z);
IMPORT	double complex	ccosh(double complex z);
IMPORT	float complex	ccoshf(float complex z);
IMPORT	long double complex	ccoshl(long double complex z);
IMPORT	double complex	cexp(double complex z);
IMPORT	float complex	cexpf(float complex z);
IMPORT	long double complex	cexpl(long double complex z);
IMPORT	double	cimag(double complex z);
IMPORT	float	cimagf(float complex z);
IMPORT	long double	cimagl(long double complex z);
IMPORT	double complex	clog(double complex z);
IMPORT	float complex	clogf(float complex z);
IMPORT	long double complex	clogl(long double complex z);
IMPORT	double complex	conj(double complex z);
IMPORT	float complex	conjf(float complex z);
IMPORT	long double complex	conjl(long double complex z);
IMPORT	double complex	cpow(double complex x, double complex y);
IMPORT	float complex	cpowf(float complex x, float complex y);
IMPORT	long double complex	cpowl(long double complex x, long double complex y);
IMPORT	double complex	cproj(double complex z);
IMPORT	float complex	cprojf(float complex z);
IMPORT	long double complex	cprojl(long double complex z);
IMPORT	double	creal(double complex z);
IMPORT	float	crealf(float complex z);
IMPORT	long double	creall(long double complex z);
IMPORT	double complex	csin(double complex z);
IMPORT	float complex	csinf(float complex z);
IMPORT	long double complex	csinl(long double complex z);
IMPORT	double complex	csinh(double complex z);
IMPORT	float complex	csinhf(float complex z);
IMPORT	long double complex	csinhl(long double complex z);
IMPORT	double complex	csqrt(double complex z);
IMPORT	float complex	csqrtf(float complex z);
IMPORT	long double complex	csqrtl(long double complex z);
IMPORT	double complex	ctan(double complex z);
IMPORT	float complex	ctanf(float complex z);
IMPORT	long double complex	ctanl(long double complex z);
IMPORT	complex	ctanh(double complex z);
IMPORT	float complex	ctanhf(float complex z);
IMPORT	long double complex	ctanhl(long double complex z);

#ifdef __cplusplus
}
#endif
#endif /* _COMPLEX_H_ */

