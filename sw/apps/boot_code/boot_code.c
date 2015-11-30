// TO BE COMPILED ONLY ONCE


#include <spi.h>
#include <gpio.h>
#include <uart.h>
#include <utils.h>
#include <pulpino.h>

const char g_numbers[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void jump_and_start(volatile int *ptr)
{
#ifdef __riscv__
  asm("jalr x0, %0\n"
      "nop\n"
      "nop\n"
      "nop\n"
      : : "r" (ptr) );
#else
  asm("l.jr\t%0\n"
      "l.nop\n"
      "l.nop\n"
      "l.nop\n"
      : : "r" (ptr) );
#endif
}

int main()
{
  /* sets direction for SPI master pins with only one CS */
  spi_setup_master(1);

  /* divide sys clock by 4 */
  *(volatile int*) (SPI_REG_CLKDIV) = 0x4;

  /* wait some time to have proper power up of external flash */
  for (int i = 0; i < 1000; i++) __asm__ volatile ("nop\n");

  uart_send("Loading from SPI\n", 17);

  int vendor_id[2];
  //reads flash ID
  spi_setup_cmd_addr(0x9F, 8, 0, 0);
  spi_set_datalen(64);
  spi_setup_dummy(0, 0);
  spi_start_transaction(SPI_CMD_RD, SPI_CSN0);
  spi_read_fifo(vendor_id, 64);


  // id should be 0x0102194D
  if (((vendor_id[0] >> 24) & 0xFF) == 0x01) { // is vendor spansion?
    if ( (((vendor_id[0] >> 8) & 0xFFFF) == 0x0219) || (((vendor_id[0] >> 8) & 0xFFFF) == 0x2018) ) { // check flash model is 128MB or 256MB 1.8V

      // sends write enable command
      spi_setup_cmd_addr(0x06, 8, 0, 0);
      spi_set_datalen(0);
      spi_start_transaction(SPI_CMD_WR, SPI_CSN0);
      while ((spi_get_status() & 0xFFFF) != 1);

      // Enables QPI
      spi_setup_cmd_addr(0x71, 8, 0x80000348, 32); // cmd 0x71 write any register
      spi_set_datalen(0);
      spi_start_transaction(SPI_CMD_WR, SPI_CSN0);
      while ((spi_get_status() & 0xFFFF) != 1);

      int header_ptr[8];
      int addr = 0;
      // read header
      spi_setup_dummy(8, 0);

      spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
      spi_set_datalen(8 * 32);
      spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);
      spi_read_fifo(header_ptr, 8 * 32);


      int data_start = header_ptr[0];
      int *data = (int *) header_ptr[1];
      int data_size = header_ptr[2];
      int data_blocks = header_ptr[3];

      int instr_start = header_ptr[4];
      int *instr = (int *) header_ptr[5];
      int instr_size =  header_ptr[6];
      int instr_blocks = header_ptr[7];

      // Instruction RAM
      uart_send("Copying Instructions\n", 22);
      addr = data_start;
      spi_setup_dummy(8, 0);
      for (int i = 0; i < instr_blocks; i++) { //reads 16 4KB blocks
        spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
        spi_set_datalen(32768);
        spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);
        spi_read_fifo(instr, 32768);

        instr = instr + 0x400;  // new address = old address + 1024 words
        addr   = addr   + 0x1000; // new address = old address + 4KB

        uart_send("Block ", 6);
        uart_send(&g_numbers[i], 1);
        uart_send(" done\n", 6);
        // waiting for uart to be done
        uart_wait_tx_done();
      }

      while ((spi_get_status() & 0xFFFF) != 1);

      // Read Data RAM
      uart_send("Copying Data\n", 14);
      addr = data_start;
      spi_setup_dummy(8, 0);
      for (int i = 0; i < data_blocks; i++) { //reads 16 4KB blocks
        spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
        spi_set_datalen(32768);
        spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);
        spi_read_fifo(data, 32768);

        data = data + 0x400;  // new address = old address + 1024 words
        addr   = addr   + 0x1000; // new address = old address + 4KB

        uart_send("Block ", 6);
        uart_send(&g_numbers[i], 1);
        uart_send(" done\n", 6);
        // waiting for uart to be done
        uart_wait_tx_done();
      }
    } else {
      uart_send("ERROR: Spansion SPI flash not found\n", 36);
    }
  } else {
      uart_send("ERROR: Spansion SPI flash not found\n", 36);
  }

  uart_send("Done, disable and flush I$, jumping to L2\n", 42);

  uart_wait_tx_done();

  //jump to program start address (L2 base address)
  jump_and_start((volatile int *)(ISNTR_RAM_BASE_ADDR));
}