// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <i2c.h>

void i2c_setup(int prescaler,int enable) {
    *(volatile int*) (I2C_REG_PRE) = prescaler;
    *(volatile int*) (I2C_REG_CTR) = enable;
}

int i2c_get_ack(void) {
    while ((i2c_get_status() & I2C_STATUS_TIP) == 0); // need TIP go to 1
    while ((i2c_get_status() & I2C_STATUS_TIP) != 0); // and then go back to 0
    return !(i2c_get_status() & I2C_STATUS_RXACK); // invert since signal is active low
}

void i2c_send_data(int value) {
    *(volatile int*) (I2C_REG_TX) = value;
}

void i2c_send_command(int value) {
    *(volatile int*) (I2C_REG_CMD) = value;
}

int i2c_get_status() {
  volatile int status;
  status = *(volatile int*) (I2C_REG_STATUS);
  return status;
}

int i2c_get_data() {
  volatile int status;
  status = *(volatile int*) (I2C_REG_RX);
  return status;
}

int i2c_busy(void) {
    return ((i2c_get_status() & I2C_STATUS_BUSY) == I2C_STATUS_BUSY);
}
