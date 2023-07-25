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
 *	@(#)fs_break.c
 *
 */

#include "fsdefs.h"

/*
 *  Call FIMP break function
 */
EXPORT	void	xfs_break_impl(fs_cond_t *con, ID tid, BOOL set)
{
	INT (*breakfn)(coninf_t *coninf, ID tid, BOOL set);

	if ((con->c_fimpsd->p_fimp.flags & FIMP_FLAG_USEABORT) != 0) {
		breakfn = con->c_fimpsd->p_fimp.breakfn;
		if (breakfn != NULL) {
			(void)(*breakfn)(&con->c_coninf, tid, set);
		}
	}
}

/*
 *  Break the task execution
 */
LOCAL	INT	xfs_break_tsk(ID tid)
{
	fs_tsd_t	*tsd;

	tsd = (fs_tsd_t *)fs_tsd_get(tid);
	if (tsd == NULL) return EX_INVAL;

	/* Disable dispatch to ensure atomic operations
					when called in task context */
	tk_dis_dsp();
	if (tsd->t_stat.c.exec_fs != 0) {
		if (tsd->t_stat.c.exec_fimp != 0) {
			tsd->t_stat.c.break_called = 1;
			tk_ena_dsp();
			xfs_break_impl(tsd->t_con, tid, TRUE);
		} else {
			tsd->t_stat.c.break_done = 1;
			tk_ena_dsp();
		}
		return 1;
	}
	tk_ena_dsp();
	return 0;
}

/*
 *  fs_break() - Break function service
 */
EXPORT	ER	xfs_break(ID tid)
{
	INT	cnt, sts;

	cnt = sts = 0;

	if (tid == TSK_ALL) {	/* Break all tasks */
		for (tid = 1; sts >= 0; tid++) {
			if ((sts = xfs_break_tsk(tid)) > 0) cnt++;
		}
		sts = 0;
	} else {		/* Break specified task */
		if ((sts = fs_task_exist(tid)) == 0) {
			if ((sts = xfs_break_tsk(tid)) > 0) cnt++;
		}
	}
	return (sts >= 0) ? cnt : sts;
}

