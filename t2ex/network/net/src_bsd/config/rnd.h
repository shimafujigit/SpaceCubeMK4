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

/*	$NetBSD: rnd.h,v 1.21 2008/04/28 20:24:11 martin Exp $	*/

/*-
 * Copyright (c) 1997 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Michael Graff <explorer@flame.org>.  This code uses ideas and
 * algorithms from the Linux driver written by Ted Ts'o.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SYS_RND_H_
#define	_SYS_RND_H_

#ifndef _KERNEL
#include <sys/cdefs.h>
#endif /* !_KERNEL */

#include <sys/types.h>

#ifdef _KERNEL
#ifndef T2EX
#include <sys/queue.h>
#else
#include <sys/_queue.h>
#endif
#endif

#define	RND_DEV_RANDOM	0	/* minor devices for random and kinda random */
#define	RND_DEV_URANDOM	1

/*
 * Size of entropy pool in 32-bit words.  This _MUST_ be a power of 2.  Don't
 * change this unless you really know what you are doing...
 */
#ifndef RND_POOLWORDS
#define	RND_POOLWORDS	128
#endif
#define	RND_POOLBITS	(RND_POOLWORDS * 32)

/*
 * Number of bytes returned per hash.  This value is used in both
 * rnd.c and rndpool.c to decide when enough entropy exists to do a
 * hash to extract it.
 */
#define	RND_ENTROPY_THRESHOLD	10

/*
 * Size of the event queue.  This _MUST_ be a power of 2.
 */
#ifndef RND_EVENTQSIZE
#define	RND_EVENTQSIZE	128
#endif

typedef struct
{
	uint32_t	poolsize;
	uint32_t 	threshold;
	uint32_t	maxentropy;

	uint32_t	added;
	uint32_t	curentropy;
	uint32_t	removed;
	uint32_t	discarded;
	uint32_t	generated;
} rndpoolstat_t;


typedef struct {
	uint32_t	cursor;		/* current add point in the pool */
	uint32_t	rotate;		/* how many bits to rotate by */
	rndpoolstat_t	stats;		/* current statistics */
	uint32_t	pool[RND_POOLWORDS]; /* random pool data */
} rndpool_t;

typedef struct {
	char		name[16];	/* device name */
	uint32_t	last_time;	/* last time recorded */
	uint32_t	last_delta;	/* last delta value */
	uint32_t	last_delta2;	/* last delta2 value */
	uint32_t	total;		/* entropy from this source */
	uint32_t	type;		/* type */
	uint32_t	flags;		/* flags */
	void		*state;		/* state informaiton */
} rndsource_t;


/*
 * Flags to control the source.  Low byte is type, upper bits are flags.
 */
#define	RND_FLAG_NO_ESTIMATE	0x00000100	/* don't estimate entropy */
#define	RND_FLAG_NO_COLLECT	0x00000200	/* don't collect entropy */

#define	RND_TYPE_UNKNOWN	0	/* unknown source */
#define	RND_TYPE_DISK		1	/* source is physical disk */
#define	RND_TYPE_NET		2	/* source is a network device */
#define	RND_TYPE_TAPE		3	/* source is a tape drive */
#define	RND_TYPE_TTY		4	/* source is a tty device */
#define	RND_TYPE_RNG		5	/* source is a random number
					   generator */
#define	RND_TYPE_MAX		5	/* last type id used */

#ifdef _KERNEL
typedef struct __rndsource_element rndsource_element_t;

struct __rndsource_element {
	LIST_ENTRY(__rndsource_element) list; /* the linked list */
	rndsource_t	data;		/* the actual data */
};

/*
 * Used by rnd_extract_data() and rndpool_extract_data() to describe how
 * "good" the data has to be.
 */
#define	RND_EXTRACT_ANY		0  /* extract anything, even if no entropy */
#define	RND_EXTRACT_GOOD	1  /* return as many good bytes
				      (short read ok) */

#define RND_ENABLED(rp) \
        (((rp)->data.flags & RND_FLAG_NO_COLLECT) == 0)

void		rndpool_init(rndpool_t *);
void		rndpool_init_global(void);
uint32_t	rndpool_get_entropy_count(rndpool_t *);
void		rndpool_get_stats(rndpool_t *, void *, int);
void		rndpool_increment_entropy_count(rndpool_t *, uint32_t);
uint32_t	*rndpool_get_pool(rndpool_t *);
uint32_t	rndpool_get_poolsize(void);
void		rndpool_add_data(rndpool_t *, void *, uint32_t, uint32_t);
uint32_t	rndpool_extract_data(rndpool_t *, void *, uint32_t,
		    uint32_t);

void		rnd_init(void);
void		rnd_add_uint32(rndsource_element_t *, uint32_t);
void		rnd_add_data(rndsource_element_t *, void *, uint32_t,
		    uint32_t);
uint32_t	rnd_extract_data(void *, uint32_t, uint32_t);
void		rnd_attach_source(rndsource_element_t *, const char *,
		    uint32_t, uint32_t);
void		rnd_detach_source(rndsource_element_t *);

#endif /* _KERNEL */

#define	RND_MAXSTATCOUNT	10	/* 10 sources at once max */

/*
 * return "count" random entries, starting at "start"
 */
typedef struct {
	uint32_t	start;
	uint32_t	count;
	rndsource_t	source[RND_MAXSTATCOUNT];
} rndstat_t;

/*
 * return information on a specific source by name
 */
typedef struct {
	char		name[16];
	rndsource_t	source;
} rndstat_name_t;

/*
 * set/clear device flags.  If type is set to 0xff, the name is used
 * instead.  Otherwise, the flags set/cleared apply to all devices of
 * the specified type, and the name is ignored.
 */
typedef struct {
	char		name[16];	/* the name we are adjusting */
	uint32_t	type;		/* the type of device we want */
	uint32_t	flags;		/* flags to set or clear */
	uint32_t	mask;		/* mask for the flags we are setting */
} rndctl_t;

typedef struct {
	uint32_t	len;
	uint32_t	entropy;
	u_char		data[RND_POOLWORDS * 4];
} rnddata_t;

#define	RNDGETENTCNT	_IOR('R',  101, uint32_t) /* get entropy count */
#define	RNDGETSRCNUM	_IOWR('R', 102, rndstat_t) /* get rnd source info */
#define	RNDGETSRCNAME	_IOWR('R', 103, rndstat_name_t) /* get src by name */
#define	RNDCTL		_IOW('R',  104, rndctl_t)  /* set/clear source flags */
#define	RNDADDDATA	_IOW('R',  105, rnddata_t) /* add data to the pool */
#define	RNDGETPOOLSTAT	_IOR('R',  106, rndpoolstat_t)

#endif /* !_SYS_RND_H_ */
