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
 *	cpu_conf.h (SH7750R)
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
 * Definition of minimum system stack size
 *	Minimum system stack size when setting the system stack size
 *	per task by 'tk_cre_tsk().'
 */
#define MIN_SYS_STACK_SIZE	1024

/*
 * Maximum logical space ID 
 */
#define MAX_LSID		255

#endif /* _CPU_CONF_ */
