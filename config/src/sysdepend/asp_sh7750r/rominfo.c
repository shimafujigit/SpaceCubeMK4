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
 *	@(#)rominfo.c (ASP-SH7750R)
 *
 *	ROM information
 */

#include <basic.h>
#include <sys/rominfo.h>
#include "rominfo_conf.h"

RomInfo rominfo = {
	RI_KERNEL_START,	/* Kernel startup address */
	RI_SYSCONF,		/* SYSCONF top */
	RI_DEVCONF,		/* DEVCONF top */
	RI_USERAREA_TOP,	/* RAM user area top */
	RI_USERINIT,		/* User initialization program address */
	RI_RESETINIT,		/* Reset initialization program address */
	{0},			/* Reserved (always 0) */

	RI_RDSK_TYPE,		/* ROM disk type */
	RI_RDSK_BLOCK,		/* ROM disk block size */
	RI_RDSK_START,		/* ROM disk start address */
	RI_RDSK_END,		/* ROM disk end address */
	{0},			/* Reserved (always 0) */
	RI_USER_CODE_START,	/* Start address of user dode */
	RI_USER_CODE_END,	/* End address of user code */
	RI_USER_DATA_START,	/* Start address od user data */
	RI_USER_DATA_END	/* End address of user data */
};

