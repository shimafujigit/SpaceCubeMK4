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

/*	$NetBSD: ns_samedomain.c,v 1.5.14.1 2011/01/09 20:43:28 riz Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1995,1999 by Internet Software Consortium.
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

#include <sys/cdefs.h>
#ifndef lint
#ifdef notdef
static const char rcsid[] = "Id: ns_samedomain.c,v 1.6 2005/04/27 04:56:40 sra Exp";
#else
__RCSID("$NetBSD: ns_samedomain.c,v 1.5.14.1 2011/01/09 20:43:28 riz Exp $");
#endif
#endif

#ifdef T2EX
#include <t2ex/errno.h>
#endif

#include "port_before.h"

#include <sys/types.h>
#include <arpa/nameser.h>
#include <errno.h>
#include <string.h>

#ifndef T2EX
#include "port_after.h"
#endif

#ifndef _LIBC
/*%
 *	Check whether a name belongs to a domain.
 *
 * Inputs:
 *\li	a - the domain whose ancestory is being verified
 *\li	b - the potential ancestor we're checking against
 *
 * Return:
 *\li	boolean - is a at or below b?
 *
 * Notes:
 *\li	Trailing dots are first removed from name and domain.
 *	Always compare complete subdomains, not only whether the
 *	domain name is the trailing string of the given name.
 *
 *\li	"host.foobar.top" lies in "foobar.top" and in "top" and in ""
 *	but NOT in "bar.top"
 */

int
ns_samedomain(const char *a, const char *b) {
	size_t la, lb;
	int diff, i, escaped;
	const char *cp;

	la = strlen(a);
	lb = strlen(b);

	/* Ignore a trailing label separator (i.e. an unescaped dot) in 'a'. */
	if (la != 0U && a[la - 1] == '.') {
		escaped = 0;
		/* Note this loop doesn't get executed if la==1. */
		for (i = la - 2; i >= 0; i--)
			if (a[i] == '\\') {
				if (escaped)
					escaped = 0;
				else
					escaped = 1;
			} else
				break;
		if (!escaped)
			la--;
	}

	/* Ignore a trailing label separator (i.e. an unescaped dot) in 'b'. */
	if (lb != 0U && b[lb - 1] == '.') {
		escaped = 0;
		/* note this loop doesn't get executed if lb==1 */
		for (i = lb - 2; i >= 0; i--)
			if (b[i] == '\\') {
				if (escaped)
					escaped = 0;
				else
					escaped = 1;
			} else
				break;
		if (!escaped)
			lb--;
	}

	/* lb == 0 means 'b' is the root domain, so 'a' must be in 'b'. */
	if (lb == 0U)
		return (1);

	/* 'b' longer than 'a' means 'a' can't be in 'b'. */
	if (lb > la)
		return (0);

	/* 'a' and 'b' being equal at this point indicates sameness. */
	if (lb == la)
		return (strncasecmp(a, b, lb) == 0);

	/* Ok, we know la > lb. */

	diff = la - lb;

	/*
	 * If 'a' is only 1 character longer than 'b', then it can't be
	 * a subdomain of 'b' (because of the need for the '.' label
	 * separator).
	 */
	if (diff < 2)
		return (0);

	/*
	 * If the character before the last 'lb' characters of 'b'
	 * isn't '.', then it can't be a match (this lets us avoid
	 * having "foobar.com" match "bar.com").
	 */
	if (a[diff - 1] != '.')
		return (0);

	/*
	 * We're not sure about that '.', however.  It could be escaped
         * and thus not a really a label separator.
	 */
	escaped = 0;
	for (i = diff - 2; i >= 0; i--)
		if (a[i] == '\\') {
			if (escaped)
				escaped = 0;
			else
				escaped = 1;
		} else
			break;
	if (escaped)
		return (0);
	  
	/* Now compare aligned trailing substring. */
	cp = a + diff;
	return (strncasecmp(cp, b, lb) == 0);
}

/*%
 *	is "a" a subdomain of "b"?
 */
int
ns_subdomain(const char *a, const char *b) {
	return (ns_samename(a, b) != 1 && ns_samedomain(a, b));
}
#endif

#ifndef T2EX
/*%
 *	make a canonical copy of domain name "src"
 *
 * notes:
 * \code
 *	foo -> foo.
 *	foo. -> foo.
 *	foo.. -> foo.
 *	foo\. -> foo\..
 *	foo\\. -> foo\\.
 * \endcode
 */

int
ns_makecanon(const char *src, char *dst, size_t dstsize) {
	size_t n = strlen(src);

	if (n + sizeof "." > dstsize) {			/*%< Note: sizeof == 2 */
#ifndef T2EX
		errno = EMSGSIZE;
		return (-1);
#else
		return ERRNOtoER(EMSGSIZE);
#endif
	}
	strcpy(dst, src);
	while (n >= 1U && dst[n - 1] == '.')		/*%< Ends in "." */
		if (n >= 2U && dst[n - 2] == '\\' &&	/*%< Ends in "\." */
		    (n < 3U || dst[n - 3] != '\\'))	/*%< But not "\\." */
			break;
		else
			dst[--n] = '\0';
	dst[n++] = '.';
	dst[n] = '\0';
	return (0);
}
#endif

#ifdef T2EX
static int
ns_countcanon(const char *src) {
	size_t n = strlen(src);

	while (n >= 1U && src[n - 1] == '.') {		/*%< Ends in "." */
		if (n >= 2U && src[n - 2] == '\\' &&	/*%< Ends in "\." */
		    (n < 3U || src[n - 3] != '\\')) {	/*%< But not "\\." */
			break;
		}
		else {
			n--;
		}
	}

	return n;
}
#endif

/*%
 *	determine whether domain name "a" is the same as domain name "b"
 *
 * return:
 *\li	-1 on error
 *\li	0 if names differ
 *\li	1 if names are the same
 */
int
ns_samename(const char *a, const char *b) {
#ifndef T2EX
	char ta[NS_MAXDNAME], tb[NS_MAXDNAME];

	if (ns_makecanon(a, ta, sizeof ta) < 0 ||
	    ns_makecanon(b, tb, sizeof tb) < 0)
		return (-1);
	if (strcasecmp(ta, tb) == 0)
		return (1);
	else
		return (0);
#else
	int na, nb;

	na = ns_countcanon(a);
	nb = ns_countcanon(b);

	if (na != nb) {
		return 0;
	}
	if (strncasecmp(a, b, na) == 0) {
		return 1;
	}
	else {
		return 0;
	}
#endif
}

/*! \file */
