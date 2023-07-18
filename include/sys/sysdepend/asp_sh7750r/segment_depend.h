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
 *	@(#)segment_depend.h (sys/ASP-SH7750R)
 *
 *	Segment management system dependent definitions (SH7750R)
 */

#ifndef __SYS_SEGMENT_DEPEND_H__
#define __SYS_SEGMENT_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Access level definitions */
typedef enum {
	MapUser		= 0x02,
	MapSystem	= 0x00,
	MapRead		= 0x00,
	MapWrite	= 0x01,
	MapExecute	= 0x00
} MapModeLevel;

/*
 * MapMemory() attr
 */
#define MM_USER		0x40U	/* User */
#define MM_SYSTEM	0x00U	/* System */
#define MM_READ		0x00U	/* Read */
#define MM_WRITE	0x20U	/* Write */
#define MM_EXECUTE	0x00U	/* Execute */
#define MM_CDIS		0x08U	/* Cache disabled */

#ifdef __cplusplus
}
#endif
#endif /* __SYS_SEGMENT_DEPEND_H__ */
