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

/*	$OpenBSD: wchar.h,v 1.22 2012/01/05 20:37:50 kettenis Exp $	*/
/*	$NetBSD: wchar.h,v 1.16 2003/03/07 07:11:35 tshiozak Exp $	*/

/*-
 * Copyright (c)1999 Citrus Project,
 * All rights reserved.
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

/*-
 * Copyright (c) 1999, 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Julian Coleman.
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
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _WCHAR_H_
#define _WCHAR_H_

#include <sys/cdefs.h>
#include <sys/_types.h>

#ifndef	NULL
#ifdef	__GNUG__
#define	NULL	__null
#elif defined(__cplusplus)
#define	NULL	0L
#else
#define	NULL	((void *)0)
#endif
#endif

#include <stdio.h> /* for FILE* */

#if !defined(_WCHAR_T_DEFINED_) && !defined(__cplusplus)
#define _WCHAR_T_DEFINED_
typedef	__wchar_t	wchar_t;
#endif

#ifndef	_MBSTATE_T_DEFINED_
#define	_MBSTATE_T_DEFINED_
typedef	__mbstate_t	mbstate_t;
#endif

#ifndef	_WINT_T_DEFINED_
#define	_WINT_T_DEFINED_
typedef	__wint_t	wint_t;
#endif

#ifndef	_SIZE_T_DEFINED_
#define	_SIZE_T_DEFINED_
typedef	__size_t	size_t;
#endif

#ifndef	WEOF
#define	WEOF 	((wint_t)-1)
#endif

#ifndef	WCHAR_MIN
#define	WCHAR_MIN	(-0x7fffffff - 1)
#endif
#ifndef	WCHAR_MAX
#define	WCHAR_MAX	0x7fffffff
#endif

__BEGIN_DECLS
wint_t	btowc(int);
size_t	mbrlen(const char * __restrict, size_t, mbstate_t * __restrict);
size_t	mbrtowc(wchar_t * __restrict, const char * __restrict, size_t,
	    mbstate_t * __restrict);
int	mbsinit(const mbstate_t *);
size_t	mbsrtowcs(wchar_t * __restrict, const char ** __restrict, size_t,
	    mbstate_t * __restrict);
size_t	wcrtomb(char * __restrict, wchar_t, mbstate_t * __restrict);
wchar_t	*wcscat(wchar_t * __restrict, const wchar_t * __restrict);
wchar_t	*wcschr(const wchar_t *, wchar_t);
int	wcscmp(const wchar_t *, const wchar_t *);
int	wcscoll(const wchar_t *, const wchar_t *);
wchar_t	*wcscpy(wchar_t * __restrict, const wchar_t * __restrict);
size_t	wcscspn(const wchar_t *, const wchar_t *);
size_t	wcslen(const wchar_t *);
wchar_t	*wcsncat(wchar_t * __restrict, const wchar_t * __restrict,
	    size_t);
int	wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t	*wcsncpy(wchar_t * __restrict , const wchar_t * __restrict,
	    size_t);
wchar_t	*wcspbrk(const wchar_t *, const wchar_t *);
wchar_t	*wcsrchr(const wchar_t *, wchar_t);
size_t	wcsrtombs(char * __restrict, const wchar_t ** __restrict, size_t,
	    mbstate_t * __restrict);
size_t	wcsspn(const wchar_t *, const wchar_t *);
wchar_t	*wcsstr(const wchar_t *, const wchar_t *);
wchar_t *wcstok(wchar_t * __restrict, const wchar_t * __restrict,
		     wchar_t ** __restrict);
size_t	wcsxfrm(wchar_t *, const wchar_t *, size_t);
wchar_t	*wcswcs(const wchar_t *, const wchar_t *);
wchar_t	*wmemchr(const wchar_t *, wchar_t, size_t);
int	wmemcmp(const wchar_t *, const wchar_t *, size_t);
wchar_t	*wmemcpy(wchar_t * __restrict, const wchar_t * __restrict,
	    size_t);
wchar_t	*wmemmove(wchar_t *, const wchar_t *, size_t);
wchar_t	*wmemset(wchar_t *, wchar_t, size_t);

size_t	wcslcat(wchar_t *, const wchar_t *, size_t);
size_t	wcslcpy(wchar_t *, const wchar_t *, size_t);
int	wcswidth(const wchar_t *, size_t);
int	wctob(wint_t);
int	wcwidth(wchar_t);

double wcstod(const wchar_t * __restrict, wchar_t ** __restrict);
long int wcstol(const wchar_t * __restrict, wchar_t ** __restrict, int base);
unsigned long int wcstoul(const wchar_t * __restrict, wchar_t ** __restrict,
		int base);

#if __POSIX_VISIBLE >= 200809
wchar_t	*wcsdup(const wchar_t *);
int wcscasecmp(const wchar_t *, const wchar_t *);
int wcsncasecmp(const wchar_t *, const wchar_t *, size_t);
#endif

#if __ISO_C_VISIBLE >= 1999
float	wcstof(const wchar_t * __restrict, wchar_t ** __restrict);
long double wcstold(const wchar_t * __restrict, wchar_t ** __restrict);
size_t wcsftime(wchar_t * __restrict, size_t, const wchar_t *__restrict,
	const struct tm *__restrict);
#endif

#if (defined(__GNUC__) && __GNUC__ >= 2 && !defined(__STRICT_ANSI__)) || \
    __ISO_C_VISIBLE >= 1999
/* LONGLONG */
long long int wcstoll(const wchar_t * __restrict,
	wchar_t ** __restrict, int base);
/* LONGLONG */
unsigned long long int wcstoull(const wchar_t * __restrict,
	wchar_t ** __restrict, int base);
#endif

wint_t ungetwc(wint_t, FILE *);
wint_t fgetwc(FILE *);
wchar_t *fgetws(wchar_t * __restrict, int, FILE * __restrict);
wint_t getwc(FILE *);
wint_t getwchar(void);
wint_t fputwc(wchar_t, FILE *);
int fputws(const wchar_t * __restrict, FILE * __restrict);
wint_t putwc(wchar_t, FILE *);
wint_t putwchar(wchar_t);

int fwide(FILE *, int);

int fwprintf(FILE * __restrict, const wchar_t * __restrict, ...);
int swprintf(wchar_t * __restrict, size_t, const wchar_t * __restrict, ...);
int vfwprintf(FILE * __restrict, const wchar_t * __restrict, __va_list);
int vswprintf(wchar_t * __restrict, size_t, const wchar_t * __restrict,
	__va_list);
int vwprintf(const wchar_t * __restrict, __va_list);
int wprintf(const wchar_t * __restrict, ...);

int fwscanf(FILE * __restrict, const wchar_t * __restrict, ...);
int swscanf(const wchar_t * __restrict, const wchar_t * __restrict, ...);
int vfwscanf(FILE * __restrict, const wchar_t * __restrict, __va_list);
int vswscanf(const wchar_t * __restrict, const wchar_t * __restrict, __va_list);
int vwscanf(const wchar_t * __restrict, __va_list);
int wscanf(const wchar_t * __restrict, ...);

#define getwc(f) fgetwc(f)
#define getwchar() getwc(stdin)
#define putwc(wc, f) fputwc((wc), (f))
#define putwchar(wc) putwc((wc), stdout)
__END_DECLS

#endif /* !_WCHAR_H_ */
