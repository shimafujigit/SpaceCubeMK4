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
	.globl	Csym(fs_fstat)
	.type	Csym(fs_fstat), %function
Csym(fs_fstat):
	/* File management functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

