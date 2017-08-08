// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


#include <utils.h>
#include <stdio.h>
#include <spi.h>
#include <bench.h>

#define CMD_PP                          0x02    // Page program
#define CMD_RDSR1                       0x05    // read status register 1
#define CMD_WREN                        0x06    // Write Enable
#define CMD_P4E                         0x20    // Parameter 4-kB erase
#define CMD_SE                          0xD8    // Uniform Sector Erase (64kB or 256kB)
#define CMD_WRAR                        0x71    // Write to any register
#define CMD_RDCR                        0x35    // read configuration register
#define CMD_4READ                       0x13    // read with 4 byte address
#define CMD_4QIOR                       0xEC    // read with 4 byte address, QPI


void flash_write_qpi(unsigned int address, int* buffer, unsigned int size);
int flash_get_wip_qpi();
int flash_get_cr1();
void flash_sector_erase_parameter(unsigned int address);
void flash_sector_erase_uniform(unsigned int address);
void flash_read_qpi(unsigned int address, int* buffer, unsigned int size);

void check_standard_mode(testresult_t *result, void (*start)(), void (*stop)());
void check_qpi_mode(testresult_t *result, void (*start)(), void (*stop)());
void flash_check(testresult_t *result, unsigned int size_initial, unsigned int increase);

testcase_t testcases[] = {
  { .name = "SPI Master Standard Mode",   .test = check_standard_mode       },
  { .name = "SPI Master QPI Mode",        .test = check_qpi_mode            },
  {0, 0}
};

int main() {
  return run_suite(testcases);
}

void check_standard_mode(testresult_t *result, void (*start)(), void (*stop)()) {
  volatile int k;

  start();

  int id;

  result->errors = 0;

  // waste some time and wait for flash to power up
  for (int i = 0; i < 33333; i++) k = 0;

  *(volatile int*) (SPI_REG_CLKDIV) = 0x4;

  spi_setup_master(1); //sets direction for SPI master pins with only one CS

  //reads flash ID
  spi_setup_cmd_addr(0x9F, 8, 0, 0);
  spi_set_datalen(32);
  spi_setup_dummy(0, 0);
  spi_start_transaction(SPI_CMD_RD, SPI_CSN0);
  spi_read_fifo(&id, 32);

  if (id != 0x0102194D)
    result->errors++;

  printf("ID: %X\n", (unsigned int) id);

  //sends write enable command
  spi_setup_cmd_addr(0x06, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_WR, SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  stop();
}

void check_qpi_mode(testresult_t *result, void (*start)(), void (*stop)()) {
  int id;
  int i;
  unsigned int flash_addr = 0;
  char buffer[512];

  start();

  *(volatile int*) (SPI_REG_CLKDIV) = 0x4;

  //enables QPI
  spi_setup_cmd_addr(0x71, 8, 0x80000348, 32);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_WR, SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  //reads ID in QPI mode
  spi_setup_cmd_addr(0x9F, 8, 0, 0);
  spi_set_datalen(32);
  spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);
  spi_read_fifo(&id, 32);

  if (id != 0x0102194D)
    result->errors++;

  printf("ID: %X\n", (unsigned int) id);

  //sends write enable command
  spi_setup_cmd_addr(0x06, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR, SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  //disables QPI
  spi_setup_cmd_addr(0x71, 8, 0x80000308, 32);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR, SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);


  // check where parameter sectors are
  int cr1 = flash_get_cr1();
  if((cr1 & (1 << 2)) != 0) {
    printf("Spansion flash reports that parameter sectors are at the top, this program was written for parameter sectors at the bottom, exiting\n");
    result->errors++;
    return;
  }

  //-----------------------------------------------------------------------//
  // enable QPI mode                                                       //
  //-----------------------------------------------------------------------//

  //sends write enable command
  spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_WR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  // send WRAR
  spi_setup_cmd_addr(CMD_WRAR, 8, (0x800003 << 8) | 0x48, 32);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_WR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  //-----------------------------------------------------------------------//
  // set page size of the flash to 512 bytes                               //
  //-----------------------------------------------------------------------//

  //sends write enable command
  spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  // send WRAR
  spi_setup_cmd_addr(CMD_WRAR, 8, (0x800004 << 8) | 0x10, 32);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);


  //---------------------------------------------------------------------//
  // check if a sector erase is needed and perform it
  //---------------------------------------------------------------------//
  for(i = 0; i < 512; i++)
    buffer[i] = i;

  flash_sector_erase_parameter(flash_addr);

  flash_write_qpi(flash_addr, (int*)buffer, 512);

  //---------------------------------------------------------------------//
  // read back data from flash
  //---------------------------------------------------------------------//

  // check if a write is in progress and wait until it ends
  while(flash_get_wip_qpi());

  flash_read_qpi(0, buffer, 512);
  for(i = 0; i < 512; i++) {
    if (buffer[i] != (i & 0xFF)) {
      printf ("Content of flash memory is incorrect at index %d: got %X, expected %X\n", i, buffer[i], i);
      result->errors++;
    }
  }

  flash_check(result, 12, 4);

  flash_check(result, 24, 12);

  flash_check(result, 4, 4);

  stop();
}

//----------------------------------------------------------------------------//
// Writes to spansion spi flash
// address and size must be in bytes
//----------------------------------------------------------------------------//
void flash_write_qpi(unsigned int address, int* buffer, unsigned int size) {
  // check if a write is in progress and wait until it ends
  while(flash_get_wip_qpi());

  //-------------------------------------------------------------------------//
  // sends write enable command
  //-------------------------------------------------------------------------//
  spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  //-------------------------------------------------------------------------//
  // write to flash
  //-------------------------------------------------------------------------//

  spi_setup_cmd_addr(CMD_PP, 8, address << 8, 24);
  spi_set_datalen(size * 8);

  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);

  spi_write_fifo(buffer, size * 8);

  while ((spi_get_status() & 0xFFFF) != 1);
}

//----------------------------------------------------------------------------//
// Checks if the WIP flag is set in the flash
// The WIP flags indicates if a page program or erase operation is currently
// running
//----------------------------------------------------------------------------//
int flash_get_wip_qpi() {
  // check WIP
  int status;
  while ((spi_get_status() & 0xFFFF) != 1);
  spi_setup_cmd_addr(CMD_RDSR1, 8, 0, 0);
  spi_set_datalen(8);
  spi_start_transaction(SPI_CMD_QRD,SPI_CSN0);
  spi_read_fifo(&status, 8);
  return status & 0x1;
}

//----------------------------------------------------------------------------//
// Gets the configuration register 1 contents
//----------------------------------------------------------------------------//
int flash_get_cr1() {
  int status;
  while ((spi_get_status() & 0xFFFF) != 1);
  spi_setup_cmd_addr(CMD_RDCR, 8, 0, 0);
  spi_set_datalen(8);
  spi_start_transaction(SPI_CMD_RD,SPI_CSN0);
  spi_read_fifo(&status, 8);
  return status;
}

//----------------------------------------------------------------------------//
// parameter sector erase
// There are 8 special sectors with 4 kB each in this flash, those sectors are
// at the top, the bottom or not in the address space
// In order to erase one of those sectors, a parameter erase must be used
//----------------------------------------------------------------------------//
void flash_sector_erase_parameter(unsigned int address) {
  // check if a write/erase is in progress and wait until it ends
  while(flash_get_wip_qpi());

  //sends write enable command
  spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  // send P4E
  spi_setup_cmd_addr(CMD_P4E, 8, (address << 8), 24);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);
}

void flash_sector_erase_uniform(unsigned int address) {
  // check if a write/erase is in progress and wait until it ends
  while(flash_get_wip_qpi());

  //sends write enable command
  spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);

  // send P4E
  spi_setup_cmd_addr(CMD_SE, 8, (address << 8), 24);
  spi_set_datalen(0);
  spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
  while ((spi_get_status() & 0xFFFF) != 1);
}


//----------------------------------------------------------------------------//
// Read data from flash
// size is in bytes
//----------------------------------------------------------------------------//
void flash_read_qpi(unsigned int address, int* buffer, unsigned int size) {
  int tmp;
  int i;
  while ((spi_get_status() & 0xFFFF) != 1);
  spi_setup_cmd_addr(CMD_4QIOR, 8, address, 32);
  spi_setup_dummy(10, 0);
  spi_set_datalen(size * 8);
  spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);
  spi_read_fifo(buffer, size * 8);
}

void waste_time() {
  int i;
  volatile int k = 0;
  for(i = 0; i < 500; i++) k = 0;
}

void flash_check(testresult_t *result, unsigned int size_initial, unsigned int increase) {
  unsigned int flash_addr;
  unsigned int address;
  char buffer[512];
  int i;

  address = 0;
  flash_addr = 0;

  //---------------------------------------------------------------------//
  // check if a sector erase is needed and perform it
  //---------------------------------------------------------------------//
  for(i = 0; i < 512; i++)
    buffer[i] = i;

  flash_sector_erase_parameter(flash_addr);

  {
    unsigned int address = 0;
    unsigned int size = 512;
    unsigned int size_int;
    // check if a write is in progress and wait until it ends
    while(flash_get_wip_qpi());

    //-------------------------------------------------------------------------//
    // sends write enable command
    //-------------------------------------------------------------------------//
    spi_setup_cmd_addr(CMD_WREN, 8, 0, 0);
    spi_set_datalen(0);
    spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);
    while ((spi_get_status() & 0xFFFF) != 1);

    //-------------------------------------------------------------------------//
    // write to flash
    //-------------------------------------------------------------------------//

    spi_setup_cmd_addr(CMD_PP, 8, address << 8, 24);
    spi_set_datalen(size * 8);

    spi_start_transaction(SPI_CMD_QWR,SPI_CSN0);

    size_int = size_initial;
    for (int j = 0; j < size; j += size_int) {
      waste_time();

      size_int = (size - j) > (size_int + increase) ? (size_int + increase) : (size - j);

      spi_write_fifo(&buffer[j], size_int * 8);
    }

    while ((spi_get_status() & 0xFFFF) != 1);
  }

  //---------------------------------------------------------------------//
  // read back data from flash
  //---------------------------------------------------------------------//

  // check if a write is in progress and wait until it ends
  while(flash_get_wip_qpi());

  {
    unsigned int address = 0;
    unsigned int size = 512;
    unsigned int size_int;
    int tmp;

    while ((spi_get_status() & 0xFFFF) != 1);

    spi_setup_cmd_addr(CMD_4QIOR, 8, address, 32);
    spi_setup_dummy(10, 0);
    spi_set_datalen(size * 8);
    spi_start_transaction(SPI_CMD_QRD, SPI_CSN0);

    size_int = size_initial;
    for (int j = 0; j < size; j += size_int) {
      waste_time();

      size_int = (size - j) > (size_int + increase) ? (size_int + increase) : (size - j);

      spi_read_fifo(&buffer[j], size_int * 8);
    }
  }

  for(i = 0; i < 512; i++) {
    if (buffer[i] != (i & 0xFF)) {
      printf ("Content of flash memory is incorrect at index %d: got %X, expected %X\n", i, buffer[i], i);
      result->errors++;
    }
  }
}
