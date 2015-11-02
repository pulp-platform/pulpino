#include "timer.h"

int set_time = 0;

void reset_timer(void) {
	set_time = TIRA = 0;
}

void start_timer(void) {
	TPRA = 0x1;
}

void stop_timer(void) {
	TPRA = 0x0;
}

int get_time(void) {
	return TIRA - set_time;
}