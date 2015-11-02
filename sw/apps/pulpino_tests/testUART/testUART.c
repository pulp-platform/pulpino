#include "string.h"
#include <pulp.h>
#include <gpio.h>



#define BAUD_RATE                       1000000
#define SOC_FREQ                        200
#define CLUSTER_FREQ                    500

char* str_out = "Hello UART!\n";


int main()
{
  if(get_core_id() == 0) {
    set_soc_frequency(SOC_FREQ);
    set_cluster_frequency(CLUSTER_FREQ);


    set_pin_function(PIN_MSPI_CSN2, FUNC_EXT1);
    uart_set_cfg(0, SOC_FREQ, BAUD_RATE);

    uart_send(str_out, 12);

    unsigned int i;
    for(i = 0; i < 5; i++)
      printf("Nr %d\n", i,0,0,0);

    // wait until all UART output was done, i.e. tx_busy is 0
    while( (*(volatile int*)(CLK_REG_UART_STATUS) & 0x1) == 0x1);

    eoc(0);
  }
}

