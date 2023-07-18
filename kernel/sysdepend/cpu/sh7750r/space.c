/*
 *----------------------------------------------------------------------
 *    UCT T-Kernel 2.0 DevKit tuned for i.MX6
 *
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:   1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2013/01/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	space.c
 */
#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>
#include <sys/sysinfo.h>
#include <sys/rominfo.h>

#include <tm/tmonitor.h>

#if	USE_MEM_PROTECT

/* PDE
	UB	sz1:1;	// Page Size: 00:1K, 01:4K, 10:64K, 11:1M = 0
	UB	pr:2;	// Protect: 00:S-R, 01:S-RW, 10:U-R, 11:U-RW
	UB	sz2:1;	// = 1
	UB	c:1;	// Cache = 1
	UB	d:1;	// Dirty = 1
	UB	sh:1;	// Shared = 0
	UB	wt:1;	// Write Through = 0 (Write Back)
*/
#define	PDE_sr		0x000
#define	PDE_srw		0x020
#define	PDE_ur		0x040
#define	PDE_urw		0x060
#define	PDE_sz		0x010
#define	PDE_c		0x008
#define	PDE_d		0x004
#define	PDE_wt		0x001
#define	PDE_VALID	0x100

#define	_PTEL(prn, pde)	( ((ppn) << 10) | (pde) )

#define	PDE_U_CODE	( PDE_ur | PDE_sz | PDE_c )
#define	PDE_U_DATA	( PDE_urw | PDE_sz | PDE_c | PDE_d )
#define	PDE_S_CODE	( PDE_sr | PDE_sz | PDE_c )
#define	PDE_S_DATA	( PDE_srw | PDE_sz | PDE_c | PDE_d )

#define	ITLB_ADR	0xF2000000
#define	ITLB_DAT1	0xF3000000
#define	ITLB_DAT2	0xF3800000

typedef	struct {
	UW	vpn_lo;	
	UW	vpn_hi;
	UB	ent[1];		/* PDE */
} PTAB;

LOCAL	PTAB	*p_tab;

#define	PAGE_SZ		(4 * 1024)
#define	VPN(laddr)	(((UW)laddr) / PAGE_SZ)
#define	LPADR(laddr)	(((UW)laddr) & 0xfffff000)
#define	UPADR(laddr)	(((UW)laddr) & 0x7ffff000)
#define	PPADR(laddr)	(((UW)laddr) & 0x1ffff000)
#define	ALIGN_WU(adr)	(((UW)(adr) + sizeof(UW) - 1) & ~(sizeof(UW) - 1))

IMPORT	void	purge_tlb_page( UW laddr );
IMPORT	void	tlb_miss_hdr( void );

/*
 *  Set valid ITLB entry : should ne called in P2 area, with DI
 */
EXPORT	void	setValidITLB( UW laddr )
{
	UW	pn = VPN(laddr);

	if ( pn >= p_tab->vpn_lo && pn < p_tab->vpn_hi ) {
		/* Set ITLB entry for ICACHE operation */
		*((_UW*)ITLB_ADR)  = LPADR(laddr) | PDE_VALID;
		*((_UW*)ITLB_DAT1) = PPADR(laddr) | PDE_VALID | PDE_U_CODE;
		*((_UW*)ITLB_DAT2) = 0;
	}
}

/*
 *  Purge all TLB 
 */
EXPORT	void	purgeAllTLB( void )
{
	UINT	imsk;

	DI(imsk);
	*((_UW*)MMUCR) |= MMU_TF;
	EI(imsk);
}

/*
 *  Create logical space
 */
EXPORT	void	creUserSpace( void *laddr, INT nblk, INT rw )
{
	UW	pn, la, pde;
	UINT	imsk;

	switch (rw) {
	case 0:	pde = PDE_S_CODE;	break;
	case 1:	pde = PDE_S_DATA;	break;
	case 2:	pde = PDE_U_CODE;	break;
	case 3:
	default:	pde = PDE_U_DATA;
	}

	la = UPADR(laddr);
	pn = VPN(la);
	if ( pn >= p_tab->vpn_lo && pn + nblk < p_tab->vpn_hi ) {
		for ( pn -= p_tab->vpn_lo; --nblk >= 0; pn++, la += PAGE_SZ ) {
			DI(imsk);
			p_tab->ent[pn] = pde;
			purge_tlb_page(la);
			EI(imsk);
		}
	}
}

/*
 *  Delete logical space
 */
EXPORT	void	delUserSpace( void *laddr, INT nblk )
{
	UW	pn, la;
	UINT	imsk;

	la = UPADR(laddr);
	pn = VPN(la);
	if ( pn >= p_tab->vpn_lo && pn + nblk < p_tab->vpn_hi ) {
		for ( pn -= p_tab->vpn_lo; --nblk >= 0; pn++, la += PAGE_SZ ) {
			DI(imsk);
			if (p_tab->ent[pn] != 0) {
				FlushCache((void *)la, PAGE_SZ);
				purge_tlb_page(la);
				p_tab->ent[pn] = 0;
			}
			EI(imsk);
		}
	}
}

/*
 *  Initialize User Space
 */
EXPORT	ER	initUserSpace( void )
{
/* Low-level memory management information */
IMPORT	void	*lowmem_top, *lowmem_limit;

	UW	top, end, ucs, uce, uds, ude;

	/* User space address */
	top = VPN(UPADR(lowmem_top));
	end = VPN(UPADR(lowmem_limit + PAGE_SZ - 1));
	ucs = VPN(UPADR(ROMInfo->s_usr_code));
	uce = VPN(UPADR(ROMInfo->e_usr_code + PAGE_SZ - 1));
	uds = VPN(UPADR(ROMInfo->s_usr_data));
	ude = VPN(UPADR(ROMInfo->e_usr_data + PAGE_SZ - 1));

	if (ucs < uce) {
		if (top > ucs) top = ucs;
		if (end < uce) end = uce;
	}
	if (uds < ude) {
		if (top > uds) top = uds;
		if (end < ude) end = ude;
	}

	/* Allocate Page Table at lowmem_top */
	p_tab = (PTAB*)ALIGN_WU(lowmem_top);
	lowmem_top = (void *)((UW)lowmem_top +
				ALIGN_WU(sizeof(PTAB) + end - top));

	/* Initialize Page Table */
	p_tab->vpn_lo = top;
	p_tab->vpn_hi = end;
	memset(p_tab->ent, 0, end - top);

	/* Set User Code Space */
	if (ucs < uce) memset(&p_tab->ent[ucs - top], PDE_U_CODE, uce - ucs);

	/* Set User Data Space */
	if (uds < ude) memset(&p_tab->ent[uds - top], PDE_U_DATA, ude - uds);

	/* Regist TLB miss handler */
	SCArea->tlbmisshdr = (FP)tlb_miss_hdr;

	/* Enable MMU */
	*(_UW*)PTEH  = 0;
	*(_UW*)PTEL  = 0;
	*(_UW*)PTEA  = 0;
	*(_UW*)TTB   = (UW)p_tab;
	*(_UW*)MMUCR = MMU_AT | MMU_TF;

	return E_OK;
}

#endif	/* USE_MEM_PROTECT */

