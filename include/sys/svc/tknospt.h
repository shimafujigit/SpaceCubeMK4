/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   2.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/06/14.
 *
 *----------------------------------------------------------------------
 */

/*
 *	Not Supported T-Kernel system call
 *
 *	To select not support system call,
 *	remark "USE_FUNC_TK_XXX_YYY" macro in "config/tk_config_func.h"
 *
 */

#ifndef	__TKNOSPTTBL_H__
#define	__TKNOSPTTBL_H__

#if !defined(USE_FUNC_TK_CRE_TSK)
#define	_tk_cre_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_TSK)
#define	_tk_del_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_STA_TSK)
#define	_tk_sta_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_EXT_TSK)
#define	_tk_ext_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_EXD_TSK)
#define	_tk_exd_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_TER_TSK)
#define	_tk_ter_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_DIS_DSP)
#define	_tk_dis_dsp	no_support
#endif

#if !defined(USE_FUNC_TK_ENA_DSP)
#define	_tk_ena_dsp	no_support
#endif

#if !defined(USE_FUNC_TK_CHG_PRI)
#define	_tk_chg_pri	no_support
#endif

#if !defined(USE_FUNC_TK_CHG_SLT)
#define	_tk_chg_slt	no_support
#define	_tk_chg_slt_u	no_support
#endif

#if !defined(USE_FUNC_TK_ROT_RDQ)
#define	_tk_rot_rdq	no_support
#endif

#if !defined(USE_FUNC_TK_REL_WAI)
#define	_tk_rel_wai	no_support
#endif

#if !defined(USE_FUNC_TK_GET_TID)
#define	_tk_get_tid	no_support
#endif

#if !defined(USE_FUNC_TK_GET_TSP)
#define	_tk_get_tsp	no_support
#endif

#if !defined(USE_FUNC_TK_SET_TSP)
#define	_tk_set_tsp	no_support
#endif

#if !defined(USE_FUNC_TK_GET_RID)
#define	_tk_get_rid	no_support
#endif

#if !defined(USE_FUNC_TK_SET_RID)
#define	_tk_set_rid	no_support
#endif

#if !defined(USE_FUNC_TK_GET_REG)
#define	_tk_get_reg	no_support
#endif

#if !defined(USE_FUNC_TK_SET_REG)
#define	_tk_set_reg	no_support
#endif

#if !defined(USE_FUNC_TK_GET_CPR)
#define	_tk_get_cpr	no_support
#endif

#if !defined(USE_FUNC_TK_SET_CPR)
#define	_tk_set_cpr	no_support
#endif

#if !defined(USE_FUNC_TK_INF_TSK)
#define	_tk_inf_tsk	no_support
#define	_tk_inf_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TK_REF_TSK)
#define	_tk_ref_tsk	no_support
#define	_tk_ref_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TK_DEF_TEX)
#define	_tk_def_tex	no_support
#endif

#if !defined(USE_FUNC_TK_ENA_TEX)
#define	_tk_ena_tex	no_support
#endif

#if !defined(USE_FUNC_TK_DIS_TEX)
#define	_tk_dis_tex	no_support
#endif

#if !defined(USE_FUNC_TK_RAS_TEX)
#define	_tk_ras_tex	no_support
#endif

#if !defined(USE_FUNC_TK_END_TEX)
#define	_tk_end_tex	no_support
#endif

#if !defined(USE_FUNC_TK_REF_TEX)
#define	_tk_ref_tex	no_support
#endif

#if !defined(USE_FUNC_TK_SUS_TSK)
#define	_tk_sus_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_RSM_TSK)
#define	_tk_rsm_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_FRSM_TSK)
#define	_tk_frsm_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_SLP_TSK)
#define	_tk_slp_tsk	no_support
#define	_tk_slp_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TK_WUP_TSK)
#define	_tk_wup_tsk	no_support
#endif

#if !defined(USE_FUNC_TK_CAN_WUP)
#define	_tk_can_wup	no_support
#endif

#if !defined(USE_FUNC_TK_SIG_TEV)
#define	_tk_sig_tev	no_support
#endif

#if !defined(USE_FUNC_TK_WAI_TEV)
#define	_tk_wai_tev	no_support
#define	_tk_wai_tev_u	no_support
#endif

#if !defined(USE_FUNC_TK_DIS_WAI)
#define	_tk_dis_wai	no_support
#endif

#if !defined(USE_FUNC_TK_ENA_WAI)
#define	_tk_ena_wai	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_SEM)
#define	_tk_cre_sem	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_SEM)
#define	_tk_del_sem	no_support
#endif

#if !defined(USE_FUNC_TK_SIG_SEM)
#define	_tk_sig_sem	no_support
#endif

#if !defined(USE_FUNC_TK_WAI_SEM)
#define	_tk_wai_sem	no_support
#endif

#if !defined(USE_FUNC_TK_REF_SEM)
#define	_tk_ref_sem	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_MTX)
#define	_tk_cre_mtx	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_MTX)
#define	_tk_del_mtx	no_support
#endif

#if !defined(USE_FUNC_TK_LOC_MTX)
#define	_tk_loc_mtx	no_support
#define	_tk_loc_mtx_u	no_support
#endif

#if !defined(USE_FUNC_TK_UNL_MTX)
#define	_tk_unl_mtx	no_support
#endif

#if !defined(USE_FUNC_TK_REF_MTX)
#define	_tk_ref_mtx	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_FLG)
#define	_tk_cre_flg	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_FLG)
#define	_tk_del_flg	no_support
#endif

#if !defined(USE_FUNC_TK_SET_FLG)
#define	_tk_set_flg	no_support
#endif

#if !defined(USE_FUNC_TK_CLR_FLG)
#define	_tk_clr_flg	no_support
#endif

#if !defined(USE_FUNC_TK_WAI_FLG)
#define	_tk_wai_flg	no_support
#endif

#if !defined(USE_FUNC_TK_REF_FLG)
#define	_tk_ref_flg	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_MBX)
#define	_tk_cre_mbx	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_MBX)
#define	_tk_del_mbx	no_support
#endif

#if !defined(USE_FUNC_TK_SND_MBX)
#define	_tk_snd_mbx	no_support
#endif

#if !defined(USE_FUNC_TK_RCV_MBX)
#define	_tk_rcv_mbx	no_support
#define	_tk_rcv_mbx_u	no_support
#endif

#if !defined(USE_FUNC_TK_REF_MBX)
#define	_tk_ref_mbx	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_MBF)
#define	_tk_cre_mbf	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_MBF)
#define	_tk_del_mbf	no_support
#endif

#if !defined(USE_FUNC_TK_SND_MBF)
#define	_tk_snd_mbf	no_support
#define	_tk_snd_mbf_u	no_support
#endif

#if !defined(USE_FUNC_TK_RCV_MBF)
#define	_tk_rcv_mbf	no_support
#define	_tk_rcv_mbf_u	no_support
#endif

#if !defined(USE_FUNC_TK_REF_MBF)
#define	_tk_ref_mbf	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_POR)
#define	_tk_cre_por	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_POR)
#define	_tk_del_por	no_support
#endif

#if !defined(USE_FUNC_TK_CAL_POR)
#define	_tk_cal_por	no_support
#define	_tk_cal_por_u	no_support
#endif

#if !defined(USE_FUNC_TK_ACP_POR)
#define	_tk_acp_por	no_support
#define	_tk_acp_por_u	no_support
#endif

#if !defined(USE_FUNC_TK_FWD_POR)
#define	_tk_fwd_por	no_support
#endif

#if !defined(USE_FUNC_TK_RPL_RDV)
#define	_tk_rpl_rdv	no_support
#endif

#if !defined(USE_FUNC_TK_REF_POR)
#define	_tk_ref_por	no_support
#endif

#if !defined(USE_FUNC_TK_DEF_INT)
#define	_tk_def_int	no_support
#endif

#if !defined(USE_FUNC_TK_RET_INT)
#define	_tk_ret_int	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_MPL)
#define	_tk_cre_mpl	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_MPL)
#define	_tk_del_mpl	no_support
#endif

#if !defined(USE_FUNC_TK_GET_MPL)
#define	_tk_get_mpl	no_support
#define	_tk_get_mpl_u	no_support
#endif

#if !defined(USE_FUNC_TK_REL_MPL)
#define	_tk_rel_mpl	no_support
#endif

#if !defined(USE_FUNC_TK_REF_MPL)
#define	_tk_ref_mpl	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_MPF)
#define	_tk_cre_mpf	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_MPF)
#define	_tk_del_mpf	no_support
#endif

#if !defined(USE_FUNC_TK_GET_MPF)
#define	_tk_get_mpf	no_support
#define	_tk_get_mpf_u	no_support
#endif

#if !defined(USE_FUNC_TK_REL_MPF)
#define	_tk_rel_mpf	no_support
#endif

#if !defined(USE_FUNC_TK_REF_MPF)
#define	_tk_ref_mpf	no_support
#endif

#if !defined(USE_FUNC_TK_SET_TIM)
#define	_tk_set_tim	no_support
#define	_tk_set_tim_u	no_support
#endif

#if !defined(USE_FUNC_TK_GET_TIM)
#define	_tk_get_tim	no_support
#define	_tk_get_tim_u	no_support
#endif

#if !defined(USE_FUNC_TK_GET_OTM)
#define	_tk_get_otm	no_support
#define	_tk_get_otm_u	no_support
#endif

#if !defined(USE_FUNC_TK_DLY_TSK)
#define	_tk_dly_tsk	no_support
#define	_tk_dly_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_CYC)
#define	_tk_cre_cyc	no_support
#define	_tk_cre_cyc_u	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_CYC)
#define	_tk_del_cyc	no_support
#endif

#if !defined(USE_FUNC_TK_STA_CYC)
#define	_tk_sta_cyc	no_support
#endif

#if !defined(USE_FUNC_TK_STP_CYC)
#define	_tk_stp_cyc	no_support
#endif

#if !defined(USE_FUNC_TK_REF_CYC)
#define	_tk_ref_cyc	no_support
#define	_tk_ref_cyc_u	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_ALM)
#define	_tk_cre_alm	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_ALM)
#define	_tk_del_alm	no_support
#endif

#if !defined(USE_FUNC_TK_STA_ALM)
#define	_tk_sta_alm	no_support
#define	_tk_sta_alm_u	no_support
#endif

#if !defined(USE_FUNC_TK_STP_ALM)
#define	_tk_stp_alm	no_support
#endif

#if !defined(USE_FUNC_TK_REF_ALM)
#define	_tk_ref_alm	no_support
#define	_tk_ref_alm_u	no_support
#endif

#if !defined(USE_FUNC_TK_REF_VER)
#define	_tk_ref_ver	no_support
#endif

#if !defined(USE_FUNC_TK_REF_SYS)
#define	_tk_ref_sys	no_support
#endif

#if !defined(USE_FUNC_TK_DEF_SSY)
#define	_tk_def_ssy	no_support
#endif

#if !defined(USE_FUNC_TK_STA_SSY)
#define	_tk_sta_ssy	no_support
#endif

#if !defined(USE_FUNC_TK_CLN_SSY)
#define	_tk_cln_ssy	no_support
#endif

#if !defined(USE_FUNC_TK_EVT_SSY)
#define	_tk_evt_ssy	no_support
#endif

#if !defined(USE_FUNC_TK_REF_SSY)
#define	_tk_ref_ssy	no_support
#endif

#if !defined(USE_FUNC_TK_CRE_RES)
#define	_tk_cre_res	no_support
#endif

#if !defined(USE_FUNC_TK_DEL_RES)
#define	_tk_del_res	no_support
#endif

#if !defined(USE_FUNC_TK_GET_RES)
#define	_tk_get_res	no_support
#endif

#if !defined(USE_FUNC_TK_SET_POW)
#define	_tk_set_pow	no_support
#endif

#else	/*__TKNOSPTTBL_H__*/

#undef	_tk_cre_tsk
#undef	_tk_del_tsk
#undef	_tk_sta_tsk
#undef	_tk_ext_tsk
#undef	_tk_exd_tsk
#undef	_tk_ter_tsk
#undef	_tk_dis_dsp
#undef	_tk_ena_dsp
#undef	_tk_chg_pri
#undef	_tk_chg_slt
#undef	_tk_rot_rdq
#undef	_tk_rel_wai
#undef	_tk_get_tid
#undef	_tk_get_tsp
#undef	_tk_set_tsp
#undef	_tk_get_rid
#undef	_tk_set_rid
#undef	_tk_get_reg
#undef	_tk_set_reg
#undef	_tk_get_cpr
#undef	_tk_set_cpr
#undef	_tk_inf_tsk
#undef	_tk_ref_tsk
#undef	_tk_def_tex
#undef	_tk_ena_tex
#undef	_tk_dis_tex
#undef	_tk_ras_tex
#undef	_tk_end_tex
#undef	_tk_ref_tex
#undef	_tk_sus_tsk
#undef	_tk_rsm_tsk
#undef	_tk_frsm_tsk
#undef	_tk_slp_tsk
#undef	_tk_wup_tsk
#undef	_tk_can_wup
#undef	_tk_sig_tev
#undef	_tk_wai_tev
#undef	_tk_dis_wai
#undef	_tk_ena_wai
#undef	_tk_cre_sem
#undef	_tk_del_sem
#undef	_tk_sig_sem
#undef	_tk_wai_sem
#undef	_tk_ref_sem
#undef	_tk_cre_mtx
#undef	_tk_del_mtx
#undef	_tk_loc_mtx
#undef	_tk_unl_mtx
#undef	_tk_ref_mtx
#undef	_tk_cre_flg
#undef	_tk_del_flg
#undef	_tk_set_flg
#undef	_tk_clr_flg
#undef	_tk_wai_flg
#undef	_tk_ref_flg
#undef	_tk_cre_mbx
#undef	_tk_del_mbx
#undef	_tk_snd_mbx
#undef	_tk_rcv_mbx
#undef	_tk_ref_mbx
#undef	_tk_cre_mbf
#undef	_tk_del_mbf
#undef	_tk_snd_mbf
#undef	_tk_rcv_mbf
#undef	_tk_ref_mbf
#undef	_tk_cre_por
#undef	_tk_del_por
#undef	_tk_cal_por
#undef	_tk_acp_por
#undef	_tk_fwd_por
#undef	_tk_rpl_rdv
#undef	_tk_ref_por
#undef	_tk_def_int
#undef	_tk_ret_int
#undef	_tk_cre_mpl
#undef	_tk_del_mpl
#undef	_tk_get_mpl
#undef	_tk_rel_mpl
#undef	_tk_ref_mpl
#undef	_tk_cre_mpf
#undef	_tk_del_mpf
#undef	_tk_get_mpf
#undef	_tk_rel_mpf
#undef	_tk_ref_mpf
#undef	_tk_set_tim
#undef	_tk_get_tim
#undef	_tk_get_otm
#undef	_tk_dly_tsk
#undef	_tk_cre_cyc
#undef	_tk_del_cyc
#undef	_tk_sta_cyc
#undef	_tk_stp_cyc
#undef	_tk_ref_cyc
#undef	_tk_cre_alm
#undef	_tk_del_alm
#undef	_tk_sta_alm
#undef	_tk_stp_alm
#undef	_tk_ref_alm
#undef	_tk_ref_ver
#undef	_tk_ref_sys
#undef	_tk_def_ssy
#undef	_tk_sta_ssy
#undef	_tk_cln_ssy
#undef	_tk_evt_ssy
#undef	_tk_ref_ssy
#undef	_tk_cre_res
#undef	_tk_del_res
#undef	_tk_get_res
#undef	_tk_set_pow
#undef	_tk_chg_slt_u
#undef	_tk_inf_tsk_u
#undef	_tk_ref_tsk_u
#undef	_tk_slp_tsk_u
#undef	_tk_wai_tev_u
#undef	_tk_dly_tsk_u
#undef	_tk_wai_sem_u
#undef	_tk_wai_flg_u
#undef	_tk_rcv_mbx_u
#undef	_tk_loc_mtx_u
#undef	_tk_snd_mbf_u
#undef	_tk_rcv_mbf_u
#undef	_tk_cal_por_u
#undef	_tk_acp_por_u
#undef	_tk_get_mpl_u
#undef	_tk_get_mpf_u
#undef	_tk_set_tim_u
#undef	_tk_get_tim_u
#undef	_tk_get_otm_u
#undef	_tk_cre_cyc_u
#undef	_tk_ref_cyc_u
#undef	_tk_sta_alm_u
#undef	_tk_ref_alm_u

#endif	/*__TKNOSPTTBL_H__*/
