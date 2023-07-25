#include <basic.h>

/* Network Driver (device type, I/O address, IRQ) */
/*	device type  = 0x00011	MICROCHIP LAN9221 */
/*	device type |= 0x00800	always power-on */
/*	device type |= 0x10000	full-duplex mode */
EXPORT const UB DEVCONF [] = {
	"DEBUGMODE	1\n"
	"NETDEV0	0x10011 0x81000000 9\n"
};
