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
; *	T-Kernel/DS system call branch table [HEW]
; *
; *	   created from dbgspt.h
; */

	.include "machine.inc"

	.macro _TDSC_ENTRY name
	.aif	"\name" NE "_no_support"	;; avoid 801 (W) MULTIPLE SYMBOLS
	.import	\name
	.aendi
	.data.l	\name
	.endm

N_TDFN:	.equ	53

	_TDSC_ENTRY	__td_lst_tsk
	_TDSC_ENTRY	__td_lst_sem
	_TDSC_ENTRY	__td_lst_flg
	_TDSC_ENTRY	__td_lst_mbx
	_TDSC_ENTRY	__td_lst_mtx
	_TDSC_ENTRY	__td_lst_mbf
	_TDSC_ENTRY	__td_lst_por
	_TDSC_ENTRY	__td_lst_mpf
	_TDSC_ENTRY	__td_lst_mpl
	_TDSC_ENTRY	__td_lst_cyc
	_TDSC_ENTRY	__td_lst_alm
	_TDSC_ENTRY	__td_lst_ssy
	_TDSC_ENTRY	__td_ref_sem
	_TDSC_ENTRY	__td_ref_flg
	_TDSC_ENTRY	__td_ref_mbx
	_TDSC_ENTRY	__td_ref_mtx
	_TDSC_ENTRY	__td_ref_mbf
	_TDSC_ENTRY	__td_ref_por
	_TDSC_ENTRY	__td_ref_mpf
	_TDSC_ENTRY	__td_ref_mpl
	_TDSC_ENTRY	__td_ref_cyc
	_TDSC_ENTRY	__td_ref_alm
	_TDSC_ENTRY	__td_ref_ssy
	_TDSC_ENTRY	__td_ref_tsk
	_TDSC_ENTRY	__td_ref_tex
	_TDSC_ENTRY	__td_inf_tsk
	_TDSC_ENTRY	__td_get_reg
	_TDSC_ENTRY	__td_set_reg
	_TDSC_ENTRY	__td_ref_sys
	_TDSC_ENTRY	__td_get_tim
	_TDSC_ENTRY	__td_get_otm
	_TDSC_ENTRY	__td_rdy_que
	_TDSC_ENTRY	__td_sem_que
	_TDSC_ENTRY	__td_flg_que
	_TDSC_ENTRY	__td_mbx_que
	_TDSC_ENTRY	__td_mtx_que
	_TDSC_ENTRY	__td_smbf_que
	_TDSC_ENTRY	__td_rmbf_que
	_TDSC_ENTRY	__td_cal_que
	_TDSC_ENTRY	__td_acp_que
	_TDSC_ENTRY	__td_mpf_que
	_TDSC_ENTRY	__td_mpl_que
	_TDSC_ENTRY	__td_hok_svc
	_TDSC_ENTRY	__td_hok_dsp
	_TDSC_ENTRY	__td_hok_int
	_TDSC_ENTRY	__td_ref_dsname
	_TDSC_ENTRY	__td_set_dsname
	_TDSC_ENTRY	__td_ref_cyc_u
	_TDSC_ENTRY	__td_ref_alm_u
	_TDSC_ENTRY	__td_ref_tsk_u
	_TDSC_ENTRY	__td_inf_tsk_u
	_TDSC_ENTRY	__td_get_tim_u
	_TDSC_ENTRY	__td_get_otm_u
