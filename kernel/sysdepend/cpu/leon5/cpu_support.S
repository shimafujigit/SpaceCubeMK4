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
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.01
 *    Copyright (c) 2021,2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	cpu_support.S (LEON5)
 *	Device-Dependent CPU Operation
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/errno.h>
#include <tk/sysdef.h>
#include <tk/asm.h>
#include <sys/sysinfo.inc>

#include "config.h"
#include "config/tk_config_func.h"
#include "cpu_conf.h"
#include "offset.h"

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
 *  Dispatcher
 * 	dispatch_to_schedtsk:
 * 		Throw away the current contexts and forcibly dispatch to
 * 		'schedtsk.'
 * 		Called directly by jump but do not return.
 * 		Called on the undefined stack state (undefined 'sp').
 * 		Called on the interrupt disable state.
 * 	dispatch_entry:
 * 		Normal dispatch processing.
 *		Called by software interrupt (irq 1)
 *
 * 	Contexts to save
 * 	Save registers except for SP to a stack. Save SP to TCB.
 *
 *  sp -> -42*4	+---------------+
 *		| %l0 - %l7	|
 *		| %i0 - %i7	|
 *	  -26*4	+---------------+
 *		| (reserve 7w)	|
 *	  -19*4	+---------------+   -
 *		| %wim		|   |
 *		| %psr		|   |
 *		| %pc		|   |
 *		| %npc		|   |
 *		| %y		|   | task context
 *		| %o0 - %o5,%o7	|   |	%sp = %o6
 *		| %g1 - %g7	|   |
 *  fp ->   0*4	+---------------+   |
 *		| %l0 - %l7	|   |
 *		| %i0 - %i7	|   |
 *		+---------------+   -
 *		|		|
 */
	.text
	.balign	4
	.globl	dispatch_to_schedtsk
	.type	dispatch_to_schedtsk, #function
dispatch_to_schedtsk:
	// interrupt disabled PSR.PIL=15 ET=1
	mov	1 << 2, %wim
	nop
	nop
	nop
	mov	PSR_S|PSR_CWPn(1)|PSR_PILn(15), %psr
	nop
	nop
	sethi	%hi(__bcc_sp_at_entry), %g1
	ld	[%g1 + %lo(__bcc_sp_at_entry)], %fp
	sub	%fp, 42*4, %sp

	sethi	%hi(Csym(ctxtsk)), %g1
#if USE_DBGSPT
	ld	[%g1 + %lo(Csym(ctxtsk))], %l4	// %l4 = ctxtsk
#endif
	st	%g0, [%g1 + %lo(Csym(ctxtsk))]	// ctxtsk = NULL

	mov	%psr, %l7			// disable dispatch
	set	PSR_PIL|PSR_ET|PSR_EF, %g1	// fpu disable
	andn	%l7, %g1, %l7			// %l7 = %psr & ~PIL|ET|EF
	wr	%l7, PSR_PILn(1), %psr		// PSR.ET=0 PIL=1
	wr	%l7, PSR_PILn(1)|PSR_ET, %psr	// PSR.ET=1
	// interrupt enabled & dispatch disabled PSR.PIL=1 ET=1

	ba,a	l_dispatch0

	.globl	Csym(dispatch_entry)
	.type	Csym(dispatch_entry), #function
Csym(dispatch_entry):
	// %l0 = %psr
	// %l1 = %pc
	// %l2 = %npc
	// interrupt disabled PSR.PIL=0 ET=0

	mov	%y, %l3			// save task context
	std	%l0, [%fp - 18*4]	// %psr %pc
	std	%l2, [%fp - 16*4]	// %npc %y
	std	%i0, [%fp - 14*4]	// %o0 %o1
	std	%i2, [%fp - 12*4]	// %o2 %o3
	std	%i4, [%fp - 10*4]	// %o4 %o5
	st	%i7, [%fp -  8*4]	// %o7
	st	%g1, [%fp -  7*4]	// %g1
	std	%g2, [%fp -  6*4]	// %g2 %g3
	std	%g4, [%fp -  4*4]	// %g4 %g5
	std	%g6, [%fp -  2*4]	// %g6 %g7

	sethi	%hi(__bcc_nwindows), %g3	// %g3 = nwin
	ld	[%g3 + %lo(__bcc_nwindows)], %g3
	and	%l0, PSR_CWP, %l4
	mov	%wim, %l5
	inc	%l4				// %l4 = cwp+1
	srl	%l5, %l4, %g2
	sub	%g3, %l4, %l4			// %l4 = nwin - (cwp+1)
	sll	%l5, %l4, %l5
	or	%l5, %g2, %g2			// %g2 = %wim ror (cwp+1)

	ba	l_save_regfile_next
	 andn	%l0, PSR_PIL|PSR_ET, %g4	// %g4 = %psr & ~PIL|ET

  l_save_regfile:
	restore				// save task context
	std	%l0, [%sp +  0*4]	// %l0 - %l7
	std	%l2, [%sp +  2*4]
	std	%l4, [%sp +  4*4]
	std	%l6, [%sp +  6*4]
	std	%i0, [%sp +  8*4]	// %i0 - %i7
	std	%i2, [%sp + 10*4]
	std	%i4, [%sp + 12*4]
	std	%i6, [%sp + 14*4]
  l_save_regfile_next:
	btst	1, %g2
	bz,a	l_save_regfile
	 srl	%g2, 1, %g2

	wr	%g4, PSR_PILn(1), %psr		// restore cwp & PIL=1 ET=0
#ifndef _FLAT
	and	%g4, PSR_CWP, %g1		// %g1 = cwp
	add	%g1, 2, %g1
	cmp	%g1, %g3			// %g3 = nwin
	bge,a	1f				// (cwp + 2) % nwin
	 sub	%g1, %g3, %g1
  1:	set	1, %g2
	sll	%g2, %g1, %g2
	mov	%g2, %wim			// %wim = 1 rol (cwp+2)
#else
	nop
	nop
	mov	%wim, %g2
	restore	%fp, 0, %fp
#endif
	st	%g2, [%fp - 19*4]		// save task context %wim
	sub	%fp, 42*4, %sp

	sethi	%hi(Csym(ctxtsk)), %g1
	ld	[%g1 + %lo(Csym(ctxtsk))], %l4	// %l4 = ctxtsk
	st	%g0, [%g1 + %lo(Csym(ctxtsk))]	// ctxtsk = NULL
	st	%sp, [%l4 + TCB_tskctxb+CTXB_ssp]	// save sp to TCB

	mov	%psr, %l7
	set	PSR_PIL|PSR_ET|PSR_EF, %g1	// fpu disable
	andn	%l7, %g1, %l7			// %l7 = %psr & ~PIL|ET|EF
	wr	%l7, PSR_PILn(1)|PSR_ET, %psr
	// interrupt enabled & dispatch disabled PSR.PIL=1 ET=1

  l_dispatch0:
#if USE_DBGSPT
	sethi	%hi(hook_stop_jmp), %g1
	ld	[%g1 + %lo(hook_stop_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_stop:
#endif

  l_dispatch1:
	wr	%l7, PSR_PILn(15), %psr		// %l7 = %psr & ~PIL|ET|EF
	wr	%l7, PSR_PILn(15)|PSR_ET, %psr
	// interrupt disabled PSR.PIL=15 ET=1

	sethi	%hi(Csym(schedtsk)), %g1
	ld	[%g1 + %lo(Csym(schedtsk))], %l4	// %l4 = schedtsk
	cmp	%l4, 0
	bne	l_dispatch2			// schedtsk ready
	 nop

#ifdef USE_FUNC_TK_SET_POW
	// nothing ready task, enter low power mode
	sethi	%hi(Csym(lowpow_discnt)), %g1
	ld	[%g1 + %lo(Csym(lowpow_discnt))], %g1
	cmp	%g1, 0				// low_pow disable ?
	bne	l_nolowpow
	 nop
	call	Csym(low_pow)			// call low_pow()
	 nop
  l_nolowpow:
#endif

	wr	%l7, PSR_PILn(1), %psr		// %l7 = %psr & ~PIL|ET|EF
	wr	%l7, PSR_PILn(1)|PSR_ET, %psr
	nop
	nop
	nop
	// interrupt enabled & dispatch disabled PSR.PIL=1 ET=1

	ba,a	l_dispatch1

  l_dispatch2:					// switch schedtsk
	// interrupt disabled PSR.PIL=15 ET=1
	sethi	%hi(Csym(ctxtsk)), %g1
	st	%l4, [%g1 + %lo(Csym(ctxtsk))]	// ctxtsk = schedtsk
	ld	[%l4 + TCB_tskctxb+CTXB_ssp], %sp  // restore sp from TCB

#if USE_DBGSPT
	sethi	%hi(hook_exec_jmp), %g1
	ld	[%g1 + %lo(hook_exec_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_exec:
#endif

	mov	%g0, %g4			// %g4 = 0 (fpu enable)
	sethi	%hi(Csym(fpu_ctxtsk)), %g1
	ld	[%g1 + %lo(Csym(fpu_ctxtsk))], %l5
	cmp	%l5, %l4
	bne,a	1f
	 sethi	%hi(PSR_EF), %g4		// %g4 = PSR_EF (fpu disable)
  1:

	ld	[%l4 + TCB_reqdct], %g3		// %g3 = DCT request
	mov	%sp, %g2
	ld	[%g2 + 23*4], %g1		// %wim
	mov	%g1, %wim			// restore wim
	ld	[%g2 + 24*4], %l7		// %psr
	set	PSR_PIL|PSR_ET|PSR_EF, %g1	// fpu disable
	andn	%l7, %g1, %l7			// %l7 = %psr & ~PIL|ET|EF
	wr	%l7, PSR_PILn(15), %psr		// restore cwp & PIL=15 ET=0
	nop
	nop
	set	0x80000000, %g1
	mov	%g2, %sp			// restore stack frame
	add	%g2, 42*4, %fp
	sub	%g3, %g1, %l6			// %l6 = DCT request

	restore					// restore task context
	ldd	[%sp +  0*4], %l0		// %l0 - %l7
	ldd	[%sp +  2*4], %l2
	ldd	[%sp +  4*4], %l4
	ldd	[%sp +  6*4], %l6
	ldd	[%sp +  8*4], %i0		// %i0 - %i7
	ldd	[%sp + 10*4], %i2
	ldd	[%sp + 12*4], %i4
	ldd	[%sp + 14*4], %i6
	save
						// restore task context
	ldd	[%fp - 18*4], %l0		// %psr %pc
	ldd	[%fp - 16*4], %l2		// %npc %y
	andn	%l0, %g4, %l0			// %g4 = 0 or PSR_EF
	mov	%l3, %y
	ldd	[%fp - 14*4], %i0		// %o0 %o1
	ldd	[%fp - 12*4], %i2		// %o2 %o3
	ldd	[%fp - 10*4], %i4		// %o4 %o5
	ld	[%fp -  8*4], %i7		// %o7
	ld	[%fp -  7*4], %g1		// %g1
	ldd	[%fp -  6*4], %g2		// %g2 %g3
	ldd	[%fp -  4*4], %g4		// %g4 %g5
	ldd	[%fp -  2*4], %g6		// %g6 %g7

	cmp	%l6, 0				// if DCT required
	be	dct_startup			// then DCT processing
	 nop

	mov	%l0, %psr
	nop
	nop

	jmp	%l1	// pc
	 rett	%l2	// npc

#if USE_DBGSPT
/*
 * Task dispatcher hook routine call
 *	void stop( ID tskid, INT lsid, UINT tskstat )
 *	void exec( ID tskid, INT lsid )
 */
	.text
	.balign	4
hook_stop:
	cmp	%l4, 0				// %l4 = ctxtsk
	be	l_notask
	 sethi	%hi(Csym(hook_stopfn)), %g1

	ld	[%g1 + %lo(Csym(hook_stopfn))], %g1
	ldub	[%l4 + TCB_state], %o2			// tskstat
	sll	%o2, 1, %o2
	ld	[%l4 + TCB_tskctxb+CTXB_lsid], %o1	// lsid
	call	%g1			// call stop(tskid, lsid, tskstat)
	 ld	[%l4 + TCB_tskid], %o0			// tskid

  l_notask:
	ba,a	ret_hook_stop

hook_exec:
#ifndef _FLAT
	sethi	%hi(__bcc_nwindows), %g3	// %g3 = nwin
	ld	[%g3 + %lo(__bcc_nwindows)], %g3
	mov	%psr, %g1
	and	%g1, PSR_CWP, %g1		// %g1 = cwp
	inc	%g1
	cmp	%g1, %g3			// %g3 = nwin
	bge,a	1f				// (cwp + 1) % nwin
	 sub	%g1, %g3, %g1
  1:	set	1, %g2
	sll	%g2, %g1, %g2
	mov	%g2, %wim			// %wim = 1 rol (cwp+1)
#endif
	add	%sp, 42*4, %fp			// temporary stack frame

	sethi	%hi(Csym(hook_execfn)), %g1
	ld	[%g1 + %lo(Csym(hook_execfn))], %g1
	ld	[%l4 + TCB_tskctxb+CTXB_lsid], %o1	// lsid
	call	%g1			// call exec(tskid, lsid)
	 ld	[%l4 + TCB_tskid], %o0			// tskid

	ba,a	ret_hook_exec

/*
 * Set/Free task dispatcher hook routine
 */
	.globl	Csym(hook_dsp)
	.type	Csym(hook_dsp), #function
Csym(hook_dsp):
	set	hook_exec, %o0
	set	hook_stop, %o1
	sethi	%hi(hook_exec_jmp), %g1
	st	%o0, [%g1 + %lo(hook_exec_jmp)]
	sethi	%hi(hook_stop_jmp), %g1
	st	%o1, [%g1 + %lo(hook_stop_jmp)]
	retl
	 nop

	.globl	Csym(unhook_dsp)
	.type	Csym(unhook_dsp), #function
Csym(unhook_dsp):
	set	ret_hook_exec, %o0
	set	ret_hook_stop, %o1
	sethi	%hi(hook_exec_jmp), %g1
	st	%o0, [%g1 + %lo(hook_exec_jmp)]
	sethi	%hi(hook_stop_jmp), %g1
	st	%o1, [%g1 + %lo(hook_stop_jmp)]
	retl
	 nop

	.data
	.balign	4
  hook_exec_jmp:	.int	ret_hook_exec
  hook_stop_jmp:	.int	ret_hook_stop

#endif /* USE_DBGSPT */

/* ======================================================================== */
/*
 * Unavailable FPU (fp_disabled exception)
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 *	%l6 = tt
 */
	.text
	.balign	4
	.globl	Csym(unavailable_fpu)
	.type	Csym(unavailable_fpu), #function
Csym(unavailable_fpu):
	// trap disabled PSR.ET=0 PIL=? / fpu disabled EF=0
	sethi	%hi(Csym(ctxtsk)), %l4
	ld	[%l4 + %lo(Csym(ctxtsk))], %l4		// %l4 = ctxtsk
	cmp	%l4, 0
	be	uf_illegal		// ctxtsk == NULL
	 set	TA_FPU, %l3
	ld	[%l4 + TCB_tskatr], %l5
	btst	%l3, %l5
	bz	uf_illegal		// (ctxtsk->tskatr & TA_FPU) == 0
	 set	PSR_EF, %l3

	wr	%l0, %l3, %psr		// PSR.EF=1

	sethi	%hi(Csym(fpu_ctxtsk)), %l5
	ld	[%l5 + %lo(Csym(fpu_ctxtsk))], %l6	// %l6 = fpu_ctxtsk
	cmp	%l6, %l4
	be	uf_return		// fpu_ctxtsk == ctxtsk
	 nop

	cmp	%l6, 0
	be	uf_load_fpuctx		// fpu_ctxtsk == NULL
	 nop

	// save fpu context
	ld	[%l6 + TCB_isstack], %l7	// fpu_ctxtsk->isstack
	std	%f0,  [%l7 +  0*4]
	std	%f2,  [%l7 +  2*4]
	std	%f4,  [%l7 +  4*4]
	std	%f6,  [%l7 +  6*4]
	std	%f8,  [%l7 +  8*4]
	std	%f10, [%l7 + 10*4]
	std	%f12, [%l7 + 12*4]
	std	%f14, [%l7 + 14*4]
	std	%f16, [%l7 + 16*4]
	std	%f18, [%l7 + 18*4]
	std	%f20, [%l7 + 20*4]
	std	%f22, [%l7 + 22*4]
	std	%f24, [%l7 + 24*4]
	std	%f26, [%l7 + 26*4]
	std	%f28, [%l7 + 28*4]
	std	%f30, [%l7 + 30*4]
	st	%fsr, [%l7 + 32*4]

  uf_load_fpuctx:
	// load fpu context
	ld	[%l4 + TCB_isstack], %l7	// ctxtsk->isstack
	ldd	[%l7 +  0*4], %f0
	ldd	[%l7 +  2*4], %f2
	ldd	[%l7 +  4*4], %f4
	ldd	[%l7 +  6*4], %f6
	ldd	[%l7 +  8*4], %f8
	ldd	[%l7 + 10*4], %f10
	ldd	[%l7 + 12*4], %f12
	ldd	[%l7 + 14*4], %f14
	ldd	[%l7 + 16*4], %f16
	ldd	[%l7 + 18*4], %f18
	ldd	[%l7 + 20*4], %f20
	ldd	[%l7 + 22*4], %f22
	ldd	[%l7 + 24*4], %f24
	ldd	[%l7 + 26*4], %f26
	ldd	[%l7 + 28*4], %f28
	ldd	[%l7 + 30*4], %f30
	ld	[%l7 + 32*4], %fsr

	st	%l4, [%l5 + %lo(Csym(fpu_ctxtsk))]	// fpu_ctxtsk = ctxtsk

  uf_return:
	wr	%l0, %l3, %psr		// restore psr + EF=1
	nop
	nop
	nop

	jmp	%l1		// pc
	 rett	%l2		// npc

  uf_illegal:
	// %l0 = psr
	// %l1 = pc
	// %l2 = npc
	// %l6 = tt
	ba,a	monitor_default_handler


/*
 * save FPU context
 *	void save_fpuctx( FPUContext* )
 *
 *	typedef struct {
 *		UW	f[32];
 *		UW	fsr;
 *		UW	rsv;
 *	} FPUContext;
 */
	.text
	.balign	4
	.globl	Csym(save_fpuctx)
	.type	Csym(save_fpuctx), #function
Csym(save_fpuctx):
	mov	%psr, %g2
	set	PSR_EF, %g1
	or	%g2, %g1, %g1
	mov	%g1, %psr		// PSR.EF=1
	nop
	nop
	nop

	std	%f0,  [%o0 +  0*4]
	std	%f2,  [%o0 +  2*4]
	std	%f4,  [%o0 +  4*4]
	std	%f6,  [%o0 +  6*4]
	std	%f8,  [%o0 +  8*4]
	std	%f10, [%o0 + 10*4]
	std	%f12, [%o0 + 12*4]
	std	%f14, [%o0 + 14*4]
	std	%f16, [%o0 + 16*4]
	std	%f18, [%o0 + 18*4]
	std	%f20, [%o0 + 20*4]
	std	%f22, [%o0 + 22*4]
	std	%f24, [%o0 + 24*4]
	std	%f26, [%o0 + 26*4]
	std	%f28, [%o0 + 28*4]
	std	%f30, [%o0 + 30*4]
	st	%fsr, [%o0 + 32*4]

	mov	%g2, %psr		// restore psr
	nop
	retl
	 nop

/* ======================================================================== */
/*
 * High level programming language routine for interrupt handler
 *	Called by BCC interrupt entry routine
 *
 * void inthdr_startup( void *inthdr, int irq )
 */
	.text
	.balign	4
	.globl	Csym(inthdr_startup)
	.type	Csym(inthdr_startup), #function
Csym(inthdr_startup):
#ifndef _FLAT
	save	%sp, -(16+8)*4, %sp
#else
	std	%fp, [%sp + 14*4]	// save %i6 %i7
	mov	%sp, %fp		// save %sp
	mov	%o7, %i7		// save %o7 (%i7 = return address)
	add	%sp, -(16+8)*4, %sp
	std	%i0, [%fp + 8*4]	// save %i0 %i1
	mov	%o0, %i0		// %i0 = inthdr
	mov	%o1, %i1		// %i1 = irq
#endif

	sethi	%hi(SCINFO_TASKINDP), %g1	// enter task independent state
	ld	[%g1 + %lo(SCINFO_TASKINDP)], %g2
	inc	%g2
	st	%g2, [%g1 + %lo(SCINFO_TASKINDP)]

#if USE_DBGSPT
	sethi	%hi(hook_ienter_jmp), %g1
	ld	[%g1 + %lo(hook_ienter_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_ienter:
#endif

	call	%i0		// call inthdr(dintno)
	 mov	%i1, %o0	// dintno = irq

#if USE_DBGSPT
	sethi	%hi(hook_ileave_jmp), %g1
	ld	[%g1 + %lo(hook_ileave_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_ileave:
#endif

	sethi	%hi(SCINFO_TASKINDP), %g1	// exit task independent state
	ld	[%g1 + %lo(SCINFO_TASKINDP)], %g2
	dec	%g2
	st	%g2, [%g1 + %lo(SCINFO_TASKINDP)]

#ifndef _FLAT
	ret
	 restore
#else
	ldd	[%fp + 8*4], %i0	// restore %i0 %i1
	mov	%fp, %sp
	ret
	 ldd	[%sp + 14*4], %fp	// restore %i6 %i7
#endif


#if USE_DBGSPT
/*
 * Interrupt handler hook routine call
 *	void enter( UINT dintno )
 *	void leave( UINT dintno )
 */
	.text
	.balign	4
hook_ienter:
	sethi	%hi(Csym(hook_ienterfn)), %g1
	ld	[%g1 + %lo(Csym(hook_ienterfn))], %g1
	call	%g1			// call ienter(dintno)
	 mov	%i1, %o0		// dintno = irq

	ba,a	ret_hook_ienter

hook_ileave:
	sethi	%hi(Csym(hook_ileavefn)), %g1
	ld	[%g1 + %lo(Csym(hook_ileavefn))], %g1
	call	%g1			// call ileave(dintno)
	 mov	%i1, %o0		// dintno = irq

	ba,a	ret_hook_ileave

/*
 * Set/Free interrupt handler hook routine
 */
	.globl	Csym(hook_int)
	.type	Csym(hook_int), #function
Csym(hook_int):
	set	hook_ienter, %o0
	set	hook_ileave, %o1
	sethi	%hi(hook_ienter_jmp), %g1
	st	%o0, [%g1 + %lo(hook_ienter_jmp)]
	sethi	%hi(hook_ileave_jmp), %g1
	st	%o1, [%g1 + %lo(hook_ileave_jmp)]
	retl
	 nop

	.globl	Csym(unhook_int)
	.type	Csym(unhook_int), #function
Csym(unhook_int):
	set	ret_hook_ienter, %o0
	set	ret_hook_ileave, %o1
	sethi	%hi(hook_ienter_jmp), %g1
	st	%o0, [%g1 + %lo(hook_ienter_jmp)]
	sethi	%hi(hook_ileave_jmp), %g1
	st	%o1, [%g1 + %lo(hook_ileave_jmp)]
	retl
	 nop

	.data
	.balign	4
  hook_ienter_jmp:	.int	ret_hook_ienter
  hook_ileave_jmp:	.int	ret_hook_ileave

#endif /* USE_DBGSPT */

/* ======================================================================== */

/*
 * Unsupported system call
 */
	.text
	.balign	4
	.globl	Csym(no_support)
	.type	Csym(no_support), #function
Csym(no_support):
	set	E_RSFN, %o0
	retl
	 nop

/*
 * System call entry table
 */
	.section ".rodata"
	.balign	4
_svctbl:
	.int	Csym(no_support)
#include "sys/svc/tknospt.h"
#include "sys/svc/tksvctbl.h"

/*
 * System call entry
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 *	%i0 - %i5 = 1st - 6th parameter
 *	%g2 = function code
 *   out:
 *	%i0 = return parameter
 *   keep:
 *	%i7 = interface library return address
 */
	.text
	.balign	4
	.globl	Csym(call_entry)
	.type	Csym(call_entry), #function
Csym(call_entry):
	// trap disabled PSR.ET=0 PIL=?
	.equ	sframe, (16+8)*4
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
	// trap enable PSR.ET=1 PIL=?
	wr	%l0, PSR_ET, %psr

#if USE_DBGSPT
	mov	0, %l3				// %l3 = exinf = 0
	sethi	%hi(hook_enter_jmp), %g1
	ld	[%g1 + %lo(hook_enter_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_enter:
#endif

	subcc	%g2, 0, %l1		// %g2 = %l1 = function code
	bpos,a	l_esvc_function		//	< 0: system call
	 mov	%g2, %o1		//	>=0: extended SVC

	// T-Kernel system call
	sra	%g2, 16, %g3		// %g3 = function number
	set	(N_TFN - 0x8000), %g1
	cmp	%g3, %g1
	bg	l_illegal_svc
	 sll	%g3, 2, %g3

#ifndef _FLAT
	mov	%i0, %o0		// 1st - 6th parameter
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
	mov	%i4, %o4
	mov	%i5, %o5
#endif

	set	_svctbl - (-0x8000 << 2), %g1
	ld	[%g1 + %g3], %g1
	call	%g1			// T-Kernel system call
	 nop
	mov	%o0, %i0		// %i0 = return value

  l_retsvc:
#if USE_DBGSPT
	sethi	%hi(hook_leave_jmp), %g1
	ld	[%g1 + %lo(hook_leave_jmp)], %g1
	jmp	%g1
	 nop
  ret_hook_leave:
#endif

	// trap disable PSR.ET=0 PIL=?
	mov	%psr, %l0
	set	PSR_ET|PSR_EF, %g1
	andn	%l0, %g1, %l0
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

	sethi	%hi(SCINFO_TASKINDP), %g1	// task independent state ?
	ld	[%g1 + %lo(SCINFO_TASKINDP)], %g1
	cmp	%g1, 0
	bne	l_nodct_svc
	 set	0x80000000, %g2

	sethi	%hi(Csym(ctxtsk)), %g1		// DCT required ?
	ld	[%g1 + %lo(Csym(ctxtsk))], %g1
	ld	[%g1 + TCB_reqdct], %g1
	cmp	%g1, %g2
	bne	l_nodct_svc
	 mov	%psr, %l0	// %l0 = psr

	btst	PSR_PIL, %l0			// dispatch disable ?
	bnz	l_nodct_svc
	 mov	%l2, %l1	// %l1 = pc

	ba	dct_startup			// go to DCT
	 inc	4, %l2		// %l2 = npc

  l_nodct_svc:
	jmp	%l2	// npc
	 rett	%l2+4	// npc + 4


  l_esvc_function:
	// extended SVC
	call	Csym(svc_ientry)	// svc_ientry(pk_para, fncd)
#ifndef _FLAT
	 mov	%i0, %o0		// pk_para
#else
	 nop
#endif
	ba	l_retsvc
	 mov	%o0, %i0		// %i0 = return value


  l_illegal_svc:
	set	E_RSFN, %i0
	ba,a	l_retsvc


#if USE_DBGSPT
/*
 * System call/Extended SVC hook routine call
 *	VP enter( FN fncd, TD_CALINF *calinf, ... )
 *	void leave( FN fncd, INT ret, VP exinf )
 *
 *	typedef struct td_calinf {
 *		VP	sp;
 *		VP	o7;
 *		VP	pc;
 *		VP	npc;
 *	} TD_CALINF;
 */
	.text
	.balign	4
hook_enter:
	sub	%sp, (2+4)*4, %sp

#ifdef _FLAT
	std	%i0, [%fp +  8*4]	// save %i0 - %i5
	std	%i2, [%fp + 10*4]
	std	%i4, [%fp + 12*4]
	mov	%o0, %i0		// save parameter
	mov	%o1, %i1
	mov	%o2, %i2
	mov	%o3, %i3
	mov	%o4, %i4
	mov	%o5, %i5
#endif

	st	%i5, [%sp + (16+7+1)*4]	// 6th - 1st parameter
	st	%i4, [%sp + (16+7+0)*4]
	mov	%i3, %o5
	mov	%i2, %o4
	mov	%i1, %o3
	mov	%i0, %o2
	sub	%fp, 4*4, %o1		// %o1 = &calinf
	mov	%g2, %o0		// %o0 = fncd

	st	%fp, [%o1 + 0*4]	// calinf.sp
	st	%i7, [%o1 + 1*4]	// calinf.o7
	st	%l1, [%o1 + 2*4]	// calinf.pc
	st	%l2, [%o1 + 3*4]	// calinf.npc

	sethi	%hi(Csym(hook_enterfn)), %g1
	ld	[%g1 + %lo(Csym(hook_enterfn))], %g1
	call	%g1			// exinf = enter(fncd, ...)
	 mov	%g2, %l1		// %l1 = save fncd
	mov	%l1, %g2		// %g2 = restore fncd

	mov	%o0, %l3		// %l3 = exinf

#ifdef _FLAT
	mov	%i0, %o0		// restore parameter
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
	mov	%i4, %o4
	mov	%i5, %o5
	ldd	[%fp +  8*4], %i0	// restore %i0 - %i5
	ldd	[%fp + 10*4], %i2
	ldd	[%fp + 12*4], %i4
#endif

	ba	ret_hook_enter
	 add	%sp, (2+4)*4, %sp

hook_leave:
	mov	%l3, %o2		// exinf
	mov	%i0, %o1		// ret
	mov	%l1, %o0		// fncd

	sethi	%hi(Csym(hook_leavefn)), %g1
	ld	[%g1 + %lo(Csym(hook_leavefn))], %g1
	call	%g1			// call leave(fncd, ret, exinf)
	 nop

	ba,a	ret_hook_leave

/*
 * Set/Free system call/extended SVC hook routine
 */
	.globl	Csym(hook_svc)
	.type	Csym(hook_svc), #function
Csym(hook_svc):
	set	hook_enter, %o0
	set	hook_leave, %o1
	sethi	%hi(hook_enter_jmp), %g1
	st	%o0, [%g1 + %lo(hook_enter_jmp)]
	sethi	%hi(hook_leave_jmp), %g1
	st	%o1, [%g1 + %lo(hook_leave_jmp)]
	retl
	 nop

	.globl	Csym(unhook_svc)
	.type	Csym(unhook_svc), #function
Csym(unhook_svc):
	set	ret_hook_enter, %o0
	set	ret_hook_leave, %o1
	sethi	%hi(hook_enter_jmp), %g1
	st	%o0, [%g1 + %lo(hook_enter_jmp)]
	sethi	%hi(hook_leave_jmp), %g1
	st	%o1, [%g1 + %lo(hook_leave_jmp)]
	retl
	 nop

	.data
	.balign	4
  hook_enter_jmp:	.int	ret_hook_enter
  hook_leave_jmp:	.int	ret_hook_leave

#endif /* USE_DBGSPT */

/* ======================================================================== */

#if USE_DBGSPT
/*
 * Debugger support function service call entry table
 */
	.section ".rodata"
	.balign	4
_tdsvctbl:
	.int	Csym(no_support)
#include "sys/svc/tdnospt.h"
#include "sys/svc/tdsvctbl.h"

/*
 * Debugger support function service call entry
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
	.globl	Csym(call_dbgspt)
	.type	Csym(call_dbgspt), #function
Csym(call_dbgspt):
	// trap disabled PSR.ET=0 PIL=?
	.equ	sframe, (16+8)*4
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
	// trap enable PSR.ET=1 PIL=?
	set	PSR_ET|PSR_EF, %g1
	andn	%l0, %g1, %l0
	wr	%l0, PSR_ET, %psr

	sra	%g2, 16, %g3		// %g3 = function number
	set	(N_TDFN - 0x8000), %g1
	cmp	%g3, %g1
	bg	b_illegal_svc
	 sll	%g3, 2, %g3

#ifndef _FLAT
	mov	%i0, %o0		// 1st - 4th parameter
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
#endif

	set	_tdsvctbl - (-0x8000 << 2), %g1
	ld	[%g1 + %g3], %g1
	call	%g1			// T-Kernel/DS service call
	 nop
	mov	%o0, %i0		// %i0 = return value

  b_retsvc:
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


  b_illegal_svc:
	set	E_RSFN, %i0
	ba,a	b_retsvc

#endif /* USE_DBGSPT */

/* ======================================================================== */
/*
 * High level programming language routine for timer handler
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 */
	.text
	.balign	4
	.globl	Csym(timer_handler_startup)
	.type	Csym(timer_handler_startup), #function
Csym(timer_handler_startup):
	// trap disabled PSR.ET=0 PIL=?
	.equ	sframe, (16+7+5)*4
	st	%g1, [%fp - 5*4 + 0*4]		// save %g1 - %g4
	std	%g2, [%fp - 5*4 + 1*4]
	st	%g4, [%fp - 5*4 + 3*4]
	mov	%y, %g1				// save %y
	st	%g1, [%fp - 5*4 + 4*4]
#ifndef _FLAT
	enter_trap
	sub	%fp, sframe, %sp
#else
	std	%l0, [%fp - sframe + 0*4]	// psr pc
	st	%l2, [%fp - sframe + 2*4]	// npc
	restore
	std	%fp, [%sp + 14*4]		// save %i6 %i7
	mov	%sp, %fp			// save %o6 %o7
	mov	%o7, %i7
	sub	%fp, sframe, %sp
	std	%i0, [%fp +  8*4]		// save %i0 - %i5
	std	%i2, [%fp + 10*4]
	std	%i4, [%fp + 12*4]
	mov	%o0, %i0			// save %o0 - %o5
	mov	%o1, %i1
	mov	%o2, %i2
	mov	%o3, %i3
	mov	%o4, %i4
	mov	%o5, %i5
	std	%l0, [%fp + 0*4]		// save %l0 - %l3
	std	%l2, [%fp + 2*4]
	ldd	[%sp + 0*4], %l0		// %l1 = pc
	ld	[%sp + 2*4], %l2		// %l2 = npc
	mov	%l0, %l3			// %l3 = psr (original)
	mov	%psr, %l0			// %l0 = psr (cwp+1)
#endif
	set	PSR_PIL|PSR_ET|PSR_EF, %g1
	andn	%l0, %g1, %g1
	wr	%g1, PSR_PIL, %psr
	wr	%g1, PSR_PIL|PSR_ET, %psr
	// interrupt disable PSR.PIL=15 ET=1 EF=0

	sethi	%hi(SCINFO_TASKINDP), %g1	// enter task independent state
	ld	[%g1 + %lo(SCINFO_TASKINDP)], %g2
	inc	%g2
	st	%g2, [%g1 + %lo(SCINFO_TASKINDP)]

	call	Csym(timer_handler)		// call timer_handler()
	 nop
	// interrupt disabled

	sethi	%hi(SCINFO_TASKINDP), %g1	// exit task independent state
	ld	[%g1 + %lo(SCINFO_TASKINDP)], %g2
	dec	%g2
	st	%g2, [%g1 + %lo(SCINFO_TASKINDP)]

	// trap disable PSR.ET=0 PIL=?
	mov	%l0, %psr
#ifndef _FLAT
	leave_trap
	mov	%l0, %psr			// restore %psr
#else
	mov	%l1, %g1			// pc
	mov	%l2, %g2			// npc
	mov	%l3, %g3			// psr
	ldd	[%fp + 0*4], %l0		// restore %l0 - %l3
	ldd	[%fp + 2*4], %l2
	mov	%i0, %o0			// restore %o0 - %o5
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
	mov	%i4, %o4
	mov	%i5, %o5
	ldd	[%fp +  8*4], %i0		// restore %i0 - %i5
	ldd	[%fp + 10*4], %i2
	ldd	[%fp + 12*4], %i4
	mov	%fp, %sp			// restore %o6 %o7
	mov	%i7, %o7
	ldd	[%sp + 14*4], %fp		// restore %i6 %i7
	save
	mov	%g1, %l1			// %l1 = pc
	mov	%g2, %l2			// %l2 = npc
	mov	%g3, %psr			// restore %psr
#endif
	ld	[%fp - 5*4 + 4*4], %g1		// restore %y
	mov	%g1, %y
	ld	[%fp - 5*4 + 0*4], %g1		// restore %g1 - %g4
	ldd	[%fp - 5*4 + 1*4], %g2
	ld	[%fp - 5*4 + 3*4], %g4

	jmp	%l1		// pc
	 rett	%l2		// npc

/* ======================================================================== */
/*
 * Delayed context trap (DCT)
 *	Task exception handler startup
 *
 *   in:
 *	%l0 = psr
 *	%l1 = pc
 *	%l2 = npc
 *
 *   make stack frame:
 *		+---------------+
 *	sp ->	| save area	| register window %i# %l#
 *		|     (17 word)	| hidden parameter
 *		+---------------+
 *	  +17*4	| texcd		|
 *		|      (6 word)	|
 *		+---------------+
 *	  +23*4	| psr		|
 *		| pc		|
 *		| npc		|
 *		|      (3 word)	|
 *		+---------------+
 *	  +26*4	| save area	| save registers %y %g# %f# %fsr
 *		|     (42 word)	|
 *		+---------------+
 *	fp ->	|		|
 */
	.text
	.balign	4
dct_startup:
	// trap disabled PSR.ET=0 PIL=0
	.equ	sframe, (17+6+3+42)*4
	st	%g1, [%fp - sframe + 27*4]	// save %g1 - %g4
	std	%g2, [%fp - sframe + 28*4]
	st	%g4, [%fp - sframe + 30*4]
	mov	%y, %g1				// save %y
	st	%g1, [%fp - sframe + 26*4]
#ifndef _FLAT
	sub	%fp, sframe, %sp
#else
	std	%l0, [%fp - sframe + 0*4]	// psr pc
	st	%l2, [%fp - sframe + 2*4]	// npc
	restore
	std	%fp, [%sp + 14*4]		// save %i6 %i7
	mov	%sp, %fp			// save %o6 %o7
	mov	%o7, %i7
	sub	%fp, sframe, %sp
	std	%i0, [%fp +  8*4]		// save %i0 - %i5
	std	%i2, [%fp + 10*4]
	std	%i4, [%fp + 12*4]
	mov	%o0, %i0			// save %o0 - %o5
	mov	%o1, %i1
	mov	%o2, %i2
	mov	%o3, %i3
	mov	%o4, %i4
	mov	%o5, %i5
	std	%l0, [%fp + 0*4]		// save %l0 - %l3
	std	%l2, [%fp + 2*4]
	ldd	[%sp + 0*4], %l0		// %l1 = pc
	ld	[%sp + 2*4], %l2		// %l2 = npc
	mov	%l0, %l3			// %l3 = psr (original)
	mov	%psr, %l0			// %l0 = psr (cwp+1)
#endif
	set	PSR_EF, %g1
	andn	%l0, %g1, %l0			// psr.EF=0
	wr	%l0, PSR_PIL, %psr
	wr	%l0, PSR_PIL|PSR_ET, %psr
	// interrupt disable PSR.ET=1 PIL=15

#ifndef _FLAT
	st	%l0, [%sp + 23*4]		// psr
#else
	andn	%l3, %g1, %l3			// psr.EF=0
	st	%l3, [%sp + 23*4]		// psr
#endif
	st	%l1, [%sp + 24*4]		// pc
	st	%l2, [%sp + 25*4]		// npc

	call	Csym(setup_texhdr)		// texhdr = setup_texhdr(&texcd)
	 add	%sp, 16*4, %o0			// %o0 = &texcd
	// interrupt enabled

	cmp	%o0, 0				// %o0 = texhdr
	be	l_notexhdr
	 mov	%l0, %psr			// restore psr
	// trap disabled PSR.ET=0 PIL=0

	ld	[%sp + 16*4], %g1
	st	%g1, [%sp + 17*4]		// store texcd

	and	%l0, PSR_CWP, %g2		// save instruction without trap
	deccc	%g2
	bpos	1f
	 sethi	%hi(__bcc_nwindows_min1), %g1
	ld	[%g1 + %lo(__bcc_nwindows_min1)], %g2
  1:	andn	%l0, PSR_CWP, %g1
	wr	%g1, %g2, %psr			// psr.cwp = cwp - 1
	nop
	nop
	nop

	ld	[%fp + 26*4], %g1		// restore %y
	mov	%g1, %y
	ld	[%fp + 27*4], %g1		// restore %g1 - %g4
	ldd	[%fp + 28*4], %g2
	ld	[%fp + 30*4], %g4

	jmp	%i0	// pc  = texhdr
	 rett	%i0+4	// npc = texhdr + 4

/*
 * return to task from task exception handler
 */
	.globl	Csym(rettex_entry)
	.type	Csym(rettex_entry), #function
Csym(rettex_entry):
	// trap disabled PSR.ET=0 PIL=0
	restore

#ifndef _FLAT
	ld	[%sp + 23*4], %l0		// psr
#else
	ld	[%sp + 23*4], %l3		// psr
#endif
	ld	[%sp + 24*4], %l1		// pc
	ld	[%sp + 25*4], %l2		// npc

  l_notexhdr:
	// trap disabled PSR.ET=0 PIL=0
#ifndef _FLAT
	leave_trap
	mov	%l0, %psr			// restore %psr
#else
	mov	%l1, %g1			// pc
	mov	%l2, %g2			// npc
	mov	%l3, %g3			// psr
	ldd	[%fp + 0*4], %l0		// restore %l0 - %l3
	ldd	[%fp + 2*4], %l2
	mov	%i0, %o0			// restore %o0 - %o5
	mov	%i1, %o1
	mov	%i2, %o2
	mov	%i3, %o3
	mov	%i4, %o4
	mov	%i5, %o5
	ldd	[%fp +  8*4], %i0		// restore %i0 - %i5
	ldd	[%fp + 10*4], %i2
	ldd	[%fp + 12*4], %i4
	mov	%fp, %sp			// restore %o6 %o7
	mov	%i7, %o7
	ldd	[%sp + 14*4], %fp		// restore %i6 %i7
	save
	mov	%g1, %l1			// %l1 = pc
	mov	%g2, %l2			// %l2 = npc
	mov	%g3, %psr			// restore %psr
#endif
	ld	[%fp - sframe + 26*4], %g1	// restore %y
	mov	%g1, %y
	ld	[%fp - sframe + 27*4], %g1	// restore %g1 - %g4
	ldd	[%fp - sframe + 28*4], %g2
	ld	[%fp - sframe + 30*4], %g4

	jmp	%l1	// pc
	 rett	%l2	// npc

/* ======================================================================== */
