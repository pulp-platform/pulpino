#ifndef _SPI_H_
#define _SPI_H_

#define SPI_QPI    1
#define SPI_NO_QPI 0

#define SPI_CMD_RD    0
#define SPI_CMD_WR    1
#define SPI_CMD_QRD   2
#define SPI_CMD_QWR   3
#define SPI_CMD_SWRST 4

#define SPI_CSN0 0
#define SPI_CSN1 1
#define SPI_CSN2 2
#define SPI_CSN3 3

#include <pulp.h>

#define SPI_REG_STATUS                ( SPI_BASE_ADDR + 0x00 )
#define SPI_REG_CLKDIV                ( SPI_BASE_ADDR + 0x04 )
#define SPI_REG_SPICMD                ( SPI_BASE_ADDR + 0x08 )
#define SPI_REG_SPIADR                ( SPI_BASE_ADDR + 0x0C )
#define SPI_REG_SPILEN                ( SPI_BASE_ADDR + 0x10 )
#define SPI_REG_SPIDUM                ( SPI_BASE_ADDR + 0x14 )
#define SPI_REG_TXFIFO                ( SPI_BASE_ADDR + 0x18 )
#define SPI_REG_RXFIFO                ( SPI_BASE_ADDR + 0x20 )
#define SPI_REG_INTCFG                ( SPI_BASE_ADDR + 0x24 )
#define SPI_REG_INTSTA                ( SPI_BASE_ADDR + 0x28 )

void spi_setup_slave();
void spi_setup_master(int numcs);

void spi_send_data_noaddr(int cmd, char *data, int datalen, int useQpi);

void spi_setup_cmd_addr(int cmd, int cmdlen, int addr, int addrlen);
void spi_set_datalen(int datalen);

void spi_setup_dummy(int dummy_rd, int dummy_wr);

void spi_start_transaction(int trans_type, int csnum);

void spi_write_fifo(int *data, int datalen);
void spi_read_fifo(int *data, int datalen);

int spi_get_status();

#endif // _SPI_H_
