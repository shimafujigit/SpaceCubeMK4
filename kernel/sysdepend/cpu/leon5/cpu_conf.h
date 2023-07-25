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
 *	cpu_conf.h (ASP-LEON5)
 *	CPU-Dependent OS Configuration Information
 */

#ifndef _CPU_CONF_
#define _CPU_CONF_

/*
 * Define 1 when using MMU
 *   0: not using MMU
 *   1: using MMU
 */
#define USE_MMU			0

/*
 * Integrate user stacks and system stacks together at all protected levels.
 *   0: not using Single Stack
 *   1: using Single Stack
 */
#define USE_SINGLE_STACK	1

/*
 * Minimum ring level for user tasks (valid only if USE_MMU = 1)
 *   [0, 3]
 */
#define MMU_MIN_USER_LEVEL	2		/* Added for T2EX */

/*
 * Definition of minimum system stack size
 *	Minimum system stack size when setting the system stack size
 *	per task by 'tk_cre_tsk().'
 */
#define MIN_SYS_STACK_SIZE	512

/*
 * Maximum logical space ID
 */
#define MAX_LSID		0xff	/* (not support) MMU context register */

#endif /* _CPU_CONF_ */
