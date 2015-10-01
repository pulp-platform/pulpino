
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

  IER = 0x55555555;
  
  printf("Reg Valu: %d", IER);

  return 0;
}
