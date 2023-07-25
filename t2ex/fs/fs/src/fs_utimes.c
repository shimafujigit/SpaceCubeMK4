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
 *	@(#)fs_utimes.c
 *
 */

#include "fsdefs.h"

/*
 *  Convert SYSTIM to SYSTIM_U
 */
LOCAL	void	SYSTIM_to_SYSTIM_U(SYSTIM tim, SYSTIM_U * tim_u)
{
	*tim_u = ((SYSTIM_U) tim.hi << 32) | tim.lo;
	*tim_u = *tim_u * 1000;
}

/*
 *  Convert timeval to SYSTIM_U
 */
LOCAL	void	timeval_to_SYSTIM_U(const struct timeval *timeval,
					SYSTIM_U * tim_u)
{
	struct tm tm;

	(void)dt_localtime(timeval->tv_sec, NULL, &tm);
	(void)dt_mktime_us(&tm, NULL, tim_u);
	*tim_u += timeval->tv_usec;
}

/*
 *  Do utimes funciton
 */
LOCAL	INT	utimes_us(fs_env_t *env, const B *name, SYSTIM_U * times_u)
{
	fs_path_t	path;
	INT		sts;

	/* Parse the pathname & get path structure */
	sts = fs_path_parse(env, name, &path, TRUE);
	if (sts != 0) goto exit0;

	if (times_u == NULL) {		/* Use system time */
		(void)tk_get_tim_u(&env->t_misc.t_times_u[0], 0);
		env->t_misc.t_times_u[1] = env->t_misc.t_times_u[0];
		times_u = env->t_misc.t_times_u;
	}

	/* Call FIMP UTIMES_US service */
	env->t_cmd.r_utimes_us.path = path.p_name;
	env->t_cmd.r_utimes_us.times_u = times_u;
	sts = fs_callfimp(env, path.p_con, FIMP_UTIMES_US);

	/* Release path structure */
	fs_path_release(env, &path);
	if (sts == 0) return 0;
exit0:
	env->t_errno = sts;
	return -1;
}

/*
 *  fs_utimes() - Set file times
 */
EXPORT	INT	xfs_utimes(fs_env_t *env, const B *name,
					const struct timeval *tim)
{
	SYSTIM_U	*times_u;

	times_u = NULL;
	if (tim != NULL) {
		timeval_to_SYSTIM_U(&tim[0], &env->t_misc.t_times_u[0]);
		timeval_to_SYSTIM_U(&tim[1], &env->t_misc.t_times_u[1]);
		times_u = env->t_misc.t_times_u;
	}

	return utimes_us(env, name, times_u);
}

/*
 *  fs_utimes_us() - Set file times
 */
EXPORT	INT	xfs_utimes_us(fs_env_t *env, const B *name,
					const SYSTIM_U *tim_u)
{
	return utimes_us(env, name, (SYSTIM_U *)tim_u);
}

/*
 *  fs_utimes_ms() - Set file times
 */
EXPORT	INT	xfs_utimes_ms(fs_env_t *env, const B *name,
						const SYSTIM *tim)
{
	SYSTIM_U	*times_u;

	times_u = NULL;
	if (tim != NULL) {
		SYSTIM_to_SYSTIM_U(tim[0], &env->t_misc.t_times_u[0]);
		SYSTIM_to_SYSTIM_U(tim[1], &env->t_misc.t_times_u[1]);
		times_u = env->t_misc.t_times_u;
	}
	return utimes_us(env, name, times_u);
}

