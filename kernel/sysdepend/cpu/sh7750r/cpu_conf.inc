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
; *	cpu_conf.inc (SH7750R)
; *	CPU-Dependent OS Configuration Information
; */

	.aifdef _CPU_CONF_
	.aelse
_CPU_CONF_: .define " "

;/*
; * Define 1 when using MMU
; *   0: not using MMU
; *   1: using MMU
; */
USE_MMU:	.equ	0

;/*
; * Definition of minimum system stack size
; *	Minimum system stack size when setting the system stack size
; *	per task by 'tk_cre_tsk().'
; */
MIN_SYS_STACK_SIZE:	.equ	1024

;/*
; * Maximum logical space ID 
; */
MAX_LSID:	.equ	255

;/* ------------------------------------------------------------------------ */
;/*
; *	SH specific setting
; */

;/*
; * Define this value as 1 when using MDR DCT (only for SH3)
; *   0: not using MDR DCT
; *   1: using MDR DCT
; */
USE_MDR_DCT:	.assigna	0

;/*
; * Define this value as 1 when using SH3 DSP
; *   0: not using SH3 DSP
; *   1: using SH3 DSP
; */
USE_SH3_DSP:	.assigna	0

	.aendi ;/* _CPU_CONF_ */
