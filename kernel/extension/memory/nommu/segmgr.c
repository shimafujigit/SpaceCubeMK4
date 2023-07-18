/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *    Modified by T-Engine Forum at 2013/01/12.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the UCT T-Kernel 2.0
 *    Changed by UC Technology at 2013/01/29.
 *
 *    UCT T-Kernel 2.0 DevKit Version 2.00.01
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	segmgr.c (T-Kernel/SM)
 *	No MMU version: segment management
 */

#include "sysmgr.h"
#include "cache_info.h"
#include <sys/segment.h>
#include <sys/svc/ifsegment.h>

#if defined(USE_FUNC_CNVPHYSICALADDR)
/*
 * Conversion to physical address
 *	Cache control required, but cache control by page cannot be
 *	provided without MMU, so not implemented.
 */
LOCAL INT _CnvPhysicalAddr( CONST void *laddr, INT len, void **paddr )
{
	*paddr = toPhysicalAddress(laddr);

	/* Cache off not possible, so cache flash only is performed. */
	FlushCache(laddr, len);

	return len;
}
#endif /* USE_FUNC_CNVPHYSICALADDR */

#if defined(USE_FUNC_GETSPACEINFO)
/*
 * Get address space information
 */
LOCAL ER _GetSpaceInfo( CONST void *addr, INT len, T_SPINFO *pk_spinfo )
{
	T_RSMB	rsmb;
	ER	ercd = E_OK;
	INT	cont, sz;
	CONST void	*page, *va, *pa;

	if ( len <= 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	ercd = ChkSpaceR(addr, len);
	if ( ercd < E_OK ){
		goto err_ret;
	}

	ercd = RefSysMemInfo(&rsmb);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	pk_spinfo->paddr   = toPhysicalAddress(addr);
	pk_spinfo->page	   = (void*)((UW)(pk_spinfo->paddr) & ~(rsmb.blksz-1));
	pk_spinfo->pagesz  = rsmb.blksz;
	pk_spinfo->cachesz = GetCacheLineSize();

	cont = 0;
	page = pk_spinfo->page;
	va = addr;
	while ( cont < len ) {
		pa = toPhysicalAddress(va);
		if ( (pa < page) || (((UW)page + rsmb.blksz) <= (UW)pa) ) {
			break;
		}
		sz = rsmb.blksz - ((UW)pa - (UW)page);
		cont += sz;
		page = (void*)((UW)page + rsmb.blksz);
		va = (void*)((UW)va + sz);
	}
	pk_spinfo->cont = ( cont <= len )? cont: len;

	return ercd;

err_ret:
	DEBUG_PRINT(("_GetSpaceInfo ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_GETSPACEINFO) */

/* ------------------------------------------------------------------------ */

IMPORT	INT	svc_call_limit;		/* SVC protected level (T-Kernel/OS) */

#define TA_RNGS		( svc_call_limit << 8 )

#if defined(USE_FUNC_MAPMEMORY)
/*
 * Memory map
 *	Cache control required, but cache control by page cannot be
 *	provided without MMU (except for certain machines), so not implemented.
 */
LOCAL ER _MapMemory( CONST void *paddr, INT len, UINT attr, void **laddr )
{
	ER	ercd;

	if ( len <= 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}

	if ( paddr == NULL ) {
#if	USE_MEM_PROTECT
		UINT a = ( (attr & MM_USER) != 0 )? TA_RNG3: TA_RNGS;
#else
		UINT a = ( (attr & MM_USER) != 0 )? TA_RNG2: TA_RNG1;
#endif
		if ( (attr & MM_CDIS) != 0 ) {
			a |= TA_NOCACHE;
		}

		/* Allocate memory automatically */
		*laddr = GetSysMemBlk((INT)smPageCount((UW)len), a);
		if ( *laddr == NULL ) {
			ercd = E_NOMEM;
			goto err_ret;
		}

	} else {
		/* Logical address conversion */
		*laddr = toLogicalAddress(paddr);

		/* Flush cache */
		FlushCache(*laddr, len);

		if ( (attr & MM_CDIS) != 0 ) {
			/* Allocate logical addresses for cache off area */
			*laddr = toNoCacheLogicalAddress(*laddr);
		}
	}

	return E_OK;

err_ret:
	DEBUG_PRINT(("_MapMemory ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_MAPMEMORY */


#if defined(USE_FUNC_UNMAPMEMORY)
/*
 * Memory unmap
 */
LOCAL ER _UnmapMemory( CONST void *laddr )
{
	ER	ercd;

	/* Memory release when memory is allocated automatically by MapMemory()
	 *	If the memory is not automatically allocated, RelSysMemBlk()
	 *	returns E_PAR.
	 */
	ercd = RelSysMemBlk(laddr);
	if ( ercd < E_OK && ercd != E_PAR ) {
		goto err_ret;
	}

	return E_OK;

err_ret:
	DEBUG_PRINT(("_UnmapMemory ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_UNMAPMEMORY */

/* ------------------------------------------------------------------------ */

/*
 * Address space check
 */
LOCAL ER  _ChkSpace( CONST void *laddr, INT len, UINT mode, UINT env )
{
	return ( laddr == NULL )? E_MACV: E_OK;
}

LOCAL INT _ChkSpaceTstr( CONST TC *str, INT max, UINT mode, UINT env )
{
	CONST TC	*p = str;

	if ( str == NULL ) {
		return E_MACV;
	}

	while ( *p != TNULL ) {
		p++;
		if ( --max == 0 ) {
			break;
		}
	}
	return p - str;
}

LOCAL INT _ChkSpaceBstr( CONST UB *str, INT max, UINT mode, UINT env )
{
	CONST UB	*p = str;

	if ( str == NULL ) {
		return E_MACV;
	}

	while ( *p != '\0' ) {
		p++;
		if ( --max == 0 ) {
			break;
		}
	}
	return p - str;
}

/* ------------------------------------------------------------------------ */

#if defined(USE_FUNC_FLUSHMEMCACHE)
/*
 * Memory Cache Control
 */
LOCAL ER _FlushMemCache( void *laddr, INT len, UINT mode )
{
	ER	ercd;

	if ( (mode & ~(TCM_ICACHE|TCM_DCACHE)) != 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	ercd = ChkSpaceR(laddr, len);
	if ( ercd < E_OK ){
		goto err_ret;
	}

	FlushCacheM(laddr, len, mode);

	return E_OK;

err_ret:
	DEBUG_PRINT(("_FlushMemCache ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_FLUSHMEMCACHE */

#if defined(USE_FUNC_CONTROLCACHE)
/*
 * Memory Cache Control
 */
LOCAL INT _ControlCache( void *addr, INT len, UINT mode )
{
	ER	ercd;

	if ( len <= 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	if ( mode == 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	ercd = ChkSpaceR(addr, len);
	if ( ercd < E_OK ){
		goto err_ret;
	}

	ercd = ControlCacheM(addr, len, mode);
	if( ercd != E_OK ){
		goto err_ret;
	}

	return len;

err_ret:
	DEBUG_PRINT(("_ControlCache ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_CONTROLCACHE */

/* ------------------------------------------------------------------------ */

/*
 * Extended SVC entry
 */
LOCAL INT SegSVCentry( void *para, W fn )
{
	INT	ercd;

	switch ( fn ) {
		/* Can be called from any protection levels */
#if defined(USE_FUNC_FLUSHMEMCACHE)
	  case SEG_FLUSHMEMCACHE_FN:
		break;
#endif /* USE_FUNC_FLUSHMEMCACHE */

	  default:
		/* Caller protection level check */
		ercd = ChkCallPLevel();
		if ( ercd < E_OK ) {
			goto err_ret;
		}
	}

	switch ( fn ) {
#if defined(USE_FUNC_LOCKSPACE)||defined(USE_FUNC_UNLOCKSPACE)
	  case SEG_LOCKSPACE_FN:
	  case SEG_UNLOCKSPACE_FN:
		ercd = E_OK;
		break;
#endif /* USE_FUNC_LOCKSPACE or USE_FUNC_UNLOCKSPACE */

#if defined(USE_FUNC_CNVPHYSICALADDR)
	  case SEG_CNVPHYSICALADDR_FN:
		{ SEG_CNVPHYSICALADDR_PARA *p = para;
		ercd = _CnvPhysicalAddr(p->laddr, p->len, p->paddr); }
		break;
#endif /* USE_FUNC_CNVPHYSICALADDR */

	  case SEG_CHKSPACE_FN:
		{ SEG_CHKSPACE_PARA *p = para;
		ercd = _ChkSpace(p->laddr, p->len, p->mode, p->env); }
		break;
	  case SEG_CHKSPACETSTR_FN:
		{ SEG_CHKSPACETSTR_PARA *p = para;
		ercd = _ChkSpaceTstr(p->str, p->max, p->mode, p->env); }
		break;
	  case SEG_CHKSPACEBSTR_FN:
		{ SEG_CHKSPACEBSTR_PARA *p = para;
		ercd = _ChkSpaceBstr(p->str, p->max, p->mode, p->env); }
		break;

#if defined(USE_FUNC_MAPMEMORY)
	  case SEG_MAPMEMORY_FN:
		{ SEG_MAPMEMORY_PARA *p = para;
		ercd = _MapMemory(p->paddr, p->len, p->attr, p->laddr); }
		break;
#endif /* USE_FUNC_MAPMEMORY */

#if defined(USE_FUNC_UNMAPMEMORY)
	  case SEG_UNMAPMEMORY_FN:
		{ SEG_UNMAPMEMORY_PARA *p = para;
		ercd = _UnmapMemory(p->laddr); }
		break;
#endif /* USE_FUNC_UNMAPMEMORY */

#if defined(USE_FUNC_FLUSHMEMCACHE)
	  case SEG_FLUSHMEMCACHE_FN:
		{ SEG_FLUSHMEMCACHE_PARA *p = para;
		ercd = _FlushMemCache(p->laddr, p->len, p->mode); }
		break;
#endif /* USE_FUNC_FLUSHMEMCACHE */

	  case SEG_CHKSPACELEN_FN:
	  case SEG_READMEMSPACE_FN:
	  case SEG_WRITEMEMSPACE_FN:
	  case SEG_SETMEMSPACEB_FN:
	  case SEG_MAKESPACE_FN:
	  case SEG_UNMAKESPACE_FN:
	  case SEG_CHANGESPACE_FN:
		ercd = E_NOSPT;
		break;

	  /* T-Kernel 2.0 */
#if defined(USE_FUNC_CONTROLCACHE)
	  case SEG_CONTROLCACHE_FN:
		{ SEG_CONTROLCACHE_PARA *p = para;
		ercd = _ControlCache(p->addr, p->len, p->mode); }
		break;
#endif /* USE_FUNC_CONTROLCACHE */

#if defined(USE_FUNC_GETSPACEINFO)
	  case SEG_GETSPACEINFO_FN:
		{ SEG_GETSPACEINFO_PARA *p = para;
		ercd = _GetSpaceInfo(p->addr, p->len, p->pk_spinfo); }
		break;
#endif /* USE_FUNC_GETSPACEINFO */

#if defined(USE_FUNC_SETCACHEMODE)
	  case SEG_SETCACHEMODE_FN:
		ercd = E_NOSPT;
		break;
#endif /* USE_FUNC_SETCACHEMODE */

#if defined(USE_FUNC_SETMEMORYACCESS)
	  case SEG_SETMEMORYACCESS_FN:
		ercd = E_NOSPT;
		break;
#endif /* USE_FUNC_SETMEMORYACCESS */

	  default:
		ercd = E_RSFN;
	}

err_ret:
	return ercd;
}

/*
 * Initialize segment management
 */
EXPORT ER init_segmgr( void )
{
#if	USE_MEM_PROTECT
IMPORT	ER	initUserSpace( void );

	ER	ercd;

	/* Initialize logical space */
	ercd = initUserSpace();
	if ( ercd < E_OK ) return ercd;
#endif
	return E_OK;
}

/*
 * Start segment management
 */
EXPORT ER start_segmgr( void )
{
	T_DSSY	dssy;
	ER	ercd;

	/* Register manager */
	dssy.ssyatr    = TA_NULL;
	dssy.ssypri    = SEG_PRI;
	dssy.svchdr    = (FP)SegSVCentry;
	dssy.breakfn   = NULL;
	dssy.startupfn = NULL;
	dssy.cleanupfn = NULL;
	dssy.eventfn   = NULL;
	dssy.resblksz  = 0;
	ercd = tk_def_ssy(SEG_SVC, &dssy);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	return E_OK;

err_ret:
	BMS_DEBUG_PRINT(("start_segmgr ercd = %d\n", ercd));
	return ercd;
}

/*
 * End segment manager
 */
EXPORT ER finish_segmgr( void )
{
	ER	ercd;

	/* Delete subsystem registration */
	ercd = tk_def_ssy(SEG_SVC, NULL);

#ifdef DEBUG
	if ( ercd < E_OK ) {
		DEBUG_PRINT(("finish_segmgr ercd = %d\n", ercd));
	}
#endif

	return ercd;
}
