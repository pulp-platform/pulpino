#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <pulp.h>

#define CLK_REG_DIV0 (CLK_DIV_ADDR + 0x08)
#define CLK_REG_DIV1 (CLK_DIV_ADDR + 0x0C)

void set_cluster_clk_divider(int divider); //if divider is 1 or 0 the divider is bypassed
void set_soc_clk_divider(int divider); //if divider is 1 or 0 the divider is bypassed

int read_cluster_clk_divider(); 
int read_soc_clk_divider(); 

#endif // _CLOCK_H_
