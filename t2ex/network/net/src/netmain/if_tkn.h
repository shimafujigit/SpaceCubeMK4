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
 *	@(#)if_tkn.h
 *
 */

#ifndef _IF_TKN_H_
#define _IF_TKN_H_

#include <sys/queue.h>

#include <sys/param.h>
#include <net/if.h>
#include <net/if_types.h>
#include <net/if_media.h>
#include <net/if_ether.h>

IMPORT INT tkn_dev_rbuf_num;

#ifndef TKN_NIFNAMESIZE
#define TKN_NIFNAMESIZE		8	/* size of device name */
#endif

struct tkn_nif_info;

#define TKN_RXBUF_SIZE			1520

/*
 * Receive buffer structure.
 */
struct tkn_extbuf {
	struct queue		 q;
	struct tkn_nif_info	*nifp;
	union buf_u {
		long long	 _aligner;
		uint8_t		 data[TKN_RXBUF_SIZE];
	} buf;
};

struct tkn_extbuf_hdr {
	struct queue q;
};

#define tkn_extbuf_from_data(ptr)					\
	((struct tkn_extbuf *)((char *)(ptr)				\
				- offsetof(struct tkn_extbuf, buf.data)))
#define tkn_extbuf_to_data(eptr)		\
	((eptr)->buf.data)

#define tkn_rxqh(nifp)	(&(nifp)->rx.hdr.q)

/*
 * Interface structure.
 */
struct tkn_nif_info {
	struct ethercom	nif_ec;		/* generic ethernet structure */
	struct ifmedia	nif_media;	/* interface media infomations */
	struct tkn_nif_info *nif_next;	/* next nif information */

	/* T-Kernel driver's info */
	UB nif_dev[TKN_NIFNAMESIZE];	/* name of driver(ex. Neta) */
	ID nif_dev_id;			/* descriptor id */
	ID nif_msg_id;			/* recv message id */
	ID nif_resid;			/* resource id for driver access */
	ID nif_tid;			/* driver event task */
	UW nif_kind;			/* network type of device */

	/* Receive/Transmit buffer management */
	struct rx_t {
		struct tkn_extbuf_hdr hdr;
	} rx;
	struct tx_t {
		struct tkn_extbuf *ebuf;
	} tx;
};

#define nif_ifnet	nif_ec.ec_if


/*
 *  network interface management
 */
struct tkn_nif_mng {
	int nifm_status;		/* status */
	int nifm_nifmax;		/* number of network interface */
	struct tkn_nif_info *nifm_list;	/* network interface list */

	ID nifm_hardclock;		/* clock task */
};


/**********************************************
 * Global constant variable definition
 *********************************************/

/* return code */
#define TKN_ERR_NOTHING	0
#define TKN_ERR_PARAM		(-1)
#define TKN_ERR_SYS		(-2)
#define TKN_ERR_MEM		(-3)
#define TKN_ERR_NIF		(-4)
#define TKN_ERR_DRV		(-5)
#define TKN_ERR_ELSE		(-6)


/**********************************************
 * Global function definition
 *********************************************/

extern struct tkn_nif_mng tkn_nif_mng;

int	tkn_nif_attach(struct tkn_nif_info *new_info);
int	tkn_nif_detach(struct tkn_nif_info *nif_info);
void	tkn_nif_set_mtu(struct tkn_nif_info *nif_infop, int mtu);

#endif /* _IF_TKN_H_ */
