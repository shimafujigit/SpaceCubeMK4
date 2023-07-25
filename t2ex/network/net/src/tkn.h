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
 *	@(#)tkn.h
 *
 */

#ifndef _TKN_H_
#define _TKN_H_

#include <basic.h>
#include <sys/syscall.h>

#include "netmain/tkn_init.h"

/*
 * Stack size definitions
 */
#define TKN_DRVTSK_STKSZ	(1536)
#define TKN_NETDMN_STKSZ	(5120)
#define	TKN_SPLTRAMPO_STKSZ	(2560)

/*
 * For mutual exclusion.
 */
IMPORT void LockTKN( void );
IMPORT void UnlockTKN( void );
IMPORT void LockSPL( void );
IMPORT void UnlockSPL( void );

/*
 * Invoke a system call.
 */
IMPORT int tkn_syscall( int sys_no, void *v, int *retval );

/*
 * BPF (Berkeley Packet Filter)
 */
IMPORT int tkn_bpf_open( const char *path, int oflag, struct file * );

/*
 * Tunneling
 */
IMPORT int tkn_tun_open( const char *path, int oflag, struct file * );

IMPORT ER init_netdmn( void );
IMPORT ER finish_netdmn( void );

#endif /* _TKN_H_ */
