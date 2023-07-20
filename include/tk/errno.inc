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
 *	@(#)errno.inc (T-Kernel)
 *
 *	T-Kernel Error Code
 */

#ifndef __TK_ERRNO_H__
#define	__TK_ERRNO_H__	1

.equ	E_OK,		0		/* Completed successfully */

.equ	E_SYS,		(( -5 << 16) | (0 & 0xffff))	/* System error */
.equ	E_NOCOP,	(( -6 << 16) | (0 & 0xffff))	/* Coprocessor disable */
.equ	E_NOSPT,	(( -9 << 16) | (0 & 0xffff))	/* Unsupported function */
.equ	E_RSFN,		((-10 << 16) | (0 & 0xffff))	/* Reserved function code number */
.equ	E_RSATR,	((-11 << 16) | (0 & 0xffff))	/* Reserved attribute */
.equ	E_PAR,		((-17 << 16) | (0 & 0xffff))	/* Parameter error */
.equ	E_ID,		((-18 << 16) | (0 & 0xffff))	/* Incorrect ID number */
.equ	E_CTX,		((-25 << 16) | (0 & 0xffff))	/* Context error */
.equ	E_MACV,		((-26 << 16) | (0 & 0xffff))	/* Inaccessible memory/access violation */
.equ	E_OACV,		((-27 << 16) | (0 & 0xffff))	/* Object access violation */
.equ	E_ILUSE,	((-28 << 16) | (0 & 0xffff))	/* Incorrect system call use */
.equ	E_NOMEM,	((-33 << 16) | (0 & 0xffff))	/* Insufficient memory */
.equ	E_LIMIT,	((-34 << 16) | (0 & 0xffff))	/* Exceed system limits */
.equ	E_OBJ,		((-41 << 16) | (0 & 0xffff))	/* Incorrect object state */
.equ	E_NOEXS,	((-42 << 16) | (0 & 0xffff))	/* Object does not exist */
.equ	E_QOVR,		((-43 << 16) | (0 & 0xffff))	/* Queuing overflow */
.equ	E_RLWAI,	((-49 << 16) | (0 & 0xffff))	/* Forcibly release wait state */
.equ	E_TMOUT,	((-50 << 16) | (0 & 0xffff))	/* Polling fail/time out */
.equ	E_DLT,		((-51 << 16) | (0 & 0xffff))	/* Waited object was deleted */
.equ	E_DISWAI,	((-52 << 16) | (0 & 0xffff))	/* Release wait caused by wait disable */

.equ	E_IO,		((-57 << 16) | (0 & 0xffff))	/* Output/input error */
.equ	E_NOMDA,	((-58 << 16) | (0 & 0xffff))	/* No media */
.equ	E_BUSY,		((-65 << 16) | (0 & 0xffff))	/* Busy state */
.equ	E_ABORT,	((-66 << 16) | (0 & 0xffff))	/* Aborted */
.equ	E_RONLY,	((-67 << 16) | (0 & 0xffff))	/* write protected */

#endif /* __TK_ERRNO_H__ */
