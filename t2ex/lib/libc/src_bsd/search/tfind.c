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

/*	$OpenBSD: tfind.c,v 1.5 2005/03/30 18:51:49 pat Exp $	*/

/*
 * Tree search generalized from Knuth (6.2.2) Algorithm T just like
 * the AT&T man page says.
 *
 * The node_t structure is for internal use only, lint doesn't grok it.
 *
 * Written by reading the System V Interface Definition, not the code.
 *
 * Totally public domain.
 */
/*LINTLIBRARY*/
#include <search.h>

typedef struct node_t
{
    char	  *key;
    struct node_t *llink, *rlink;
} node;

/* find a node, or return 0 */
void *
tfind(const void *vkey, void * const *vrootp,
    int (*compar)(const void *, const void *))
{
    char *key = (char *)vkey;
    node **rootp = (node **)vrootp;

    if (rootp == (struct node_t **)0)
	return ((struct node_t *)0);
    while (*rootp != (struct node_t *)0) {	/* T1: */
	int r;
	if ((r = (*compar)(key, (*rootp)->key)) == 0)	/* T2: */
	    return (*rootp);		/* key found */
	rootp = (r < 0) ?
	    &(*rootp)->llink :		/* T3: follow left branch */
	    &(*rootp)->rlink;		/* T4: follow right branch */
    }
    return (node *)0;
}
