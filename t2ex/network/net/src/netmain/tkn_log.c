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
 *	@(#)tkn_log.c
 *
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>


#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>

#include <stdarg.h>
#include <sys/syslog.h>

LOCAL void log_output(const char* format, va_list ap)
{
	static char buf[128];

	vsnprintf(buf, sizeof(buf), format, ap);
	buf[sizeof(buf) - 1] = '\0';

	tm_putstring((UB*)buf);
}

#ifdef T2EX_LOG
/* log level */
#ifndef TKN_LOG_LEVEL
#define TKN_LOG_LEVEL LOG_WARNING
#endif

int tkn_log_level = TKN_LOG_LEVEL;

/*
 * message logging - instead of kernel log func. 'log()'
 */
void
log(int level, const char* format, ...)
{
	va_list list;

	if (level > tkn_log_level) {
		return;
	}

	va_start(list, format);
	log_output(format, list);
	va_end(list);
}
#endif

void tkn_panic(const char* format, ...)
{
	va_list list;

	tm_putstring((UB*)"panic: ");

	va_start(list, format);
	log_output(format, list);
	va_end(list);

	tm_putstring((UB*)"\n" );

	/* display register */
	tm_command((UB*)"R C");

	for (;;) tm_monitor();			/* stop the system */
}
