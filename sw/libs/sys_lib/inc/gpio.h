#ifndef _GPIO_H_
#define _GPIO_H_

#include <pulp.h>
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

#define SOC_CTRL_PADFUN0              ( CLK_DIV_ADDR + 0x00 )
#define SOC_CTRL_PADFUN1              ( CLK_DIV_ADDR + 0x04 )

#define PIN_SSPI_SIO0 0
#define PIN_SSPI_SIO1 1
#define PIN_SSPI_SIO2 2
#define PIN_SSPI_SIO3 3
#define PIN_SSPI_CSN  4
#define PIN_SSPI_SCK  5
#define PIN_MSPI_SIO0 6
#define PIN_MSPI_SIO1 7
#define PIN_MSPI_SIO2 8
#define PIN_MSPI_SIO3 9
#define PIN_MSPI_CSN0 10
#define PIN_MSPI_CSN1 11
#define PIN_MSPI_CSN2 12
#define PIN_MSPI_CSN3 13
#define PIN_MSPI_SCK  14

#define FUNC_SPI  0
#define FUNC_GPIO 1
#define FUNC_EXT1 2
#define FUNC_EXT2 3

#define DIR_IN  1
#define DIR_OUT 0

void set_pin_function(int pinnumber, int function);
int  get_pin_function(int pinnumber);
 
void set_gpio_pin_direction(int pinnumber, int direction); 
int  get_gpio_pin_direction(int pinnumber);

void set_gpio_pin_value(int pinnumber, int value); 
int  get_gpio_pin_value(int pinnumber); 

#endif // _GPIO_H_
