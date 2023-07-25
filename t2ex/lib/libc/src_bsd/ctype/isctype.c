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

/*	$OpenBSD: isctype.c,v 1.11 2005/08/08 08:05:34 espie Exp $ */
/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
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
 */

#define _ANSI_LIBRARY
#include <ctype.h>
#include <stdio.h>

#undef isalnum
int
isalnum(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_U|_L|_N)));
}

#undef isalpha
int
isalpha(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_U|_L)));
}

#undef isblank
int
isblank(int c)
{
	return (c == ' ' || c == '\t');
}

#undef iscntrl
int
iscntrl(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _C));
}

#undef isdigit
int
isdigit(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _N));
}

#undef isgraph
int
isgraph(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_P|_U|_L|_N)));
}

#undef islower
int
islower(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _L));
}

#undef isprint
int
isprint(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_P|_U|_L|_N|_B)));
}

#undef ispunct
int
ispunct(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _P));
}

#undef isspace
int
isspace(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _S));
}

#undef isupper
int
isupper(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & _U));
}

#undef isxdigit
int
isxdigit(int c)
{
	return (c == EOF ? 0 : ((_ctype_ + 1)[(unsigned char)c] & (_N|_X)));
}

#undef isascii
int
isascii(int c)
{
	return ((unsigned int)c <= 0177);
}

#undef toascii
int
toascii(int c)
{
	return (c & 0177);
}

#undef _toupper
int
_toupper(int c)
{
	return (c - 'a' + 'A');
}

#undef _tolower
int
_tolower(int c)
{
	return (c - 'A' + 'a');
}
