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

/*	$NetBSD: list.h,v 1.4.28.1 2011/01/09 20:43:28 riz Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1997,1999 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef LIST_H
#define LIST_H 1
#include <isc/assertions.h>

#define LIST(type) struct { type *head, *tail; }
#define INIT_LIST(list) \
	do { (list).head = NULL; (list).tail = NULL; } while (/*CONSTCOND*/0)

#define LINK(type) struct { type *prev, *next; }
#define INIT_LINK_TYPE(elt, link, type) \
	do { \
		(elt)->link.prev = (type *)(-1); \
		(elt)->link.next = (type *)(-1); \
	} while (/*CONSTCOND*/0)
#define INIT_LINK(elt, link) \
	INIT_LINK_TYPE(elt, link, void)
#define LINKED(elt, link) ((void *)((elt)->link.prev) != (void *)(-1) && \
			   (void *)((elt)->link.next) != (void *)(-1))

#define HEAD(list) ((list).head)
#define TAIL(list) ((list).tail)
#define EMPTY(list) ((list).head == NULL)

#define PREPEND(list, elt, link) \
	do { \
		INSIST(!LINKED(elt, link));\
		if ((list).head != NULL) \
			(list).head->link.prev = (elt); \
		else \
			(list).tail = (elt); \
		(elt)->link.prev = NULL; \
		(elt)->link.next = (list).head; \
		(list).head = (elt); \
	} while (/*CONSTCOND*/0)

#define APPEND(list, elt, link) \
	do { \
		INSIST(!LINKED(elt, link));\
		if ((list).tail != NULL) \
			(list).tail->link.next = (elt); \
		else \
			(list).head = (elt); \
		(elt)->link.prev = (list).tail; \
		(elt)->link.next = NULL; \
		(list).tail = (elt); \
	} while (/*CONSTCOND*/0)

#define UNLINK_TYPE(list, elt, link, type) \
	do { \
		INSIST(LINKED(elt, link));\
		if ((elt)->link.next != NULL) \
			(elt)->link.next->link.prev = (elt)->link.prev; \
		else { \
			INSIST((list).tail == (elt)); \
			(list).tail = (elt)->link.prev; \
		} \
		if ((elt)->link.prev != NULL) \
			(elt)->link.prev->link.next = (elt)->link.next; \
		else { \
			INSIST((list).head == (elt)); \
			(list).head = (elt)->link.next; \
		} \
		INIT_LINK_TYPE(elt, link, type); \
	} while (/*CONSTCOND*/0)
#define UNLINK(list, elt, link) \
	UNLINK_TYPE(list, elt, link, void)

#define PREV(elt, link) ((elt)->link.prev)
#define NEXT(elt, link) ((elt)->link.next)

#define INSERT_BEFORE(list, before, elt, link) \
	do { \
		INSIST(!LINKED(elt, link));\
		if ((before)->link.prev == NULL) \
			PREPEND(list, elt, link); \
		else { \
			(elt)->link.prev = (before)->link.prev; \
			(before)->link.prev = (elt); \
			(elt)->link.prev->link.next = (elt); \
			(elt)->link.next = (before); \
		} \
	} while (/*CONSTCOND*/0)

#define INSERT_AFTER(list, after, elt, link) \
	do { \
		INSIST(!LINKED(elt, link));\
		if ((after)->link.next == NULL) \
			APPEND(list, elt, link); \
		else { \
			(elt)->link.next = (after)->link.next; \
			(after)->link.next = (elt); \
			(elt)->link.next->link.prev = (elt); \
			(elt)->link.prev = (after); \
		} \
	} while (/*CONSTCOND*/0)

#define ENQUEUE(list, elt, link) APPEND(list, elt, link)
#define DEQUEUE(list, elt, link) UNLINK(list, elt, link)

#endif /* LIST_H */
/*! \file */
