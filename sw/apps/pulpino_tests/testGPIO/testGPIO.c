#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "gpio.h"

int main()
{
  if(get_core_id() == 0) {

    set_pin_function(PIN_MSPI_CSN1, FUNC_EXT2);
    if (get_pin_function(PIN_MSPI_CSN1) == FUNC_EXT2) {
       printf("Succesfully enabled func 2 on PIN_MSPI_CSN1\n",0,0,0,0); 
    } else {
       printf("ERROR on enabling func 2 on PIN_MSPI_CSN1\n",0,0,0,0); 
    }
 
    set_pin_function(PIN_MSPI_CSN2, FUNC_GPIO);
    set_gpio_pin_direction(PIN_MSPI_CSN2, DIR_OUT); 
    if (get_gpio_pin_direction(PIN_MSPI_CSN2) == DIR_OUT) {
       printf("Succesfully set out dir on PIN_MSPI_CSN2\n",0,0,0,0); 
    } else {
       printf("ERROR on setting out dir on PIN_MSPI_CSN2\n",0,0,0,0); 
    }

//void set_gpio_pin_value(int pinnumber, int value); 
//int  get_gpio_pin_value(int pinnumber); 

  printf("Done!!!\n",0,0,0,0);
  
  eoc(0);
  }
}
