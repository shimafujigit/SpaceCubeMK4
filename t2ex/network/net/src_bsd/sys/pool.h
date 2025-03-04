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

/*	$NetBSD: pool.h,v 1.64 2008/07/04 16:38:59 ad Exp $	*/

/*-
 * Copyright (c) 1997, 1998, 1999, 2000, 2007 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Paul Kranenburg; by Jason R. Thorpe of the Numerical Aerospace
 * Simulation Facility, NASA Ames Research Center.
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

#ifndef _SYS_POOL_H_
#define _SYS_POOL_H_

#ifdef _KERNEL
#define	__POOL_EXPOSE
#endif

#if defined(_KERNEL_OPT)
#include "opt_pool.h"
#endif

#ifdef __POOL_EXPOSE
#include <sys/mutex.h>
#include <sys/condvar.h>
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#include <sys/time.h>
#include <sys/tree.h>
#include <sys/callback.h>
#endif

#define	POOL_PADDR_INVALID	((paddr_t) -1)

#ifdef __POOL_EXPOSE
struct pool;

struct pool_allocator {
	void		*(*pa_alloc)(struct pool *, int);
	void		(*pa_free)(struct pool *, void *);
	unsigned int	pa_pagesz;

	/* The following fields are for internal use only. */
	kmutex_t	pa_lock;
	TAILQ_HEAD(, pool) pa_list;	/* list of pools using this allocator */
	int		pa_flags;
#define	PA_INITIALIZED	0x01
	int		pa_pagemask;
	int		pa_pageshift;
	struct vm_map *pa_backingmap;
#if defined(_KERNEL)
	struct vm_map **pa_backingmapptr;
	SLIST_ENTRY(pool_allocator) pa_q;
#endif /* defined(_KERNEL) */
};

LIST_HEAD(pool_pagelist,pool_item_header);

struct pool {
	TAILQ_ENTRY(pool)
			pr_poollist;
	struct pool_pagelist
			pr_emptypages;	/* Empty pages */
	struct pool_pagelist
			pr_fullpages;	/* Full pages */
	struct pool_pagelist
			pr_partpages;	/* Partially-allocated pages */
	struct pool_item_header	*pr_curpage;
	struct pool	*pr_phpool;	/* Pool item header pool */
	struct pool_cache *pr_cache;	/* Cache for this pool */
	unsigned int	pr_size;	/* Size of item */
	unsigned int	pr_align;	/* Requested alignment, must be 2^n */
	unsigned int	pr_itemoffset;	/* Align this offset in item */
	unsigned int	pr_minitems;	/* minimum # of items to keep */
	unsigned int	pr_minpages;	/* same in page units */
	unsigned int	pr_maxpages;	/* maximum # of pages to keep */
	unsigned int	pr_npages;	/* # of pages allocated */
	unsigned int	pr_itemsperpage;/* # items that fit in a page */
	unsigned int	pr_slack;	/* unused space in a page */
	unsigned int	pr_nitems;	/* number of available items in pool */
	unsigned int	pr_nout;	/* # items currently allocated */
	unsigned int	pr_hardlimit;	/* hard limit to number of allocated
					   items */
	unsigned int	pr_refcnt;	/* ref count for pagedaemon, etc */
	struct pool_allocator *pr_alloc;/* back-end allocator */
	TAILQ_ENTRY(pool) pr_alloc_list;/* link on allocator's pool list */

	/* Drain hook. */
	void		(*pr_drain_hook)(void *, int);
	void		*pr_drain_hook_arg;

	const char	*pr_wchan;	/* tsleep(9) identifier */
	unsigned int	pr_flags;	/* r/w flags */
	unsigned int	pr_roflags;	/* r/o flags */
#define	PR_WAITOK	0x01	/* Note: matches KM_SLEEP */
#define PR_NOWAIT	0x02	/* Note: matches KM_NOSLEEP */
#define PR_WANTED	0x04
#define PR_PHINPAGE	0x40
#define PR_LOGGING	0x80
#define PR_LIMITFAIL	0x100	/* even if waiting, fail if we hit limit */
#define PR_RECURSIVE	0x200	/* pool contains pools, for vmstat(8) */
#define PR_NOTOUCH	0x400	/* don't use free items to keep internal state*/
#define PR_NOALIGN	0x800	/* don't assume backend alignment */
#define	PR_LARGECACHE	0x1000	/* use large cache groups */

	/*
	 * `pr_lock' protects the pool's data structures when removing
	 * items from or returning items to the pool, or when reading
	 * or updating read/write fields in the pool descriptor.
	 *
	 * We assume back-end page allocators provide their own locking
	 * scheme.  They will be called with the pool descriptor _unlocked_,
	 * since the page allocators may block.
	 */
	kmutex_t	pr_lock;
	kcondvar_t	pr_cv;
	int		pr_ipl;

	SPLAY_HEAD(phtree, pool_item_header) pr_phtree;

	int		pr_maxcolor;	/* Cache colouring */
	int		pr_curcolor;
	int		pr_phoffset;	/* Offset in page of page header */

	/*
	 * Warning message to be issued, and a per-time-delta rate cap,
	 * if the hard limit is reached.
	 */
	const char	*pr_hardlimit_warning;
	struct timeval	pr_hardlimit_ratecap;
	struct timeval	pr_hardlimit_warning_last;

	/*
	 * Instrumentation
	 */
	unsigned long	pr_nget;	/* # of successful requests */
	unsigned long	pr_nfail;	/* # of unsuccessful requests */
	unsigned long	pr_nput;	/* # of releases */
	unsigned long	pr_npagealloc;	/* # of pages allocated */
	unsigned long	pr_npagefree;	/* # of pages released */
	unsigned int	pr_hiwat;	/* max # of pages in pool */
	unsigned long	pr_nidle;	/* # of idle pages */

	/*
	 * Diagnostic aides.
	 */
	struct pool_log	*pr_log;
	int		pr_curlogentry;
	int		pr_logsize;

	const char	*pr_entered_file; /* reentrancy check */
	long		pr_entered_line;

	struct callback_entry pr_reclaimerentry;
	void		*pr_freecheck;
	void		*pr_qcache;
};

/*
 * Cache group sizes, assuming 4-byte paddr_t on !_LP64.
 * All groups will be aligned to CACHE_LINE_SIZE.
 */
#ifdef _LP64
#define	PCG_NOBJECTS_NORMAL	15	/* 256 byte group */
#define	PCG_NOBJECTS_LARGE	63	/* 1024 byte group */
#else
#define	PCG_NOBJECTS_NORMAL	14	/* 124 byte group */
#define	PCG_NOBJECTS_LARGE	62	/* 508 byte group */
#endif

typedef struct pcgpair {
	void	*pcgo_va;		/* object virtual address */
	paddr_t	pcgo_pa;		/* object physical address */
} pcgpair_t;

/* The pool cache group. */
typedef struct pool_cache_group {
	struct pool_cache_group	*pcg_next;	/* link to next group */
	u_int			pcg_avail;	/* # available objects */
	u_int			pcg_size;	/* max number objects */
	pcgpair_t 		pcg_objects[1];	/* the objects */
} pcg_t;

typedef struct pool_cache_cpu {
	uint64_t		cc_misses;
	uint64_t		cc_hits;
	struct pool_cache_group	*cc_current;
	struct pool_cache_group	*cc_previous;	
	struct pool_cache	*cc_cache;
	int			cc_ipl;
	int			cc_cpuindex;
#ifndef T2EX
#ifdef _KERNEL
	ipl_cookie_t		cc_iplcookie;
#endif
#endif
} pool_cache_cpu_t;

struct pool_cache {
	/* Pool layer. */
	struct pool	pc_pool;
	
	/* Cache layer. */
	kmutex_t	pc_lock;	/* locks cache layer */
	TAILQ_ENTRY(pool_cache)
			pc_cachelist;	/* entry on global cache list */
	pcg_t		*pc_emptygroups;/* list of empty cache groups */
	pcg_t		*pc_fullgroups;	/* list of full cache groups */
	pcg_t		*pc_partgroups;	/* groups for reclamation */
	struct pool	*pc_pcgpool;	/* Pool of cache groups */
	int		pc_pcgsize;	/* Use large cache groups? */
	int		pc_ncpu;	/* number cpus set up */
	int		(*pc_ctor)(void *, void *, int);
	void		(*pc_dtor)(void *, void *);
	void		*pc_arg;	/* for ctor/ctor */
	uint64_t	pc_hits;	/* cache layer hits */
	uint64_t	pc_misses;	/* cache layer misses */
	uint64_t	pc_contended;	/* contention events on cache */
	unsigned int	pc_nempty;	/* empty groups in cache */
	unsigned int	pc_nfull;	/* full groups in cache */
	unsigned int	pc_npart;	/* partial groups in cache */
	unsigned int	pc_refcnt;	/* ref count for pagedaemon, etc */
	void		*pc_freecheck;

	/* CPU layer. */
	pool_cache_cpu_t pc_cpu0 __aligned(CACHE_LINE_SIZE);
	void		*pc_cpus[MAXCPUS] __aligned(CACHE_LINE_SIZE);
};

#endif /* __POOL_EXPOSE */

typedef struct pool_cache *pool_cache_t;

#ifdef _KERNEL
/*
 * pool_allocator_kmem is the default that all pools get unless
 * otherwise specified.  pool_allocator_nointr is provided for
 * pools that know they will never be accessed in interrupt
 * context.
 */
extern struct pool_allocator pool_allocator_kmem;
extern struct pool_allocator pool_allocator_nointr;
extern struct pool_allocator pool_allocator_meta;
#ifdef POOL_SUBPAGE
/* The above are subpage allocators in this case. */
extern struct pool_allocator pool_allocator_kmem_fullpage;
extern struct pool_allocator pool_allocator_nointr_fullpage;
#endif

#ifndef T2EX
struct link_pool_init {	/* same as args to pool_init() */
	struct pool *pp;
	size_t size;
	u_int align;
	u_int align_offset;
	int flags;
	const char *wchan;
	struct pool_allocator *palloc;
	int ipl;
};
#define	POOL_INIT(pp, size, align, align_offset, flags, wchan, palloc, ipl)\
struct pool pp;								\
static const struct link_pool_init _link_ ## pp[1] = {			\
	{ &pp, size, align, align_offset, flags, wchan, palloc, ipl }	\
};									\
__link_set_add_rodata(pools, _link_ ## pp)
#endif

#ifndef T2EX
void		pool_subsystem_init(void);

void		pool_init(struct pool *, size_t, u_int, u_int,
		    int, const char *, struct pool_allocator *, int);
#else
int		pool_subsystem_init(void);
int		pool_subsystem_finish(void);

int		pool_init(struct pool *, size_t, u_int, u_int,
		    int, const char *, struct pool_allocator *, int);
#endif
void		pool_destroy(struct pool *);

void		pool_set_drain_hook(struct pool *,
		    void (*)(void *, int), void *);

void		*pool_get(struct pool *, int);
void		pool_put(struct pool *, void *);
int		pool_reclaim(struct pool *);

#ifdef POOL_DIAGNOSTIC
/*
 * These versions do reentrancy checking.
 */
void		*_pool_get(struct pool *, int, const char *, long);
void		_pool_put(struct pool *, void *, const char *, long);
int		_pool_reclaim(struct pool *, const char *, long);
#define		pool_get(h, f)	_pool_get((h), (f), __FILE__, __LINE__)
#define		pool_put(h, v)	_pool_put((h), (v), __FILE__, __LINE__)
#define		pool_reclaim(h)	_pool_reclaim((h), __FILE__, __LINE__)
#endif /* POOL_DIAGNOSTIC */

int		pool_prime(struct pool *, int);
void		pool_setlowat(struct pool *, int);
void		pool_sethiwat(struct pool *, int);
void		pool_sethardlimit(struct pool *, int, const char *, int);
void		pool_drain_start(struct pool **, uint64_t *);
void		pool_drain_end(struct pool *, uint64_t);

/*
 * Debugging and diagnostic aides.
 */
void		pool_print(struct pool *, const char *);
void		pool_printit(struct pool *, const char *,
		    void (*)(const char *, ...));
void		pool_printall(const char *, void (*)(const char *, ...));
int		pool_chk(struct pool *, const char *);

/*
 * Pool cache routines.
 */
pool_cache_t	pool_cache_init(size_t, u_int, u_int, u_int, const char *,
		    struct pool_allocator *, int, int (*)(void *, void *, int),
		    void (*)(void *, void *), void *);
#ifndef T2EX
void		pool_cache_bootstrap(pool_cache_t, size_t, u_int, u_int, u_int,
		    const char *, struct pool_allocator *, int,
		    int (*)(void *, void *, int), void (*)(void *, void *),
		    void *);
#else
int		pool_cache_bootstrap(pool_cache_t, size_t, u_int, u_int, u_int,
		    const char *, struct pool_allocator *, int,
		    int (*)(void *, void *, int), void (*)(void *, void *),
		    void *);
#endif
void		pool_cache_destroy(pool_cache_t);
void		*pool_cache_get_paddr(pool_cache_t, int, paddr_t *);
void		pool_cache_put_paddr(pool_cache_t, void *, paddr_t);
void		pool_cache_destruct_object(pool_cache_t, void *);
void		pool_cache_invalidate(pool_cache_t);
bool		pool_cache_reclaim(pool_cache_t);
void		pool_cache_set_drain_hook(pool_cache_t,
		    void (*)(void *, int), void *);
void		pool_cache_setlowat(pool_cache_t, int);
void		pool_cache_sethiwat(pool_cache_t, int);
void		pool_cache_sethardlimit(pool_cache_t, int, const char *, int);
#ifndef T2EX
void		pool_cache_cpu_init(struct cpu_info *);
#else
int		pool_cache_cpu_init(struct cpu_info *);
int		pool_cache_cpu_finish(struct cpu_info *);
#endif

#define		pool_cache_get(pc, f) pool_cache_get_paddr((pc), (f), NULL)
#define		pool_cache_put(pc, o) pool_cache_put_paddr((pc), (o), \
				          POOL_PADDR_INVALID)

void 		pool_whatis(uintptr_t, void (*)(const char *, ...));
#endif /* _KERNEL */

#endif /* _SYS_POOL_H_ */
