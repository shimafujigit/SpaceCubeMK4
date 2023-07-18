/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2013/01/28.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2013 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	@(#)smalloc.c (libtk)
 *
 *	Non-resident shared memory allocation
 */

#include "libtk.h"

#ifndef SMALLOCTEST

EXPORT void* Smalloc( size_t size )
{
	void	*p;

	UMEMLOCK( return NULL )
	p = _mem_malloc(size, &_Smacb);
	UMEMUNLOCK()

	return p;
}

EXPORT void* Scalloc( size_t nmemb, size_t size )
{
	void	*p;

	UMEMLOCK( return NULL )
	p = _mem_calloc(nmemb, size, &_Smacb);
	UMEMUNLOCK()

	return p;
}

EXPORT void* Srealloc( void *ptr, size_t size )
{
	void	*p;

	UMEMLOCK( return NULL )
	p = _mem_realloc(ptr, size, &_Smacb);
	UMEMUNLOCK()

	return p;
}

EXPORT void Sfree( void *ptr )
{
	UMEMLOCK( return )
	_mem_free(ptr, &_Smacb);
	UMEMUNLOCK()
}

#else /* SMALLOCTEST */

EXPORT void Smalloctest( int mode )
{
	UMEMLOCK( return )
	_mem_malloctest(mode, &_Smacb);
	UMEMUNLOCK()
}

EXPORT BOOL Smalloccheck( void *ptr )
{
	BOOL	v;

	UMEMLOCK( return FALSE )
	v = _mem_malloccheck(ptr, &_Smacb);
	UMEMUNLOCK()

	return v;
}

#endif /* SMALLOCTEST */
