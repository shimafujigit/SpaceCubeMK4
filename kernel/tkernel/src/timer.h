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
 *	timer.h (T-Kernel/OS)
 *	Timer Module Definition
 */

#ifndef _TIMER_
#define _TIMER_

#include <longlong.h>

/*
 * conversion of milliseconds and microseconds
 */
#define to_msec(usec)	( ((usec) + (D)999) / 1000 ) /* round up */
#define to_usec(msec)	( (msec) * (D)1000 )

#define to_usec_tmo(ms)	( ( (ms) > 0 )? to_usec(ms): (ms) )

/*
 * SYSTIM internal expression and conversion
 */
typedef	longlong	LSYSTIM;	/* SYSTIM int. expression */

IMPORT LSYSTIM toLSYSTIM( SYSTIM *time );

IMPORT LSYSTIM toLSYSTIM( CONST SYSTIM *time );

IMPORT SYSTIM toSYSTIM( LSYSTIM ltime, UINT *us );

/*
 * Definition of timer event block
 */
typedef void	(*CBACK)(void*);	/* Type of callback function */

typedef struct timer_event_block {
	QUEUE	queue;		/* Timer event queue */
	LSYSTIM	time;		/* Event time */
	CBACK	callback;	/* Callback function */
	void	*arg;		/* Argument to be sent to callback function */
} TMEB;

/*
 * Timer interrupt interval (us)
 */
IMPORT RELTIM_U	TIMER_PERIOD;

/*
 * Current time (Software clock)
 */
IMPORT LSYSTIM	current_time;	/* System operation time */
IMPORT LSYSTIM	real_time_ofs;	/* Difference from actual time */

/* Actual time */
#define real_time()	( ll_add(current_time, real_time_ofs) )

/*
 * Timer initialization and stop
 */
IMPORT ER   timer_initialize( void );
IMPORT void timer_shutdown( void );

/*
 * Obtain the time for correction
 */
IMPORT RELTIM_U adjust_time( void );

/*
 * Register timer event onto timer queue
 */
IMPORT void timer_insert( TMEB *evt, TMO_U tmout, CBACK cback, void *arg );
IMPORT void timer_insert_reltim( TMEB *event, RELTIM_U tmout, CBACK callback, void *arg );
IMPORT void timer_insert_abs( TMEB *evt, LSYSTIM time, CBACK cback, void *arg );

/*
 * Delete from timer queue
 */
IMPORT void timer_delete( TMEB *event );

#endif /* _TIMER_ */
