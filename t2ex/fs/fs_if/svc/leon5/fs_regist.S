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
	.globl	Csym(fs_regist)
	.type	Csym(fs_regist), %function
Csym(fs_regist):
	/* File management functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

