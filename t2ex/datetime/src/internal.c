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
 *	@(#)internal.c
 *
 *       T2EX: calendar functions
 *       internal definitions
 */

#include <basic.h>
#include <ctype.h>
#include "internal.h"

/* Number of days for each month (non-leap year) */
EXPORT const H _dt_mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* Name of the month */
EXPORT const char _dt_monthnm[9*12 + 1] = \
	"January  "	\
	"February "	\
	"March    "	\
	"April    "	\
	"May      "	\
	"June     "	\
	"July     "	\
	"August   "	\
	"September"	\
	"October  "	\
	"November "	\
	"December "	;

/* Name of the week */
EXPORT const char _dt_weeknm[9*7 + 1] = \
	"Sunday   "	\
	"Monday   "	\
	"Tuesday  "	\
	"Wednesday"	\
	"Thursday "	\
	"Friday   "	\
	"Saturday "	;

/* AM/PM */
EXPORT const char _dt_ampm[2*2 + 1] = "AM" "PM";

/* Fetch a single strftime/strptime token  */
EXPORT int _dt_spec( const char **format, char *cp )
{
	char c; const char *p;

        p = *format; c = *p;
	if (c == '%') {
		for(;;) {
			c = *++p;

			/* Locale modifiers are simply ignored */
			if ((c == 'E' || c == 'O') && isalpha(p[1]))  {
				c = *++p;
			}
			
			if (isalpha(c)) {
				*format = ++p;
				*cp = 0;
				return c;
			}
			else {
				break;
			}
		}
	}

	if (c != 0) { p++; }
	*format = p;
	*cp = c;
	return (c != 0);
}
