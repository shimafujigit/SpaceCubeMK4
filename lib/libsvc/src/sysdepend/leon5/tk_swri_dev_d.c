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
 *	T-Kernel extended SVC interface library (leon5)
 *
 *	   created from <tk/devmgr.h>
 */


#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/ifdevmgr.h>

#ifdef DEVICE_TK_SWRI_DEV_D_FN

EXPORT ER tk_swri_dev_d( ID dd, D start_d, CONST void *buf, W size, W *asize )
{
	volatile DEVICE_TK_SWRI_DEV_D_PARA	para;
	register int	o0	asm("%o0");
	register int	g2	asm("%g2") = DEVICE_TK_SWRI_DEV_D_FN;

	para.dd = dd;
	para.start_d = start_d;
	para.buf = buf;
	para.size = size;
	para.asize = asize;
	o0 = (int)&para;

	Asm("ta	%2": "+r"(o0): "r"(g2), "i"(TRAP_SVC));

	return o0;
}

#endif

