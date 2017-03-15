// Copyright 2016 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/**
 * @file
 * @brief GPIO library.
 *
 * Provides GPIO helper function like setting
 * input/output direction and reading/writing the pins.
 *
 */
#ifndef _GPIO_H_
#define _GPIO_H_

#include <pulpino.h>

#define GPIO_REG_PADDIR               ( GPIO_BASE_ADDR + 0x00 )
#define GPIO_REG_PADIN                ( GPIO_BASE_ADDR + 0x04 )
#define GPIO_REG_PADOUT               ( GPIO_BASE_ADDR + 0x08 )
#define GPIO_REG_INTEN                ( GPIO_BASE_ADDR + 0x0C )
#define GPIO_REG_INTTYPE0             ( GPIO_BASE_ADDR + 0x10 )
#define GPIO_REG_INTTYPE1             ( GPIO_BASE_ADDR + 0x14 )
#define GPIO_REG_INTSTATUS            ( GPIO_BASE_ADDR + 0x18 )

#define GPIO_REG_PADCFG0              ( GPIO_BASE_ADDR + 0x20 )
#define GPIO_REG_PADCFG1              ( GPIO_BASE_ADDR + 0x24 )
#define GPIO_REG_PADCFG2              ( GPIO_BASE_ADDR + 0x28 )
#define GPIO_REG_PADCFG3              ( GPIO_BASE_ADDR + 0x2C )
#define GPIO_REG_PADCFG4              ( GPIO_BASE_ADDR + 0x30 )
#define GPIO_REG_PADCFG5              ( GPIO_BASE_ADDR + 0x24 )
#define GPIO_REG_PADCFG6              ( GPIO_BASE_ADDR + 0x38 )
#define GPIO_REG_PADCFG7              ( GPIO_BASE_ADDR + 0x3C )

#define SOC_CTRL_PADFUN               ( SOC_CTRL_BASE_ADDR + 0x00 )



#define PADDIR 				REGP(GPIO_REG_PADDIR)
#define PADIN 				REGP(GPIO_REG_PADIN)
#define PADOUT 				REGP(GPIO_REG_PADOUT)
#define INTEN 				REGP(GPIO_REG_INTEN)
#define INTTYPE0 			REGP(GPIO_REG_INTTYPE0)
#define INTTYPE1 			REGP(GPIO_REG_INTTYPE1)
#define INTSTATUS 			REGP(GPIO_REG_INTSTATUS)

#define PADCFG0 			REGP(GPIO_REG_PADCFG0)
#define PADCFG1 			REGP(GPIO_REG_PADCFG1)
#define PADCFG2 			REGP(GPIO_REG_PADCFG2)
#define PADCFG3 			REGP(GPIO_REG_PADCFG3)
#define PADCFG4 			REGP(GPIO_REG_PADCFG4)
#define PADCFG5 			REGP(GPIO_REG_PADCFG5)
#define PADCFG6 			REGP(GPIO_REG_PADCFG6)
#define PADCFG7 			REGP(GPIO_REG_PADCFG7)

#define PADFUN 				REGP(SOC_CTRL_PADFUN)


#define FUNC_GPIO 1

#define DIR_IN  0
#define DIR_OUT 1

#define GPIO_IRQ_FALL  0x3
#define GPIO_IRQ_RISE  0x2
#define GPIO_IRQ_LEV0  0x0
#define GPIO_IRQ_LEV1  0x1

void set_pin_function(int pinnumber, int function);
int  get_pin_function(int pinnumber);

void set_gpio_pin_direction(int pinnumber, int direction);
int  get_gpio_pin_direction(int pinnumber);

void set_gpio_pin_value(int pinnumber, int value);
int  get_gpio_pin_value(int pinnumber);

void set_gpio_pin_irq_type(int pinnumber, int type);
void set_gpio_pin_irq_en(int pinnumber, int enable);
int get_gpio_irq_status();

#endif // _GPIO_H_
