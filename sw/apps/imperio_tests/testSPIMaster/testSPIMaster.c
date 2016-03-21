// Copyright 2015 ETH Zurich and University of Bologna.
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

void check_standard_mode(testresult_t *result, void (*start)(), void (*stop)());
void check_qpi_mode(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "SPI Master Standard Mode",   .test = check_standard_mode       },
  { .name = "SPI Master QPI Mode",        .test = check_qpi_mode            },
  {0, 0}
};

int main() {
  run_suite(testcases);
  return 0;
}

void check_standard_mode(testresult_t *result, void (*start)(), void (*stop)()) {

    start();

    int id;

    result->errors = 0;

    // waste some time and wait for flash to power up
    // asm volatile is needed so that this loop is not optimized out
    for (int i = 0; i < 33333; i++) {
#ifdef __riscv__
      __asm__ volatile ("nop");
#else
      __asm__ volatile ("l.nop");
#endif
    }

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

    start();
    int id;
    result->errors = 0;

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

    stop();
}
