/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for ASP-LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/11/5.
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
#include <stdlib.h>

#include "config/tk_config_func.h"

#include "userconfig.h"

#if	USE_MYPRINTF
#include "myprintf.c_inc"
#define	P			printf
#define	Gets(buf, bufsz)	my_getline(buf, bufsz)
#else
#define	P			tm_printf
#define	Gets(buf, bufsz)	tm_getline((UB*)(buf))
#endif

#define	N_ARGS		16

/*
	test programs
*/
#if	USE_TEST_TASK || USE_TEST_SERIAL
#include "test_prog.c_inc"
#endif

/*
	ref command
*/
#include "ref_command.c_inc"

/*
	call command
*/
LOCAL	void	cmd_call(INT ac, char *av[])
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
LOCAL	INT	setup_param(char *bp, char **av)
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
EXPORT	INT	exec_cmd(char *cmd)
{
	INT	ac;
	char	*av[N_ARGS];

	ac = setup_param(cmd, av);
	if (ac < 1) return 0;

	if (strncmp(av[0], "ref", 3) == 0) {
		cmd_ref(ac, av);

	} else if (strncmp(av[0], "call", 4) == 0) {
		cmd_call(ac, av);

#if	1
	} else if (strncmp(av[0], "dly", 4) == 0) {
		// for time accuracy check
		INT	n = strtol(av[1], NULL, 0);
		P("dly %d sec ...", n);
		tk_dly_tsk(n * 1000);
		P(" end\n");

	} else if (strncmp(av[0], "malloc", 6) == 0) {
		// for memory allocation check
		INT	n = strtol(av[1], NULL, 0);
		P("malloc(%x) --> %x\n", n, Kmalloc(n));

	} else if (strncmp(av[0], "free", 4) == 0) {
		// for memory free check
		void	*p = (void*)strtoul(av[1], NULL, 0);
		P("free(%x)\n", p);
		Kfree(p);
#endif

#if	USE_TEST_TASK
	} else if (strncmp(av[0], "test-task", 9) == 0) {
		test_task();
#endif

#if	USE_TEST_SERIAL
	} else if (strncmp(av[0], "test-serial", 9) == 0) {
		test_serial();
#endif
	} else if (strncmp(av[0], "?", 1) == 0) {
		P("ref  [item]\n");
		P("call addr [p1 p2 p3]\n");
#if	USE_TEST_TASK
		P("test-task\n");
#endif
#if	USE_TEST_SERIAL
		P("test-serial\n");
#endif
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
	char	buf[128];
	INT	fin, n;

	/* command processing */
	for (fin = 0; fin == 0; ) {
		P("T2 >> ");
		Gets(buf, sizeof(buf));
		for (n = strlen(buf); --n >= 0 && buf[n] < ' ';) buf[n] = '\0';

		if (strncmp(buf, "quit", 1) == 0) {
			fin = 1;

		} else if ( buf[0] == '#' ) {		/* tmonitor command */
			tm_command((UB*)&buf[1]);

		} else {				/* misc. command */
			if (exec_cmd(buf) == 0) {
				P("q[uit]      quit\n");
				P("# [cmd]     exec t-monitor command\n");
				P("?           command help\n");
				P("<command>   misc. command\n");
			}
		}
	}
}

