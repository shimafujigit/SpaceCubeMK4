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
 *	@(#)fimp_fat.h
 *
 */

#ifndef __FIMP_FAT_H__
#define __FIMP_FAT_H__

#include <t2ex/fs.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 *  Configuration value : SYSCONF
 */
#define SYSCONF_MaxOpenF		(UB*)"FsMaxFile"
#define SYSCONF_TaskPriority		(UB*)"FiFAT_TskPri"
#define SYSCONF_TaskStackSize		(UB*)"FiFAT_StkSz"
#define SYSCONF_CacheFATMemorySize	(UB*)"FiFAT_FCacheSz"
#define SYSCONF_CacheFATRatio		(UB*)"FiFAT_FCacheNs"
#define SYSCONF_CacheRootMemorySize	(UB*)"FiFAT_RCacheSz"
#define SYSCONF_CacheRootRatio		(UB*)"FiFAT_RCacheNs"
#define SYSCONF_CacheDataMemorySize	(UB*)"FiFAT_DCacheSz"
#define SYSCONF_CacheDataRatio		(UB*)"FiFAT_DCacheNs"
#define SYSCONF_LastAccess		(UB*)"FsAccessTime"

#define DEFAULT_MaxOpenF		(64)
#define DEFAULT_TaskPriority		(100)
#define DEFAULT_TaskStackSize		(2 * 1024)
#define DEFAULT_CacheFATMemorySize	(320 * 1024)
#define DEFAULT_CacheFATRatio		(4)
#define DEFAULT_CacheRootMemorySize	(16 * 1024)
#define DEFAULT_CacheRootRatio		(4)
#define DEFAULT_CacheDataMemorySize	(1024 * 1024)
#define DEFAULT_CacheDataRatio		(64)
#define DEFAULT_LastAccess		(1)

/*
 *  FATFS FIMP entry
 */
IMPORT	const	fs_fimp_t	fimp_fatfs_entry;

#ifdef	__cplusplus
}
#endif

#endif	/* __FIMP_FAT_H__ */

