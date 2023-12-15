#ifndef _SPCMK4_SPI_H_
#define _SPCMK4_SPI_H_

#define fpga_reg_write( addr,data )	( *((_UW *)addr) = (_UW)data )
#define fpga_reg_read( addr )		( *((_UW *)addr) )

/* SPIクロック設定一覧 */
#define SPI_CLK_25MHZ			( 0x00 )
#define SPI_CLK_12_5MHZ			( 0x11 )
#define SPI_CLK_8_33MHZ			( 0x22 )
#define SPI_CLK_6_25MHZ			( 0x33 )
#define SPI_CLK_5MHZ			( 0x44 )
#define SPI_CLK_4_17MHZ			( 0x55 )
#define SPI_CLK_3_57MHZ			( 0x66 )
#define SPI_CLK_3_125MHZ		( 0x77 )
#define SPI_CLK_2_78MHZ			( 0x88 )
#define SPI_CLK_2_5MHZ			( 0x99 )
#define SPI_CLK_2_273MHZ		( 0xAA )
#define SPI_CLK_2_083MHZ		( 0xBB )
#define SPI_CLK_1_923MHZ		( 0xCC )
#define SPI_CLK_1_786MHZ		( 0xDD )
#define SPI_CLK_1_667MHZ		( 0xEE )
#define SPI_CLK_1_563MHZ		( 0xFF )

/* SPIレジスタアドレス */
#define ADDR_SPI_SENDDRAM		( 0x80000B00 )
#define ADDR_SPI_RECIVEDRAM		( 0x80000B04 )
#define ADDR_SPI_CMD_STAT		( 0x80000B08 )
#define ADDR_SPI_CLK			( 0x80000B0C )
#define ADDR_SPI_INT_STAT		( 0x80000B10 )
#define ADDR_SPI_INT_CTRL		( 0x80000B14 )
#define ADDR_SPI_GPIO_DATA		( 0x80000B18 )
#define ADDR_SPI_GPIO_DIR		( 0x80000B1C )

#define SPI_DMA_START			( 0xC0000000 )

#define SPI_GPIO1_HIGH			( 0x00000002 )
#define SPI_GPIO1_LOW			( 0x00000000 )

#define MSK_V2MDRDY				( 0x00000001 )


void spi_init(void);
void spi_SendRecive(char * send_addr, char * rcv_addr, UINT len);

#endif
