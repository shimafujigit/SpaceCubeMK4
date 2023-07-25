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

/* $NetBSD: pmf.h,v 1.12 2008/05/05 00:16:18 jmcneill Exp $ */

/*-
 * Copyright (c) 2007 Jared D. McNeill <jmcneill@invisible.ca>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SYS_PMF_H
#define _SYS_PMF_H

#ifdef _KERNEL

typedef enum {
	PMFE_DISPLAY_ON,
	PMFE_DISPLAY_REDUCED,
	PMFE_DISPLAY_STANDBY,
	PMFE_DISPLAY_SUSPEND,
	PMFE_DISPLAY_OFF,
	PMFE_DISPLAY_BRIGHTNESS_UP,
	PMFE_DISPLAY_BRIGHTNESS_DOWN,
	PMFE_AUDIO_VOLUME_UP,
	PMFE_AUDIO_VOLUME_DOWN,
	PMFE_AUDIO_VOLUME_TOGGLE,
	PMFE_CHASSIS_LID_CLOSE,
	PMFE_CHASSIS_LID_OPEN
} pmf_generic_event_t;

enum pmf_flags {
	  PMF_F_NONE = 0x0
	, PMF_F_SELF = 0x1
};

typedef enum pmf_flags pmf_flags_t;

#define	PMF_FN_PROTO1	pmf_flags_t
#define	PMF_FN_ARGS1	pmf_flags_t flags

#define	PMF_FN_PROTO	, pmf_flags_t
#define	PMF_FN_ARGS	, pmf_flags_t flags
#define	PMF_FN_CALL	, flags

#define PMF_FLAGS_FMT	"%x" /* "n/a" */

void	pmf_init(void);

bool	pmf_event_inject(device_t, pmf_generic_event_t);
bool	pmf_event_register(device_t, pmf_generic_event_t,
			   void (*)(device_t), bool);
void	pmf_event_deregister(device_t, pmf_generic_event_t,
			     void (*)(device_t), bool);

bool		pmf_set_platform(const char *, const char *);
const char	*pmf_get_platform(const char *);

bool		pmf_system_resume(PMF_FN_PROTO1);
bool		pmf_system_bus_resume(PMF_FN_PROTO1);
bool		pmf_system_suspend(PMF_FN_PROTO1);
void		pmf_system_shutdown(int);

bool		pmf_device_register1(device_t,
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t, int));
/* compatibility */
#define pmf_device_register(__d, __s, __r) \
	pmf_device_register1((__d), (__s), (__r), NULL)

void		pmf_device_deregister(device_t);
bool		pmf_device_suspend(device_t PMF_FN_PROTO);
bool		pmf_device_resume(device_t PMF_FN_PROTO);

bool		pmf_device_suspend_self(device_t);
bool		pmf_device_resume_self(device_t);

bool		pmf_device_recursive_suspend(device_t PMF_FN_PROTO);
bool		pmf_device_recursive_resume(device_t PMF_FN_PROTO);
bool		pmf_device_resume_subtree(device_t PMF_FN_PROTO);

struct ifnet;
void		pmf_class_network_register(device_t, struct ifnet *);

bool		pmf_class_input_register(device_t);
bool		pmf_class_display_register(device_t);

#endif /* !_KERNEL */

#endif /* !_SYS_PMF_H */
