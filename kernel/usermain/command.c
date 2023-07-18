/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for ASP-SH7750R
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/01/23.
 *
 *----------------------------------------------------------------------
 */

/*
 *	command.c
 *
 *	simple command processing
 */

#include <basic.h>
#include <libstr.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "../../config/tk_config_func.h"

#define	P			tm_printf
#define	Gets(buf, bufsz)	tm_getline((UB*)(buf))

#define	N_ARGS		16

/*
	ref command
*/
#include "ref_command.c"

/*
	call command
*/
LOCAL	void	cmd_call(INT ac, B *av[])
{
	FP	fnc;
	W	p1, p2, p3;

	if (ac < 2) return;

	fnc = (FP)strtol(av[1], NULL, 0);
	p1 = (ac >= 3) ? strtol(av[2], NULL, 0) : 0;
	p2 = (ac >= 4) ? strtol(av[3], NULL, 0) : 0;
	p3 = (ac >= 5) ? strtol(av[4], NULL, 0) : 0;

	(*fnc)(p1, p2, p3);
}

/*
	setup parameters
*/
LOCAL	INT	setup_param(B *bp, B **av)
{
	INT	ac;

	for (ac = 0; ac < N_ARGS; ac++) {
		while (*((UB*)bp) <= ' ' && *bp != '\0') bp++;
		if (*bp == '\0') break;
		av[ac] = bp;
		while (*((UB*)bp) > ' ') bp++;
		if (*bp != '\0') {
			*bp++ = '\0';
		}
	}
	av[ac] = NULL;
	return ac;
}

/*
	execute command
*/
EXPORT	INT	exec_cmd(B *cmd)
{
	INT	ac;
	B	*av[N_ARGS];

	ac = setup_param(cmd, av);
	if (ac < 1) return 0;

	if (strncmp(av[0], "ref", 3) == 0) {
		cmd_ref(ac, av);

	} else if (strncmp(av[0], "call", 4) == 0) {
		cmd_call(ac, av);

	} else if (strncmp(av[0], "?", 1) == 0) {
		P("ref  [item]\n");
		P("call addr [p1 p2 p3]\n");
	} else {
		return 0;
	}
	return 1;
}

/*
 *	Application main entry
 */
EXPORT	void	command( void )
{
	B	buf[256];
	INT	fin, n;

	/* command processing */
	for (fin = 0; fin == 0; ) {
		P("T2 >> ");
		Gets(buf, sizeof(buf));
		for (n = strlen(buf); --n >= 0 && buf[n] < ' ';) buf[n] = '\0';

		if (strncmp(buf, "quit", 1) == 0) {
			fin = 1;

		/* misc. command */
		} else {
			if (exec_cmd(buf) == 0) {
				P("q[uit]      quit\n");
				P("?           command help\n");
				P("<command>   misc. command\n");
			}
		}
	}
}

