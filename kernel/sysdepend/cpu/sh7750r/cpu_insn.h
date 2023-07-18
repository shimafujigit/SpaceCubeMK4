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
 *	cpu_insn.h (SH7750R)
 *	CPU-Dependent Operation
 */

#ifndef _CPU_INSN_
#define _CPU_INSN_

#include <smachine.h>
#include <sys/sysinfo.h>

/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

/*
 * Get SR
 */
#define getSR()			(UINT)get_cr()

/* ------------------------------------------------------------------------ */
/*
 *	EIT-related
 */

/*
 * Monitor use vector number
 */
#define VECNO_TRAPA	( 0x02c / 4 )	/* TRAPA exception handler */
#define VECNO_BREAK	( 0x03c / 4 )	/* User break point handler */
#define VECNO_MONITOR	( 0x1c0 / 4 )	/* Monitor service call */
#define VECNO_DEFAULT	( 0x200 / 4 )	/* Default handler */
#define VECNO_TLBMISS	( 0x204 / 4 )	/* TLB miss exception handler */

/*
 * For saving monitor exception handler
 */
typedef struct monhdr {
	FP	trapa_hdr;
	FP	break_hdr;
	FP	monitor_hdr;
	FP	default_hdr;
	FP	tlbmiss_hdr;
} MONHDR;

/* For saving monitor exception handler */
IMPORT MONHDR	SaveMonHdr;

/*
 * Set interrupt handler
 */
IMPORT void define_inthdr( INT vecno, FP inthdr );

IMPORT VW	*int_stack_top;	/* EIT stack top */

/*
 * If it is the task-independent part, TRUE
 *	If EIT stack is consumed, the task independent part
 */
IMPORT VW *getISP( void );
IMPORT BOOL isTaskIndependent( void );

/*
 * Move to/Restore task independent part
 */
IMPORT void EnterTaskIndependent( void );
IMPORT void LeaveTaskIndependent( void );

/* ------------------------------------------------------------------------ */

#endif /* _CPU_INSN_ */
