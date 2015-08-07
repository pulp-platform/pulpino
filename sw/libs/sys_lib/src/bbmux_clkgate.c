#include "bbmux_clkgate.h"

int
read_bbmux_reg(int id)
{
  volatile int bbmux_reg;
  bbmux_reg = *(volatile int*) (id*4+BBMUX_BASE_ADDR);
  return bbmux_reg;
}

int
read_clkgate_reg(int id)
{
  volatile int clkgate_reg;
  clkgate_reg = *(volatile int*) (id*4+CLKGATE_BASE_ADDR);
  return clkgate_reg;
}

void
set_clkgate_reg(int id, int data)
{
  *(volatile int*) (id*4+CLKGATE_BASE_ADDR) = data;
}

void
set_bbmux_reg(int id, int data)
{
  *(volatile int*) (id*4+BBMUX_BASE_ADDR) = data;
}
