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
 *	@(#)ptimer.c (libtk/ASP-LEON5)
 *
 *	Physical timer operations
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/ptimer.h>
#include <tk/sysdef.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/gptimer.h>

/*
 * Available timers
 */
typedef	struct	ptimer {
	volatile struct gptimer_timer_regs	*tmr;
	INTVEC	irq;
	FP	hdr;		/* user-defined interrupt hander */
	void*	exinf;		/* user-defined exntended information */
} PTIMER;

#define	N_PTIMER	1

LOCAL	PTIMER	pTimers[N_PTIMER];

/*
 * Check the physical timer number
 */
LOCAL PTIMER *chk_ptmrno( UINT ptmrno )
{
	PTIMER	*pt;

	if ( (ptmrno < 1) || (ptmrno > N_PTIMER) ) return NULL;
	pt = &pTimers[ptmrno - 1];

	if ( pt->tmr == NULL ) {
		if ( __bcc_timer_handle == 0 ||
		     __bcc_timer_interrupt == 0 ) return NULL;
		pt->tmr = &((struct gptimer_regs*)__bcc_timer_handle)->timer[1];
		pt->irq = __bcc_timer_interrupt + 1;
	}

	return pt;
}

/*
 * Timer interrupt handler
 */
LOCAL void timer_inthdr( UINT dintno )
{
	PTIMER	*pt = &pTimers[0];
	UW	c;

	c = pt->tmr->ctrl;
	if ( (c & GPTIMER_CTRL_IP) == 0 ) return;

	if ( (c & GPTIMER_CTRL_RS) == 0 ) c &= ~GPTIMER_CTRL_IE;

	pt->tmr->ctrl = c; /* clear interrupt */

	if ( pt->hdr != NULL ) {
		/* call user-defined interrupt handler */
		(*pt->hdr)(pt->exinf);
	}
}

/* ------------------------------------------------------------------------ */

/*
 * Start physical timer
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode )
{
	T_DINT	dint;
	UW	m;
	ER	err;
	PTIMER	*pt;

	/* check parameter */
	if ( (mode != TA_ALM_PTMR) && (mode != TA_CYC_PTMR) ) return E_PAR;
	if ( limit == 0 ) return E_PAR;

	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	/* stop timer */
	pt->tmr->ctrl = 0;

	DisableInt(pt->irq);
	ClearInt(pt->irq);

	/* Set counter */
	pt->tmr->counter = 0;
	pt->tmr->reload = limit;

	/* Set up timer interrupt handler */
	dint.intatr = TA_HLNG;
	dint.inthdr = (FP)timer_inthdr;
	err = tk_def_int(pt->irq, &dint);
	if ( err < E_OK ) return err;

	/* Enable timer interrupt */
	EnableInt(pt->irq);

	/* timer mode */
	m =	GPTIMER_CTRL_IP |
		GPTIMER_CTRL_IE |
		GPTIMER_CTRL_LD |
		GPTIMER_CTRL_EN;
	if ( (mode & TA_CYC_PTMR) != 0 ) m |= GPTIMER_CTRL_RS;

	/* Start counting */
	pt->tmr->ctrl = m;

	return E_OK;
}

/*
 * Stop physical timer
 */
EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	PTIMER	*pt;

	/* check parameter */
	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	/* Stop counting */
	pt->tmr->ctrl = 0;

	/* Stop timer interrupt */
	DisableInt(pt->irq);

	return E_OK;
}

/*
 * Obtain the count of the physical timer
 */
EXPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	PTIMER	*pt;

	/* check parameter */
	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	if ( (pt->tmr->ctrl & GPTIMER_CTRL_IE) == 0 ) {
		/* Timer Stopped */
		*p_count = 0;
	} else {
		/* Timer Started */
		*p_count = pt->tmr->reload - pt->tmr->counter;
	}

	return E_OK;
}

/*
 * Definition of physical timer hander
 */
EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	UINT	imsk;
	PTIMER	*pt;

	/* check parameter */
	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	if ( pk_dptmr != NULL ) {
		if ( (pk_dptmr->ptmratr & ~(TA_ASM|TA_HLNG)) != 0 )
					return E_PAR;
		DI(imsk);
		pt->exinf = pk_dptmr->exinf;
		pt->hdr   = pk_dptmr->ptmrhdr;
		EI(imsk);
	} else {
		pt->hdr   = NULL;
	}

	return E_OK;
}

/*
 * Obtain the configuration information of physical timer
 */
EXPORT ER GetPhysicalTimerConfig( UINT ptmrno, T_RPTMR *pk_rptmr )
{
	PTIMER	*pt;

	/* check parameter */
	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	/* set configuration */
	pk_rptmr->ptmrclk  = 1000000; /* Hz */
	pk_rptmr->maxcount = 0xffffffff;
	pk_rptmr->defhdr   = TRUE;

	return E_OK;
}
