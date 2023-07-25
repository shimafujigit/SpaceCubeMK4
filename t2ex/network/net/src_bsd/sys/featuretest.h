/*
 *----------------------------------------------------------------------
 *    T2EX Software Package
 *
 *    Copyright 2012 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2012/12/12.
 *
 *----------------------------------------------------------------------
 */
/*
 * This software package is available for use, modification, 
 * and redistribution in accordance with the terms of the attached 
 * T-License 2.0.
 * If you want to redistribute the source code, you need to attach 
 * the T-License 2.0 document.
 * There's no obligation to publish the content, and no obligation 
 * to disclose it to the T-Engine Forum if you have modified the 
 * software package.
 * You can also distribute the modified source code. In this case, 
 * please register the modification to T-Kernel traceability service.
 * People can know the history of modifications by the service, 
 * and can be sure that the version you have inherited some 
 * modification of a particular version or not.
 *
 *    http://trace.t-engine.org/tk/?lang=en
 *    http://trace.t-engine.org/tk/?lang=ja
 *
 * As per the provisions of the T-License 2.0, T-Engine Forum 
 * ensures that the the portion of the software that is copyrighted 
 * by Ken Sakamura or the T-Engine Forum does not infringe the 
 * copyrights of a third party.
 * However, it does not make any warranty other than this.
 * DISCLAIMER: T-Engine Forum and Ken Sakamura shall not be held
 * responsible for any consequences or damages caused directly or
 * indirectly by the use of this software package.
 *
 * Materials copyrighted by third parties are included in the 
 * software package. Please use them according to the individual 
 * copyright notice and license for these parts.
 */

/*	$NetBSD: featuretest.h,v 1.8 2005/12/11 12:25:20 christos Exp $	*/

/*
 * Written by Klaus Klein <kleink@NetBSD.org>, February 2, 1998.
 * Public domain.
 *
 * NOTE: Do not protect this header against multiple inclusion.  Doing
 * so can have subtle side-effects due to header file inclusion order
 * and testing of e.g. _POSIX_SOURCE vs. _POSIX_C_SOURCE.  Instead,
 * protect each CPP macro that we want to supply.
 */

/*
 * Feature-test macros are defined by several standards, and allow an
 * application to specify what symbols they want the system headers to
 * expose, and hence what standard they want them to conform to.
 * There are two classes of feature-test macros.  The first class
 * specify complete standards, and if one of these is defined, header
 * files will try to conform to the relevant standard.  They are:
 *
 * ANSI macros:
 * _ANSI_SOURCE			ANSI C89
 *
 * POSIX macros:
 * _POSIX_SOURCE == 1		IEEE Std 1003.1 (version?)
 * _POSIX_C_SOURCE == 1		IEEE Std 1003.1-1990
 * _POSIX_C_SOURCE == 2		IEEE Std 1003.2-1992
 * _POSIX_C_SOURCE == 199309L	IEEE Std 1003.1b-1993
 * _POSIX_C_SOURCE == 199506L	ISO/IEC 9945-1:1996
 * _POSIX_C_SOURCE == 200112L	IEEE Std 1003.1-2001
 *
 * X/Open macros:
 * _XOPEN_SOURCE		System Interfaces and Headers, Issue 4, Ver 2
 * _XOPEN_SOURCE_EXTENDED == 1	XSH4.2 UNIX extensions
 * _XOPEN_SOURCE == 500		System Interfaces and Headers, Issue 5
 * _XOPEN_SOURCE == 520		Networking Services (XNS), Issue 5.2
 * _XOPEN_SOURCE == 600		IEEE Std 1003.1-2001, XSI option
 *
 * NetBSD macros:
 * _NETBSD_SOURCE == 1		Make all NetBSD features available.
 *
 * If more than one of these "major" feature-test macros is defined,
 * then the set of facilities provided (and namespace used) is the
 * union of that specified by the relevant standards, and in case of
 * conflict, the earlier standard in the above list has precedence (so
 * if both _POSIX_C_SOURCE and _NETBSD_SOURCE are defined, the version
 * of rename() that's used is the POSIX one).  If none of the "major"
 * feature-test macros is defined, _NETBSD_SOURCE is assumed.
 *
 * There are also "minor" feature-test macros, which enable extra
 * functionality in addition to some base standard.  They should be
 * defined along with one of the "major" macros.  The "minor" macros
 * are:
 *
 * _REENTRANT
 * _ISOC99_SOURCE
 * _LARGEFILE_SOURCE		Large File Support
 *		<http://ftp.sas.com/standards/large.file/x_open.20Mar96.html>
 */

#if defined(_POSIX_SOURCE) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE	1L
#endif

#if !defined(_ANSI_SOURCE) && !defined(_POSIX_C_SOURCE) && \
    !defined(_XOPEN_SOURCE) && !defined(_NETBSD_SOURCE)
#define _NETBSD_SOURCE 1
#endif

#if ((_POSIX_C_SOURCE - 0) >= 199506L || (_XOPEN_SOURCE - 0) >= 500) && \
    !defined(_REENTRANT)
#define _REENTRANT
#endif
