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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)rominfo_depend.h (sys/ASP-LEON5)
 *
 *	ROM information 
 */

#ifndef __SYS_ROMINFO_DEPEND_H__
#define __SYS_ROMINFO_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ROM information 
 */
typedef struct {
	FP	kernel;		/* Kernel startup address */
	UB	*sysconf;	/* SYSCONF top */
	UB	*devconf;	/* DEVCONF top */
	void	*userarea;	/* RAM user area top */
	FP	userinit;	/* User initialization program address */
	FP	resetinit;	/* Reset initialization program address */
	UW	dbgport_bps;	/* Speed of Debug serial port (bps) */
	VW	rsv[9];		/* Reserved (always zero) */
	UW	rd_type;	/* ROM disk type */
	UW	rd_blksz;	/* ROM disk block size */
	UW	rd_saddr;	/* ROM disk start address */
	UW	rd_eaddr;	/* ROM disk end address */
	VW	rsv2[12];	/* Reserved (always zero) */
} RomInfo;

/* Start address of ROMInfo */
IMPORT const RomInfo rominfo;
#define ROMInfo	( &rominfo )

/* common tag in DEVCONF[] --> config/src/sysdepend/<TARGET>/DEVCONF */
#define DCTAG_DEBUGMODE	((UB*)"DEBUGMODE")	/* Debug mode (1:dbg 0:nrm) */

/* common tag in SYSCONF[] --> config/src/sysdepend/<TARGET>/SYSCONF */
/* Product information */
#define SCTAG_TSYSNAME	((UB*)"TSysName")	/* System name */

#define SCTAG_MAKER	((UB*)"Maker")		/* Maker Code */
#define SCTAG_PRODUCTID	((UB*)"ProductID")	/* Kernel Identifier */
#define SCTAG_SPECVER	((UB*)"SpecVer")	/* Specification Version */
#define SCTAG_PRODUCTVER ((UB*)"ProductVer")	/* Product Version */
#define SCTAG_PRODUCTNO	((UB*)"ProductNo")	/* Product Number [0]-[3] */

/* T-Kernel/OS */
#define SCTAG_TMAXTSKID	((UB*)"TMaxTskId")	/* Max task ID */
#define SCTAG_TMAXSEMID	((UB*)"TMaxSemId")	/* Max semaphore ID */
#define SCTAG_TMAXFLGID	((UB*)"TMaxFlgId")	/* Max event flag ID */
#define SCTAG_TMAXMBXID	((UB*)"TMaxMbxId")	/* Max mail box ID */
#define SCTAG_TMAXMTXID	((UB*)"TMaxMtxId")	/* Max mutex ID */
#define SCTAG_TMAXMBFID	((UB*)"TMaxMbfId")	/* Max message buffer ID */
#define SCTAG_TMAXPORID	((UB*)"TMaxPorId")	/* Max rendezvous port ID */
#define SCTAG_TMAXMPFID	((UB*)"TMaxMpfId")	/* Max fix-size mem pool ID */
#define SCTAG_TMAXMPLID	((UB*)"TMaxMplId")	/* Max var-size mem pool ID */
#define SCTAG_TMAXCYCID	((UB*)"TMaxCycId")	/* Max cyclic handler ID */
#define SCTAG_TMAXALMID	((UB*)"TMaxAlmId")	/* Max alarm handler ID */
#define SCTAG_TMAXRESID	((UB*)"TMaxResId")	/* Max resource group ID */
#define SCTAG_TMAXSSYID	((UB*)"TMaxSsyId")	/* Max sub system ID */
#define SCTAG_TMAXSSYPRI ((UB*)"TMaxSsyPri")	/* Max sub system priority */

#define SCTAG_TSYSSTKSZ	((UB*)"TSysStkSz")	/* Default sys stack sz (b) */
#define SCTAG_TSVCLIMIT	((UB*)"TSVCLimit")	/* SVC protection level */
#define SCTAG_TTIMPERIOD ((UB*)"TTimPeriod")	/* Timer interval (msec) */


/* T-Kernel/SM */				/* Max number of ... */
#define SCTAG_TMAXREGDEV ((UB*)"TMaxRegDev")	/* devices registration */
#define SCTAG_TMAXOPNDEV ((UB*)"TMaxOpnDev")	/* devices open */
#define SCTAG_TMAXREQDEV ((UB*)"TMaxReqDev")	/* device requests */

#define SCTAG_TDEVTMBFSZ ((UB*)"TDEvtMbfSz")	/* Event notification message */
						/* buffer size (b), */
						/* Max length of message (b) */

/* Task Event(1-8) */				/* Message management */
#define SCTAG_TEV_MSGEVT ((UB*)"TEV_MsgEvt")	/* Recieve message */
#define SCTAG_TEV_MSGBRK ((UB*)"TEV_MsgBrk")	/* Message waiting state */

#define SCTAG_TEV_GDI	 ((UB*)"TEV_GDI")	/* GDI interface */
#define SCTAG_TEV_FFLOCK ((UB*)"TEV_FFLock")	/* FIFO lock waiting state */

/* Segment manager */
#define SCTAG_REALMEMEND ((UB*)"RealMemEnd")	/* RAM bottom address */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_ROMINFO_DEPEND_H__ */
