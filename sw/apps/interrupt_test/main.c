
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "event.h"
int main()
{
  uart_set_cfg(0, 0);

  uart_send("Hello!\n", 7);

  uart_wait_tx_done();

  printf("Hello World!!!!!\n");

  IER = 0xff;
  
  for(int i = 0; i < 100; i++)
  	asm volatile ( "nop" );

  printf("Reg Valu: %d", IAR);

  return 0;
}
