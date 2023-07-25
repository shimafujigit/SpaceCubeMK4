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
 *	@(#)gmtime.c
 *
 *       T2EX: calendar functions
 *       dt_gmtime, dt_gmtime_ms, dt_gmtime_us
 */

#include <basic.h>
#include <errno.h>
#include <tk/tkernel.h>
#include <t2ex/datetime.h>
#include "internal.h"

EXPORT ER dt_gmtime_us( SYSTIM_U tim_u, struct tm* result )
{
	SYSTIM_U rem = tim_u;
	int d, i, md;

	if (tim_u < 0) {
		return EX_OVERFLOW;
	}

	result->tm_isdst = 0;

	result->tm_usec = (rem % 1000000);
	rem /= 1000000;
	result->tm_sec = (rem % 60);
	rem /= 60;
	result->tm_min = (rem % 60);
	rem /= 60;
	result->tm_hour = (rem % 24);
	rem /= 24;

	d = rem;
	result->tm_wday = weekday(d);

	d += daysbefore1985;
	result->tm_year = -1900 + 1;

	result->tm_year += 400 * (d / days400);
	d %= days400;
	result->tm_year += 100 * (d / days100);
	d %= days100;
	result->tm_year += 4 * (d / days4);
	d %= days4;
	result->tm_year += (d / days1);
	d %= days1;

	result->tm_yday = d;
	for (i = 0; i < 12; i++) {
		md = (i == 1 && isleap(result->tm_year)) ? 29 : _dt_mdays[i];
		if (d < md) {
			break;
		}
		d -= md;
	}

	result->tm_mon = i;
	result->tm_mday = 1 + d;
	return E_OK;
}

EXPORT ER dt_gmtime( time_t tim, struct tm* result )
{
	return dt_gmtime_us(s_to_us(tim), result);
}

EXPORT ER dt_gmtime_ms( const SYSTIM* tim_m, struct tm* result )
{
	return dt_gmtime_us(ms_to_us(tim_m), result);
}
