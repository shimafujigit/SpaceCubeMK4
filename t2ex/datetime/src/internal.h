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
 *	internal.h
 *
 *       T2EX: calendar functions
 *       internal definitions
 */

#ifndef _T2EX_DATETIME_INTERNAL_
#define _T2EX_DATETIME_INTERNAL_

#include <basic.h>
#include <tk/tkernel.h>
#include <t2ex/datetime.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Number of days in 400 years */
#define days400 (365 * 303 + 366 * 97)

/* Number of days in 100 years (does not match 400 years boundary) */
#define days100 (365 * 76 + 366 * 24)

/* Number of days in 4 years (does not match 100 years boundary) */
#define days4 (365 * 3 + 366)

/* Number of days in a year (does not match 4 years boundary) */
#define days1 (365)

/* Leap year? */
#define isleap(year)	 (((year)%4 == 0 && (year)%100 != 0) || (year)%400 == 0)

/* Number of days from 1-1985 */
#define daysbefore1985 (724641)

/* Number of days between 1970-1985 */
#define daysbetween1970and1985 (5479)

/*
 * day of the week (day: the number of days from 1985-01-01) 
 *
 *	    January 1985    
 *	Su Mo Tu We Th Fr Sa
 *	       1  2  3  4  5
 *	 6  7  8  9 10 11 12
 *	13 14 15 16 17 18 19
 *	20 21 22 23 24 25 26
 *	27 28 29 30 31
 */
#define	weekday(day)	( ((day) + 2) % 7 )

/* Time conversions */
Inline time_t us_to_s( SYSTIM_U tim_u )
{
	return (tim_u / 1000000) - daysbetween1970and1985 * 24 * 60 * 60;
}

Inline SYSTIM us_to_ms( SYSTIM_U tim_u )
{
	SYSTIM tim_m;
	tim_u /= 1000;
	tim_m.hi = (tim_u >> 32);
	tim_m.lo = (UW)tim_u;
	return tim_m;
}

Inline SYSTIM_U s_to_us( time_t tim )
{
	return ((SYSTIM_U)tim) * 1000000 + ((SYSTIM_U)daysbetween1970and1985) * 24 * 60 * 60 * 1000000;
}

Inline SYSTIM_U ms_to_us( const SYSTIM* tim_m )
{
	return (((SYSTIM_U)tim_m->hi) << 32 | ((SYSTIM_U)tim_m->lo)) * 1000;
}

/* Calculate total number of days since 1985 */
Inline int _dt_dcount(int year) {
	int d = year - 1, days = 0;

	days += days400 * (d / 400);
	d %= 400;
	days += days100 * (d / 100);
	d %= 100;
	days += days4 * (d / 4);
	d %= 4;
	days += days1 * d;

	return days - daysbefore1985;
}

/* Number of days for each month (non-leap year) */
IMPORT const H _dt_mdays[12];

/* Name of the month */
IMPORT const char _dt_monthnm[9*12 + 1];

/* Name of the week */
IMPORT const char _dt_weeknm[9*7 + 1];

/* AM/PM */
IMPORT const char _dt_ampm[2*2 + 1];

/* Fetch a single strftime/strptime token  */
IMPORT int _dt_spec( const char **format, char *cp );

#ifdef __cplusplus
}
#endif
#endif /* _T2EX_DATETIME_INTERNAL_ */
