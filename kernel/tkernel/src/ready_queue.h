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
 *	ready_queue.h (T-Kernel/OS)
 *	Ready Queue Operation Routine
 */

#ifndef _READY_QUEUE_
#define _READY_QUEUE_

#include <libstr.h>
#include "bitop.h"

/*
 * Definition of ready queue structure
 *	In the ready queue, the task queue 'tskque' is provided per priority.
 *	The task TCB is registered onto queue with the applicable priority.
 *	For effective ready queue search, the bitmap area 'bitmap' is provided
 *	to indicate whether there are tasks in task queue per priority.
 *	
 *	Also, to search a task at the highest priority in the ready queue
 *    	effectively, put the highest task priority in the 'top_priority' field.
 *	If the ready queue is empty, set the value in this field to NUM_PRI.
 *	In this case, to return '0' with refering 'tskque[top_priority]',
 *      there is 'null' field which is always '0'.
 *
 *	Multiple READY tasks with kernel lock do not exist at the same time.
 */

#define BITMAPSZ	( sizeof(UW) * 8 )
#define NUM_BITMAP	( (NUM_PRI + BITMAPSZ - 1) / BITMAPSZ )

typedef	struct ready_queue {
	INT	top_priority;		/* Highest priority in ready queue */
	QUEUE	tskque[NUM_PRI];	/* Task queue per priority */
	TCB	*null;			/* When the ready queue is empty, */
	UW	bitmap[NUM_BITMAP];	/* Bitmap area per priority */
	TCB	*klocktsk;		/* READY task with kernel lock */
} RDYQUE;

IMPORT RDYQUE	ready_queue;

/*
 * Ready queue initialization
 */
IMPORT void ready_queue_initialize( RDYQUE *rq );

/*
 * Return the highest priority task in ready queue
 */
IMPORT TCB* ready_queue_top( RDYQUE *rq );

/*
 * Return the priority of the highest priority task in the ready queue
 */
IMPORT INT ready_queue_top_priority( const RDYQUE *rq );

/*
 * Insert task in ready queue
 *	Insert it at the end of the same priority tasks with task priority
 *	indicated with 'tcb'. Set the applicable bit in the bitmap area and
 *	update 'top_priority' if necessary. When updating 'top_priority,'
 *	return TRUE, otherwise FALSE.
 */
IMPORT BOOL ready_queue_insert( RDYQUE *rq, TCB *tcb );

/*
 * Insert task at head in ready queue
 */
IMPORT void ready_queue_insert_top( RDYQUE *rq, TCB *tcb );

/*
 * Delete task from ready queue
 *	Take out TCB from the applicable priority task queue, and if the task
 *	queue becomes empty, clear the applicable bit from the bitmap area.
 *	In addition, update 'top_priority' if the deleted task had the highest
 *	priority. In such case, use the bitmap area to search the second
 *	highest priority task.
 */
IMPORT void ready_queue_delete( RDYQUE *rq, TCB *tcb );

/*
 * Move the task, whose ready queue priority is 'priority', at head of
 * queue to the end of queue. Do nothing, if the queue is empty.
 */
IMPORT void ready_queue_rotate( RDYQUE *rq, INT priority );

/*
 * Put 'tcb' to the end of ready queue.
 */
IMPORT TCB* ready_queue_move_last( RDYQUE *rq, TCB *tcb );

#endif /* _READY_QUEUE_ */
