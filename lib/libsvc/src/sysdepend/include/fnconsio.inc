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
; *    Changed by UC Technology at 2012/12/25.
; *    
; *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
; *    Copyright (c) 2012 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; */

;/*
; *	Extended SVC function code [HEW]
; *
; *	   created from <sys/consio.h>
; */

	.include "sys/ssid.inc"

CONSIO_CONSOLE_GET_FN:	.define	"(H'00100400 | CONSIO_SVC)"
CONSIO_CONSOLE_PUT_FN:	.define	"(H'00110400 | CONSIO_SVC)"
CONSIO_CONSOLE_CONF_FN:	.define	"(H'00120200 | CONSIO_SVC)"
CONSIO_CONSOLE_IN_FN:	.define	"(H'00200300 | CONSIO_SVC)"
CONSIO_CONSOLE_OUT_FN:	.define	"(H'00210300 | CONSIO_SVC)"
CONSIO_CONSOLE_CTL_FN:	.define	"(H'00220300 | CONSIO_SVC)"

