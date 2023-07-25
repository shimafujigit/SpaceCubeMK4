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
 *	@(#)ip.h
 *
 */

#ifndef __NETINET_IP_H__
#define	__NETINET_IP_H__

#include <basic.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	IPVERSION	4

struct ip {
#if BIGENDIAN
	unsigned int ip_v:4,		/* version */
		     ip_hl:4;		/* header length */
#else
	unsigned int ip_hl:4,		/* header length */
		     ip_v:4;		/* version */
#endif
	uint8_t  ip_tos;		/* type of service */
	uint16_t ip_len;		/* total length */
	uint16_t ip_id;			/* identification */
	uint16_t ip_off;		/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_EF 0x8000			/* evil flag, per RFC 3514 */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	uint8_t  ip_ttl;		/* time to live */
	uint8_t  ip_p;			/* protocol */
	uint16_t ip_sum;		/* checksum */
	struct	  in_addr ip_src, ip_dst; /* source and dest address */
} __packed;

#define	IP_MAXPACKET	65535		/* maximum packet size */
#define	IP_MINFRAGSIZE	69		/* minumum size that can be fraged */

#ifdef __cplusplus
}
#endif
#endif /* __NETINET_IP_H__ */
