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
	.globl	Csym(pm_loadspg)
	.type	Csym(pm_loadspg), %function
Csym(pm_loadspg):
	/* Program load functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

