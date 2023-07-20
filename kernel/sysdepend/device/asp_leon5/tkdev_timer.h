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
 *	tkdev_timer.h (ASP-LEON5)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/syslib.h>
#include <sys/sysinfo.h>

/*
 * Settable interval range (micro seconds)
 */
#define MIN_TIMER_PERIOD	1000
#define MAX_TIMER_PERIOD	50000

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
IMPORT void start_hw_timer( void );

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
 *
 * void clear_hw_timer_interrupt( void )
 * void end_of_hw_timer_interrupt( void )
 */
IMPORT void clear_hw_timer_interrupt( void );
#define	end_of_hw_timer_interrupt()	/* nothing */

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
IMPORT void terminate_hw_timer( void );

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
IMPORT UINT get_hw_timer_nsec( void );
IMPORT UINT get_hw_timer_usec( void );

#endif /* _TKDEV_TIMER_ */
