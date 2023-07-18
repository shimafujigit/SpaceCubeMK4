/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	usermain.c (usermain)
 *	User Main
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#ifndef	EXEC_TEST_SUITE
/* Device drivers */
IMPORT ER ConsoleIO( INT ac, UB *av[] );
IMPORT ER RsDrv( INT ac, UB *av[] );
#endif

/*
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */
EXPORT	INT	usermain( void )
{
#ifdef	EXEC_TEST_SUITE

	tsmain();

#else	/* EXEC_TEST_SUITE */

	ER	ercd;

	tm_putstring((UB*)"T-Kernel 2.0 for ASP-SH7750R\n");	

	/* Start the device drivers */
	ercd = ConsoleIO(0, NULL);
	tm_printf("ConsoleIO - %s\n", (ercd < E_OK) ? "ERR" : "OK");

	ercd = RsDrv( 0, NULL );
	tm_printf("RsDrv - %s\n", (ercd < E_OK) ? "ERR" : "OK");

	/* Exec command */
	command();

	tm_putstring((UB*)"Push any key to shutdown the T-Kernel.\n");
	tm_getchar(-1);

	/* Stop the device drivers */
	RsDrv(-1, NULL);

	ConsoleIO(-1, NULL);

#endif	/* EXEC_TEST_SUITE */

	return 0;
}
