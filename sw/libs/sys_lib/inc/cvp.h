#ifndef _CVP_H_
#define _CVP_H_

#include <pulp.h>

#define SOC_FLL_BASE_ADDR     ( CVP_BASE_ADDR  + 0x00 )
#define SOC_FLL_CONFIG_REG_1  ( SOC_FLL_BASE_ADDR + 0x08 )
#define SOC_FLL_CONFIG_REG_2  ( SOC_FLL_BASE_ADDR + 0x10 )
#define SOC_FLL_CONFIG_REG_3  ( SOC_FLL_BASE_ADDR + 0x18 )

#define CLUSTER_FLL_BASE_ADDR    ( CVP_BASE_ADDR  + 0x20 )
#define CLUSTER_FLL_CONFIG_REG_1 ( CLUSTER_FLL_BASE_ADDR + 0x08 )
#define CLUSTER_FLL_CONFIG_REG_2 ( CLUSTER_FLL_BASE_ADDR + 0x10 )
#define CLUSTER_FLL_CONFIG_REG_3 ( CLUSTER_FLL_BASE_ADDR + 0x18 )

void set_soc_frequency(int frequency);
void set_cluster_frequency(int frequency);

#endif // _CVP_H_
