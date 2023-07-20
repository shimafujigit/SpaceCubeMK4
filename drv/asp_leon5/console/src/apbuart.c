/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for ASP-LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/11/15.
 *
 *----------------------------------------------------------------------
 */

/*
 *	apbuart.c	Console/Low-level serial I/O driver
 *
 *	Serial line low-level driver (for ASP-LEON5: APBUART)
 */

#include "line_drv.h"
#include <sys/sysinfo.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/apbuart.h>
#include <drv/apbuart.h>
#include <drv/osal.h>

/*
 *  Define the serial controller operation function groups
 */
LOCAL	ER	apbuart_in( LINE_INFO *li, UB *buf, W len, W *alen, W tmout );
LOCAL	ER	apbuart_out( LINE_INFO *li, UB *buf, W len, W *alen, W tmout );
LOCAL	ER	apbuart_ctl( LINE_INFO *li, W kind, UW *arg );
LOCAL	void	apbuart_up( LINE_INFO *li );
LOCAL	void	apbuart_down( LINE_INFO *li );

EXPORT SC_FUNC	ScFuncAPBUART = {
	apbuart_in, apbuart_out, apbuart_ctl, apbuart_up, apbuart_down
};

/* I/O operation macros */
#define	IOB		((volatile struct apbuart_regs*)scdefs->c.iobase)
#define	InW(ix)		(IOB->ix)
#define	OutW(ix, dt)	(IOB->ix = (dt))

#define	APBUART_DEVNO	(scdefs->dt[0])		/* APBUART device no */

#define	RxBFULL		0x80			/* Receive-buffer overflow */

#define	APBUART_STATUS_ERROR	(APBUART_STATUS_FE |	\
				 APBUART_STATUS_PE |	\
				 APBUART_STATUS_OV |	\
				 APBUART_STATUS_BR)

#define	APBUART_CTRL_NS		(1 << 15)
#define	APBUART_CTRL_SI		(1 << 14)
#define	APBUART_CTRL_DI		(1 << 13)
#define	APBUART_CTRL_BI		(1 << 12)

/*
 *  Disable APBUART interrupt
 */
LOCAL	void	apbuart_disint( SC_DEFS *scdefs )
{
	DisableInt(scdefs->c.intvec);
}

/*
 *  Enable APBUART interrupt
 */
LOCAL	void	apbuart_enaint( SC_DEFS *scdefs )
{
	EnableInt(scdefs->c.intvec);
}

/*
 *  Suspend/Resume hardware settings (clock, i/o port, etc)
 */
LOCAL	void	apbuart_res_sus( SC_DEFS *scdefs, BOOL resume )
{
	/* Nothing to do */
}

/*
 *  Initialize the serial controller (mode setting)
 */
LOCAL	 ER	apbuart_hwinit( LINE_INFO *li, RsMode mode, RsFlow flow )
{
	SC_DEFS	*scdefs = &li->scdefs;
	struct apbuart_priv *priv = (void*)scdefs->c.iostep;
	UW	imsk;
	UW	ctrl;

	/* Parameter check: data:8 bits, stop:1/2 bits */
	if ( mode.parity == 3 || mode.datalen < 3 || (mode.stopbits & 1) != 0 )
		return E_PAR;

	ctrl = 0;

	/* No H/W flow control */
	if ( flow.csflow != 0 || flow.rsflow != 0 ) ctrl |= APBUART_CTRL_FL;

	/* Set line mode */
	if ( mode.stopbits != 0 ) ctrl |= APBUART_CTRL_NS; /* stop 2 bits */
	switch ( mode.parity ) {
	  case 1:	/* odd parity */
		ctrl |= APBUART_CTRL_PE|APBUART_CTRL_PS;
		break;
	  case 2:	/* even parity */
		ctrl |= APBUART_CTRL_PE;
		break;
	}

	DI(imsk);

	/* wait TX done */
	while ( (InW(status) & APBUART_STATUS_TS) == 0 );

	/* set UART mode */
	OutW(scaler, priv->apbfreq / (mode.baud * 8 + 7));
	OutW(ctrl, ctrl);

	/* Initialize serial information */
	li->mode    = mode;
	li->flow    = flow;
	li->flowsts = li->scdefs.flowsts;
	li->in_rptr = li->in_wptr  = 0;
	li->ou_rptr = li->ou_wptr  = li->ou_cnt = 0;
	li->lsts    = li->lstshist = 0;

	li->enable = 1;

	if ( li->suspend == 0 ) {
		/* TX,RX enable */
		ctrl |=	APBUART_CTRL_RF | APBUART_CTRL_BI |
			APBUART_CTRL_DI | APBUART_CTRL_RI |
			APBUART_CTRL_TE | APBUART_CTRL_RE;
		OutW(ctrl, ctrl);
	}

	EI(imsk);

	return E_OK;
}

/*
 *  Interrupt processing : status
 */
LOCAL	BOOL	apbuart_st_inthdr( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UW	lsts;

	lsts = InW(status) & APBUART_STATUS_ERROR;
	if ( lsts != 0 ) {
		/* RX error */
		OutW(status, 0);	/* clear error */

		/* Discard receive data */
		while ( (InW(status) & APBUART_STATUS_DR) != 0 ) InW(data);

		/* Record error */
		li->lsts |= lsts;
		li->lstshist |= lsts;

		/* Generate event when the receive-buffer is empty */
		if ( li->in_wptr == li->in_rptr ) {
			tk_set_flg(li->flg, FLG_IN_NORM);
		}

		return FALSE;
	}

	return TRUE;
}

/*
 *  Interrupt processing : receive
 */
LOCAL	void	apbuart_rx_inthdr( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;
	UB	c;
	UW	ptr, nptr;

	while ( (InW(status) & APBUART_STATUS_DR) != 0 ) {

		/* Get receive data */
		c = InW(data);

		/* Call the external function */
		if ( li->extfn != NULL ) {
			if ( (*li->extfn)(li->extpar, c) ) continue;
		}

		/* Send flow control : XON/XOFF only. */
		if ( li->flow.sxflow != 0 ) {
			if ( c == XOFF ) {
				li->flow.rcvxoff = 1;
				continue;
			}
			if ( li->flow.rcvxoff != 0 &&
			     ( c == XON || li->flow.xonany != 0 ) ) {
				li->flow.rcvxoff = 0;
				continue;
			}
		}

		/* Record and discard the error status
		   when the receive buffer is full  */
		ptr = li->in_wptr;
		nptr = PTRMASK(li, ptr + 1);
		if ( nptr == li->in_rptr ) {
			li->lsts |= RxBFULL;	/* Rx Buffer Full */
			li->lstshist |= RxBFULL;
			continue;
		}

		/* Store the receive data to the receive-buffer */
		li->in_buf[ptr] = c;
		li->in_wptr = nptr;

		/* Generate the event when a receive-buffer is empty */
		if ( ptr == li->in_rptr ) {
			tk_set_flg(li->flg, FLG_IN_NORM);
		}

		/* Receive flow control */
		if ( li->flow.rxflow != 0 && IN_BUF_REMAIN(li) < XOFF_MARGIN ) {

			/* XON/XOFF flow control */
			if ( li->flowsts.sndxoff == 0 ) {
				li->flowsts.sndxoff = 1;
				li->flowsts.reqchar = XOFF;
			}
		}
	}
}

/*
 *  Interrupt processing : transmit
 */
LOCAL	void	apbuart_tx_inthdr( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;
	W	n;

	/* Check XOFF/CS flow control */
	if ( li->flow.rcvxoff != 0 ) {
		/* Tx interrupt disable */
		OutW(ctrl, InW(ctrl) & ~APBUART_CTRL_TF);
		return;
	}

	if ( (InW(status) & APBUART_STATUS_TF) != 0 ) {
		/* Tx interrupt enable */
		OutW(ctrl, InW(ctrl) | APBUART_CTRL_TF);
		return;
	}

	/* Transmit */
	n = li->ou_wptr - li->ou_rptr;		/* Send data counts */

	if ( li->flowsts.reqchar != 0 ) {	/* Send XON/XOFF */
		OutW(data, li->flowsts.reqchar & 0xff);
		li->flowsts.reqchar = 0;
	}

	while ( n > 0 ) {			/* Send data */
		if ( (InW(status) & APBUART_STATUS_TF) != 0 ) break;

		OutW(data, li->ou_buf[OU_PTRMASK(li, li->ou_rptr++)]);
		n--;
	}
	if ( n == 0 ) {
		/* Tx interrupt disable */
		OutW(ctrl, InW(ctrl) & ~APBUART_CTRL_TF);
	} else {
		/* Tx interrupt enable */
		OutW(ctrl, InW(ctrl) | APBUART_CTRL_TF);
	}

	/* Send (completion)-notification event */
	if ( n < li->ou_cnt ) {
		tk_set_flg(li->flg, FLG_OU_NORM);
		li->ou_cnt = 0;
	}
}

/*
 *  Interrupt processing
 */
LOCAL	void	apbuart_inthdr( UINT dintno )
{
	LINE_INFO	*li;
	SC_DEFS		*scdefs;
	W		n;

	/* Search the port informartion */
	for ( li = &LineInfo[0], n = nPorts; ; li++ ) {
		if ( --n < 0 ) return;
		scdefs = &li->scdefs;
		if ( dintno == scdefs->c.intvec ) break;
	}

	if ( li->enable != 0 ) {
		/* RX or Status interrupt */
		if ( apbuart_st_inthdr(li) ) {
			apbuart_rx_inthdr(li);
		}
		/* Tx interrupt */
		apbuart_tx_inthdr(li);
	}
}

/*
 *  Set the line status
 */
LOCAL	RsStat	make_line_stat( LINE_INFO *li, UB lsts )
{
	union {
		RsStat	stat;
		UW	uw;
	} u;

	u.uw = 0;
	if ( lsts & RxBFULL )		u.stat.BE = 1;
	if ( lsts & APBUART_STATUS_BR )	u.stat.BD = 1;
	if ( lsts & APBUART_STATUS_FE )	u.stat.FE = 1;
	if ( lsts & APBUART_STATUS_PE )	u.stat.PE = 1;
	if ( lsts & APBUART_STATUS_OV )	u.stat.OE = 1;
	u.stat.XF = li->flow.rcvxoff;
	u.stat.DR = 1;		/* Not available */
	u.stat.CD = 1;		/* Not available */
	u.stat.CS = 1;		/* Not available */
	//u.stat.CI = 0;	/* Not available */

	return u.stat;
}

/*
 *  One character input
 */
LOCAL	WRTN	recv_one_char( LINE_INFO *li, W tmout )
{
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
	if ( li->flowsts.sndxoff != 0 && IN_BUF_REMAIN(li) > XON_MARGIN ) {
		DI(imsk);
		/* It is a receive-stop status.
			However, the receive-buffer has some empty space  */
		if ( li->flowsts.sndxoff != 0 ) {
			/* XOFF flow control */
			li->flowsts.sndxoff = 0;
			li->flowsts.reqchar = XON;
			apbuart_tx_inthdr(li);
		}
		EI(imsk);
	}

	return c;
}

/*
 *  Input from serial port
 */
LOCAL	ER	apbuart_in( LINE_INFO *li, UB *buf, W len, W *alen, W tmout )
{
	W	c;
	UW	rsz = 0;
	RsErr	err;

	if ( li->enable == 0 ) return E_NOMDA;

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
LOCAL	ER	apbuart_out( LINE_INFO *li, UB *buf, W len, W *alen, W tmout )
{
	RsErr	err;
	W	sz, n;
	RTN	r;
	UW	wptr, ptr, imsk;
	UINT	flgptn;
	ER	er;

	if ( li->enable == 0 ) return E_NOMDA;

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

		/* start tramsmit */
		apbuart_tx_inthdr(li);

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
LOCAL	ER	apbuart_ctl( LINE_INFO *li, W kind, UW *arg )
{
	SC_DEFS	*scdefs = &li->scdefs;
	ER	err = E_OK;
	UW	imsk;
	W	n;

	if ( li->enable == 0 ) return E_NOMDA;

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
				apbuart_res_sus(scdefs, TRUE);	/* Resume */
			}

			/* Reset controller */
			li->suspend = ( kind == RS_SUSPEND ) ? 1 : 0;
			err = apbuart_hwinit(li, li->mode, li->flow);

			if ( kind == RS_SUSPEND ) {
				apbuart_res_sus(scdefs, FALSE);	/* Suspend */
			}
		}
		break;

	  case DN_RSMODE:	/* Set the communication mode */
		err = apbuart_hwinit(li, *(RsMode*)arg, li->scdefs.flow);
		break;

	  case -DN_RSMODE:	/* Get the communication mode */
		*(RsMode*)arg = li->mode;
		break;

	  case DN_RSFLOW:	/* Set the flow control */
		err = apbuart_hwinit(li, li->mode, *(RsFlow*)arg);
		break;

	  case -DN_RSFLOW:	/* Get the flow control */
		*(RsFlow*)arg = li->flow;
		break;

	  case -DN_RSSTAT:	/* Get the line status */
		*(RsStat*)arg = make_line_stat( li, li->lstshist );
		li->lstshist = 0;
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

	  case DN_RSBREAK:	/* Send break signal */
	  case RS_LINECTL:	/* Set the "ON/OFF" of control line */
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
LOCAL void	apbuart_down( LINE_INFO *li )
{
	SC_DEFS	*scdefs = &li->scdefs;

	if ( li->enable != 0 ) {
		apbuart_disint(scdefs);			/* Disable interrupt */
		tk_def_int(scdefs->c.intvec, NULL);	/* Delete int handler */
		apbuart_res_sus(scdefs, FALSE);		/* Suspend */
		li->enable = 0;

#if 0	/* omit for safety */
		/* BCC driver close */
		apbuart_close((struct apbuart_priv*)scdefs->c.iostep);
#endif
	}
}

/*
 *	Start up the serial port
 */
LOCAL void	apbuart_up( LINE_INFO *li )
{
	struct apbuart_priv *priv;

	SC_DEFS	*scdefs = &li->scdefs;
	T_DINT	dint;
	W	i;
	ER	err;

	/* BCC driver open */
	priv = apbuart_open(APBUART_DEVNO);
	if ( priv == NULL ) {
		li->enable = 0;
		return;
	}
	scdefs->c.iobase = (UW)priv->regs;
	scdefs->c.iostep = (UW)priv;
	i = bcc_int_map_get(priv->interrupt);
	scdefs->c.intvec = ( i < 0 )? priv->interrupt: i;

	/* Disable interrupt */
	apbuart_disint(scdefs);

	/* Define Interrupt handler */
	dint.intatr = TA_HLNG;
	dint.inthdr = (FP)apbuart_inthdr;
	err = tk_def_int(scdefs->c.intvec, &dint);
	if ( err >= E_OK ) {
		apbuart_res_sus(scdefs, TRUE);	/* Resume */

		/* Set suspend mode except debug port */
		li->suspend = !( isDebugPort(li) && _isDebugMode() );

		/* Initialize port */
		apbuart_hwinit(li, li->scdefs.mode, li->scdefs.flow);

		/* Enable interrupt */
		apbuart_enaint(scdefs);
	} else {
		/* Shutdown port */
		li->enable = 1;
		apbuart_down(li);
	}
}

/* ------------------------------------------------------------------------ */

/*
 * BCC driver registration
 */
EXPORT void apbuart_devreg( BOOL StartUp )
{
	UW	imask;

	if ( StartUp ) {
		DI(imask);
		if ( apbuart_dev_count() == 0 ) {
			apbuart_autoinit();
		}
		EI(imask);
	}
}
