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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysinfo_depend.inc (sys/ASP-LEON5)
 *
 *	System common information 
 *
 */

#ifndef __SYS_SYSINFO_DEPEND_H__
#define	__SYS_SYSINFO_DEPEND_H__	1

/*
 * System common information 
 */
.equ	SCINFO,			syscommonarea + 0
.equ	SCINFO_RAMTOP,		SCINFO + 0*4		/* SCInfo.ramtop */
.equ	SCINFO_RAMEND,		SCINFO + 1*4		/* SCInfo.ramend */
.equ	SCINFO_TASKINDP,	SCINFO + 4*4		/* SCInfo.taskindp */

#endif /* __SYS_SYSINFO_DEPEND_H__ */
