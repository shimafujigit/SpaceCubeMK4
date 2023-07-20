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
 *	@(#)tmon.inc (libtm)
 *
 *	T-Monitor library common definitions 
 *
 *	Also accessed from the assembler 
 */

#include <machine.inc>
#include "sysdepend/asp_leon5/tmsvc.inc"
#include <tk/errno.inc>

/*
 * Function code 
 */
.equ	TM_MONITOR,	0
.equ	TM_GETCHAR,	1
.equ	TM_PUTCHAR,	2
.equ	TM_GETLINE,	3
.equ	TM_PUTSTRING,	4
.equ	TM_COMMAND,	5
.equ	TM_READDISK,	6
.equ	TM_WRITEDISK,	7
.equ	TM_INFODISK,	8
.equ	TM_EXIT,	9

.equ	TM_EXTSVC,	255
