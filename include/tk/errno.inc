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
; *	@(#)errno.inc (T-Kernel)
; *
; *	T-Kernel Error Code
; */

	.aifdef __TK_ERRNO_H__
	.aelse
__TK_ERRNO_H__: .define " "

E_OK:		.equ	0		;/* Completed successfully */

E_SYS:		.equ	((-5 << 16) | (0 & H'ffff))	;/* System error */
E_NOCOP:	.equ	((-6 << 16) | (0 & H'ffff))	;/* Coprocessor disable */
E_NOSPT:	.equ	((-9 << 16) | (0 & H'ffff))	;/* Unsupported function */
E_RSFN:		.equ	((-10 << 16) | (0 & H'ffff))	;/* Reserved function code number */
E_RSATR:	.equ	((-11 << 16) | (0 & H'ffff))	;/* Reserved attribute */
E_PAR:		.equ	((-17 << 16) | (0 & H'ffff))	;/* Parameter error */
E_ID:		.equ	((-18 << 16) | (0 & H'ffff))	;/* Incorrect ID number */
E_CTX:		.equ	((-25 << 16) | (0 & H'ffff))	;/* Context error */
E_MACV:		.equ	((-26 << 16) | (0 & H'ffff))	;/* Inaccessible memory/access violation */
E_OACV:		.equ	((-27 << 16) | (0 & H'ffff))	;/* Object access violation */
E_ILUSE:	.equ	((-28 << 16) | (0 & H'ffff))	;/* Incorrect system call use */
E_NOMEM:	.equ	((-33 << 16) | (0 & H'ffff))	;/* Insufficient memory */
E_LIMIT:	.equ	((-34 << 16) | (0 & H'ffff))	;/* Exceed system limits */
E_OBJ:		.equ	((-41 << 16) | (0 & H'ffff))	;/* Incorrect object state */
E_NOEXS:	.equ	((-42 << 16) | (0 & H'ffff))	;/* Object does not exist */
E_QOVR:		.equ	((-43 << 16) | (0 & H'ffff))	;/* Queuing overflow */
E_RLWAI:	.equ	((-49 << 16) | (0 & H'ffff))	;/* Forcibly release wait state */
E_TMOUT:	.equ	((-50 << 16) | (0 & H'ffff))	;/* Polling fail/time out */
E_DLT:		.equ	((-51 << 16) | (0 & H'ffff))	;/* Waited object was deleted */
E_DISWAI:	.equ	((-52 << 16) | (0 & H'ffff))	;/* Release wait caused by wait disable */

E_IO:		.equ	((-57 << 16) | (0 & H'ffff))	;/* Output/input error */
E_NOMDA:	.equ	((-58 << 16) | (0 & H'ffff))	;/* No media */
E_BUSY:		.equ	((-65 << 16) | (0 & H'ffff))	;/* Busy state */
E_ABORT:	.equ	((-66 << 16) | (0 & H'ffff))	;/* Aborted */
E_RONLY:	.equ	((-67 << 16) | (0 & H'ffff))	;/* write protected */

	.aendi ;/* __TK_ERRNO_H__ */
