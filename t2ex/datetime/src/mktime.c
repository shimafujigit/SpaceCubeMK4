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
 *	@(#)mktime.c
 *
 *       T2EX: calendar functions
 *       dt_mktime, dt_mktime_ms, dt_mktime_us
 */

#include <basic.h>
#include <errno.h>
#include <tk/tkernel.h>
#include <t2ex/datetime.h>
#include "internal.h"

LOCAL ER utc_mktime( struct tm* tm, SYSTIM_U* result )
{
	int i, days, yday;

	/* FIXME: check values */
	if (tm->tm_year < 85) {
		return EX_OVERFLOW;
	}

	/* Calculate total number of days since 1985 */
	days = _dt_dcount(tm->tm_year + 1900);

	for (i = 0, yday = 0; i < tm->tm_mon; i++) {
		if (i == 1 && isleap(tm->tm_year + 1900)) {
			yday += 29;
		}
		else {
			yday += _dt_mdays[i];
		}
	}

	yday += (tm->tm_mday - 1);
	days += yday;

	/* Return the result */
	*result = ((SYSTIM_U)days) * 24 * 60 * 60 * 1000000
		+ ((SYSTIM_U)tm->tm_hour) * 60 * 60 * 1000000
		+ ((SYSTIM_U)tm->tm_min) * 60 * 1000000
		+ ((SYSTIM_U)tm->tm_sec) * 1000000
		+ ((SYSTIM_U)tm->tm_usec);
	tm->tm_wday = weekday(days);
	tm->tm_yday = yday;
	return E_OK;
}

EXPORT ER dt_mktime_us( struct tm* tm, const struct tzinfo* tz, SYSTIM_U* result )
{
	struct tzinfo systz;
	SYSTIM_U val;
	ER er;

	if (tz == NULL) {
		/* If tz is not specified, use system timezone */
		er = dt_getsystz(&systz);
		if (er < E_OK) {
			return er;
		}
		tz = &systz;
	}

	/* struct tm (as UTC) -> SYSTIM_U */
	er = utc_mktime(tm, &val);
	if (er < E_OK) {
		return er;
	}

	/* Adjust offset */
	val += (tm->tm_isdst ? (SYSTIM_U)tz->dst_offset : (SYSTIM_U)tz->offset) * 1000000;
	if (val < 0) {
		return EX_OVERFLOW;
	}

	if (result) {
		*result = val;
	}
	return E_OK;
}

EXPORT ER dt_mktime( struct tm* tm, const struct tzinfo* tz, time_t* result )
{
	SYSTIM_U tim_u;
	ER er = dt_mktime_us(tm, tz, &tim_u);
	if (er < E_OK) {
		return er;
	}
	if (result) {
		*result = us_to_s(tim_u);
	}
	return E_OK;
}

EXPORT ER dt_mktime_ms( struct tm* tm, const struct tzinfo* tz, SYSTIM* result )
{
	SYSTIM_U tim_u;
	ER er = dt_mktime_us(tm, tz, &tim_u);
	if (er < E_OK) {
		return er;
	}
	if (result) {
		*result = us_to_ms(tim_u);
	}
	return E_OK;
}
