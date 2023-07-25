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

/*	$OpenBSD: param.h,v 1.17 2011/09/20 22:02:13 miod Exp $	*/
/*	$NetBSD: param.h,v 1.9 2002/03/24 03:37:23 thorpej Exp $	*/

/*
 * Copyright (c) 1994,1995 Mark Brinicombe.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the RiscBSD team.
 * 4. The name "RiscBSD" nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RISCBSD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL RISCBSD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_SPARC_PARAM_H_
#define	_SPARC_PARAM_H_

#define MACHINE_ARCH	"arm"
#define _MACHINE_ARCH	arm

/*
 * Machine dependent constants for ARM6+ processors
 */
/* These are defined in the Port File before it includes
 * this file. */

#define	PAGE_SHIFT	12		/* LOG2(NBPG) */
#define	PGSHIFT		12		/* LOG2(NBPG) */
#define	PAGE_SIZE	(1 << PAGE_SHIFT)	/* bytes/page */
#define	NBPG		(1 << PAGE_SHIFT)	/* bytes/page */
#define	PAGE_MASK	(PAGE_SIZE - 1)
#define PGOFSET		(PAGE_SIZE - 1)
#define	NPTEPG		(PAGE_SIZE/(sizeof (pt_entry_t)))

#define UPAGES          2               /* pages of u-area */
#define USPACE          (UPAGES * PAGE_SIZE) /* total size of u-area */
#define	USPACE_ALIGN	(0)		/* u-area alignment 0-none */

#ifndef MSGBUFSIZE
#define MSGBUFSIZE	PAGE_SIZE	/* default message buffer size */
#endif

/*
 * Minimum and maximum sizes of the kernel malloc arena in PAGE_SIZE-sized
 * logical pages.
 */
#define	NKMEMPAGES_MIN_DEFAULT	((4 * 1024 * 1024) >> PAGE_SHIFT)
#define	NKMEMPAGES_MAX_DEFAULT	((64 * 1024 * 1024) >> PAGE_SHIFT)

/* Constants used to divide the USPACE area */

/*
 * The USPACE area contains :
 * 1. the user structure for the process
 * 2. the fp context for FP emulation
 * 3. the kernel (svc) stack
 * 4. the undefined instruction stack
 *
 * The layout of the area looks like this
 *
 * | user area | FP context | undefined stack | kernel stack |
 *
 * The size of the user area is known.
 * The size of the FP context is variable depending of the FP emulator
 * in use and whether there is hardware FP support. However we can put
 * an upper limit on it.
 * The undefined stack needs to be at least 512 bytes. This is a requirement
 * of the FP emulators
 * The kernel stack should be at least 4K in size.
 *
 * The stack top addresses are used to set the stack pointers. The stack bottom
 * addresses are the addresses monitored by the diagnostic code for stack
 * overflows.
 */

#define FPCONTEXTSIZE			(0x100)
#define USPACE_SVC_STACK_TOP		(USPACE)
#define USPACE_SVC_STACK_BOTTOM		(USPACE_SVC_STACK_TOP - 0x1000)
#define	USPACE_UNDEF_STACK_TOP		(USPACE_SVC_STACK_BOTTOM - 0x10)
#define USPACE_UNDEF_STACK_BOTTOM	(sizeof(struct user) + FPCONTEXTSIZE + 10)

#ifdef _KERNEL
#ifndef _LOCORE
void	delay (unsigned);
#define DELAY(x)	delay(x)
#endif
#endif

/*
 * Machine dependent constants for all ARM processors
 */

/*
 * For KERNEL code:
 *	MACHINE must be defined by the individual port.  This is so that
 *	uname returns the correct thing, etc.
 *
 *	MACHINE_ARCH may be defined by individual ports as a temporary
 *	measure while we're finishing the conversion to ELF.
 *
 * For non-KERNEL code:
 *	If ELF, MACHINE and MACHINE_ARCH are forced to "arm/armeb".
 */


#define	MID_MACHINE	MID_ARM6

#define	ALIGNBYTES		_ALIGNBYTES
#define	ALIGN(p)		_ALIGN(p)
#define	ALIGNED_POINTER(p,t)	_ALIGNED_POINTER(p,t)

/* ARM-specific macro to align a stack pointer (downwards). */
#define STACKALIGNBYTES		(8 - 1)
#define STACKALIGN(p)		((u_long)(p) &~ STACKALIGNBYTES)

#define	DEV_BSHIFT	9		/* log2(DEV_BSIZE) */
#define	DEV_BSIZE	(1 << DEV_BSHIFT)
#define	BLKDEV_IOSIZE	2048

#ifndef MAXPHYS
#define	MAXPHYS		(64 * 1024)		/* max I/O transfer size */
#endif

/* pages ("clicks") to disk blocks */
#define	ctod(x)	((x) << (PAGE_SHIFT - DEV_BSHIFT))
#define	dtoc(x)	((x) >> (PAGE_SHIFT - DEV_BSHIFT))

#define	btodb(bytes)	 		/* calculates (bytes / DEV_BSIZE) */ \
	((bytes) >> DEV_BSHIFT)
#define	dbtob(db)			/* calculates (db * DEV_BSIZE) */ \
	((db) << DEV_BSHIFT)

/*
 * Constants related to network buffer management.
 */
#define	NMBCLUSTERS	4096		/* map size, max cluster allocation */

#define ovbcopy bcopy

#if defined(_KERNEL) && !defined(_LOCORE)
#include <sys/param.h>
#include <machine/cpu.h>
#endif

#endif	/* _SPARC_PARAM_H_ */
