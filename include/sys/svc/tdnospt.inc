;/*
; *----------------------------------------------------------------------
; *    UCT T-Kernel 2.0 DevKit tuned
; *
; *    Copyright (c) 2013 UC Technology. All Rights Reserved.
; *----------------------------------------------------------------------
; *
; *    Version:   2.00.00
; *    Released by UC Technology (http://www.uctec.com/) at 2013/06/14.
; *
; *----------------------------------------------------------------------
; */

;/*
; *	Not Supported T-Kernel system call
; *
; *	To select not support system call,
; *	remark "USE_FUNC_TD_XXX_YYY" macro in "config/tk_config_func.h"
; *
; */

	.aifdef	USE_FUNC_TD_LST_TSK
	.aelse
__td_lst_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_SEM
	.aelse
__td_lst_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_FLG
	.aelse
__td_lst_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_MBX
	.aelse
__td_lst_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_MTX
	.aelse
__td_lst_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_MBF
	.aelse
__td_lst_mbf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_POR
	.aelse
__td_lst_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_MPF
	.aelse
__td_lst_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_MPL
	.aelse
__td_lst_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_CYC
	.aelse
__td_lst_cyc:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_ALM
	.aelse
__td_lst_alm:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_LST_SSY
	.aelse
__td_lst_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_SEM
	.aelse
__td_ref_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_FLG
	.aelse
__td_ref_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_MBX
	.aelse
__td_ref_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_MTX
	.aelse
__td_ref_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_MBF
	.aelse
__td_ref_mbf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_POR
	.aelse
__td_ref_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_MPF
	.aelse
__td_ref_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_MPL
	.aelse
__td_ref_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_CYC
	.aelse
__td_ref_cyc:	.define	"_no_support"
__td_ref_cyc_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_ALM
	.aelse
__td_ref_alm:	.define	"_no_support"
__td_ref_alm_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_SSY
	.aelse
__td_ref_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_TSK
	.aelse
__td_ref_tsk:	.define	"_no_support"
__td_ref_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_TEX
	.aelse
__td_ref_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_INF_TSK
	.aelse
__td_inf_tsk:	.define	"_no_support"
__td_inf_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_GET_REG
	.aelse
__td_get_reg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_SET_REG
	.aelse
__td_set_reg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_SYS
	.aelse
__td_ref_sys:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_GET_TIM
	.aelse
__td_get_tim:	.define	"_no_support"
__td_get_tim_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_GET_OTM
	.aelse
__td_get_otm:	.define	"_no_support"
__td_get_otm_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_RDY_QUE
	.aelse
__td_rdy_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_SEM_QUE
	.aelse
__td_sem_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_FLG_QUE
	.aelse
__td_flg_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_MBX_QUE
	.aelse
__td_mbx_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_MTX_QUE
	.aelse
__td_mtx_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_SMBF_QUE
	.aelse
__td_smbf_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_RMBF_QUE
	.aelse
__td_rmbf_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_CAL_QUE
	.aelse
__td_cal_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_ACP_QUE
	.aelse
__td_acp_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_MPF_QUE
	.aelse
__td_mpf_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_MPL_QUE
	.aelse
__td_mpl_que:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_HOK_SVC
	.aelse
__td_hok_svc:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_HOK_DSP
	.aelse
__td_hok_dsp:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_HOK_INT
	.aelse
__td_hok_int:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_REF_DSNAME
	.aelse
__td_ref_dsname:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TD_SET_DSNAME
	.aelse
__td_set_dsname:	.define	"_no_support"
	.aendi

