#include "timer.h"

void reset_timer(void) {
  TIRA = 0;
}

void start_timer(void) {
  TPRA = 0x1;
}

void stop_timer(void) {
  TPRA = 0x0;
}

int get_time(void) {
  return TIRA;
}
