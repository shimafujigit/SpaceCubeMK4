/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *	Extended SVC function code
 *
 */

#include <sys/ssid.h>

#define SERIAL_SERIAL_IN_FN	(0x00010500 | SERIAL_SVC)
#define SERIAL_SERIAL_OUT_FN	(0x00020500 | SERIAL_SVC)
#define SERIAL_SERIAL_CTL_FN	(0x00030300 | SERIAL_SVC)

