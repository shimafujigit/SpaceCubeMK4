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
; *	   created from <tk/devmgr.h>
; */

	.include "sys/ssid.inc"

DEVICE_TK_OPN_DEV_FN:	.define	"(H'00010200 | DEVICE_SVC)"
DEVICE_TK_CLS_DEV_FN:	.define	"(H'00020200 | DEVICE_SVC)"
DEVICE_TK_REA_DEV_FN:	.define	"(H'00030500 | DEVICE_SVC)"
DEVICE_TK_SREA_DEV_FN:	.define	"(H'00040500 | DEVICE_SVC)"
DEVICE_TK_WRI_DEV_FN:	.define	"(H'00050500 | DEVICE_SVC)"
DEVICE_TK_SWRI_DEV_FN:	.define	"(H'00060500 | DEVICE_SVC)"
DEVICE_TK_WAI_DEV_FN:	.define	"(H'00070500 | DEVICE_SVC)"
DEVICE_TK_SUS_DEV_FN:	.define	"(H'00080100 | DEVICE_SVC)"
DEVICE_TK_GET_DEV_FN:	.define	"(H'00090200 | DEVICE_SVC)"
DEVICE_TK_REF_DEV_FN:	.define	"(H'000a0200 | DEVICE_SVC)"
DEVICE_TK_OREF_DEV_FN:	.define	"(H'000b0200 | DEVICE_SVC)"
DEVICE_TK_LST_DEV_FN:	.define	"(H'000c0300 | DEVICE_SVC)"
DEVICE_TK_EVT_DEV_FN:	.define	"(H'000d0300 | DEVICE_SVC)"
DEVICE_TK_REA_DEV_DU_FN:	.define	"(H'000e0500 | DEVICE_SVC)"
DEVICE_TK_SREA_DEV_D_FN:	.define	"(H'000f0500 | DEVICE_SVC)"
DEVICE_TK_WRI_DEV_DU_FN:	.define	"(H'00100500 | DEVICE_SVC)"
DEVICE_TK_SWRI_DEV_D_FN:	.define	"(H'00110500 | DEVICE_SVC)"
DEVICE_TK_WAI_DEV_U_FN:	.define	"(H'00120500 | DEVICE_SVC)"
DEVICE_TK_DEF_DEV_FN:	.define	"(H'01000300 | DEVICE_SVC)"
DEVICE_TK_REF_IDV_FN:	.define	"(H'01010100 | DEVICE_SVC)"

