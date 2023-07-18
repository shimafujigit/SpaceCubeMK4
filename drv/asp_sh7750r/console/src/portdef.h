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
 *    Changed by UC Technology at 2013/1/8.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for ASP-SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	portdef.h	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver system-dependent port definition
 *	(ASP-SH7750R)
 */

#include <tk/syslib.h>

/* Port definition */
IMPORT	SC_FUNC	ScFuncSCIF;

#define	N_PORTS		1

LOCAL  SC_DEFS	PortDefs[N_PORTS] = {
	{
		&ScFuncSCIF,
		{	0xFFE80000,	/* I/O base address */
			12,		/* iostep: -> Interrupt Level */
			IV_SCIF		/* Interrupt Vector */
		},
		0,			/* fctl: -> ENABLE */
		{0, 0, 0},		/* dt[3]: Not used */
		{0, 3, 0, 0, 19200},	/* default mode */
		{0, 0, 0, 0, 0, 0, 0},	/* default flow control */
		{0, 0, 0, 0},		/* default flow status */
	}
};

#define	INIT_AUXPORT(sup)		/* Nothing */
#define	START_AUXPORT(sup)		/* Nothing */

