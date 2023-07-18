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
 *	rs_main.c	RS driver main
 */

#include "rsdrv.h"
#include <libstr.h>

/*
 *  RS driver information
 */
#ifndef	MAX_RSPORT
#define	MAX_RSPORT	1
#endif
LOCAL	RsInfo	rs_info[MAX_RSPORT];

/*
 *  Registration or deletion of a device driver
 */
LOCAL	INT	regist_device( RsInfo *rsinf, BOOL reg )
{
static	const	GDefDev	def = {
		NULL,		/* exinf */
		"rsa",		/* devnm */
		3,		/* maxreqq */
		0,		/* drvatr */
		0,		/* devatr */
		0,		/* nsub */
		1,		/* blksz */
		rsOpenFn,	/* open */
		rsCloseFn,	/* close */
		rsAbortFn,	/* abort */
		rsEventFn,	/* event */
	};
	GDefDev		devdef;

	devdef = def;
	devdef.exinf = rsinf;
	devdef.devnm[2] += rsinf->port;	/* 0:rsa, 1:rsb, 2:rsc ... */

	return ( reg ) ?	GDefDevice( &devdef, NULL, &rsinf->gdi ) :
				GDelDevice( rsinf->gdi );
}

/*
 *  Create and start a task
 */
LOCAL	ER	crst_task( void *tsk, PRI pri, void *par )
{
	T_CTSK	ctsk;
	ID	tskid;
	ER	err;

	/* Create a task */
	ctsk.exinf   = par;
	ctsk.task    = tsk;
	ctsk.itskpri = pri;
	ctsk.stksz   = DEF_STACKSZ;
	ctsk.tskatr  = TA_HLNG | TA_RNG0;

	tskid = err = tk_cre_tsk( &ctsk );
	if ( err >= E_OK ) {
		/* Start a task */
		err = tk_sta_tsk( tskid, (INT)par );
		if ( err >= E_OK ) {
			return tskid;
		}
		tk_del_tsk( tskid );
	}
	return err;
}

/*
 *  Start RS driver
 */
LOCAL	INT	start_driver( W port, PRI pri )
{
static	UB	nm[] = "rsa";
	ER	err;
	RsInfo	*rsinf;

	if (port >= MAX_RSPORT) return E_LIMIT;

	/* Initialize RS driver information */
	rsinf = &rs_info[port];
	memset( rsinf, 0, sizeof(RsInfo) );

	rsinf->port = port;
	rsinf->snd_tmout = TMO_FEVR;
	rsinf->rcv_tmout = TMO_FEVR;

	/* Create exclusive lock */
	nm[2] = 'a' + port;	/* nm = "rsa", "rsb" ... */
	err = CreateMLock( &rsinf->mlock, nm );
	if ( err < E_OK ) goto EEXIT0;

	/* Regist device driver */
	if ( regist_device( rsinf, TRUE ) < E_OK ) goto EEXIT1;

	/* Creat and start send data task */
	rsinf->snd_tskid = err = crst_task( rsSendTask, pri, rsinf );
	if ( err < E_OK ) goto EEXIT2;

	/* Creat and start recv data task */
	rsinf->rcv_tskid = err = crst_task( rsRecvTask, pri, rsinf );
	if ( err < E_OK ) goto EEXIT3;

	/* Creat and start accept request task */
	rsinf->acc_tskid = err = crst_task( rsAcceptTask, pri, rsinf );
	if ( err < E_OK ) goto EEXIT4;

	return E_OK;

EEXIT4:
	tk_ter_tsk( rsinf->rcv_tskid );
	tk_del_tsk( rsinf->rcv_tskid );
EEXIT3:
	tk_ter_tsk( rsinf->snd_tskid );
	tk_del_tsk( rsinf->snd_tskid );
EEXIT2:
	regist_device( rsinf, FALSE );
EEXIT1:
	DeleteMLock( &rsinf->mlock );
EEXIT0:
	memset( rsinf, 0, sizeof(RsInfo) );
	return err;
}

/*
 *  Entry of RS driver
 */
EXPORT	ER	RsDrv( INT ac, UB *av[] )
{
	INT	i;
	PRI	tpri;

	if ( ac < 0 ) {
		/* Shutdown RS driver */
		for ( i = 0; i < MAX_RSPORT; i++ ) {
			RsInfo	*rsinf = &rs_info[i];

			if ( rsinf->acc_tskid <= 0 ) continue;

			/* Finish accept task */
			rsinf->finish = 1;
			GDI_SendCmd( USERCMD_FINISH, rsinf->gdi );
			while ( rsinf->acc_tskid > 0 ) tk_dly_tsk( 10 );

			/* Finish send and receive tasks */
			tk_wup_tsk( rsinf->snd_tskid );
			tk_wup_tsk( rsinf->rcv_tskid );
			while ( rsinf->snd_tskid > 0 ||
				rsinf->rcv_tskid > 0 ) tk_dly_tsk( 10 );

			/* Unregist device */
			regist_device( rsinf, FALSE );

			/* Delete Lock */
			DeleteMLock( &rsinf->mlock );
		}
	} else {
		/* Startup RS driver */

		/* Get parameter */
		tpri = DEF_TSKPRI;
		for ( i = 1; i < ac; i++ ) {
			char	*arg;
			arg = (char *)av[i];
			switch ( *arg++ ) {
			case '!':	/* Task priority */
				tpri = strtol( arg, &arg, 0 );
				break;
			default:
				return E_PAR;
			}
		}

		/* Start RS driver for each existing serial port */
		for ( i = 0; i < MAX_RSPORT; i++ ) {
			RsStat	st;
			if ( serial_ctl( i, -DN_RSSTAT, (UW*)&st ) >= E_OK ) {
				start_driver( i, tpri );
			}
		}
	}
	return E_OK;
}

