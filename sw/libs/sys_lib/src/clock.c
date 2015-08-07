#include "clock.h"

void set_cluster_clk_divider(int divider) {
  *(volatile int*) (CLK_REG_DIV0) = divider;
}

void set_soc_clk_divider(int divider) {
  *(volatile int*) (CLK_REG_DIV1) = divider;
}

int read_cluster_clk_divider()
{
  volatile int div_reg;
  div_reg = *(volatile int*) (CLK_REG_DIV0);
  return div_reg;
}

int read_soc_clk_divider()
{
  volatile int div_reg;
  div_reg = *(volatile int*) (CLK_REG_DIV1);
  return div_reg;
}

