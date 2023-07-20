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
#	makeifex.pl
#
#	generate extended SVC interface library for LEON5
#

sub makelibex
{
	print LIB <<EndOfIfLibBody;

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$fn_h>

#ifdef ${prefix}_${Func}_FN

	.text
	.balign	4
	.globl	Csym(${func})
	.type	Csym(${func}), #function
Csym(${func}):
	st	%o0, [%sp+17*4]
	st	%o1, [%sp+18*4]
	st	%o2, [%sp+19*4]
	st	%o3, [%sp+20*4]
	st	%o4, [%sp+21*4]
	st	%o5, [%sp+22*4]
	add	%sp, 17*4, %o0
	set	${prefix}_${Func}_FN, %g2
	ta	TRAP_SVC
	retl
	nop

#endif

EndOfIfLibBody
}

sub makelibex_c
{
	print LIB <<EndOfIfLibBodyC1;

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/$if_h>

#ifdef ${prefix}_${Func}_FN

EndOfIfLibBodyC1

	printf LIB "EXPORT %s $func( %s )\n{\n",
		( $ret eq "" )? "void": $ret,
		( $#para < 0 )? "void": join(", ", @para);
	print LIB "\tvolatile ${prefix}_${Func}_PARA	para;\n"
		if ( $#para >= 0 );

	print LIB <<EndOfIfLibBodyC2;
	register int	o0	asm("%o0");
	register int	g2	asm("%g2") = ${prefix}_${Func}_FN;

EndOfIfLibBodyC2

	if ( $#para >= 0 ) {
		for ( $j = 0; $j <= $#para; $j++ ) {
			( $type, $vname, $xp, $pt ) = &split_para($para[$j]);
			print LIB "	para.$vname = $pt$vname;\n";
		}
		print LIB "	o0 = (int)&para;\n\n";
	} else {
		print LIB "	o0 = 0;\n\n";
	}

	print LIB '	Asm("ta	%2": "+r"(o0): "r"(g2), "i"(TRAP_SVC));', "\n";
	print LIB "\n	return o0;\n" if ( $ret ne "" );

	print LIB <<EndOfIfLibBodyC3;
}

#endif

EndOfIfLibBodyC3
}

1;
