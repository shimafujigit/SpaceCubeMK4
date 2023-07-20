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
 *	icrt0.S (ASP-LEON5)
 *	System Startup
 */

#include "machine.h"

	.text
	.balign	4
	.globl	Csym(KernelStart)
	.type	Csym(KernelStart), #function
Csym(KernelStart):
/*
 * Use the stack set by the boot loader
 *
 * ROM system startup
 */
	call	Csym(ROM_startup)
	 nop

/*
 * Main system startup
 */
	call	Csym(sysmain)		// System start up
	 nop
l_end:					// Not suppose to return from 'main,'
	ba,a	l_end			// Safty guard
