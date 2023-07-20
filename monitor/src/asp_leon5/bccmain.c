/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/10/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	bccmain.c
 */

#include <basic.h>
#include <tk/sysdef.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>

/* BCC */
extern void *sbrk(ptrdiff_t incr);

/* monitor.c */
IMPORT UW reset_proc( void );

#define	BCC_HEAP_SIZE	(32 * 1024)
EXPORT	VP	lowmem_top, lowmem_end;

/*
 * BCC main
 */
EXPORT int main( int ac, char *av[] )
{
	UW	kernel_entry;
	UW	psr;
	char	*p;
	int	s;

	Asm("	mov	%%psr, %0	" : "=r"(psr));
	psr |= PSR_S|PSR_PS|PSR_PIL;
	psr &= ~(PSR_ET|PSR_EF|PSR_EC);
	Asm("	wr	%0, %%psr	\n"
	    "	wr	%0, %1, %%psr	\n"
	    "	nop; nop; nop		" : : "r"(psr), "i"(PSR_ET));

	/* initialize heap memory */
	sbrk(0);
	s = __bcc_heap_max - __bcc_heap_min;
	if ( s > BCC_HEAP_SIZE * 2 ) {
		s -= BCC_HEAP_SIZE;
	} else {
		s /= 2;
	}
	p = sbrk(s);
	lowmem_top = p;
	lowmem_end = p + s;

	/* initialize basic hardware */
	kernel_entry = reset_proc();

	/* jump to kernel entry */
	((FP)kernel_entry)(0);
}
