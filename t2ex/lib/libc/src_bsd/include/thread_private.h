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

/* $OpenBSD: thread_private.h,v 1.25 2011/10/16 06:29:56 guenther Exp $ */

/* PUBLIC DOMAIN: No Rights Reserved. Marco S Hyman <marc@snafu.org> */

#ifndef _THREAD_PRIVATE_H_
#define _THREAD_PRIVATE_H_

/*
 * This file defines the thread library interface to libc.  Thread
 * libraries must implement the functions described here for proper
 * inter-operation with libc.   libc contains weak versions of the
 * described functions for operation in a non-threaded environment.
 */

/*
 * This variable is 0 until a second thread is created.
 */
#if 1
#define _THREAD_PRIVATE_MUTEX(name)
#define _THREAD_PRIVATE_MUTEX_LOCK(t) __libc_lock_findfp()
extern void __libc_lock_findfp(void);
#define _THREAD_PRIVATE_MUTEX_UNLOCK(t) __libc_unlock_findfp()
extern void __libc_unlock_findfp(void);
#else
extern int __isthreaded;

/*
 * Weak symbols are used in libc so that the thread library can
 * efficiently wrap libc functions.
 * 
 * Use WEAK_NAME(n) to get a libc-private name for n (_weak_n),
 *     WEAK_ALIAS(n) to generate the weak symbol n pointing to _weak_n,
 *     WEAK_PROTOTYPE(n) to generate a prototype for _weak_n (based on n).
 */
#define WEAK_NAME(name)		__CONCAT(_weak_,name)
#define WEAK_ALIAS(name)	__weak_alias(name, WEAK_NAME(name))
#ifdef __GNUC__
#define WEAK_PROTOTYPE(name)	__typeof__(name) WEAK_NAME(name)
#else
#define WEAK_PROTOTYPE(name)	/* typeof() only in gcc */
#endif

/*
 * Ditto for hand-written syscall stubs:
 * 
 * Use STUB_NAME(n) to get the strong name of the stub: _thread_sys_n
 *     STUB_ALIAS(n) to generate the weak symbol n pointing to _thread_sys_n,
 *     STUB_PROTOTYPE(n) to generate a prototype for _thread_sys_n (based on n).
 */
#define STUB_NAME(name)		__CONCAT(_thread_sys_,name)
#define STUB_ALIAS(name)	__weak_alias(name, STUB_NAME(name))
#ifdef __GNUC__
#define STUB_PROTOTYPE(name)	__typeof__(name) STUB_NAME(name)
#else
#define STUB_PROTOTYPE(name)	/* typeof() only in gcc */
#endif

/*
 * helper macro to make unique names in the thread namespace
 */
#define __THREAD_NAME(name)	__CONCAT(_thread_tagname_,name)

/*
 * helper functions that exist as (weak) null functions in libc and
 * (strong) functions in the thread library.   These functions:
 *
 * _thread_tag_lock:
 *	lock the mutex associated with the given tag.   If the given
 *	tag is NULL a tag is first allocated.
 *
 * _thread_tag_unlock:
 *	unlock the mutex associated with the given tag.   If the given
 *	tag is NULL a tag is first allocated.
 *
 * _thread_tag_storage:
 *	return a pointer to per thread instance of data associated
 *	with the given tag.  If the given tag is NULL a tag is first
 *	allocated.
 *
 * _thread_mutex_lock:
 *	lock the given mutex. If the given mutex is NULL,
 *	rely on rthreads/pthreads implementation to initialize
 *	the mutex before locking.
 *
 * _thread_mutex_unlock:
 *	unlock the given mutex.
 *
 * _thread_mutex_destroy:
 *	destroy the given mutex.
 */
void	_thread_tag_lock(void **);
void	_thread_tag_unlock(void **);
void   *_thread_tag_storage(void **, void *, size_t, void *);
void	_thread_mutex_lock(void **);
void	_thread_mutex_unlock(void **);
void	_thread_mutex_destroy(void **);

/*
 * Macros used in libc to access thread mutex, keys, and per thread storage.
 * _THREAD_PRIVATE_KEY and _THREAD_PRIVATE_MUTEX are different macros for
 * historical reasons.   They do the same thing, define a static variable
 * keyed by 'name' that identifies a mutex and a key to identify per thread
 * data.
 */
#define _THREAD_PRIVATE_KEY(name)					\
	static void *__THREAD_NAME(name)
#define _THREAD_PRIVATE_MUTEX(name)					\
	static void *__THREAD_NAME(name)
#define _THREAD_PRIVATE_MUTEX_LOCK(name)				\
	_thread_tag_lock(&(__THREAD_NAME(name)))
#define _THREAD_PRIVATE_MUTEX_UNLOCK(name)				\
	_thread_tag_unlock(&(__THREAD_NAME(name)))
#define _THREAD_PRIVATE(keyname, storage, error)			\
	_thread_tag_storage(&(__THREAD_NAME(keyname)), &(storage),	\
			    sizeof (storage), error)

/*
 * Macros used in libc to access mutexes.
 */
#define _MUTEX_LOCK(mutex)						\
	do {								\
		if (__isthreaded)					\
			_thread_mutex_lock(mutex);			\
	} while (0)
#define _MUTEX_UNLOCK(mutex)						\
	do {								\
		if (__isthreaded)					\
			_thread_mutex_unlock(mutex);			\
	} while (0)
#define _MUTEX_DESTROY(mutex)						\
	do {								\
		if (__isthreaded)					\
			_thread_mutex_destroy(mutex);			\
	} while (0)

/*
 * Resolver code is special cased in that it uses global keys.
 */
extern void *__THREAD_NAME(_res);
extern void *__THREAD_NAME(_res_ext);
extern void *__THREAD_NAME(serv_mutex);

/*
 * malloc lock/unlock prototypes and definitions
 */
void	_thread_malloc_lock(void);
void	_thread_malloc_unlock(void);

#define _MALLOC_LOCK()		do {					\
					if (__isthreaded)		\
						_thread_malloc_lock();	\
				} while (0)
#define _MALLOC_UNLOCK()	do {					\
					if (__isthreaded)		\
						_thread_malloc_unlock();\
				} while (0)

void	_thread_atexit_lock(void);
void	_thread_atexit_unlock(void);

#define _ATEXIT_LOCK()		do {					\
					if (__isthreaded)		\
						_thread_atexit_lock();	\
				} while (0)
#define _ATEXIT_UNLOCK()	do {					\
					if (__isthreaded)		\
						_thread_atexit_unlock();\
				} while (0)

void	_thread_arc4_lock(void);
void	_thread_arc4_unlock(void);

#define _ARC4_LOCK()		do {					\
					if (__isthreaded)		\
						_thread_arc4_lock();	\
				} while (0)
#define _ARC4_UNLOCK()		do {					\
					if (__isthreaded)		\
						_thread_arc4_unlock();\
				} while (0)

#endif
#endif /* _THREAD_PRIVATE_H_ */
