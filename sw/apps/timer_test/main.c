
#include "string_lib.h"
#include "utils.h"
#include "uart.h"

int main()
{
  start_timer();

  qprintf("Time is %d\n", get_time(), 0, 0, 0);
  qprintf("Hello World!!!!!\n", 0, 0, 0, 0);

  return 0;
}
