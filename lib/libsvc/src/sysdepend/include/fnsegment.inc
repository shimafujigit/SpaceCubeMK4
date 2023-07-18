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
; *	   created from <sys/segment.h>
; */

	.include "sys/ssid.inc"

SEG_LOCKSPACE_FN:	.define	"(H'01000200 | SEG_SVC)"
SEG_UNLOCKSPACE_FN:	.define	"(H'01010200 | SEG_SVC)"
SEG_CNVPHYSICALADDR_FN:	.define	"(H'01020300 | SEG_SVC)"
SEG_CHKSPACE_FN:	.define	"(H'01030400 | SEG_SVC)"
SEG_CHKSPACETSTR_FN:	.define	"(H'01040400 | SEG_SVC)"
SEG_CHKSPACEBSTR_FN:	.define	"(H'01050400 | SEG_SVC)"
SEG_CHKSPACELEN_FN:	.define	"(H'01060500 | SEG_SVC)"
SEG_READMEMSPACE_FN:	.define	"(H'01070400 | SEG_SVC)"
SEG_WRITEMEMSPACE_FN:	.define	"(H'01080400 | SEG_SVC)"
SEG_SETMEMSPACEB_FN:	.define	"(H'01090400 | SEG_SVC)"
SEG_FLUSHMEMCACHE_FN:	.define	"(H'010a0300 | SEG_SVC)"
SEG_SETCACHEMODE_FN:	.define	"(H'010b0300 | SEG_SVC)"
SEG_CONTROLCACHE_FN:	.define	"(H'010c0300 | SEG_SVC)"
SEG_GETSPACEINFO_FN:	.define	"(H'010d0300 | SEG_SVC)"
SEG_SETMEMORYACCESS_FN:	.define	"(H'010e0300 | SEG_SVC)"
SEG_MAPMEMORY_FN:	.define	"(H'02000400 | SEG_SVC)"
SEG_UNMAPMEMORY_FN:	.define	"(H'02010100 | SEG_SVC)"
SEG_MAKESPACE_FN:	.define	"(H'10000400 | SEG_SVC)"
SEG_UNMAKESPACE_FN:	.define	"(H'10010300 | SEG_SVC)"
SEG_CHANGESPACE_FN:	.define	"(H'10020400 | SEG_SVC)"

