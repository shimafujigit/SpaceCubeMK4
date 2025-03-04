/*
 *----------------------------------------------------------------------
 *    micro T-Kernel
 *
 *    Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
 *    micro T-Kernel is distributed under the micro T-License.
 *----------------------------------------------------------------------
 *
 *    Version:   1.01.01
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2011/07/29.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the UCT T2AS.
 *    Changed by UC Technology at 2014/03/18.
 *
 *    UCT T2AS DevKit Version 1.00.00
 *    Copyright (c) 2014 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	tk_config_func.h
 *	System Configuration Definition
 */

#ifndef _TK_CONFIG_FUNC_H_
#define _TK_CONFIG_FUNC_H_

#include "../kernel/tkernel/src/config.h"	/* to use NUM_XXXID */

/* When removing individual functions completely, comment-out corresponding
 * definitions in the above header file. 
 *    NUM_SEMID, NUM_MTXID, NUM_FLGID, NUM_MBXID, NUM_MBFID, NUM_PORID,
 *    NUM_MPLID, NUM_MPFID, NUM_CYCID, NUM_ALMID, NUM_SSYID, NUM_OPNDEV
 *  Also, when removing debug functions completely, define USE_DBGSPT by 0.
 */

/*
 * System calls
 */
#define USE_FUNC_TK_CRE_TSK
#define USE_FUNC_TK_DEL_TSK
#define USE_FUNC_TK_STA_TSK
#define USE_FUNC_TK_EXT_TSK
#define USE_FUNC_TK_EXD_TSK
#define USE_FUNC_TK_TER_TSK
#define USE_FUNC_TK_DIS_DSP
#define USE_FUNC_TK_ENA_DSP
#define USE_FUNC_TK_CHG_PRI
#define USE_FUNC_TK_ROT_RDQ
#define USE_FUNC_TK_CHG_SLT			/* TK2 */
#define USE_FUNC_TK_INF_TSK			/* TK2 */
#define USE_FUNC_TK_GET_RID			/* TK2 */
#define USE_FUNC_TK_SET_RID			/* TK2 */
#define USE_FUNC_TK_REL_WAI
#define USE_FUNC_TK_DIS_WAI			/* TK2 */
#define USE_FUNC_TK_ENA_WAI			/* TK2 */
#define USE_FUNC_TK_GET_TID
#define USE_FUNC_TK_GET_REG
#define USE_FUNC_TK_SET_REG
#define USE_FUNC_TK_REF_TSK

#define USE_FUNC_TK_SUS_TSK
#define USE_FUNC_TK_RSM_TSK
#define USE_FUNC_TK_FRSM_TSK
#define USE_FUNC_TK_SLP_TSK
#define USE_FUNC_TK_WUP_TSK
#define USE_FUNC_TK_CAN_WUP
#define USE_FUNC_TK_WAI_TEV			/* TK2 */
#define USE_FUNC_TK_SIG_TEV			/* TK2 */

#if defined(NUM_SEMID)
#define USE_FUNC_TK_CRE_SEM
#define USE_FUNC_TK_DEL_SEM
#define USE_FUNC_TK_SIG_SEM
#define USE_FUNC_TK_WAI_SEM
#define USE_FUNC_TK_REF_SEM
#define USE_FUNC_SEMAPHORE_INITIALIZE
#define USE_FUNC_SEMCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_SEM
#define USE_FUNC_TD_REF_SEM
#define USE_FUNC_TD_SEM_QUE
#define USE_FUNC_SEMAPHORE_GETNAME
#endif
#endif

#if defined(NUM_MTXID)
#define USE_FUNC_TK_CRE_MTX
#define USE_FUNC_TK_DEL_MTX
#define USE_FUNC_TK_LOC_MTX
#define USE_FUNC_TK_UNL_MTX
#define USE_FUNC_TK_REF_MTX
#define USE_FUNC_MUTEX_INITIALIZE
#define USE_FUNC_MTXCB_TABLE
#define USE_FUNC_SIGNAL_ALL_MUTEX
#define USE_FUNC_CHG_PRI_MUTEX
#if USE_DBGSPT
#define USE_FUNC_TD_LST_MTX
#define USE_FUNC_TD_REF_MTX
#define USE_FUNC_TD_MTX_QUE
#define USE_FUNC_MUTEX_GETNAME
#endif
#endif

#if defined(NUM_FLGID)
#define USE_FUNC_TK_CRE_FLG
#define USE_FUNC_TK_DEL_FLG
#define USE_FUNC_TK_SET_FLG
#define USE_FUNC_TK_CLR_FLG
#define USE_FUNC_TK_WAI_FLG
#define USE_FUNC_TK_REF_FLG
#define USE_FUNC_EVENTFLAG_INITIALIZE
#define USE_FUNC_FLGCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_FLG
#define USE_FUNC_TD_REF_FLG
#define USE_FUNC_TD_FLG_QUE
#define USE_FUNC_EVENTFLAG_GETNAME
#endif
#endif

#if defined(NUM_MBXID)
#define USE_FUNC_TK_CRE_MBX
#define USE_FUNC_TK_DEL_MBX
#define USE_FUNC_TK_SND_MBX
#define USE_FUNC_TK_RCV_MBX
#define USE_FUNC_TK_REF_MBX
#define USE_FUNC_MAILBOX_INITIALIZE
#define USE_FUNC_MBXCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_MBX
#define USE_FUNC_TD_REF_MBX
#define USE_FUNC_TD_MBX_QUE
#define USE_FUNC_MAILBOX_GETNAME
#endif
#endif

#if defined(NUM_MBFID)
#define USE_FUNC_TK_CRE_MBF
#define USE_FUNC_TK_DEL_MBF
#define USE_FUNC_TK_SND_MBF
#define USE_FUNC_TK_RCV_MBF
#define USE_FUNC_TK_REF_MBF
#define USE_FUNC_MESSAGEBUFFER_INITIALIZE
#define USE_FUNC_MBFCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_MBF
#define USE_FUNC_TD_REF_MBF
#define USE_FUNC_TD_SMBF_QUE
#define USE_FUNC_TD_RMBF_QUE
#define USE_FUNC_MESSAGEBUFFER_GETNAME
#endif
#endif

#if defined(NUM_PORID)
#define USE_FUNC_TK_CRE_POR
#define USE_FUNC_TK_DEL_POR
#define USE_FUNC_TK_CAL_POR
#define USE_FUNC_TK_ACP_POR
#define USE_FUNC_TK_FWD_POR
#define USE_FUNC_TK_RPL_RDV
#define USE_FUNC_TK_REF_POR
#define USE_FUNC_RENDEZVOUS_INITIALIZE
#define USE_FUNC_PORCB_TABLE
#define USE_FUNC_WSPEC_CAL
#define USE_FUNC_WSPEC_RDV
#if USE_DBGSPT
#define USE_FUNC_TD_LST_POR
#define USE_FUNC_TD_REF_POR
#define USE_FUNC_TD_CAL_QUE
#define USE_FUNC_TD_ACP_QUE
#define USE_FUNC_RENDEZVOUS_GETNAME
#endif
#endif

#define USE_FUNC_HLL_INTHDR
#define USE_FUNC_TK_DEF_INT
/* #define USE_FUNC_TK_RET_INT */

#if defined(NUM_MPLID)
#define USE_FUNC_TK_CRE_MPL
#define USE_FUNC_TK_DEL_MPL
#define USE_FUNC_TK_GET_MPL
#define USE_FUNC_TK_REL_MPL
#define USE_FUNC_TK_REF_MPL
#define USE_FUNC_MEMORYPOOL_INITIALIZE
#define USE_FUNC_MPLCB_TABLE
#define USE_FUNC_MPL_WAKEUP
#if USE_DBGSPT
#define USE_FUNC_TD_LST_MPL
#define USE_FUNC_TD_REF_MPL
#define USE_FUNC_TD_MPL_QUE
#define USE_FUNC_MEMORYPOOL_GETNAME
#endif
#endif

#if defined(NUM_MPFID)
#define USE_FUNC_TK_CRE_MPF
#define USE_FUNC_TK_DEL_MPF
#define USE_FUNC_TK_GET_MPF
#define USE_FUNC_TK_REL_MPF
#define USE_FUNC_TK_REF_MPF
#define USE_FUNC_FIX_MEMORYPOOL_INITIALIZE
#define USE_FUNC_MPFCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_MPF
#define USE_FUNC_TD_REF_MPF
#define USE_FUNC_TD_MPF_QUE
#define USE_FUNC_FIX_MEMORYPOOL_GETNAME
#endif
#endif

#define USE_FUNC_TK_SET_TIM
#define USE_FUNC_TK_GET_TIM
#define USE_FUNC_TK_GET_OTM
#define USE_FUNC_TK_DLY_TSK

#if defined(NUM_CYCID)
#define USE_FUNC_TK_CRE_CYC
#define USE_FUNC_TK_DEL_CYC
#define USE_FUNC_TK_STA_CYC
#define USE_FUNC_TK_STP_CYC
#define USE_FUNC_TK_REF_CYC
#define USE_FUNC_CYCLICHANDLER_INITIALIZE
#define USE_FUNC_CYCCB_TABLE
#if USE_DBGSPT
#define USE_FUNC_TD_LST_CYC
#define USE_FUNC_TD_REF_CYC
#define USE_FUNC_CYCLICHANDLER_GETNAME
#endif
#endif

#if defined(NUM_ALMID)
#define USE_FUNC_TK_CRE_ALM
#define USE_FUNC_TK_DEL_ALM
#define USE_FUNC_TK_STA_ALM
#define USE_FUNC_TK_STP_ALM
#define USE_FUNC_TK_REF_ALM
#define USE_FUNC_ALARMHANDLER_INITIALIZE
#define USE_FUNC_ALMCB_TABLE
#define USE_FUNC_CALL_ALMHDR
#if USE_DBGSPT
#define USE_FUNC_TD_LST_ALM
#define USE_FUNC_TD_REF_ALM
#define USE_FUNC_ALARMHANDLER_GETNAME
#endif
#endif

#define USE_FUNC_TK_REF_VER
#define USE_FUNC_TK_REF_SYS
#define USE_FUNC_TK_SET_POW			/* uTK : USE_FUNC_LOWPOW_DISCNT */

#if defined(NUM_SSYID)
#define USE_FUNC_TK_DEF_SSY
#define USE_FUNC_TK_STA_SSY			/* TK2 */
#define USE_FUNC_TK_CLN_SSY			/* TK2 */
#define USE_FUNC_TK_EVT_SSY			/* TK2 */
#define USE_FUNC_TK_REF_SSY
#define USE_FUNC_SUBSYSTEM_INITIALIZE
#define USE_FUNC_SSYCB_TABLE
#define USE_FUNC_SVC_IENTRY
#if USE_DBGSPT
#define USE_FUNC_TD_LST_SSY
#define USE_FUNC_TD_REF_SSY
#endif
#define USE_FUNC_TK_DEF_TEX			/* TK2 */
#define USE_FUNC_TK_DIS_TEX			/* TK2 */
#define USE_FUNC_TK_ENA_TEX			/* TK2 */
#define USE_FUNC_CALL_BRKHDR			/* TK2 */
#define USE_FUNC_TK_RAS_TEX			/* TK2 */
#define USE_FUNC_TK_END_TEX			/* TK2 */
#define USE_FUNC_TK_REF_TEX			/* TK2 */
#if USE_DBGSPT
#define USE_FUNC_TD_REF_TEX			/* TK2 */
#endif
#define USE_FUNC_RESOURCE_GROUP_INITIALIZE	/* TK2 */
#define USE_FUNC_TK_CRE_RES			/* TK2 */
#define USE_FUNC_TK_DEL_RES			/* TK2 */
#define USE_FUNC_TK_GET_RES			/* TK2 */
#endif


/* T-Kernel/SM */				/* TK2 */

/* System Memory Management */


/*  Address Space Management */
#define USE_FUNC_SETTASKSPACE			/* TK2 */
#define USE_FUNC_CHKSPACER			/* TK2 */
#define USE_FUNC_CHKSPACERW			/* TK2 */
#define USE_FUNC_CHKSPACERE			/* TK2 */
#define USE_FUNC_CHKSPACEBSTRR			/* TK2 */
#define USE_FUNC_CHKSPACEBSTRRW			/* TK2 */
#define USE_FUNC_CHKSPACETSTRR			/* TK2 */
#define USE_FUNC_CHKSPACETSTRRW			/* TK2 */
#define USE_FUNC_LOCKSPACE			/* TK2 */
#define USE_FUNC_UNLOCKSPACE			/* TK2 */
#define USE_FUNC_CNVPHYSICALADDR		/* TK2 */
#define USE_FUNC_MAPMEMORY			/* TK2 */
#define USE_FUNC_UNMAPMEMORY			/* TK2 */
#define USE_FUNC_GETSPACEINFO			/* TK2 */
#define USE_FUNC_SETMEMORYACCESS		/* TK2 */

/* Device Management */
#if defined(NUM_OPNDEV)				/* uTK : CFN_MAX_REGDEV */
#define USE_FUNC_TK_OPN_DEV
#define USE_FUNC_TK_CLS_DEV
#define USE_FUNC_TK_REA_DEV
#define USE_FUNC_TK_SREA_DEV
#define USE_FUNC_TK_WRI_DEV
#define USE_FUNC_TK_SWRI_DEV
#define USE_FUNC_TK_WAI_DEV
#define USE_FUNC_TK_SUS_DEV
#define USE_FUNC_TK_GET_DEV
#define USE_FUNC_TK_REF_DEV
#define USE_FUNC_TK_OREF_DEV
#define USE_FUNC_TK_LST_DEV
#define USE_FUNC_TK_EVT_DEV
#define USE_FUNC_TK_DEF_DEV
#define USE_FUNC_TK_REF_IDV
#define USE_FUNC_INITIALIZE_DEVMGR
#define USE_FUNC_FINISH_DEVMGR
#define USE_FUNC_SEARCHDEVCB
#define USE_FUNC_CHECK_DEVDESC
#define USE_FUNC_DEVMGR_BREAK			/* TK2 */
#define USE_FUNC_DEVMGR_STARTUP
#define USE_FUNC_DEVMGR_CLEANUP
#define USE_FUNC_INITDEVIO
#define USE_FUNC_FINISHDEVIO
#define USE_FUNC_DEVMGRSYNC			/* TK2 */
#define USE_FUNC_DEVMGRLOCK
#define USE_FUNC_PHYDEVNM
#endif

#if USE_DBGSPT
/* Refer each object usage state */
#define USE_FUNC_TD_LST_TSK
#define USE_FUNC_TD_REF_TSK
#define USE_FUNC_TD_INF_TSK
#define USE_FUNC_TD_GET_REG
#define USE_FUNC_TD_SET_REG
#define USE_FUNC_TASK_GETNAME

/* Refer system state */
#define USE_FUNC_TD_REF_SYS
#define USE_FUNC_TD_GET_TIM
#define USE_FUNC_TD_GET_OTM

/* Refer ready queue */
#define USE_FUNC_TD_RDY_QUE

/* Execution trace */
#define USE_FUNC_TD_HOK_SVC
#define USE_FUNC_TD_HOK_DSP
#define USE_FUNC_TD_HOK_INT
#define USE_FUNC_HOOK_ENTERFN
#define USE_FUNC_HOOK_EXECFN
#define USE_FUNC_HOOK_IENTERFN

/* Object name */
#define USE_FUNC_OBJECT_GETNAME
#define USE_FUNC_TD_REF_DSNAME
#define USE_FUNC_TD_SET_DSNAME

#endif /* USE_DBGSPT */

/*
 * Other functions
 */

/* tstdlib */
#define USE_FUNC_TSTDLIB_BITCLR
#define USE_FUNC_TSTDLIB_BITSET
#define USE_FUNC_TSTDLIB_BITTEST		/* TK2 */
#define USE_FUNC_TSTDLIB_BITSEARCH0		/* TK2 */
#define USE_FUNC_TSTDLIB_BITSEARCH1
#define USE_FUNC_TSTDLIB_BITSEARCH1_BINSEARCH	/* TK2 */

/* task concerning */
#define USE_FUNC_CTXTSK
#define USE_FUNC_TCB_TABLE
#define USE_FUNC_TASK_INITIALIZE
#define USE_FUNC_MAKE_DORMANT
#define USE_FUNC_MAKE_READY
#define USE_FUNC_MAKE_NON_READY
#define USE_FUNC_CHANGE_TASK_PRIORITY
#define USE_FUNC_ROTATE_READY_QUEUE
#define USE_FUNC_ROTATE_READY_QUEUE_RUN

/* wait concerning */
#define USE_FUNC_WAIT_RELEASE_OK
#define USE_FUNC_WAIT_RELEASE_OK_ERCD
#define USE_FUNC_WAIT_RELEASE_NG
#define USE_FUNC_WAIT_RELEASE_TMOUT
#define USE_FUNC_MAKE_WAIT
#define USE_FUNC_MAKE_WAIT_RELTIM
#define USE_FUNC_WAIT_DELETE
#define USE_FUNC_WAIT_TSKID
#define USE_FUNC_GCB_MAKE_WAIT
#define USE_FUNC_GCB_MAKE_WAIT_WITH_DISWAI	/* TK2 */
#define USE_FUNC_GCB_CHANGE_PRIORITY
#define USE_FUNC_GCB_TOP_OF_WAIT_QUEUE

/*  */
#define USE_FUNC_TK_GET_TSP			/* TK2 */
#define USE_FUNC_TK_SET_TSP			/* TK2 */
#define USE_FUNC_TK_SET_CPR			/* TK2 */
#define USE_FUNC_TK_GET_CPR			/* TK2 */

/* Interrupt Management Functions */
#define USE_FUNC_ENABLEINT
#define USE_FUNC_DISABLEINT
#define USE_FUNC_CLEARINT
#define USE_FUNC_CHECKINT
#define USE_FUNC_SETINTMODE

/* Cache Control */
#define USE_FUNC_FLUSHMEMCACHE		/* TK2:Internal */
#define USE_FUNC_SETCACHEMODE			/* TK2 */
#define USE_FUNC_CONTROLCACHE			/* TK2 */

/* Physical Timer */

/* Utilities */



#endif /* _TK_CONFIG_FUNC_H_ */
