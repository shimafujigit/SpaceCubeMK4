/*
 *----------------------------------------------------------------------
 *    UCT T2AS DevKit tuned for LEON5
 *
 *    Copyright (c) 2021 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Version:	 1.00.00
 *    Released by UC Technology (http://www.uctec.com/) at 2021/10/28.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.c
 */

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>
#include <sys/sysinfo.h>

#include <bcc/bcc.h>
#include <bcc/bcc_param.h>
#include <bcc/regs/gptimer.h>
#include <drv/gpio.h>

/* monitor.c */
IMPORT	W	put_string( B *str );

/*
 *	Wait usec
 */
EXPORT	void	wait_usec( W usec )
{
	UW	clk;
	UW	i;

	if ( __bcc_timer_handle == 0 ) {
		clk = 100; /* MHz */
	} else {
		clk = ((struct gptimer_regs*)__bcc_timer_handle)
			->scaler_reload + 1;
	}

#define	LOOP_CYCLE	3	/* CPU cycles for loop */
#define	LOOP_us(n)	( clk * (n) / LOOP_CYCLE + 1 )

	for ( i = LOOP_us(usec); i != 0; i-- ) Asm("");
}

/*
 *	Initailize Hardware
 */
EXPORT	void	init_hw( void )
{
	extern void monitor_svc_handler( void ); /* monent.S */

	bcc_set_trap(EIT_TRAP(TRAP_MONITOR), monitor_svc_handler);

	gpio_autoinit();
	if ( gpio_dev_count() > 0 ) {
		struct gpio_priv *priv;
		UW const ledbit = 0xf << 16; /* GPIO 16-19 */
		priv = gpio_open(0);
		gpio_output_and(priv, ~ledbit);
		gpio_direction_or(priv, ledbit);
		gpio_close(priv);
	}
}

/*
 *	LED control
 *	val: 0xMMMMVVVV	 M:mask (1:change, 0:Unchange), V:value (1:on, 0:off)
 *	return current led status
 */
EXPORT	INT	ctrl_led( UW val )
{
	struct gpio_priv *priv;
	UW	msk, sts;

	if ( gpio_dev_count() == 0 ) return 0;

	priv = gpio_open(0);

	/* LED	GPIO 16-19 (1:on, 0:off) */
	sts = gpio_output(priv, 0, 0);
	msk = ((val >> 16) & 0x0f) << 16;
	sts &= ~msk;
	sts |= (val << 16) & msk;
	gpio_output(priv, 1, sts);

	gpio_close(priv);

	return (sts >> 16) & 0x0f;	/* Return current led status */
}

/*
 *	System Halt ( no return )
 */
EXPORT	void	system_halt( void )
{
	put_string("-- system halt --\n");

	for ( ;; ) bcc_power_down();
}
