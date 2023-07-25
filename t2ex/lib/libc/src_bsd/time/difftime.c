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

/*	$OpenBSD: difftime.c,v 1.9 2005/08/08 08:05:38 espie Exp $ */
/*
** This file is in the public domain, so clarified as of
** 1996-06-05 by Arthur David Olson.
*/

/*LINTLIBRARY*/
#define TYPE_INTEGRAL(type) (((type) 0.5) != 0.5)
#define TYPE_SIGNED(type) (((type) -1) < 0)
#include <time.h>	/* for time_t, TYPE_INTEGRAL, and TYPE_SIGNED */

double
difftime(time1, time0)
const time_t	time1;
const time_t	time0;
{
	/*
	** If (sizeof (double) > sizeof (time_t)) simply convert and subtract
	** (assuming that the larger type has more precision).
	** This is the common real-world case circa 2004.
	*/
	if (sizeof (double) > sizeof (time_t))
		return (double) time1 - (double) time0;
	if (!TYPE_INTEGRAL(time_t)) {
		/*
		** time_t is floating.
		*/
		return time1 - time0;
	}
	if (!TYPE_SIGNED(time_t)) {
		/*
		** time_t is integral and unsigned.
		** The difference of two unsigned values can't overflow
		** if the minuend is greater than or equal to the subtrahend.
		*/
		if (time1 >= time0)
			return time1 - time0;
		else	return -((double) (time0 - time1));
	}
	/*
	** time_t is integral and signed.
	** Handle cases where both time1 and time0 have the same sign
	** (meaning that their difference cannot overflow).
	*/
	if ((time1 < 0) == (time0 < 0))
		return time1 - time0;
	/*
	** time1 and time0 have opposite signs.
	** Punt if unsigned long is too narrow.
	*/
	if (sizeof (unsigned long) < sizeof (time_t))
		return (double) time1 - (double) time0;
	/*
	** Stay calm...decent optimizers will eliminate the complexity below.
	*/
	if (time1 >= 0 /* && time0 < 0 */)
		return (unsigned long) time1 +
			(unsigned long) (-(time0 + 1)) + 1;
	return -(double) ((unsigned long) time0 +
		(unsigned long) (-(time1 + 1)) + 1);
}
