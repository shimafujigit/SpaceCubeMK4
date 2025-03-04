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

/*
 *	@(#)if_dl.h
 *
 */

#ifndef	__NET_IF_DL_H__
#define	__NET_IF_DL_H__

#include <basic.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Structure of a Link-Level sockaddr:
 */
struct sockaddr_dl {
	uint8_t	    sdl_len;	/* Total length of sockaddr */
	sa_family_t sdl_family;	/* AF_LINK */
	uint16_t   sdl_index;	/* if != 0, system given index for interface */
	uint8_t	    sdl_type;	/* interface type */
	uint8_t	    sdl_nlen;	/* interface name length, no trailing 0 reqd. */
	uint8_t	    sdl_alen;	/* link level address length */
	uint8_t	    sdl_slen;	/* link layer selector length */
	/* minimum work area, can be larger; contains both if name
	 * and ll address
	 */
	char	    sdl_data[12];
};

#define	satosdl(__sa)	((struct sockaddr_dl *)(__sa))
#define	satocsdl(__sa)	((const struct sockaddr_dl *)(__sa))

#define LLADDR(s) ((char *)((s)->sdl_data + (s)->sdl_nlen))
#define CLLADDR(s) ((const char *)((s)->sdl_data + (s)->sdl_nlen))

#ifdef __cplusplus
}
#endif
#endif	/* __NET_IF_DL_H__ */
