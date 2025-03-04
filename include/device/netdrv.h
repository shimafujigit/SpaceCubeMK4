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

/*
 *	@(#)netdrv.h
 */

#ifndef __DEVICE_NETDRV_H__
#define __DEVICE_NETDRV_H__

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	Data Number
 */
typedef enum {
	DN_NETEVENT		= -100,
	DN_NETRESET		= -103,
	DN_NETADDR		= -105,
	DN_NETDEVINFO		= -110,
	DN_NETSTINFO		= -111,
	DN_NETCSTINFO		= -112,
	DN_NETRXBUF		= -113,
	DN_NETRXBUFSZ		= -114,
	DN_NETMCASTLIST		= -115,
	DN_NETALLMCAST		= -116,
} NetDrvDataNo;

/*
 *	DN_NETEVENT
 */
typedef struct {
	UH	len;
	VP	buf;
} NetEvent;

/*
 *	DN_NETADDR
 */
typedef	struct	{
	UB	c[6];
} NetAddr;

/*
 *	DN_NETDEVINFO
 */
#define	L_NETPNAME	(40)

typedef	struct {
	UB	name[L_NETPNAME];
	UW	iobase;
	UW	iosize;
	UW	intno;
	UW	kind;
	UW	ifconn;
	W	stat;
} NetDevInfo;

/* ifconn */
#define	IFC_UNKNOWN	(0)
#define	IFC_AUI		(1)
#define	IFC_TPE		(2)
#define	IFC_BNC		(3)
#define	IFC_100TX	(4)
#define	IFC_100FX	(5)
#define	IFC_AUTO	(6)
#define	IFC_1000T	(7)
#define	IFC_WLAN	(16)

/*
 *	DN_NETSTINFO, DN_NETCSTINFO
 */
typedef struct	{
	UW	rxpkt;
	UW	rxerr;
	UW	misspkt;
	UW	invpkt;
	UW	txpkt;
	UW	txerr;
	UW	txbusy;
	UW	collision;
	UW	nint;
	UW	rxint;
	UW	txint;
	UW	overrun;
	UW	hwerr;
	UW	other[3];
} NetStInfo;

/*
 *	DN_NETRXBUFSZ
 */
typedef	struct {
	W	minsz;
	W	maxsz;
} NetRxBufSz;

#ifdef __cplusplus
}
#endif
#endif /* __DEVICE_NETDRV_H__ */
