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
 *	@(#)ssid.inc (sys)
 *
 *	Subsystem ID and subsystem priority 
 *
 *	(NB) Included also from the assembler source,
 *	     so only macros can be described.
 */

#ifndef __SYS_SSID_H__
#define	__SYS_SSID_H__	1

/*
 * T-Kernel (1 - 9)
 *	1 - 6 and 9 were previously used for other purposes.
 */
.equ	CONSIO_SVC,	1		/* Console I/O */
.equ	CONSIO_PRI,		1
.equ	SERIAL_SVC,	5		/* Low-level serial I/O */
.equ	SERIAL_PRI,		1
.equ	DEVICE_SVC,	8		/* T-Kernel/SM device management */
.equ	DEVICE_PRI,		4
.equ	SYSTEM_SVC,	9		/* T-Kernel/SM system management */
.equ	SYSTEM_PRI,		1

/*
 * Subsystem (10 - )
 */
.equ	MM_SVC,		11		/* Memory management		*/
.equ	MM_PRI,			2
.equ	SM_SVC,		16		/* System management		*/
.equ	SM_PRI,			2
.equ	SEG_SVC,	17		/* Segment management		*/
.equ	SEG_PRI,		2

#endif /* __SYS_SSID_H__ */
