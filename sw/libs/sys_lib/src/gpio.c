// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <gpio.h>

void set_pin_function(int pinnumber, int function) {
  volatile int old_function;
  int addr;


  old_function = *(volatile int*) (SOC_CTRL_PADFUN);
  old_function = old_function & (~(1 << pinnumber));
  old_function = old_function | (function << pinnumber);
  *(volatile int*) (SOC_CTRL_PADFUN) = old_function;
}

int  get_pin_function(int pinnumber) {
  volatile int old_function;
  old_function = *(volatile int*) (SOC_CTRL_PADFUN);
  old_function = (old_function >> pinnumber & 0x01);
  return old_function;
}

void set_gpio_pin_direction(int pinnumber, int direction) {
  volatile int old_dir;
  old_dir = *(volatile int*) (GPIO_REG_PADDIR);
  if (direction == 0)
    old_dir &= ~(1 << pinnumber);
  else
    old_dir |= 1 << pinnumber;
  *(volatile int*) (GPIO_REG_PADDIR) = old_dir;
}

int  get_gpio_pin_direction(int pinnumber) {
  volatile int old_dir;
  old_dir = *(volatile int*) (GPIO_REG_PADDIR);
  old_dir = (old_dir >> (pinnumber*2) & 0x01);
  return old_dir;

}

void set_gpio_pin_value(int pinnumber, int value) {
  volatile int v;
  v = *(volatile int*) (GPIO_REG_PADOUT);
  if (value == 0)
    v &= ~(1 << pinnumber);
  else
    v |= 1 << pinnumber;
  *(volatile int*) (GPIO_REG_PADOUT) = v;
}

int  get_gpio_pin_value(int pinnumber) {
  volatile int v;
  v = *(volatile int*) (GPIO_REG_PADIN);
  v = (v >> pinnumber) & 0x01;
  return v;
}

void set_gpio_pin_irq_en(int pinnumber, int enable) {
  int v;
  v = *(volatile int*) (GPIO_REG_INTEN);
  if (enable == 0)
    v &= ~(1 << pinnumber);
  else
    v |= 1 << pinnumber;
  *(volatile int*) (GPIO_REG_INTEN) = v;
}

void set_gpio_pin_irq_type(int pinnumber, int type) {
  int type0;
  int type1;

  type0 = *(volatile int*) (GPIO_REG_INTTYPE0);
  type1 = *(volatile int*) (GPIO_REG_INTTYPE1);

  if ((type & 0x1) == 0)
    type0 &= ~(1 << pinnumber);
  else
    type0 |= 1 << pinnumber;

  if ((type & 0x2) == 0)
    type1 &= ~(1 << pinnumber);
  else
    type1 |= 1 << pinnumber;

  *(volatile int*) (GPIO_REG_INTTYPE0) = type0;
  *(volatile int*) (GPIO_REG_INTTYPE1) = type1;
}

int get_gpio_irq_status() {
  return *(volatile int*) (GPIO_REG_INTSTATUS);
}
