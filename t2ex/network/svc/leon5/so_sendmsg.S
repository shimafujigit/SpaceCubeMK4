/*
 *	T2EX SVC interface library (leon5)
 *
 *		created from <t2ex/socket.h>
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5 Version 1.00.00
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <machine.h>
#include <tk/sysdef.h>
#include "fnsocket.h"

	.text
	.balign	4
	.globl	Csym(so_sendmsg)
	.type	Csym(so_sendmsg), %function
Csym(so_sendmsg):
	st	%o0, [%sp+17*4]
	st	%o1, [%sp+18*4]
	st	%o2, [%sp+19*4]
	st	%o3, [%sp+20*4]
	st	%o4, [%sp+21*4]
	st	%o5, [%sp+22*4]
	add	%sp, 17*4, %o0
	set	NET_SO_SENDMSG_FN, %g2
	ta	TRAP_SVC
	retl
	nop

