/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/01/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	monitor.c
 */

#include <basic.h>
#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include <sys/rominfo.h>
#include <tm/tmonitor.h>
#include "iodefine.h"

/* T-Monitor SVC func code */
#define	TM_MONITOR	0
#define	TM_GETCHAR	1
#define	TM_PUTCHAR	2
#define	TM_GETLINE	3
#define	TM_PUTSTRING	4
#define	TM_COMMAND	5
#define	TM_READDISK	6
#define	TM_WRITEDISK	7
#define	TM_INFODISK	8
#define	TM_EXIT		9

#define	TM_EXTSVC	255
#define	TM_DEFAULT_HDR	127	/* Internal use */

/* System clock definitions */
#define	I_CLK		60	/* CPU clock (MHz) */
#define	P_CLK		30	/* Peripheral clock (MHz) */

/* Debug console baud rate */
#define	BAUD_RATE	115200	//19200

/* SVC vector for T-Monitor SVC */
#define	TMON_VEC	0x70

/* Default Handler vector */
#define	DEFH_VEC	0x80

/* Memory area definitions */
#define	RAM_TOP		0x84002000
#define	RAM_END		0x88000000

#define	CR		0x0d
#define	LF		0x0a

IMPORT	void	reset_hdr(void);
IMPORT	void	trapa_hdr(void);
IMPORT	void	default_hdr(void);
IMPORT	void	monitor_svc(void);
IMPORT	UW	get_spc(void);

IMPORT	void	init_sio(W baud);
IMPORT	void	put_sio(B ch);
IMPORT	W	get_sio(W tmo);


/*
 *	Put 1 character to console
 */
LOCAL	W	put_char(W c)
{
	if (c == LF || c == CR) {
		put_sio(CR);
		c = LF;
	}
	put_sio(c);
	return 0;
}

/*
 *	Put string to console
 */
LOCAL	W	put_string(B *str)
{
	B	c;

	while ((c = *str++) != '\0') {
		if (put_char(c) < 0) return -1;
	}
	return 0;
}

LOCAL	void	put_hex(B *str, UW v)
{
	INT	i, n;

	if (str != NULL) put_string(str);

	for (i = 0; i < 8; i++) {
		n = (v >> (28 - 4 * i)) & 0x0F;
		put_char((n < 10) ? ('0' + n) : ('A' + n - 10));
	}
}

/*
 *	Get 1 character from console
 */
LOCAL	W	get_char(W tmo)
{
	return get_sio(tmo);
}

/*
 *	Get string from console
 */
LOCAL	W	get_string(B *str)
{
	W	c;
	B	*p;

	for (p = str; ; ) {
		if ((c = get_sio(10)) <= 0) continue;
		if (c == CR || c == LF) break;
		*p++ = c;
		put_char(c);	/* echo back */
	}
	put_char(CR);		/* echo back */
	*p = '\0';
	return p - str;
}

/*
 *	T-Monitor Entry - SVC function
 */
EXPORT	W	ent_monitor(W fn_code, W *par)
{
	W	ret = 0;
	W	vec, kind;
	B	*bp;

	switch (fn_code) {
	case TM_DEFAULT_HDR:
		vec = (UW)par[0];
		kind = vec % 4;
		vec /= 4;
		switch(kind) {
		case 1:
			put_hex("** Undefined TRAPA ", vec);
			break;
		case 2:
			put_hex("** Undefined Interrupt INTEVT: ", vec << 5);
			break;
		default:
			switch(vec) {
			case 0x02: bp = "TLB miss/invalid(read)";	break;
			case 0x03: bp = "TLB miss/invalid(write)";	break;
			case 0x04: bp = "Initial page write";		break;
			case 0x05: bp = "TLB protection(read)";	break;
			case 0x06: bp = "TLB protection(write)";	break;
			case 0x07: bp = "CPU address error(read)";	break;
			case 0x08: bp = "CPU address error(write)";	break;
			case 0x09: bp = "FPU exception";		break;
			case 0x0C: bp = "Invalid instruction";		break;
			case 0x0D: bp = "Invalid instruction(slot)";	break;
			case 0x0E: bp = "NMI interrupt";		break;
			default:   bp = " ";
			}
			put_hex("\n** Undefined Exception EXPEVT: ", vec << 5);
			put_string("\n-- ");
			put_string(bp);
			if (vec <= 0x08) put_hex(" TEA: ", (UW)MMU.TEA);
		}
		put_hex(" @ PC: ", (UW)get_spc());
		put_char(LF);
		while(1);
		break;

	case TM_MONITOR:
		put_string("-- tm_monitor() --\n");
		while(1);
		break;

	case TM_GETCHAR:
		ret = get_char(*par);
		break;

	case TM_PUTCHAR:
		put_char(*par);
		break;

	case TM_GETLINE:
		ret = get_string((B*)*par);
		break;

	case TM_PUTSTRING:
		put_string((B*)*par);
		break;

	case TM_EXIT:
		put_string("-- tm_exit() --\n");
		while(1);
		break;

	case TM_EXTSVC:
		switch(*par) {
		case TMEF_PORTBPS:
			ret = BAUD_RATE;
			break;
		default:
			ret = -1;
		}
		break;
	case TM_COMMAND:
	case TM_READDISK:
	case TM_WRITEDISK:
	case TM_INFODISK:
	default:
		ret = -1;
		break;
	}
	return ret;
}

/*
 *	Reset processing
 */
EXPORT	UW	reset_proc(void)
{
	UW	*p, n;
	FP	fp;

	/* Initialize FEMI */
	FEMI.MDCR.BIT.HIZMEM = 1;
	FEMI.A0MCR.LONG = 0x00271110;
	FEMI.A1MCR.LONG = 0x00271110;
	FEMI.A2MCR.LONG = 0x00271110;
	FEMI.A3MCR.LONG = 0x00271110;
	FEMI.A4MCR.LONG = 0x00271110;

	/* Initialize INTC */
	INTC1.ICR.WORD = 0;
	INTC1.IPRA.WORD = 0;
	INTC1.IPRB.WORD = 0;
	INTC1.IPRC.WORD = 0;
	INTC1.IPRD.WORD = 0;

	/* Initialize SYSINFO */
	for (p = (UW*)SCArea; p < (UW*)SCArea->intstack;) *p++ = 0;

	SCInfo.Iclk = I_CLK;
	SCInfo.Pclk = P_CLK;

	SCInfo.ramtop = (void *)RAM_TOP;
	n = (UW)ROMInfo->userarea;
	if (n < RAM_TOP || n > RAM_END) n = RAM_END;
	SCInfo.ramend = (void *)n;

	/* Setup EIT vector */
	SCArea->intvec[0x00] = (FP)default_hdr;		/* TRAPA 0x00 */
	SCArea->intvec[0x01] = (FP)default_hdr;		/* TRAPA 0x01 */
	SCArea->intvec[0x0B] = (FP)trapa_hdr;		/* TRAPA */
	SCArea->intvec[0x0E] = (FP)default_hdr;		/* NMI */
	SCArea->intvec[0x0F] = (FP)default_hdr;		/* UBC */
	SCArea->intvec[TMON_VEC] = (FP)monitor_svc;	/* TM SVC */
	SCArea->intvec[DEFH_VEC] = (FP)default_hdr;

	/* Initialize serial I/O */
	init_sio(BAUD_RATE);

	/* Execute user reset process */
	fp = ROMInfo->resetinit;
	if (fp != NULL) (*fp)();

	/* Start messsage */
	put_string("** ASP-SH7750R - Monitor **\n");

	/* Return Kernel entry */
	return (UW)ROMInfo->kernel;
}

