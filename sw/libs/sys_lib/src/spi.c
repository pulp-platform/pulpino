// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <spi.h>
#include <gpio.h>

void spi_setup_slave() {
    set_pin_function(PIN_SSPI_SIO0, FUNC_SPI);
    set_pin_function(PIN_SSPI_SIO1, FUNC_SPI);
    set_pin_function(PIN_SSPI_SIO2, FUNC_SPI);
    set_pin_function(PIN_SSPI_SIO3, FUNC_SPI);
    set_pin_function(PIN_SSPI_CSN, FUNC_SPI);
}

void spi_setup_master(int numcs) {
    set_pin_function(PIN_MSPI_SIO0, FUNC_SPI);
    set_pin_function(PIN_MSPI_SIO1, FUNC_SPI);
    set_pin_function(PIN_MSPI_SIO2, FUNC_SPI);
    set_pin_function(PIN_MSPI_SIO3, FUNC_SPI);
    if (numcs > 0)
        set_pin_function(PIN_MSPI_CSN0, FUNC_SPI);
    if (numcs > 1)
        set_pin_function(PIN_MSPI_CSN1, FUNC_SPI);
    if (numcs > 2)
        set_pin_function(PIN_MSPI_CSN2, FUNC_SPI);
    if (numcs > 3)
        set_pin_function(PIN_MSPI_CSN3, FUNC_SPI);
}

void spi_send_data_noaddr(int cmd, char *data, int datalen, int useQpi);

void spi_setup_cmd_addr(int cmd, int cmdlen, int addr, int addrlen) {
    int cmd_reg;
    cmd_reg = cmd << (32 - cmdlen);
    *(volatile int*) (SPI_REG_SPICMD) = cmd_reg;
    *(volatile int*) (SPI_REG_SPIADR) = addr;
    *(volatile int*) (SPI_REG_SPILEN) = (cmdlen & 0x3F) | ((addrlen << 8) & 0x3F00);
}

void spi_setup_dummy(int dummy_rd, int dummy_wr) {
    *(volatile int*) (SPI_REG_SPIDUM) = ((dummy_wr << 16) & 0xFFFF0000) | (dummy_rd & 0xFFFF);
}

void spi_set_datalen(int datalen) {
    volatile int old_len;
    old_len = *(volatile int*) (SPI_REG_SPILEN);
    old_len = ((datalen << 16) & 0xFFFF0000) | (old_len & 0xFFFF);
    *(volatile int*) (SPI_REG_SPILEN) = old_len;
}

void spi_start_transaction(int trans_type, int csnum) {
    *(volatile int*) (SPI_REG_STATUS) = ((1 << (csnum + 8)) & 0xF00) | ((1 << trans_type) & 0xFF);
}

int spi_get_status() {
    volatile int status;
    status = *(volatile int*) (SPI_REG_STATUS);
    return status;
}

void spi_write_fifo(int *data, int datalen) {
    volatile int num_words, i;

    num_words = (datalen >> 5) & 0x7FF;

    if ( (datalen & 0x1F) != 0)
        num_words++;

    for (i = 0; i < num_words; i++) {
        while ((((*(volatile int*) (SPI_REG_STATUS)) >> 24) & 0xFF) >= 8);
        *(volatile int*) (SPI_REG_TXFIFO) = data[i];
    }
}

void spi_read_fifo(int *data, int datalen) {
    volatile int num_words, i;

    num_words = (datalen >> 5) & 0x7FF;

    if ( (datalen & 0x1F) != 0)
        num_words++;

    for (i = 0; i < num_words; i++) {
        while ((((*(volatile int*) (SPI_REG_STATUS)) >> 16) & 0xFF) == 0);
        data[i] = *(volatile int*) (SPI_REG_RXFIFO);
    }
}


