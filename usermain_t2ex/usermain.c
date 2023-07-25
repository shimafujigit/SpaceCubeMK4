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
 *	usermain.c (usermain)
 *	User Main (T2EX)
 */

#include <basic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "userconfig.h"

#ifndef	EXEC_TEST_SUITE
/* Device Drivers */
IMPORT ER ConsoleIO( INT ac, UB *av[] );
#if	USE_RSDRV
IMPORT ER RsDrv( INT ac, UB *av[] );
#endif
//IMPORT ER ClockDrv( INT ac, UB *av[] );
//IMPORT ER SysDiskDrv( INT ac, UB *av[] );
//IMPORT ER ScreenDrv( INT ac, UB *av[] );
//IMPORT ER KbPdDrv( INT ac, UB *av[] );
//IMPORT ER LowKbPdDrv( INT ac, UB *av[] );
IMPORT ER NetDrv( INT ac, UB *av[] );

/* T2EX extension modules */
IMPORT ER dt_main( INT ac, UB* av[] );
//IMPORT ER pm_main( INT ac, UB* av[] );
//IMPORT ER fs_main( INT ac, UB* av[] );
IMPORT ER so_main( INT ac, UB* av[] );

/* Application program */
IMPORT void appl_main(void);
#endif	/*EXEC_TEST_SUITE */

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

	ER	err;

	tm_putstring((UB*)START_MSG);

	/* start the device driver */
	err = ConsoleIO(0, NULL);
	tm_putstring(err >= E_OK ? "ConsoleIO - OK\n" : "ConsoleIO - ERR\n");

#if	USE_RSDRV
	err = RsDrv( 0, NULL );
	tm_printf("RsDrv - %s\n", (err < E_OK) ? "ERR" : "OK");
#endif

//	err = ClockDrv(0, NULL);
//	tm_putstring(err >= E_OK ? "ClockDrv - OK\n" : "ClockDrv - ERR\n");

//	err = SysDiskDrv(0, NULL);
//	tm_putstring(err >= E_OK ? "SysDiskDrv - OK\n" : "SysDiskDrv - ERR\n");

//	err = ScreenDrv(0, NULL);
//	tm_putstring(err >= E_OK ? "ScreenDrv - OK\n" : "ScreenDrv - ERR\n");

//	err = KbPdDrv(0, NULL);
//	tm_putstring(err >= E_OK ? "KbPdDrv - OK\n" : "KbPdDrv - ERR\n");

//	err = LowKbPdDrv(0, NULL);
//	tm_putstring(err >= E_OK ? "LowKbPdDrv - OK\n" : "LowKbPdDrv - ERR\n");

	err = NetDrv(0, NULL);
	tm_putstring(err >= E_OK ? "NetDrv - OK\n" : "NetDrv - ERR\n");

	/* start the T2EX extension modules */
#ifdef	USE_T2EX_DT
	err = dt_main(0, NULL);
	tm_putstring(err >= E_OK ? "dt_main(0) - OK\n":"dt_main(0) - ERR\n");
#endif
#ifdef	USE_T2EX_PM
	err = pm_main(0, NULL);
	tm_putstring(err >= E_OK ? "pm_main(0) - OK\n":"pm_main(0) - ERR\n");
#endif
#ifdef	USE_T2EX_FS
	err = fs_main(0, NULL);
	tm_putstring(err >= E_OK ? "fs_main(0) - OK\n":"fs_main(0) - ERR\n");
#endif
#ifdef	USE_T2EX_NET
	err = so_main(0, NULL);
	tm_putstring(err >= E_OK ? "so_main(0) - OK\n":"so_main(0) - ERR\n");
#endif

	/* initialize stdio */
//	libc_stdio_init();

	/* start the T2EX application */
	tm_putstring("*** T2EX Application program start !!\n");
	appl_main();

	/* shutdowm the T2EX extension modules */
#ifdef	USE_T2EX_DT
	err = dt_main(-1, NULL);
	tm_putstring(err >= E_OK ? "dt_main(-1) - OK\n":"dt_main(-1) - ERR\n");
#endif
#ifdef	USE_T2EX_PM
	err = pm_main(-1, NULL);
	tm_putstring(err >= E_OK ? "pm_main(-1) - OK\n":"pm_main(-1) - ERR\n");
#endif
#ifdef	USE_T2EX_FS
	err = fs_main(-1, NULL);
	tm_putstring(err >= E_OK ? "fs_main(-1) - OK\n":"fs_main(-1) - ERR\n");
#endif
#ifdef	USE_T2EX_NET
	err = so_main(-1, NULL);
	tm_putstring(err >= E_OK ? "so_main(-1) - OK\n":"so_main(-1) - ERR\n");
#endif

	tm_putstring((UB*)"Push 'g' key to shutdown the T-Kernel.\n");
	tm_getchar(-1);

#if	USE_RSDRV
	RsDrv(-1, NULL);
#endif

//	tm_monitor();
#endif	/* EXEC_TEST_SUITE */

	return 0;
}

