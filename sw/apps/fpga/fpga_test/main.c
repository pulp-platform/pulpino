// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


#include "string_lib.h"
#include "utils.h"
#include "gpio.h"
#include "uart.h"

void waste_time() {
  int i;
  for(i = 0; i < 100000; i++) asm volatile("nop");
}

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

int main()
{
  int i;

  for(i = 0; i < 8; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
  }

  for(i = 16; i < 20; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
  }

  while(1) {
    set_gpio_pin_value( 8, get_gpio_pin_value( 0));
    set_gpio_pin_value( 9, get_gpio_pin_value( 1));
    set_gpio_pin_value(10, get_gpio_pin_value( 2));
    set_gpio_pin_value(11, get_gpio_pin_value( 3));
    set_gpio_pin_value(12, get_gpio_pin_value( 4));
    set_gpio_pin_value(13, get_gpio_pin_value( 5));
    set_gpio_pin_value(14, get_gpio_pin_value( 6));
    set_gpio_pin_value(15, get_gpio_pin_value( 7));

    printf("Enter: %d\n", get_gpio_pin_value(16));
    printf("Up:    %d\n", get_gpio_pin_value(20));
    printf("Down:  %d\n", get_gpio_pin_value(17));
    printf("Left:  %d\n", get_gpio_pin_value(18));
    printf("Right: %d\n", get_gpio_pin_value(19));

    waste_time();
  }

  return 0;
}
