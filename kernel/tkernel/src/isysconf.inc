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
; *	isysconf.inc (T-Kernel/OS)
; *	Default Value Definition for System Configuration File
; */

	.aifdef _ISYSCONF_
	.aelse
_ISYSCONF_: .define " "

;/*
; * Definition of unused system call
; */
	.aifdef NUM_SEMID
	.aelse
_tk_cre_sem:	.define		"no_support"
_tk_del_sem:	.define		"no_support"
_tk_sig_sem:	.define		"no_support"
_tk_wai_sem:	.define		"no_support"
_tk_ref_sem:	.define		"no_support"
_td_lst_sem:	.define		"no_support"
_td_ref_sem:	.define		"no_support"
_td_sem_que:	.define		"no_support"
	.aendi

	.aifdef NUM_FLGID
	.aelse
_tk_cre_flg:	.define		"no_support"
_tk_del_flg:	.define		"no_support"
_tk_set_flg:	.define		"no_support"
_tk_clr_flg:	.define		"no_support"
_tk_wai_flg:	.define		"no_support"
_tk_ref_flg:	.define		"no_support"
_td_lst_flg:	.define		"no_support"
_td_ref_flg:	.define		"no_support"
_td_flg_que:	.define		"no_support"
	.aendi

	.aifdef NUM_MBXID
	.aelse
_tk_cre_mbx:	.define		"no_support"
_tk_del_mbx:	.define		"no_support"
_tk_snd_mbx:	.define		"no_support"
_tk_rcv_mbx:	.define		"no_support"
_tk_ref_mbx:	.define		"no_support"
_td_lst_mbx:	.define		"no_support"
_td_ref_mbx:	.define		"no_support"
_td_mbx_que:	.define		"no_support"
	.aendi

	.aifdef NUM_MBFID
	.aelse
_tk_cre_mbf:	.define		"no_support"
_tk_del_mbf:	.define		"no_support"
_tk_snd_mbf:	.define		"no_support"
_tk_rcv_mbf:	.define		"no_support"
_tk_ref_mbf:	.define		"no_support"
_td_lst_mbf:	.define		"no_support"
_td_ref_mbf:	.define		"no_support"
_td_smbf_que:	.define		"no_support"
_td_rmbf_que:	.define		"no_support"
	.aendi

	.aifdef NUM_PORID
	.aelse
_tk_cre_por:	.define		"no_support"
_tk_del_por:	.define		"no_support"
_tk_cal_por:	.define		"no_support"
_tk_acp_por:	.define		"no_support"
_tk_fwd_por:	.define		"no_support"
_tk_rpl_rdv:	.define		"no_support"
_tk_ref_por:	.define		"no_support"
_td_lst_por:	.define		"no_support"
_td_ref_por:	.define		"no_support"
_td_acp_que:	.define		"no_support"
_td_cal_que:	.define		"no_support"
	.aendi

	.aifdef NUM_MTXID
	.aelse
_tk_cre_mtx:	.define		"no_support"
_tk_del_mtx:	.define		"no_support"
_tk_loc_mtx:	.define		"no_support"
_tk_unl_mtx:	.define		"no_support"
_tk_ref_mtx:	.define		"no_support"
_td_lst_mtx:	.define		"no_support"
_td_ref_mtx:	.define		"no_support"
_td_mtx_que:	.define		"no_support"
	.aendi

	.aifdef NUM_MPLID
	.aelse
_tk_cre_mpl:	.define		"no_support"
_tk_del_mpl:	.define		"no_support"
_tk_get_mpl:	.define		"no_support"
_tk_rel_mpl:	.define		"no_support"
_tk_ref_mpl:	.define		"no_support"
_td_lst_mpl:	.define		"no_support"
_td_ref_mpl:	.define		"no_support"
_td_mpl_que:	.define		"no_support"
	.aendi

	.aifdef NUM_MPFID
	.aelse
_tk_cre_mpf:	.define		"no_support"
_tk_del_mpf:	.define		"no_support"
_tk_get_mpf:	.define		"no_support"
_tk_rel_mpf:	.define		"no_support"
_tk_ref_mpf:	.define		"no_support"
_td_lst_mpf:	.define		"no_support"
_td_ref_mpf:	.define		"no_support"
_td_mpf_que:	.define		"no_support"
	.aendi

	.aifdef NUM_CYCID
	.aelse
_tk_cre_cyc:	.define		"no_support"
_tk_del_cyc:	.define		"no_support"
_tk_sta_cyc:	.define		"no_support"
_tk_stp_cyc:	.define		"no_support"
_tk_ref_cyc:	.define		"no_support"
_td_lst_cyc:	.define		"no_support"
_td_ref_cyc:	.define		"no_support"
	.aendi

	.aifdef NUM_ALMID
	.aelse
_tk_cre_alm:	.define		"no_support"
_tk_del_alm:	.define		"no_support"
_tk_sta_alm:	.define		"no_support"
_tk_stp_alm:	.define		"no_support"
_tk_ref_alm:	.define		"no_support"
_td_lst_alm:	.define		"no_support"
_td_ref_alm:	.define		"no_support"
	.aendi

	.aendi ;/* _ISYSCONF_ */
