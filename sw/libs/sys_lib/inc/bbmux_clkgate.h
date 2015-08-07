#ifndef __BBMUX_CLKGATE_H__
#define __BBMUX_CLKGATE_H__

#include "pulp.h"

#define BBMUX_BASE_ADDR        (BBMUX_CLKGATE_BASE_ADDR + 0x0)
#define CLKGATE_BASE_ADDR      CLKGATE_REG_BASE

// to identify a clkgate or bbmux register simply
// add the following offsets to the base address
// 0x00 = TCDM bank 0
// 0x04 = TCDM bank 1
// 0x08 = TCDM bank 2
// 0x0c = TCDM bank 3
// 0x10 = CORE 0
// 0x14 = CORE 1
// 0x18 = CORE 2
// 0x1c = CORE 3
// 0x20 = Cluster Internal 0
// 0x24 = SCM TCDM 0

// clkgate registers take values 0 (not gated) and 1 (clock gated)

// bbmux registers take values 00/01/10/11 (default 00)

int read_bbmux_reg(int);
int read_clkgate_reg(int);

void set_bbmux_reg(int, int);
void set_clkgate_reg(int, int);

#endif
