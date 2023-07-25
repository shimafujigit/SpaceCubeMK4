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

/*	$NetBSD: resolv_mt.h,v 1.1.1.2.32.1 2011/01/09 20:43:27 riz Exp $	*/

#ifndef _RESOLV_MT_H
#define _RESOLV_MT_H

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

/* Access functions for the libresolv private interface */

int	__res_enable_mt(void);
int	__res_disable_mt(void);

/* Per-thread context */

typedef struct {
int	no_hosts_fallback_private;
int	retry_save;
int	retry_private;
char	inet_nsap_ntoa_tmpbuf[255*3];
char	sym_ntos_unname[20];
char	sym_ntop_unname[20];
char	p_option_nbuf[40];
char	p_time_nbuf[40];
char	precsize_ntoa_retbuf[sizeof "90000000.00"];
char	loc_ntoa_tmpbuf[sizeof
"1000 60 60.000 N 1000 60 60.000 W -12345678.00m 90000000.00m 90000000.00m 90000000.00m"];
char	p_secstodate_output[15];
} mtctxres_t;

/* Thread-specific data (TSD) */

mtctxres_t	*___mtctxres(void);
#define mtctxres	(___mtctxres())

/* Various static data that should be TSD */

#define sym_ntos_unname		(mtctxres->sym_ntos_unname)
#define sym_ntop_unname		(mtctxres->sym_ntop_unname)
#define inet_nsap_ntoa_tmpbuf	(mtctxres->inet_nsap_ntoa_tmpbuf)
#define p_option_nbuf		(mtctxres->p_option_nbuf)
#define p_time_nbuf		(mtctxres->p_time_nbuf)
#define precsize_ntoa_retbuf	(mtctxres->precsize_ntoa_retbuf)
#define loc_ntoa_tmpbuf		(mtctxres->loc_ntoa_tmpbuf)
#define p_secstodate_output	(mtctxres->p_secstodate_output)

#endif /* _RESOLV_MT_H */
