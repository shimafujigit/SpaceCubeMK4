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
 *	@(#)machine_depend.inc (sys/ASP-LEON5)
 *
 *	Machine type definition (system dependent)
 */

#ifndef __SYS_MACHINE_DEPEND_H__
#define	__SYS_MACHINE_DEPEND_H__	1

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * BIGENDIAN		1 if big endian 
 * VIRTUAL_ADDRESS	1 if virtual memory 
 * ALLOCA_NOSPT		1 if alloca() is not supported 
 */

/* ----- ASP-LEON5(SPARC) definition ----- */

.equ	CPU_SPARC,		1
.equ	ASP_LEON5,		1
.equ	ALLOW_MISALIGN,		0
.equ	BIGENDIAN,		1
.equ	VIRTUAL_ADDRESS,	0
.equ	ALLOCA_NOSPT,		0

.equ	USE_MEM_PROTECT,	0	/* for Memory protection */

#endif /* __SYS_MACHINE_DEPEND_H__ */
