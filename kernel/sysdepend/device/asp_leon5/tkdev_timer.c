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
 *	tkdev_timer.c (ASP-LEON5)
 *	Hardware-Dependent Timer Processing
 */

#include "kernel.h"
#include "timer.h"
#include "task.h"
#include "tkdev_timer.h"
#include "tkdev_conf.h"

LOCAL volatile struct gptimer_timer_regs	*tmr;

/*
 * Set timer
 */
LOCAL void init_hw_timer( void )
{
	if ( __bcc_timer_handle == 0 ||
	     __bcc_timer_interrupt == 0 ) return;
	tmr = &((struct gptimer_regs*)__bcc_timer_handle)->timer[0];

	/* stop timer */
	tmr->ctrl = 0;

	/* Set counter */
	tmr->counter = 0;
	tmr->reload = GPTCLK_MHz * TIMER_PERIOD - 1;

	/* Start counting */
	tmr->ctrl =
		GPTIMER_CTRL_IP |
		GPTIMER_CTRL_IE |
		GPTIMER_CTRL_LD |
		GPTIMER_CTRL_RS |
		GPTIMER_CTRL_EN;
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
EXPORT void start_hw_timer( void )
{
IMPORT	void	timer_handler_startup( void );

	/* Set timer */
	init_hw_timer();

	/* Interrupt handler definition */
	define_inthdr(TIMER_INTVECNO, (FP)timer_handler_startup);

	/* Timer interrupt enable */
	EnableInt(GPTIRQ);
}

/*
 * Clear timer interrupt
 */
EXPORT void clear_hw_timer_interrupt( void )
{
	tmr->ctrl = tmr->ctrl;
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
EXPORT void terminate_hw_timer( void )
{
	/* Stop timer interrupt */
	DisableInt(GPTIRQ);

	/* Stop counting */
	tmr->ctrl = 0;
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
EXPORT UINT get_hw_timer_nsec( void )
{
	UW	ofs, max;
	UW	unf;

#define	TimerUNF()	(tmr->ctrl & GPTIMER_CTRL_IP)

	max = tmr->reload;
	do {
		unf = TimerUNF();
		ofs = tmr->counter;
	} while ( unf != TimerUNF() );
	ofs = max - ofs;
	if ( unf != 0 ) ofs += max + 1;

	return ofs * 100 / GPTCLK_MHz * 10;
}

EXPORT UINT get_hw_timer_usec( void )
{
	UW	ofs, max;
	UW	unf;

	max = tmr->reload;
	do {
		unf = TimerUNF();
		ofs = tmr->counter;
	} while ( unf != TimerUNF() );
	ofs = max - ofs;
	if ( unf != 0 ) ofs += max + 1;

	return (ofs + 1) / GPTCLK_MHz;
}
