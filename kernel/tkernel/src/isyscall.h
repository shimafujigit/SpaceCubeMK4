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
 *    Changed by UC Technology at 2013/01/23.
 *
 *    UCT T-Kernel 2.0 DevKit Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	isyscall.h (T-Kernel/OS)
 *	System Call Int. Format Definition
 */

#ifndef _ISYSCALL_
#define _ISYSCALL_

/* ------------------------------------------------------------------------ */

#if TA_GP
# define _CALL(p1, p2, p3, hdr, cb)	\
		CallUserHandler((INT)(p1), (INT)(p2), (INT)(p3), (FP)(hdr), (cb)->gp)
# define CallUserHandlerP1(   p1,         hdr, cb)	_CALL(p1, 0,  0,  hdr, cb)
# define CallUserHandlerP2(   p1, p2,     hdr, cb)	_CALL(p1, p2, 0,  hdr, cb)
# define CallUserHandlerP3(   p1, p2, p3, hdr, cb)	_CALL(p1, p2, p3, hdr, cb)
# define CallUserHandlerP2_GP(p1, p2,     hdr, cb)	_CALL(p1, p2, gp, hdr, cb)
#else
# define CallUserHandlerP1(   p1,         hdr, cb)	(*(hdr))(p1)
# define CallUserHandlerP2(   p1, p2,     hdr, cb)	(*(hdr))(p1, p2)
# define CallUserHandlerP3(   p1, p2, p3, hdr, cb)	(*(hdr))(p1, p2, p3)
# define CallUserHandlerP2_GP(p1, p2,     hdr, cb)	(*(hdr))(p1, p2)
#endif

/* ------------------------------------------------------------------------ */

#if TA_GP
#define P0(void)		( int _1,int _2,int _3,int _4,int _5, void *gp )
#define P1(p1)			( p1,    int _2,int _3,int _4,int _5, void *gp )
#define P2(p1, p2)		( p1, p2,       int _3,int _4,int _5, void *gp )
#define P3(p1, p2, p3)		( p1, p2, p3,          int _4,int _5, void *gp )
#define P4(p1, p2, p3, p4)	( p1, p2, p3, p4,             int _5, void *gp )
#define P5(p1, p2, p3, p4, p5)	( p1, p2, p3, p4, p5,                 void *gp )
#define P2GP(p1, p2)		( p1, p2,			      void *gp )
#else
#define P0(void)		( void )
#define P1(p1)			( p1 )
#define P2(p1, p2)		( p1, p2 )
#define P3(p1, p2, p3)		( p1, p2, p3 )
#define P4(p1, p2, p3, p4)	( p1, p2, p3, p4 )
#define P5(p1, p2, p3, p4, p5)	( p1, p2, p3, p4, p5 )
#define P2GP(p1, p2)		( p1, p2 )
#endif

/* ------------------------------------------------------------------------ */

/* T-Kernel/OS */
IMPORT ID _tk_cre_tsk P1( CONST T_CTSK *pk_ctsk );
IMPORT ER _tk_del_tsk( ID tskid );
IMPORT ER _tk_sta_tsk( ID tskid, INT stacd );
IMPORT void _tk_ext_tsk( void );
IMPORT void _tk_exd_tsk( void );
IMPORT ER _tk_ter_tsk( ID tskid );
IMPORT ER _tk_dis_dsp( void );
IMPORT ER _tk_ena_dsp( void );
IMPORT ER _tk_chg_pri( ID tskid, PRI tskpri );
IMPORT ER _tk_chg_slt( ID tskid, RELTIM slicetime );
IMPORT ER _tk_rot_rdq( PRI tskpri );
IMPORT ER _tk_rel_wai( ID tskid );
IMPORT ID _tk_get_tid( void );
IMPORT ER _tk_get_tsp( ID tskid, T_TSKSPC *pk_tskspc );
IMPORT ER _tk_set_tsp( ID tskid, CONST T_TSKSPC *pk_tskspc );
IMPORT ID _tk_get_rid( ID tskid );
IMPORT ID _tk_set_rid( ID tskid, ID resid );
IMPORT ER _tk_get_reg( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs );
IMPORT ER _tk_set_reg( ID tskid, CONST T_REGS *pk_regs, CONST T_EIT *pk_eit, CONST T_CREGS *pk_cregs );
IMPORT ER _tk_get_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs );
IMPORT ER _tk_set_cpr( ID tskid, INT copno, CONST T_COPREGS *pk_copregs );
IMPORT ER _tk_inf_tsk( ID tskid, T_ITSK *pk_itsk, BOOL clr );
IMPORT ER _tk_ref_tsk( ID tskid, T_RTSK *pk_rtsk );
IMPORT ER _tk_def_tex( ID tskid, CONST T_DTEX *pk_dtex );
IMPORT ER _tk_ena_tex( ID tskid, UINT texptn );
IMPORT ER _tk_dis_tex( ID tskid, UINT texptn );
IMPORT ER _tk_ras_tex( ID tskid, INT texcd );
IMPORT INT _tk_end_tex( BOOL enatex );
IMPORT ER _tk_ref_tex( ID tskid, T_RTEX *pk_rtex );
IMPORT ER _tk_sus_tsk( ID tskid );
IMPORT ER _tk_rsm_tsk( ID tskid );
IMPORT ER _tk_frsm_tsk( ID tskid );
IMPORT ER _tk_slp_tsk( TMO tmout );
IMPORT ER _tk_wup_tsk( ID tskid );
IMPORT INT _tk_can_wup( ID tskid );
IMPORT ER _tk_sig_tev( ID tskid, INT tskevt );
IMPORT INT _tk_wai_tev( INT waiptn, TMO tmout );
IMPORT INT _tk_dis_wai( ID tskid, UINT waitmask );
IMPORT ER _tk_ena_wai( ID tskid );
IMPORT ER _tk_chg_slt_u( ID tskid, RELTIM_U slicetime_u );
IMPORT ER _tk_inf_tsk_u( ID tskid, T_ITSK_U *pk_itsk_u, BOOL clr );
IMPORT ER _tk_ref_tsk_u( ID tskid, T_RTSK_U *pk_rtsk_u );
IMPORT ER _tk_slp_tsk_u( TMO_U tmout_u );
IMPORT INT _tk_wai_tev_u( INT waiptn, TMO_U tmout_u );
IMPORT ER _tk_set_tim_u( SYSTIM_U tim_u );
IMPORT ER _tk_get_tim_u( SYSTIM_U *tim_u, UINT *ofs );
IMPORT ER _tk_get_otm_u( SYSTIM_U *tim_u, UINT *ofs );
IMPORT ER _tk_dly_tsk_u( RELTIM_U dlytim_u );


#if defined(USE_FUNC_TK_CRE_SEM)
IMPORT ID _tk_cre_sem( CONST T_CSEM *pk_csem );
#endif
#if defined(USE_FUNC_TK_DEL_SEM)
IMPORT ER _tk_del_sem( ID semid );
#endif
#if defined(USE_FUNC_TK_SIG_SEM)
IMPORT ER _tk_sig_sem( ID semid, INT cnt );
#endif
#if defined(USE_FUNC_TK_WAI_SEM)
IMPORT ER _tk_wai_sem( ID semid, INT cnt, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REF_SEM)
IMPORT ER _tk_ref_sem( ID semid, T_RSEM *pk_rsem );
#endif
#if defined(USE_FUNC_TK_WAI_SEM)
IMPORT ER _tk_wai_sem_u( ID semid, INT cnt, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_SEM)
IMPORT INT _td_lst_sem( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_SEM)
IMPORT ER  _td_ref_sem( ID semid, TD_RSEM *rsem );
#endif
#if defined(USE_FUNC_TD_SEM_QUE)
IMPORT INT _td_sem_que( ID semid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_MTX)
IMPORT ID _tk_cre_mtx( CONST T_CMTX *pk_cmtx );
#endif
#if defined(USE_FUNC_TK_DEL_MTX)
IMPORT ER _tk_del_mtx( ID mtxid );
#endif
#if defined(USE_FUNC_TK_LOC_MTX)
IMPORT ER _tk_loc_mtx( ID mtxid, TMO tmout );
#endif
#if defined(USE_FUNC_TK_UNL_MTX)
IMPORT ER _tk_unl_mtx( ID mtxid );
#endif
#if defined(USE_FUNC_TK_REF_MTX)
IMPORT ER _tk_ref_mtx( ID mtxid, T_RMTX *pk_rmtx );
#endif
#if defined(USE_FUNC_TK_LOC_MTX)
IMPORT ER _tk_loc_mtx_u( ID mtxid, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_MTX)
IMPORT INT _td_lst_mtx( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_MTX)
IMPORT ER  _td_ref_mtx( ID mtxid, TD_RMTX *rmtx );
#endif
#if defined(USE_FUNC_TD_MTX_QUE)
IMPORT INT _td_mtx_que( ID mtxid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_FLG)
IMPORT ID _tk_cre_flg( CONST T_CFLG *pk_cflg );
#endif
#if defined(USE_FUNC_TK_DEL_FLG)
IMPORT ER _tk_del_flg( ID flgid );
#endif
#if defined(USE_FUNC_TK_SET_FLG)
IMPORT ER _tk_set_flg( ID flgid, UINT setptn );
#endif
#if defined(USE_FUNC_TK_CLR_FLG)
IMPORT ER _tk_clr_flg( ID flgid, UINT clrptn );
#endif
#if defined(USE_FUNC_TK_WAI_FLG)
IMPORT ER _tk_wai_flg( ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REF_FLG)
IMPORT ER _tk_ref_flg( ID flgid, T_RFLG *pk_rflg );
#endif
#if defined(USE_FUNC_TK_WAI_FLG)
IMPORT ER _tk_wai_flg_u( ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_FLG)
IMPORT INT _td_lst_flg( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_FLG)
IMPORT ER  _td_ref_flg( ID flgid, TD_RFLG *rflg );
#endif
#if defined(USE_FUNC_TD_FLG_QUE)
IMPORT INT _td_flg_que( ID flgid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_MBX)
IMPORT ID _tk_cre_mbx( CONST T_CMBX* pk_cmbx );
#endif
#if defined(USE_FUNC_TK_DEL_MBX)
IMPORT ER _tk_del_mbx( ID mbxid );
#endif
#if defined(USE_FUNC_TK_SND_MBX)
IMPORT ER _tk_snd_mbx( ID mbxid, T_MSG *pk_msg );
#endif
#if defined(USE_FUNC_TK_RCV_MBX)
IMPORT ER _tk_rcv_mbx( ID mbxid, T_MSG **ppk_msg, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REF_MBX)
IMPORT ER _tk_ref_mbx( ID mbxid, T_RMBX *pk_rmbx );
#endif
#if defined(USE_FUNC_TK_RCV_MBX)
IMPORT ER _tk_rcv_mbx_u( ID mbxid, T_MSG **ppk_msg, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_MBX)
IMPORT INT _td_lst_mbx( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_MBX)
IMPORT ER  _td_ref_mbx( ID mbxid, TD_RMBX *rmbx );
#endif
#if defined(USE_FUNC_TD_MBX_QUE)
IMPORT INT _td_mbx_que( ID mbxid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_MBF)
IMPORT ID _tk_cre_mbf( CONST T_CMBF *pk_cmbf );
#endif
#if defined(USE_FUNC_TK_DEL_MBF)
IMPORT ER _tk_del_mbf( ID mbfid );
#endif
#if defined(USE_FUNC_TK_SND_MBF)
IMPORT ER _tk_snd_mbf( ID mbfid, CONST void *msg, INT msgsz, TMO tmout );
#endif
#if defined(USE_FUNC_TK_RCV_MBF)
IMPORT INT _tk_rcv_mbf( ID mbfid, void *msg, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REF_MBF)
IMPORT ER _tk_ref_mbf( ID mbfid, T_RMBF *pk_rmbf );
#endif
#if defined(USE_FUNC_TK_SND_MBF)
IMPORT ER _tk_snd_mbf_u( ID mbfid, CONST void *msg, INT msgsz, TMO_U tmout_u );
#endif
#if defined(USE_FUNC_TK_RCV_MBF)
IMPORT INT _tk_rcv_mbf_u( ID mbfid, void *msg, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_MBF)
IMPORT INT _td_lst_mbf( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_MBF)
IMPORT ER  _td_ref_mbf( ID mbfid, TD_RMBF *rmbf );
#endif
#if defined(USE_FUNC_TD_SMBF_QUE)
IMPORT INT _td_smbf_que( ID mbfid, ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_RMBF_QUE)
IMPORT INT _td_rmbf_que( ID mbfid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_POR)
IMPORT ID _tk_cre_por( CONST T_CPOR *pk_cpor );
#endif
#if defined(USE_FUNC_TK_DEL_POR)
IMPORT ER _tk_del_por( ID porid );
#endif
#if defined(USE_FUNC_TK_CAL_POR)
IMPORT INT _tk_cal_por( ID porid, UINT calptn, void *msg, INT cmsgsz, TMO tmout );
#endif
#if defined(USE_FUNC_TK_ACP_POR)
IMPORT INT _tk_acp_por( ID porid, UINT acpptn, RNO *p_rdvno, void *msg, TMO tmout );
#endif
#if defined(USE_FUNC_TK_FWD_POR)
IMPORT ER _tk_fwd_por( ID porid, UINT calptn, RNO rdvno, void *msg, INT cmsgsz );
#endif
#if defined(USE_FUNC_TK_RPL_RDV)
IMPORT ER _tk_rpl_rdv( RNO rdvno, void *msg, INT rmsgsz );
#endif
#if defined(USE_FUNC_TK_REF_POR)
IMPORT ER _tk_ref_por( ID porid, T_RPOR *pk_rpor );
#endif
#if defined(USE_FUNC_TK_CAL_POR)
IMPORT INT _tk_cal_por_u( ID porid, UINT calptn, void *msg, INT cmsgsz, TMO_U tmout_u );
#endif
#if defined(USE_FUNC_TK_ACP_POR)
IMPORT INT _tk_acp_por_u( ID porid, UINT acpptn, RNO *p_rdvno, void *msg, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_POR)
IMPORT INT _td_lst_por( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_POR)
IMPORT ER  _td_ref_por( ID porid, TD_RPOR *rpor );
#endif
#if defined(USE_FUNC_TD_CAL_QUE)
IMPORT INT _td_cal_que( ID porid, ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_ACP_QUE)
IMPORT INT _td_acp_que( ID porid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_DEF_INT)
IMPORT ER _tk_def_int P2( UINT dintno, CONST T_DINT *pk_dint );
#endif
#if defined(USE_FUNC_TK_RET_INT)
IMPORT void _tk_ret_int( void );
#endif


#if defined(USE_FUNC_TK_CRE_MPL)
IMPORT ID _tk_cre_mpl( CONST T_CMPL *pk_cmpl );
#endif
#if defined(USE_FUNC_TK_DEL_MPL)
IMPORT ER _tk_del_mpl( ID mplid );
#endif
#if defined(USE_FUNC_TK_GET_MPL)
IMPORT ER _tk_get_mpl( ID mplid, INT blksz, void **p_blk, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REL_MPL)
IMPORT ER _tk_rel_mpl( ID mplid, void *blk );
#endif
#if defined(USE_FUNC_TK_REF_MPL)
IMPORT ER _tk_ref_mpl( ID mplid, T_RMPL *pk_rmpl );
#endif
#if defined(USE_FUNC_TK_GET_MPL)
IMPORT ER _tk_get_mpl_u( ID mplid, INT blksz, void **p_blk, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_MPL)
IMPORT INT _td_lst_mpl( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_MPL)
IMPORT ER  _td_ref_mpl( ID mplid, TD_RMPL *rmpl );
#endif
#if defined(USE_FUNC_TD_MPL_QUE)
IMPORT INT _td_mpl_que( ID mplid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_CRE_MPF)
IMPORT ID _tk_cre_mpf( CONST T_CMPF *pk_cmpf );
#endif
#if defined(USE_FUNC_TK_DEL_MPF)
IMPORT ER _tk_del_mpf( ID mpfid );
#endif
#if defined(USE_FUNC_TK_GET_MPF)
IMPORT ER _tk_get_mpf( ID mpfid, void **p_blf, TMO tmout );
#endif
#if defined(USE_FUNC_TK_REL_MPF)
IMPORT ER _tk_rel_mpf( ID mpfid, void *blf );
#endif
#if defined(USE_FUNC_TK_REF_MPF)
IMPORT ER _tk_ref_mpf( ID mpfid, T_RMPF *pk_rmpf );
#endif
#if defined(USE_FUNC_TK_GET_MPF)
IMPORT ER _tk_get_mpf_u( ID mpfid, void **p_blf, TMO_U tmout_u );
#endif

#if defined(USE_FUNC_TD_LST_MPF)
IMPORT INT _td_lst_mpf( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_MPF)
IMPORT ER  _td_ref_mpf( ID mpfid, TD_RMPF *rmpf );
#endif
#if defined(USE_FUNC_TD_MPF_QUE)
IMPORT INT _td_mpf_que( ID mpfid, ID list[], INT nent );
#endif


#if defined(USE_FUNC_TK_SET_TIM)
IMPORT ER _tk_set_tim( CONST SYSTIM *pk_tim );
#endif
#if defined(USE_FUNC_TK_GET_TIM)
IMPORT ER _tk_get_tim( SYSTIM *pk_tim );
#endif
#if defined(USE_FUNC_TK_GET_OTM)
IMPORT ER _tk_get_otm( SYSTIM *pk_tim );
#endif
#if defined(USE_FUNC_TK_DLY_TSK)
IMPORT ER _tk_dly_tsk( RELTIM dlytim );
#endif


#if defined(USE_FUNC_TK_CRE_CYC)
IMPORT ID _tk_cre_cyc P1( CONST T_CCYC *pk_ccyc );
#endif
#if defined(USE_FUNC_TK_DEL_CYC)
IMPORT ER _tk_del_cyc( ID cycid );
#endif
#if defined(USE_FUNC_TK_STA_CYC)
IMPORT ER _tk_sta_cyc( ID cycid );
#endif
#if defined(USE_FUNC_TK_STP_CYC)
IMPORT ER _tk_stp_cyc( ID cycid );
#endif
#if defined(USE_FUNC_TK_REF_CYC)
IMPORT ER _tk_ref_cyc( ID cycid, T_RCYC *pk_rcyc );
#endif
#if defined(USE_FUNC_TK_CRE_CYC)
IMPORT ID _tk_cre_cyc_u( CONST T_CCYC_U *pk_ccyc_u );
#endif
#if defined(USE_FUNC_TK_REF_CYC)
IMPORT ER _tk_ref_cyc_u( ID cycid, T_RCYC_U *pk_rcyc_u );
#endif

#if defined(USE_FUNC_TD_LST_CYC)
IMPORT INT _td_lst_cyc( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_CYC)
IMPORT ER  _td_ref_cyc( ID cycid, TD_RCYC *rcyc );
#endif
#if defined(USE_FUNC_TD_REF_CYC)
IMPORT ER  _td_ref_cyc_u( ID cycid, TD_RCYC_U *rcyc_u );
#endif


#if defined(USE_FUNC_TK_CRE_ALM)
IMPORT ID _tk_cre_alm P1( CONST T_CALM *pk_calm );
#endif
#if defined(USE_FUNC_TK_DEL_ALM)
IMPORT ER _tk_del_alm( ID almid );
#endif
#if defined(USE_FUNC_TK_STA_ALM)
IMPORT ER _tk_sta_alm( ID almid, RELTIM almtim );
#endif
#if defined(USE_FUNC_TK_STP_ALM)
IMPORT ER _tk_stp_alm( ID almid );
#endif
#if defined(USE_FUNC_TK_REF_ALM)
IMPORT ER _tk_ref_alm( ID almid, T_RALM *pk_ralm );
#endif
#if defined(USE_FUNC_TK_STA_ALM)
IMPORT ER _tk_sta_alm_u( ID almid, RELTIM_U almtim_u );
#endif
#if defined(USE_FUNC_TK_REF_ALM)
IMPORT ER _tk_ref_alm_u( ID almid, T_RALM_U *pk_ralm_u );
#endif

#if defined(USE_FUNC_TD_LST_ALM)
IMPORT INT _td_lst_alm( ID list[], INT nent );
#endif
#if defined(USE_FUNC_TD_REF_ALM)
IMPORT ER  _td_ref_alm( ID almid, TD_RALM *ralm );
#endif
#if defined(USE_FUNC_TD_REF_ALM)
IMPORT ER  _td_ref_alm_u( ID almid, TD_RALM_U *ralm_u );
#endif


#if defined(USE_FUNC_TK_REF_VER)
IMPORT ER _tk_ref_ver( T_RVER *pk_rver );
#endif
#if defined(USE_FUNC_TK_REF_SYS)
IMPORT ER _tk_ref_sys( T_RSYS *pk_rsys );
#endif
#if defined(USE_FUNC_TK_DEF_SSY)
IMPORT ER _tk_def_ssy P2( ID ssid, CONST T_DSSY *pk_dssy );
#endif
#if defined(USE_FUNC_TK_STA_SSY)
IMPORT ER _tk_sta_ssy( ID ssid, ID resid, INT info );
#endif
#if defined(USE_FUNC_TK_CLN_SSY)
IMPORT ER _tk_cln_ssy( ID ssid, ID resid, INT info );
#endif
#if defined(USE_FUNC_TK_EVT_SSY)
IMPORT ER _tk_evt_ssy( ID ssid, INT evttyp, ID resid, INT info );
#endif
#if defined(USE_FUNC_TK_REF_SSY)
IMPORT ER _tk_ref_ssy( ID ssid, T_RSSY *pk_rssy );
#endif
#if defined(USE_FUNC_TK_CRE_RES)
IMPORT ID _tk_cre_res( void );
#endif
#if defined(USE_FUNC_TK_DEL_RES)
IMPORT ER _tk_del_res( ID resid );
#endif
#if defined(USE_FUNC_TK_GET_RES)
IMPORT ER _tk_get_res( ID resid, ID ssid, void **p_resblk );
#endif
#if defined(USE_FUNC_TK_SET_POW)
IMPORT ER _tk_set_pow( UINT powmode );
#endif

/* T-Kernel/DS */
IMPORT INT _td_lst_tsk( ID list[], INT nent );
IMPORT INT _td_lst_ssy( ID list[], INT nent );
IMPORT ER  _td_ref_ssy( ID ssid, TD_RSSY *rssy );
IMPORT ER  _td_ref_tsk( ID tskid, TD_RTSK *rtsk );
IMPORT ER  _td_ref_tex( ID tskid, TD_RTEX *rtex );
IMPORT ER  _td_inf_tsk( ID tskid, TD_ITSK *itsk, BOOL clr );
IMPORT ER  _td_get_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs );
IMPORT ER  _td_set_reg( ID tskid, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs );
IMPORT ER  _td_ref_sys( TD_RSYS *rsys );
IMPORT ER  _td_get_tim( SYSTIM *tim, UINT *ofs );
IMPORT ER  _td_get_otm( SYSTIM *tim, UINT *ofs );
IMPORT INT _td_rdy_que( PRI pri, ID list[], INT nent );
IMPORT ER  _td_hok_svc( CONST TD_HSVC *hsvc );
IMPORT ER  _td_hok_dsp( CONST TD_HDSP *hdsp );
IMPORT ER  _td_hok_int( CONST TD_HINT *hint );
IMPORT ER  _td_ref_dsname( UINT type, ID id, UB *dsname );
IMPORT ER  _td_set_dsname( UINT type, ID id, CONST UB *dsname );
IMPORT ER  _td_ref_tsk_u( ID tskid, TD_RTSK_U *rtsk_u );
IMPORT ER  _td_inf_tsk_u( ID tskid, TD_ITSK_U *itsk_u, BOOL clr );
IMPORT ER  _td_get_tim_u( SYSTIM_U *tim_u, UINT *ofs );
IMPORT ER  _td_get_otm_u( SYSTIM_U *tim_u, UINT *ofs );

/* T-Kernel/SM */
IMPORT INT _tk_get_cfn( UB *name, INT *val, INT max );
IMPORT INT _tk_get_cfs( UB *name, UB *buf, INT max );

#endif /* _ISYSCALL_ */
