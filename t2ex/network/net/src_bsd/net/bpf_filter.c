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

/*	$NetBSD: bpf_filter.c,v 1.35.4.1.2.1 2011/03/20 21:28:13 bouyer Exp $	*/

/*-
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from the Stanford/CMU enet packet filter,
 * (net/enet.c) distributed as part of 4.3BSD, and code contributed
 * to Berkeley by Steven McCanne and Van Jacobson both of Lawrence
 * Berkeley Laboratory.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)bpf_filter.c	8.1 (Berkeley) 6/10/93
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: bpf_filter.c,v 1.35.4.1.2.1 2011/03/20 21:28:13 bouyer Exp $");

#if 0
#if !(defined(lint) || defined(KERNEL))
static const char rcsid[] =
    "@(#) Header: bpf_filter.c,v 1.33 97/04/26 13:37:18 leres Exp  (LBL)";
#endif
#endif

#include <sys/param.h>
#include <sys/time.h>
#include <sys/kmem.h>
#include <sys/endian.h>

#if NBPFILTER > 0
#define EXTRACT_SHORT(p)	be16dec(p)
#define EXTRACT_LONG(p)		be32dec(p)

#ifdef _KERNEL
#include <sys/mbuf.h>
#define MINDEX(len, m, k) 		\
{ 					\
	len = m->m_len; 		\
	while (k >= len) { 		\
		k -= len; 		\
		m = m->m_next; 		\
		if (m == 0) 		\
			return 0; 	\
		len = m->m_len; 	\
	} 				\
}

static int m_xword (struct mbuf *, uint32_t, int *);
static int m_xhalf (struct mbuf *, uint32_t, int *);

static int
m_xword(struct mbuf *m, uint32_t k, int *err)
{
	int len;
	u_char *cp, *np;
	struct mbuf *m0;

	*err = 1;
	MINDEX(len, m, k);
	cp = mtod(m, u_char *) + k;
	if (len >= k + 4) {
		*err = 0;
		return EXTRACT_LONG(cp);
	}
	m0 = m->m_next;
	if (m0 == 0 || m0->m_len + len - k < 4)
		return 0;
	*err = 0;
	np = mtod(m0, u_char *);
	switch (len - k) {

	case 1:
		return (cp[0] << 24) | (np[0] << 16) | (np[1] << 8) | np[2];

	case 2:
		return (cp[0] << 24) | (cp[1] << 16) | (np[0] << 8) | np[1];

	default:
		return (cp[0] << 24) | (cp[1] << 16) | (cp[2] << 8) | np[0];
	}
}

static int
m_xhalf(struct mbuf *m, uint32_t k, int *err)
{
	int len;
	u_char *cp;
	struct mbuf *m0;

	*err = 1;
	MINDEX(len, m, k);
	cp = mtod(m, u_char *) + k;
	if (len >= k + 2) {
		*err = 0;
		return EXTRACT_SHORT(cp);
	}
	m0 = m->m_next;
	if (m0 == 0)
		return 0;
	*err = 0;
	return (cp[0] << 8) | mtod(m0, u_char *)[0];
}
#else /* _KERNEL */
#include <stdlib.h>
#endif /* !_KERNEL */

#include <net/bpf.h>

/*
 * Execute the filter program starting at pc on the packet p
 * wirelen is the length of the original packet
 * buflen is the amount of data present
 */
u_int
bpf_filter(struct bpf_insn *pc, u_char *p, u_int wirelen, u_int buflen)
{
	uint32_t A, X, k;
	uint32_t mem[BPF_MEMWORDS];

	if (pc == 0)
		/*
		 * No filter means accept all.
		 */
		return (u_int)-1;
	A = 0;
	X = 0;
	--pc;
	for (;;) {
		++pc;
		switch (pc->code) {

		default:
#ifdef _KERNEL
			return 0;
#else
			abort();
			/*NOTREACHED*/
#endif
		case BPF_RET|BPF_K:
			return (u_int)pc->k;

		case BPF_RET|BPF_A:
			return (u_int)A;

		case BPF_LD|BPF_W|BPF_ABS:
			k = pc->k;
			if (k + sizeof(int32_t) > buflen) {
#ifdef _KERNEL
				int merr = 0;	/* XXX: GCC */

				if (buflen != 0)
					return 0;
				A = m_xword((struct mbuf *)p, k, &merr);
				if (merr != 0)
					return 0;
				continue;
#else
				return 0;
#endif
			}
			A = EXTRACT_LONG(&p[k]);
			continue;

		case BPF_LD|BPF_H|BPF_ABS:
			k = pc->k;
			if (k + sizeof(int16_t) > buflen) {
#ifdef _KERNEL
				int merr;

				if (buflen != 0)
					return 0;
				A = m_xhalf((struct mbuf *)p, k, &merr);
				if (merr != 0)
					return 0;
				continue;
#else
				return 0;
#endif
			}
			A = EXTRACT_SHORT(&p[k]);
			continue;

		case BPF_LD|BPF_B|BPF_ABS:
			k = pc->k;
			if (k >= buflen) {
#ifdef _KERNEL
				struct mbuf *m;
				int len;

				if (buflen != 0)
					return 0;
				m = (struct mbuf *)p;
				MINDEX(len, m, k);
				A = mtod(m, u_char *)[k];
				continue;
#else
				return 0;
#endif
			}
			A = p[k];
			continue;

		case BPF_LD|BPF_W|BPF_LEN:
			A = wirelen;
			continue;

		case BPF_LDX|BPF_W|BPF_LEN:
			X = wirelen;
			continue;

		case BPF_LD|BPF_W|BPF_IND:
			k = X + pc->k;
			if (k + sizeof(int32_t) > buflen) {
#ifdef _KERNEL
				int merr = 0;	/* XXX: GCC */

				if (buflen != 0)
					return 0;
				A = m_xword((struct mbuf *)p, k, &merr);
				if (merr != 0)
					return 0;
				continue;
#else
				return 0;
#endif
			}
			A = EXTRACT_LONG(&p[k]);
			continue;

		case BPF_LD|BPF_H|BPF_IND:
			k = X + pc->k;
			if (k + sizeof(int16_t) > buflen) {
#ifdef _KERNEL
				int merr = 0;	/* XXX: GCC */

				if (buflen != 0)
					return 0;
				A = m_xhalf((struct mbuf *)p, k, &merr);
				if (merr != 0)
					return 0;
				continue;
#else
				return 0;
#endif
			}
			A = EXTRACT_SHORT(&p[k]);
			continue;

		case BPF_LD|BPF_B|BPF_IND:
			k = X + pc->k;
			if (k >= buflen) {
#ifdef _KERNEL
				struct mbuf *m;
				int len;

				if (buflen != 0)
					return 0;
				m = (struct mbuf *)p;
				MINDEX(len, m, k);
				A = mtod(m, u_char *)[k];
				continue;
#else
				return 0;
#endif
			}
			A = p[k];
			continue;

		case BPF_LDX|BPF_MSH|BPF_B:
			k = pc->k;
			if (k >= buflen) {
#ifdef _KERNEL
				struct mbuf *m;
				int len;

				if (buflen != 0)
					return 0;
				m = (struct mbuf *)p;
				MINDEX(len, m, k);
				X = (mtod(m, char *)[k] & 0xf) << 2;
				continue;
#else
				return 0;
#endif
			}
			X = (p[pc->k] & 0xf) << 2;
			continue;

		case BPF_LD|BPF_IMM:
			A = pc->k;
			continue;

		case BPF_LDX|BPF_IMM:
			X = pc->k;
			continue;

		case BPF_LD|BPF_MEM:
			A = mem[pc->k];
			continue;

		case BPF_LDX|BPF_MEM:
			X = mem[pc->k];
			continue;

		case BPF_ST:
			mem[pc->k] = A;
			continue;

		case BPF_STX:
			mem[pc->k] = X;
			continue;

		case BPF_JMP|BPF_JA:
			pc += pc->k;
			continue;

		case BPF_JMP|BPF_JGT|BPF_K:
			pc += (A > pc->k) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JGE|BPF_K:
			pc += (A >= pc->k) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JEQ|BPF_K:
			pc += (A == pc->k) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JSET|BPF_K:
			pc += (A & pc->k) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JGT|BPF_X:
			pc += (A > X) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JGE|BPF_X:
			pc += (A >= X) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JEQ|BPF_X:
			pc += (A == X) ? pc->jt : pc->jf;
			continue;

		case BPF_JMP|BPF_JSET|BPF_X:
			pc += (A & X) ? pc->jt : pc->jf;
			continue;

		case BPF_ALU|BPF_ADD|BPF_X:
			A += X;
			continue;

		case BPF_ALU|BPF_SUB|BPF_X:
			A -= X;
			continue;

		case BPF_ALU|BPF_MUL|BPF_X:
			A *= X;
			continue;

		case BPF_ALU|BPF_DIV|BPF_X:
			if (X == 0)
				return 0;
			A /= X;
			continue;

		case BPF_ALU|BPF_AND|BPF_X:
			A &= X;
			continue;

		case BPF_ALU|BPF_OR|BPF_X:
			A |= X;
			continue;

		case BPF_ALU|BPF_LSH|BPF_X:
			A <<= X;
			continue;

		case BPF_ALU|BPF_RSH|BPF_X:
			A >>= X;
			continue;

		case BPF_ALU|BPF_ADD|BPF_K:
			A += pc->k;
			continue;

		case BPF_ALU|BPF_SUB|BPF_K:
			A -= pc->k;
			continue;

		case BPF_ALU|BPF_MUL|BPF_K:
			A *= pc->k;
			continue;

		case BPF_ALU|BPF_DIV|BPF_K:
			A /= pc->k;
			continue;

		case BPF_ALU|BPF_AND|BPF_K:
			A &= pc->k;
			continue;

		case BPF_ALU|BPF_OR|BPF_K:
			A |= pc->k;
			continue;

		case BPF_ALU|BPF_LSH|BPF_K:
			A <<= pc->k;
			continue;

		case BPF_ALU|BPF_RSH|BPF_K:
			A >>= pc->k;
			continue;

		case BPF_ALU|BPF_NEG:
			A = -A;
			continue;

		case BPF_MISC|BPF_TAX:
			X = A;
			continue;

		case BPF_MISC|BPF_TXA:
			A = X;
			continue;
		}
	}
}

#ifdef _KERNEL
/*
 * Return true if the 'fcode' is a valid filter program.
 * The constraints are that each jump be forward and to a valid
 * code, that memory accesses are within valid ranges (to the
 * extent that this can be checked statically; loads of packet
 * data have to be, and are, also checked at run time), and that
 * the code terminates with either an accept or reject.
 *
 * The kernel needs to be able to verify an application's filter code.
 * Otherwise, a bogus program could easily crash the system.
 */
CTASSERT(BPF_MEMWORDS == sizeof(uint16_t) * NBBY);

int
bpf_validate(struct bpf_insn *f, int signed_len)
{
	u_int i, from, len, ok = 0;
	const struct bpf_insn *p;
#if defined(KERNEL) || defined(_KERNEL)
	uint16_t *mem, invalid;
	size_t size;
#endif

	len = (u_int)signed_len;
	if (len < 1)
		return 0;
#if defined(KERNEL) || defined(_KERNEL)
	if (len > BPF_MAXINSNS)
		return 0;
#endif 
	if (BPF_CLASS(f[len - 1].code) != BPF_RET)
		return 0;

#if defined(KERNEL) || defined(_KERNEL)
	mem = kmem_zalloc(size = sizeof(*mem) * len, KM_SLEEP);
#ifdef T2EX
	if ( mem == NULL ) {
		return 0;
	}
#endif
	invalid = ~0;   /* All is invalid on startup */
#endif

	for (i = 0; i < len; ++i) {
#if defined(KERNEL) || defined(_KERNEL)
		/* blend in any invalid bits for current pc */
		invalid |= mem[i];
#endif
		p = &f[i];
		switch (BPF_CLASS(p->code)) {
		/*
		 * Check that memory operations use valid addresses.
		 */
		case BPF_LD:
		case BPF_LDX:
			switch (BPF_MODE(p->code)) {
			case BPF_MEM:
				/*
				 * There's no maximum packet data size
				 * in userland.  The runtime packet length
				 * check suffices.
				 */
#if defined(KERNEL) || defined(_KERNEL)
				/*
				 * More strict check with actual packet length
				 * is done runtime.
				 */
				if (p->k >= BPF_MEMWORDS)
					goto out;
				/* check for current memory invalid */
				if (invalid & (1 << p->k))
					goto out;
#endif
				break;
			case BPF_ABS:
			case BPF_IND:
			case BPF_MSH:
			case BPF_IMM:
			case BPF_LEN:
				break;
			default:
				goto out;
			}
			break;
		case BPF_ST:
		case BPF_STX:
			if (p->k >= BPF_MEMWORDS)
				goto out;
#if defined(KERNEL) || defined(_KERNEL)
			/* validate the memory word */
			invalid &= ~(1 << p->k);
#endif
			break;
		case BPF_ALU:
			switch (BPF_OP(p->code)) {
			case BPF_ADD:
			case BPF_SUB:
			case BPF_MUL:
			case BPF_OR:
			case BPF_AND:
			case BPF_LSH:
			case BPF_RSH:
			case BPF_NEG:
				break;
			case BPF_DIV:
				/*
				 * Check for constant division by 0.
				 */
				if (BPF_SRC(p->code) == BPF_K && p->k == 0)
					goto out;
				break;
			default:
				goto out;
			}
			break;
		case BPF_JMP:
			/*
			 * Check that jumps are within the code block,
			 * and that unconditional branches don't go
			 * backwards as a result of an overflow.
			 * Unconditional branches have a 32-bit offset,
			 * so they could overflow; we check to make
			 * sure they don't.  Conditional branches have
			 * an 8-bit offset, and the from address is <=
			 * BPF_MAXINSNS, and we assume that BPF_MAXINSNS
			 * is sufficiently small that adding 255 to it
			 * won't overflow.
			 *
			 * We know that len is <= BPF_MAXINSNS, and we
			 * assume that BPF_MAXINSNS is < the maximum size
			 * of a u_int, so that i + 1 doesn't overflow.
			 */
			from = i + 1;
			switch (BPF_OP(p->code)) {
			case BPF_JA:
				if (from + p->k >= len)
					goto out;
#if defined(KERNEL) || defined(_KERNEL)
				if (from + p->k < from)
					goto out;
				/*
				 * mark the currently invalid bits for the
				 * destination
				 */
				mem[from + p->k] |= invalid;
				invalid = 0;
#endif
				break;
			case BPF_JEQ:
			case BPF_JGT:
			case BPF_JGE:
			case BPF_JSET:
				if (from + p->jt >= len || from + p->jf >= len) 
					goto out;
#if defined(KERNEL) || defined(_KERNEL)
				/*
				 * mark the currently invalid bits for both
				 * possible jump destinations
				 */
				mem[from + p->jt] |= invalid;
				mem[from + p->jf] |= invalid;
				invalid = 0;
#endif
				break;
			default:
				goto out;
			}
			break;
		case BPF_RET:
			break;
		case BPF_MISC:
			break;
		default:
			goto out;
		}
	}
	ok = 1;
out:
#if defined(KERNEL) || defined(_KERNEL)
	kmem_free(mem, size);
#endif
	return ok;
}
#endif
#endif
