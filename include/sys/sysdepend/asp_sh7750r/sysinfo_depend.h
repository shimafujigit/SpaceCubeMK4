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
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)sysinfo_depend.h (sys/ASP-SH7750R)
 *
 *	System common information 
 *
 *   0x84001000	+-----------------------+
 *		|Vector table of	| (EXPEVT  / 0x20 * 4) + 0x84001000
 *		|exception codes and	| (INTEVT2 / 0x20 * 4) + 0x84001000
 *		|interrupt factors	|
 *   0x840011c0	+-----------------------+
 *		|TRAPA vector table	| TRA + 0x84001000
 *		|  TRAPA 0x70 - 0x7f	|
 *   0x84001200	+-----------------------+
 *		|Default handler	|
 *   0x84001204	+-----------------------+
 *		|TLB miss exception	| VBR + 0x400 exception handler
 *		|handler		|
 *   0x84001208	+-----------------------+
 *		|System common info.	|
 *   0x84001280	+-----------------------+
 *		|EIT stack		| (640 byte)
 *   0x84001500	+-----------------------+
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
	VW		rsv[20];	/* Reserved (always 0) */
	UB		bootdev[8];	/* Boot device name */
	BootMode	bm;		/* Boot mode */
	UW		loop64us;	/* Loop count per 64 micro sec */
	UB		*sysconf;	/* SYSCONF top */
	UB		*devconf;	/* DEVCONF top */
	FP		chkmem;		/* Monitor memory check function */
	UH		Iclk;		/* SH7750R internal clock (MHz) */
	UH		Pclk;		/* SH7750R peripheral clock (MHz) */
	void 		*ramtop;		/* RAM free space top */
	void		*ramend;		/* RAM free space end */
} SysCommonInfo;

/*
 * System common area 
 */
#define N_INTVEC	128
#define N_INTSTACK	160
typedef struct {
	FP		intvec[N_INTVEC];	/* EIT vector */
	FP		defaulthdr;		/* Default handler */
	FP		tlbmisshdr;		/* TLB miss exception handler */
	SysCommonInfo	scinfo;			/* System common information */
	UW		intstack[N_INTSTACK];	/* Interrupt stack area */
} SysCommonArea;

#define SCArea	( (SysCommonArea*)0x84001000 )
#define SCInfo	( SCArea->scinfo )

#endif /* _in_asm_source_ */

/*
 * Vector table addresses 
 */
#define VECTBL		0x84001000	/* Vector table top */
#define DEFAULTHDR	0x84001200	/* Default handler */
#define TLBMISSHDR	0x84001204	/* TLB miss exception handler */
#define	SCINFO_RAMTOP	0x84001278	/* SCInfo.ramtop */
#define	SCINFO_RAMEND	0x8400127c	/* SCInfo.ramend */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SYSINFO_DEPEND_H__ */
