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
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)ptimer.c (libtk/ASP-SH7750R)
 *
 *	Physical timer operations
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/ptimer.h>
#include <sys/sysinfo.h>

/*
 * Available timers
 */
typedef	struct	ptimer {
	UW	base;		/* base address of timer registers  */
	INTVEC	intvec;		/* interrupt vector */
	UB	intlvl;		/* interrupt level */
	UB	ps;		/* input clock prescaller */
	UB	sbit;		/* start bit in TSTR/TSTR2 */
	UB	mode;		/* start mode */
	FP	hdr;		/* user-defined interrupt hander */
	void*	exinf;		/* user-defined exntended information */
} PTIMER;

/* no	unit	ps		res.(usec)	max time(h:m:s)
   1	TMU1	0:Pclk/4   	0.133		 0:09:32
   2	TMU2	0:Pclk/4   	0.133		 0:09:32
*/
LOCAL	PTIMER	pTimers[] = {
	{ 0xFFD80014, 0x420, 13, 0, 0x02},	/* 1:TMU1 */
	{ 0xFFD80020, 0x440, 13, 0, 0x04},	/* 2:TMU2 */
};
#define	N_PTIMER		( sizeof(pTimers) / sizeof(PTIMER) )

/*
 * Timer registers
 */
/* common */
#define	TOCR		0xFFD80000	/* Output Control [8] */
#define	TSTR		0xFFD80004	/* Start TMU0-2   [8] */

/* individual */
#define	TCOR(pt)	((pt)->base + 0x00)	/* Constant [32] */
#define	TCNT(pt)	((pt)->base + 0x04)	/* Counter [32] */
#define	TCR(pt)		((pt)->base + 0x08)	/* Control [16] */

/*
 * Timer Input clock with PreScaller
 */
#define	PTMR_CLK(ps)	( ((SCInfo.Pclk) * 1000 * 1000) >> (((ps) + 1) * 2) )

/*
 * Check the physical timer number
 */
LOCAL	PTIMER	*chk_ptmrno( UINT ptmrno )
{
	if ( (ptmrno < 1) || (ptmrno > N_PTIMER) ) return NULL;
	return &pTimers[ptmrno - 1];
}

/*
 * Timer interrupt handler
 */
LOCAL void timer_inthdr( UINT dintno, void *sp )
{
	INT	i;
	PTIMER	*pt;

	for ( pt = &pTimers[0], i = N_PTIMER; --i >= 0; pt++ ) {
		if ( pt->intvec != dintno ) continue;

		/* Clear UNF interrupt flag */
		out_h(TCR(pt), pt->ps | 0x20);

		if ( pt->mode == TA_ALM_PTMR ) {
			/* stop counting */
			out_b(TSTR, in_b(TSTR) & ~pt->sbit);
		}

		if ( pt->hdr != NULL ) {
			/* call user-defined interrupt handler */
			(*pt->hdr)(pt->exinf);
		}
		break;
	}
}

/* ------------------------------------------------------------------------ */

/*
 * Start physical timer
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode )
{
	UINT	imsk;
	T_DINT	dint;
	ER	err;
	PTIMER	*pt;

	/* check parameter */
	if ( (mode != TA_ALM_PTMR) && (mode != TA_CYC_PTMR) ) return E_PAR;
	if ( limit == 0 ) return E_PAR;

	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	/* stop timer */
	DI(imsk);
	out_b(TSTR, in_b(TSTR) & ~pt->sbit);
	EI(imsk);

	/* set clock prescaller and enable UNF(underflow) interrupt */
	out_h(TCR(pt), pt->ps | 0x20);

	/* set counter */
	out_w(TCOR(pt), limit);
	out_w(TCNT(pt), limit);

	/* set up timer interrupt handler */
	dint.intatr = TA_HLNG;
	dint.inthdr = (FP)timer_inthdr;
	err = tk_def_int(pt->intvec, &dint);
	if ( err < E_OK ) return err;

	/* save mode */
	pt->mode = mode;

	/* start counting */
	DI(imsk);
	out_b(TSTR, in_b(TSTR) | pt->sbit);
	EI(imsk);

	/* enable timer interrupt */
	EnableInt(pt->intvec, pt->intlvl);

	return E_OK;
}

/*
 * Stop physical timer
 */
EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	UINT	imsk;
	PTIMER	*pt;

	/* check parameter */
	pt = chk_ptmrno(ptmrno);
	if ( pt == NULL ) return E_PAR;

	/* Stop timer interrupt */
	DisableInt(pt->intvec);

	/* Stop counting */
	DI(imsk);
	out_b(TSTR, in_b(TSTR) & ~pt->sbit);
	EI(imsk);

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

	if ( (in_b(TSTR) & pt->sbit) == 0 ) {
		/* Timer Stopped */
		*p_count = 0;
	} else {
		/* Timer Started */
		*p_count = in_w(TCOR(pt)) - in_w(TCNT(pt));
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
	pk_rptmr->ptmrclk  = PTMR_CLK(pt->ps);
	pk_rptmr->maxcount = 0xFFFFFFFF;
	pk_rptmr->defhdr   = TRUE;

	return E_OK;
}
