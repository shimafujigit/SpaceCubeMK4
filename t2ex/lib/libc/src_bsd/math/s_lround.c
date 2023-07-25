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

/*	$OpenBSD: s_lround.c,v 1.4 2011/07/06 00:02:42 martynas Exp $	*/
/* $NetBSD: lround.c,v 1.2 2004/10/13 15:18:32 drochner Exp $ */

/*-
 * Copyright (c) 2004
 *	Matthias Drochner. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* LINTLIBRARY */

#include <sys/types.h>
#include <sys/limits.h>
#include <float.h>
#include <math.h>
#include <ieeefp.h>
#include <machine/ieee.h>
#include "math_private.h"

#ifndef LROUNDNAME
#define LROUNDNAME lround
#define RESTYPE long int
#define RESTYPE_MIN LONG_MIN
#define RESTYPE_MAX LONG_MAX
#endif

#define RESTYPE_BITS (sizeof(RESTYPE) * 8)

RESTYPE
LROUNDNAME(double x)
{
	u_int32_t i0, i1;
	int e, s, shift;
	RESTYPE res;

	GET_HIGH_WORD(i0, x);
	e = i0 >> DBL_FRACHBITS;
	s = e >> DBL_EXPBITS;
	e = (e & 0x7ff) - DBL_EXP_BIAS;

	/* 1.0 x 2^-1 is the smallest number which can be rounded to 1 */
	if (e < -1)
		return (0);
	/* 1.0 x 2^31 (or 2^63) is already too large */
	if (e >= (int)RESTYPE_BITS - 1)
		return (s ? RESTYPE_MIN : RESTYPE_MAX); /* ??? unspecified */

	/* >= 2^52 is already an exact integer */
	if (e < DBL_FRACBITS) {
		/* add 0.5, extraction below will truncate */
		x += (s ? -0.5 : 0.5);
	}

	EXTRACT_WORDS(i0, i1, x);
	e = ((i0 >> DBL_FRACHBITS) & 0x7ff) - DBL_EXP_BIAS;
	i0 &= 0xfffff;
	i0 |= (1 << DBL_FRACHBITS);

	shift = e - DBL_FRACBITS;
	if (shift >=0)
		res = (shift < RESTYPE_BITS ? (RESTYPE)i1 << shift : 0);
	else
		res = (shift > -RESTYPE_BITS ? (RESTYPE)i1 >> -shift : 0);
	shift += 32;
	if (shift >=0)
		res |= (shift < RESTYPE_BITS ? (RESTYPE)i0 << shift : 0);
	else
		res |= (shift > -RESTYPE_BITS ? (RESTYPE)i0 >> -shift : 0);

	return (s ? -res : res);
}

#if	LDBL_MANT_DIG == 53
#ifdef	lint
/* PROTOLIB1 */
long int lroundl(long double);
#else	/* lint */
__weak_alias(lroundl, lround);
#endif	/* lint */
#endif	/* LDBL_MANT_DIG == 53 */
