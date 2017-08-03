// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


#include <i2c.h>
#include <utils.h>
#include <bench.h>
#include <stdio.h>

#define BAUD_RATE                       100000

#define I2C_PRESCALER 0x63 //(soc_freq/(5*i2cfreq))-1 with i2cfreq = 100Khz

void check(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "i2c",   .test = check       },
  {0, 0}
};

int main() {
  return run_suite(testcases);
}

void check(testresult_t *result, void (*start)(), void (*stop)()) {
  int value;

  result->errors = 0;
  value = 0;

  i2c_setup(I2C_PRESCALER, I2C_CTR_EN);

  i2c_send_data(0xA0); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn

  i2c_send_command(I2C_START_WRITE); //do a start bit and send data

  i2c_get_ack();

  i2c_send_data(0x00); //addr MSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  i2c_get_ack();

  i2c_send_data(0x00); //addr LSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  i2c_get_ack();

  for (int i = 0; i < 16; i++) {
    i2c_send_data(i);            //write i-th byte into fifo
    i2c_send_command(I2C_WRITE); //send data on the i2c bus
    i2c_get_ack();               //wait for ack
  }

  i2c_send_command(I2C_STOP);      //do a stop bit, initiate eeprom write
  while(i2c_busy());

  // acknowledge polling
  do {
    i2c_send_data(0xA0);
    i2c_send_command(I2C_START_WRITE);
  } while (!i2c_get_ack());

  i2c_send_command(I2C_STOP);      //do a stop bit, finish acknowledge polling transfer
  while(i2c_busy());


  //---------------------------------------------------------------------------
  // read back data
  //---------------------------------------------------------------------------
  i2c_send_data(0xA0); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
  i2c_send_command(I2C_START_WRITE); //do a start bit and send data

  if(!i2c_get_ack()) {
    printf("No ack received from EEPROM for readback command\n");
    result->errors++;
    return;
  }

  i2c_send_data(0x00); //addr MSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  if(!i2c_get_ack()) {
    printf("No ack received from EEPROM for addr high\n");
    result->errors++;
    return;
  }

  i2c_send_data(0x00); //addr LSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  if(!i2c_get_ack()) {
    printf("No ack received from EEPROM for addr low\n");
    result->errors++;
    return;
  }

  i2c_send_command(I2C_STOP);      //do a stop bit, initiate eeprom write
  while(i2c_busy());


  i2c_send_data(0xA1); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
  i2c_send_command(I2C_START_WRITE); //do a start bit and send data

  if(!i2c_get_ack()) {
    printf("No ack received from EEPROM before sending read\n");
    result->errors++;
    return;
  }

  for (int i = 0; i < 16; i++) {
    if (i == 15)
      i2c_send_command(I2C_STOP_READ); //do a stop bit and read data
    else
      i2c_send_command(I2C_READ); //do a start bit and read data

    if (!i2c_get_ack()) {
      printf("No ack received from EEPROM for byte %d\n", i);
      result->errors++;
      return;
    }

    value = i2c_get_data();
    printf("Received %d expecting %d\n", value, i);
    if (value != i) {
      result->errors++;
    }
  }
}
