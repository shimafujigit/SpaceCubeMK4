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

/*	$NetBSD: runetable.c,v 1.12 2003/08/07 16:43:04 agc Exp $	*/

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
 * $FreeBSD: src/lib/libc/locale/table.c,v 1.13 2000/02/08 07:43:25 obrien Exp $
 */

#include <sys/cdefs.h>

#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <assert.h>
#include <wchar.h>
#include "rune.h"
#include "rune_local.h"
#include "citrus_ctype.h"
#include "citrus_none.h"
#include <stdlib.h>

const _RuneLocale _DefaultRuneLocale = {
    _RUNE_MAGIC_1,
    "NONE",
    _DEFAULT_INVALID_RUNE,

    {	/*00*/	_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*08*/	_CTYPE_C,
		_CTYPE_C|_CTYPE_S|_CTYPE_B,
		_CTYPE_C|_CTYPE_S,
		_CTYPE_C|_CTYPE_S,
		_CTYPE_C|_CTYPE_S,
		_CTYPE_C|_CTYPE_S,
		_CTYPE_C,
		_CTYPE_C,
	/*10*/	_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*18*/	_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*20*/	_CTYPE_S|_CTYPE_B|_CTYPE_R|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
	/*28*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
	/*30*/	_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|0,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|1,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|2,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|3,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|4,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|5,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|6,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|7,
	/*38*/	_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|8,
		_CTYPE_D|_CTYPE_R|_CTYPE_G|_CTYPE_X|_CTYPE_SW1|9,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
	/*40*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|10,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|11,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|12,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|13,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|14,
		_CTYPE_U|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|15,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*48*/	_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*50*/	_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*58*/	_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_U|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
	/*60*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|10,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|11,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|12,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|13,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|14,
		_CTYPE_L|_CTYPE_X|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1|15,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*68*/	_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*70*/	_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
	/*78*/	_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_L|_CTYPE_R|_CTYPE_G|_CTYPE_A|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
		_CTYPE_C,
	/*80*/  _CTYPE_C, 
		_CTYPE_C, 
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*88*/  _CTYPE_C, 
		_CTYPE_C, 
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*90*/  _CTYPE_C, 
		_CTYPE_C, 
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
	/*98*/  _CTYPE_C, 
		_CTYPE_C, 
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
		_CTYPE_C,
    	/*A0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*A8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*B0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*B8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*C0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*C8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*D0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*D8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*E0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*E8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*F0*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    	/*F8*/	_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    		_CTYPE_P|_CTYPE_R|_CTYPE_G|_CTYPE_SW1,
    },
    {	0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,
     	0x08,	0x09,	0x0a,	0x0b,	0x0c,	0x0d,	0x0e,	0x0f,
	0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,
     	0x18,	0x19,	0x1a,	0x1b,	0x1c,	0x1d,	0x1e,	0x1f,
	0x20,	0x21,	0x22,	0x23,	0x24,	0x25,	0x26,	0x27,
     	0x28,	0x29,	0x2a,	0x2b,	0x2c,	0x2d,	0x2e,	0x2f,
	0x30,	0x31,	0x32,	0x33,	0x34,	0x35,	0x36,	0x37,
     	0x38,	0x39,	0x3a,	0x3b,	0x3c,	0x3d,	0x3e,	0x3f,
	0x40,	'a',	'b',	'c',	'd',	'e',	'f',	'g',
     	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o',
	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',
     	'x',	'y',	'z',	0x5b,	0x5c,	0x5d,	0x5e,	0x5f,
	0x60,	'a',	'b',	'c',	'd',	'e',	'f',	'g',
     	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o',
	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',
     	'x',	'y',	'z',	0x7b,	0x7c,	0x7d,	0x7e,	0x7f,
	0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,
     	0x88,	0x89,	0x8a,	0x8b,	0x8c,	0x8d,	0x8e,	0x8f,
	0x90,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
     	0x98,	0x99,	0x9a,	0x9b,	0x9c,	0x9d,	0x9e,	0x9f,
	0xa0,	0xa1,	0xa2,	0xa3,	0xa4,	0xa5,	0xa6,	0xa7,
     	0xa8,	0xa9,	0xaa,	0xab,	0xac,	0xad,	0xae,	0xaf,
	0xb0,	0xb1,	0xb2,	0xb3,	0xb4,	0xb5,	0xb6,	0xb7,
     	0xb8,	0xb9,	0xba,	0xbb,	0xbc,	0xbd,	0xbe,	0xbf,
	0xc0,	0xc1,	0xc2,	0xc3,	0xc4,	0xc5,	0xc6,	0xc7,
     	0xc8,	0xc9,	0xca,	0xcb,	0xcc,	0xcd,	0xce,	0xcf,
	0xd0,	0xd1,	0xd2,	0xd3,	0xd4,	0xd5,	0xd6,	0xd7,
     	0xd8,	0xd9,	0xda,	0xdb,	0xdc,	0xdd,	0xde,	0xdf,
	0xe0,	0xe1,	0xe2,	0xe3,	0xe4,	0xe5,	0xe6,	0xe7,
     	0xe8,	0xe9,	0xea,	0xeb,	0xec,	0xed,	0xee,	0xef,
	0xf0,	0xf1,	0xf2,	0xf3,	0xf4,	0xf5,	0xf6,	0xf7,
     	0xf8,	0xf9,	0xfa,	0xfb,	0xfc,	0xfd,	0xfe,	0xff,
    },
    {	0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,
     	0x08,	0x09,	0x0a,	0x0b,	0x0c,	0x0d,	0x0e,	0x0f,
	0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,
     	0x18,	0x19,	0x1a,	0x1b,	0x1c,	0x1d,	0x1e,	0x1f,
	0x20,	0x21,	0x22,	0x23,	0x24,	0x25,	0x26,	0x27,
     	0x28,	0x29,	0x2a,	0x2b,	0x2c,	0x2d,	0x2e,	0x2f,
	0x30,	0x31,	0x32,	0x33,	0x34,	0x35,	0x36,	0x37,
     	0x38,	0x39,	0x3a,	0x3b,	0x3c,	0x3d,	0x3e,	0x3f,
	0x40,	'A',	'B',	'C',	'D',	'E',	'F',	'G',
     	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',
     	'X',	'Y',	'Z',	0x5b,	0x5c,	0x5d,	0x5e,	0x5f,
	0x60,	'A',	'B',	'C',	'D',	'E',	'F',	'G',
     	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',
     	'X',	'Y',	'Z',	0x7b,	0x7c,	0x7d,	0x7e,	0x7f,
	0x80,	0x81,	0x82,	0x83,	0x84,	0x85,	0x86,	0x87,
     	0x88,	0x89,	0x8a,	0x8b,	0x8c,	0x8d,	0x8e,	0x8f,
	0x90,	0x91,	0x92,	0x93,	0x94,	0x95,	0x96,	0x97,
     	0x98,	0x99,	0x9a,	0x9b,	0x9c,	0x9d,	0x9e,	0x9f,
	0xa0,	0xa1,	0xa2,	0xa3,	0xa4,	0xa5,	0xa6,	0xa7,
     	0xa8,	0xa9,	0xaa,	0xab,	0xac,	0xad,	0xae,	0xaf,
	0xb0,	0xb1,	0xb2,	0xb3,	0xb4,	0xb5,	0xb6,	0xb7,
     	0xb8,	0xb9,	0xba,	0xbb,	0xbc,	0xbd,	0xbe,	0xbf,
	0xc0,	0xc1,	0xc2,	0xc3,	0xc4,	0xc5,	0xc6,	0xc7,
     	0xc8,	0xc9,	0xca,	0xcb,	0xcc,	0xcd,	0xce,	0xcf,
	0xd0,	0xd1,	0xd2,	0xd3,	0xd4,	0xd5,	0xd6,	0xd7,
     	0xd8,	0xd9,	0xda,	0xdb,	0xdc,	0xdd,	0xde,	0xdf,
	0xe0,	0xe1,	0xe2,	0xe3,	0xe4,	0xe5,	0xe6,	0xe7,
     	0xe8,	0xe9,	0xea,	0xeb,	0xec,	0xed,	0xee,	0xef,
	0xf0,	0xf1,	0xf2,	0xf3,	0xf4,	0xf5,	0xf6,	0xf7,
     	0xf8,	0xf9,	0xfa,	0xfb,	0xfc,	0xfd,	0xfe,	0xff,
    },
    { 0, NULL },
    { 0, NULL },
    { 0, NULL },
    NULL, 0,
    "646",
   &_citrus_ctype_none,
    {
	    { NULL, NULL, NULL },
	    { NULL, NULL, NULL },
    },
    {
	    { "alnum", _CTYPE_A|_CTYPE_D },
	    { "alpha", _CTYPE_A },
	    { "blank", _CTYPE_B },
	    { "cntrl", _CTYPE_C },
	    { "digit", _CTYPE_D },
	    { "graph", _CTYPE_G },
	    { "lower", _CTYPE_L },
	    { "print", _CTYPE_R },
	    { "punct", _CTYPE_P },
	    { "space", _CTYPE_S },
	    { "upper", _CTYPE_U },
	    { "xdigit", _CTYPE_X },
    },
    NULL
};

const _RuneLocale *_CurrentRuneLocale = &_DefaultRuneLocale;
