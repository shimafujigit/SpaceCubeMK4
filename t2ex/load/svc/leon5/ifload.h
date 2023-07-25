/*
 *	T2EX SVC parameter packet
 *
 */

#include <basic.h>
#include <t2ex/load.h>
#include <sys/str_align.h>
#include "fnload.h"

typedef struct {
	const struct pm* prog;	_align64
	UINT attr;	_align64
	pm_entry_t** entry;	_align64
} PM_PM_LOAD_PARA;

typedef struct {
	const struct pm* prog;	_align64
	INT ac;	_align64
	UB* (*av);	_align64
} PM_PM_LOADSPG_PARA;

typedef struct {
	ID progid;	_align64
	struct pm_stat* status;	_align64
} PM_PM_STATUS_PARA;

typedef struct {
	ID progid;	_align64
} PM_PM_UNLOAD_PARA;

