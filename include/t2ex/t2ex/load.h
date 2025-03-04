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
 *	load.h (T2EX)
 *	T2EX: program load functions
 */

#ifndef _T2EX_LOAD_
#define _T2EX_LOAD_

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Regular program module entry point */
typedef int pm_entry_t( BOOL startup, void* arg );

/* Program module designation */
struct pm {
	ATR	pmtype;	/* program module type */
	void*	pmhdr;	/* load target */
};

/* Program module types */
#define PM_FILE	0x01	/* program module on file system */
#define PM_PTR	0x02	/* program module on memory */

/* Program module status */
struct pm_stat {
	BOOL	sysprg;
	UINT	attr;
	void*	entry;
	void*	start;
	void*	end;
};

IMPORT ER pm_main( INT ac, UB* av[] );

/*
 * Definition for interface library automatic generation (mkiflib)
 */
/*** DEFINE_IFLIB
[INCLUDE FILE]
<t2ex/load.h>

[PREFIX]
PM
***/

/* [BEGIN SYSCALLS] */
/* Program load function */
/* ALIGN_NO 1 */
IMPORT ID pm_load( const struct pm* prog, UINT attr, pm_entry_t** entry );
IMPORT ID pm_loadspg( const struct pm* prog, INT ac, UB* av[] );
IMPORT ER pm_status( ID progid, struct pm_stat* status );
IMPORT ER pm_unload( ID progid );
/* [END SYSCALLS] */

#ifdef __cplusplus
}
#endif
#endif /* _T2EX_LOAD_ */
