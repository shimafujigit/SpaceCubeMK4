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
 *	accept.c	RS driver processing
 */
#include "rsdrv.h"

/*
 *  Check request data count
 */
LOCAL	INT	chk_datacnt( T_DEVREQ *req, W datasz )
{
	W	reqcnt = req->size;

	return ( reqcnt < datasz && reqcnt != 0 ) ? E_PAR : reqcnt;
}

/*
 *  Receive data from the serial port - use serial_in()
 */
LOCAL	INT	receive_data( RsInfo *rsinf, T_DEVREQ *req, W *alen )
{
	ER	err;

	err = serial_in( rsinf->port, req->buf, req->size,
						alen, rsinf->rcv_tmout );
	return ( err < E_OK ) ? err : *alen;
}

/*
 *  Send data to the serial port -- use serial_out()
 */
LOCAL	INT	send_data( RsInfo *rsinf, T_DEVREQ *req, W *alen )
{
	ER	err;

	err = serial_out( rsinf->port, req->buf, req->size,
						alen, rsinf->snd_tmout );
	return ( err < E_OK ) ? err : *alen;
}

/*
 *  Send Beak to the serial port -- use serial_ctl()
 */
LOCAL	INT	send_break( RsInfo *rsinf, T_DEVREQ *req )
{
	UW	brk_time;
	ER	err;

	if ( ( err = chk_datacnt( req, sizeof(UW) ) ) <= 0 ) {
		return ( err == 0 ) ? sizeof(UW) : err;
	}

	brk_time = *(UW*)req->buf;
	if ( brk_time > 0 ) {
		err = serial_ctl( rsinf->port, DN_RSBREAK, &brk_time );
	}
	return ( err < E_OK ) ? err : sizeof(UW);
}

/*
 *  Contol port parameter -- use serial_ctl()
 */
LOCAL	INT	cntrl_port( RsInfo *rsinf, T_DEVREQ *req, W dn, W sz )
{
	ER	err;

	if ( ( err = chk_datacnt( req, sz ) ) <= 0 ) {
		return ( err == 0 ) ? sz : err;
	}

	err = serial_ctl( rsinf->port, dn, req->buf );
	return ( err < E_OK ) ? err : sz;
}

/*
 *  Get (W) parameter
 */
LOCAL	INT	get_param_w( T_DEVREQ *req, W *param )
{
	ER	err;

	if ( ( err = chk_datacnt( req, sizeof(W) ) ) <= 0 ) {
		return ( err == 0 ) ? sizeof(W) : err;
	}
	*((W*)req->buf) = *param;
	return sizeof(W);
}

/*
 *  Set (W) parameter
 */
LOCAL	INT	set_param_w( T_DEVREQ *req, W *param )
{
	ER	err;

	if ( ( err = chk_datacnt( req, sizeof(W) ) ) <= 0 ) {
		return ( err == 0 ) ? sizeof(W) : err;
	}
	*param = *((W*)req->buf);
	return sizeof(W);
}

/*
 *  Read request processing
 */
LOCAL	ER	read_request( RsInfo *rsinf, T_DEVREQ *req )
{
	ER	err;

	if ( req->size < 0 ) return E_PAR;

	switch ( req->start ) {
	  case 0:
		err = receive_data( rsinf, req, &req->asize );
		break;

	  case DN_RSMODE:
		err = cntrl_port( rsinf,  req, -DN_RSMODE, sizeof(RsMode) );
		break;

	  case DN_RSFLOW:
		err = cntrl_port( rsinf, req, -DN_RSFLOW, sizeof(RsFlow) );
		break;

	  case DN_RSSTAT:
		err = cntrl_port( rsinf, req, -DN_RSSTAT, sizeof(RsStat) );
		break;

	  case DN_RSSNDTMO:
		err = get_param_w( req, &rsinf->snd_tmout );
		break;

	  case DN_RSRCVTMO:
		err = get_param_w( req, &rsinf->rcv_tmout );
		break;

	  case DN_RSBREAK:	/* Write-Only */
	  default:
		err = E_PAR;
	}
	if ( err < E_OK ) return err;

	req->asize = (W)err;
	return E_OK;
}

/*
 *  Write request processing
 */
LOCAL	ER	write_request( RsInfo *rsinf, T_DEVREQ *req )
{
	ER	err;

	if ( req->size < 0 ) return E_PAR;

	switch ( req->start ) {
	  case 0:
		err = send_data( rsinf, req, &req->asize );
		break;

	  case DN_RSMODE:
		err = cntrl_port( rsinf, req, DN_RSMODE, sizeof(RsMode) );
		break;

	  case DN_RSFLOW:
		err = cntrl_port( rsinf, req, DN_RSFLOW, sizeof(RsFlow) );
		break;

	  case DN_RSBREAK:
		err = send_break( rsinf, req );
		break;

	  case DN_RSSNDTMO:
		err = set_param_w( req, &rsinf->snd_tmout );
		break;

	  case DN_RSRCVTMO:
		err = set_param_w( req, &rsinf->rcv_tmout );
		break;

	  case DN_RSSTAT:	/* Read-Only */
	  default:
		err = E_PAR;
	}
	if ( err < E_OK ) return err;

	req->asize = (W)err;
	return E_OK;
}

/*
 *  Open request proceesing
 */
EXPORT	ER	rsOpenFn( ID devid, UINT omode, GDI gdi )
{
	ER	err;
	RsInfo	*rsinf = GDI_exinf(gdi);

	if ( rsinf->opened == FALSE ) {
		/* First time open, resume low level driver */
		err = serial_ctl( rsinf->port, RS_RESUME, NULL );
		if ( err < E_OK ) return err;

		rsinf->opened = TRUE;
	}
	return E_OK;
}

/*
 *  Close request processing
 */
EXPORT	ER	rsCloseFn( ID devid, UINT option, GDI gdi )
{
	ER	err;
	RsInfo	*rsinf = GDI_exinf(gdi);

	/* Suspend low level driver regardless of opened state
				in order to force suspended state */
	err = serial_ctl( rsinf->port, RS_SUSPEND, NULL );
	if ( err < E_OK ) return err;

	rsinf->opened = FALSE;
	return E_OK;
}

/*
 *  Process abort
 */
LOCAL	ER	do_abort( RsInfo *rsinf, T_DEVREQ *req )
{
	ER	err;

	if ( req == NULL ) {
		/* If any request is in process, abort it */
		if ( ! (rsinf->send_req != NULL || rsinf->recv_req != NULL) )
			return E_OK;
	} else {
		/* If the req is in process, abort it */
		if ( ! (rsinf->send_req == req || rsinf->recv_req == req) )
			return E_OK;
	}

	/* Abort low level driver */
	err = serial_ctl( rsinf->port, RS_ABORT, NULL );
	if ( err < E_OK ) return err;

	/* Wait for completion of abort */
	LockSND();
	UnlockSND();
	LockRCV();
	UnlockRCV();
	return E_OK;
}

/*
 *  Abort request processing
 */
EXPORT	ER	rsAbortFn( T_DEVREQ *req, GDI gdi )
{
	RsInfo	*rsinf = GDI_exinf(gdi);
	ER	err;

	LockACC();
	err = ( req == NULL ) ? E_OK : do_abort( rsinf, req );
	UnlockACC();

	return err;
}

/*
 *  Suspend request preocessing
 */
LOCAL	ER	do_suspend( RsInfo *rsinf )
{
	ER	er, err = E_OK;

	/* Set to suspended state */
	LockACC();

	/* Abort requests in processing */
	er = do_abort( rsinf, NULL );
	if ( er < E_OK ) err = er;

	/* Suspend low level driver regarless of opened state
					in order to force suspended state */
	er = serial_ctl( rsinf->port, RS_SUSPEND, NULL );
	if ( er < E_OK && err != E_NOMDA ) err = er;

	rsinf->suspended = TRUE;
	return err;
}

/*
 *  Resume request processing
 */
LOCAL	ER	do_resume( RsInfo *rsinf )
{
	ER	err = E_OK;

	/* Ignore when not suspended state */
	if ( rsinf->suspended == FALSE ) return E_OK;

	if ( rsinf->opened != FALSE ) {
		/* Resume low level driver */
		err = serial_ctl( rsinf->port, RS_RESUME, NULL );
		if ( err < E_OK ) {
			if ( err == E_NOMDA ) err = E_OK;
		}
	}
	rsinf->suspended = FALSE;

	/* Recover from suspended state */
	UnlockACC();
	return err;
}

/*
 *  Event request processing
 */
EXPORT	INT	rsEventFn(INT evttyp, void *evtinf, GDI gdi)
{
	RsInfo	*rsinf = GDI_exinf(gdi);

	switch (evttyp) {
	  case TDV_SUSPEND:
		return do_suspend( rsinf );
	  case TDV_RESUME:
		return do_resume( rsinf );
	}
	return E_PAR;
}

/*
 *  Accept device request task
 */
EXPORT	void	rsAcceptTask( RsInfo *rsinf )
{
	T_DEVREQ	*req;
	ER		err;

	for ( ;; ) {

		/* Wait for device request */
		err = GDI_Accept( &req, DRP_NORMREQ | DRP_USERCMD,
							TMO_FEVR, rsinf->gdi );
		if ( err < E_OK ) continue;

		if ( (err & (1 << USERCMD_FINISH)) != 0 ) {
			/* Abort request in processing */
			do_abort( rsinf, NULL );
			break;		/* Finsh */
		}

		/* Lock for device request */
		LockACC();

		if ( req->start >= 0 ) {
			if ( req->start != 0 || req->size < 0 ) {
				err = E_PAR;

			} else if ( req->cmd == TDC_WRITE && 
					rsinf->send_req == NULL ) {
				/* Forward request to rsSendTask */
				rsinf->send_req = req;
				tk_wup_tsk( rsinf->snd_tskid );

			} else if ( req->cmd == TDC_READ &&
					rsinf->recv_req == NULL ) {
				/* Forward request to rsRecvTask */
				rsinf->recv_req = req;
				tk_wup_tsk( rsinf->rcv_tskid );

			} else {
				err = E_BUSY;
			}

			UnlockACC();
			if ( err >= E_OK ) continue;
		} else {
			/* Abort request in processing */
			err = do_abort( rsinf, NULL );
			if ( err >= E_OK ) {
				/* Set task space to the requested task */
				err = tk_set_tsp( TSK_SELF, &req->tskspc );
				if ( err >= E_OK ) {
					/* Execute request */
					err = (req->cmd == TDC_WRITE) ?
						write_request( rsinf, req ) :
						read_request( rsinf, req );
				}
			}
			/* Unlock for device request */
			UnlockACC();
		}
		/* Reply to the request */
		req->error = err;
		GDI_Reply( req, rsinf->gdi );
	}

	/* Finish own task */
	rsinf->acc_tskid = 0;
	tk_exd_tsk();
}

/*
 *  Send data task
 */
EXPORT	void	rsSendTask( RsInfo *rsinf )
{
	T_DEVREQ	*req;
	ER		err;

	for ( ;; ) {

		/* Wait for receive request */
		err = tk_slp_tsk( TMO_FEVR );
		if ( err < E_OK ) continue;
		if ( rsinf->finish != 0 ) break;	/* Finish */

		/* Start processing of the request */
		req = rsinf->send_req;
		if ( req == NULL ) continue;

		/* Lock for send porcess */
		LockSND();

		/* Set task space to the requested task */
		err = tk_set_tsp( TSK_SELF, &req->tskspc );
		if ( err >= E_OK ) {
			/* Execute write request */
			err = write_request( rsinf, req );
		}

		/* Finish processing of the request */
		rsinf->send_req = NULL;

		/* Unlock for send porcess */
		UnlockSND();

		/* Reply to the request */
		req->error = err;
		GDI_Reply( req, rsinf->gdi );
	}

	/* Finish own task */
	rsinf->snd_tskid = 0;
	tk_exd_tsk();
}

/*
 *  Receive data task
 */
EXPORT	void	rsRecvTask( RsInfo *rsinf )
{
	T_DEVREQ	*req;
	ER		err;

	for ( ;; ) {

		/* Wait for receive request */
		err = tk_slp_tsk( TMO_FEVR );
		if ( err < E_OK ) continue;
		if ( rsinf->finish != 0 ) break;	/* Finish */

		/* Start processing of the request */
		req = rsinf->recv_req;
		if ( req == NULL ) continue;

		/* Lock for receive porcess */
		LockRCV();

		/* Set task space to the requested task */
		err = tk_set_tsp( TSK_SELF, &req->tskspc );
		if ( err >= E_OK ) {
			/* Execute read request */
			err = read_request( rsinf, req );
		}

		/* Finish processing of the request */
		rsinf->recv_req = NULL;

		/* Unlock for receive porcess */
		UnlockRCV();

		/* Reply to the request */
		req->error = err;
		GDI_Reply( req, rsinf->gdi );
	}

	/* Finish own task */
	rsinf->rcv_tskid = 0;
	tk_exd_tsk();
}
