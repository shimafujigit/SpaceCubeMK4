#include <basic.h>
#include <tk/tkernel.h>
#include "spcmk4_spi.h"

void spi_init(void)
{

	fpga_reg_write(ADDR_SPI_CLK, SPI_CLK_5MHZ);		// SPI通信クロック設定
	fpga_reg_write(ADDR_SPI_GPIO_DIR, 0x00000001);	// GPIOの入出力方向を設定
}

void spi_SendRecive(char * send_addr, char * rcv_addr, UINT len)
{
	_UW res = 0;

	fpga_reg_write(ADDR_SPI_SENDDRAM, send_addr);
	fpga_reg_write(ADDR_SPI_RECIVEDRAM, rcv_addr);					// 受信データアドレスを設定

	fpga_reg_write(ADDR_SPI_GPIO_DATA, 0x00000004);				// GPIO1 = High
	while((fpga_reg_read(ADDR_SPI_GPIO_DATA) & MSK_V2MDRDY ) == 0);	// V2M_Drdy待ち
	fpga_reg_write(ADDR_SPI_CMD_STAT, (SPI_DMA_START | len));		// SPI通信開始
	fpga_reg_write(ADDR_SPI_GPIO_DATA, SPI_GPIO1_LOW);				// GPIO1 = Low

//	while(((fpga_reg_read(ADDR_SPI_GPIO_DATA) & MSK_V2MDRDY) == 1); // SPI通信完了待ち

}
