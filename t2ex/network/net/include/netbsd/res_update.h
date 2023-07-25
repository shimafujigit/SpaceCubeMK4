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

/*	$NetBSD: res_update.h,v 1.6.14.1 2011/01/09 20:43:27 riz Exp $	*/

/*
 * Copyright (c) 2004 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1999 by Internet Software Consortium, Inc.
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

/*
 *	Id: res_update.h,v 1.3 2005/04/27 04:56:15 sra Exp
 */

#ifndef __RES_UPDATE_H
#define __RES_UPDATE_H

#include <sys/types.h>
#include <arpa/nameser.h>
#include <isc/list.h>
#include <resolv.h>

/*%
 * This RR-like structure is particular to UPDATE.
 */
struct ns_updrec {
	LINK(struct ns_updrec) r_link, r_glink;
	ns_sect		r_section;	/*%< ZONE/PREREQUISITE/UPDATE */
	char *		r_dname;	/*%< owner of the RR */
	ns_class	r_class;	/*%< class number */
	ns_type		r_type;		/*%< type number */
	uint32_t	r_ttl;		/*%< time to live */
	u_char *	r_data;		/*%< rdata fields as text string */
	u_int		r_size;		/*%< size of r_data field */
	int		r_opcode;	/*%< type of operation */
	/* following fields for private use by the resolver/server routines */
	struct databuf *r_dp;		/*%< databuf to process */
	struct databuf *r_deldp;	/*%< databuf's deleted/overwritten */
	u_int		r_zone;		/*%< zone number on server */
};
typedef struct ns_updrec ns_updrec;
typedef	LIST(ns_updrec)	ns_updque;

#define res_mkupdate		__res_mkupdate
#define res_update		__res_update
#define res_mkupdrec		__res_mkupdrec
#define res_freeupdrec		__res_freeupdrec
#define res_nmkupdate		__res_nmkupdate
#define res_nupdate		__res_nupdate

int		res_mkupdate(ns_updrec *, u_char *, int);
int		res_update(ns_updrec *);
ns_updrec *	res_mkupdrec(int, const char *, u_int, u_int, u_long);
void		res_freeupdrec(ns_updrec *);
int		res_nmkupdate(res_state, ns_updrec *, u_char *, int);
int		res_nupdate(res_state, ns_updrec *, ns_tsig_key *);

#endif /*__RES_UPDATE_H*/
