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
 *	@(#)tkn_clock.c
 *
 */

#include "netmain/tkn_clock.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/intr.h>
#include "netmain/tkn_spl.h"


int hardclock_ticks;
static int gClockId = 0;

static void hand(void* arg)
{
	(void)arg;

	callout_hardclock();
}

void
tkn_clockintr(void)
{
	hardclock_ticks++;
	tkn_spl_submit_intr_di(gClockId);
}

int
tkn_clock_register(void)
{
	int id = tkn_spl_make_handler(hand, SOFTINT_CLOCK, NULL);
	if (id < 0)
		goto err_register;

	gClockId = id;
	return 0;

err_register:
	return id;
}

int tkn_clock_unregister(void)
{
	return tkn_spl_delete_handler(gClockId);
}
