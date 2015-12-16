
#include <stdio.h>
#include "gpio.h"
#include "int.h"
#include "utils.h"

#define SW_0_BIT    (1 <<  0)
#define SW_1_BIT    (1 <<  1)
#define SW_2_BIT    (1 <<  2)
#define SW_3_BIT    (1 <<  3)
#define SW_4_BIT    (1 <<  4)
#define SW_5_BIT    (1 <<  5)
#define SW_6_BIT    (1 <<  6)
#define SW_7_BIT    (1 <<  7)

#define BTN_C_BIT   (1 << 16)
#define BTN_D_BIT   (1 << 17)
#define BTN_L_BIT   (1 << 18)
#define BTN_R_BIT   (1 << 19)
#define BTN_U_BIT   (1 << 20)

void gpio_int_handler(void* unused);

int main()
{
  int i;

  // setup GPIO pins
  for(i = 0; i < 8; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
  }

  for(i = 16; i < 20; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
    set_gpio_pin_irq_en(i, 1);
    set_gpio_pin_irq_type(i, GPIO_IRQ_RISE);
  }

  IER = 0xFFFFFFFF;
  EER = 0x0;

  int_init();
  int_add(GPIO_EVENT, gpio_int_handler, NULL);
  int_enable();

  printf("Going to sleep and wait for interrupts now\n");

  sleep();

  return 0;
}

void gpio_int_handler(void* unused) {
  printf("GPIO EVENT SEEN\n");
}
