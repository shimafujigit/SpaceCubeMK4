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

/*	$NetBSD: math.h,v 1.47.8.2 2010/03/12 21:58:42 riz Exp $	*/

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

/*
 * @(#)fdlibm.h 5.1 93/09/24
 */

#ifndef _MATH_H_
#define _MATH_H_

#include <sys/cdefs.h>
#include <sys/featuretest.h>

union __float_u {
	unsigned char __dummy[sizeof(float)];
	float __val;
};

union __double_u {
	unsigned char __dummy[sizeof(double)];
	double __val;
};

union __long_double_u {
	unsigned char __dummy[sizeof(long double)];
	long double __val;
};

#include <machine/math.h>		/* may use __float_u, __double_u,
					   or __long_double_u */

#ifdef __HAVE_LONG_DOUBLE
#define	__fpmacro_unary_floating(__name, __arg0)			\
	/* LINTED */							\
	((sizeof (__arg0) == sizeof (float))				\
	?	__ ## __name ## f (__arg0)				\
	: (sizeof (__arg0) == sizeof (double))				\
	?	__ ## __name ## d (__arg0)				\
	:	__ ## __name ## l (__arg0))
#else
#define	__fpmacro_unary_floating(__name, __arg0)			\
	/* LINTED */							\
	((sizeof (__arg0) == sizeof (float))				\
	?	__ ## __name ## f (__arg0)				\
	:	__ ## __name ## d (__arg0))
#endif /* __HAVE_LONG_DOUBLE */

/*
 * ANSI/POSIX
 */
/* 7.12#3 HUGE_VAL, HUGELF, HUGE_VALL */
#if __GNUC_PREREQ__(3, 3)
#define HUGE_VAL	__builtin_huge_val()
#else
extern const union __double_u __infinity;
#define HUGE_VAL	__infinity.__val
#endif

/*
 * ISO C99
 */
#if !defined(_ANSI_SOURCE) && !defined(_POSIX_C_SOURCE) && \
    !defined(_XOPEN_SOURCE) || \
    ((__STDC_VERSION__ - 0) >= 199901L) || \
    ((_POSIX_C_SOURCE - 0) >= 200112L) || \
    ((_XOPEN_SOURCE  - 0) >= 600) || \
    defined(_ISOC99_SOURCE) || defined(_NETBSD_SOURCE)
/* 7.12#3 HUGE_VAL, HUGELF, HUGE_VALL */
#if __GNUC_PREREQ__(3, 3)
#define	HUGE_VALF	__builtin_huge_valf()
#define	HUGE_VALL	__builtin_huge_vall()
#else
extern const union __float_u __infinityf;
#define	HUGE_VALF	__infinityf.__val

extern const union __long_double_u __infinityl;
#define	HUGE_VALL	__infinityl.__val
#endif

/* 7.12#4 INFINITY */
#if __GNUC_PREREQ__(3, 3)
#define	INFINITY	__builtin_inff()
#elif defined(__INFINITY)
#define	INFINITY	__INFINITY	/* float constant which overflows */
#else
#define	INFINITY	HUGE_VALF	/* positive infinity */
#endif /* __INFINITY */

/* 7.12#5 NAN: a quiet NaN, if supported */
#ifdef __HAVE_NANF
extern const union __float_u __nanf;
#define	NAN		__nanf.__val
#endif /* __HAVE_NANF */

/* 7.12#6 number classification macros */
#define	FP_INFINITE	0x00
#define	FP_NAN		0x01
#define	FP_NORMAL	0x02
#define	FP_SUBNORMAL	0x03
#define	FP_ZERO		0x04
/* NetBSD extensions */
#define	_FP_LOMD	0x80		/* range for machine-specific classes */
#define	_FP_HIMD	0xff

#endif /* !_ANSI_SOURCE && ... */

/*
 * XOPEN/SVID
 */
#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#define	MAXFLOAT	((float)3.40282346638528860e+38)
extern int signgam;
#endif /* _XOPEN_SOURCE || _NETBSD_SOURCE */

#if defined(_NETBSD_SOURCE)
enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version

/* if global variable _LIB_VERSION is not desirable, one may
 * change the following to be a constant by:
 *	#define _LIB_VERSION_TYPE const enum version
 * In that case, after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time, it cannot be modified
 * in the middle of a program
 */
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#define _IEEE_  fdlibm_ieee
#define _SVID_  fdlibm_svid
#define _XOPEN_ fdlibm_xopen
#define _POSIX_ fdlibm_posix

#ifndef __cplusplus
struct exception {
	int type;
	const char *name;
	double arg1;
	double arg2;
	double retval;
};
#endif

#define	HUGE		MAXFLOAT

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS		1.41484755040568800000e+16

#define	DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6

#endif /* _NETBSD_SOURCE */

__BEGIN_DECLS
/*
 * ANSI/POSIX
 */
double	acos(double);
double	asin(double);
double	atan(double);
double	atan2(double, double);
double	cos(double);
double	sin(double);
double	tan(double);

double	cosh(double);
double	sinh(double);
double	tanh(double);

double	exp(double);
double	frexp(double, int *);
double	ldexp(double, int);
double	log(double);
double	log2(double);
double	log10(double);
double	modf(double, double *);

double	pow(double, double);
double	sqrt(double);

double	ceil(double);
double	fabs(double);
double	floor(double);
double	fmod(double, double);

#if defined(_XOPEN_SOURCE) || defined(_NETBSD_SOURCE)
double	erf(double);
double	erfc(double);
double	gamma(double);
double	hypot(double, double);
int	finite(double);
double	j0(double);
double	j1(double);
double	jn(int, double);
double	lgamma(double);
double	y0(double);
double	y1(double);
double	yn(int, double);

#if (_XOPEN_SOURCE - 0) >= 500 || defined(_NETBSD_SOURCE)
double	acosh(double);
double	asinh(double);
double	atanh(double);
double	cbrt(double);
double	expm1(double);
int	ilogb(double);
double	log1p(double);
double	logb(double);
double	nextafter(double, double);
double	remainder(double, double);
double	rint(double);
double	scalb(double, double);
#endif /* (_XOPEN_SOURCE - 0) >= 500 || defined(_NETBSD_SOURCE)*/
#endif /* _XOPEN_SOURCE || _NETBSD_SOURCE */

/*
 * ISO C99
 */
#if !defined(_ANSI_SOURCE) && !defined(_POSIX_C_SOURCE) && \
    !defined(_XOPEN_SOURCE) || \
    ((__STDC_VERSION__ - 0) >= 199901L) || \
    ((_POSIX_C_SOURCE - 0) >= 200112L) || \
    ((_XOPEN_SOURCE  - 0) >= 600) || \
    defined(_ISOC99_SOURCE) || defined(_NETBSD_SOURCE)
/* 7.12.3.1 int fpclassify(real-floating x) */
#define	fpclassify(__x)	__fpmacro_unary_floating(fpclassify, __x)

/* 7.12.3.2 int isfinite(real-floating x) */
#define	isfinite(__x)	__fpmacro_unary_floating(isfinite, __x)

/* 7.12.3.5 int isnormal(real-floating x) */
#define	isnormal(__x)	(fpclassify(__x) == FP_NORMAL)

/* 7.12.3.6 int signbit(real-floating x) */
#define	signbit(__x)	__fpmacro_unary_floating(signbit, __x)

/* 7.12.4 trigonometric */

float	acosf(float);
float	asinf(float);
float	atanf(float);
float	atan2f(float, float);
float	cosf(float);
float	sinf(float);
float	tanf(float);

/* 7.12.5 hyperbolic */

float	acoshf(float);
float	asinhf(float);
float	atanhf(float);
float	coshf(float);
float	sinhf(float);
float	tanhf(float);

/* 7.12.6 exp / log */

float	expf(float);
float	expm1f(float);
float	frexpf(float, int *);
int	ilogbf(float);
float	ldexpf(float, int);
float	logf(float);
float	log2f(float);
float	log10f(float);
float	log1pf(float);
float	logbf(float);
float	modff(float, float *);
float	scalbnf(float, int);

/* 7.12.7 power / absolute */

float	cbrtf(float);
float	fabsf(float);
float	hypotf(float, float);
float	powf(float, float);
float	sqrtf(float);

/* 7.12.8 error / gamma */

float	erff(float);
float	erfcf(float);
float	lgammaf(float);

/* 7.12.9 nearest integer */

float	ceilf(float);
float	floorf(float);
float	rintf(float);
double	round(double);
float	roundf(float);
double	trunc(double);
float	truncf(float);
long int	lrint(double);
long int	lrintf(float);
/* LONGLONG */
long long int	llrint(double);
/* LONGLONG */
long long int	llrintf(float);
long int	lround(double);
long int	lroundf(float);
/* LONGLONG */
long long int	llround(double);
/* LONGLONG */
long long int	llroundf(float);

/* 7.12.10 remainder */

float	fmodf(float, float);
float	remainderf(float, float);

/* 7.12.11 manipulation */

float	copysignf(float, float);
double	nan(const char *);
float	nanf(const char *);
long double	nanl(const char *);
float	nextafterf(float, float);

/* 7.12.14 comparision */

#define isunordered(x, y)	(isnan(x) || isnan(y))
#define isgreater(x, y)		(!isunordered((x), (y)) && (x) > (y))
#define isgreaterequal(x, y)	(!isunordered((x), (y)) && (x) >= (y))
#define isless(x, y)		(!isunordered((x), (y)) && (x) < (y))
#define islessequal(x, y)	(!isunordered((x), (y)) && (x) <= (y))
#define islessgreater(x, y)	(!isunordered((x), (y)) && \
				 ((x) > (y) || (y) > (x)))
double	fdim(double, double);
double	fmax(double, double);
double	fmin(double, double);
float	fdimf(float, float);
float	fmaxf(float, float);
float	fminf(float, float);
long double fdiml(long double, long double);
long double fmaxl(long double, long double);
long double fminl(long double, long double);

#endif /* !_ANSI_SOURCE && ... */

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_C_SOURCE) || \
    !defined(_XOPEN_SOURCE) || \
    ((__STDC_VERSION__ - 0) >= 199901L) || \
    ((_POSIX_C_SOURCE - 0) >= 200112L) || \
    defined(_ISOC99_SOURCE) || defined(_NETBSD_SOURCE)
/* 7.12.3.3 int isinf(real-floating x) */
#ifdef __isinf
#define	isinf(__x)	__isinf(__x)
#else
#define	isinf(__x)	__fpmacro_unary_floating(isinf, __x)
#endif

/* 7.12.3.4 int isnan(real-floating x) */
#ifdef __isnan
#define	isnan(__x)	__isnan(__x)
#else
#define	isnan(__x)	__fpmacro_unary_floating(isnan, __x)
#endif
#endif /* !_ANSI_SOURCE && ... */

#if defined(_NETBSD_SOURCE)
#ifndef __cplusplus
int	matherr(struct exception *);
#endif

/*
 * IEEE Test Vector
 */
double	significand(double);

/*
 * Functions callable from C, intended to support IEEE arithmetic.
 */
double	copysign(double, double);
double	scalbn(double, int);

/*
 * BSD math library entry points
 */
double	drem(double, double);

#endif /* _NETBSD_SOURCE */

#if defined(_NETBSD_SOURCE) || defined(_REENTRANT)
/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
double	gamma_r(double, int *);
double	lgamma_r(double, int *);
#endif /* _NETBSD_SOURCE || _REENTRANT */


#if defined(_NETBSD_SOURCE)

/* float versions of ANSI/POSIX functions */

float	gammaf(float);
int	isinff(float);
int	isnanf(float);
int	finitef(float);
float	j0f(float);
float	j1f(float);
float	jnf(int, float);
float	y0f(float);
float	y1f(float);
float	ynf(int, float);

float	scalbf(float, float);

/*
 * float version of IEEE Test Vector
 */
float	significandf(float);

/*
 * float versions of BSD math library entry points
 */
float	dremf(float, float);
#endif /* _NETBSD_SOURCE */

#if defined(_NETBSD_SOURCE) || defined(_REENTRANT)
/*
 * Float versions of reentrant version of gamma & lgamma; passes
 * signgam back by reference as the second argument; user must
 * allocate space for signgam.
 */
float	gammaf_r(float, int *);
float	lgammaf_r(float, int *);
#endif /* !... || _REENTRANT */

/*
 * Library implementation
 */
int	__fpclassifyf(float);
int	__fpclassifyd(double);
int	__isfinitef(float);
int	__isfinited(double);
int	__isinff(float);
int	__isinfd(double);
int	__isnanf(float);
int	__isnand(double);
int	__signbitf(float);
int	__signbitd(double);

#ifdef __HAVE_LONG_DOUBLE
int	__fpclassifyl(long double);
int	__isfinitel(long double);
int	__isinfl(long double);
int	__isnanl(long double);
int	__signbitl(long double);
#endif
__END_DECLS

#endif /* _MATH_H_ */
