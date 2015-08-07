
#include "bar.h"
#include "string_lib.h"
#include "utils.h"

volatile unsigned int setme = 1;

int main() {

  while(setme);

  qprintf("Hello!\n");
  qprintf("Hello!\n", 0);
  qprintf("Hello!\n", 0, 0);
  qprintf("Hello!\n", 0, 0, 0);
  qprintf("Hello!\n", 0, 0, 0, 0);

  qprintf("char %c!\n", 'H');
  qprintf("string %s!\n", "STR");
  qprintf("HEX %X!\n", 0xABCD0123);
  qprintf("hex %x!\n", 0xabcd0123);
  qprintf("Dec %d!\n", 1337);
  qprintf("Neg %d!\n", -1337);
  qprintf("Un %u!\n", 1337);

  synch_barrier();
  eoc(0);
  return 0;
}
