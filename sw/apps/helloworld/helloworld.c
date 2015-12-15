
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "gpio.h"

int main()
{
  // activate UART module
  CGREG |= (1 << 0x01);
  uart_set_cfg(0, 1);
  set_gpio_pin_direction(0, DIR_OUT);
  set_gpio_pin_value(0, 1);
  uart_send("Hello!\n", 7);

  uart_wait_tx_done();
  printf("Hello World!!!!!\n");
  set_gpio_pin_value(0, 0);
  return 0;
}
