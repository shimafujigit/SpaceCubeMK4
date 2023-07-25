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
 *	@(#)string.h
 *
 */

#ifndef _STRING_H_
#define	_STRING_H_

#include <basic.h>
#include <errno.h>
#include <tk/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

IMPORT	void	*memccpy(void *dst, const void *src, int c, size_t n);
IMPORT	void	*memchr(const void *s, int c, size_t n);
IMPORT	int	memcmp(const void *s1, const void *s2, size_t n);
IMPORT	void	*memcpy(void *dst, const void *src, size_t n);
IMPORT	void	*memmove(void *dst, const void *src, size_t n);
IMPORT	void	*memset(void *s, int c, size_t n);
IMPORT	char	*strcat(char *dst, const char *src);
IMPORT	char	*strchr(const char *s, int c);
IMPORT	int	strcmp(const char *s1, const char *s2);
IMPORT	int	strcoll(const char *s1, const char *s2);
IMPORT	char	*strcpy(char *dst, const char *src);
IMPORT	size_t strcspn(const char *s1, const char *s2);
IMPORT	char	*strdup(const char *s);
IMPORT	size_t strlen(const char *s);
IMPORT	char	*strncat(char *dst, const char *src, size_t n);
IMPORT	int	strncmp(const char *s1, const char *s2, size_t n);
IMPORT	char	*strncpy(char *dst, const char *src, size_t n);
IMPORT	char	*strpbrk(const char *s1, const char *s2);
IMPORT	char	*strrchr(const char *s, int c);
IMPORT	size_t strspn(const char *s1, const char *s2);
IMPORT	char	*strstr(const char *s1, const char *s2);
IMPORT	char	*strtok_r(char *str, const char *sep, char **lasts);
IMPORT	size_t strxfrm(char *dst, const char *src, size_t n);
IMPORT	int	strerror_r(errno_t errnum, char *buf, size_t buflen);
IMPORT	int	strercd_r(ER ercd, char *buf, size_t buflen);

#ifdef __cplusplus
}
#endif
#endif /* _STRING_H_ */

