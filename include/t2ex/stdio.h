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
 *	@(#)stdio.h
 *
 */

#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdtype.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <tk/typedef.h>
#include <sys/types.h>

typedef	struct _FILE {		/* File handle */
	int	opaque[23];
} FILE;

typedef	off_t	fpos_t;		/* Position within file */
typedef	off64_t	fpos64_t;	/* Position within file (64bits) */

#define EOF		(-1)		/* File termination */

#define _IOFBF		0		/* Complete buffering mode */
#define _IOLBF		1		/* Line-buffering mode */
#define _IONBF		2		/* Non-buffering mode */

#define BUFSIZ		1024		/* Buffer size	minimum: 256 */
#define FOPEN_MAX	20		/* Open file count  minimum: 8 */
#define FILENAME_MAX	256		/* File name length */

#define TMP_MAX 	25		/* Temporary file count  minimum: 25 */
#define L_tmpnam	16		/* Temporary file name length */

#define SEEK_SET	0		/* From the top */
#define SEEK_CUR	1		/* From the current position */
#define SEEK_END	2		/* From the termination */

#if	1
IMPORT 	FILE	__sF[];
#define	stdin		(&__sF[0])	/* Standard input */
#define	stdout		(&__sF[1])	/* Standard output */
#define	stderr		(&__sF[2])	/* Standard error output */
#else
IMPORT	FILE	*__stdin, *__stdout, *__stderr;
#define stdin		__stdin		/* Standard input */
#define stdout		__stdout	/* Standard output */
#define stderr		__stderr	/* Standard error output */
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* File access function */
IMPORT	FILE	*fopen(const char *path, const char *mode);
IMPORT	FILE	*fopen_eno(const char *path, const char *mode, errno_t *eno);
IMPORT	FILE	*freopen(const char *path, const char *mode, FILE *stream);
IMPORT	FILE	*freopen_eno(const char *path, const char *mode, FILE *stream, errno_t *eno);
IMPORT	int	fclose(FILE *stream);
IMPORT	int	fclose_eno(FILE *stream, errno_t *eno);
IMPORT	int	fflush(FILE *stream);
IMPORT	void	setbuf(FILE *stream, char *buf);
IMPORT	int	setvbuf(FILE *stream, char *buf, int mode, size_t size);

/* File descriptor access function */
IMPORT	int	fileno(FILE *stream);
IMPORT	FILE	*fdopen(int fd, const char *mode);
IMPORT	FILE	*fdopen_eno(int fd, const char *mode, errno_t *eno);

/* Character I/O function */
IMPORT	int	fputc(int c, FILE *stream);
#define	putc(c, fp)	fputc(c, fp)
#define	putchar(c)	fputc(c, stdout)
IMPORT	int	fputs(const char *s, FILE *stream);
IMPORT	int	puts(const char *s);
IMPORT	int	fgetc(FILE *stream);
#define	getc(fp)	fgetc(fp)
#define	getchar()	fgetc(stdin)
IMPORT	char	*fgets(char *s, int size, FILE *stream);
IMPORT	int	ungetc(int c, FILE *stream);

/* Direct I/O function */
IMPORT	size_t	fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
IMPORT	size_t	fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* File positioning function */
IMPORT	int	fseek(FILE *stream, off_t offset, int whence);
IMPORT	int	fseek64(FILE *stream, off64_t offset, int whence);
IMPORT	int	fgetpos(FILE *stream, fpos_t *pos);
IMPORT	int	fgetpos64(FILE *stream, fpos64_t *pos);
IMPORT	int	fsetpos(FILE *stream, const fpos_t *pos);
IMPORT	int	fsetpos64(FILE *stream, const fpos64_t *pos);
IMPORT	long	ftell(FILE *stream);
IMPORT	int64_t	ftell64(FILE *stream);
IMPORT	void	rewind(FILE *stream);

/* Error processing function */
IMPORT	void	clearerr(FILE *stream);
IMPORT	int	feof(FILE *stream);
IMPORT	errno_t	ferror(FILE *stream);

/* Initializer */
IMPORT	ER	libc_stdio_init(void);
IMPORT	ER	libc_stdio_cleanup(void);

/* printf/scanf function */
IMPORT	int	fprintf(FILE *stream, const char *format, ...);
IMPORT	int	printf(const char *format, ...);
IMPORT	int	snprintf(char *str, size_t size, const char *format, ...);
IMPORT	int	sprintf(char *str, const char *format, ...);
IMPORT	int	vfprintf(FILE *stream, const char *format, va_list ap);
IMPORT	int	vprintf(const char *format, va_list ap);
IMPORT	int	vsnprintf(char *str, size_t size, const char *format, va_list ap);
IMPORT	int	vsprintf(char *str, const char *format, va_list ap);
IMPORT	int	fscanf(FILE *stream, const char *format, ...);
IMPORT	int	scanf(const char *format, ...);
IMPORT	int	sscanf(const char *str, const char *format, ...);
IMPORT	int	vfscanf(FILE *stream, const char *format, va_list ap);
IMPORT	int	vscanf(const char *format, va_list ap);
IMPORT	int	vsscanf(const char *str, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif
#endif	/* __STDIO_H__ */

