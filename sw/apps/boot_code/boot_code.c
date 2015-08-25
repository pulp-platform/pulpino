// TO BE COMPILED ONLY ONCE


#include <spi.h>
#include <gpio.h>
#include <uart.h>
#include <icache_ctrl.h>
#include <utils.h>


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
  volatile int * ptr;
  volatile int i;
  int addr;
  int rd_id[2];
  int err=0;

  int *l2_ptr;
  int *tcdm_ptr;
  int header_ptr[8];
  int l2_size;
  int tcdm_size;
  int l2_blocks;
  int tcdm_blocks;
  int l2_start;
  int tcdm_start;


  if(get_core_id() == 0) {

    spi_setup_master(1); //sets direction for SPI master pins with only one CS

    *(volatile int*) (SPI_REG_CLKDIV) = 0x4;//divide sys clock by 4

    // setup UART
    uart_set_cfg(0, 26);

    for (i=0; i<1000; i++); //wait some time to have proper power up of external flash

    uart_send("Loading from SPI\n", 17);

    //reads flash ID
    spi_setup_cmd_addr(0x9F, 8, 0, 0);
    spi_set_datalen(64);
    spi_setup_dummy(0,0);
    spi_start_transaction(SPI_CMD_RD, SPI_CSN0);
    spi_read_fifo(rd_id, 64);


    // id should be 0x0102194D
    if (((rd_id[0] >> 24) & 0xFF) == 0x01) { // is vendor spansion?
      if ( (((rd_id[0] >> 8) & 0xFFFF) == 0x0219) || (((rd_id[0] >> 8) & 0xFFFF) == 0x2018) ) { // check flash model is 128MB or 256MB 1.8V
        //sends write enable command
        spi_setup_cmd_addr(0x06, 8, 0, 0);
        spi_set_datalen(0);
        spi_start_transaction(SPI_CMD_WR,SPI_CSN0);
        while ((spi_get_status() & 0xFFFF) != 1);

        //enables QPI
        spi_setup_cmd_addr(0x71, 8, 0x80000348, 32); // cmd 0x71 write any register
        spi_set_datalen(0);
        spi_start_transaction(SPI_CMD_WR,SPI_CSN0);
        while ((spi_get_status() & 0xFFFF) != 1);


        addr = 0;
        // read header
        spi_setup_dummy(8,0);

        spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
        spi_set_datalen(8*32);
        spi_start_transaction(SPI_CMD_QRD,SPI_CSN0);
        spi_read_fifo(header_ptr, 8*32);

        l2_start = header_ptr[0];
        l2_ptr = (int *) header_ptr[1];
        l2_size = header_ptr[2];
        l2_blocks = header_ptr[3];

        tcdm_start = header_ptr[4];
        tcdm_ptr = (int *) header_ptr[5];
        tcdm_size =  header_ptr[6];
        tcdm_blocks = header_ptr[7];

        // read clock divider values for uart etc.

        /* // read TCDM */
        uart_send("Copy TCDM\n", 10);
        addr = tcdm_start;
        spi_setup_dummy(8,0);
        for (i=0;i<tcdm_blocks;i++) { //reads 16 4KB blocks
          spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
          spi_set_datalen(32768);
          spi_start_transaction(SPI_CMD_QRD,SPI_CSN0);
          spi_read_fifo(tcdm_ptr, 32768);

          tcdm_ptr = tcdm_ptr + 0x400;  // new address = old address + 1024 words
          addr   = addr   + 0x1000; // new address = old address + 4KB

          uart_send("Block ", 6);
          uart_send(&g_numbers[i], 1);
          uart_send(" done\n", 6);
          // waiting for uart to be done
          while( (*(volatile int*)(UART_REG_LSR) & (0x01 << 6)) == 0x00);
        }

        while ((spi_get_status() & 0xFFFF) != 1);

        // read L2
        uart_send("Copy L2\n", 8);
        addr = l2_start;
        spi_setup_dummy(8,0);
        for (i=0;i<l2_blocks;i++) { //reads 16 4KB blocks
          spi_setup_cmd_addr(0xEB, 8, ((addr << 8) & 0xFFFFFF00), 32); // cmd 0xEB fast read, needs 8 dummy cycles
          spi_set_datalen(32768);
          spi_start_transaction(SPI_CMD_QRD,SPI_CSN0);
          spi_read_fifo(l2_ptr, 32768);

          l2_ptr = l2_ptr + 0x400;  // new address = old address + 1024 words
          addr   = addr   + 0x1000; // new address = old address + 4KB

          uart_send("Block ", 6);
          uart_send(&g_numbers[i], 1);
          uart_send(" done\n", 6);
          // waiting for uart to be done
          while( (*(volatile int*)(UART_REG_LSR) & (0x01 << 6)) == 0x00);
        }
      } else {
        err++;
      }
    } else {
      err++;
    }

    if(err != 0) {
      uart_send("ERROR: Spansion SPI flash not found\n",36);
    }

    uart_send("Done, disable and flush I$, jumping to L2\n",42);

    uart_wait_tx_done();

    // deactivate // will be invalidated when it is re-enabled
    disable_all_icache_banks();

    //jump to program start address (L2 base address)
    jump_and_start((volatile int *)(L2_MEM_BASE_ADDR));
  }
}


// overwrite interrupt, exception handlers etc.

void default_exception_handler_c() {
  uart_send("exception!\n",11);
  uart_wait_tx_done();
}

void int_main() {
  uart_send("interrupt!\n",11);
  uart_wait_tx_done();
}

