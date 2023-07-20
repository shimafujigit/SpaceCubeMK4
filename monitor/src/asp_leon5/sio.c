/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/10/27.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sio.c
 */

#include <basic.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/apbuart.h>
#include <drv/osal.h>

/* sysdepend.c */
IMPORT	void	wait_usec( W usec );

/* Receive buffer */
LOCAL	UB	rcvbuf[16];
LOCAL	UB	iptr, optr;
#define	PTRMSK		(sizeof(rcvbuf) - 1)

/*
 *	Initailize Serial I/O
 */
EXPORT	void	init_sio( W baud )
{
	volatile struct apbuart_regs *regs;

	if ( __bcc_con_handle == 0 ) {
		if ( __bcc_con_init() != BCC_OK ) return;
	}
	regs = (void*)__bcc_con_handle;

	/* Set baudrate */
	regs->scaler = osal_busfreq() / (baud * 8 + 7);

	/* Initialize Recv Buffer */
	optr = iptr = 0;
}

/*
 *	Receive
 */
LOCAL	void	recv_sio( W tmo )
{
	volatile struct apbuart_regs *regs = (void*)__bcc_con_handle;
	const UW errsts = APBUART_STATUS_FE |
			  APBUART_STATUS_PE |
			  APBUART_STATUS_OV |
			  APBUART_STATUS_BR;
	UB	c;


	if ( __bcc_con_handle == 0 ) return;

	/* Input data into the receive buffer */
	while ( (UB)(iptr - optr) < sizeof(rcvbuf) ) {

		if ( (regs->status & APBUART_STATUS_DR) == 0 ) {
			/* RX empty */
			if ( iptr != optr ) break;	/* alreay received */
			if ( tmo >= 0 && --tmo < 0 ) break;	/* Time out */
			wait_usec(20);
			continue;
		}

		/* Input data */
		c = regs->data;

		if ( (regs->status & errsts) != 0 ) {
			/* RX error */
			while ( (regs->status & APBUART_STATUS_DR) != 0 ) {
				c = regs->data;	/* discard receive data */
			}
			regs->status = 0;	/* clear error */
			continue;
		}

		/* Set receoved data into buffer */
		rcvbuf[iptr++ & PTRMSK] = c;
	}
}

/*
 *	Output to SIO
 */
EXPORT	void	put_sio( B ch )
{
	volatile struct apbuart_regs *regs = (void*)__bcc_con_handle;

	if ( __bcc_con_handle == 0 ) return;

	/* wait TX ready */
	while ( (regs->status & APBUART_STATUS_TE) == 0 ) recv_sio(0);

	/* send */
	regs->data = ch & 0xff;

	/* wait TX ready */
	while ( (regs->status & APBUART_STATUS_TE) == 0 ) recv_sio(0);
}

/*
 *	Input from SIO
 */
EXPORT	W	get_sio( W tmo )
{
	if ( tmo > 0 ) tmo *= 1000/20;	/* 20 usec unit */
	recv_sio(tmo);

	/* return data in receive buffer */
	return ( iptr == optr ) ? -1 : rcvbuf[optr++ & PTRMSK];
}
