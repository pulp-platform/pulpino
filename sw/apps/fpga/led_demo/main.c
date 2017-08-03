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

void left_to_right_single(unsigned int first, unsigned int last);
void left_to_right_on(unsigned int first, unsigned int last);
void left_to_right_off(unsigned int first, unsigned int last);
void left_to_right_on_slow(unsigned int first, unsigned int last);
void left_to_right_off_slow(unsigned int first, unsigned int last);

void waste_time() {
  int i;
  for(i = 0; i < 10000; i++) asm volatile("nop");
}

#define SW_0_BIT    (1 <<  0)
#define SW_1_BIT    (1 <<  1)
#define SW_2_BIT    (1 <<  2)
#define SW_3_BIT    (1 <<  3)
#define SW_4_BIT    (1 <<  4)
#define SW_5_BIT    (1 <<  5)
#define SW_6_BIT    (1 <<  6)
#define SW_7_BIT    (1 <<  7)

#define LED_0        8
#define LED_1        9
#define LED_2       10
#define LED_3       11
#define LED_4       12
#define LED_5       13
#define LED_6       14
#define LED_7       15

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

  for(i = 8; i < 16; i++) {
    set_gpio_pin_direction(i, DIR_OUT);
    set_pin_function(i, FUNC_GPIO);
  }

  for(i = 16; i < 20; i++) {
    set_gpio_pin_direction(i, DIR_IN);
    set_pin_function(i, FUNC_GPIO);
  }

  while(1) {
    left_to_right_on_slow(LED_1, LED_7);
    left_to_right_off_slow(LED_1, LED_7);

    waste_time();

    left_to_right_on(LED_1, LED_7);
    left_to_right_off(LED_1, LED_7);

    waste_time();

    left_to_right_single(LED_1, LED_7);
    left_to_right_single(LED_1, LED_7);
    left_to_right_single(LED_1, LED_7);
  }

  return 0;
}

void left_to_right_single(unsigned int first, unsigned int last) {
  unsigned int i;
  unsigned int j;

  for(i = first; i <= last; i++) {

    for(j = first; j <= last; j++) {
      set_gpio_pin_value(j, 0);
    }

    set_gpio_pin_value(i, 1);

    waste_time();
  }
}

void left_to_right_on(unsigned int first, unsigned int last) {
  unsigned int i;
  unsigned int j;


  for(j = first; j <= last; j++) {
    set_gpio_pin_value(j, 0);
  }

  for(i = first; i <= last; i++) {

    set_gpio_pin_value(i, 1);

    waste_time();
  }
}

void left_to_right_off(unsigned int first, unsigned int last) {
  unsigned int i;
  unsigned int j;


  for(j = first; j <= last; j++) {
    set_gpio_pin_value(j, 1);
  }

  for(i = first; i <= last; i++) {

    set_gpio_pin_value(i, 0);

    waste_time();
  }
}

void left_to_right_on_slow(unsigned int first, unsigned int last) {
  int i;
  int j;

  for(j = first; j <= last; j++) {
    set_gpio_pin_value(j, 0);
  }

  for(i = last; i > first; i--) {

    left_to_right_single(first, i);
  }
}

void left_to_right_off_slow(unsigned int first, unsigned int last) {
  int i;
  int j;

  for(j = first; j <= last; j++) {
    set_gpio_pin_value(j, 1);
  }

  for(i = last; i >= first; i--) {

    left_to_right_single(i, last);
  }
}

