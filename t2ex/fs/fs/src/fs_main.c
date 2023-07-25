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
 *	@(#)fs_main.c
 *
 */

#include "fsdefs.h"

/* Default "console" file system */
IMPORT	const	fs_fimp_t	fimp_consolefs_entry;
LOCAL	const	B		console_name[] = "consolefs";
LOCAL	const	B		console_con_name[] = "console";

/* Default "FAT" file system */
IMPORT	const	fs_fimp_t	fimp_fatfs_entry;

/*
 *  Initialize Console FS and stdio
 */
LOCAL	INT	stdio_initialize(void)
{
	INT	sts, stdinfd, stdoutfd, stderrfd;

	/* Regist "console" FIMP */
	sts = fs_regist(console_name, &fimp_consolefs_entry, NULL);
	if (sts != 0) goto exit0;

	/* Attach "console" FIMP */
	sts = fs_attach(NULL, console_con_name, console_name, 0, NULL);
	if (sts != 0) goto exit0;

	/* Open stdio */
	stdinfd  = fs_open("/console/stdin",  O_RDONLY);
	stdoutfd = fs_open("/console/stdout", O_WRONLY);
	stderrfd = fs_open("/console/stderr", O_WRONLY);

	if (stdinfd != STDIN_FILENO || stdoutfd != STDOUT_FILENO ||
						stderrfd != STDERR_FILENO) {
		(void)fs_close(stdinfd);
		(void)fs_close(stdoutfd);
		(void)fs_close(stderrfd);
		sts = EX_AGAIN;
	}
exit0:
	return sts;
}

/*
 *  Initialize Fat FS
 */
LOCAL	INT	fatfs_initialize(void)
{
	/* Regist "FAT" FIMP */
	return fs_regist(FIMP_FAT, &fimp_fatfs_entry, 0);
}

/*
 *  File system function : main entry
 */
EXPORT	INT	fs_main(INT ac, UB *arg[])
{
	INT	sts;

	sts = 0;
	if (ac >= 0) {		/* Startup */
		if (fs_tk_is_initialized() == 0) {
			sts = fs_tk_initialize();
			if (sts != 0) goto exit0;

			sts = stdio_initialize();
			if (sts == 0) {
				sts = fatfs_initialize();
			}
			if (sts != 0) {
				(void)fs_tk_finalize();
			}
		}
	} else {		/* Finish */
		/* Assume all tasks are never executing fs_xxx() call !! */
		if (fs_tk_is_initialized() != 0) {
			fs_sync();
			sts = fs_tk_finalize();
		}
	}
exit0:
	return sts;
}

