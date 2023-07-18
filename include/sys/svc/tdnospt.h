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
#define	_td_lst_tsk	no_support
#endif

#if !defined(USE_FUNC_TD_LST_SEM)
#define	_td_lst_sem	no_support
#endif

#if !defined(USE_FUNC_TD_LST_FLG)
#define	_td_lst_flg	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBX)
#define	_td_lst_mbx	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MTX)
#define	_td_lst_mtx	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MBF)
#define	_td_lst_mbf	no_support
#endif

#if !defined(USE_FUNC_TD_LST_POR)
#define	_td_lst_por	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPF)
#define	_td_lst_mpf	no_support
#endif

#if !defined(USE_FUNC_TD_LST_MPL)
#define	_td_lst_mpl	no_support
#endif

#if !defined(USE_FUNC_TD_LST_CYC)
#define	_td_lst_cyc	no_support
#endif

#if !defined(USE_FUNC_TD_LST_ALM)
#define	_td_lst_alm	no_support
#endif

#if !defined(USE_FUNC_TD_LST_SSY)
#define	_td_lst_ssy	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SEM)
#define	_td_ref_sem	no_support
#endif

#if !defined(USE_FUNC_TD_REF_FLG)
#define	_td_ref_flg	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MBX)
#define	_td_ref_mbx	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MTX)
#define	_td_ref_mtx	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MBF)
#define	_td_ref_mbf	no_support
#endif

#if !defined(USE_FUNC_TD_REF_POR)
#define	_td_ref_por	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MPF)
#define	_td_ref_mpf	no_support
#endif

#if !defined(USE_FUNC_TD_REF_MPL)
#define	_td_ref_mpl	no_support
#endif

#if !defined(USE_FUNC_TD_REF_CYC)
#define	_td_ref_cyc	no_support
#define	_td_ref_cyc_u	no_support
#endif

#if !defined(USE_FUNC_TD_REF_ALM)
#define	_td_ref_alm	no_support
#define	_td_ref_alm_u	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SSY)
#define	_td_ref_ssy	no_support
#endif

#if !defined(USE_FUNC_TD_REF_TSK)
#define	_td_ref_tsk	no_support
#define	_td_ref_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TD_REF_TEX)
#define	_td_ref_tex	no_support
#endif

#if !defined(USE_FUNC_TD_INF_TSK)
#define	_td_inf_tsk	no_support
#define	_td_inf_tsk_u	no_support
#endif

#if !defined(USE_FUNC_TD_GET_REG)
#define	_td_get_reg	no_support
#endif

#if !defined(USE_FUNC_TD_SET_REG)
#define	_td_set_reg	no_support
#endif

#if !defined(USE_FUNC_TD_REF_SYS)
#define	_td_ref_sys	no_support
#endif

#if !defined(USE_FUNC_TD_GET_TIM)
#define	_td_get_tim	no_support
#define	_td_get_tim_u	no_support
#endif

#if !defined(USE_FUNC_TD_GET_OTM)
#define	_td_get_otm	no_support
#define	_td_get_otm_u	no_support
#endif

#if !defined(USE_FUNC_TD_RDY_QUE)
#define	_td_rdy_que	no_support
#endif

#if !defined(USE_FUNC_TD_SEM_QUE)
#define	_td_sem_que	no_support
#endif

#if !defined(USE_FUNC_TD_FLG_QUE)
#define	_td_flg_que	no_support
#endif

#if !defined(USE_FUNC_TD_MBX_QUE)
#define	_td_mbx_que	no_support
#endif

#if !defined(USE_FUNC_TD_MTX_QUE)
#define	_td_mtx_que	no_support
#endif

#if !defined(USE_FUNC_TD_SMBF_QUE)
#define	_td_smbf_que	no_support
#endif

#if !defined(USE_FUNC_TD_RMBF_QUE)
#define	_td_rmbf_que	no_support
#endif

#if !defined(USE_FUNC_TD_CAL_QUE)
#define	_td_cal_que	no_support
#endif

#if !defined(USE_FUNC_TD_ACP_QUE)
#define	_td_acp_que	no_support
#endif

#if !defined(USE_FUNC_TD_MPF_QUE)
#define	_td_mpf_que	no_support
#endif

#if !defined(USE_FUNC_TD_MPL_QUE)
#define	_td_mpl_que	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_SVC)
#define	_td_hok_svc	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_DSP)
#define	_td_hok_dsp	no_support
#endif

#if !defined(USE_FUNC_TD_HOK_INT)
#define	_td_hok_int	no_support
#endif

#if !defined(USE_FUNC_TD_REF_DSNAME)
#define	_td_ref_dsname	no_support
#endif

#if !defined(USE_FUNC_TD_SET_DSNAME)
#define	_td_set_dsname	no_support
#endif

#else	/*__TDNOSPT_H__*/

#undef	_td_lst_tsk
#undef	_td_lst_sem
#undef	_td_lst_flg
#undef	_td_lst_mbx
#undef	_td_lst_mtx
#undef	_td_lst_mbf
#undef	_td_lst_por
#undef	_td_lst_mpf
#undef	_td_lst_mpl
#undef	_td_lst_cyc
#undef	_td_lst_alm
#undef	_td_lst_ssy
#undef	_td_ref_sem
#undef	_td_ref_flg
#undef	_td_ref_mbx
#undef	_td_ref_mtx
#undef	_td_ref_mbf
#undef	_td_ref_por
#undef	_td_ref_mpf
#undef	_td_ref_mpl
#undef	_td_ref_cyc
#undef	_td_ref_alm
#undef	_td_ref_ssy
#undef	_td_ref_tsk
#undef	_td_ref_tex
#undef	_td_inf_tsk
#undef	_td_get_reg
#undef	_td_set_reg
#undef	_td_ref_sys
#undef	_td_get_tim
#undef	_td_get_otm
#undef	_td_rdy_que
#undef	_td_sem_que
#undef	_td_flg_que
#undef	_td_mbx_que
#undef	_td_mtx_que
#undef	_td_smbf_que
#undef	_td_rmbf_que
#undef	_td_cal_que
#undef	_td_acp_que
#undef	_td_mpf_que
#undef	_td_mpl_que
#undef	_td_hok_svc
#undef	_td_hok_dsp
#undef	_td_hok_int
#undef	_td_ref_dsname
#undef	_td_set_dsname
#undef	_td_ref_cyc_u
#undef	_td_ref_alm_u
#undef	_td_ref_tsk_u
#undef	_td_inf_tsk_u
#undef	_td_get_tim_u
#undef	_td_get_otm_u

#endif	/*__TDNOSPT_H__*/
