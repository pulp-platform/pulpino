
#include "bar.h"
#include "string_lib.h"
#include "utils.h"

volatile unsigned int setme = 1;
volatile unsigned int setme2 = 1;

int main() {

#ifdef __riscv__
  asm volatile ("EBREAK");
  asm volatile ("EBREAK");
#endif

  while(setme);

  qprintf("Hello!\n");

  while(setme2);

  return 0;
}
