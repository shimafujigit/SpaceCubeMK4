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
 *    Changed by UC Technology at 2013/01/28.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	cache.c (SH7750R)
 *	Cache Operation
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>

#if	USE_MEM_PROTECT
IMPORT	void	setValidITLB( UW laddr );
#endif

#define PAGESIZE	0x1000U		/* Page size */
#define CACHE_LINESZ	0x20U		/* Chache line size */

#define ICACHE_ADR_TOP	0xf0000000	/* Instruction cache address array */
#define ICACHE_DAT_TOP	0xf1000000	/* Instruction cache data array */
#define ICACHE_WAY_MSK	0x00002000
#define ICACHE_ENT_MSK	0x00001fe0

#define DCACHE_ADR_TOP	0xf4000000	/* Data cache address array */
#define DCACHE_DAT_TOP	0xf5000000	/* Data cache data array */
#define DCACHE_WAY_MSK	0x00004000
#define DCACHE_ENT_MSK	0x00003fe0

#define CACHE_V		0x00000001	/* Valid */
#define CACHE_U		0x00000002	/* Dirty */
#define CACHE_TAG	0xfffffc00	/* Tag */

#define CACHE_A		0x00000008	/* Associative specification */

/*
 * Obtain cache line size
 */
EXPORT INT GetCacheLineSize( void )
{
	return CACHE_LINESZ;
}

/*
 *  One page cache flush
 *	Flush one page cache (4KB) corresponding to logical address 'laddr'.
 *	'laddr' must be top address of page.
 *	
 *	Cache control program must be located at non-cached area,
 *	so P2 area (as shadow area) is used to execute the program.
 *	For this purpose, call the program with address pointed by
 *	'_FlushCache_', and don't call '_flush_cache_' directly.
 */
#pragma inline_asm(data_cache)
static void data_cache( UW laddr )
{
	ocbp	@r4
}

LOCAL void _flush_cache_( UW laddr, UW mode )
{
	UW	icarray, ent;

	ent = laddr;
	if ( (in_w(CCR) & CCR_IIX) != 0 ) {
		ent >>= 25 - 12;
	}
	ent &= ICACHE_ENT_MSK & ~(PAGESIZE-1);

	if ( (mode & TCM_ICACHE) != 0 ) {
		icarray = ICACHE_ADR_TOP | CACHE_A | ent;
#if	USE_MEM_PROTECT
		/* Set Valid ITLB to success Intstruction cache invalidation */
		setValidITLB(laddr);
#endif
		for ( ent = 0; ent < PAGESIZE; ent += CACHE_LINESZ ) {
			/* Instruction cache */
			*(UW*)(icarray + ent) = (laddr + ent) & CACHE_TAG;
		}
	}
	if ( (mode & TCM_DCACHE) != 0 ) {
		for ( ent = 0; ent < PAGESIZE; ent += CACHE_LINESZ ) {
			/* Data cache */
			data_cache(laddr + ent);
		}
	}
}

LOCAL FP _FlushCacheP2_ = _flush_cache_;

/*
 * Cache flush
 *	Flush cache between 'laddr' and 'len' bytes areas.
 *	Writeback and disable cache
 */
EXPORT void FlushCacheM( void *laddr, INT len, UINT mode )
{
	UW	top, end;
	UINT	imask;
	FP FlushCacheP2;

	top = (UW)laddr & ~(PAGESIZE-1);
	end = (UW)laddr + len;

	while ( top < end ) {

		DI(imask);
		FlushCacheP2 = (FP)(((UW)_FlushCacheP2_) + 0x20000000);
		(*FlushCacheP2)(top, mode);
		EI(imask);

		top += PAGESIZE;
	}
}

EXPORT void FlushCache( void *laddr, INT len )
{
	FlushCacheM(laddr, len, TCM_ICACHE|TCM_DCACHE);
}

/*
 * Control cache
 *	mode := [CC_FLUSH] | [CC_INVALIDATE]
 */
EXPORT ER ControlCacheM( void *laddr, INT len, UINT mode )
{
	return E_NOSPT;
}

