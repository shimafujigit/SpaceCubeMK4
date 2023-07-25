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
 *	@(#)if_ether.h
 *
 */

#ifndef	__NET_IF_ETHER_H__
#define	__NET_IF_ETHER_H__

#include <basic.h>
#include <stdint.h>
#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	ETHER_ADDR_LEN	6	/* length of an Ethernet address */
#define	ETHER_TYPE_LEN	2	/* length of the Ethernet type field */
#define	ETHER_CRC_LEN	4	/* length of the Ethernet CRC */
#define	ETHER_HDR_LEN	((ETHER_ADDR_LEN * 2) + ETHER_TYPE_LEN)
#define	ETHER_MIN_LEN	64	/* minimum frame length, including CRC */
#define	ETHER_MAX_LEN	1518	/* maximum frame length, including CRC */
#define	ETHER_MAX_LEN_JUMBO 9018 /* maximum jumbo frame len, including CRC */

#define	ETHER_VLAN_ENCAP_LEN 4	/* length of 802.1Q VLAN encapsulation */
#define	ETHER_PPPOE_ENCAP_LEN 8	/* length of PPPoE encapsulation */

struct ether_header {
	uint8_t  ether_dhost[ETHER_ADDR_LEN];
	uint8_t  ether_shost[ETHER_ADDR_LEN];
	uint16_t ether_type;
} __packed;

#ifdef __cplusplus
}
#endif
#endif	/* __NET_IF_ETHER_H__ */
