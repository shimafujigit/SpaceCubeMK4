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

/*	$OpenBSD: rune.h,v 1.2 2011/03/15 22:27:48 stsp Exp $ */
/*	$NetBSD: rune.h,v 1.9 2003/08/07 16:43:04 agc Exp $	*/

/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Paul Borman at Krystal Technologies.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)rune.h	8.1 (Berkeley) 6/27/93
 */

#ifndef	_RUNE_H_
#define	_RUNE_H_

#include <stdio.h>
#include <wchar.h>
#include "runetype.h"

/*
 * map _RTYPE_x to _CTYPE_x
 *
 * XXX: these should be defined in ctype.h and used in isxxx macros.
 *      (note: current isxxx macros use "old" NetBSD masks and
 *       _CTYPE_x are not public.)
 */
#define	_CTYPE_A	_RUNETYPE_A
#define	_CTYPE_C	_RUNETYPE_C
#define	_CTYPE_D	_RUNETYPE_D
#define	_CTYPE_G	_RUNETYPE_G
#define	_CTYPE_L	_RUNETYPE_L
#define	_CTYPE_P	_RUNETYPE_P
#define	_CTYPE_S	_RUNETYPE_S
#define	_CTYPE_U	_RUNETYPE_U
#define	_CTYPE_X	_RUNETYPE_X
#define	_CTYPE_B	_RUNETYPE_B
#define	_CTYPE_R	_RUNETYPE_R
#define	_CTYPE_I	_RUNETYPE_I
#define	_CTYPE_T	_RUNETYPE_T
#define	_CTYPE_Q	_RUNETYPE_Q
#define	_CTYPE_SWM	_RUNETYPE_SWM
#define	_CTYPE_SWS	_RUNETYPE_SWS
#define	_CTYPE_SW0	_RUNETYPE_SW0
#define	_CTYPE_SW1	_RUNETYPE_SW1
#define	_CTYPE_SW2	_RUNETYPE_SW2
#define	_CTYPE_SW3	_RUNETYPE_SW3

/*
 * global variables
 */
extern size_t __mb_len_max_runtime;
extern const _RuneLocale _DefaultRuneLocale;
extern const _RuneLocale *_CurrentRuneLocale;

#endif	/*! _RUNE_H_ */
