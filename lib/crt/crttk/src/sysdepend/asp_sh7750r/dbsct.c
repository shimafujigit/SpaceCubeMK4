/*
 *----------------------------------------------------------------------
 *    T-Kernel 2.0 Software Package
 *
 *    Copyright 2011 by Ken Sakamura.
 *    This software is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Released by T-Engine Forum(http://www.t-engine.org/) at 2011/05/17.
 *
 *----------------------------------------------------------------------
 *    Changes: Adapted to the ASP-SH7750R Board.
 *    Changed by UC Technology at 2012/12/20.
 *    
 *    UCT T-Kernel 2.0 DevKit tuned for SH7750R Version 1.00.00
 *    Copyright (c) 2012 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#pragma section $DSEC
const struct {
	char *rom_s;       /* Start address of the initialized data section in ROM */
	char *rom_e;       /* End address of the initialized data section in ROM   */
	char *ram_s;       /* Start address of the initialized data section in RAM */
	char *ram_e;       /* End address of the initialized data section in RAM   */
}DTBL[]= {
	{__sectop("D"), __secend("D"), __sectop("R"), __secend("R")},
};
#pragma section $BSEC
const struct {
	char *b_s;         /* Start address of non-initialized data section */
	char *b_e;         /* End address of non-initialized data section */
}BTBL[]= {
	{__sectop("B"), __secend("B")},
};
