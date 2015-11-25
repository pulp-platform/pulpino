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

#define FUNC_GPIO 1

#define DIR_IN  0
#define DIR_OUT 1

void set_pin_function(int pinnumber, int function);
int  get_pin_function(int pinnumber);

void set_gpio_pin_direction(int pinnumber, int direction);
int  get_gpio_pin_direction(int pinnumber);

void set_gpio_pin_value(int pinnumber, int value);
int  get_gpio_pin_value(int pinnumber);

#endif // _GPIO_H_
