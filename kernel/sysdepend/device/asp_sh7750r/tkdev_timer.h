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
 *	tkdev_timer.h (SH7750R)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

/*
 * Settable interval range (micro seconds)
 */
#define MIN_TIMER_PERIOD	1000
#define MAX_TIMER_PERIOD	50000

/*
 * Timer control register setting value
 *	Interrupt enable, 16 dividing
 */
#define TCR0_INIT	( TCR_UNIE | TCR_TPSC_P4 )

/*
 * Set timer
 */
IMPORT void init_hw_timer( void );

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
 */
IMPORT void clear_hw_timer_interrupt( void );
IMPORT void end_of_hw_timer_interrupt( void );

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
