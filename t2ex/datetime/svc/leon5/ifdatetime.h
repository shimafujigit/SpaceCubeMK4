/*
 *	T2EX SVC parameter packet
 *
 */

#include <basic.h>
#include <t2ex/datetime.h>
#include <sys/str_align.h>
#include "fndatetime.h"

typedef struct {
	const struct tzinfo* tz;	_align64
} DT_DT_SETSYSTZ_PARA;

typedef struct {
	struct tzinfo* tz;	_align64
} DT_DT_GETSYSTZ_PARA;

