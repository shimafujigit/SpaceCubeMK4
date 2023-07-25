/*
 *	T2EX SVC parameter packet
 *
 */

#include <basic.h>
#include <t2ex/fs.h>
#include <sys/str_align.h>
#include "fnfs.h"

typedef struct {
	const char *fimpnm;	_align64
	const fs_fimp_t *fimp;	_align64
	void *info;	_align64
} FS_FS_REGIST_PARA;

typedef struct {
	const char *fimpnm;	_align64
} FS_FS_UNREGIST_PARA;

typedef struct {
	const char *devnm;	_align64
	const char *connm;	_align64
	const char *fimpnm;	_align64
	int flags;	_align64
	void *info;	_align64
} FS_FS_ATTACH_PARA;

typedef struct {
	const char *connm;	_align64
} FS_FS_DETACH_PARA;

typedef struct {
	ID tskid;	_align64
} FS_FS_BREAK_PARA;

typedef struct {
	const char *path;	_align64
} FS_FS_CHDIR_PARA;

typedef struct {
	int fd;	_align64
} FS_FS_FCHDIR_PARA;

typedef struct {
	char *buf;	_align64
	size_t size;	_align64
} FS_FS_GETCWD_PARA;

typedef struct {
	const char *path;	_align64
	mode_t mode;	_align64
} FS_FS_CHMOD_PARA;

typedef struct {
	int fd;	_align64
	mode_t mode;	_align64
} FS_FS_FCHMOD_PARA;

typedef struct {
	int fd;	_align64
} FS_FS_FSYNC_PARA;

typedef struct {
	int fd;	_align64
} FS_FS_FDATASYNC_PARA;

typedef struct {
	const char *path;	_align64
	mode_t mode;	_align64
} FS_FS_MKDIR_PARA;

typedef struct {
	const char *path;	_align64
} FS_FS_RMDIR_PARA;

typedef struct {
	int fd;	_align64
	struct dirent* buf;	_align64
	size_t bufsz;	_align64
} FS_FS_GETDENTS_PARA;

typedef struct {
	int fd;	_align64
	void *buf;	_align64
	size_t count;	_align64
} FS_FS_READ_PARA;

typedef struct {
	int fd;	_align64
	const void *buf;	_align64
	size_t count;	_align64
} FS_FS_WRITE_PARA;

typedef struct {
	int fd;	_align64
} FS_FS_CLOSE_PARA;

typedef struct {
	const char *oldpath;	_align64
	const char *newpath;	_align64
} FS_FS_RENAME_PARA;

typedef struct {
	const char *path;	_align64
} FS_FS_UNLINK_PARA;

typedef struct {
	const char *path;	_align64
	struct stat *buf;	_align64
} FS_FS_STAT_PARA;

typedef struct {
	const char *path;	_align64
	struct stat_us *ubuf;	_align64
} FS_FS_STAT_US_PARA;

typedef struct {
	const char *path;	_align64
	struct stat_ms *mbuf;	_align64
} FS_FS_STAT_MS_PARA;

typedef struct {
	const char *path;	_align64
	struct stat64 *buf64;	_align64
} FS_FS_STAT64_PARA;

typedef struct {
	const char *path;	_align64
	struct stat64_us *ubuf64;	_align64
} FS_FS_STAT64_US_PARA;

typedef struct {
	const char *path;	_align64
	struct stat64_ms *mbuf64;	_align64
} FS_FS_STAT64_MS_PARA;

typedef struct {
	int fd;	_align64
	struct stat *buf;	_align64
} FS_FS_FSTAT_PARA;

typedef struct {
	int fd;	_align64
	struct stat_us *ubuf;	_align64
} FS_FS_FSTAT_US_PARA;

typedef struct {
	int fd;	_align64
	struct stat_ms *mbuf;	_align64
} FS_FS_FSTAT_MS_PARA;

typedef struct {
	int fd;	_align64
	struct stat64 *buf64;	_align64
} FS_FS_FSTAT64_PARA;

typedef struct {
	int fd;	_align64
	struct stat64_us *ubuf64;	_align64
} FS_FS_FSTAT64_US_PARA;

typedef struct {
	int fd;	_align64
	struct stat64_ms *mbuf64;	_align64
} FS_FS_FSTAT64_MS_PARA;

typedef struct {
	const char *path;	_align64
	off_t length;	_align64
} FS_FS_TRUNCATE_PARA;

typedef struct {
	const char *path;	_align64
	off64_t length64;	_align64
} FS_FS_TRUNCATE64_PARA;

typedef struct {
	int fd;	_align64
	off_t length;	_align64
} FS_FS_FTRUNCATE_PARA;

typedef struct {
	int fd;	_align64
	off64_t length64;	_align64
} FS_FS_FTRUNCATE64_PARA;

typedef struct {
	const char *path;	_align64
	const struct timeval (*tim);	_align64
} FS_FS_UTIMES_PARA;

typedef struct {
	const char *path;	_align64
	const SYSTIM_U (*tim_u);	_align64
} FS_FS_UTIMES_US_PARA;

typedef struct {
	const char *path;	_align64
	const SYSTIM (*tim_m);	_align64
} FS_FS_UTIMES_MS_PARA;

typedef struct {
	const char *path;	_align64
	struct statvfs *buf;	_align64
} FS_FS_STATVFS_PARA;

typedef struct {
	int fd;	_align64
	struct statvfs *buf;	_align64
} FS_FS_FSTATVFS_PARA;

typedef struct {
	const char *path;	_align64
	int oflag;	_align64
	mode_t mode;	_align64
} FS__FS_OPEN_PARA;

typedef struct {
	int fd;	_align64
	off_t offset;	_align64
	int whence;	_align64
	off_t *roff;	_align64
} FS__FS_LSEEK_PARA;

typedef struct {
	int fd;	_align64
	off64_t offset64;	_align64
	int whence;	_align64
	off64_t *roff;	_align64
} FS__FS_LSEEK64_PARA;

typedef struct {
	int fd;	_align64
	int request;	_align64
	void *arg;	_align64
} FS__FS_IOCTL_PARA;

typedef struct {
	int fd;	_align64
	int cmd;	_align64
	void *arg;	_align64
} FS__FS_FCNTL_PARA;

