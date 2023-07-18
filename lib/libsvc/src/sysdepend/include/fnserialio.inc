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
; *	   created from <device/serialio.h>
; */

	.include "sys/ssid.inc"

SERIAL_SERIAL_IN_FN:	.define	"(H'00010500 | SERIAL_SVC)"
SERIAL_SERIAL_OUT_FN:	.define	"(H'00020500 | SERIAL_SVC)"
SERIAL_SERIAL_CTL_FN:	.define	"(H'00030300 | SERIAL_SVC)"

