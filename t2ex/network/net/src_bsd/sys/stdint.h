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

/*	$NetBSD: stdint.h,v 1.6 2008/04/28 20:24:11 martin Exp $	*/

/*-
 * Copyright (c) 2001, 2004 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Klaus Klein.
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

#ifndef _SYS_STDINT_H_
#define _SYS_STDINT_H_

#include <sys/cdefs.h>
#include <machine/int_types.h>

#ifndef	int8_t
typedef	__int8_t	int8_t;
#define	int8_t		__int8_t
#endif

#ifndef	uint8_t
typedef	__uint8_t	uint8_t;
#define	uint8_t		__uint8_t
#endif

#ifndef	int16_t
typedef	__int16_t	int16_t;
#define	int16_t		__int16_t
#endif

#ifndef	uint16_t
typedef	__uint16_t	uint16_t;
#define	uint16_t	__uint16_t
#endif

#ifndef	int32_t
typedef	__int32_t	int32_t;
#define	int32_t		__int32_t
#endif

#ifndef	uint32_t
typedef	__uint32_t	uint32_t;
#define	uint32_t	__uint32_t
#endif

#ifndef	int64_t
typedef	__int64_t	int64_t;
#define	int64_t		__int64_t
#endif

#ifndef	uint64_t
typedef	__uint64_t	uint64_t;
#define	uint64_t	__uint64_t
#endif

#ifndef	intptr_t
typedef	__intptr_t	intptr_t;
#define	intptr_t	__intptr_t
#endif

#ifndef	uintptr_t
typedef	__uintptr_t	uintptr_t;
#define	uintptr_t	__uintptr_t
#endif

#include <machine/int_mwgwtypes.h>

#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)
#include <machine/int_limits.h>
#endif

#if !defined(__cplusplus) || defined(__STDC_CONSTANT_MACROS)
#include <machine/int_const.h>
#endif

#include <machine/wchar_limits.h>

#endif /* !_SYS_STDINT_H_ */
