/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R
 *
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2012/12/20.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sio.c
 */

#include <basic.h>
#include "iodefine.h"

/* Receive buffer */
LOCAL	UB	rcvbuf[64];
LOCAL	UW	iptr;
LOCAL	UW	optr;
#define	PTRMSK		(sizeof(rcvbuf) - 1)

#define	I_CLK		60
#define	P_CLK		30
#define	LOOP_us(n)	((n) * I_CLK / 4 + 1)

/*
 *	Wait usec
 */
LOCAL	void	wait_usec(W usec)
{
	UW	i;

	for (i = LOOP_us(usec); i != 0; i--);
}

/*
 *	Initailize Serial I/O
 */
EXPORT	void	init_sio(W baud)
{
	SCIF.SCSCR.WORD = 0x0000;	/* SCIF Reset */
	SCIF.SCFCR.WORD = 0x0006;	/* FIFO clear */
	SCIF.SCSMR.WORD = 0x0000;	/* No Parity, 8 bits, 1 Stop, PCLK */
	/* Set Baud Rate : (P_CLK * 1000000 / (32 * baud)) - 1 + 0.5 */
	SCIF.SCBRR = ((P_CLK * 100000) / (32 * baud / 100) - 5) / 10;
					/* Set Baud Rate  */

	wait_usec( 4000000 / baud );	/* Wait over 1 bit time */

	SCIF.SCFCR.WORD = 0x30;			/* FIFO tx level = empty */
	SCIF.SCFSR.WORD = ~(SCIF.SCFSR.WORD);	/* Clear Status */
	SCIF.SCSCR.WORD = 0x30;			/* TE, RE, No Interrupt */

	/* Initialize Recv Buffer */
	optr = iptr = 0;
}

/*
 *	Output to SIO
 */
EXPORT	void	put_sio(B ch)
{
	/* Wait Tx FIFO empty */
	while (SCIF.SCFSR.BIT.TDFE == 0);

	/* Send one character */
	SCIF.SCFTDR = ch;
	while (SCIF.SCFSR.BIT.TEND == 0);

	/* Clear Tx status */
	SCIF.SCFSR.BIT.TDFE = 0;
	SCIF.SCFSR.BIT.TEND = 0;
}

/*
 *	Input from SIO
 */
EXPORT	W	get_sio(W tmo)
{
	W	c, sts;

	if (tmo > 0) tmo *= 20;	/* 50 usec unit */

	/* Input data into the receive buffer */
	while (iptr - optr < sizeof(rcvbuf)) {

		/* Check FIFO rcv data counts */
		if ( SCIF.SCFDR.BIT.R == 0) {
			if (iptr != optr) break;	/* alreay received */
			if (tmo >= 0 && --tmo < 0) break;	/* Time out */
			wait_usec(50);
			continue;
		}
		/* Input data */
		c = SCIF.SCFRDR;

		/* Clear ER, RDF, DR */
		sts = SCIF.SCFSR.WORD;
		SCIF.SCFSR.WORD = ~ (sts & 0x93);
		if ( SCIF.SCLSR.BIT.ORER != 0 ) {
			SCIF.SCLSR.BIT.ORER = 0;	/* Clear */
			continue;
		}

		/* Check error */
		if ((sts & 0x9C) != 0) continue;

		/* Set receoved data into buffer */
		rcvbuf[iptr++ & PTRMSK] = c;
	}

	/* return data in receive buffer */
	return (iptr == optr) ? -1 : rcvbuf[optr++ & PTRMSK];
}

