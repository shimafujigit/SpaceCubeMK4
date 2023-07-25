/*
 *	T2EX SVC interface library (leon5)
 *
 *		created from <t2ex/load.h>
 */

#include <machine.h>
#include <tk/sysdef.h>
#include "fnload.h"

	.text
	.balign	4
	.globl	Csym(pm_unload)
	.type	Csym(pm_unload), %function
Csym(pm_unload):
	/* Program load functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

