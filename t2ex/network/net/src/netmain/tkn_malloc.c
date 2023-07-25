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
 *	@(#)tkn_malloc.c
 *
 */

/* T-Kernel/Network memory allocation routines */

#include <tk/tkernel.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/syslog.h>

#include <netmain/tkn_malloc.h>


#ifndef TKN_MEMWAIT
#define TKN_MEMWAIT	10	// task wait time(ms) if cannot alloc memory
#endif
#ifndef TKN_MEMWAIT_LOG
#define TKN_MEMWAIT_LOG	1000
#endif

/*
 * protocol stack memory tables standard definision
 */
MALLOC_DEFINE(M_DEVBUF, "devbuf", "device driver memory");
MALLOC_DEFINE(M_FREE, "free", "should be on free list");
MALLOC_DEFINE(M_PCB, "pcb", "protocol control block");
MALLOC_DEFINE(M_SOFTINTR, "softintr", "Softinterrupt structures");
MALLOC_DEFINE(M_RTABLE, "routetbl", "routing tables");
MALLOC_DEFINE(M_FTABLE, "fragtbl", "fragment reassembly header");
MALLOC_DEFINE(M_IPMOPTS, "ip_moptions", "internet multicast options");
MALLOC_DEFINE(M_TEMP, "temp", "temporary memory");
MALLOC_DEFINE(M_IOV, "temp", "temporary memory");
MALLOC_DEFINE(M_KMEM, "kmem", "kernel memory");
#ifdef TKN_MEMSTATS
MALLOC_DEFINE(M_NULL, "null", "null entry"); /* null entry */
#endif

#ifdef TKN_MEMSTATS
/*
 * Memory statistics
 *
 * Count the number of calling malloc()/free().
 */
struct malloc_type *kmemstatistics;
#endif

/* BSD kernel style malloc() */
void *
tkn_malloc(u_long size, struct malloc_type *type, int flags)
{
	void *ptr;

#ifdef TKN_MEMSTATS
	/* add to kmemstatistics chain */
	if (type->ks_next==0) {
		/* invalid type request */
		if (type == M_NULL)
			log(LOG_ERR,
			    "[TKN %s] invalid request type M_NULL\n", __func__);
		/* init: add null entry to tail */
		if (kmemstatistics==NULL)
			malloc_type_attach(M_NULL);

		// log(LOG_DEBUG, "[TKN %s] malloc_type_attach type %p\n", __func__, type);
		malloc_type_attach(type);
	}
#endif

	type->ks_calls++;

	ptr = Kmalloc(size);
	if (ptr != NULL) {
		/* clean memory? */
		if (flags & M_ZERO)
			memset(ptr, 0, size);

		type->ks_inuse++;

		return ptr;
	}

	if ((flags & M_NOWAIT) == 0) {
		log(LOG_ERR,
			"[TKN %s]: tid %d malloc failed(waitok)\n",
			__func__, tk_get_tid());
	}

	return NULL;
}

/* BSD kernel style free() */
void
tkn_free(void *addr, struct malloc_type *type)
{
	if (addr) {
		type->ks_inuse--;
		Kfree(addr);
	}
}

ER
tkn_kmem_init(void)
{
	return 0;
}

ER
tkn_kmem_finish(void)
{
	return E_OK;
}

/* allocate 1 page for pool memory (page aligned memory) */
vaddr_t
tkn_alloc_poolpage1(boolean_t waitok)
{
	ER er;
	void *kaddr;

	er = tk_get_smb(&kaddr, 1, TA_RNG0);
	if (er != E_OK) {
		if (waitok) {
			log(LOG_ERR,
			    "[TKN %s]: tid %d tk_get_smb failed(waitok)\n",
			    __func__, tk_get_tid());
		}
		return NULL;
	}
	if ((vaddr_t)kaddr & (NBPG - 1))
		panic("[TKN]%s: Memory Alignment Error", __func__);

	return (vaddr_t)kaddr;
}

/* free 1 page for pool memory */
void
tkn_free_poolpage1(vaddr_t addr)
{
	if (addr & (NBPG - 1))
		panic("[TKN]%s: Memory Alignment Error", __func__);

	tk_rel_smb((VP)addr);
}

#ifdef TKN_MEMSTATS
/*
 * Add a malloc type for statistics
 */
void
malloc_type_attach(struct malloc_type *type)
{
	if (type->ks_magic != M_MAGIC)
		panic("malloc_type_attach: bad magic");

#ifdef DIAGNOSTIC
	{
		struct malloc_type *ksp;
		for (ksp = kmemstatistics; ksp != NULL; ksp = ksp->ks_next) {
			if (ksp == type)
				panic("malloc_type_attach: already on list");
		}
	}
#endif

#if 0 /* no limitation */
	type->ks_limit = 0;
#endif

	type->ks_next = kmemstatistics;
	kmemstatistics = type;
}
#endif /* TKN_MEMSTATS */
