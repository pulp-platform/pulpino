#include <i2c.h>
#include "utils.h"

#define BAUD_RATE                       100000
#define SOC_FREQ                        100
#define CLUSTER_FREQ                    100

#define I2C_PRESCALER 0xC7 //(soc_freq/(5*i2cfreq))-1 with i2cfreq = 100Khz

int main()
{
  volatile int * ptr_antonio;
  int value;
  int error;

  if(get_core_id() == 0) {
    error = 0;
    value = 0;

    set_soc_frequency(SOC_FREQ);
    set_cluster_frequency(CLUSTER_FREQ);

    i2c_setup(I2C_PRESCALER,I2C_CTR_EN);

    i2c_send_data(0xA0); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn

    i2c_send_command(I2C_START_WRITE); //do a start bit and send data

    i2c_get_ack();

    i2c_send_data(0x00); //addr MSBs
    i2c_send_command(I2C_WRITE); //do a start bit and send data

    i2c_get_ack();

    i2c_send_data(0x00); //addr LSBs
    i2c_send_command(I2C_WRITE); //do a start bit and send data

    i2c_get_ack();

    for (int i=0;i<16;i++) {
        i2c_send_data(i);            //write i-th byte into fifo
        i2c_send_command(I2C_WRITE); //send data on the i2c bus
        i2c_get_ack();               //wait for ack
    }

    i2c_send_command(I2C_STOP);      //do a stop bit


    for(int i=0;i<1000;i++) i2c_get_status(); //wait some time using dummy calls if not compiler removes the delay


    //Read back data
    i2c_send_data(0xA0); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn

    i2c_send_command(I2C_START_WRITE); //do a start bit and send data

    i2c_get_ack();

    i2c_send_data(0x00); //addr MSBs
    i2c_send_command(I2C_WRITE); //do a start bit and send data

    i2c_get_ack();

    i2c_send_data(0x00); //addr LSBs
    i2c_send_command(I2C_WRITE); //do a start bit and send data

    i2c_get_ack();

    for(int i=0;i<100;i++) i2c_get_status(); //wait some time

    i2c_send_data(0xA1); // write to EEprom with A0,A1=1 1010 B0 A1 A0 R/Wn
    i2c_send_command(I2C_START_WRITE); //do a start bit and send data

    i2c_get_ack();

    i2c_send_command(I2C_READ); //do a start bit and send data

    //i2c_get_ack();
    for (int i=0;i<16;i++) {
      i2c_send_command(I2C_READ); //do a start bit and send data
      i2c_get_ack();
      value = i2c_get_data();
      if (value != i) {
          //printf("ERROR expecting %h got %h",i,value);
          error ++;
      }
    }

    i2c_send_command(I2C_STOP); //do a stop bit

    ptr_antonio = (int*)(0x10001000);
    ptr_antonio[1] = error;

    if (error == 0)
      ptr_antonio[0] = 0xABBAABBA;
    else
      ptr_antonio[0] = 0xDEADBEEF;
  }

  return 0;
}
