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
 *	cache_info.h (SH7750R)
 *	Cache Information
 */

#ifndef _CACHE_INFO_
#define _CACHE_INFO_

/*
 * Set non-cache area memory
 *	When using the control table for non-cache area memory 
 *	by memory manager routines, define the address of non-cache area.
 *	
 *	When not using, specify 0 for UseNoCacheMemoryTable.
 */
#define USE_NOCACHE_MEMTBL	(0)	/* Do not use */

#define NoCacheMemoryTop	(0)	/* Top address of non-cache area  */
#define NoCacheMemoryEnd	(0)	/* End address of non-cache area  */

/*
 * Conversion between page number and address
 *	When switching ON/Off of cache by an address, 
 *	define the conversion formula for the following Macro:
 */
#define CachingAddr(p)		( (void *)((UW)(p) & ~0x20000000U) )
#define NoCachingAddr(p)	( (void *)((UW)(p) | 0x20000000U) )

/*
 * Conversion between physical address and logical address of
 * real memory area (physical space)
 */
#define toLogicalAddress(paddr)		(void *)(paddr)
#define toPhysicalAddress(laddr)	(void *)(laddr)

/*
 * Convert logical address into logical address of the cache off area
 */
#define toNoCacheLogicalAddress(laddr)	\
	(void *)(((UW)(laddr) & 0x1fffffffU) | 0xa0000000U)

#endif /* _CACHE_INFO_ */
