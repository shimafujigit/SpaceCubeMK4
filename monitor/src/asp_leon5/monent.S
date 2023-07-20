/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/10/29.
 *
 *----------------------------------------------------------------------
 */

/*
 * Minimum T-Monitor for ASP-LEON5
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <tk/asm.h>

/*
 * enter trap macro
 *   in:
 *	%l0 = psr
 *   working use:
 *	%g1 %g3
 */
.macro enter_trap
	mov	%wim, %g3		// %g3 = wim
	srl	%g3, %l0, %g1		// wim >> cwp
	cmp	%g1, 1
	bne	1f
	 sethi	%hi(__bcc_nwindows_min1), %g1

	ld	[%g1 + %lo(__bcc_nwindows_min1)], %g1
	sll	%g3, %g1, %g1
	srl	%g3, 1, %g3

	save
	wr	%g3, %g1,%wim		// wim = wim ror 1

	std	%l0, [%sp +  0*4]	// store %l0 - %l7
	std	%l2, [%sp +  2*4]
	std	%l4, [%sp +  4*4]
	std	%l6, [%sp +  6*4]
	std	%i0, [%sp +  8*4]	// store %i0 - %i7
	std	%i2, [%sp + 10*4]
	std	%i4, [%sp + 12*4]
	std	%i6, [%sp + 14*4]
	restore
  1:
.endm

/*
 * leave trap macro
 *   in:
 *	%l0 = psr
 *   working use:
 *	%l5 %l6 %l7
 */
.macro leave_trap
	sethi	%hi(__bcc_nwindows_min1), %l7
	ld	[%l7 + %lo(__bcc_nwindows_min1)], %l7
	and	%l0, PSR_CWP, %l6
	inc	%l6			// %l6 = cwp + 1
	cmp	%l6, %l7
	bg,a	1f			// wrap around
	 mov	0, %l6
  1:
	mov	%wim, %l5		// %l5 = wim
	srl	%l5, %l6, %l6		// wim >> (cwp + 1)
	cmp	%l6, 1
	bne	2f
	 srl	%l5, %l7, %l6
	sll	%l5, 1, %l5
	wr	%l5, %l6, %wim		// wim = wim rol 1
	nop
	nop
	nop

	restore
	ldd	[%sp +  0*4], %l0	// load %l0 - %l7
	ldd	[%sp +  2*4], %l2
	ldd	[%sp +  4*4], %l4
	ldd	[%sp +  6*4], %l6
	ldd	[%sp +  8*4], %i0	// load %i0 - %i7
	ldd	[%sp + 10*4], %i2
	ldd	[%sp + 12*4], %i4
	ldd	[%sp + 14*4], %i6
	save
  2:
.endm

/* ======================================================================== */

/*
 * Monitor svc handler
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 *	%i0 - %i3 = 1st - 4th parameter
 *	%g2 = function code
 *   out:
 *	%i0 = return parameter
 *   keep:
 *	%i7 = interface library return address
 */
	.text
	.balign	4
	.globl	Csym(monitor_svc_handler)
	.type	Csym(monitor_svc_handler), #function
Csym(monitor_svc_handler):
	// trap disabled PSR.ET=0 PIL=?
	.equ	sframe, (16+8+4)*4
#ifndef _FLAT
	enter_trap
	sub	%fp, sframe, %sp
#else
	std	%l0, [%fp - sframe + 0*4]	// psr pc
	st	%l2, [%fp - sframe + 2*4]	// npc
	restore
	std	%fp, [%sp + 14*4]		// save %i6 %i7
	mov	%sp, %fp
	mov	%o7, %i7
	sub	%fp, sframe, %sp
	std	%l0, [%fp + 0*4]		// save %l0 %l1
	std	%l2, [%fp + 2*4]		// save %l2 %l3
	st	%i0, [%fp + 8*4]		// save %i0
	ldd	[%sp + 0*4], %l0		// %l1 = pc
	ld	[%sp + 2*4], %l2		// %l2 = npc
	mov	%psr, %l0			// %l0 = psr
#endif
	// interrupt disable PSR.PIL=15 ET=1
	andn	%l0, PSR_PIL|PSR_ET, %g1
	wr	%g1, PSR_PIL, %psr
	wr	%g1, PSR_PIL|PSR_ET, %psr

#ifndef _FLAT
	std	%i0, [%fp - 4*4]	// par[0] par[1]
	std	%i2, [%fp - 2*4]	// par[2] par[3]
#else
	std	%o0, [%fp - 4*4]	// par[0] par[1]
	std	%o2, [%fp - 2*4]	// par[2] par[3]
#endif
	sub	%fp, 4*4, %o1		// par
	call	Csym(ent_monitor)	// ent_monitor(fn_code, par)
	 mov	%g2, %o0		// fn_code
	mov	%o0, %i0		// %i0 = return value

	// trap disable PSR.ET=0 PIL=?
	mov	%l0, %psr
#ifndef _FLAT
	leave_trap
#else
	mov	%l2, %g2			// npc
	mov	%i0, %o0			// return value
	ldd	[%fp + 0*4], %l0		// restore %l0 %l1
	ldd	[%fp + 2*4], %l2		// restore %l2 %l3
	ld	[%fp + 8*4], %i0		// restore %i0
	mov	%fp, %sp
	mov	%i7, %o7
	ldd	[%sp + 14*4], %fp		// restore %i6 %i7
	save
	mov	%g2, %l2			// %l2 = npc
#endif

	jmp	%l2	// npc
	 rett	%l2+4	// npc + 4


/*
 * Monitor default handler
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 *	%l6 = tt
 */
	.globl	monitor_default_handler
	.type	monitor_default_handler, #function
monitor_default_handler:
	mov	%l0, %i0	// psr
	mov	%l1, %i1	// pc
	mov	%l2, %i2	// npc
	mov	%l6, %i3	// tt

	ba	Csym(monitor_svc_handler)	// never return
	 mov	127, %g2			// fn_code = TM_DEFAULT_HDR

/* ------------------------------------------------------------------------ */
