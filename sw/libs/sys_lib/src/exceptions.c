#include <spr-defs.h>
#include "string_lib.h"
#include "utils.h"
#include "events.h"

// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
__attribute__((interrupt)) __attribute__((weak))
void default_exception_handler_c(unsigned exception_address,unsigned epc)
{
  for(;;);
}

// use weak attribute here, so we can overwrite this function to provide custom exception handlers, e.g. for tests
__attribute__((interrupt)) __attribute__((weak))
void illegal_insn_handler_c(void)
{
  for(;;);
}
