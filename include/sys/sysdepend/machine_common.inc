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
; *	@(#)machine_common.inc (sys)
; *
; *	Machine type definition
; */

	.aifdef __SYS_MACHINE_COMMON_H__
	.aelse
__SYS_MACHINE_COMMON_H__: .define " "

	.aifdef _ASP_SH7750R_
	.include "sys/sysdepend/asp_sh7750r/machine_depend.inc"
	.aendi

	.aendi ;/* __SYS_MACHINE_COMMON_H__ */
