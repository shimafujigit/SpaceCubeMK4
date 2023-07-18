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
; *	@(#)ssid.inc (sys)
; *
; *	Subsystem ID and subsystem priority 
; *
; *	(NB) Included also from the assembler source,
; *	     so only macros can be described.
; */

	.aifdef __SYS_SSID_H__
	.aelse
__SYS_SSID_H__: .define " "

;/*
; * T-Kernel (1 - 9)
; *	1 - 6 and 9 were previously used for other purposes.
; */
CONSIO_SVC	.equ	1		;/* Console I/O */
CONSIO_PRI	.equ		1
SERIAL_SVC	.equ	5		;/* Low-level serial I/O */
SERIAL_PRI	.equ		1
DEVICE_SVC	.equ	8		;/* T-Kernel/SM device management */
DEVICE_PRI	.equ		4
SYSTEM_SVC	.equ	9		;/* T-Kernel/SM system management */
SYSTEM_PRI	.equ		1

;/*
; * Subsystem (10 - )
; */
MM_SVC		.equ	11		;/* Memory management		*/
MM_PRI		.equ		2
SM_SVC		.equ	16		;/* System management		*/
SM_PRI		.equ		2
SEG_SVC		.equ	17		;/* Segment management		*/
SEG_PRI		.equ		2

	.aendi ;/* __SYS_SSID_H__ */
