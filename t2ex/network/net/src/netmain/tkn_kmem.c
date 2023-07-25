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
 *	@(#)tkn_kmem.c
 *
 */

#include <tk/tkernel.h>
#include <libstr.h>

#include <sys/kmem.h>
#include <sys/malloc.h>

void *
kmem_alloc(size_t size, km_flag_t kmflags)
{

	return malloc(size, M_KMEM, (kmflags & KM_SLEEP) ? M_WAITOK : M_NOWAIT);
}

void *
kmem_zalloc(size_t size, km_flag_t kmflags)
{
	void *p;

	p = kmem_alloc(size, kmflags);
	if (p != NULL) {
		bzero(p, size);
	}
	return p;
}

void
kmem_free(void *p, size_t size)
{
	(void)size;

	free(p, M_KMEM);
}

#ifndef T2EX
void
kmem_init(void)
{

}

size_t
kmem_roundup_size(size_t size)
{
	return size;
}
#endif
