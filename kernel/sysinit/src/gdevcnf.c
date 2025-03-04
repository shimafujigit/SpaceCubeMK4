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
 *      gdevcnf.c (sysinit)
 *      DEVCONF access utilities
 */

#include "sysinit.h"
#include <device/devconf.h>
#include <sys/sysinfo.h>
#include <libstr.h>
#define DevConfPtr	(SCInfo.devconf)

LOCAL	UB	*SearchDevConf(UB *name)
{
	W	len;
	UB	*p;

	len = (W)strlen((char*)name);
	if (len > 0) {
		if (len > 16) {
			len = 16;
		}
		for (p = DevConfPtr; *p; ) {
			if (*p > ' ' && *p != '#' && memcmp(name, p, (size_t)len) == 0) {
				return p;
			}
			while (*p && *p++ != '\n') {
				;
			}
		}
	}
	return	(UB*)NULL;
}

LOCAL	UB	*SkipNext(UB *p)
{
	for (; *p > ' ' && *p != '#'; p++) {
		;
	}
	for ( ; ; p++) {
		if (*p == '\0' || *p == '\n' || *p == '#') {
			return (UB*)NULL;
		}
		if (*p > ' ') {
			break;
		}
	}
	return p;
}

EXPORT	W	GetDevConf(UB *name, W *val)
{
	W	cnt = 0;
	UB	*p;

	if ( (p = SearchDevConf(name)) != NULL ) {
		for ( ; (p = SkipNext(p)) && cnt < L_DEVCONF_VAL; cnt++) {
			*val++ = (W)strtoul((char*)p, (char**)&p, 0);
		}
	}
	return cnt;
}

EXPORT	W	GetDevConfStr(UB *name, UB *str)
{
	UB	*p;
	UB	*d = str;

	if ( (p = SearchDevConf(name)) != NULL ) {
		if ( (p = SkipNext(p)) != NULL ) {
			while (*p && *p != '\n' && *p != '#') {
				*d++ = *p++;
				if (d - str >= L_DEVCONF_STR - 1) {
					break;
				}
			}
		}
	}
	*d = 0;
	return d - str;
}
