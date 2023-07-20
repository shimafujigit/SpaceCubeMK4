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
 *	portdef.h	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver system-dependent port definition
 *	(ASP-LEON5)
 */

/* Port definition */
IMPORT	SC_FUNC	ScFuncAPBUART;

#define	N_PORTS		1

LOCAL  SC_DEFS	PortDefs[N_PORTS] = {
	{
		&ScFuncAPBUART,
		{	0,		/* I/O base address */
			0,		/* iostep */
			0		/* Interrupt Number */
		},
		{0, 0, 0, 0},		/* dt[0..4] */
		{0, 3, 0, 0, 19200},	/* default mode */
		{0, 0, 0, 0, 0, 0, 0},	/* default flow control */
		{0, 0, 0, 0},		/* default flow status */
	}
};

IMPORT void apbuart_devreg( BOOL StartUp );

#define	INIT_AUXPORT(sup)		apbuart_devreg(sup)
#define	START_AUXPORT(sup)		/* Nothing */

