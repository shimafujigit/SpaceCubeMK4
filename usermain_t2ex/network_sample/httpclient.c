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
 *	@(#)httpclient.c
 *
 */

#include <t2ex/socket.h>
#include <tk/tkernel.h>

#include <stdio.h>
#include <strings.h>

#include "util.h"
#include "httpclient.h"

int http_get(const char* host, const char* path, char* buf, size_t buflen)
{
	int re;
	int sd;
	int error = 0;
	in_addr_t hostaddr;
	struct sockaddr_in sa;
	char req[1024];
	size_t offset;

	hostaddr = resolv_host(host);
	if ( hostaddr == INADDR_NONE ) {
		return -1;
	}

	sd = so_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	DEBUG_PRINT(("http_get: so_socket = %d(%d, %d)\n", sd, MERCD(sd), SERCD(sd)));
	if ( sd < 0 ) {
		return sd;
	}

	bzero(&sa, sizeof sa);
	sa.sin_len = sizeof sa;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = hostaddr;
	sa.sin_port = htons(80);
	re = so_connect(sd, (struct sockaddr*)&sa, sizeof sa);
	DEBUG_PRINT(("http_get: so_connect = %d(%d, %d)\n", re, MERCD(re), SERCD(re)));
	if ( re < 0 ) {
		return re;
	}

	sprintf(req, "GET %s HTTP/1.1\r\nhost: %s\r\nConnection: close\r\n\r\n", path, host);
	re = so_write(sd, req, strlen(req));
	DEBUG_PRINT(("http_get: so_write = %d(%d, %d)\n", re, MERCD(re), SERCD(re)));

	for(offset = 0; offset < buflen;) {
		re = so_read(sd, &buf[offset], buflen - offset);
		DEBUG_PRINT(("http_get: so_read = %d(%d, %d)\n", re, MERCD(re), SERCD(re)));
		if ( re == 0 ) {
			break;
		} else if ( re < 0 ) {
			error = re;
			break;
		}
		offset += re;
	}

	so_close(sd);

	return error;
}
