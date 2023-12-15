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
#include "spcmk4_spi.h"

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

#define BUFSIZE_SPI		(128)

EXPORT	void	command( void )
{
	_UW		res = 0;
	char	buf[128];
	char 	*send_data;		/* Send data buffer */
	char 	*rcv_data;		/* Recive data buffer */
	INT 	send_len = 0;	/* Data length */
	INT		wk_len = 0;
	INT		fin, n, i;

	res = (_UW)Kmalloc(BUFSIZE_SPI+16);
	if(res != 0)
	{
		if(res & 0x0000000F)
			send_data = (res + 0x10) & 0xFFFFFFF0;
		else
			send_data = res;
	}
	else
	{
		P("Faild malloc send_data\\r\n");
		return;
	}

	res = (_UW)Kmalloc(BUFSIZE_SPI+16);
	if(res != 0)
	{
		if(res & 0x0000000F)
			rcv_data = (res + 0x10) & 0xFFFFFFF0;
		else
			rcv_data = res;
	}
	else
	{
		P("Faild malloc rcv_data\\r\n");
		return;
	}


	for(i=0; i<(BUFSIZE_SPI+16); i++)
	{
		send_data[i] = 0;
		rcv_data[i] = 0;
	}


	/* command processing */
	for (fin = 0; fin == 0; ) {
		P("Input >> ");
		Gets(buf, sizeof(buf));
		for (n = strlen(buf); --n >= 0 && buf[n] < ' ';) buf[n] = '\0';
		P("\r\n");

		if (strncmp(buf, "q", 1) == 0)
		{
			fin = 1;
		}
		/* Send data */
		else if ( strncmp(buf, "0x", 2) == 0)
		{
			send_len = 0;
			wk_len = strlen(buf) - 2;
			/* Set sending data */
			for(i=0; i<wk_len;i+=2)
			{
				send_data[send_len++] = ((buf[i+3]-0x30)<<4) + buf[i+2] - 0x30;
			}
			P("\r\n");
			P("addr : data\r\n");
			for(i=0; i<send_len; i++)
			{
				P("0x%08x : 0x%02x\r\n", (send_data+i), send_data[i]);
			}
			P("\r\n");

			/* Start sending and receiving */
			spi_SendRecive(send_data ,rcv_data ,send_len);
			P("Recive : 0x");
			for(i=0; i<send_len; i++)
			{
				P("%02x", rcv_data[i]);
			}
			P("\r\n\r\n");
		}
		else
		{
			P("q          quit\n");
			P("0x[data]   Send data\n");
		}
	}
}

