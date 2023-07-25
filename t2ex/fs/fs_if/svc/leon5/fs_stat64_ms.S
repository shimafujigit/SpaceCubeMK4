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
	.globl	Csym(fs_stat64_ms)
	.type	Csym(fs_stat64_ms), %function
Csym(fs_stat64_ms):
	/* File management functions is not supported. */
	set	E_NOSPT, %o0
	retl
	nop

