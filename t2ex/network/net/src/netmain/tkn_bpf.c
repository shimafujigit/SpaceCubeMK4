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
 *	@(#)tkn_bpf.c
 *
 */

#include <tk/tkernel.h>
#include <sys/_queue.h>

#include <sys/types.h>
#include <sys/file.h>
#include <sys/systm.h>
#include <sys/conf.h>
//#include <stdlib.h>
long int strtol(const char *nptr, char **endptr, int base);
#if 0
#include <string.h>
#endif
#include <sys/errno.h>
#ifndef T2EX
#include <fcntl.h>
#else
#include <sys/fcntl.h>
#endif

#include "../tkn.h"

#ifndef NBPFILTER
#define	NBPFILTER	0
#endif

#if NBPFILTER > 0

/* Device major number for BPF. */
#define DEV_BPF		23
extern const struct cdevsw bpf_cdevsw;
static const struct cdevsw *d_ops = &bpf_cdevsw;
#define getdev(fp)	((dev_t)(fp)->f_data)

static int
devo_read(struct file *fp, off_t *offset, struct uio *uio, kauth_cred_t cred,
    int flags)
{
	(void)offset;
	(void)cred;
	(void)flags;

	return d_ops->d_read(getdev(fp), uio, 0);
}

static int
devo_write(struct file *fp, off_t *offset, struct uio *uio, kauth_cred_t cred,
    int flags)
{
	(void)offset;
	(void)cred;
	(void)flags;

	return d_ops->d_write(getdev(fp), uio, 0);
}

static int
devo_open(dev_t dev, int oflag)
{
	return d_ops->d_open(dev, oflag, 0, NULL);
}

static int
devo_close(struct file *fp)
{
	return d_ops->d_close(getdev(fp), 0, 0, NULL);
}

static int
devo_ioctl(struct file *fp, u_long cmd, void *data)
{
	return d_ops->d_ioctl(getdev(fp), cmd, data, 0, NULL);
}

static int
devo_poll(struct file *fp, int events)
{
	return d_ops->d_poll(getdev(fp), events, NULL);
}

static struct fileops devops = {
	devo_read, devo_write, devo_ioctl,
	(void *)nostop, devo_poll, (void *)nosize,
	devo_close, (void *)nullkqfilter,
	NULL, NULL, NULL
};

int
tkn_bpf_open(const char *path, int oflag, struct file *fp)
{
	int error;
	const char bpf[] = "/dev/bpf";
	const int len = sizeof(bpf) - 1;

	if (strncmp(path, bpf, len) != 0)
		goto err_invalid_name;

	char *end = NULL;
	int no = strtol(path + len, &end, 10);

	if (end && *end == '\0' && 0 <= no) {
		dev_t dev = makedev(DEV_BPF, no);
		error = devo_open(dev, oflag);
		if (error)
			goto err_open_device;

		memset(fp, 0, sizeof(struct file));

		fp->f_flag = FFLAGS(oflag & O_ACCMODE);
		fp->f_type = DTYPE_MISC;
		fp->f_ops = &devops;
		fp->f_data = (void *)dev;

		return 0;
	}

err_invalid_name:
	return ENXIO;

err_open_device:
	return (error);
}

#else /* NBPFILTER */

int
tkn_bpf_open(const char *path, int oflag, struct file *fp)
{
	return ENXIO;
}

#endif /* NBPFILTER */
