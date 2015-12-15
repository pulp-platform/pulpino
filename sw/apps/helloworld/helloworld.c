
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "gpio.h"
#include "pulpino.h"
#include "timer.h"
int main()
{
  uart_set_cfg(0, 1);
  set_gpio_pin_direction(0, DIR_OUT);
  set_gpio_pin_value(0, 1);
  uart_send("Hello!\n", 7);
  printf("REG: %x\n", CGREG);
  uart_wait_tx_done();
  printf("Hello World!!!!!\n");
  set_gpio_pin_value(0, 0);
  return 0;
}
