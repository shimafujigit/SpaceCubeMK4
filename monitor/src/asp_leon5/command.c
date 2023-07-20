/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit
 *
 *    Copyright (c) 2014 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2014/04/18.
 *
 *----------------------------------------------------------------------
 */

/*
 *	command.c
 *
 *	 command processing
 */

#include <basic.h>

/* monitor.c */
IMPORT	W	put_char( W c );
IMPORT	W	put_string( B *str );
IMPORT	W	get_char( W tmo );
IMPORT	W	get_string( B *buf, W buflen );
IMPORT	void	put_hex2( UW v );
IMPORT	void	put_hex4( UW v );
IMPORT	void	put_hex8( UW v );

/* sysdepend.c */
IMPORT	void	system_halt( void );

#define	L_LINE	40			/* length of one input line */
LOCAL	UB	line_buf[L_LINE];	/* Line input buffer */
LOCAL	UB	*lptr;			/* line pointer */
LOCAL	UB	m_unit;			/* Memory unit */
LOCAL	UW	m_addr;			/* Memory address */

#define	PROMPT	"TM> "			/* prompt */

/*
 *	Console display functions
 */
#define DSP_S(s)	put_string(s)
#define DSP_LF		put_char('\n')
#define DSP_C(c)	put_char(c)
#define DSP_X2(x)	put_hex2(x)
#define DSP_X4(x)	put_hex4(x)
#define DSP_X8(x)	put_hex8(x)

/*
 *	Skip spaces
 */
LOCAL	void	skip_space( void )
{
	while ( *lptr != '\0' && *lptr <= ' ' ) lptr++;
}

/*
 *	Get hexadecimal value
 */
LOCAL	void	get_line( B *str )
{
	DSP_S(str);
	get_string((B*)(lptr = line_buf), sizeof(line_buf));
}

/*
 *	Get hexadecimal value
 */
LOCAL	INT	get_hexval( UW *val )
{
	UB	c, *p;
	UW	v;

	skip_space();
	p = lptr;
	for ( v = 0; ( (c = *lptr) >= '0' && c <= '9' ) ||
			( c >= 'A' && c <= 'F' ) ||
				( c >= 'a' && c <= 'f' ); lptr++ ) {
		//if ( v >= 0x10000000 ) break;
		v <<= 4;
		v += ( c >= 'a' ? (c - 'a' + 10) :
			( c >= 'A' ? (c - 'A' + 10) : ( c - '0') ) );
	}
	if ( p == lptr ) return 0;
	*val = v;
	return 1;
}

/*
 *	Get memory address
 */
LOCAL	void	get_m_addr( void )
{
	get_hexval(&m_addr);
	m_addr &= ~(m_unit - 1);
}

/*
 *	Display memory content
 */
LOCAL	void	dsp_memory( void )
{
	switch( m_unit ) {
	case 4:	DSP_X8(*((_UW*)m_addr));	break;
	case 2:	DSP_X4(*((_UH*)m_addr));	break;
	//case 1:
	default:	DSP_X2(*((_UB*)m_addr));
	}
}

/*
 *	Set memory content
 */
LOCAL	void	put_memory( UW v )
{
	switch( m_unit ) {
	case 4:	*((_UW*)m_addr) = (UW)v;	break;
	case 2:	*((_UH*)m_addr) = (UH)v;	break;
	//case 1:
	default:	*((_UB*)m_addr) = (UB)v;
	}
}

/*
 *	Display memory address
 */
LOCAL	void	dsp_m_addr( void )
{
	DSP_X8(m_addr);
	DSP_S(": ");
}

/*
 *	Get memory unit
 */
LOCAL	void	get_m_unit( void )
{
	INT	v;

	switch( *lptr++ ) {
	case 'w':	v = 4;	break;
	case 'h':	v = 2;	break;
	default:	lptr--;
	case 'b':	v = 1;	break;
	}
	m_unit = v;
}

/*
 *	Dump memroy command
 *
 *	d{b|h|w} [start_addr[ data_cnt]]
 */
LOCAL	void	cmd_dump( void )
{
	W	i;
	UW	len = 0;

	get_m_unit();
	get_m_addr();
	get_hexval(&len);
	len *= m_unit;

	for ( i = 0; ; ) {
		if ( (i % 16) == 0 ) {
			if ( i != 0 ) DSP_LF;
			dsp_m_addr();
		} else {
			DSP_C(' ');
		}
		dsp_memory();
		m_addr += m_unit;
		if ( (i += m_unit) >= len ) break;
	}
	DSP_LF;
}

/*
 *	Modify memory command
 *
 *	m{b|h|w} [start_addr[ data]]
 */
LOCAL	void	cmd_modify( void )
{
	UW	dt;

	get_m_unit();
	get_m_addr();

	if ( get_hexval(&dt) == 0 ) {
		for ( ;; ) {
			dsp_m_addr();
			dsp_memory();
			get_line(" -> ");
			if ( get_hexval(&dt) > 0 ) {
				put_memory(dt);
			} else if ( *lptr == '^' ) {
				m_addr -= m_unit;
				continue;
			} else if ( *lptr == '.' || *lptr == '/' ) {
				break;
			}
			m_addr += m_unit;
		}
	} else {
		put_memory(dt);
	}
}

/*
 *	Command interpreter
 */
EXPORT	void	command_proc( B *cmd )
{
	for ( ;; ) {
		if ( cmd != NULL ) {
			lptr = (UB*)cmd;
		} else {
			get_line(PROMPT);
		}
		skip_space();
		switch( *lptr++ ) {
		case 'd':	cmd_dump();	break;
		case 'm':	cmd_modify();	break;
		case 'g':
			{	UW	adr;
				if ( get_hexval(&adr) > 0 && adr != 0 ) {
					(*((FP)adr))();
				}
			}
			return;

		case 'e':	system_halt();	/* No return */
		case '?':	put_string(
					"d{b|h|w} [adr[ cnt]] - dump\n"
					"m{b|h|w} [adr[ dat]] - modify\n"
					"g [adr] - go/return\n"
					"e - halt\n");		break;
		case '\0':	cmd = NULL;	break;
		}
		if ( cmd != NULL ) break;
	}
}
