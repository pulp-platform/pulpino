#include <string_lib.h>

#include <bench.h>

/* this test has no meaning on the PULPino platform - its here just for legacy reasons */

int main() {
  int i;
  int coreid = get_core_id();

  // do some work on core 2 (to check if barrier works)
  if (coreid == 1) {
    int num = (get_core_id() + 1) * 10;
    for (i = 0; i < num; i++) {
      coreid += get_core_id() + 1;
    }
  }

  synch_barrier();

  printf("rv_basic: end\n", coreid, 0, 0, 0);

  // write core state
  {
    long long *cstate = 0x10001000;
    cstate += get_core_id();
    *cstate = 0xabbaabba;
  }

  print_summary(0);

  return coreid;
}

