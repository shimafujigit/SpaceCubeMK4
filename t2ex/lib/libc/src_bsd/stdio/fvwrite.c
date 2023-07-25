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

/*	$OpenBSD: fvwrite.c,v 1.17 2009/11/09 00:18:27 kurt Exp $ */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "local.h"
#include "fvwrite.h"

/*
 * Write some memory regions.  Return zero on success, EOF on error.
 *
 * This routine is large and unsightly, but most of the ugliness due
 * to the three different kinds of output buffering is handled here.
 */
int
__sfvwrite(FILE *fp, struct __suio *uio)
{
	size_t len;
	char *p;
	struct __siov *iov;
	int w, s;
	char *nl;
	int nlknown, nldist;

	if ((len = uio->uio_resid) == 0)
		return (0);
	/* make sure we can write */
	if (cantwrite(fp)) {
		__sseterr(fp, EBADF);
		return (EOF);
	}

#define	MIN(a, b) ((a) < (b) ? (a) : (b))
#define	COPY(n)	  (void)memcpy((void *)fp->_p, (void *)p, (size_t)(n))

	iov = uio->uio_iov;
	p = iov->iov_base;
	len = iov->iov_len;
	iov++;
#define GETIOV(extra_work) \
	while (len == 0) { \
		extra_work; \
		p = iov->iov_base; \
		len = iov->iov_len; \
		iov++; \
	}
	if (fp->_flags & __SNBF) {
		/*
		 * Unbuffered: write up to BUFSIZ bytes at a time.
		 */
		do {
			GETIOV(;);
			w = (*fp->_write)(fp->_cookie, p, MIN(len, BUFSIZ));
			if (w <= 0)
				goto err;
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	} else if ((fp->_flags & __SLBF) == 0) {
		/*
		 * Fully buffered: fill partially full buffer, if any,
		 * and then flush.  If there is no partial buffer, write
		 * one _bf._size byte chunk directly (without copying).
		 *
		 * String output is a special case: write as many bytes
		 * as fit, but pretend we wrote everything.  This makes
		 * snprintf() return the number of bytes needed, rather
		 * than the number used, and avoids its write function
		 * (so that the write function can be invalid).
		 */
		do {
			GETIOV(;);
			if ((fp->_flags & (__SALC | __SSTR)) ==
			    (__SALC | __SSTR) && fp->_w < len) {
				size_t blen = fp->_p - fp->_bf._base;
				unsigned char *_base;
				int _size;

				/* Allocate space exponentially. */
				_size = fp->_bf._size;
				do {
					_size = (_size << 1) + 1;
				} while (_size < blen + len);
				_base = realloc(fp->_bf._base, _size + 1);
				if (_base == NULL)
					goto err;
				fp->_w += _size - fp->_bf._size;
				fp->_bf._base = _base;
				fp->_bf._size = _size;
				fp->_p = _base + blen;
			}
			w = fp->_w;
			if (fp->_flags & __SSTR) {
				if (len < w)
					w = len;
				COPY(w);	/* copy MIN(fp->_w,len), */
				fp->_w -= w;
				fp->_p += w;
				w = len;	/* but pretend copied all */
			} else if (fp->_p > fp->_bf._base && len > w) {
				/* fill and flush */
				COPY(w);
				/* fp->_w -= w; */ /* unneeded */
				fp->_p += w;
				if (__sflush(fp))
					goto err;
			} else if (len >= (w = fp->_bf._size)) {
				/* write directly */
				w = (*fp->_write)(fp->_cookie, p, w);
				if (w <= 0)
					goto err;
			} else {
				/* fill and done */
				w = len;
				COPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	} else {
		/*
		 * Line buffered: like fully buffered, but we
		 * must check for newlines.  Compute the distance
		 * to the first newline (including the newline),
		 * or `infinity' if there is none, then pretend
		 * that the amount to write is MIN(len,nldist).
		 */
		nlknown = 0;
		nldist = 0;	/* XXX just to keep gcc happy */
		do {
			GETIOV(nlknown = 0);
			if (!nlknown) {
				nl = memchr((void *)p, '\n', len);
				nldist = nl ? nl + 1 - p : len + 1;
				nlknown = 1;
			}
			s = MIN(len, nldist);
			w = fp->_w + fp->_bf._size;
			if (fp->_p > fp->_bf._base && s > w) {
				COPY(w);
				/* fp->_w -= w; */
				fp->_p += w;
				if (__sflush(fp))
					goto err;
			} else if (s >= (w = fp->_bf._size)) {
				w = (*fp->_write)(fp->_cookie, p, w);
				if (w <= 0)
				 	goto err;
			} else {
				w = s;
				COPY(w);
				fp->_w -= w;
				fp->_p += w;
			}
			if ((nldist -= w) == 0) {
				/* copied the newline: flush and forget */
				if (__sflush(fp))
					goto err;
				nlknown = 0;
			}
			p += w;
			len -= w;
		} while ((uio->uio_resid -= w) != 0);
	}
	return (0);

err:
	fp->_flags |= __SERR;
	return (EOF);
}
