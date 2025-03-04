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

/*	$OpenBSD: sensors.h,v 1.31 2011/09/16 15:44:31 yuo Exp $	*/

/*
 * Copyright (c) 2003, 2004 Alexander Yurchenko <grange@openbsd.org>
 * Copyright (c) 2006 Constantine A. Murenin <cnst+openbsd@bugmail.mojo.ru>
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SYS_SENSORS_H_
#define _SYS_SENSORS_H_

/* Sensor types */
enum sensor_type {
	SENSOR_TEMP,			/* temperature (muK) */
	SENSOR_FANRPM,			/* fan revolution speed */
	SENSOR_VOLTS_DC,		/* voltage (muV DC) */
	SENSOR_VOLTS_AC,		/* voltage (muV AC) */
	SENSOR_OHMS,			/* resistance */
	SENSOR_WATTS,			/* power (muW) */
	SENSOR_AMPS,			/* current (muA) */
	SENSOR_WATTHOUR,		/* power capacity */
	SENSOR_AMPHOUR,			/* power capacity */
	SENSOR_INDICATOR,		/* boolean indicator */
	SENSOR_INTEGER,			/* generic integer value */
	SENSOR_PERCENT,			/* percent */
	SENSOR_LUX,			/* illuminance (mulx) */
	SENSOR_DRIVE,			/* disk */
	SENSOR_TIMEDELTA,		/* system time error (nSec) */
	SENSOR_HUMIDITY,		/* humidity (m%RH) */
	SENSOR_FREQ,			/* frequency (muHz) */
	SENSOR_ANGLE,			/* angle (mudegrees) */
	SENSOR_MAX_TYPES
};

#ifndef _KERNEL
static const char * const sensor_type_s[SENSOR_MAX_TYPES + 1] = {
	"temp",
	"fan",
	"volt",
	"acvolt",
	"resistance",
	"power",
	"current",
	"watthour",
	"amphour",
	"indicator",
	"raw",
	"percent",
	"illuminance",
	"drive",
	"timedelta",
	"humidity",
	"frequency",
	"angle",
	"undefined"
};
#endif	/* !_KERNEL */

#define SENSOR_DRIVE_EMPTY	1
#define SENSOR_DRIVE_READY	2
#define SENSOR_DRIVE_POWERUP	3
#define SENSOR_DRIVE_ONLINE	4
#define SENSOR_DRIVE_IDLE	5
#define SENSOR_DRIVE_ACTIVE	6
#define SENSOR_DRIVE_REBUILD	7
#define SENSOR_DRIVE_POWERDOWN	8
#define SENSOR_DRIVE_FAIL	9
#define SENSOR_DRIVE_PFAIL	10

/* Sensor states */
enum sensor_status {
	SENSOR_S_UNSPEC,		/* status is unspecified */
	SENSOR_S_OK,			/* status is ok */
	SENSOR_S_WARN,			/* status is warning */
	SENSOR_S_CRIT,			/* status is critical */
	SENSOR_S_UNKNOWN		/* status is unknown */
};

/* Sensor data:
 * New fields should be added at the end to encourage backwards compat
 */
struct sensor {
	char desc[32];			/* sensor description, may be empty */
	struct timeval tv;		/* sensor value last change time */
	int64_t value;			/* current value */
	enum sensor_type type;		/* sensor type */
	enum sensor_status status;	/* sensor status */
	int numt;			/* sensor number of .type type */
	int flags;			/* sensor flags */
#define SENSOR_FINVALID		0x0001	/* sensor is invalid */
#define SENSOR_FUNKNOWN		0x0002	/* sensor value is unknown */
};

/* Sensor device data:
 * New fields should be added at the end to encourage backwards compat
 */
struct sensordev {
	int num;			/* sensordev number */
	char xname[16];			/* unix device name */
	int maxnumt[SENSOR_MAX_TYPES];
	int sensors_count;
};

#ifdef _KERNEL

/* Sensor data */
struct ksensor {
	SLIST_ENTRY(ksensor) list;	/* device-scope list */
	char desc[32];			/* sensor description, may be empty */
	struct timeval tv;		/* sensor value last change time */
	int64_t value;			/* current value */
	enum sensor_type type;		/* sensor type */
	enum sensor_status status;	/* sensor status */
	int numt;			/* sensor number of .type type */
	int flags;			/* sensor flags, ie. SENSOR_FINVALID */
};
SLIST_HEAD(ksensors_head, ksensor);

/* Sensor device data */
struct ksensordev {
	SLIST_ENTRY(ksensordev)	list;
	int num;			/* sensordev number */
	char xname[16];			/* unix device name */
	int maxnumt[SENSOR_MAX_TYPES];
	int sensors_count;
	struct ksensors_head sensors_list;
};

/* struct ksensordev */
void			 sensordev_install(struct ksensordev *);
void			 sensordev_deinstall(struct ksensordev *);
int			 sensordev_get(int, struct ksensordev **);

/* struct ksensor */
void			 sensor_attach(struct ksensordev *, struct ksensor *);
void			 sensor_detach(struct ksensordev *, struct ksensor *);
int			 sensor_find(int, enum sensor_type, int, struct ksensor **);

/* task scheduling */
struct sensor_task;
struct sensor_task	*sensor_task_register(void *, void (*)(void *), int);
void			 sensor_task_unregister(struct sensor_task *);

#endif	/* _KERNEL */

#endif	/* !_SYS_SENSORS_H_ */
