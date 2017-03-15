#include "boardsupport.h"
#include "gpio.h"
#include "timer.h"

#warning boardsupport.c compiled

void initialise_board()
{
}

void start_trigger()
{
  set_gpio_pin_value(0, 1);
  start_timer();
}
void stop_trigger()
{
    stop_timer();
    set_gpio_pin_value(0, 0);
    printf("Execution time: %d\n", get_time());
}