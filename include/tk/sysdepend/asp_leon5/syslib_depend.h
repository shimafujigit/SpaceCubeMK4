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
 *	@(#)syslib_depend.h (tk/ASP-LEON5)
 *
 *	T-Kernel/SM LEON5 Library
 */

#ifndef __TK_SYSLIB_DEPEND_H__
#define __TK_SYSLIB_DEPEND_H__

#include <tk/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CPU interrupt control
 *	'intsts' is the value of PSR register in CPU
 */
IMPORT UINT disint( void );		/* Disable interrupt */
IMPORT UINT enaint( UINT intsts );	/* Enable(Restore) interrupt */

#define DI(intsts)	( (intsts) = disint() )
#define EI(intsts)	( enaint(intsts) )
#define isDI(intsts)	( ((intsts) & 0x0f00U) == 0x0f00U )

/*
 * Interrupt vector
 *	interrupt vector is the IRQ source number. (1-31)
 */
typedef UINT	INTVEC;

/* Convert to the interrupt definition number */
#define DINTNO(intvec)		(intvec)

/*
 * Enable interrupt
 */
IMPORT void EnableInt( INTVEC intvec );

/*
 * Disable interrupt
 */
IMPORT void DisableInt( INTVEC intvec );

/*
 * Clear interrupt request
 */
IMPORT void ClearInt( INTVEC intvec );

/*
 * Check the existence of interrupt request
 */
IMPORT BOOL CheckInt( INTVEC intvec );

/* ------------------------------------------------------------------------ */

/*
 * I/O port access
 *	Only memory mapped I/O
 */
#define out_w(port, data)	(*(_UW*)(port) = (data))
#define out_h(port, data)	(*(_UH*)(port) = (data))
#define out_b(port, data)	(*(_UB*)(port) = (data))

#define in_w(port)		(*(_UW*)(port))
#define in_h(port)		(*(_UH*)(port))
#define in_b(port)		(*(_UB*)(port))

#ifdef __cplusplus
}
#endif
#endif /* __TK_SYSLIB_DEPEND_H__ */
