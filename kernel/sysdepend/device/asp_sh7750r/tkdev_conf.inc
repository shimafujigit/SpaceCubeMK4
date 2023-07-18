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
; *	tkdev_conf.inc (SH7750R)
; *	Target System Configuration 
; */

	.aifdef _TKDEV_CONF_
	.aelse
_TKDEV_CONF_: .define " "

;/*
; * Timer TMU0
; */
VECNO_TMU0:	.equ	( H'400 >> 5 )	;/* TMU0 interrupt vector number */

TOCR:		.equ	H'ffd80000	;/* B:Timer output control register */
TSTR:		.equ	H'ffd80004	;/* B:Timer start register */

TCOR0:		.equ	H'ffd80008	;/* W:Timer constant register */
TCNT0:		.equ	H'ffd8000c	;/* W:Timer counter */
TCR0:		.equ	H'ffd80010	;/* H:Timer control register */

TSTR_STR0:	.equ	H'01		;/* TMU0 start (TSTR) */
TSTR_STR1:	.equ	H'02		;/* TMU1 start (TSTR) */
TSTR_STR2:	.equ	H'04		;/* TMU2 start (TSTR) */

TCR_UNF:	.equ	H'0100		;/* Underflow flag */
TCR_UNIE:	.equ	H'0020		;/* Underflow interrupt enable */
TCR_CKEG:	.equ	H'0018		;/* Clock edge */
TCR_TPSC:	.equ	H'0007		;/* Timer prescaler */
TCR_TPSC_P4:	.equ	H'0000		;/*	Peripheral clock/4 */
TCR_TPSC_P16:	.equ	H'0001		;/*	Peripheral clock/16 */
TCR_TPSC_P64:	.equ	H'0002		;/*	Peripheral clock/64 */
TCR_TPSC_P256:	.equ	H'0003		;/*	Peripheral clock/256 */
TCR_TPSC_P1024:	.equ	H'0004		;/*	Peripheral clock/1024 */
TCR_TPSC_RTC:	.equ	H'0006		;/*	RTC clock */
TCR_TPSC_EXT:	.equ	H'0007		;/*	External clock */

;/*
; * Timer interrupt level
; */
TIMER_INTLEVEL:	.equ	8

	.aendi ;/* _TKDEV_CONF_ */
