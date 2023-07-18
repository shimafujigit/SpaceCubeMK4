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
#     Changes: Adapted to the ASP-SH7750R Board.
#     Changed by UC Technology at 2012/12/20.
#     
#     UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
#     Copyright (c) 2012 UC Technology. All Rights Reserved.
# ----------------------------------------------------------------------

#
#	makeifex.pl
#
#	generate extended SVC interface library for SH7750R [HEW]
#

sub makelibex
{
	print LIB <<EndOfHeadIfLibBody;
	.include "machine.inc"
	.include "tk/sysdef.inc"
	.include "sys/svc/$fn_inc"

	.aifdef	${prefix}_${Func}_FN

	.section P,code
	.align	2
	.global	_${func}
_${func}:
EndOfHeadIfLibBody

	print LIB "\tmov.l\tr7, \@-r15\n" if ( ${argcnt} >= 4);
	print LIB "\tmov.l\tr6, \@-r15\n" if ( ${argcnt} >= 3);
	print LIB "\tmov.l\tr5, \@-r15\n" if ( ${argcnt} >= 2);
	print LIB "\tmov.l\tr4, \@-r15\n" if ( ${argcnt} >= 1);

	print LIB <<EndOfExtIfLibBody;
	mov.l	fno, r0
	mov	r15, r4
	trapa	#TRAP_SVC
	rts
	add	#${argcnt}*4, r15

	.align	4
fno:	.data.l	${prefix}_${Func}_FN

	.aendi

	.end

EndOfExtIfLibBody
}

1;
