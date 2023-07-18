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
#	Makefile for gmake
#	libsvc (system-dependent description)
#

# T-Engine type
MACHINE = sh7750r
TETYPE = asp

# system dependencies
IFLIB_SYSDEP = device/serialio.h

# default base directory (from ../../src)
BD ?= ../../../

# ----------------------------------------------------------------------------

# source file path
S	= ../../src
IFSRC	= $(S)/sysdepend/$(MACHINE)
INC	= $(S)/sysdepend/include

# auto generation header/source files
IFLIB_TK =	$(BD)/include/tk/syscall.h
IFLIB_TD =	$(BD)/include/tk/dbgspt.h
IFLIB_SVC =	tk/devmgr.h \
		tk/sysmgr.h \
		sys/consio.h \
		sys/segment.h \
		$(IFLIB_SYSDEP)

# ----------------------------------------------------------------------------
# generafe interface library source file

.PHONY: source clean_source

source:
	( cd $(S); ../build_src/$(MACHINE)/mktksvc $(MACHINE) $(IFLIB_TK) );
	( cd $(S); ../build_src/$(MACHINE)/mktdsvc $(MACHINE) $(IFLIB_TD) );
	( cd $(S) ; \
	for iflib_src in $(IFLIB_SVC) ; do \
	  ../build_src/$(MACHINE)/mkiflib $(MACHINE) $(BD)/include/$$iflib_src ; \
	done );

clean_source:
	( cd $(INC) ; $(RM) *.h )
	( cd $(IFSRC) ; $(RM) *.S )

