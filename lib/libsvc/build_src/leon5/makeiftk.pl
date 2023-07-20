#! /usr/local/bin/perl
#
# ----------------------------------------------------------------------
#     T-Kernel 2.0 Software Package
#
#     Copyright 2011 by Ken Sakamura.
#     This software is distributed under the T-License 2.0.
# ----------------------------------------------------------------------
#
#     Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
#     Modified by T-Engine Forum at 2012/11/07.
#
# ----------------------------------------------------------------------
#     UCT T2AS DevKit tuned for LEON5 Version 1.00.00
#     Copyright (c) 2021 UC Technology. All Rights Reserved.
# ----------------------------------------------------------------------

#
#	makeiftk.pl
#
#	generate interface library for LEON5
#

sub makelib
{
	print LIB <<EndOfIfLibBody;

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$fn_h>

#ifdef TFN_${Func}

	.text
	.balign	4
	.globl	Csym(${func})
	.type	Csym(${func}), #function
Csym(${func}):
	set	TFN_${Func}, %g2
	ta	TRAP_SVC
	retl
	nop

#endif

EndOfIfLibBody
}

1;
