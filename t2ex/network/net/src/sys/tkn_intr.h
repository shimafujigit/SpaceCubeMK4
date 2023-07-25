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
 *	@(#)tkn_intr.h
 *
 */

#ifndef _TKN_INTR_H_
#define _TKN_INTR_H_

/*
 * A higher value means a higher priority.
 */
#define IPL_NONE	0
#define IPL_SOFTCLOCK	1
#define IPL_SOFTBIO	2
#define IPL_SOFTNET	3
#define IPL_SOFTSERIAL	4
#define IPL_VM		5
#define IPL_SCHED	6
#define IPL_HIGH	7

#define NIPL		8

int tkn_spl_lock(int level);
int tkn_spl_unlock(int level);

#define	splsoftnet()	tkn_spl_lock(IPL_SOFTNET)
//#define	splnet()	tkn_spl_lock(IPL_NET)
#define	splvm()		tkn_spl_lock(IPL_VM)
#define	splsched()	tkn_spl_lock(IPL_SCHED)
#define	splhigh()	tkn_spl_lock(IPL_HIGH)

#define	spl0()		tkn_spl_unlock(IPL_NONE)
#define	splx(x)		tkn_spl_unlock(x)

#endif /* !_TKN_INTR_H_ */
