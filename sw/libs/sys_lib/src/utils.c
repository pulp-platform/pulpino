#include "spr-defs.h"
#include "utils.h"
#include "pulp.h"

#ifndef __riscv__
// write to special purpose register
void
mtspr(unsigned long spr, unsigned long value)
{
  asm("l.mtspr\t\t%0,%1,0": : "r" (spr), "r" (value));
}

// read from special purpose register
unsigned long 
mfspr(unsigned long spr)
{
  unsigned long value;
  asm("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr));
  return value;
}

#endif

// get core id
int
get_core_id()
{
  return 0;
}

// get number of cores
int
get_core_num()
{
  return NUM_CORES;
}

// get cluster ID
int
get_cluster_id()
{
  return 0;
}

// exit loop
void
exit (int i)
{
  while (1);
}

// signal end of computation
void eoc(int i){
  *(volatile int*)(EOC_UNIT_BASE_ADDR) = 1;

#ifdef CHIP_USED
  // setup GPIO
  set_pin_function(15, 1);
  set_gpio_pin_direction(14, 1);
  set_gpio_pin_value(14, 1);
#endif
}

// set fetch enable
void fetch_enable() {
  *(volatile int*)(FETCH_EN_UNIT_BASE_ADDR) = 1;
}

// sleep for 'iter' iterations. each iteration is approx 10 cycles
void sleep(volatile int iter)
{ 
  while(iter--);
}
