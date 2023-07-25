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
 *	@(#)search.h
 *
 */

#ifndef _SEARCH_H_
#define	_SEARCH_H_

#include <basic.h>

typedef struct {
	char	*key;
	void	*data;
} ENTRY;

typedef enum {
	FIND,
	ENTER
} ACTION;

typedef enum {
	preorder,
	postorder,
	endorder,
	leaf
} VISIT;

struct hsearch_data {
	void	*htable;
	int	htablesize;
};

#ifdef __cplusplus
extern "C" {
#endif

IMPORT	int	hcreate_r(size_t nel, struct hsearch_data *htab);
IMPORT	void	hdestroy_r(struct hsearch_data* htab);
IMPORT	int	hsearch_r(ENTRY item, ACTION action, ENTRY **result, struct hsearch_data *htab);
IMPORT	void	insque(void *element, void *pred);
IMPORT	void	remque(void *element);
IMPORT	void	*lfind(const void *key, const void *base, size_t *nelp, size_t width, int (*compar)(const void *, const void *));
IMPORT	void	*lsearch(const void *key, void *base, size_t *nelp, size_t width, int (*compar)(const void *, const void *));
IMPORT	void	*tdelete(const void *key, void **rootp, int(*compar)(const void *, const void *));
IMPORT	void	*tfind(const void *key, void *const *rootp, int(*compar)(const void *, const void *));
IMPORT	void	*tsearch(const void *key, void **rootp, int(*compar)(const void *, const void *));
IMPORT	void	twalk(const void *root, void (*action)(const void *, VISIT, int ));

#ifdef __cplusplus
}
#endif
#endif /* _SEARCH_H_ */

