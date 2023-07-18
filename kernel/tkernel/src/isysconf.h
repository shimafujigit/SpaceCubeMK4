/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the UCT T-Kernel 2.0
 *    Changed by UC Technology at 2013/01/22.
 *
 *    UCT T-Kernel 2.0 DevKit Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	isysconf.h (T-Kernel/OS)
 *	Default Value Definition for System Configuration File
 */

#ifndef _ISYSCONF_
#define _ISYSCONF_

/*
 * Definition of unused system call
 */
#if !defined(USE_FUNC_TK_CRE_SEM)
#define _tk_cre_sem	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_SEM)
#define _tk_del_sem	no_support
#endif
#if !defined(USE_FUNC_TK_SIG_SEM)
#define _tk_sig_sem	no_support
#endif
#if !defined(USE_FUNC_TK_WAI_SEM)
#define _tk_wai_sem	no_support
#endif
#if !defined(USE_FUNC_TK_REF_SEM)
#define _tk_ref_sem	no_support
#endif
#if !defined(USE_FUNC_TK_WAI_SEM)
#define _tk_wai_sem_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_SEM)
#define _td_lst_sem	no_support
#endif
#if !defined(USE_FUNC_TD_REF_SEM)
#define _td_ref_sem	no_support
#endif
#if !defined(USE_FUNC_TD_SEM_QUE)
#define _td_sem_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_FLG)
#define _tk_cre_flg	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_FLG)
#define _tk_del_flg	no_support
#endif
#if !defined(USE_FUNC_TK_SET_FLG)
#define _tk_set_flg	no_support
#endif
#if !defined(USE_FUNC_TK_CLR_FLG)
#define _tk_clr_flg	no_support
#endif
#if !defined(USE_FUNC_TK_WAI_FLG)
#define _tk_wai_flg	no_support
#endif
#if !defined(USE_FUNC_TK_REF_FLG)
#define _tk_ref_flg	no_support
#endif
#if !defined(USE_FUNC_TK_WAI_FLG)
#define _tk_wai_flg_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_FLG)
#define _td_lst_flg	no_support
#endif
#if !defined(USE_FUNC_TD_REF_FLG)
#define _td_ref_flg	no_support
#endif
#if !defined(USE_FUNC_TD_FLG_QUE)
#define _td_flg_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_MBX)
#define _tk_cre_mbx	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_MBX)
#define _tk_del_mbx	no_support
#endif
#if !defined(USE_FUNC_TK_SND_MBX)
#define _tk_snd_mbx	no_support
#endif
#if !defined(USE_FUNC_TK_RCV_MBX)
#define _tk_rcv_mbx	no_support
#endif
#if !defined(USE_FUNC_TK_REF_MBX)
#define _tk_ref_mbx	no_support
#endif
#if !defined(USE_FUNC_TK_RCV_MBX)
#define _tk_rcv_mbx_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBX)
#define _td_lst_mbx	no_support
#endif
#if !defined(USE_FUNC_TD_REF_MBX)
#define _td_ref_mbx	no_support
#endif
#if !defined(USE_FUNC_TD_MBX_QUE)
#define _td_mbx_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_MBF)
#define _tk_cre_mbf	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_MBF)
#define _tk_del_mbf	no_support
#endif
#if !defined(USE_FUNC_TK_SND_MBF)
#define _tk_snd_mbf	no_support
#endif
#if !defined(USE_FUNC_TK_RCV_MBF)
#define _tk_rcv_mbf	no_support
#endif
#if !defined(USE_FUNC_TK_REF_MBF)
#define _tk_ref_mbf	no_support
#endif
#if !defined(USE_FUNC_TK_SND_MBF)
#define _tk_snd_mbf_u	no_support
#endif
#if !defined(USE_FUNC_TK_RCV_MBF)
#define _tk_rcv_mbf_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBF)
#define _td_lst_mbf	no_support
#endif
#if !defined(USE_FUNC_TD_REF_MBF)
#define _td_ref_mbf	no_support
#endif
#if !defined(USE_FUNC_TD_SMBF_QUE)
#define _td_smbf_que	no_support
#endif
#if !defined(USE_FUNC_TD_RMBF_QUE)
#define _td_rmbf_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_POR)
#define _tk_cre_por	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_POR)
#define _tk_del_por	no_support
#endif
#if !defined(USE_FUNC_TK_CAL_POR)
#define _tk_cal_por	no_support
#endif
#if !defined(USE_FUNC_TK_ACP_POR)
#define _tk_acp_por	no_support
#endif
#if !defined(USE_FUNC_TK_FWD_POR)
#define _tk_fwd_por	no_support
#endif
#if !defined(USE_FUNC_TK_RPL_RDV)
#define _tk_rpl_rdv	no_support
#endif
#if !defined(USE_FUNC_TK_REF_POR)
#define _tk_ref_por	no_support
#endif
#if !defined(USE_FUNC_TK_CAL_POR)
#define _tk_cal_por_u	no_support
#endif
#if !defined(USE_FUNC_TK_ACP_POR)
#define _tk_acp_por_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_POR)
#define _td_lst_por	no_support
#endif
#if !defined(USE_FUNC_TD_REF_POR)
#define _td_ref_por	no_support
#endif
#if !defined(USE_FUNC_TD_ACP_QUE)
#define _td_acp_que	no_support
#endif
#if !defined(USE_FUNC_TD_CAL_QUE)
#define _td_cal_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_MTX)
#define _tk_cre_mtx	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_MTX)
#define _tk_del_mtx	no_support
#endif
#if !defined(USE_FUNC_TK_LOC_MTX)
#define _tk_loc_mtx	no_support
#endif
#if !defined(USE_FUNC_TK_UNL_MTX)
#define _tk_unl_mtx	no_support
#endif
#if !defined(USE_FUNC_TK_REF_MTX)
#define _tk_ref_mtx	no_support
#endif
#if !defined(USE_FUNC_TK_LOC_MTX)
#define _tk_loc_mtx_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MTX)
#define _td_lst_mtx	no_support
#endif
#if !defined(USE_FUNC_TD_REF_MTX)
#define _td_ref_mtx	no_support
#endif
#if !defined(USE_FUNC_TD_MTX_QUE)
#define _td_mtx_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_MPL)
#define _tk_cre_mpl	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_MPL)
#define _tk_del_mpl	no_support
#endif
#if !defined(USE_FUNC_TK_GET_MPL)
#define _tk_get_mpl	no_support
#endif
#if !defined(USE_FUNC_TK_REL_MPL)
#define _tk_rel_mpl	no_support
#endif
#if !defined(USE_FUNC_TK_REF_MPL)
#define _tk_ref_mpl	no_support
#endif
#if !defined(USE_FUNC_TK_GET_MPL)
#define _tk_get_mpl_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPL)
#define _td_lst_mpl	no_support
#endif
#if !defined(USE_FUNC_TD_REF_MPL)
#define _td_ref_mpl	no_support
#endif
#if !defined(USE_FUNC_TD_MPL_QUE)
#define _td_mpl_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_MPF)
#define _tk_cre_mpf	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_MPF)
#define _tk_del_mpf	no_support
#endif
#if !defined(USE_FUNC_TK_GET_MPF)
#define _tk_get_mpf	no_support
#endif
#if !defined(USE_FUNC_TK_REL_MPF)
#define _tk_rel_mpf	no_support
#endif
#if !defined(USE_FUNC_TK_REF_MPF)
#define _tk_ref_mpf	no_support
#endif
#if !defined(USE_FUNC_TK_GET_MPF)
#define _tk_get_mpf_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPF)
#define _td_lst_mpf	no_support
#endif
#if !defined(USE_FUNC_TD_REF_MPF)
#define _td_ref_mpf	no_support
#endif
#if !defined(USE_FUNC_TD_MPF_QUE)
#define _td_mpf_que	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_CYC)
#define _tk_cre_cyc	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_CYC)
#define _tk_del_cyc	no_support
#endif
#if !defined(USE_FUNC_TK_STA_CYC)
#define _tk_sta_cyc	no_support
#endif
#if !defined(USE_FUNC_TK_STP_CYC)
#define _tk_stp_cyc	no_support
#endif
#if !defined(USE_FUNC_TK_REF_CYC)
#define _tk_ref_cyc	no_support
#endif
#if !defined(USE_FUNC_TK_CRE_CYC)
#define _tk_cre_cyc_u	no_support
#endif
#if !defined(USE_FUNC_TK_REF_CYC)
#define _tk_ref_cyc_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_CYC)
#define _td_lst_cyc	no_support
#endif
#if !defined(USE_FUNC_TD_REF_CYC)
#define _td_ref_cyc	no_support
#endif
#if !defined(USE_FUNC_TD_REF_CYC)
#define _td_ref_cyc_u	no_support
#endif


#if !defined(USE_FUNC_TK_CRE_ALM)
#define _tk_cre_alm	no_support
#endif
#if !defined(USE_FUNC_TK_DEL_ALM)
#define _tk_del_alm	no_support
#endif
#if !defined(USE_FUNC_TK_STA_ALM)
#define _tk_sta_alm	no_support
#endif
#if !defined(USE_FUNC_TK_STP_ALM)
#define _tk_stp_alm	no_support
#endif
#if !defined(USE_FUNC_TK_REF_ALM)
#define _tk_ref_alm	no_support
#endif
#if !defined(USE_FUNC_TK_STA_ALM)
#define _tk_sta_alm_u	no_support
#endif
#if !defined(USE_FUNC_TK_REF_ALM)
#define _tk_ref_alm_u	no_support
#endif

#if !defined(USE_FUNC_TD_LST_ALM)
#define _td_lst_alm	no_support
#endif
#if !defined(USE_FUNC_TD_REF_ALM)
#define _td_ref_alm	no_support
#endif
#if !defined(USE_FUNC_TD_REF_ALM)
#define _td_ref_alm_u	no_support
#endif

#endif /* _ISYSCONF_ */
