/*
 *	T2EX SVC interface library (leon5)
 *
 *		created from <t2ex/fs.h>
 */

#include <machine.h>
#include <tk/sysdef.h>
#include "fnfs.h"

	.text
	.balign	4
	.globl	Csym(fs_fchmod)
	.type	Csym(fs_fchmod), %function
Csym(fs_fchmod):
	/* File management functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

