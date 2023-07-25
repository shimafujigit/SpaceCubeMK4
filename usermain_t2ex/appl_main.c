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
 *	@(#)appl_main.c
 *
 */
#include <basic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#ifdef	USE_T2EX_FS
#define	P			printf
#define	Gets(buf, bufsz)	fgets(buf, bufsz, stdin)
#else
#define	P			tm_printf
#define	Gets(buf, bufsz)	tm_getline(buf)
#endif

/* Command functions */
IMPORT	INT	exec_cmd(B *cmd);
IMPORT	void	init_calendar_date(void);

/*
 *	Application main entry
 */
EXPORT	void	appl_main( void )
{
	B	buf[256];
	INT	fin;

	/* initialize calendar date */
	init_calendar_date();

	/* command processing */
	for (fin = 0; fin == 0; ) {
		P("T2EX >> ");
		Gets(buf, sizeof(buf));
		if (strncmp(buf, "quit", 1) == 0) {
			fin = 1;

		/* t-monitor */
		} else if (strncmp(buf, "#", 1) == 0) {
			tm_command(&buf[1]);
		/* misc. command */
		} else {
			if (exec_cmd(buf) == 0) {
				P("q[uit]      quit\n");
				P("# [cmd]     exec t-monitor command\n");
				P("?           command help\n");
				P("<command>   misc. command\n");
			}
		}
	}
}

