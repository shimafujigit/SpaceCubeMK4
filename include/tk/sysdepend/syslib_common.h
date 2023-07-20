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
 *	@(#)syslib_common.h (tk)
 *
 *	System Library
 */

#ifndef __TK_SYSLIB_COMMON_H__
#define __TK_SYSLIB_COMMON_H__

/*
 * CPU-dependent
 */
#if STD_SH7727
#  include <tk/sysdepend/std_sh7727/syslib_depend.h>
#endif
#if STD_SH7751R
#  include <tk/sysdepend/std_sh7751r/syslib_depend.h>
#endif
#if MIC_M32104
#  include <tk/sysdepend/mic_m32104/syslib_depend.h>
#endif
#if STD_S1C38K
#  include <tk/sysdepend/std_s1c38k/syslib_depend.h>
#endif
#if STD_MC9328
#  include <tk/sysdepend/std_mc9328/syslib_depend.h>
#endif
#if MIC_VR4131
#  include <tk/sysdepend/mic_vr4131/syslib_depend.h>
#endif
#if STD_VR5500
#  include <tk/sysdepend/std_vr5500/syslib_depend.h>
#endif
#if STD_MB87Q1100
#  include <tk/sysdepend/std_mb87q1100/syslib_depend.h>
#endif
#if STD_SH7760
#  include <tk/sysdepend/std_sh7760/syslib_depend.h>
#endif
#if TEF_EM1D
#  include <tk/sysdepend/tef_em1d/syslib_depend.h>
#endif
#if ASP_SH7254R
#  include <tk/sysdepend/asp_sh7254r/syslib_depend.h>
#endif
#if ASP_RH850P1M
#  include <tk/sysdepend/asp_rh850p1m/syslib_depend.h>
#endif
#if ASP_LEON5
#  include <tk/sysdepend/asp_leon5/syslib_depend.h>
#endif

#endif /* __TK_SYSLIB_COMMON_H__ */
