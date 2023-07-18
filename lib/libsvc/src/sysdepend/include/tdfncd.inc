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
; *	T-Kernel/DS function code [HEW]
; *
; *	   created from dbgspt.h
; */

TDFN_LST_TSK:	.define	"H'80010200"
TDFN_LST_SEM:	.define	"H'80020200"
TDFN_LST_FLG:	.define	"H'80030200"
TDFN_LST_MBX:	.define	"H'80040200"
TDFN_LST_MTX:	.define	"H'80050200"
TDFN_LST_MBF:	.define	"H'80060200"
TDFN_LST_POR:	.define	"H'80070200"
TDFN_LST_MPF:	.define	"H'80080200"
TDFN_LST_MPL:	.define	"H'80090200"
TDFN_LST_CYC:	.define	"H'800a0200"
TDFN_LST_ALM:	.define	"H'800b0200"
TDFN_LST_SSY:	.define	"H'800c0200"
TDFN_REF_SEM:	.define	"H'800d0200"
TDFN_REF_FLG:	.define	"H'800e0200"
TDFN_REF_MBX:	.define	"H'800f0200"
TDFN_REF_MTX:	.define	"H'80100200"
TDFN_REF_MBF:	.define	"H'80110200"
TDFN_REF_POR:	.define	"H'80120200"
TDFN_REF_MPF:	.define	"H'80130200"
TDFN_REF_MPL:	.define	"H'80140200"
TDFN_REF_CYC:	.define	"H'80150200"
TDFN_REF_ALM:	.define	"H'80160200"
TDFN_REF_SSY:	.define	"H'80170200"
TDFN_REF_TSK:	.define	"H'80180200"
TDFN_REF_TEX:	.define	"H'80190200"
TDFN_INF_TSK:	.define	"H'801a0300"
TDFN_GET_REG:	.define	"H'801b0400"
TDFN_SET_REG:	.define	"H'801c0400"
TDFN_REF_SYS:	.define	"H'801d0100"
TDFN_GET_TIM:	.define	"H'801e0200"
TDFN_GET_OTM:	.define	"H'801f0200"
TDFN_RDY_QUE:	.define	"H'80200300"
TDFN_SEM_QUE:	.define	"H'80210300"
TDFN_FLG_QUE:	.define	"H'80220300"
TDFN_MBX_QUE:	.define	"H'80230300"
TDFN_MTX_QUE:	.define	"H'80240300"
TDFN_SMBF_QUE:	.define	"H'80250300"
TDFN_RMBF_QUE:	.define	"H'80260300"
TDFN_CAL_QUE:	.define	"H'80270300"
TDFN_ACP_QUE:	.define	"H'80280300"
TDFN_MPF_QUE:	.define	"H'80290300"
TDFN_MPL_QUE:	.define	"H'802a0300"
TDFN_HOK_SVC:	.define	"H'802b0100"
TDFN_HOK_DSP:	.define	"H'802c0100"
TDFN_HOK_INT:	.define	"H'802d0100"
TDFN_REF_DSNAME:	.define	"H'802e0300"
TDFN_SET_DSNAME:	.define	"H'802f0300"
TDFN_REF_CYC_U:	.define	"H'80300200"
TDFN_REF_ALM_U:	.define	"H'80310200"
TDFN_REF_TSK_U:	.define	"H'80320200"
TDFN_INF_TSK_U:	.define	"H'80330300"
TDFN_GET_TIM_U:	.define	"H'80340200"
TDFN_GET_OTM_U:	.define	"H'80350200"

