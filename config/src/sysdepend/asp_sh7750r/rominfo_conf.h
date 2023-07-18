/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the SDS-IMX6 Board.
 *    Changed by UC Technology at 2013/01/29.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for i.MX6 Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo_conf.h (ASP-SH7750R)
 *
 *	RomInfo configuration
 */

#ifndef _ROMINFO_CONF_
#define _ROMINFO_CONF_

IMPORT UB SYSCONF[], DEVCONF[];

/* Kernel address */
#define RI_KERNEL_START	(FP)0x84002000	/* Kernel start address */
#define RI_SYSCONF	(UB*)SYSCONF	/* SYSCONF top */
#define RI_DEVCONF	(UB*)DEVCONF	/* DEVCONF top */

/* User definition */
#define RI_USERAREA_TOP	(void*)0x84400000	/* RAM user area top */
#define RI_USERINIT	(FP)NULL	/* User initialization program */
#define RI_RESETINIT	(FP)NULL	/* Reset initialization program */

/* Rom disk */
#define RI_RDSK_TYPE	(UW)0		/* ROM disk type */
#define RI_RDSK_BLOCK	(UW)0		/* ROM disk block size */
#define RI_RDSK_START	(UW)0		/* ROM disk start address */
#define RI_RDSK_END	(UW)0		/* ROM disk end address */

/* User Protect Code Area definition */
#define	RI_USER_CODE_START	(UW)0	/* Start address of user dode */
#define	RI_USER_CODE_END	(UW)0	/* End address of user code */
#define	RI_USER_DATA_START	(UW)0	/* Start address od user data */
#define	RI_USER_DATA_END	(UW)0	/* End address of user data */

#endif /* _ROMINFO_CONF_ */
