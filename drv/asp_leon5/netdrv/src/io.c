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
 *	@(#)io.c
 *
 */

#include	"netdrv.h"

/*
 *	Memory block outputs (8 bit)	cnt : number of bytes
 */
EXPORT	void	OutMemB(UW iob, W ix, UB *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		out_b(port, *buf++);
	}
}

/*
 *	Memory block inputs (8 bit)	cnt : number of bytes
 */
EXPORT	void	InMemB(UW iob, W ix, UB *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		*buf++ = in_b(port);
	}
}

/*
 *	Memory block outputs (16 bit)	cnt : number of half words
 */
EXPORT	void	OutMemH(UW iob, W ix, UH *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		out_h(port, *buf++);
	}
}

/*
 *	Memory block inputs (16 bit)	cnt : number of half words
 */
EXPORT	void	InMemH(UW iob, W ix, UH *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		*buf++ = in_h(port);
	}
}

/*
 *	Memory block outputs (32 bit)	cnt : number of words
 */
EXPORT	void	OutMemW(UW iob, W ix, UW *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		out_w(port, *buf++);
	}
}

/*
 *	Memory block inputs (32 bit)	cnt : number of words
 */
EXPORT	void	InMemW(UW iob, W ix, UW *buf, W cnt)
{
	UW	port = iob + (ix * IOSTEP);

	while (--cnt >= 0) {
		*buf++ = in_w(port);
	}
}

