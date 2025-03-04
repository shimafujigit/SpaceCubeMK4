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

/*	$NetBSD: subr_prf2.c,v 1.3 2008/09/23 22:20:24 pooka Exp $	*/

/*-
 * Copyright (c) 1986, 1988, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *
 *	@(#)subr_prf.c	8.4 (Berkeley) 5/4/95
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: subr_prf2.c,v 1.3 2008/09/23 22:20:24 pooka Exp $");

#include <sys/param.h>
#ifndef T2EX
#include <sys/kprintf.h>
#endif
#include <sys/syslog.h>
#include <sys/systm.h>

/*
 * This function is in its own separate module to permit easier
 * standalone compilation without pulling in kprintf(), panic(), etcetc.
 */

#ifdef T2EX_LOG
const char hexdigits[] = "0123456789abcdef";
const char HEXDIGITS[] = "0123456789ABCDEF";
#endif

/*
 * tablefull: warn that a system table is full
 */

void
tablefull(const char *tab, const char *hint)
{
	if (hint)
		log(LOG_ERR, "%s: table is full - %s\n", tab, hint);
	else
		log(LOG_ERR, "%s: table is full\n", tab);
}

#ifndef T2EX
/*
 * bitmask_snprintf: print an interpreted bitmask to a buffer
 *
 * => returns pointer to the buffer
 */
char *
bitmask_snprintf(u_quad_t val, const char *p, char *bf, size_t buflen)
{
	char *bp, *q;
	size_t left;
	const char *sbase;
	char snbuf[KPRINTF_BUFSIZE];
	int base, bit, ch, len, sep;
	u_quad_t field;

	bp = bf;
	memset(bf, 0, buflen);

	/*
	 * Always leave room for the trailing NULL.
	 */
	left = buflen - 1;

	/*
	 * Print the value into the buffer.  Abort if there's not
	 * enough room.
	 */
	if (buflen < KPRINTF_BUFSIZE)
		return (bf);

	ch = *p++;
	base = ch != '\177' ? ch : *p++;
	sbase = base == 8 ? "%qo" : base == 10 ? "%qd" : base == 16 ? "%qx" : 0;
	if (sbase == 0)
		return (bf);	/* punt if not oct, dec, or hex */

	snprintf(snbuf, sizeof(snbuf), sbase, val);
	for (q = snbuf ; *q ; q++) {
		*bp++ = *q;
		left--;
	}

	/*
	 * If the value we printed was 0 and we're using the old-style format,
	 * or if we don't have room for "<x>", we're done.
	 */
	if (((val == 0) && (ch != '\177')) || left < 3)
		return (bf);

#define PUTBYTE(b, c, l) do {	\
	*(b)++ = (c);		\
	if (--(l) == 0)		\
		goto out;	\
} while (/*CONSTCOND*/ 0)
#define PUTSTR(b, p, l) do {		\
	int c;				\
	while ((c = *(p)++) != 0) {	\
		*(b)++ = c;		\
		if (--(l) == 0)		\
			goto out;	\
	}				\
} while (/*CONSTCOND*/ 0)

	/*
	 * Chris Torek's new bitmask format is identified by a leading \177
	 */
	sep = '<';
	if (ch != '\177') {
		/* old (standard) format. */
		for (;(bit = *p++) != 0;) {
			if (val & (1 << (bit - 1))) {
				PUTBYTE(bp, sep, left);
				for (; (ch = *p) > ' '; ++p) {
					PUTBYTE(bp, ch, left);
				}
				sep = ',';
			} else
				for (; *p > ' '; ++p)
					continue;
		}
	} else {
		/* new quad-capable format; also does fields. */
		field = val;
		while ((ch = *p++) != '\0') {
			bit = *p++;	/* now 0-origin */
			switch (ch) {
			case 'b':
				if (((u_int)(val >> bit) & 1) == 0)
					goto skip;
				PUTBYTE(bp, sep, left);
				PUTSTR(bp, p, left);
				sep = ',';
				break;
			case 'f':
			case 'F':
				len = *p++;	/* field length */
				field = (val >> bit) & ((1ULL << len) - 1);
				if (ch == 'F')	/* just extract */
					break;
				PUTBYTE(bp, sep, left);
				sep = ',';
				PUTSTR(bp, p, left);
				PUTBYTE(bp, '=', left);
				sprintf(snbuf, sbase, field);
				q = snbuf; PUTSTR(bp, q, left);
				break;
			case '=':
			case ':':
				/*
				 * Here "bit" is actually a value instead,
				 * to be compared against the last field.
				 * This only works for values in [0..255],
				 * of course.
				 */
				if ((int)field != bit)
					goto skip;
				if (ch == '=')
					PUTBYTE(bp, '=', left);
				else {
					PUTBYTE(bp, sep, left);
					sep = ',';
				}
				PUTSTR(bp, p, left);
				break;
			default:
			skip:
				while (*p++ != '\0')
					continue;
				break;
			}
		}
	}
	if (sep != '<')
		PUTBYTE(bp, '>', left);

out:
	return (bf);

#undef PUTBYTE
#undef PUTSTR
}
#endif /* T2EX */
