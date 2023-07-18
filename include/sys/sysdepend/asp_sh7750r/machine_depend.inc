;/*
; *----------------------------------------------------------------------
; *    T-Kernel 2.0 Software Package
; *
; *    Copyright 2011 by Ken Sakamura.
; *    This software is distributed under the T-License 2.0.
; *----------------------------------------------------------------------
; *
; *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
; *
; *----------------------------------------------------------------------
; *    Changes: Adapted to the ASP-SH7750R Board.
; *    Changed by UC Technology at 2013/01/28.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2013 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */


;/*
; *	@(#)machine_depend.inc (sys/ASP-SH7750R)
; *
; *	Machine type definition (system dependent)
; */

	.aifdef __SYS_MACHINE_DEPEND_H__
	.aelse
__SYS_MACHINE_DEPEND_H__: .define " "

;/*
; * CPU_xxxx		CPU type
; * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
; * BIGENDIAN		1 if big endian 
; * VIRTUAL_ADDRESS	1 if virtual memory 
; * ALLOCA_NOSPT		1 if alloca() is not supported 
; */

;/* ----- ASP-SH7750R(SH4) definition ----- */

_CPU_SH4_:		.define	" "
CPU_SH4:		.assigna	1
CPU_SH7750R:		.equ	1
ASP_SH7750R:		.assigna	1
ALLOW_MISALIGN:		.equ	0
BIGENDIAN:		.equ	1	; /* !! */
ENDIAN_BIG:		.assigna	1
VIRTUAL_ADDRESS:	.equ	1
ALLOCA_NOSPT:		.equ	0

USE_MEM_PROTECT:	.assigna	1	; /* for Memory protection */

	.aendi ;/* __SYS_MACHINE_DEPEND_H__ */
