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
 *	@(#)fs_desc.c
 *
 */

#include "fsdefs.h"

/*
 * Generic ientifier consists of the following structure:
 *
 *  bits  0- 3	type (0-15), 0 means the descriptor is unused
 *  bits  4-17	index in descpool (0 - descmax - 1)
 *  bits 18-30	identifer version (0 - 8191)
 *  bits 31	always 0
 */
#define TYPE_MASK	0x000f
#define INDX_MASK	0x3fff
#define INDX_SHIFT	4
#define VERS_INC	(1 << 18)
#define IDENT_MASK	0x7fffffff

/*
 *  Initialize descriptor lists
 */
EXPORT	INT	fs_descmgr_init(fs_descmgr_t * const mgr,
				UINT descmax, UINT descsize, UINT type)
{
	fs_desc_t	*desc;
	INT		ix;

	/* Align descriptor size to sizeof(W) */
	descsize = (descsize + sizeof(W) - 1) / sizeof(W) * sizeof(W);

	/* Allocate maximum number of descriptors */
	desc = (fs_desc_t *)fs_malloc(descsize * descmax);
	if (desc == NULL) {
		return EX_NOMEM;
	}
	mgr->m_descmax = descmax;
	mgr->m_descsize = descsize;
	mgr->m_type = type;
	mgr->m_usecnt = 0;
	mgr->m_descpool = desc;

	/* Make free-list */
	mgr->m_freelist = desc;		/* top */
	for (ix = 0;;) {
		desc->d_ident = ix << INDX_SHIFT;
		if (++ix >= descmax) break;
		desc->d_list.next = (QUEUE*)((UB*)desc + descsize);
		desc = (fs_desc_t*)desc->d_list.next;
	}
	desc->d_list.next = NULL;	/* List terminator */

	return 0;
}

/*
 *  Finalize descriptor lists
 */
EXPORT	INT	fs_descmgr_fini(fs_descmgr_t * const mgr)
{
	if (mgr->m_usecnt != 0) return EX_BUSY;

	fs_free(mgr->m_descpool);
	memset(mgr, 0, sizeof(fs_descmgr_t));
	return 0;
}

/*
 *  Allocate descriptor
 */
EXPORT	fs_desc_t	*fs_descmgr_alloc(fs_descmgr_t * const mgr)
{
	fs_desc_t	*desc;
	UINT		ident;

	fs_lock_lock(LOCKNUM_DESC);

	/* get a free descriptor from the free list */
	desc = mgr->m_freelist;
	if (desc != NULL) {
		mgr->m_freelist = (fs_desc_t*)desc->d_list.next;
		mgr->m_usecnt++;
	}
	fs_lock_unlock(LOCKNUM_DESC);

	if (desc != NULL) {
		/* Clear descriptor preserving its identifier */
		ident = desc->d_ident;
		memset(desc, 0, mgr->m_descsize);
		desc->d_ident = ident;
	}
	return desc;
}

/*
 *  Free descriptor
 */
LOCAL	void	fs_free_desc(fs_descmgr_t * const mgr, fs_desc_t *desc)
{
	/* clear type & increment ident version number */
	desc->d_ident = ((desc->d_ident & ~TYPE_MASK) + VERS_INC) & IDENT_MASK;

	/* put into free-list */
	desc->d_list.next = (QUEUE*)mgr->m_freelist;
	mgr->m_freelist = desc;
	if (mgr->m_usecnt > 0) mgr->m_usecnt--;
}

/*
 *  Get descriptor from id and type
 */
EXPORT	fs_desc_t	*fs_descmgr_get(fs_descmgr_t * const mgr, UINT ident)
{
	fs_desc_t	*desc;
	INT		ix;

	desc = NULL;
	if ((ident & TYPE_MASK) != mgr->m_type) goto exit0;
	ix = (ident >> INDX_SHIFT) & INDX_MASK;
	if (ix >= mgr->m_descmax) goto exit0;

	/* Compute desc descriptor address and check identifier */
	fs_lock_lock(LOCKNUM_DESC);
	desc = (fs_desc_t*)((UB*)mgr->m_descpool + ix * mgr->m_descsize);
	if (desc->d_ident == ident) {
		desc->d_refc++;		/* OK */
	} else {
		desc = NULL;		/* invalid */
	}
	fs_lock_unlock(LOCKNUM_DESC);
exit0:
	return desc;
}

/*
 *  Free FS descriptor of a given size index
 */
EXPORT	void	fs_descmgr_free(fs_descmgr_t * const mgr, fs_desc_t *desc)
{
	fs_lock_lock(LOCKNUM_DESC);
	fs_free_desc(mgr, desc);
	fs_lock_unlock(LOCKNUM_DESC);
}

/*
 *  Release FS descriptor of a given size index
 */
EXPORT	void	fs_descmgr_release(fs_descmgr_t * const mgr, fs_desc_t *desc)
{
	fs_lock_lock(LOCKNUM_DESC);

	if (desc->d_refc > 0) desc->d_refc--;
	if (desc->d_refc == 0 && (desc->d_ident & TYPE_MASK) == 0) {
		fs_free_desc(mgr, desc);
	}
	fs_lock_unlock(LOCKNUM_DESC);
}

/*
 *  Validate the descriptor
 */
EXPORT	UINT	fs_descmgr_validate(fs_descmgr_t * const mgr, fs_desc_t *desc)
{
	desc->d_ident |= mgr->m_type;
	return desc->d_ident;
}

/*
 *  Invalidate the descriptor
 */
EXPORT	void fs_descmgr_invalidate(fs_descmgr_t * const mgr, fs_desc_t *desc)
{
	desc->d_ident &= ~TYPE_MASK;
}

/*
 *  Get descriptor index ftom descriptor identifier
 */
EXPORT	INT fs_descmgr_ident2index(UINT ident)
{
	return (ident >> INDX_SHIFT) & INDX_MASK;
}

