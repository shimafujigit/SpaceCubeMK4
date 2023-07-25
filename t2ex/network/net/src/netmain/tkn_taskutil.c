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
 *	@(#)tkn_taskutil.c
 *
 */

#include "netmain/tkn_taskutil.h"

#include <sys/param.h>
#include <sys/kernel.h>
#if 0
#include <string.h>
#endif

#include <tk/tkernel.h>

static __inline VP
strexinf(const char *s)
{
	union exinf {
		B c[4];
		VP p;
	} exinf;

	if (s)
		strncpy((char*)exinf.c, s, sizeof(exinf.c));
	else
		exinf.p = NULL;

	return exinf.p;
}

#if 0
/*
 * Convert a string into a VP-type object to store it in 'exinf'.
 */
VP
tkn_strexinf(const char *s)
{
	return strexinf(s);
}
#endif

ID
tkn_cre_sem(const char *name)
{
	T_CSEM csem = {
		.sematr  = TA_TFIFO,
		.maxsem  = 65535,
		.isemcnt = 0,
		.exinf   = strexinf(name),
	};
	return tk_cre_sem(&csem);
}

ID
tkn_cre_mbf(INT maxmsz, INT bufsz, const char *name)
{
	T_CMBF cmbf = {
		.mbfatr = TA_TFIFO,
		.maxmsz = maxmsz,
		.bufsz	= bufsz,
		.exinf	= strexinf(name),
	};
	return tk_cre_mbf(&cmbf);
}

ID
tkn_cre_tsk(void (*entry)(INT, VP), PRI prio, INT stksz, VP exinf)
{
	ER     er;
	T_CTSK ctsk = {
		.exinf   = exinf,
		.tskatr  = (TA_HLNG | TA_RNG0),
		.task    = entry,
		.itskpri = prio,
		.stksz   = stksz ? stksz : (16 * 1024 * 2),
		.sstksz  = 0,
		.stkptr  = NULL,
		.uatb    = NULL,
		.lsid    = 0,
		.resid   = 0,
	};

	er = tk_cre_tsk(&ctsk);
	return er;
}
