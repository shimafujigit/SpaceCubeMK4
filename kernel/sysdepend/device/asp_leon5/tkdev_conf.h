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
 *	tkdev_conf.h (ASP-LEON5)
 *	Target System Configuration 
 */

#ifndef _TKDEV_CONF_
#define _TKDEV_CONF_

#include <tk/sysdef.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/gptimer.h>

/*
 * GPTIMER
 */
#define	GPTCLK_MHz	1			/* 1 MHz */
#define	GPTIRQ		__bcc_timer_interrupt

#define	TIMER_INTLEVEL	GPTIRQ			/* Interrupt level */
#define	TIMER_INTVECNO	EIT_INT(GPTIRQ)		/* Interrupt vector */

#endif /* _TKDEV_CONF_ */
