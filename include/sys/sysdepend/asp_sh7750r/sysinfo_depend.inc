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
; *    Changed by UC Technology at 2012/12/20.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	@(#)sysinfo_depend.inc (sys/ASP-SH7750R)
; *
; *	System common information 
; *
; *   0x84001000+-----------------------+
; *		|Vector table of	| (EXPEVT  / 0x20 * 4) + 0x84001000
; *		|exception codes and	| (INTEVT2 / 0x20 * 4) + 0x84001000
; *		|interrupt factors	|
; *   0x840011c0+-----------------------+
; *		|TRAPA vector table	| TRA + 0x84001000
; *		|  TRAPA 0x70 - 0x7f	|
; *   0x84001200+-----------------------+
; *		|Default handler	|
; *   0x84001204+-----------------------+
; *		|TLB miss exception	| VBR + 0x400 exception handler
; *		|handler		|
; *   0x84001208+-----------------------+
; *		|System common info.	|
; *   0x84001280+-----------------------+
; *		|EIT stack		| (640 byte)
; *   0x84001500+-----------------------+
; */

	.aifdef __SYS_SYSINFO_DEPEND_H__
	.aelse
__SYS_SYSINFO_DEPEND_H__: .define " "

;/*
; * Vector table addresses 
; */
VECTBL:		.equ	H'84001000	;/* Vector table top */
DEFAULTHDR:	.equ	H'84001200	;/* Default handler */
TLBMISSHDR:	.equ	H'84001204	;/* TLB miss exception handler */
SCINFO_RAMTOP:	.equ	H'84001278	;/* SCInfo.ramtop */
SCINFO_RAMEND:	.equ	H'8400127c	;/* SCInfo.ramend */

	.aendi ;/* __SYS_SYSINFO_DEPEND_H__ */
