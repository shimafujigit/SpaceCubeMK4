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
 *	@(#)inthdr.c
 *
 */

#include	"netdrv.h"

#define	NIC_INTVEC	1 /*IV_GPIO(41)*/
#define	EXBIntMode(x)	/*SetIntMode(x, IM_ENA | IM_LEVEL | IM_LOW | IM_ASYN)*/
#define	EXBIntEna(x)	EnableInt(x)
#define	EXBIntDis(x)	DisableInt(x)
#define	EXBIntClr(x)	/* */
#define	EXBIntEnd(x)	/* */

/*
 *	Network adapter table for fixed vector interrupt
 */
LOCAL	NetInf	*irqinf[MAX_NETDEV] = {NULL};

/*
 *	Interrupt handler (fixed vector interrupt)
 */
LOCAL	void	FixIntHdr(INTVEC vec)
{
	INT	i;
	NetInf	*inf;

	for (i = 0; i < MAX_NETDEV; i++) {
		inf = irqinf[i];
		if (inf != NULL && inf->di.intno == vec &&
				inf->di.stat >= E_OK ) {  /* Target */
			(*(inf->inthdr))(inf);	/* Execute interrupt handler */
			break;
		}
	}
	/* IRC interrupt finish processing */
	EXBIntClr(vec);
	EXBIntEnd(vec);
}

/*
 *	Define interrupt handler
 */
EXPORT	ER	DefIntHdr(NetInf *inf, BOOL regist)
{
	ER	er;
	T_DINT	dint;
	W	vec;

	dint.intatr = TA_HLNG;

	/* Fixed vector hardware */
	if ((vec = inf->di.intno) <= 0) {
		inf->di.intno = vec = NIC_INTVEC;
	}
	irqinf[inf->netno] = (regist) ? inf : NULL;
	dint.inthdr = (VP)FixIntHdr;
	er = tk_def_int(vec, (regist) ? &dint : NULL);

	if (er >= E_OK && regist) {
		EXBIntMode(vec);
		EXBIntEna(vec);
	} else {
		EXBIntDis(vec);
	}

	return er;
}

