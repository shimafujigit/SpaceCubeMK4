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
 *    Changed by UC Technology at 2013/01/29.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	wait.c (T-Kernel/OS)
 *	Common Routine for Synchronization
 */

#include "kernel.h"
#include "task.h"
#include "wait.h"

#if defined(USE_FUNC_WAIT_RELEASE_TMOUT)||defined(USE_FUNC_WAIT_RELEASE_OK)||defined(USE_FUNC_WAIT_RELEASE_OK_ERCD)||defined(USE_FUNC_WAIT_RELEASE_NG)||defined(USE_FUNC_WAIT_DELETE)
/*
 * Update the task state to release wait. When it becomes ready state,
 * connect to the ready queue.
 * Call when the task is in the wait state (including double wait).
 */
#pragma inline(make_non_wait)
static void make_non_wait( TCB *tcb )
{
	if ( tcb->state == TS_WAIT ) {
		make_ready(tcb);
	} else {
		tcb->state = TS_SUSPEND;
	}
}
#endif /* USE_FUNC_WAIT_RELEASE_TMOUT or USE_FUNC_WAIT_RELEASE_OK or USE_FUNC_WAIT_RELEASE_OK_ERCD or USE_FUNC_WAIT_RELEASE_NG or USE_FUNC_WAIT_DELETE */

#if defined(USE_FUNC_WAIT_RELEASE_OK)||defined(USE_FUNC_WAIT_RELEASE_OK_ERCD)||defined(USE_FUNC_WAIT_RELEASE_NG)||defined(USE_FUNC_WAIT_DELETE)
/*
 * Release wait state of the task.
 */
#pragma inline(wait_release)
static void wait_release( TCB *tcb )
{
	timer_delete(&tcb->wtmeb);
	QueRemove(&tcb->tskque);
	make_non_wait(tcb);
}
#endif /* USE_FUNC_WAIT_RELEASE_OK or USE_FUNC_WAIT_RELEASE_OK_ERCD or USE_FUNC_WAIT_RELEASE_NG or USE_FUNC_WAIT_DELETE */

#if defined(USE_FUNC_WAIT_RELEASE_OK)
EXPORT void wait_release_ok( TCB *tcb )
{
	wait_release(tcb);
	*tcb->wercd = E_OK;
}
#endif /* USE_FUNC_WAIT_RELEASE_OK */

#if defined(USE_FUNC_WAIT_RELEASE_OK_ERCD)
EXPORT void wait_release_ok_ercd( TCB *tcb, ER ercd )
{
	wait_release(tcb);
	*tcb->wercd = ercd;
}
#endif /* USE_FUNC_WAIT_RELEASE_OK_ERCD */

#if defined(USE_FUNC_WAIT_RELEASE_NG)
EXPORT void wait_release_ng( TCB *tcb, ER ercd )
{
	wait_release(tcb);
	if ( tcb->wspec->rel_wai_hook != NULL ) {
		(*tcb->wspec->rel_wai_hook)(tcb);
	}
	*tcb->wercd = ercd;
}
#endif /* USE_FUNC_WAIT_RELEASE_NG */

#if defined(USE_FUNC_WAIT_RELEASE_TMOUT)
EXPORT void wait_release_tmout( TCB *tcb )
{
	QueRemove(&tcb->tskque);
	make_non_wait(tcb);
	if ( tcb->wspec->rel_wai_hook != NULL ) {
		(*tcb->wspec->rel_wai_hook)(tcb);
	}
}
#endif /* USE_FUNC_WAIT_RELEASE_TMOUT */

#if defined(USE_FUNC_MAKE_WAIT)
/*
 * Change the active task state to wait state and connect to the
 * timer event queue.
 *	Normally, 'ctxtsk' is in the RUN state, but when an interrupt
 *	occurs during executing system call, 'ctxtsk' may become the
 *	other state by system call called in the interrupt handler.
 *	However, it does not be in WAIT state.
 *
 *	"include/tk/typedef.h"
 *	typedef	INT		TMO;
 *	typedef UINT		RELTIM;
 *	#define TMO_FEVR	(-1)
 */
EXPORT void make_wait( TMO_U tmout, ATR atr )
{
	switch ( ctxtsk->state ) {
	  case TS_READY:
		make_non_ready(ctxtsk);
		ctxtsk->state = TS_WAIT;
		break;
	  case TS_SUSPEND:
		ctxtsk->state = TS_WAITSUS;
		break;
	}
	ctxtsk->nodiswai = ( (atr & TA_NODISWAI) != 0 )? TRUE: FALSE;
	timer_insert(&ctxtsk->wtmeb, tmout, (CBACK)wait_release_tmout, ctxtsk);
}
#endif /* USE_FUNC_MAKE_WAIT */

#if defined(USE_FUNC_MAKE_WAIT_RELTIM)
EXPORT void make_wait_reltim( RELTIM_U tmout, ATR atr )
{
	switch ( ctxtsk->state ) {
	  case TS_READY:
		make_non_ready(ctxtsk);
		ctxtsk->state = TS_WAIT;
		break;
	  case TS_SUSPEND:
		ctxtsk->state = TS_WAITSUS;
		break;
	}
	ctxtsk->nodiswai = ( (atr & TA_NODISWAI) != 0 )? TRUE: FALSE;
	timer_insert_reltim(&ctxtsk->wtmeb, tmout, (CBACK)wait_release_tmout, ctxtsk);
}
#endif /* USE_FUNC_MAKE_WAIT_RELTIM */

#if defined(USE_FUNC_WAIT_DELETE)
/*
 * Release all tasks connected to the wait queue, and define it
 * as E_DLT error.
 */
EXPORT void wait_delete( QUEUE *wait_queue )
{
	TCB	*tcb;

	while ( !isQueEmpty(wait_queue) ) {
		tcb = (TCB*)wait_queue->next;
		wait_release(tcb);
		*tcb->wercd = E_DLT;
	}
}
#endif /* USE_FUNC_WAIT_DELETE */

#if defined(USE_FUNC_WAIT_TSKID)
/*
 * Get ID of the head task in the wait queue.
 */
EXPORT ID wait_tskid( QUEUE *wait_queue )
{
	if ( isQueEmpty(wait_queue) ) {
		return 0;
	}

	return ((TCB*)wait_queue->next)->tskid;
}
#endif /* USE_FUNC_WAIT_TSKID */

#if defined(USE_FUNC_GCB_MAKE_WAIT)
/*
 * Change the active task state to wait state and connect to the timer wait
 * queue and the object wait queue. Also set 'wid' in 'ctxtsk'.
 */
EXPORT void gcb_make_wait( GCB *gcb, TMO_U tmout )
{
	ctxtsk->wid = gcb->objid;
	make_wait(tmout, gcb->objatr);
	if ( (gcb->objatr & TA_TPRI) != 0 ) {
		queue_insert_tpri(ctxtsk, &gcb->wait_queue);
	} else {
		QueInsert(&ctxtsk->tskque, &gcb->wait_queue);
	}
}
#endif /* USE_FUNC_GCB_MAKE_WAIT */

#if defined(USE_FUNC_GCB_MAKE_WAIT_WITH_DISWAI)
/*
 * 'gcb_make_wait' with wait disable check function
 */
EXPORT void gcb_make_wait_with_diswai( GCB *gcb, TMO_U tmout )
{
	if ( is_diswai(gcb, ctxtsk, ctxtsk->wspec->tskwait) ) {
		*ctxtsk->wercd = E_DISWAI;
	} else {
		*ctxtsk->wercd = E_TMOUT;
		if ( tmout != TMO_POL ) {
			gcb_make_wait(gcb, tmout);
		}
	}
}
#endif /* USE_FUNC_GCB_MAKE_WAIT_WITH_DISWAI */

#if defined(USE_FUNC_GCB_CHANGE_PRIORITY)
/*
 * When the task priority changes, adjust the task position at the wait queue.
 * It is called only if the object attribute TA_TPRI is specified.
 *
 */
EXPORT void gcb_change_priority( GCB *gcb, TCB *tcb )
{
	QueRemove(&tcb->tskque);
	queue_insert_tpri(tcb, &gcb->wait_queue);
}
#endif /* USE_FUNC_GCB_CHANGE_PRIORITY */

#if defined(USE_FUNC_GCB_TOP_OF_WAIT_QUEUE)
/*
 * Search the first task of wait queue include "tcb" with target.
 * (Not insert "tcb" into wait queue.)
 *
 */
EXPORT TCB* gcb_top_of_wait_queue( GCB *gcb, TCB *tcb )
{
	TCB	*q;

	if ( isQueEmpty(&gcb->wait_queue) ) {
		return tcb;
	}

	q = (TCB*)gcb->wait_queue.next;
	if ( (gcb->objatr & TA_TPRI) == 0 ) {
		return q;
	}

	return ( tcb->priority < q->priority )? tcb: q;
}
#endif /* USE_FUNC_GCB_TOP_OF_WAIT_QUEUE */


/*
 * Cancel task wait state.
 *	Remove the task from the timer queue and the wait queue.
 *	Do not update the task state.
 */
#pragma inline(wait_cancel)
static void wait_cancel( TCB *tcb )
{
	timer_delete(&tcb->wtmeb);
	QueRemove(&tcb->tskque);
}

/*
 * Connect the task to the prioritized wait queue.
 */
#pragma inline(queue_insert_tpri)
static void queue_insert_tpri( TCB *tcb, QUEUE *queue )
{
	QUEUE *q;
	QUEUE *start, *end;
	UB val;
	W offset;

	start = end = queue;
	val = tcb->priority;
	offset = offsetof(TCB,priority);

	for ( q = start->next; q != end; q = q->next ) {
		if ( *(UB*)((VB*)q + offset) > val ) {
			break;
		}
	}

	QueInsert(&tcb->tskque, q);
}

/*
 * Check wait disable
 */
#pragma inline(is_diswai)
static BOOL is_diswai( GCB *gcb, TCB *tcb, UINT tskwait )
{
	return ( (tcb->waitmask & tskwait) != 0
	      && (gcb->objatr & TA_NODISWAI) == 0 );
}
