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

/* $NetBSD: device.h,v 1.112.6.1 2009/03/15 19:43:48 snj Exp $ */

/*
 * Copyright (c) 1996, 2000 Christopher G. Demetriou
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *          This product includes software developed for the
 *          NetBSD Project.  See http://www.NetBSD.org/ for
 *          information about NetBSD.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * --(license Id: LICENSE.proto,v 1.1 2000/06/13 21:40:26 cgd Exp )--
 */

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This software was developed by the Computer Systems Engineering group
 * at Lawrence Berkeley Laboratory under DARPA contract BG 91-66 and
 * contributed to Berkeley.
 *
 * All advertising materials mentioning features or use of this software
 * must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Lawrence Berkeley Laboratories.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)device.h	8.2 (Berkeley) 2/17/94
 */

#ifndef _SYS_DEVICE_H_
#define	_SYS_DEVICE_H_

#include <sys/evcnt.h>
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif

typedef struct device *device_t;
#ifdef _KERNEL
#include <sys/pmf.h>
#endif

#ifndef T2EX
#include <prop/proplib.h>
#endif

/*
 * Minimal device structures.
 * Note that all ``system'' device types are listed here.
 */
typedef enum devclass {
	DV_DULL,		/* generic, no special info */
	DV_CPU,			/* CPU (carries resource utilization) */
	DV_DISK,		/* disk drive (label, etc) */
	DV_IFNET,		/* network interface */
	DV_TAPE,		/* tape device */
	DV_TTY,			/* serial line interface (?) */
	DV_AUDIODEV,		/* audio device */
	DV_DISPLAYDEV,		/* display device */
	DV_BUS			/* bus device */
} devclass_t;

/*
 * Actions for ca_activate.
 */
typedef enum devact {
	DVACT_ACTIVATE,		/* activate the device */
	DVACT_DEACTIVATE	/* deactivate the device */
} devact_t;

typedef enum {
	DVA_SYSTEM,
	DVA_HARDWARE
} devactive_t;

typedef struct cfdata *cfdata_t;
typedef struct cfdriver *cfdriver_t;
typedef struct cfattach *cfattach_t;

struct device {
	devclass_t	dv_class;	/* this device's classification */
	TAILQ_ENTRY(device) dv_list;	/* entry on list of all devices */
	cfdata_t	dv_cfdata;	/* config data that found us
					   (NULL if pseudo-device) */
	cfdriver_t	dv_cfdriver;	/* our cfdriver */
	cfattach_t	dv_cfattach;	/* our cfattach */
	int		dv_unit;	/* device unit number */
	char		dv_xname[16];	/* external name (name + unit) */
	device_t	dv_parent;	/* pointer to parent device
					   (NULL if pseudo- or root node) */
	int		dv_depth;	/* number of parents until root */
	int		dv_flags;	/* misc. flags; see below */
	void		*dv_private;	/* this device's private storage */
	int		*dv_locators;	/* our actual locators (optional) */
#ifndef T2EX
	prop_dictionary_t dv_properties;/* properties dictionary */
#endif

	size_t		dv_activity_count;
	void		(**dv_activity_handlers)(device_t, devactive_t);

	bool		(*dv_driver_suspend)(device_t PMF_FN_PROTO);
	bool		(*dv_driver_resume)(device_t PMF_FN_PROTO);
	bool		(*dv_driver_shutdown)(device_t, int);
	bool		(*dv_driver_child_register)(device_t);

	void		*dv_bus_private;
	bool		(*dv_bus_suspend)(device_t PMF_FN_PROTO);
	bool		(*dv_bus_resume)(device_t PMF_FN_PROTO);
	bool		(*dv_bus_shutdown)(device_t, int);
	void		(*dv_bus_deregister)(device_t);

	void		*dv_class_private;
	bool		(*dv_class_suspend)(device_t PMF_FN_PROTO);
	bool		(*dv_class_resume)(device_t PMF_FN_PROTO);
	void		(*dv_class_deregister)(device_t);

	void		*dv_pmf_private;
};

/* dv_flags */
#define	DVF_ACTIVE		0x0001	/* device is activated */
#define	DVF_PRIV_ALLOC		0x0002	/* device private storage != device */
#define	DVF_POWER_HANDLERS	0x0004	/* device has suspend/resume support */
#define	DVF_CLASS_SUSPENDED	0x0008	/* device class suspend was called */
#define	DVF_DRIVER_SUSPENDED	0x0010	/* device driver suspend was called */
#define	DVF_BUS_SUSPENDED	0x0020	/* device bus suspend was called */
#define	DVF_SELF_SUSPENDED	0x0040	/* device suspended itself */

TAILQ_HEAD(devicelist, device);

enum deviter_flags {
	  DEVITER_F_RW =		0x1
	, DEVITER_F_SHUTDOWN =		0x2
	, DEVITER_F_LEAVES_FIRST =	0x4
	, DEVITER_F_ROOT_FIRST =	0x8
};

typedef enum deviter_flags deviter_flags_t;

struct deviter {
	device_t	di_prev;
	deviter_flags_t	di_flags;
	int		di_curdepth;
	int		di_maxdepth;
};

typedef struct deviter deviter_t;

/*
 * Description of a locator, as part of interface attribute definitions.
 */
struct cflocdesc {
	const char *cld_name;
	const char *cld_defaultstr; /* NULL if no default */
	int cld_default;
};

/*
 * Description of an interface attribute, provided by potential
 * parent device drivers, referred to by child device configuration data.
 */
struct cfiattrdata {
	const char *ci_name;
	int ci_loclen;
	const struct cflocdesc ci_locdesc[
#if defined(__GNUC__) && __GNUC__ <= 2
		0
#endif
	];
};

/*
 * Description of a configuration parent.  Each device attachment attaches
 * to an "interface attribute", which is given in this structure.  The parent
 * *must* carry this attribute.  Optionally, an individual device instance
 * may also specify a specific parent device instance.
 */
struct cfparent {
	const char *cfp_iattr;		/* interface attribute */
	const char *cfp_parent;		/* optional specific parent */
	int cfp_unit;			/* optional specific unit
					   (-1 to wildcard) */
};

/*
 * Configuration data (i.e., data placed in ioconf.c).
 */
struct cfdata {
	const char *cf_name;		/* driver name */
	const char *cf_atname;		/* attachment name */
	short	cf_unit;		/* unit number */
	short	cf_fstate;		/* finding state (below) */
	int	*cf_loc;		/* locators (machine dependent) */
	int	cf_flags;		/* flags from config */
	const struct cfparent *cf_pspec;/* parent specification */
};
#define FSTATE_NOTFOUND		0	/* has not been found */
#define	FSTATE_FOUND		1	/* has been found */
#define	FSTATE_STAR		2	/* duplicable */
#define FSTATE_DSTAR		3	/* has not been found, and disabled */
#define FSTATE_DNOTFOUND	4	/* duplicate, and disabled */

/*
 * Multiple configuration data tables may be maintained.  This structure
 * provides the linkage.
 */
struct cftable {
	cfdata_t	ct_cfdata;	/* pointer to cfdata table */
	TAILQ_ENTRY(cftable) ct_list;	/* list linkage */
};
TAILQ_HEAD(cftablelist, cftable);

typedef int (*cfsubmatch_t)(device_t, cfdata_t, const int *, void *);

/*
 * `configuration' attachment and driver (what the machine-independent
 * autoconf uses).  As devices are found, they are applied against all
 * the potential matches.  The one with the best match is taken, and a
 * device structure (plus any other data desired) is allocated.  Pointers
 * to these are placed into an array of pointers.  The array itself must
 * be dynamic since devices can be found long after the machine is up
 * and running.
 *
 * Devices can have multiple configuration attachments if they attach
 * to different attributes (busses, or whatever), to allow specification
 * of multiple match and attach functions.  There is only one configuration
 * driver per driver, so that things like unit numbers and the device
 * structure array will be shared.
 */
struct cfattach {
	const char *ca_name;		/* name of attachment */
	LIST_ENTRY(cfattach) ca_list;	/* link on cfdriver's list */
	size_t	  ca_devsize;		/* size of dev data (for malloc) */
	int	  ca_flags;		/* flags for driver allocation etc */
	int	(*ca_match)(device_t, cfdata_t, void *);
	void	(*ca_attach)(device_t, device_t, void *);
	int	(*ca_detach)(device_t, int);
	int	(*ca_activate)(device_t, devact_t);
	/* technically, the next 2 belong into "struct cfdriver" */
	int	(*ca_rescan)(device_t, const char *,
			     const int *); /* scan for new children */
	void	(*ca_childdetached)(device_t, device_t);
};
LIST_HEAD(cfattachlist, cfattach);

#define	CFATTACH_DECL2(name, ddsize, matfn, attfn, detfn, actfn, \
	rescanfn, chdetfn) \
struct cfattach __CONCAT(name,_ca) = {					\
	.ca_name		= ___STRING(name),			\
	.ca_devsize		= ddsize,				\
	.ca_match 		= matfn,				\
	.ca_attach		= attfn,				\
	.ca_detach		= detfn,				\
	.ca_activate		= actfn,				\
	.ca_rescan		= rescanfn,				\
	.ca_childdetached	= chdetfn,				\
}

#define	CFATTACH_DECL(name, ddsize, matfn, attfn, detfn, actfn)		\
	CFATTACH_DECL2(name, ddsize, matfn, attfn, detfn, actfn, NULL, NULL)

#define	CFATTACH_DECL2_NEW(name, ddsize, matfn, attfn, detfn, actfn, \
	rescanfn, chdetfn) \
struct cfattach __CONCAT(name,_ca) = {					\
	.ca_name		= ___STRING(name),			\
	.ca_devsize		= ddsize,				\
	.ca_flags		= DVF_PRIV_ALLOC,			\
	.ca_match 		= matfn,				\
	.ca_attach		= attfn,				\
	.ca_detach		= detfn,				\
	.ca_activate		= actfn,				\
	.ca_rescan		= rescanfn,				\
	.ca_childdetached	= chdetfn,				\
}

#define	CFATTACH_DECL_NEW(name, ddsize, matfn, attfn, detfn, actfn)		\
	CFATTACH_DECL2_NEW(name, ddsize, matfn, attfn, detfn, actfn, NULL, NULL)

/* Flags given to config_detach(), and the ca_detach function. */
#define	DETACH_FORCE	0x01		/* force detachment; hardware gone */
#define	DETACH_QUIET	0x02		/* don't print a notice */

struct cfdriver {
	LIST_ENTRY(cfdriver) cd_list;	/* link on allcfdrivers */
	struct cfattachlist cd_attach;	/* list of all attachments */
	device_t *cd_devs;		/* devices found */
	const char *cd_name;		/* device name */
	enum	devclass cd_class;	/* device classification */
	int	cd_ndevs;		/* size of cd_devs array */
	const struct cfiattrdata * const *cd_attrs; /* attributes provided */
};
LIST_HEAD(cfdriverlist, cfdriver);

#define	CFDRIVER_DECL(name, class, attrs)				\
struct cfdriver __CONCAT(name,_cd) = {					\
	.cd_name		= ___STRING(name),			\
	.cd_class		= class,				\
	.cd_attrs		= attrs,				\
}

/*
 * The cfattachinit is a data structure used to associate a list of
 * cfattach's with cfdrivers as found in the static kernel configuration.
 */
struct cfattachinit {
	const char *cfai_name;		 /* driver name */
	struct cfattach * const *cfai_list;/* list of attachments */
};
/*
 * the same, but with a non-constant list so it can be modified
 * for LKM bookkeeping
 */
struct cfattachlkminit {
	const char *cfai_name;		/* driver name */
	struct cfattach **cfai_list;	/* list of attachments */
};

/*
 * Configuration printing functions, and their return codes.  The second
 * argument is NULL if the device was configured; otherwise it is the name
 * of the parent device.  The return value is ignored if the device was
 * configured, so most functions can return UNCONF unconditionally.
 */
typedef int (*cfprint_t)(void *, const char *);		/* XXX const char * */
#define	QUIET	0		/* print nothing */
#define	UNCONF	1		/* print " not configured\n" */
#define	UNSUPP	2		/* print " not supported\n" */

/*
 * Pseudo-device attach information (function + number of pseudo-devs).
 */
struct pdevinit {
	void	(*pdev_attach)(int);
	int	pdev_count;
};

/* This allows us to wildcard a device unit. */
#define	DVUNIT_ANY	-1

#ifdef _KERNEL

extern struct cfdriverlist allcfdrivers;/* list of all cfdrivers */
extern struct devicelist alldevs;	/* list of all devices */
extern struct cftablelist allcftables;	/* list of all cfdata tables */
extern device_t booted_device;		/* the device we booted from */
extern device_t booted_wedge;		/* the wedge on that device */
extern int booted_partition;		/* or the partition on that device */

struct vnode *opendisk(struct device *);
int config_handle_wedges(struct device *, int);

void	config_init(void);
void	drvctl_init(void);
void	configure(void);
void	configure2(void);

int	config_cfdriver_attach(struct cfdriver *);
int	config_cfdriver_detach(struct cfdriver *);

int	config_cfattach_attach(const char *, struct cfattach *);
int	config_cfattach_detach(const char *, struct cfattach *);

int	config_cfdata_attach(cfdata_t, int);
int	config_cfdata_detach(cfdata_t);

struct cfdriver *config_cfdriver_lookup(const char *);
struct cfattach *config_cfattach_lookup(const char *, const char *);
const struct cfiattrdata *cfiattr_lookup(const char *, const struct cfdriver *);

int	config_stdsubmatch(device_t, cfdata_t, const int *, void *);
cfdata_t config_search_loc(cfsubmatch_t, device_t,
				 const char *, const int *, void *);
cfdata_t config_search_ia(cfsubmatch_t, device_t,
				 const char *, void *);
cfdata_t config_rootsearch(cfsubmatch_t, const char *, void *);
device_t config_found_sm_loc(device_t, const char *, const int *,
			     void *, cfprint_t, cfsubmatch_t);
device_t config_found_ia(device_t, const char *, void *, cfprint_t);
device_t config_found(device_t, void *, cfprint_t);
device_t config_rootfound(const char *, void *);
device_t config_attach_loc(device_t, cfdata_t, const int *, void *, cfprint_t);
device_t config_attach(device_t, cfdata_t, void *, cfprint_t);
int	config_match(device_t, cfdata_t, void *);

device_t config_attach_pseudo(cfdata_t);

int	config_detach(device_t, int);
int	config_detach_children(device_t, int flags);
int	config_activate(device_t);
int	config_deactivate(device_t);
void	config_defer(device_t, void (*)(device_t));
void	config_deferred(device_t);
void	config_interrupts(device_t, void (*)(device_t));
void	config_pending_incr(void);
void	config_pending_decr(void);

int	config_finalize_register(device_t, int (*)(device_t));
void	config_finalize(void);

device_t	device_lookup(cfdriver_t, int);
void		*device_lookup_private(cfdriver_t, int);
#ifdef __HAVE_DEVICE_REGISTER
void		device_register(device_t, void *);
#endif

devclass_t	device_class(device_t);
cfdata_t	device_cfdata(device_t);
cfdriver_t	device_cfdriver(device_t);
cfattach_t	device_cfattach(device_t);
int		device_unit(device_t);
const char	*device_xname(device_t);
device_t	device_parent(device_t);
bool		device_is_active(device_t);
bool		device_is_enabled(device_t);
bool		device_has_power(device_t);
int		device_locator(device_t, u_int);
void		*device_private(device_t);
#ifndef T2EX
prop_dictionary_t device_properties(device_t);
#endif

device_t	deviter_first(deviter_t *, deviter_flags_t);
void		deviter_init(deviter_t *, deviter_flags_t);
device_t	deviter_next(deviter_t *);
void		deviter_release(deviter_t *);

bool		device_active(device_t, devactive_t);
bool		device_active_register(device_t,
				       void (*)(device_t, devactive_t));
void		device_active_deregister(device_t,
				         void (*)(device_t, devactive_t));

bool		device_is_a(device_t, const char *);

device_t	device_find_by_xname(const char *);
device_t	device_find_by_driver_unit(const char *, int);

bool		device_pmf_is_registered(device_t);

bool		device_pmf_driver_suspend(device_t PMF_FN_PROTO);
bool		device_pmf_driver_resume(device_t PMF_FN_PROTO);
bool		device_pmf_driver_shutdown(device_t, int);

bool		device_pmf_driver_register(device_t,
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t, int));
void		device_pmf_driver_deregister(device_t);

bool		device_pmf_driver_child_register(device_t);
void		device_pmf_driver_set_child_register(device_t,
		    bool (*)(device_t));

void		*device_pmf_bus_private(device_t);
bool		device_pmf_bus_suspend(device_t PMF_FN_PROTO);
bool		device_pmf_bus_resume(device_t PMF_FN_PROTO);
bool		device_pmf_bus_shutdown(device_t, int);

void		*device_pmf_private(device_t);
void		device_pmf_unlock(device_t PMF_FN_PROTO);
bool		device_pmf_lock(device_t PMF_FN_PROTO);

bool		device_is_self_suspended(device_t);
void		device_pmf_self_suspend(device_t PMF_FN_PROTO);
void		device_pmf_self_resume(device_t PMF_FN_PROTO);
bool		device_pmf_self_wait(device_t PMF_FN_PROTO);

void		device_pmf_bus_register(device_t, void *,
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t, int),
		    void (*)(device_t));
void		device_pmf_bus_deregister(device_t);

void		*device_pmf_class_private(device_t);
bool		device_pmf_class_suspend(device_t PMF_FN_PROTO);
bool		device_pmf_class_resume(device_t PMF_FN_PROTO);

void		device_pmf_class_register(device_t, void *,
		    bool (*)(device_t PMF_FN_PROTO),
		    bool (*)(device_t PMF_FN_PROTO),
		    void (*)(device_t));
void		device_pmf_class_deregister(device_t);

#endif /* _KERNEL */

#endif /* !_SYS_DEVICE_H_ */
