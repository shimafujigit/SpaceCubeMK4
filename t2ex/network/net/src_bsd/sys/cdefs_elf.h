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

/*	$NetBSD: cdefs_elf.h,v 1.30 2008/07/21 15:22:19 lukem Exp $	*/

/*
 * Copyright (c) 1995, 1996 Carnegie-Mellon University.
 * All rights reserved.
 *
 * Author: Chris G. Demetriou
 *
 * Permission to use, copy, modify and distribute this software and
 * its documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 *
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND
 * FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 *
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */

#ifndef _SYS_CDEFS_ELF_H_
#define	_SYS_CDEFS_ELF_H_

#ifdef __LEADING_UNDERSCORE
#define	_C_LABEL(x)	__CONCAT(_,x)
#define _C_LABEL_STRING(x)	"_"x
#else
#define	_C_LABEL(x)	x
#define _C_LABEL_STRING(x)	x
#endif

#if __STDC__
#define	___RENAME(x)	__asm(___STRING(_C_LABEL(x)))
#else
#ifdef __LEADING_UNDERSCORE
#define	___RENAME(x)	____RENAME(_/**/x)
#define	____RENAME(x)	__asm(___STRING(x))
#else
#define	___RENAME(x)	__asm(___STRING(x))
#endif
#endif

#define	__indr_reference(sym,alias)	/* nada, since we do weak refs */

#if __STDC__
#define	__strong_alias(alias,sym)	       				\
    __asm(".global " _C_LABEL_STRING(#alias) "\n"			\
	    _C_LABEL_STRING(#alias) " = " _C_LABEL_STRING(#sym));

#define	__weak_alias(alias,sym)						\
    __asm(".weak " _C_LABEL_STRING(#alias) "\n"			\
	    _C_LABEL_STRING(#alias) " = " _C_LABEL_STRING(#sym));

/* Do not use __weak_extern, use __weak_reference instead */
#define	__weak_extern(sym)						\
    __asm(".weak " _C_LABEL_STRING(#sym));

#if __GNUC_PREREQ__(4, 0)
#define	__weak_reference(sym)	__attribute__((__weakref__))
#else
#define	__weak_reference(sym)	; __asm(".weak " _C_LABEL_STRING(#sym))
#endif

#define	__warn_references(sym,msg)					\
    __asm(".pushsection .gnu.warning." #sym "\n"			\
	  ".ascii \"" msg "\"\n"					\
	  ".popsection");

#else /* !__STDC__ */

#ifdef __LEADING_UNDERSCORE
#define __weak_alias(alias,sym) ___weak_alias(_/**/alias,_/**/sym)
#define	___weak_alias(alias,sym)					\
    __asm(".weak alias\nalias = sym");
#else
#define	__weak_alias(alias,sym)						\
    __asm(".weak alias\nalias = sym");
#endif
#ifdef __LEADING_UNDERSCORE
#define __weak_extern(sym) ___weak_extern(_/**/sym)
#define	___weak_extern(sym)						\
    __asm(".weak sym");
#else
#define	__weak_extern(sym)						\
    __asm(".weak sym");
#endif
#define	__warn_references(sym,msg)					\
    __asm(".pushsection .gnu.warning.sym\n"				\
	  ".ascii \"" msg "\"\n"					\
	  ".popsection");

#endif /* !__STDC__ */

#if __STDC__
#define	__SECTIONSTRING(_sec, _str)					\
	__asm(".pushsection " #_sec "\n"				\
	      ".asciz \"" _str "\"\n"					\
	      ".popsection")
#else
#define	__SECTIONSTRING(_sec, _str)					\
	__asm(".pushsection _sec\n"					\
	      ".asciz \"" _str "\"\n"					\
	      ".popsection")
#endif

#define	__IDSTRING(_n,_s)		__SECTIONSTRING(.ident,_s)

#define	__RCSID(_s)			__IDSTRING(rcsid,_s)
#define	__SCCSID(_s)
#define __SCCSID2(_s)
#define	__COPYRIGHT(_s)			__SECTIONSTRING(.copyright,_s)

#define	__KERNEL_RCSID(_n, _s)		__RCSID(_s)
#define	__KERNEL_SCCSID(_n, _s)
#define	__KERNEL_COPYRIGHT(_n, _s)	__COPYRIGHT(_s)

#ifndef __lint__
#define	__link_set_make_entry(set, sym)					\
	static void const * const __link_set_##set##_sym_##sym		\
	    __section("link_set_" #set) __used = &sym
#define	__link_set_make_entry2(set, sym, n)				\
	static void const * const __link_set_##set##_sym_##sym##_##n	\
	    __section("link_set_" #set) __used = &sym[n]
#else
#define	__link_set_make_entry(set, sym)					\
	extern void const * const __link_set_##set##_sym_##sym
#define	__link_set_make_entry2(set, sym, n)				\
	extern void const * const __link_set_##set##_sym_##sym##_##n
#endif /* __lint__ */

#define	__link_set_add_text(set, sym)	__link_set_make_entry(set, sym)
#define	__link_set_add_rodata(set, sym)	__link_set_make_entry(set, sym)
#define	__link_set_add_data(set, sym)	__link_set_make_entry(set, sym)
#define	__link_set_add_bss(set, sym)	__link_set_make_entry(set, sym)
#define	__link_set_add_text2(set, sym, n)   __link_set_make_entry2(set, sym, n)
#define	__link_set_add_rodata2(set, sym, n) __link_set_make_entry2(set, sym, n)
#define	__link_set_add_data2(set, sym, n)   __link_set_make_entry2(set, sym, n)
#define	__link_set_add_bss2(set, sym, n)    __link_set_make_entry2(set, sym, n)

#define	__link_set_decl(set, ptype)					\
	extern ptype * const __start_link_set_##set[];			\
	extern ptype * const __stop_link_set_##set[]			\

#define	__link_set_start(set)	(__start_link_set_##set)
#define	__link_set_end(set)	(__stop_link_set_##set)

#define	__link_set_count(set)						\
	(__link_set_end(set) - __link_set_start(set))

#endif /* !_SYS_CDEFS_ELF_H_ */
