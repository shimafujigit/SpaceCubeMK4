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
 *	@(#)ssid.h (sys)
 *
 *	Subsystem ID and subsystem priority
 *
 *	(NB) Included also from the assembler source,
 *	     so only macros can be described.
 */

#ifndef __SYS_SSID_H__
#define __SYS_SSID_H__

/*
 * T-Kernel (1 - 9)
 *	1 - 6 and 9 were previously used for other purposes.
 */
#define CONSIO_SVC	1		/* Console I/O */
#define CONSIO_PRI		1
#define DEVICE_SVC	8		/* T-Kernel/SM device management */
#define DEVICE_PRI		4
#define SYSTEM_SVC	9		/* T-Kernel/SM system management */
#define SYSTEM_PRI		1

/*
 * Subsystem (10 - )
 */
#define MM_SVC		11		/* Memory management		*/
#define MM_PRI			2
#define	FS_SVC		12		/* File system management	*/
#define	FS_PRI			2
#define PM_SVC		14		/* Program module management	*/
#define PM_PRI			6
#define DT_SVC		15		/* Calendar management		*/
#define DT_PRI			8
#define SM_SVC		16		/* System management		*/
#define SM_PRI			2
#define SEG_SVC		17		/* Segment management		*/
#define SEG_PRI			2
#define	NET_SVC		18		/* Network management		*/
#define	NET_PRI			8

#endif /* __SYS_SSID_H__ */

