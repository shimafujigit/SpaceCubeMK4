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
 *	@(#)segment_depend.h (sys/ASP-LEON5)
 *
 *	Segment management system dependent definitions (LEON5)
 */

#ifndef __SYS_SEGMENT_DEPEND_H__
#define __SYS_SEGMENT_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Access level definitions */
typedef enum {
	MapUser		= 0x00,
	MapSystem	= 0x04,
	MapRead		= 0x00,
	MapWrite	= 0x01,
	MapExecute	= 0x02
} MapModeLevel;

/*
 * MapMemory() attr
 */
#define MM_USER		0x00U	/* User */
#define MM_SYSTEM	0x10U	/* System */
#define MM_READ		0x00U	/* Read */
#define MM_WRITE	0x04U	/* Write */
#define MM_EXECUTE	0x08U	/* Execute */
#define MM_CDIS		0x80U	/* Cache disabled */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SEGMENT_DEPEND_H__ */
