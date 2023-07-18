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
; *	remark "USE_FUNC_TK_XXX_YYY" macro in "config/tk_config_func.h"
; *
; */

	.aifdef	USE_FUNC_TK_CRE_TSK
	.aelse
__tk_cre_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_TSK
	.aelse
__tk_del_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STA_TSK
	.aelse
__tk_sta_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_EXT_TSK
	.aelse
__tk_ext_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_EXD_TSK
	.aelse
__tk_exd_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_TER_TSK
	.aelse
__tk_ter_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DIS_DSP
	.aelse
__tk_dis_dsp:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_ENA_DSP
	.aelse
__tk_ena_dsp:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CHG_PRI
	.aelse
__tk_chg_pri:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CHG_SLT
	.aelse
__tk_chg_slt:	.define	"_no_support"
__tk_chg_slt_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_ROT_RDQ
	.aelse
__tk_rot_rdq:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REL_WAI
	.aelse
__tk_rel_wai:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_TID
	.aelse
__tk_get_tid:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_TSP
	.aelse
__tk_get_tsp:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_TSP
	.aelse
__tk_set_tsp:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_RID
	.aelse
__tk_get_rid:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_RID
	.aelse
__tk_set_rid:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_REG
	.aelse
__tk_get_reg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_REG
	.aelse
__tk_set_reg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_CPR
	.aelse
__tk_get_cpr:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_CPR
	.aelse
__tk_set_cpr:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_INF_TSK
	.aelse
__tk_inf_tsk:	.define	"_no_support"
__tk_inf_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_TSK
	.aelse
__tk_ref_tsk:	.define	"_no_support"
__tk_ref_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEF_TEX
	.aelse
__tk_def_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_ENA_TEX
	.aelse
__tk_ena_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DIS_TEX
	.aelse
__tk_dis_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RAS_TEX
	.aelse
__tk_ras_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_END_TEX
	.aelse
__tk_end_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_TEX
	.aelse
__tk_ref_tex:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SUS_TSK
	.aelse
__tk_sus_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RSM_TSK
	.aelse
__tk_rsm_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_FRSM_TSK
	.aelse
__tk_frsm_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SLP_TSK
	.aelse
__tk_slp_tsk:	.define	"_no_support"
__tk_slp_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_WUP_TSK
	.aelse
__tk_wup_tsk:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CAN_WUP
	.aelse
__tk_can_wup:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SIG_TEV
	.aelse
__tk_sig_tev:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_WAI_TEV
	.aelse
__tk_wai_tev:	.define	"_no_support"
__tk_wai_tev_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DIS_WAI
	.aelse
__tk_dis_wai:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_ENA_WAI
	.aelse
__tk_ena_wai:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_SEM
	.aelse
__tk_cre_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_SEM
	.aelse
__tk_del_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SIG_SEM
	.aelse
__tk_sig_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_WAI_SEM
	.aelse
__tk_wai_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_SEM
	.aelse
__tk_ref_sem:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_MTX
	.aelse
__tk_cre_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_MTX
	.aelse
__tk_del_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_LOC_MTX
	.aelse
__tk_loc_mtx:	.define	"_no_support"
__tk_loc_mtx_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_UNL_MTX
	.aelse
__tk_unl_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_MTX
	.aelse
__tk_ref_mtx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_FLG
	.aelse
__tk_cre_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_FLG
	.aelse
__tk_del_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_FLG
	.aelse
__tk_set_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CLR_FLG
	.aelse
__tk_clr_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_WAI_FLG
	.aelse
__tk_wai_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_FLG
	.aelse
__tk_ref_flg:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_MBX
	.aelse
__tk_cre_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_MBX
	.aelse
__tk_del_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SND_MBX
	.aelse
__tk_snd_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RCV_MBX
	.aelse
__tk_rcv_mbx:	.define	"_no_support"
__tk_rcv_mbx_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_MBX
	.aelse
__tk_ref_mbx:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_MBF
	.aelse
__tk_cre_mbf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_MBF
	.aelse
__tk_del_mbf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SND_MBF
	.aelse
__tk_snd_mbf:	.define	"_no_support"
__tk_snd_mbf_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RCV_MBF
	.aelse
__tk_rcv_mbf:	.define	"_no_support"
__tk_rcv_mbf_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_MBF
	.aelse
__tk_ref_mbf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_POR
	.aelse
__tk_cre_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_POR
	.aelse
__tk_del_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CAL_POR
	.aelse
__tk_cal_por:	.define	"_no_support"
__tk_cal_por_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_ACP_POR
	.aelse
__tk_acp_por:	.define	"_no_support"
__tk_acp_por_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_FWD_POR
	.aelse
__tk_fwd_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RPL_RDV
	.aelse
__tk_rpl_rdv:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_POR
	.aelse
__tk_ref_por:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEF_INT
	.aelse
__tk_def_int:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_RET_INT
	.aelse
__tk_ret_int:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_MPL
	.aelse
__tk_cre_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_MPL
	.aelse
__tk_del_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_MPL
	.aelse
__tk_get_mpl:	.define	"_no_support"
__tk_get_mpl_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REL_MPL
	.aelse
__tk_rel_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_MPL
	.aelse
__tk_ref_mpl:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_MPF
	.aelse
__tk_cre_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_MPF
	.aelse
__tk_del_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_MPF
	.aelse
__tk_get_mpf:	.define	"_no_support"
__tk_get_mpf_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REL_MPF
	.aelse
__tk_rel_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_MPF
	.aelse
__tk_ref_mpf:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_TIM
	.aelse
__tk_set_tim:	.define	"_no_support"
__tk_set_tim_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_TIM
	.aelse
__tk_get_tim:	.define	"_no_support"
__tk_get_tim_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_OTM
	.aelse
__tk_get_otm:	.define	"_no_support"
__tk_get_otm_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DLY_TSK
	.aelse
__tk_dly_tsk:	.define	"_no_support"
__tk_dly_tsk_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_CYC
	.aelse
__tk_cre_cyc:	.define	"_no_support"
__tk_cre_cyc_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_CYC
	.aelse
__tk_del_cyc:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STA_CYC
	.aelse
__tk_sta_cyc:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STP_CYC
	.aelse
__tk_stp_cyc:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_CYC
	.aelse
__tk_ref_cyc:	.define	"_no_support"
__tk_ref_cyc_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_ALM
	.aelse
__tk_cre_alm:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_ALM
	.aelse
__tk_del_alm:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STA_ALM
	.aelse
__tk_sta_alm:	.define	"_no_support"
__tk_sta_alm_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STP_ALM
	.aelse
__tk_stp_alm:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_ALM
	.aelse
__tk_ref_alm:	.define	"_no_support"
__tk_ref_alm_u:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_VER
	.aelse
__tk_ref_ver:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_SYS
	.aelse
__tk_ref_sys:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEF_SSY
	.aelse
__tk_def_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_STA_SSY
	.aelse
__tk_sta_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CLN_SSY
	.aelse
__tk_cln_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_EVT_SSY
	.aelse
__tk_evt_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_REF_SSY
	.aelse
__tk_ref_ssy:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_CRE_RES
	.aelse
__tk_cre_res:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_DEL_RES
	.aelse
__tk_del_res:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_GET_RES
	.aelse
__tk_get_res:	.define	"_no_support"
	.aendi

	.aifdef	USE_FUNC_TK_SET_POW
	.aelse
__tk_set_pow:	.define	"_no_support"
	.aendi

