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

/*	$OpenBSD: wcio.h,v 1.1 2005/06/17 20:40:32 espie Exp $	*/
/* $NetBSD: wcio.h,v 1.3 2003/01/18 11:30:00 thorpej Exp $ */

/*-
 * Copyright (c)2001 Citrus Project,
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
 *
 * $Citrus$
 */

#ifndef _WCIO_H_
#define _WCIO_H_

/* minimal requirement of SUSv2 */
#define WCIO_UNGETWC_BUFSIZE 1

struct wchar_io_data {
	mbstate_t wcio_mbstate_in;
	mbstate_t wcio_mbstate_out;

	wchar_t wcio_ungetwc_buf[WCIO_UNGETWC_BUFSIZE];
	size_t wcio_ungetwc_inbuf;

	int wcio_mode; /* orientation */
};

#define WCIO_GET(fp) \
	(_EXT(fp) ? &(_EXT(fp)->_wcio) : (struct wchar_io_data *)0)

#define _SET_ORIENTATION(fp, mode) \
do {\
	struct wchar_io_data *_wcio = WCIO_GET(fp); \
	if (_wcio && _wcio->wcio_mode == 0) \
		_wcio->wcio_mode = (mode);\
} while (0)

/*
 * WCIO_FREE should be called by fclose
 */
#define WCIO_FREE(fp) \
do {\
	struct wchar_io_data *_wcio = WCIO_GET(fp); \
	if (_wcio) { \
		_wcio->wcio_mode = 0;\
		_wcio->wcio_ungetwc_inbuf = 0;\
	} \
} while (0)

#define WCIO_FREEUB(fp) \
do {\
	struct wchar_io_data *_wcio = WCIO_GET(fp); \
	if (_wcio) { \
		_wcio->wcio_ungetwc_inbuf = 0;\
	} \
} while (0)

#define WCIO_INIT(fp) \
	memset(WCIO_GET(fp), 0, sizeof(struct wchar_io_data))

#endif /*_WCIO_H_*/
