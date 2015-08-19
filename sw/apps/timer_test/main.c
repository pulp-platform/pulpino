
#include "string_lib.h"
#include "utils.h"
#include "uart.h"

int main()
{
  start_timer();

  printf("Time is %d\n", get_time());
  printf("Hello World!!!!!\n");

  return 0;
}
