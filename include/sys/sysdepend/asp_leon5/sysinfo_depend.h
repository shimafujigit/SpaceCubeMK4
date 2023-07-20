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
 *	@(#)sysinfo_depend.h (sys/ASP-LEON5)
 *
 *	System common information 
 *
 */

#ifndef __SYS_SYSINFO_DEPEND_H__
#define __SYS_SYSINFO_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _in_asm_source_

/*
 * Boot mode/Operation mode 
 */
typedef union {
	struct {
#if BIGENDIAN
		UW	rsv2:17;	/* Reserved (always 0) */
		UW	basic:1;	/* When in basic operation mode 1 */
		UW	rsv:12;		/* Reserved (always 0) */
		UW	fsrcv:1;	/* When in disk repair mode 1 */
		UW	debug:1;	/* When in debug mode 1 */
#else
		UW	debug:1;	/* When in debug mode 1 */
		UW	fsrcv:1;	/* When in disk repair mode 1 */
		UW	rsv:12;		/* Reserved (always 0) */
		UW	basic:1;	/* When in basic operation mode 1 */
		UW	rsv2:17;	/* Reserved (always 0) */
#endif
	} c;
	UW	w;
} BootMode;

#define BM_DEBUG	0x00000001U	/* Debug mode */
#define BM_FSRCV	0x00000002U	/* Disk repair mode */
#define BM_BASIC	0x00004000U	/* Basic operation mode */

/*
 * System common information 
 */
typedef struct {
	void 		*ramtop;	/* RAM free space top */
	void		*ramend;	/* RAM free space end */
	UB		*sysconf;	/* SYSCONF top */
	UB		*devconf;	/* DEVCONF top */
	W		taskindp;	/* Task independent flag */
	UW		loop64us;	/* Loop count per 64 micro sec */
	BootMode	bm;		/* Boot mode */
	VW		rsv[9];		/* Reserved (always zero) */
} SysCommonInfo;

/*
 * System common area 
 */
typedef struct {
	SysCommonInfo	scinfo;			/* System common information */
} SysCommonArea;

IMPORT SysCommonArea	syscommonarea;
#define SCArea	( &syscommonarea )
#define SCInfo	( SCArea->scinfo )

#endif /* _in_asm_source_ */

/*
 * Vector table
 */
#define	EIT_FP_DISABLED		0x04
#define	EIT_TRAP(n)	( 0x80 + (n) )	/* TRAP (n=0x00¡Á0x7f) */
#define	EIT_INT(n)	( 0x10 + (n) )	/* Interrupt (n=0x01¡Á0x0f) */

#define	N_INTVEC	256		/* Vector table size */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SYSINFO_DEPEND_H__ */
