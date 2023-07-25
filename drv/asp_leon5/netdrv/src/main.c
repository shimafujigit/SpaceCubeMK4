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
 *	@(#)main.c
 *
 */

#include	"netdrv.h"

/* Get "DEVCONF" entry */
IMPORT	W	GetDevConf(UB *name, W *val);

/*
 *	Network driver information
 */
EXPORT	NetInf	Netinf[MAX_NETDEV];
EXPORT	PRI	TskPri;

/*
 *	Driver definition table
 */
IMPORT	DrvDef	DrvTab[];

/*
 *	Network device name
 */
LOCAL	const	GDefDev	NETDRV_DDev = {
	NULL,		/* exinf */
	"Neta",		/* devnm */
	1,		/* maxreqq */
	0,		/* drvatr */
	0,		/* devatr */
	0,		/* nsub */
	1,		/* blksz */
	OpenProc,	/* open */
	CloseProc,	/* close */
	NULL,		/* abort */
	EventProc,	/* event */
};
#define	P_NETUNIT	3	/* Position of unit name 'a'		*/

/*
 *	Network device DEVCONF entry name
 *		NETDEV#  kind IO-base IRQ-number
 */
LOCAL	B	NetDevEnt[] = "NETDEV0";
#define	P_NETNO		6	/* Position of device number '0'	*/

/*
 *	Register a device
 */
LOCAL	ER	RegistDevice(NetInf *inf)
{
	ER	er;
	GDefDev	devDef;

	/* Registration info. */
	devDef = NETDRV_DDev;
	devDef.exinf = inf;
	devDef.devnm[P_NETUNIT] = inf->netno + 'a';

	/* Registration of device */
	if ((er = GDefDevice(&devDef, NULL, &inf->Gdi)) >= E_OK) {
		inf->devid = GDI_devid(inf->Gdi);
		return E_OK;
	}

DP(("Net%c: RegistDevice [%#x]\n", NetUnit(inf), er));

	inf->devid = INVALID_DEVID;
	return er;
}

/*
 *	Creates and starts a task
 */
EXPORT	ID	CreTask(FP entry, PRI pri, W name, UW par, W stksz)
{
	T_CTSK	ctsk;
	ID	tskid;
	ER	er;

	/* Creates a task */
	ctsk.exinf = (VP)name;
	ctsk.task = entry;
	ctsk.itskpri = pri;
	ctsk.stksz = stksz;
	ctsk.tskatr = TA_HLNG | TA_RNG0;

	tskid = er = tk_cre_tsk(&ctsk);
	if (er >= E_OK) {
		/* Starts a task */
		er = tk_sta_tsk(tskid, par);
		if (er < E_OK) tk_del_tsk(tskid);
	}
	return (er >= E_OK) ? tskid : er;
}

/*
 *	Task for request acceptance (Normal & special requests)
 */
LOCAL	void	MainTask(NetInf *inf)
{
	ER	er;

	/* Register a device */
	er = RegistDevice(inf);
	if (er < E_OK) goto EEXIT1;

	inf->exist = TRUE;		/* is existed */

	/* Register an interrupt handler */
	if (inf->di.stat >= E_OK) {
		if (DefIntHdr(inf, TRUE) < E_OK) {
			inf->di.stat = E_LIMIT;
		} else if (isFORCE_OPEN(inf)) {	/* Force open */
			OpenProc(inf->devid, 0, inf->Gdi);
		}
	}

	/* Start acceptance of device request */
	AcceptRequest(inf);

	/* Never come hear */
EEXIT1:
	if (inf->initfn != NULL) (*(inf->initfn))(inf, -1);

DP(("Net%c: MainTask [%#x]\n", NetUnit(inf), er));

	tk_exd_tsk();
}

/*
 *	Network driver main
 */
EXPORT	ER	NetDrv(INT ac, B *av[])
{
	W	i, n, k, kk;
	VP	tskname;
	W	v[L_DEVCONF_VAL];
	NetInf	*inf;
	B	*arg;
static	W	ndrv = 0;

	if (ac < 0) {		/* Finish */
		/* Do minimum processing for shutdown.
		   Hardware interrupts should be disabled, because when the
		   interrupts are shared, a trouble may occur at next startup.
		*/
		for (inf = &Netinf[0]; ndrv > 0; ndrv--, inf++) {
			inf->opencnt = 0;
			CloseProc(inf->devid, 0, inf->Gdi);
			if (inf->di.stat >= E_OK) DefIntHdr(inf, FALSE);
			tk_ter_tsk(inf->tskid);
			GDelDevice(inf->Gdi);
			if (inf->initfn != NULL) (*(inf->initfn))(inf, -1);
		}
		return E_OK;
	}

	TskPri = DEFAULT_PRI;
	SetOBJNAME(tskname, "netd");

	/* get parameters */
	memset(v, 0, sizeof(v));
	if (ac > 1 && (arg = av[1]) != NULL) {
		switch (*arg++) {
		case '!':		/* priority */
			TskPri = strtol(arg, (VP)&arg, 0);
			break;
		case '=':
			v[0] = strtol(arg, (VP)&arg, 0);
			if (*arg++ == '.') {
				v[1] = strtol(arg, (VP)&arg, 0);
				if (*arg++ == '.') {
					v[2] = strtol(arg, (VP)&arg, 0);
				}
			}
			break;
		default:
			return E_PAR;
		}
	}

	/* Clear network device information */
	memset(Netinf, 0, sizeof(Netinf));
	for (i = 0; i < MAX_NETDEV; i++) Netinf[i].pciadr = -1;

	/* Create network driver task */
	for (ndrv = i = 0; i < MAX_NETDEV; i++) {

		/* get setting on DEVCONF */
		NetDevEnt[P_NETNO] = i + '0';
		if (i != 0 || v[0] == 0) {
			memset(v, 0, sizeof(v));
			if (GetDevConf(NetDevEnt, v) <= 0 || v[0] <= 0)
					continue;
		}
		/* initialize driver information area */
		inf = &Netinf[i];
		inf->netno = i;			/* Network number	*/
		inf->di.kind = v[0];		/* Hardware kind	*/
		inf->di.iobase =
			inf->orgbase = v[1];	/* I/O Base		*/
		inf->di.intno = v[2];		/* IRQ Number		*/
		inf->bufsz.minsz = MINPKTLEN;	/* Receive buffer size	*/
		inf->bufsz.maxsz = MAXPKTLEN;

		/* Check hardware */
		if ((kk = v[0] & HK_MASK) == HK_PCI) {	/* PCI auto detect */
			for (n = 0; (k = DrvTab[n].kind) != 0; n++) {
				if ((k & HK_PCI) != 0 && PciProbe(inf,
					DrvTab[n].pcitab) >= E_OK) break;
			}
		} else {			/* Fixed setting */
			for (n = 0; (k = DrvTab[n].kind) != 0 && k != kk; n++);
			if ((k & HK_PCI) != 0 &&
				PciProbe(inf, DrvTab[n].pcitab) < E_OK) k = 0;
		}
		if (k != 0 && (*(DrvTab[n].initfn))(inf, 0) >= E_OK) {
			inf->initfn = DrvTab[n].initfn;

			/* Create & start driver task */
			if ((inf->tskid = CreTask((FP)MainTask, TskPri,
				(W)tskname, (UW)inf, TASK_STKSZ)) >= E_OK) {
				ndrv++;		/* OK */
				continue;
			}

			(*(inf->initfn))(inf, -1);
			inf->initfn = NULL;
		}
	}
	return (ndrv > 0) ? E_OK : E_NOMDA;
}

