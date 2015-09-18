
#include "string_lib.h"
#include "utils.h"
#include "uart.h"

void waste_time() {
  int i;
  for(i = 0; i < 100000; i++) asm volatile("nop");
}

int main()
{
  int i;
  while(1) {
    for(i = 9; i < 16; i++) {
      set_gpio_pin_value(i, 1);
      waste_time();
      set_gpio_pin_value(i, 0);
      waste_time();
    }
  }

  return 0;
}
