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
; *	   created from <tk/sysmgr.h>
; */

	.include "sys/ssid.inc"

SYSTEM_TK_GET_CFN_FN:	.define	"(H'00010300 | SYSTEM_SVC)"
SYSTEM_TK_GET_CFS_FN:	.define	"(H'00020300 | SYSTEM_SVC)"
SYSTEM_TK_GET_SMB_FN:	.define	"(H'01000300 | SYSTEM_SVC)"
SYSTEM_TK_REL_SMB_FN:	.define	"(H'01010100 | SYSTEM_SVC)"
SYSTEM_TK_REF_SMB_FN:	.define	"(H'01020100 | SYSTEM_SVC)"
SYSTEM__GETKERNELCOMMONAREA_FN:	.define	"(H'10000100 | SYSTEM_SVC)"
SYSTEM__SYSLOG_SEND_FN:	.define	"(H'10010200 | SYSTEM_SVC)"

