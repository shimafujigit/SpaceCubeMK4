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
 *	@(#)tkn_resctl.c
 *
 */

#include <basic.h>
#include <tk/tkernel.h>

#include <sys/param.h>

#include <sys/socket.h>
#include <sys/malloc.h>

#include <netdb.h>

#include "tkn_resctl.h"

struct htent *host_tables = NULL;

/*
 * Both x->addr and x->host must be non-null.
 */
static struct htent* tkn_htent_new( const struct hosttable* x )
{
	struct htent *new;

	new = (struct htent *)malloc( sizeof(struct htent), M_RTABLE,
				      M_NOWAIT|M_ZERO );
	if( new == NULL ) {
		goto error1;
	}
	new->next = NULL;

	new->table.addr = (struct sockaddr *)malloc(x->addr->sa_len,
						    M_RTABLE, M_NOWAIT|M_ZERO);
	if( new->table.addr == NULL ) {
		goto error2;
	}
	memcpy( new->table.addr, x->addr, x->addr->sa_len );

	new->table.host = (char *)malloc( strlen(x->host) + 1, M_RTABLE,
					  M_NOWAIT|M_ZERO );
	if( new->table.host == NULL ) {
		goto error3;
	}
	strcpy( new->table.host, x->host );
	if( x->aliases != NULL ) {
		new->table.aliases = (char *)malloc(strlen(x->aliases) + 1,
						    M_RTABLE, M_NOWAIT|M_ZERO);

		if( new->table.aliases == NULL ) {
			goto error4;
		}
		strcpy( new->table.aliases, x->aliases );
	}
	else {
		new->table.aliases = NULL;
	}

	return new;

error4:
	free( new->table.host, M_RTABLE );
error3:
	free( new->table.addr, M_RTABLE );
error2:
	free( new, M_RTABLE );
error1:
	return NULL;
}

void tkn_htent_free( struct htent* htent )
{
	if( htent == NULL ) {
		return;
	}

	if( htent->table.addr != NULL ) {
		free( htent->table.addr, M_RTABLE );
	}
	if( htent->table.host != NULL ) {
		free( htent->table.host, M_RTABLE );
	}
	if( htent->table.aliases != NULL ) {
		free( htent->table.aliases, M_RTABLE );
	}

	free( htent, M_RTABLE );
}

int tkn_resctl_cmp_table( const struct hosttable* x, const struct hosttable* y )
{
	if( x->addr->sa_len != y->addr->sa_len ) {
		return 1;
	}
	if( memcmp( x->addr, y->addr, y->addr->sa_len ) != 0 ) {
		return 1;
	}
	if( strcmp( x->host, y->host ) != 0 ) {
		return 1;
	}
	if( x->aliases == NULL && y->aliases == NULL ) {
		return 0;
	}
	if( x->aliases == NULL || y->aliases == NULL ) {
		return 1;
	}
	if( strcmp( x->aliases, y->aliases ) != 0 ) {
		return 1;
	}

	return 0;
}

/*
 * tkn_resctl_add_table() is ONLY called from the network daemon task so that
 * this function does not called by multiple tasks at the same time. Therefore
 * the mutual exclusion is not necessary to perform this function.
 */
int tkn_resctl_add_table( const struct hosttable* table, size_t size )
{
	struct htent *found, *new_entry;

	/* Check parameter. */
	if( table == NULL || size < sizeof(struct hosttable) ) {
		return EINVAL;
	}
	if( table->addr == NULL || table->host == NULL ) {
		return EINVAL;
	}

	/* Search a duplicate entry. */
	for( found = host_tables; found != NULL; found = found->next ) {
		if(tkn_resctl_cmp_table(&found->table, table) == 0) {
			break;
		}
	}
	if( found != NULL) {
		return EEXIST;
	}

	/* Allocate and Copy */
	new_entry = tkn_htent_new( table );
	if( new_entry == NULL ) {
		return ENOMEM;
	}

	/* Insert the new etnry into the table. */
	if (host_tables == NULL) {
		host_tables = new_entry;
		new_entry->next = NULL;
	}
	else {
		new_entry->next = host_tables->next;
		host_tables->next = new_entry;
	}

	return 0;
}

int tkn_resctl_del_table( const struct hosttable* table, size_t size )
{
	struct htent *found, *prev;

	/* Check parameter. */
	if( table == NULL || size < sizeof(struct hosttable) ) {
		return EINVAL;
	}
	if( table->addr == NULL || table->host == NULL ) {
		return EINVAL;
	}

	prev = NULL;
	for( found=host_tables; found != NULL; found=found->next ) {
		if( tkn_resctl_cmp_table( &found->table, table ) == 0 ) {
			break;
		}
		prev = found;
	}
	if( found == NULL ) {
		/* There no corresponding entry. */
		return ENOENT;
	}

	if( found == host_tables ) {
		host_tables = found->next;
	}
	else {
		prev->next = found->next;
	}

	tkn_htent_free( found );

	return 0;
}

int tkn_resctl_get_tables( struct hosttable* table, size_t size, size_t *len )
{
	struct htent *entry;
	int i, num = 0;
	int strofs = 0;
	int strsz = 0;
	void *sockptr, *strptr;

	if (size < sizeof(struct sockaddr*)) {
		return EINVAL;
	}

	*len = sizeof(struct sockaddr*);
	for( entry = host_tables; entry != NULL; entry = entry->next ) {
		strsz = strlen(entry->table.host) + 1;
		if( entry->table.aliases != NULL ) {
			strsz += strlen( entry->table.aliases ) + 1;
		}

		*len += (sizeof(struct hosttable) + entry->table.addr->sa_len
			 + strsz);
		if( *len <= size ) {
			num++;
			strofs += entry->table.addr->sa_len;
		}
	}

	if( table == NULL ) {
		return 0;
	}

	sockptr = (void*)&table[num] + sizeof(struct sockaddr*);
	strptr = sockptr + strofs;

	for( entry=host_tables, i=0; i<num; i++, entry=entry->next ) {
		table[i].addr = (struct sockaddr*)sockptr;
		sockptr += entry->table.addr->sa_len;
		memcpy( table[i].addr, entry->table.addr,
			entry->table.addr->sa_len );

		table[i].host = (char*)strptr;
		strptr += strlen( entry->table.host ) + 1;
		strcpy( table[i].host, entry->table.host );

		if( entry->table.aliases != NULL ) {
			table[i].aliases = strptr;
			strptr += strlen( entry->table.aliases ) + 1;
			strcpy( table[i].aliases, entry->table.aliases );
		}
		else {
			table[i].aliases = NULL;
		}
	}
	table[num].addr = NULL;

	return 0;
}

int tkn_resctl_flush_tables( void )
{
	struct htent *entry, *next;

	for( entry = host_tables; entry != NULL; entry = next ) {
		next = entry->next;
		tkn_htent_free( entry );
	}
	host_tables = NULL;

	return 0;
}
