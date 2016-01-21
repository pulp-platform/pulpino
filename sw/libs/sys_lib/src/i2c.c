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
