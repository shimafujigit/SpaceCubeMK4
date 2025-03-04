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

/*	$NetBSD: mii.h,v 1.13 2005/12/11 12:22:42 christos Exp $	*/

/*
 * Copyright (c) 1997 Manuel Bouyer.  All rights reserved.
 *
 * Modification to match BSD/OS 3.0 MII interface by Jason R. Thorpe,
 * Numerical Aerospace Simulation Facility, NASA Ames Research Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Manuel Bouyer.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _DEV_MII_MII_H_
#define	_DEV_MII_MII_H_

/*
 * Registers common to all PHYs.
 */

#define	MII_NPHY	32	/* max # of PHYs per MII */

/*
 * MII commands, used if a device must drive the MII lines
 * manually.
 */
#define	MII_COMMAND_START	0x01
#define	MII_COMMAND_READ	0x02
#define	MII_COMMAND_WRITE	0x01
#define	MII_COMMAND_ACK		0x02

#define	MII_BMCR	0x00 	/* Basic mode control register (rw) */
#define	BMCR_RESET	0x8000	/* reset */
#define	BMCR_LOOP	0x4000	/* loopback */
#define	BMCR_SPEED0	0x2000	/* speed selection (LSB) */
#define	BMCR_AUTOEN	0x1000	/* autonegotiation enable */
#define	BMCR_PDOWN	0x0800	/* power down */
#define	BMCR_ISO	0x0400	/* isolate */
#define	BMCR_STARTNEG	0x0200	/* restart autonegotiation */
#define	BMCR_FDX	0x0100	/* Set duplex mode */
#define	BMCR_CTEST	0x0080	/* collision test */
#define	BMCR_SPEED1	0x0040	/* speed selection (MSB) */

#define	BMCR_S10	0x0000		/* 10 Mb/s */
#define	BMCR_S100	BMCR_SPEED0	/* 100 Mb/s */
#define	BMCR_S1000	BMCR_SPEED1	/* 1000 Mb/s */

#define	BMCR_SPEED(x)	((x) & (BMCR_SPEED0|BMCR_SPEED1))

#define	MII_BMSR	0x01	/* Basic mode status register (ro) */
#define	BMSR_100T4	0x8000	/* 100 base T4 capable */
#define	BMSR_100TXFDX	0x4000	/* 100 base Tx full duplex capable */
#define	BMSR_100TXHDX	0x2000	/* 100 base Tx half duplex capable */
#define	BMSR_10TFDX	0x1000	/* 10 base T full duplex capable */
#define	BMSR_10THDX	0x0800	/* 10 base T half duplex capable */
#define	BMSR_100T2FDX	0x0400	/* 100 base T2 full duplex capable */
#define	BMSR_100T2HDX	0x0200	/* 100 base T2 half duplex capable */
#define	BMSR_EXTSTAT	0x0100	/* Extended status in register 15 */
#define	BMSR_MFPS	0x0040	/* MII Frame Preamble Suppression */
#define	BMSR_ACOMP	0x0020	/* Autonegotiation complete */
#define	BMSR_RFAULT	0x0010	/* Link partner fault */
#define	BMSR_ANEG	0x0008	/* Autonegotiation capable */
#define	BMSR_LINK	0x0004	/* Link status */
#define	BMSR_JABBER	0x0002	/* Jabber detected */
#define	BMSR_EXTCAP	0x0001	/* Extended capability */

/*
 * Note that the EXTSTAT bit indicates that there is extended status
 * info available in register 15, but 802.3 section 22.2.4.3 also
 * states that that all 1000 Mb/s capable PHYs will set this bit to 1.
 */

#define	BMSR_MEDIAMASK	(BMSR_100T4|BMSR_100TXFDX|BMSR_100TXHDX| \
			 BMSR_10TFDX|BMSR_10THDX|BMSR_100T2FDX|BMSR_100T2HDX)

/*
 * Convert BMSR media capabilities to ANAR bits for autonegotiation.
 * Note the shift chopps off the BMSR_ANEG bit.
 */
#define	BMSR_MEDIA_TO_ANAR(x)	(((x) & BMSR_MEDIAMASK) >> 6)

#define	MII_PHYIDR1	0x02	/* ID register 1 (ro) */

#define	MII_PHYIDR2	0x03	/* ID register 2 (ro) */
#define	IDR2_OUILSB	0xfc00	/* OUI LSB */
#define	IDR2_MODEL	0x03f0	/* vendor model */
#define	IDR2_REV	0x000f	/* vendor revision */

#define	MII_ANAR	0x04	/* Autonegotiation advertisement (rw) */
		/* section 28.2.4.1 and 37.2.6.1 */
#define ANAR_NP		0x8000	/* Next page (ro) */
#define	ANAR_ACK	0x4000	/* link partner abilities acknowledged (ro) */
#define ANAR_RF		0x2000	/* remote fault (ro) */
#define	ANAR_FC		0x0400	/* local device supports PAUSE */
#define ANAR_T4		0x0200	/* local device supports 100bT4 */
#define ANAR_TX_FD	0x0100	/* local device supports 100bTx FD */
#define ANAR_TX		0x0080	/* local device supports 100bTx */
#define ANAR_10_FD	0x0040	/* local device supports 10bT FD */
#define ANAR_10		0x0020	/* local device supports 10bT */
#define	ANAR_CSMA	0x0001	/* protocol selector CSMA/CD */

#define	ANAR_X_FD	0x0020	/* local device supports 1000BASE-X FD */
#define	ANAR_X_HD	0x0040	/* local device supports 1000BASE-X HD */
#define	ANAR_X_PAUSE_NONE	(0 << 10)
#define	ANAR_X_PAUSE_SYM	(1 << 10)
#define	ANAR_X_PAUSE_ASYM	(2 << 10)
#define	ANAR_X_PAUSE_TOWARDS	(3 << 10)

#define	MII_ANLPAR	0x05	/* Autonegotiation lnk partner abilities (rw) */
		/* section 28.2.4.1 and 37.2.6.1 */
#define ANLPAR_NP	0x8000	/* Next page (ro) */
#define	ANLPAR_ACK	0x4000	/* link partner accepted ACK (ro) */
#define ANLPAR_RF	0x2000	/* remote fault (ro) */
#define	ANLPAR_FC	0x0400	/* link partner supports PAUSE */
#define ANLPAR_T4	0x0200	/* link partner supports 100bT4 */
#define ANLPAR_TX_FD	0x0100	/* link partner supports 100bTx FD */
#define ANLPAR_TX	0x0080	/* link partner supports 100bTx */
#define ANLPAR_10_FD	0x0040	/* link partner supports 10bT FD */
#define ANLPAR_10	0x0020	/* link partner supports 10bT */
#define	ANLPAR_CSMA	0x0001	/* protocol selector CSMA/CD */

#define	ANLPAR_X_FD	0x0020	/* local device supports 1000BASE-X FD */
#define	ANLPAR_X_HD	0x0040	/* local device supports 1000BASE-X HD */
#define	ANLPAR_X_PAUSE_MASK	(3 << 10)
#define	ANLPAR_X_PAUSE_NONE	(0 << 10)
#define	ANLPAR_X_PAUSE_SYM	(1 << 10)
#define	ANLPAR_X_PAUSE_ASYM	(2 << 10)
#define	ANLPAR_X_PAUSE_TOWARDS	(3 << 10)

#define	MII_ANER	0x06	/* Autonegotiation expansion (ro) */
		/* section 28.2.4.1 and 37.2.6.1 */
#define ANER_MLF	0x0010	/* multiple link detection fault */
#define ANER_LPNP	0x0008	/* link parter next page-able */
#define ANER_NP		0x0004	/* next page-able */
#define ANER_PAGE_RX	0x0002	/* Page received */
#define ANER_LPAN	0x0001	/* link parter autoneg-able */

#define	MII_ANNP	0x07	/* Autonegotiation next page */
		/* section 28.2.4.1 and 37.2.6.1 */

#define	MII_ANLPRNP	0x08	/* Autonegotiation link partner rx next page */
		/* section 32.5.1 and 37.2.6.1 */

			/* This is also the 1000baseT control register */
#define	MII_100T2CR	0x09	/* 100base-T2 control register */
#define	GTCR_TEST_MASK	0xe000	/* see 802.3ab ss. 40.6.1.1.2 */
#define	GTCR_MAN_MS	0x1000	/* enable manual master/slave control */
#define	GTCR_ADV_MS	0x0800	/* 1 = adv. master, 0 = adv. slave */
#define	GTCR_PORT_TYPE	0x0400	/* 1 = DCE, 0 = DTE (NIC) */
#define	GTCR_ADV_1000TFDX 0x0200 /* adv. 1000baseT FDX */
#define	GTCR_ADV_1000THDX 0x0100 /* adv. 1000baseT HDX */

			/* This is also the 1000baseT status register */
#define	MII_100T2SR	0x0a	/* 100base-T2 status register */
#define	GTSR_MAN_MS_FLT	0x8000	/* master/slave config fault */
#define	GTSR_MS_RES	0x4000	/* result: 1 = master, 0 = slave */
#define	GTSR_LRS	0x2000	/* local rx status, 1 = ok */
#define	GTSR_RRS	0x1000	/* remove rx status, 1 = ok */
#define	GTSR_LP_1000TFDX 0x0800	/* link partner 1000baseT FDX capable */
#define	GTSR_LP_1000THDX 0x0400	/* link partner 1000baseT HDX capable */
#define	GTSR_LP_ASM_DIR	0x0200	/* link partner asym. pause dir. capable */
#define	GTSR_IDLE_ERR	0x00ff	/* IDLE error count */

#define	MII_EXTSR	0x0f	/* Extended status register */
#define	EXTSR_1000XFDX	0x8000	/* 1000X full-duplex capable */
#define	EXTSR_1000XHDX	0x4000	/* 1000X half-duplex capable */
#define	EXTSR_1000TFDX	0x2000	/* 1000T full-duplex capable */
#define	EXTSR_1000THDX	0x1000	/* 1000T half-duplex capable */

#define	EXTSR_MEDIAMASK	(EXTSR_1000XFDX|EXTSR_1000XHDX| \
			 EXTSR_1000TFDX|EXTSR_1000THDX)

#endif /* _DEV_MII_MII_H_ */
