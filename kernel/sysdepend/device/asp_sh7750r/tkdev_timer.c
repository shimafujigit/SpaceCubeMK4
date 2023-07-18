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
 *	tkdev_timer.c (SH7750R)
 *	Hardware-Dependent Timer Processing
 */

#include "kernel.h"
#include "timer.h"
#include "task.h"
#include "tkdev_timer.h"

/*
 * Set timer
 */
#pragma inline(init_hw_timer)
static void init_hw_timer( void )
{
	UB	tstr;
	UW	cnt;

	/* Stop timer */
	tstr = in_b(TSTR) & ~TSTR_STR0;
	out_b(TSTR, tstr);

	/* Set timer mode */
	out_h(TCR0, TCR0_INIT);

	/* Set counter */
	cnt = TIMER_PERIOD * (UW)SCInfo.Pclk / 4 - 1;
	out_w(TCOR0, cnt);
	out_w(TCNT0, cnt);

	/* Start timer count */
	out_b(TSTR, tstr | TSTR_STR0);
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
#pragma inline(start_hw_timer)
static void start_hw_timer( void )
{
IMPORT	void	timer_handler_startup( void );
	UH	ipri;

	/* Set timer */
	init_hw_timer();

	/* Interrupt handler definition */
	define_inthdr(VECNO_TMU0, (FP)timer_handler_startup);

	/* Timer interrupt enable */
	ipri = in_h(IPRA);
	ipri = (ipri & 0x0fff) | (TIMER_INTLEVEL << 12);
	out_h(IPRA, ipri);
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both depending on hardware.
 */
#pragma inline(clear_hw_timer_interrupt)
static void clear_hw_timer_interrupt( void )
{
	/* Clear underflow flag */
	out_h(TCR0, TCR0_INIT);
}
#pragma inline(end_of_hw_timer_interrupt)
static void end_of_hw_timer_interrupt( void )
{
	/* Nothing required to do at this point */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
#pragma inline(terminate_hw_timer)
static void terminate_hw_timer( void )
{
	UH	ipri;

	/* Timer interrupt disable */
	ipri = in_h(IPRA);
	ipri = ipri & 0x0fff;
	out_h(IPRA, ipri);
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
#pragma inline(get_hw_timer_nsec)
static UINT get_hw_timer_nsec( void )
{
	UW	ofs, max;
	UH	unf;

	max = in_w(TCOR0) + 1;
	do {
		unf = in_h(TCR0) & TCR_UNF;
		ofs = max - in_w(TCNT0);
	} while ( unf != (in_h(TCR0) & TCR_UNF) );
	if ( unf != 0 ) {
		ofs += max;
	}

	return ofs * 1000 / SCInfo.Pclk * 4;
}

#pragma inline(get_hw_timer_usec)
static UINT get_hw_timer_usec( void )
{
	UW	ofs, max;
	UH	unf;

	max = in_w(TCOR0) + 1;
	do {
		unf = in_h(TCR0) & TCR_UNF;
		ofs = max - in_w(TCNT0);
	} while ( unf != (in_h(TCR0) & TCR_UNF) );
	if ( unf != 0 ) {
		ofs += max;
	}

	return ofs * 4 / SCInfo.Pclk;
}

