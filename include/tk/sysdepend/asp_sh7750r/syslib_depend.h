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
 *	@(#)syslib_depend.h (tk/ASP-SH7750R)
 *
 *	T-Kernel/SM SH7750R Library
 */

#ifndef __TK_SYSLIB_DEPEND_H__
#define __TK_SYSLIB_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CPU interrupt control
 *	'intsts' is the value of SR register in CPU
 *	disint()  Set SR.I = 15 and return the original SR to the return value.
 *	enaint()  Set SR.I = intsts.I. Do not change except for SR.I.
 *		  Return the original SR to the return value.
 */
IMPORT UINT disint( void );
IMPORT UINT enaint( UINT intsts );
#define DI(intsts)	( (intsts) = disint() )
#define EI(intsts)	( enaint(intsts) )
#define isDI(intsts)	( ((intsts) & 0x00f0U) != 0 )

/*
 * Enable the higher level of interrupt than intlevel (0-15)
 *	Set 'intlevel' to SR.I in CPU.
 *	Return the original SR to the return value.
 *	Use when enabling a multiplexed interrupt in interrupt handler.
 */
#define SetIntLevel(intlevel)	( enaint((intlevel) << 4) )

/*
 * Request the interrupt level (1-15) by the value of INTEVT (0x200-0x3c0)
 */
#define IntLevel(intevt)	( 0x1f - ((intevt) >> 5) )

/*
 * Interrupt vector
 *	Interrupt vector is the value of the interrupt factor code INTVEC 2
 */
typedef UINT	INTVEC;

/* Convert to the interrupt definition number */
#define DINTNO(intvec)	(intvec)

/*
 * Interrupt vector(INTVEC) : Typical exception code
 *	Only these typical exception codes can be used with
 *	an INTC operation functions.
 */
#define IV_TMU0		0x400	/* Timer ch.0 */
#define IV_TMU1		0x420	/* Timer ch.1 */
#define IV_TMU2		0x440	/* Timer ch.2 */
#define IV_TMU3		0xb00	/* Timer ch.3 */
#define IV_TMU4		0xb80	/* Timer ch.4 */
#define IV_RTC		0x480	/* RTC */
#define IV_SCI		0x4e0	/* SCI */
#define IV_WDT		0x560	/* WDT */
#define IV_REF		0x580	/* Memory refresh */
#define IV_HUDI		0x600	/* HUDI */
#define IV_GPIO		0x620	/* I/O port */
#define IV_DMAC		0x640	/* DMA */
#define IV_SCIF		0x700	/* SCIF */
//#define IV_PCIC0	0xa00	/* PCIC 0 */
//#define IV_PCIC1	0xa20	/* PCIC 1 */

/*
 * Conversion between IRL interrupt and interrupt vector.
 *  (*)IRL shows interrupt level('1'-'15'), not /IRL[3:0] signal value.
 */
#define IV_IRL(irl)	(0x3e0 - (0x20 * (irl)))
#define IRLNO(vec)	((0x3e0 - (vec)) / 0x20)

/*
 * Enable interrupt
 *	Set priority level to 'level' regarding interrupt specified by
 *	'intvec'.
 *	'1'-'15' is valid for 'level' and '15' is the highest level.
 *	If 'level' is invalid value, operation is not guaranteed.
 *	The INTMSK00 register is not changed  automatically, so it must be
 *	set if necessary.
 */
IMPORT void EnableInt( INTVEC intvec, INT level );

/*
 * Disable interrupt
 *	Disable interrupt specified by 'intvec' (i.e. set level 0).
 *	The INTMSK00 register is not changed  automatically, so it must be set
 *	if necessary.
 */
IMPORT void DisableInt( INTVEC intvec );

/*
 * Issue EOI(End Of Interrupt)
 */
#define EndOfInt(intvec)

/* ------------------------------------------------------------------------ */

/*
 * I/O port access
 *	Only memory mapped I/O for SH
 */
#define out_w(port, data)	*((volatile _UW*)(port)) = (data)
#define out_h(port, data)	*((volatile _UH*)(port)) = (data)
#define out_b(port, data)	*((volatile _UB*)(port)) = (data)

#define in_w(port)	*((volatile _UW*)(port))
//#define in_h(port)	((((UINT)port & 0xfffc0000U) == 0xb8240000U || \
//			((UINT)port & 0xff3c0000U) == 0xb6240000U ) ? \
//		*((volatile _UH*)(port - 0x40000)) : *((volatile _UH*)port))
#define in_h(port)	*((volatile _UH*)(port))
#define in_b(port)	*((volatile _UB*)(port))

#ifdef __cplusplus
}
#endif
#endif /* __TK_SYSLIB_DEPEND_H__ */

