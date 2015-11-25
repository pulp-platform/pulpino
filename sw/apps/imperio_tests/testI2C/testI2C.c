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
  run_suite(testcases);
  return 0;
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

  for (int i = 0; i < 100; i++) __asm__ volatile ("nop\n"); // wait some time
  // acknowledge polling
  do {
    i2c_send_data(0xA0);
    i2c_send_command(I2C_START_WRITE);
  } while (i2c_get_ack());

  // read back data
  i2c_send_data(0xA0); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn

  i2c_send_command(I2C_START_WRITE); //do a start bit and send data

  i2c_get_ack();

  i2c_send_data(0x00); //addr MSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  i2c_get_ack();

  i2c_send_data(0x00); //addr LSBs
  i2c_send_command(I2C_WRITE); //do a start bit and send data

  i2c_get_ack();

  for (int i = 0; i < 100; i++) __asm__ volatile ("nop\n"); // wait some time

  i2c_send_data(0xA1); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
  i2c_send_command(I2C_START_WRITE); //do a start bit and send data

  i2c_get_ack();

  for (int i = 0; i < 16; i++) {
    i2c_send_command(I2C_READ); //do a start bit and send data
    i2c_get_ack();
    value = i2c_get_data();
    printf("Received %d expecting %d\n", value, i);
    if (value != i) {
      result->errors++;
    }
  }

  i2c_send_command(I2C_STOP); //do a stop bit
}


