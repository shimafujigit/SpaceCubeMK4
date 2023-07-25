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
	.globl	Csym(pm_load)
	.type	Csym(pm_load), %function
Csym(pm_load):
	/* Program load functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

