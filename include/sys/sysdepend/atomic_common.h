/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*
 *	@(#)atomic_common.h (sys)
 *
 *	Atomic integer operations
 */

#ifndef __SYS_ATOMIC_COMMON_H__
#define __SYS_ATOMIC_COMMON_H__

#include <basic.h>
#include <tk/syslib.h>

#if TEF_EM1D
#  include <sys/sysdepend/tef_em1d/atomic_depend.h>
#endif
#if ASP_LEON5
/* Since there is no the ll/sc instruction in the SPARC V8 architecture, the following general implementation is used. */
/*#  include <sys/sysdepend/asp_leon5/atomic_depend.h>*/
#endif

/*
 * From here, generic implementations of atomic integer operations 
 * are defined. Each of these definitions is used only when 
 * ATOMIC_xxx_USER_MODE remains undefined at this point.
 *
 * The value of ATOMIC_xxx_USER_MODE shall be defined as follows:
 *	0: atomic_xxx implementation cannot be used in user mode
 *	1: atomic_xxx implementation can be used in user mode
 */

#ifndef ATOMIC_INC_USER_MODE
#define ATOMIC_INC_USER_MODE 0
Inline UINT atomic_inc(volatile UINT* addr)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = ++*addr;
	EI(imask);
	return re;
}
#endif /* ATOMIC_INC_USER_MODE */

#ifndef ATOMIC_DEC_USER_MODE
#define ATOMIC_DEC_USER_MODE 0
Inline UINT atomic_dec(volatile UINT* addr)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = --*addr;
	EI(imask);
	return re;
}
#endif /* ATOMIC_DEC_USER_MODE */

#ifndef ATOMIC_ADD_USER_MODE
#define ATOMIC_ADD_USER_MODE 0
Inline UINT atomic_add(volatile UINT* addr, UINT val)
{
	UINT imask;
	UINT re;
	DI(imask);
	*addr += val;
	re = *addr;
	EI(imask);
	return re;
}
#endif /* ATOMIC_ADD_USER_MODE */

#ifndef ATOMIC_SUB_USER_MODE
#define ATOMIC_SUB_USER_MODE 0
Inline UINT atomic_sub(volatile UINT* addr, UINT val)
{
	UINT imask;
	UINT re;
	DI(imask);
	*addr -= val;
	re = *addr;
	EI(imask);
	return re;
}
#endif /* ATOMIC_SUB_USER_MODE */

/* TAS: test-and-set */
#ifndef ATOMIC_XCHG_USER_MODE
#define ATOMIC_XCHG_USER_MODE 0
Inline UINT atomic_xchg(volatile UINT* addr, UINT val)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = *addr;
	*addr = val;
	EI(imask);
	return re;
}
#endif /* ATOMIC_XCHG_USER_MODE */

/* CAS: compare-and-swap */
#ifndef ATOMIC_CMPXCHG_USER_MODE
#define ATOMIC_CMPXCHG_USER_MODE 0
Inline UINT atomic_cmpxchg(volatile UINT* addr, UINT val, UINT cmp)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = *addr;
	if ( *addr == cmp ) {
		*addr = val;
	}
	EI(imask);
	return re;
}
#endif /* ATOMIC_CMPXCHG_USER_MODE */

#ifndef ATOMIC_BITSET_USER_MODE
#define ATOMIC_BITSET_USER_MODE 0
Inline UINT atomic_bitset(volatile UINT* addr, UINT setptn)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = *addr;
	*addr |= setptn;
	EI(imask);
	return re;
}
#endif /* ATOMIC_BITSET_USER_MODE */

#ifndef ATOMIC_BITCLR_USER_MODE
#define ATOMIC_BITCLR_USER_MODE 0
Inline UINT atomic_bitclr(volatile UINT* addr, UINT clrptn)
{
	UINT imask;
	UINT re;
	DI(imask);
	re = *addr;
	*addr &= clrptn;
	EI(imask);
	return re;
}
#endif /* ATOMIC_BITCLR_USER_MODE */

#endif /* __SYS_ATOMIC_COMMON_H__ */
