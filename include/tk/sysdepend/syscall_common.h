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
 *	@(#)syscall_common.h (tk)
 *
 *	T-Kernel/OS
 */

#ifndef __TK_SYSCALL_COMMON_H__
#define __TK_SYSCALL_COMMON_H__

#if STD_SH7727
#  include <tk/sysdepend/std_sh7727/cpuattr.h>
#  include <tk/sysdepend/std_sh7727/cpudef.h>
#endif
#if STD_SH7751R
#  include <tk/sysdepend/std_sh7751r/cpuattr.h>
#  include <tk/sysdepend/std_sh7751r/cpudef.h>
#endif
#if MIC_M32104
#  include <tk/sysdepend/mic_m32104/cpuattr.h>
#  include <tk/sysdepend/mic_m32104/cpudef.h>
#endif
#if STD_S1C38K
#  include <tk/sysdepend/std_s1c38k/cpuattr.h>
#  include <tk/sysdepend/std_s1c38k/cpudef.h>
#endif
#if STD_MC9328
#  include <tk/sysdepend/std_mc9328/cpuattr.h>
#  include <tk/sysdepend/std_mc9328/cpudef.h>
#endif
#if MIC_VR4131
#  include <tk/sysdepend/mic_vr4131/cpuattr.h>
#  include <tk/sysdepend/mic_vr4131/cpudef.h>
#endif
#if STD_VR5500
#  include <tk/sysdepend/std_vr5500/cpuattr.h>
#  include <tk/sysdepend/std_vr5500/cpudef.h>
#endif
#if STD_MB87Q1100
#  include <tk/sysdepend/std_mb87q1100/cpuattr.h>
#  include <tk/sysdepend/std_mb87q1100/cpudef.h>
#endif
#if STD_SH7760
#  include <tk/sysdepend/std_sh7760/cpuattr.h>
#  include <tk/sysdepend/std_sh7760/cpudef.h>
#endif
#if TEF_EM1D
#  include <tk/sysdepend/tef_em1d/cpuattr.h>
#  include <tk/sysdepend/tef_em1d/cpudef.h>
#endif
#if ASP_SH7254R
#  include <tk/sysdepend/asp_sh7254r/cpuattr.h>
#  include <tk/sysdepend/asp_sh7254r/cpudef.h>
#endif
#if ASP_RH850P1M
#  include <tk/sysdepend/asp_rh850p1m/cpuattr.h>
#  include <tk/sysdepend/asp_rh850p1m/cpudef.h>
#endif
#if ASP_LEON5
#  include <tk/sysdepend/asp_leon5/cpuattr.h>
#  include <tk/sysdepend/asp_leon5/cpudef.h>
#endif

#endif /* __TK_SYSCALL_COMMON_H__ */
