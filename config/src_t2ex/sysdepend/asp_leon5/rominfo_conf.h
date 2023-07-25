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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
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
 *	@(#)rominfo_conf.h (ASP-LEON5)
 *
 *	RomInfo configuration
 */

#ifndef _ROMINFO_CONF_
#define _ROMINFO_CONF_

IMPORT UB SYSCONF[], DEVCONF[];
IMPORT void KernelStart( void );

/* Kernel address */
#define RI_KERNEL_START	(FP)KernelStart	/* Kernel start address */
#define RI_SYSCONF	(UB*)SYSCONF	/* SYSCONF top */
#define RI_DEVCONF	(UB*)DEVCONF	/* DEVCONF top */

/* User definition */
#define RI_USERAREA_TOP	(void*)NULL	/* RAM user area top */

#define RI_USERINIT	(FP)NULL	/* User initialization program */
#define RI_RESETINIT	(FP)NULL	/* Reset initialization program */

/* Hardware configuration */
#define	RI_DBGPORT_BPS	(UW)19200	/* Debug port speed (bps) */

/* Rom disk */
#define RI_RDSK_TYPE	(UW)0		/* ROM disk type */
#define RI_RDSK_BLOCK	(UW)0		/* ROM disk block size */
#define RI_RDSK_START	(UW)0		/* ROM disk start address */
#define RI_RDSK_END	(UW)0		/* ROM disk end address */

#endif /* _ROMINFO_CONF_ */
