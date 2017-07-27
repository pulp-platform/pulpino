/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

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

//void gpio_int_handler(void* unused);
void board_setup();

int main()
{
  board_setup();

  IER = 0x1 << GPIO_EVENT;

  //int_add(GPIO_EVENT, gpio_int_handler, NULL);
  int_enable();

  printf("Going to sleep and wait for interrupts now\n");

  sleep();

  return 0;
}

void ISR_GPIO(void) {
  switch(get_gpio_irq_status()) {
    case 1 << 16: printf("Enter pressed\n"); break;
    case 1 << 17: printf("Down pressed\n");  break;
    case 1 << 18: printf("Left pressed\n");  break;
    case 1 << 19: printf("Right pressed\n"); break;
    case 1 << 20: printf("Up pressed\n");    break;
    default:      printf("Unknown GPIO\n");  break;
  }
}

void board_setup() {
  int i;

  //int_init();

  // setup GPIO pins
  for(i = 0; i < 8; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
  }

  for(i = 16; i < 21; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
    set_gpio_pin_irq_en(i, 1);
    set_gpio_pin_irq_type(i, GPIO_IRQ_RISE);
  }
  EER = 0x0;
}
