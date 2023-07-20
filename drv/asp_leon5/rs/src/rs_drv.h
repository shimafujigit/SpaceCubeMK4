/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for ASP-LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/11/1.
 *
 *----------------------------------------------------------------------
 */

/*
 *	rs_drv.h	RS driver header
 */

#include <tk/tkernel.h>
#include <tk/util.h>
#include <device/serialio.h>
#include <device/rs.h>
#include <device/gdrvif.h>
#include <sys/util.h>
#include <sys/consio.h>

/*
 *  RS driver information
 */
typedef struct {
	W		port;		/* Serial port number */
	GDI		gdi;		/* General driver I/F handle */
	FastMLock	mlock;		/* Exclusive lock */

	BOOL		opened:1;	/* Opened status */
	BOOL		suspended:1;	/* Suspened status */
	BOOL		finish:1;	/* Finish request */

	T_DEVREQ	*send_req;	/* Send request in process */
	T_DEVREQ	*recv_req;	/* Receive request in process */

	TMO		snd_tmout;	/* Send time out (ms) */
	TMO		rcv_tmout;	/* Receive time out (ms) */

	ID		snd_tskid;	/* Send task id */
	ID		rcv_tskid;	/* Receive task id */
	ID		acc_tskid;	/* Accept task id */
} RsInfo;

/*
 *  Exclusive locks
 */
#define	LockACC()	MLock  ( &(rsinf->mlock), 0 )
#define	UnlockACC()	MUnlock( &(rsinf->mlock), 0 )

#define	LockSND()	MLock  ( &(rsinf->mlock), 1 )
#define	UnlockSND()	MUnlock( &(rsinf->mlock), 1 )

#define	LockRCV()	MLock  ( &(rsinf->mlock), 2 )
#define	UnlockRCV()	MUnlock( &(rsinf->mlock), 2 )

/*
 *  Default Task priority
 */
#define	DEF_TSKPRI		25

/*
 *  Default task stack size
 */
#define	DEF_STACKSZ		2048

/*
 *  UserCommand for finish
 */
#define	USERCMD_FINISH		16

/*
 *  External functions : accept.c
 */
IMPORT	ER	rsOpenFn( ID devid, UINT omode, GDI gdi );
IMPORT	ER	rsCloseFn( ID devid, UINT option, GDI gdi );
IMPORT	ER	rsAbortFn( T_DEVREQ *devreq, GDI gdi );
IMPORT	INT	rsEventFn( INT evttyp, void *evtinf, GDI gdi );
IMPORT	void	rsAcceptTask( RsInfo *rsinf );
IMPORT	void	rsSendTask( RsInfo *rsinf );
IMPORT	void	rsRecvTask( RsInfo *rsinf );

