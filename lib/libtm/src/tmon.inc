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
; *	@(#)tmon.inc (libtm)
; *
; *	T-Monitor library common definitions 
; *
; *	Also accessed from the assembler 
; */

	.include "machine.inc"
	.include "tmsvc.inc"
	.include "tk/errno.inc"

;/*
; * Function code 
; */
TM_MONITOR:	.equ	0
TM_GETCHAR:	.equ	1
TM_PUTCHAR:	.equ	2
TM_GETLINE:	.equ	3
TM_PUTSTRING:	.equ	4
TM_COMMAND:	.equ	5
TM_READDISK:	.equ	6
TM_WRITEDISK:	.equ	7
TM_INFODISK:	.equ	8
TM_EXIT:	.equ	9

TM_EXTSVC:	.equ	255
