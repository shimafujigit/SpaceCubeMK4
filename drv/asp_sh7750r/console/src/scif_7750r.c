/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for ASP-SH7750R
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/1/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	scif_7750r.c	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver (for ASP-SH7750R: SCIF)
 */

#include "line_drv.h"
#include <sys/sysinfo.h>

/*
 *  Define the serial controller operation function groups
 */
LOCAL	ER	scif_in(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL	ER	scif_out(LINE_INFO *li, UB *buf, W len, W *alen, W tmout);
LOCAL	ER	scif_ctl(LINE_INFO *li, W kind, UW *arg);
LOCAL	void	scif_up(LINE_INFO *li);
LOCAL	void	scif_down(LINE_INFO *li);

EXPORT SC_FUNC	ScFuncSCIF = {
	scif_in, scif_out, scif_ctl, scif_up, scif_down
};

/*
 *  Definitions of SH7750R SCIF registers
 */
#define	SC_SMR		0x00	/* Serial mode			(H, RW)	*/
#define	SC_BRR		0x04	/* Bit rate			(B, RW)	*/
#define	SC_SCR		0x08	/* Serial control		(H, RW)	*/
#define	SC_FTDR		0x0C	/* Transmit FIFO data		(B, -W)	*/
#define	SC_FSR		0x10	/* Serial status		(H, RW)	*/
#define	SC_FRDR		0x14	/* Receive FIFO data		(B, R-)	*/
#define	SC_FCR		0x18	/* FIFO control			(H, RW)	*/
#define	SC_FDR		0x1C	/* FIFO data count		(H, R-)	*/
#define	SC_SPTR		0x20	/* Serial port			(H, RW)	*/
#define	SC_LSR		0x24	/* Line status			(H, RW)	*/

/* SC_SMR */
#define	SMR_CHR		0x40	/* Character length - 0:8bit 1:7bit	*/
#define	SMR_PE		0x20	/* Parity enable - 0:Dis 1:En		*/
#define	SMR_OE		0x10	/* Parity mode - 0:Even 1:Odd		*/
#define	SMR_STOP	0x08	/* Stop bit length - 0:1bit 1:2bit	*/
#define	SMR_CKS_1	0x00	/* Clock select - Pclk /  1		*/
#define	SMR_CKS_4	0x01	/* Clock select - Pclk /  4		*/
#define	SMR_CKS_16	0x02	/* Clock select - Pclk / 16		*/
#define	SMR_CKS_64	0x03	/* Clock select - Pclk / 64		*/

/* SC_SCR */
#define	SCR_TIE		0x80	/* Tx interrupt enable - 0:Dis 1:En	*/
#define	SCR_RIE		0x40	/* Rx interrupt enable - 0:Dis 1:En	*/
#define	SCR_TE		0x20	/* Tx enable - 0:Dis 1:En		*/
#define	SCR_RE		0x10	/* Rx enable - 0:Dis 1:En		*/
#define	SCR_REIE	0x08	/* ER/BR interrupt enable - 0:Dis 1:En	*/
#define	SCR_CKE		0x02	/* Clock source - 0:Internel 1:Externel	*/

/* SC_FSR */
#define	FSR_PERC	0xF000	/* Parity error count (R)		*/
#define	FSR_FERC	0x0F00	/* Framing error count (R)		*/
#define	FSR_ER		0x80	/* Rx error - 0:none 1:error (RC)	*/
#define	FSR_TEND	0x40	/* Tx end - 0:busy 1:completed (RC)	*/
#define	FSR_TDFE	0x20	/* Tx FIFO empty - 0:filled 1:empty (RC)*/
#define	FSR_BRK		0x10	/* Break detect - 0:none 1:detected (RC)*/
#define	FSR_FER		0x08	/* Framing error - 0:none 1:error (R)	*/
#define	FSR_PER		0x04	/* Parity error - 0:none 1:error (R)	*/
#define	FSR_RDF		0x02	/* Rx FIFO full - 0:empty 1:filled (RC)	*/
#define	FSR_DR		0x01	/* Rx ready - 0:data rcvd. 1:none (RC)	*/

#define	FSR_ERMSK	(FSR_ER | FSR_BRK | FSR_FER | FSR_PER)

/* SC_FCR */
#define	FCR_RSTRG	0x700	/* RTS trigger (15,1,4,6,8,10,12,14)	*/
#define	FCR_RTRG_1	0x00	/* Rx FIFO trigger -  1byte		*/
#define	FCR_RTRG_4	0x40	/* Rx FIFO trigger -  4byte		*/
#define	FCR_RTRG_8	0x80	/* Rx FIFO trigger -  8byte		*/
#define	FCR_RTRG_14	0xc0	/* Rx FIFO trigger - 14byte		*/
#define	FCR_TTRG_8	0x00	/* Tx FIFO trigger -  8byte space	*/
#define	FCR_TTRG_12	0x10	/* Tx FIFO trigger - 12byte space	*/
#define	FCR_TTRG_14	0x20	/* Tx FIFO trigger - 14byte space	*/
#define	FCR_TTRG_15	0x30	/* Tx FIFO trigger - 15byte space	*/
#define	FCR_TTRG_MSK	0x30
#define	FCR_MCE		0x08	/* Modem control enable - 0:Dis 1:En	*/
#define	FCR_TFRST	0x04	/* Tx FIFO reset - 0:Dis 1:En		*/
#define	FCR_RFRST	0x02	/* Rx FIFO reset - 0:Dis 1:En		*/
#define	FCR_LOOP	0x01	/* Loopback test - 0:Dis 1:En		*/

#define	FIFO_SIZE	16	/* FIFO size				*/

/* SC_FDR */
#define	TFCNT(n)	(((n) >> 8) & 0x1F)	/* Tx FIFO data count	*/
#define	RFCNT(n)	((n) & 0x1F)		/* Rx FIFO data count	*/

/* SC_SPTR */
#define	SPTR_RTSIO	0x80	/* RTS output enable			*/
#define	SPTR_RTSDT	0x40	/* RTS data				*/
#define	SPTR_CTSIO	0x20	/* CTS output enable			*/
#define	SPTR_CTSDT	0x10	/* CTS data				*/
#define	SPTR_SPB2IO	0x02	/* Tx output enable			*/
#define	SPTR_SPB2DT	0x01	/* Tx output data			*/

/* SC_LSR */
#define	LSR_ORER	0x01	/* Overrun error			*/

/* I/O operation macros */
#define	InB(ix)		in_b(scdefs->c.iobase + (ix))
#define	InH(ix)		in_h(scdefs->c.iobase + (ix))
#define	OutB(ix, dt)	out_b(scdefs->c.iobase + (ix), (dt))
#define	OutH(ix, dt)	out_h(scdefs->c.iobase + (ix), (dt))

/* SC_DEFS: HW port Information */
#define	INT_VEC		(scdefs->c.intvec)
#define	ERI(v)	((v) + 0x00)	/* Error interrupt */
#define	RXI(v)	((v) + 0x20)	/* Receive interrupt */
#define	BRI(v)	((v) + 0x40)	/* Break interrupt */
#define	TXI(v)	((v) + 0x60)	/* Transmit interrupt */
#define	INT_LEVEL	(scdefs->c.iostep)
#define	ENABLE		dt[0]
#define	MCTL		dt[1]
#define	FCTL		fctl

/* Additional receive error status */
#define	RxBFULL		(FSR_RDF)	/* Receive-buffer overflow */

/* I/O operation macros */
#define	ENB_TXRXINT	OutH(SC_SCR, SCR_TE | SCR_RE | SCR_RIE | SCR_TIE )
#define	DIS_TXRXINT	OutH(SC_SCR, SCR_TE | SCR_RE)
#define	ENB_RXINT	OutH(SC_SCR, InH(SC_SCR) | SCR_RIE )
#define	ENB_RXINT_INIT	OutH(SC_SCR, SCR_TE | SCR_RE | SCR_RIE )
#define	DIS_RXINT	OutH(SC_SCR, InH(SC_SCR) & ~SCR_RIE)
#define	ENB_TXINT	OutH(SC_SCR, InH(SC_SCR) | SCR_TIE)
#define	DIS_TXINT	OutH(SC_SCR, InH(SC_SCR) & ~SCR_TIE)

/*
 *  Suspend/Resume hardware settings (clock, i/o port, etc)
 */
LOCAL	void	scif_res_sus( SC_DEFS *scdefs, BOOL resume )
{
	/* Nothing to do */
}

/*
 *  Break signal control
 */
LOCAL	void	scif_breakctl( SC_DEFS *scdefs, W on )
{
	UW	imsk;

	DI(imsk);
	if ( on != 0 ) {
		OutH(SC_SPTR, scdefs->MCTL | SPTR_SPB2IO);/* TxD=0 output on */
		OutH(SC_SCR, InH(SC_SCR) & ~SCR_TE);	/* Tx Disable */
	} else {
		OutH(SC_FCR, scdefs->FCTL | FCR_TFRST);	/* Tx FIFO reset */
		OutH(SC_SCR, InH(SC_SCR) | SCR_TE);	/* Tx Enable */
		OutH(SC_FCR, scdefs->FCTL);		/* Tx FIFO enable */
		OutH(SC_SPTR, scdefs->MCTL);		/* TXD outout off */
	}
	EI(imsk);
}

/*
 *  Forcible setting of the control line
 */
LOCAL	ER	scif_linectl( LINE_INFO *li, UW cmd )
{
	SC_DEFS	*scdefs = &li->scdefs;

	/* Error when CTS/RTS flow control is ON */
	if ( (scdefs->FCTL & FCR_MCE) != 0 ) return E_PAR;

	/* ignore RSCTL_DTR, because DTR signal is not available */
	switch ( cmd & ~(RSCTL_DTR | RSCTL_RTS) ) {
	  case RSCTL_ON:
		if ( (cmd & RSCTL_RTS) == 0 ) return E_OK;
		break;

	  case RSCTL_OFF:
		if ( (cmd & RSCTL_RTS) == 0 ) return E_OK;
		cmd = 0;
		break;

	  case RSCTL_SET:
		break;

	  default:
		return E_PAR;
	}
	OutH(SC_SPTR, scdefs->MCTL |
			( ((cmd & RSCTL_RTS) != 0) ? 0 : SPTR_RTSDT ) );

	return E_OK;
}

/*
 *  Calculate bit rate settings
 */
LOCAL	ER	calc_brr( W baud, UB *brr, UB *cks )
{
	W	pclk = (W)SCInfo.Pclk * 1000000;	/* MHz -> Hz */
	W	n, s;

	/* N (0-255) = pclk(MHz) * 1000000 / (32 * baud) - 1 */

	baud *= 16;
	for ( s = 0; s < 4; s++ ) {
		n = pclk / ( 1 << (2 * s));	/* Clock source */
		n /= baud;
		n = (n + 1) / 2 - 1;		/* round-up */
		if ( n <= 255 ) break;		/* 0 <= n <= 255 */
	}
	if ( n <= 0 || s >= 4 ) return E_PAR;	/* n = 0 is treated as error */
	*brr = n;
	*cks = s;
	return E_OK;
}

/*
 *  Initialize the serial controller (mode setting)
 */
LOCAL	 ER	scif_init(LINE_INFO *li, RsMode mode, RsFlow flow )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UW	imsk;
	UB	smr, brr, fctl, mctl;

	/* Parameter check: data:7/8 bits, stop:1/2 bits */
	if ( mode.parity == 3 || mode.datalen < 2 || (mode.stopbits & 1) != 0 )
		return E_PAR;

	/* Calculate baud rate */
	if ( calc_brr(mode.baud, &brr, &smr) < E_OK ) return E_PAR;

	/* Set line mode */
	if ( mode.datalen == 2 ) smr |= SMR_CHR;	/* data 7 bits */
	if ( mode.stopbits != 0 ) smr |= SMR_STOP;	/* stop 2 bits */
	switch ( mode.parity ) {
	  case 1:	smr |= SMR_PE | SMR_OE;	break;
	  case 2:	smr |= SMR_PE;		break;
	}

	/* FIFO control : Set small rx trigger level accordng to the baud rate
		in order to minimize the receive delay */
	fctl = FCR_TTRG_8 | ( ( mode.baud < 9600 ) ? FCR_RTRG_1 :
			( ( mode.baud < 38400 ) ? FCR_RTRG_4 : FCR_RTRG_8 ) );

	/* CTS/RTS H/W flow control */
	if ( li->suspend != 0 ) {
		mctl = SPTR_RTSIO | SPTR_RTSDT;		/* RTS OFF */
	} else if (flow.csflow != 0 || flow.rsflow != 0) {
		flow.csflow = flow.rsflow = 1;		/* Force both on */
		fctl |= FCR_MCE;			/* H/W flow control */
		mctl = 0;				/* RTS out by H/W */
	} else {
		mctl = SPTR_RTSIO;			/* RTS ON */
	}

	scdefs->FCTL = fctl;
	scdefs->MCTL = mctl;

	DI(imsk);

	OutH(SC_SCR, 0x00);			/* Disable Tx/Rx/Int */
	OutH(SC_FCR, FCR_TFRST | FCR_RFRST);	/* Tx/Rx FIFO reset */
	OutH(SC_FSR, ~InH(SC_FSR));		/* Clear int status */
	OutH(SC_LSR, ~InH(SC_LSR));		/* Clear line status */
	OutH(SC_SPTR, mctl);			/* RTS soft control */
	li->msts = InH(SC_SPTR);		/* Initialize modem status */
	OutH(SC_SMR, smr);			/* Set data format */
	OutB(SC_BRR, brr);			/* Set bit rate */

	EI(imsk);

	WaitUsec( 4000000 / mode.baud );	/* Wait over 1 bit time */

	DI(imsk);

	/* Initialize serial information */
	li->mode    = mode;
	li->flow    = flow;
	li->flowsts = li->scdefs.flowsts;
	li->in_rptr = li->in_wptr  = 0;
	li->ou_rptr = li->ou_wptr  = li->ou_cnt = 0;
	li->lsts    = li->lstshist = 0;

	scdefs->ENABLE = 1;

	OutH(SC_FCR, fctl);			/* Set FIFO level */
	if ( li->suspend == 0 ) {
		ENB_RXINT_INIT;			/* Enable RxInt */
	}

	EI(imsk);

	return E_OK;
}

/*
 *  Interrupt processing : receive, error, break
 */
LOCAL	void	scif_rx_inthdr( UINT dintno )
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	W		i;
	UB		c;
	UW		ptr, nptr;
	UH		fsr;

	/* Search the port informartion */
	for (li = &LineInfo[0], i = nPorts; ; li++) {
		if (--i < 0) return;
		if ( dintno == RXI(li->scdefs.c.intvec) ||
		     dintno == ERI(li->scdefs.c.intvec) ||
		     dintno == BRI(li->scdefs.c.intvec) ) break;
	}
	scdefs = &li->scdefs;

	if ( scdefs->ENABLE == 0 ) {	/* Invalid */
		/* Disable Tx/Rx/Int */
		OutH(SC_SCR, 0x00);
		return;
	}

	/* Check BREAK detect */
	fsr = InH(SC_FSR);
	if ( (fsr & FSR_BRK) != 0 ) {
		/* Clear BREAK flag and record error */
		OutH(SC_FSR, fsr & ~FSR_BRK);
		li->lsts     |= FSR_BRK;
		li->lstshist |= FSR_BRK;
	}

	/* Execute the processing while receive data exists */
	while ( RFCNT(InH(SC_FDR)) != 0 ) {

		/* Get receive data */
		c = InB(SC_FRDR);

		/* Get receive status */
		fsr = InH(SC_FSR);

		/* Clear status flag */
		OutH(SC_FSR, fsr & ~(FSR_ER | FSR_RDF | FSR_DR));

		fsr &= ~LSR_ORER;
		if ( (InH(SC_LSR) & LSR_ORER) != 0 ) {	/* Overrun error */
			OutH(SC_LSR, 0);	/* Clear status */
			/* Reset Rx FIFO */
			OutH(SC_FCR, scdefs->FCTL | FCR_RFRST);
			fsr |= LSR_ORER;
			OutH(SC_FCR, scdefs->FCTL);
		}

		/* Check error */
		if ( (fsr & (FSR_ER | LSR_ORER)) != 0 ) {
			/* Record error */
			li->lsts |= (fsr &= FSR_ERMSK | LSR_ORER);
			li->lstshist |= fsr;

			/* Generate event when the receive-buffer is empty */
			if ( li->in_wptr == li->in_rptr ) {
				tk_set_flg( li->flg, FLG_IN_NORM );
			}
			continue;	/* Discard receive data */
		}

		/* Call the external function */
		if ( li->extfn != NULL ) {
			if ( (*li->extfn)( li->extpar, c ) ) continue;
		}

		/* Send flow control : XON/XOFF only.
			CTS flow contol is performed by H/W */
		if ( li->flow.sxflow != 0 ) {
			if ( c == XOFF ) {
				li->flow.rcvxoff = 1;
				DIS_TXINT;	/* Stop transmit */
				continue;
			}
			if ( li->flow.rcvxoff != 0 && 
				( c == XON || li->flow.xonany != 0 ) ) {
				li->flow.rcvxoff = 0;
				ENB_TXINT;	/* Re-start transmit */
				continue;
			}
		}

		/* Record and discard the error status
					when the receive buffer is full  */
		ptr = li->in_wptr;
		nptr = PTRMASK(li, ptr + 1);
		if ( nptr == li->in_rptr ) {
			li->lsts |= RxBFULL;		/* Rx Buffer Full */
			li->lstshist |= RxBFULL;
			continue;
		}

		/* Store the receive data to the receive-buffer */
		li->in_buf[ptr] = c;
		li->in_wptr = nptr;

		/* Generate the event when a receive-buffer is empty */
		if ( ptr == li->in_rptr ) {
			tk_set_flg( li->flg, FLG_IN_NORM );
		}

		/* Receive flow control */
		if ( ( li->flow.rxflow == 0 && li->flow.rsflow == 0 ) ||
			IN_BUF_REMAIN(li) >= XOFF_MARGIN ) continue;

		/* XON/XOFF flow control */
		if ( li->flow.rxflow != 0 && li->flowsts.sndxoff == 0 ) {
			li->flowsts.sndxoff = 1;
			li->flowsts.reqchar = XOFF;
			ENB_TXINT;	/* Transmit XOFF */
		}

		/* RTS flow control */
		if ( li->flow.rsflow != 0 && li->flowsts.rsoff == 0 ) {
			li->flowsts.rsoff = 1;
			/* Stop Receive :
				RTS becomes OFF by H/W when Rx FIFO is FULL */
			DIS_RXINT;
			break;
		}
	}

	/* Clear receive interrupt */
	OutH(SC_FSR, InH(SC_FSR) & ~(FSR_ER | FSR_RDF | FSR_DR));
}

/*
 *  Interrupt processing : transmit
 */
LOCAL	void	scif_tx_inthdr( UINT dintno )
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	W		i, n, cnt;

	/* Search the port informartion */
	for (li = &LineInfo[0], i = nPorts; ; li++) {
		if (--i < 0) return;
		if ( dintno == TXI(li->scdefs.c.intvec) ) break;
	}
	scdefs = &li->scdefs;

	if ( scdefs->ENABLE == 0 ) {	/* Invalid */
		/* Disable Tx/Rx/Int */
		OutH(SC_SCR, 0x00);
		return;
	}

	/* Update modem status */
	li->msts = InH(SC_SPTR);

	/* Check Tx FIFO full : Tx FIFO may be FULL when CTS is OFF */
	i = FIFO_SIZE - TFCNT(InH(SC_FDR));
	if ( i <= 0 ) goto EEXIT;

	/* Check XOFF/CS flow control */
	if ( li->flow.rcvxoff != 0 ) {
		DIS_TXINT;
		goto EEXIT;
	}

	/* Transmit */
	cnt = n = li->ou_wptr - li->ou_rptr;	/* Send data counts */
	if ( li->flowsts.reqchar ) n++;		/* Send XON/XOFF */
	if ( n > i ) n = i;			/* Limit to max size */

	/* Send XON/XOFF */
	if ( li->flowsts.reqchar != 0 ) {
		OutB(SC_FTDR, li->flowsts.reqchar);
		li->flowsts.reqchar = 0;
		n--;
	}

	/* Send data */
	if (n > 0) {
		cnt -= n;		/* Remaining data size */

		/* Adjust Tx FIFO trigger level */
		scdefs->FCTL &= ~FCR_TTRG_MSK;
		scdefs->FCTL |= (cnt <= 0) ? FCR_TTRG_15 : FCR_TTRG_8;
		OutH(SC_FCR, scdefs->FCTL);

		/* Enable Tx interrupt */
		//ENB_TXINT;

		while ( --n >= 0 ) {
			OutB(SC_FTDR, li->ou_buf[
					OU_PTRMASK(li, li->ou_rptr++)]);
		}
	} else {
		/* No data to send, disable Tx interrupt */
		DIS_TXINT;
	}

	/* Send (completion)-notification event */
	if ( cnt < li->ou_cnt ) {
		tk_set_flg(li->flg, FLG_OU_NORM);
		li->ou_cnt = 0;
	}
EEXIT:
	/* Clear transmit interrupt */
	OutH(SC_FSR, InH(SC_FSR) & ~(FSR_TDFE | FSR_TEND));
}

/*
 *  Set the line status
 */
LOCAL	RsStat	make_line_stat(LINE_INFO *li, UB lsts)
{
	SC_DEFS	*scdefs = &li->scdefs;
	union {
		RsStat	stat;
		UW	uw;
	} u;

	u.uw = 0;
	if ( lsts & RxBFULL ) u.stat.BE = 1;
	if ( lsts & FSR_FER ) u.stat.FE = 1;
	if ( lsts & FSR_PER ) u.stat.PE = 1;
	if ( lsts & LSR_ORER) u.stat.OE = 1;
	u.stat.XF = li->flow.rcvxoff;
	if ( lsts & FSR_BRK ) u.stat.BD = 1;
	u.stat.DR = 1;		/* Not available */
	u.stat.CD = 1;		/* Not available */
	if ( li->msts & SPTR_CTSDT ) u.stat.CS = 1;
	//u.stat.CI = 0;	/* Not available */

	return u.stat;
}

/*
 *  One character input
 */
LOCAL	WRTN	recv_one_char(LINE_INFO *li, W tmout)
{
	SC_DEFS	*scdefs = &li->scdefs;
	UB	c;
	UINT	flgptn;
	UW	imsk;
	ER	er;

	/* Wait for receiving while the receive buffer is empty */
	while ( li->in_wptr == li->in_rptr ) {

		if ( tmout == 0 ) return RTN_NONE; /* There is no wait */

		/* Wait for receiving or line status change */
		er = tk_wai_flg(li->flg, FLG_IN_WAIPTN,
				 TWF_ORW | TWF_BITCLR, &flgptn, tmout);

		if ( er == E_TMOUT ) return RTN_TMOUT;
		if ( er < E_OK || (flgptn & FLG_IN_ABORT) ) return RTN_ABORT;
		if ( li->lsts != 0 ) return RTN_ERR;	/* Receive error */
	}

	/* Get the receive data */
	c = li->in_buf[li->in_rptr];
	li->in_rptr = PTRMASK(li, li->in_rptr + 1);

	/* Receive flow control */
	if ( ( li->flowsts.rsoff != 0 || li->flowsts.sndxoff != 0 )
				&& IN_BUF_REMAIN(li) > XON_MARGIN ) {
		DI(imsk);
		/* It is a receive-stop status.
			However, the receive-buffer has some empty space  */
		if ( li->flowsts.rsoff != 0 ) {
			ENB_RXINT;		/* Re-start Receive */
			li->flowsts.rsoff = 0;
		}
		if ( li->flowsts.sndxoff != 0 ) {
			/* XOFF flow control */
			li->flowsts.sndxoff = 0;
			li->flowsts.reqchar = XON;
			ENB_TXINT;		/* Transmit XON */
		}
		EI(imsk);
	}

	return c;
}

/*
 *  Input from serial port
 */
LOCAL	ER	scif_in(LINE_INFO *li, UB *buf, W len, W *alen, W tmout)
{
	SC_DEFS	*scdefs = &li->scdefs;
	W	c;
	UW	rsz = 0;
	RsErr	err;

	if ( scdefs->ENABLE == 0 ) return E_NOMDA;

	/* Special processing to avoid deadlock at the time of polling */
	if ( tmout == 0 && li->in_wptr == li->in_rptr ) {
		err.w = ( len <= 0 ) ? E_OK : E_BUSY;
		goto EEXIT;
	}

	/* Console input processing lock */
	if ( consMLock(&li->lock, IN_LOCK) < E_OK ) {
		err.s = make_line_stat(li, 0);
		err.c.ErrorClass = MERCD(E_IO);
		err.c.Aborted = 1;
		goto EEXIT;
	}

	err.w = E_OK;

	if ( len <= 0 ) {
		/* Get the number of the received bytes */
		rsz = IN_BUF_SIZE(li);
	} else {
		li->lsts = 0;  /* Clear the error status */

		/* Receive the data */
		for ( rsz = 0; rsz < len; rsz++ ) {
			c = recv_one_char(li, tmout);
			if ( c >= RTN_OK ) {
				buf[rsz] = c;
				continue;
			}
			if ( c == RTN_NONE ) {
				err.w = ( rsz > 0 ) ? E_OK : E_BUSY;
			} else {
				err.s = make_line_stat(li, li->lsts);
				err.c.ErrorClass = MERCD(E_IO);
				if ( c == RTN_TMOUT ) err.c.Timout = 1;
				if ( c == RTN_ABORT ) err.c.Aborted = 1;
			}
			break;
		}
	}

	/* Clear the ABORT flag */
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, IN_LOCK);

 EEXIT:
	*alen = rsz;	/* The number of the actually received bytes */
	return err.w;
}

/*
 *  Output to the serial port
 */
LOCAL	ER	scif_out(LINE_INFO *li, UB *buf, W len, W *alen, W tmout)
{
	SC_DEFS	*scdefs = &li->scdefs;
	RsErr	err;
	W	sz, n;
	RTN	r;
	UW	wptr, ptr, imsk;
	UINT	flgptn;
	ER	er;

	if ( scdefs->ENABLE == 0 ) return E_NOMDA;

	*alen = 0;

	/* Console output processing lock */
	if ( consMLock(&li->lock, OU_LOCK) < E_OK ) {
		err.s = make_line_stat(li, 0);
		err.c.ErrorClass = MERCD(E_IO);
		err.c.Aborted = 1;
		goto EEXIT;
	}

	err.w = E_OK;

	/*
	 * An output handler is read by the interrupt handler, so the page-out
	 * shall not be executed.
	 * Therefore, the resident memory shall be used as the output buffer
	 * instead of using the user-specified buffer "buffer", and the output
	 * data shall be copied.
	 */

	for ( wptr = li->ou_wptr; len > 0; ) {

		/* Copy the send data to the output buffer */
		sz = li->ou_rptr + OUBUFSZ - li->ou_wptr;
		if (sz > len) sz = len;
		n = OUBUFSZ - (ptr = OU_PTRMASK(li, li->ou_wptr));
		if (n > sz) n = sz;
		if (n > 0)  memcpy(&li->ou_buf[ptr], buf, n);
		if (sz > n) memcpy(&li->ou_buf[0], &buf[n], sz - n);
		buf += sz;
		len -= sz;

		DI(imsk);
		if ( len <= 0 ) {
			/* All the send data is copied to the output buffer.
			 * Therefore, notification shall be received
			 * when it is completed
			 */
			n = 1;
			if ( tmout == 0 ) n = 0; /* No wait for completion */

		} else {
			/* The all send-data can not be copied to the output
			 * buffer, so notification shall be received when an
			 * empty space is available
			 */
			if ( (n = OUBUFSZ - len) < 20 ) n = 20;
		}
		li->ou_cnt = n;
		li->ou_wptr += sz;

		/* Enable Tx interrupt to start tramsmit */
		ENB_TXINT;

		EI(imsk);

		/* Wait-for-completion of sending */
		for ( r = RTN_OK; li->ou_cnt != 0; ) {
			ptr = li->ou_rptr;
			er = tk_wai_flg(li->flg, FLG_OU_WAIPTN,
					TWF_ORW | TWF_BITCLR, &flgptn, tmout);
			if ( er == E_TMOUT ) {
				/* Time-out error shall occur only when even
				 * one character has not been sent from the
				 * previous time.
				 */
				if ( ptr == li->ou_rptr ) {
					r = RTN_TMOUT;
					break;
				}
			} else if ( er < E_OK || (flgptn & FLG_OU_ABORT) ) {
				r = RTN_ABORT;
				break;
			}
		}
		if ( r < RTN_OK ) {
			err.s = make_line_stat(li, 0);
			err.c.ErrorClass = MERCD(E_IO);
			if ( r == RTN_TMOUT ) err.c.Timout  = 1;
			if ( r == RTN_ABORT ) err.c.Aborted = 1;
			DI(imsk);
			li->ou_wptr = li->ou_rptr;	/* Stop sending */
			EI(imsk);
			break;
		}
	}

	*alen = li->ou_wptr - wptr;	/* Actual already sent byte */
	li->ou_cnt = 0;

	/* Clear the "ABORT" flag */
	tk_clr_flg(li->flg, ~(FLG_IN_ABORT | FLG_OU_ABORT));

	/* Release the lock */
	consMUnlock(&li->lock, OU_LOCK);

EEXIT:
	return err.w;
}

/*
 * Control the serial port
 */
LOCAL	ER	scif_ctl(LINE_INFO *li, W kind, UW *arg)
{
	SC_DEFS	*scdefs = &li->scdefs;
	ER	err = E_OK;
	UW	imsk;
	W	n;

	if ( scdefs->ENABLE == 0 ) return E_NOMDA;

	/* Lock is unnecessary */

	switch (kind) {
	  case RS_ABORT:	/* Abort (release wait) */
		tk_set_flg(li->flg, FLG_IN_ABORT | FLG_OU_ABORT); /* Release */
		break;

	  case RS_SUSPEND:	/* Transit to the suspend status  */
	  case RS_RESUME:	/* Return from the suspend status */
		/* Debug port shall not transit to the "suspend"
						status in debug mode */
		if ( !(isDebugPort(li) && _isDebugMode()) ) {
			if ( kind == RS_RESUME ) {
				scif_res_sus(scdefs, TRUE);	/* Resume */
			}

			/* Reset controller */
			li->suspend = ( kind == RS_SUSPEND ) ? 1 : 0;
			err = scif_init( li, li->mode, li->flow );

			if ( kind == RS_SUSPEND ) {
				scif_res_sus(scdefs, FALSE);	/* Suspend */
			}
		}
		break;

	  case DN_RSMODE:	/* Set the communication mode */
		err = scif_init( li, *(RsMode*)arg, li->scdefs.flow );
		break;

	  case -DN_RSMODE:	/* Get the communication mode */
		*(RsMode*)arg = li->mode;
		break;

	  case DN_RSFLOW:	/* Set the flow control */
		err = scif_init( li, li->mode, *(RsFlow*)arg );
		break;

	  case -DN_RSFLOW:	/* Get the flow control */
		*(RsFlow*)arg = li->flow;
		break;

	  case -DN_RSSTAT:	/* Get the line status */
		*(RsStat*)arg = make_line_stat( li, li->lstshist );
		li->lstshist = 0;
		break;

	  case DN_RSBREAK:	/* Send break signal */
		if ( (n = *arg) > 0 ) {
			scif_breakctl(scdefs, 1);	/* BREAK ON */
			tk_dly_tsk(n);			/* wait */
			scif_breakctl(scdefs, 0);	/* BREAK OFF */
		}
		break;

	  case RS_RCVBUFSZ:	/* Set the receive-buffer */
		if ( (n = *arg) < MIN_INBUFSZ ) {
			err = E_PAR;
			break;
		}
		if ( n != li->in_bufsz ) {
			UB *new, *old;
			new = Malloc(n);
			if ( new != NULL ) {
				DI(imsk);
				old = li->in_buf;
				li->in_buf = new;
				li->in_rptr = li->in_wptr = 0;
				li->in_bufsz = n;
				EI(imsk);
				Free(old);
			} else {
				err = E_NOMEM;
			}
		}
		break;

	  case -RS_RCVBUFSZ:	/* Get the receive-buffer size */
		*arg = li->in_bufsz;
		break;

	  case RS_LINECTL:	/* Set the "ON/OFF" of control line */
		err = scif_linectl(li, *arg);
		break;

	  case RS_EXTFUNC:	/* Set the external processing function */
		if ( li->extfn != NULL && (FUNCP)arg[0] != NULL ) {
			err = E_OBJ;	/* Already set */
		} else {
			DI(imsk);
			li->extfn  = (FUNCP)arg[0];
			li->extpar = arg[1];
			EI(imsk);
		}
		break;

	  case DN_RS16450:	/* Set the hardware configuration */
	  case -DN_RS16450:	/* Get the hardware configuration */
		err = E_NOSPT;
		break;

	  default:
		err = E_PAR;
		break;
	}

	return	err;
}

/*
 *	Shutdown the serial port
 */
LOCAL void	scif_down( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;

	if ( scdefs->ENABLE != 0 ) {
		DisableInt( INT_VEC );		/* Disable interrupt */
		OutH( SC_SCR, 0x00 );		/* Disable Tx/Rx/Int */
		tk_def_int( ERI(INT_VEC), NULL ); /* Delete int handlers */
		tk_def_int( RXI(INT_VEC), NULL );
		tk_def_int( BRI(INT_VEC), NULL );
		tk_def_int( TXI(INT_VEC), NULL );
		scif_res_sus( scdefs, FALSE );	/* Suspend */
		scdefs->ENABLE = 0;
	}
}

/*
 *	Start up the serial port
 */
LOCAL void	scif_up( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;
	T_DINT	dint;
	ER	err;

	/* Disable interrupt */
	DisableInt( INT_VEC );

	/* Disable Tx/Rx/Int */
	OutH( SC_SCR, 0x00 );

	/* Define Interrupt handler */
	dint.intatr = TA_HLNG;
	dint.inthdr = (FP)scif_rx_inthdr;
	err  = tk_def_int( RXI(INT_VEC), &dint );
	err |= tk_def_int( ERI(INT_VEC), &dint );
	err |= tk_def_int( BRI(INT_VEC), &dint );
	dint.inthdr = (FP)scif_tx_inthdr;
	err |= tk_def_int( TXI(INT_VEC), &dint );

	if ( err >= E_OK ) {
		scif_res_sus( scdefs, TRUE );	/* Resume */

		/* Set suspend mode except debug port */
		li->suspend = !( isDebugPort(li) && _isDebugMode() );

		/* Initialize port */
		scif_init( li, li->scdefs.mode, li->scdefs.flow );

		/* Enable interrupt */
		EnableInt( INT_VEC, INT_LEVEL );
	} else {
		/* Shutdown port */
		scdefs->ENABLE = 1;
		scif_down( li );
	}
}
