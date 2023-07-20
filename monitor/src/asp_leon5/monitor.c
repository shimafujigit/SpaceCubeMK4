/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/10/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	monitor.c
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include <sys/sysinfo.h>
#include <sys/rominfo.h>
#include <tm/tmonitor.h>

#define	START_MSG	"** ASP-LEON5 - Monitor **\n"

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

/* sio.c */
IMPORT	void	init_sio( W baud );
IMPORT	void	put_sio( B ch );
IMPORT	W	get_sio( W tmo );

/* sysdepend.c */
IMPORT	void	init_hw( void );
IMPORT	void	system_halt( void );
IMPORT	INT	ctrl_led( UW val );

/* command.c */
IMPORT	void	command_proc( B *cmd );

/* Debug console baud rate */
LOCAL	W	BAUD_RATE;

#define	DEF_BAUD	19200

#define	CR	0x0d
#define	LF	0x0a

/*
 *	Put 1 character to console
 */
EXPORT	W	put_char( W c )
{
	if ( c == LF || c == CR ) {
		put_sio(CR);
		c = LF;
	}
	put_sio(c);
	return 0;
}

/*
 *	Put string to console
 */
EXPORT	W	put_string( B *str )
{
	B	c;

	while ( (c = *str++) != '\0' ) {
		if ( put_char(c) < 0 ) return -1;
	}
	return 0;
}

/*
 *	Put hexadecimal digits to console
 */
EXPORT	void	put_hex2( UW v )
{
static	const	B	HexC[] = "0123456789ABCDEF";

	put_char(HexC[(v >> 4) & 0x0F]);
	put_char(HexC[v & 0x0F]);
}
EXPORT	void	put_hex4( UW v )
{
	put_hex2(v >> 8);
	put_hex2(v);
}
EXPORT	void	put_hex8( UW v )
{
	put_hex4(v >> 16);
	put_hex4(v);
}
EXPORT	void	put_hexval( B *str, UW v )
{
	put_string(str);
	put_hex8(v);
}

/*
 *	Get 1 character from console
 */
EXPORT	W	get_char( W tmo )
{
	return get_sio(tmo);
}

/*
 *	Get string from console
 */
EXPORT	W	get_string( B *str )
{
	W	c;
	B	*p;

	for ( p = str; ; ) {
		if ( (c = get_sio(10)) <= 0 ) continue;
		if ( c == CR || c == LF ) break;	/* end of line */
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
EXPORT	W	ent_monitor( W fn_code, W *par )
{
	W	n = 0;

	switch ( fn_code ) {
	case TM_DEFAULT_HDR:		/* Internal */
		put_hexval("\n** Undefined EIT ", par[3]);
		put_hexval(" pc=", par[1]);
		put_hexval(" npc=", par[2]);
		put_hexval(" psr=", par[0]);
		put_char(LF);
		command_proc(NULL);
		system_halt();		/* No return */
		break;

	case TM_MONITOR:
		put_string("-- tm_monitor() --\n");
		command_proc(NULL);
		break;

	case TM_GETCHAR:
		n = get_char(*par);
		break;

	case TM_PUTCHAR:
		put_char(*par);
		break;

	case TM_GETLINE:
		n = get_string((B*)*par);
		break;

	case TM_PUTSTRING:
		put_string((B*)*par);
		break;

	case TM_EXIT:
		put_string("-- tm_exit() --\n");
		system_halt();		/* No return */
		break;

	case TM_EXTSVC:
		n = -1;
		switch(*par) {
		case TMEF_PORTBPS:
			n = BAUD_RATE;
			break;

		case TMEF_LED:
			n = ctrl_led(par[1]);
			break;
		}
		break;

	case TM_COMMAND:
		command_proc((B*)*par);
		break;

	case TM_READDISK:
	case TM_WRITEDISK:
	case TM_INFODISK:
	default:
		n = -1;
	}
	return n;
}

/*
 *	Reset processing
 */
EXPORT	UW	reset_proc( void )
{
	extern VP lowmem_top, lowmem_end; /* bccmain.c */
	FP	fp;

	/* Initialize Hardware */
	init_hw();

	/* Initialize SYSINFO */
	SCInfo.ramtop = lowmem_top;
	SCInfo.ramend = lowmem_end;
	if ( ROMInfo->userarea > SCInfo.ramtop &&
	     ROMInfo->userarea < SCInfo.ramend ) {
		SCInfo.ramend = ROMInfo->userarea;
	}

	/* Initialize serial I/O */
	BAUD_RATE = ( ROMInfo->dbgport_bps != 0 ) ?
					ROMInfo->dbgport_bps : DEF_BAUD;
	init_sio(BAUD_RATE);

	/* Execute user reset process */
	fp = ROMInfo->resetinit;
	if ( fp != NULL ) (*fp)();

	/* Start messsage */
	put_string(START_MSG);

	/* Return Kernel entry */
	if ( ROMInfo->kernel != NULL ) {
		return (UW)ROMInfo->kernel;
	}
	/* Command */
	command_proc(NULL);

	return 0;
}
