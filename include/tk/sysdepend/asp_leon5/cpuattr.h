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
 *	@(#)cpuattr.h (tk/ASP-LEON5)
 *
 *	Type Specific Function
 */

#ifndef __TK_CPUATTR_H__
#define __TK_CPUATTR_H__

/*
 * Global pointer support
 *   0: No global pointer support
 */
#define TA_GP		0		/* No global pointer support */

/*
 * Using FPU (depend on CPU)
 *   0: not using FPU
 *   TA_COPn(n = 0-3): using FPU
 */
#define TA_FPU		0x00001000U	/* TA_COP0: Use coprocessor (ID=0) */

#endif /* __TK_CPUATTR_H__ */
