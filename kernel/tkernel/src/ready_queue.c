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
 *	ready_queue.c (T-Kernel/OS)
 *	Ready Queue Operation Routine
 */

#include <basic.h>
#include "kernel.h"
#include "task.h"
#include "ready_queue.h"

/*
 * Ready queue initialization
 */
#pragma inline(ready_queue_initialize)
static void ready_queue_initialize( RDYQUE *rq )
{
	INT	i;

	rq->top_priority = NUM_PRI;
	for ( i = 0; i < NUM_PRI; i++ ) {
		QueInit(&rq->tskque[i]);
	}
	rq->null = NULL;
	rq->klocktsk = NULL;
	memset(rq->bitmap, 0, sizeof(rq->bitmap));
}

/*
 * Return the highest priority task in ready queue
 */
#pragma inline(ready_queue_top)
static TCB* ready_queue_top( RDYQUE *rq )
{
	/* If there is a task at kernel lock, that is the highest priority task */
	if ( rq->klocktsk != NULL ) {
		return rq->klocktsk;
	}

	return (TCB*)rq->tskque[rq->top_priority].next;
}

/*
 * Return the priority of the highest priority task in the ready queue
 */
#pragma inline(ready_queue_top_priority)
static INT ready_queue_top_priority( const RDYQUE *rq )
{
	return rq->top_priority;
}

/*
 * Insert task in ready queue
 *	Insert it at the end of the same priority tasks with task priority 
 *	indicated with 'tcb'. Set the applicable bit in the bitmap area and 
 *	update 'top_priority' if necessary. When updating 'top_priority,' 
 *	return TRUE, otherwise FALSE.
 */
#pragma inline(ready_queue_insert)
static BOOL ready_queue_insert( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;

	tcb->slicecnt = 0;

	QueInsert(&tcb->tskque, &rq->tskque[priority]);
	tstdlib_bitset(rq->bitmap, priority);

	if ( tcb->klocked ) {
		rq->klocktsk = tcb;
	}

	if ( priority < rq->top_priority ) {
		rq->top_priority = priority;
		return TRUE;
	}
	return FALSE;
}

/*
 * Insert task at head in ready queue 
 */
#pragma inline(ready_queue_insert_top)
static void ready_queue_insert_top( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;

	QueInsert(&tcb->tskque, rq->tskque[priority].next);
	tstdlib_bitset(rq->bitmap, priority);

	if ( tcb->klocked ) {
		rq->klocktsk = tcb;
	}

	if ( priority < rq->top_priority ) {
		rq->top_priority = priority;
	}
}

/*
 * Delete task from ready queue
 *	Take out TCB from the applicable priority task queue, and if the task 
 *	queue becomes empty, clear the applicable bit from the bitmap area.
 *	In addition, update 'top_priority' if the deleted task had the highest 
 *	priority. In such case, use the bitmap area to search the second
 *	highest priority task.
 */
#pragma inline(ready_queue_delete)
static void ready_queue_delete( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;
	INT	i;

	if ( rq->klocktsk == tcb ) {
		rq->klocktsk = NULL;
	}

	QueRemove(&tcb->tskque);
	if ( tcb->klockwait ) {
		/* Delete from kernel lock wait queue */
		tcb->klockwait = FALSE;
		return;
	}
	if ( !isQueEmpty(&rq->tskque[priority]) ) {
		return;
	}

	tstdlib_bitclr(rq->bitmap, priority);
	if ( priority != rq->top_priority ) {
		return;
	}

	i = tstdlib_bitsearch1_binsearch(rq->bitmap, priority, NUM_PRI - priority);
	if ( i >= 0 ) {
		rq->top_priority = priority + i;
	} else {
		rq->top_priority = NUM_PRI;
	}
}

/*
 * Move the task, whose ready queue priority is 'priority', at head of
 * queue to the end of queue. Do nothing, if the queue is empty.
 */
#pragma inline(ready_queue_rotate)
static void ready_queue_rotate( RDYQUE *rq, INT priority )
{
	QUEUE	*tskque = &rq->tskque[priority];
	TCB	*tcb;

	tcb = (TCB*)QueRemoveNext(tskque);
	if ( tcb != NULL ) {
		QueInsert((QUEUE*)tcb, tskque);

		tcb->slicecnt = 0;
	}
}

/*
 * Put 'tcb' to the end of ready queue. 
 */
#pragma inline(ready_queue_move_last)
static TCB* ready_queue_move_last( RDYQUE *rq, TCB *tcb )
{
	QUEUE	*tskque = &rq->tskque[tcb->priority];

	QueRemove(&tcb->tskque);
	QueInsert(&tcb->tskque, tskque);

	tcb->slicecnt = 0;

	return (TCB*)tskque->next;	/* New task at head of queue */
}

