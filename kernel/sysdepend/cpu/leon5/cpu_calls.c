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
 *	cpu_calls.c (ASP-LEON5)
 *	CPU-Dependent System Call
 */

#include "kernel.h"
#include "task.h"
#include "check.h"
#include "cpu_task.h"

#include <sys/sysinfo.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"

#include <bcc/bcc.h>

#if defined(USE_FUNC_TK_DIS_DSP)
/*
 * Dispatch enable/disable
 */
SYSCALL ER _tk_dis_dsp( void )
{
	CHECK_CTX(!in_loc());

	BEGIN_CRITICAL_SECTION;
	dispatch_disabled = DDS_DISABLE;
	_psr_ = (_psr_ & ~PSR_PIL) | PSR_PILn(1);	/* psr.PIL = 1 */
	END_CRITICAL_SECTION;

	return E_OK;
}
#endif /* USE_FUNC_TK_DIS_DSP */

#if defined(USE_FUNC_TK_ENA_DSP)
/*
 * Dispatch enable
 */
SYSCALL ER _tk_ena_dsp( void )
{
	CHECK_CTX(!in_loc());

	BEGIN_CRITICAL_SECTION;
	dispatch_disabled = DDS_ENABLE;
	_psr_ = (_psr_ & ~PSR_PIL) | PSR_PILn(0);	/* psr.PIL = 0 */
	END_CRITICAL_SECTION;

	return E_OK;
}
#endif /* USE_FUNC_TK_ENA_DSP */

/* ------------------------------------------------------------------------ */

/*
 * High level programming language supported
 */

#if defined(USE_FUNC_HLL_INTHDR)
#if 0
/* High level programming language interrupt handler entry */
EXPORT FP hll_inthdr[N_INTVEC];
#else
LOCAL struct bcc_isr_node hll_isrnode[30];
#endif
#endif /* USE_FUNC_HLL_INTHDR */

#if defined(USE_FUNC_TK_DEF_INT)
/* High level programming language-supported routine */
IMPORT void inthdr_startup( void *inthdr, int irq );

/*
 * Interrupt handler definition
 */
SYSCALL ER _tk_def_int( UINT dintno, CONST T_DINT *pk_dint )
{
#if defined(USE_FUNC_HLL_INTHDR)
	struct bcc_isr_node	*isr_node;
	int	i;
	ER	ercd = E_OK;

	CHECK_PAR(dintno >= 2 && dintno <= 31);

	if ( pk_dint != NULL ) {	/* Set interrupt handler */
		CHECK_RSATR(pk_dint->intatr, TA_HLNG);

		BEGIN_CRITICAL_SECTION;
		if ( (pk_dint->intatr & TA_HLNG) != 0 ) {
			isr_node = &hll_isrnode[dintno - 2];
			if ( isr_node->source != 0 ) {
				bcc_isr_unregister_node(isr_node);
			}
			isr_node->source  = dintno;
			isr_node->handler = inthdr_startup;
			isr_node->arg     = pk_dint->inthdr;
			i = bcc_isr_register_node(isr_node);
			if ( i != BCC_OK ) {
				isr_node->source = 0;
				ercd = E_SYS;
			}
		} else {
#if 0
			i = bcc_set_trap(dintno, pk_dint->inthdr);
			if ( i != BCC_OK ) ercd = E_SYS;
			bcc_flush_cache();
#else
			ercd = E_NOSPT;
#endif
		}
		END_CRITICAL_SECTION;
	} else {			/* Free interrupt handler */
		BEGIN_CRITICAL_SECTION;
		isr_node = &hll_isrnode[dintno - 2];
		if ( isr_node->source != 0 ) {
			bcc_isr_unregister_node(isr_node);
			isr_node->source = 0;
		}
		END_CRITICAL_SECTION;
	}

	return ercd;
#else
	return E_NOSPT;
#endif
}
#endif /* USE_FUNC_TK_DEF_INT */

/* ------------------------------------------------------------------------ */

#if defined(USE_FUNC_TK_GET_TSP)
/*
 * Get task space
 */
SYSCALL ER _tk_get_tsp( ID tskid, T_TSKSPC *pk_tskspc )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		pk_tskspc->uatb = tcb->tskctxb.uatb;
		pk_tskspc->lsid = tcb->tskctxb.lsid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_TSP */

#if defined(USE_FUNC_TK_SET_TSP)
/*
 * Set task space
 */
SYSCALL ER _tk_set_tsp( ID tskid, CONST T_TSKSPC *pk_tskspc )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST) {
		ercd = E_NOEXS;
	} else {
		tcb->tskctxb.uatb = pk_tskspc->uatb;
		tcb->tskctxb.lsid = pk_tskspc->lsid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_TSP */

/* ------------------------------------------------------------------------ */

#if defined(USE_FUNC_TK_SET_REG)||defined(USE_FUNC_TD_SET_REG)
/*
 * Set task register contents
 */
LOCAL void set_reg( TCB *tcb, CONST T_REGS *regs,
				CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	SStackFrame	*ssp;
	UW		psr, wim;
	const UW	psr_mask = PSR_ICC(0xf);

	ssp = tcb->tskctxb.ssp;
	psr = ssp->psr;
	wim = ssp->wim;

	if ( cregs != NULL ) ssp = cregs->sp;

	if ( regs != NULL ) {
		ssp->g1 = regs->g1;
		ssp->g2 = regs->g2;
		ssp->g3 = regs->g3;
		ssp->g4 = regs->g4;
		ssp->g5 = regs->g5;
		ssp->g6 = regs->g6;
		ssp->g7 = regs->g7;

		ssp->o0 = regs->o0;
		ssp->o1 = regs->o1;
		ssp->o2 = regs->o2;
		ssp->o3 = regs->o3;
		ssp->o4 = regs->o4;
		ssp->o5 = regs->o5;
		ssp->o7 = regs->o7;

		ssp->l0 = regs->l0;
		ssp->l1 = regs->l1;
		ssp->l2 = regs->l2;
		ssp->l3 = regs->l3;
		ssp->l4 = regs->l4;
		ssp->l5 = regs->l5;
		ssp->l6 = regs->l6;
		ssp->l7 = regs->l7;

		ssp->i0 = regs->i0;
		ssp->i1 = regs->i1;
		ssp->i2 = regs->i2;
		ssp->i3 = regs->i3;
		ssp->i4 = regs->i4;
		ssp->i5 = regs->i5;
		ssp->i6 = regs->i6;
		ssp->i7 = regs->i7;

		ssp->y = regs->y;
	}

	if ( eit != NULL ) {
		ssp->pc  = eit->pc;
		ssp->npc = eit->npc;
		ssp->psr = (psr & ~psr_mask) | (eit->psr & psr_mask);
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp  = cregs->sp;
		ssp->wim = wim;
	}
}
#endif /* USE_FUNC_TK_SET_REG or USE_FUNC_TD_SET_REG */

#if defined(USE_FUNC_TK_SET_REG)
/*
 * Set task register contents
 */
SYSCALL ER _tk_set_reg( ID tskid, CONST T_REGS *pk_regs,
				CONST T_EIT *pk_eit, CONST T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		set_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_REG */

#if defined(USE_FUNC_TK_GET_REG)||defined(USE_FUNC_TD_GET_REG)
/*
 * Get task register contents
 */
LOCAL void get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;

	ssp = tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		regs->g1 = ssp->g1;
		regs->g2 = ssp->g2;
		regs->g3 = ssp->g3;
		regs->g4 = ssp->g4;
		regs->g5 = ssp->g5;
		regs->g6 = ssp->g6;
		regs->g7 = ssp->g7;

		regs->o0 = ssp->o0;
		regs->o1 = ssp->o1;
		regs->o2 = ssp->o2;
		regs->o3 = ssp->o3;
		regs->o4 = ssp->o4;
		regs->o5 = ssp->o5;
		regs->o7 = ssp->o7;

		regs->l0 = ssp->l0;
		regs->l1 = ssp->l1;
		regs->l2 = ssp->l2;
		regs->l3 = ssp->l3;
		regs->l4 = ssp->l4;
		regs->l5 = ssp->l5;
		regs->l6 = ssp->l6;
		regs->l7 = ssp->l7;

		regs->i0 = ssp->i0;
		regs->i1 = ssp->i1;
		regs->i2 = ssp->i2;
		regs->i3 = ssp->i3;
		regs->i4 = ssp->i4;
		regs->i5 = ssp->i5;
		regs->i6 = ssp->i6;
		regs->i7 = ssp->i7;

		regs->y = ssp->y;
	}

	if ( eit != NULL ) {
		eit->pc  = ssp->pc;
		eit->npc = ssp->npc;
		eit->psr = ssp->psr;
	}

	if ( cregs != NULL ) {
		cregs->sp = tcb->tskctxb.ssp;
	}
}
#endif /* USE_FUNC_TK_GET_REG or USE_FUNC_TD_GET_REG */

#if defined(USE_FUNC_TK_GET_REG)
/*
 * Get task register contents
 */
SYSCALL ER _tk_get_reg( ID tskid,
		T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		get_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_REG */

#if defined(USE_FUNC_TK_SET_CPR)
/*
 * Set task coprocessor register
 */
SYSCALL ER _tk_set_cpr( ID tskid, INT copno, CONST T_COPREGS *pk_copregs )
{
	ATR	copatr = TA_COP0 << copno;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);
	CHECK_PAR((copatr & available_cop) != 0);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else if ( (tcb->tskatr & copatr) == 0 ) {
		ercd = E_PAR;
	} else {
		FPUContext *fpu = tcb->isstack;

		if ( fpu_ctxtsk == tcb ) fpu_ctxtsk = NULL;

		memcpy(fpu->f, pk_copregs->cop0.f, sizeof(UW)*32);
		fpu->fsr = pk_copregs->cop0.fsr;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_CPR */

#if defined(USE_FUNC_TK_GET_CPR)
/*
 * Get task coprocessor register
 */
SYSCALL ER _tk_get_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs )
{
	ATR	copatr = TA_COP0 << copno;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);
	CHECK_PAR((copatr & available_cop) != 0);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else if ( (tcb->tskatr & copatr) == 0 ) {
		ercd = E_PAR;
	} else {
		FPUContext *fpu = tcb->isstack;

		if ( fpu_ctxtsk == tcb ) save_fpuctx(fpu);

		memcpy(pk_copregs->cop0.f, fpu->f, sizeof(UW)*32);
		pk_copregs->cop0.fsr = fpu->fsr;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_CPR */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if defined(USE_FUNC_TD_SET_REG)
/*
 * Set task register
 */
SYSCALL ER _td_set_reg( ID tskid, CONST T_REGS *regs,
				CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		set_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SET_REG */

#if defined(USE_FUNC_TD_GET_REG)
/*
 * Get task register
 */
SYSCALL ER _td_get_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		get_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_GET_REG */

#endif /* USE_DBGSPT */
