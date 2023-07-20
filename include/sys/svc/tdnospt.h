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
 *	remark "USE_FUNC_TD_XXX_YYY" macro in "config/tk_config_func.h"
 *
 */

#ifndef	__TDNOSPT_H__
#define	__TDNOSPT_H__

#if !defined(USE_FUNC_TD_LST_TSK)
#define	td_lst_tsk	no_support
#endif

#if !defined(USE_FUNC_TD_LST_SEM)
#define	td_lst_sem	no_support
#endif

#if !defined(USE_FUNC_TD_LST_FLG)
#define	td_lst_flg	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBX)
#define	td_lst_mbx	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MTX)
#define	td_lst_mtx	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBF)
#define	td_lst_mbf	no_support
#endif

#if !defined(USE_FUNC_TD_LST_POR)
#define	td_lst_por	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPF)
#define	td_lst_mpf	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPL)
#define	td_lst_mpl	no_support
#endif

#if !defined(USE_FUNC_TD_LST_CYC)
#define	td_lst_cyc	no_support
#endif

#if !defined(USE_FUNC_TD_LST_ALM)
#define	td_lst_alm	no_support
#endif

#if !defined(USE_FUNC_TD_LST_SSY)
#define	td_lst_ssy	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SEM)
#define	td_ref_sem	no_support
#endif

#if !defined(USE_FUNC_TD_REF_FLG)
#define	td_ref_flg	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MBX)
#define	td_ref_mbx	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MTX)
#define	td_ref_mtx	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MBF)
#define	td_ref_mbf	no_support
#endif

#if !defined(USE_FUNC_TD_REF_POR)
#define	td_ref_por	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MPF)
#define	td_ref_mpf	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MPL)
#define	td_ref_mpl	no_support
#endif

#if !defined(USE_FUNC_TD_REF_CYC)
#define	td_ref_cyc	no_support
#define	td_ref_cyc_u	no_support
#endif

#if !defined(USE_FUNC_TD_REF_ALM)
#define	td_ref_alm	no_support
#define	td_ref_alm_u	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SSY)
#define	td_ref_ssy	no_support
#endif

#if !defined(USE_FUNC_TD_REF_TSK)
#define	td_ref_tsk	no_support
#endif

#if !defined(USE_FUNC_TD_REF_TEX)
#define	td_ref_tex	no_support
#define	td_ref_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TD_INF_TSK)
#define	td_inf_tsk	no_support
#define	td_inf_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TD_GET_REG)
#define	td_get_reg	no_support
#endif

#if !defined(USE_FUNC_TD_SET_REG)
#define	td_set_reg	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SYS)
#define	td_ref_sys	no_support
#endif

#if !defined(USE_FUNC_TD_GET_TIM)
#define	td_get_tim	no_support
#define	td_get_tim_u	no_support
#endif

#if !defined(USE_FUNC_TD_GET_OTM)
#define	td_get_otm	no_support
#define	td_get_otm_u	no_support
#endif

#if !defined(USE_FUNC_TD_RDY_QUE)
#define	td_rdy_que	no_support
#endif

#if !defined(USE_FUNC_TD_SEM_QUE)
#define	td_sem_que	no_support
#endif

#if !defined(USE_FUNC_TD_FLG_QUE)
#define	td_flg_que	no_support
#endif

#if !defined(USE_FUNC_TD_MBX_QUE)
#define	td_mbx_que	no_support
#endif

#if !defined(USE_FUNC_TD_MTX_QUE)
#define	td_mtx_que	no_support
#endif

#if !defined(USE_FUNC_TD_SMBF_QUE)
#define	td_smbf_que	no_support
#endif

#if !defined(USE_FUNC_TD_RMBF_QUE)
#define	td_rmbf_que	no_support
#endif

#if !defined(USE_FUNC_TD_CAL_QUE)
#define	td_cal_que	no_support
#endif

#if !defined(USE_FUNC_TD_ACP_QUE)
#define	td_acp_que	no_support
#endif

#if !defined(USE_FUNC_TD_MPF_QUE)
#define	td_mpf_que	no_support
#endif

#if !defined(USE_FUNC_TD_MPL_QUE)
#define	td_mpl_que	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_SVC)
#define	td_hok_svc	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_DSP)
#define	td_hok_dsp	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_INT)
#define	td_hok_int	no_support
#endif

#if !defined(USE_FUNC_TD_REF_DSNAME)
#define	td_ref_dsname	no_support
#endif

#if !defined(USE_FUNC_TD_SET_DSNAME)
#define	td_set_dsname	no_support
#endif

#else	/*__TDNOSPT_H__*/

#undef	td_lst_tsk
#undef	td_lst_sem
#undef	td_lst_flg
#undef	td_lst_mbx
#undef	td_lst_mtx
#undef	td_lst_mbf
#undef	td_lst_por
#undef	td_lst_mpf
#undef	td_lst_mpl
#undef	td_lst_cyc
#undef	td_lst_alm
#undef	td_lst_ssy
#undef	td_ref_sem
#undef	td_ref_flg
#undef	td_ref_mbx
#undef	td_ref_mtx
#undef	td_ref_mbf
#undef	td_ref_por
#undef	td_ref_mpf
#undef	td_ref_mpl
#undef	td_ref_cyc
#undef	td_ref_alm
#undef	td_ref_ssy
#undef	td_ref_tsk
#undef	td_ref_tex
#undef	td_inf_tsk
#undef	td_get_reg
#undef	td_set_reg
#undef	td_ref_sys
#undef	td_get_tim
#undef	td_get_otm
#undef	td_rdy_que
#undef	td_sem_que
#undef	td_flg_que
#undef	td_mbx_que
#undef	td_mtx_que
#undef	td_smbf_que
#undef	td_rmbf_que
#undef	td_cal_que
#undef	td_acp_que
#undef	td_mpf_que
#undef	td_mpl_que
#undef	td_hok_svc
#undef	td_hok_dsp
#undef	td_hok_int
#undef	td_ref_dsname
#undef	td_set_dsname
#undef	td_ref_cyc_u
#undef	td_ref_alm_u
#undef	td_ref_tsk_u
#undef	td_inf_tsk_u
#undef	td_get_tim_u
#undef	td_get_otm_u

#endif	/*__TDNOSPT_H__*/
