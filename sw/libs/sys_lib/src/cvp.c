#include <cvp.h>

void set_soc_frequency(int frequency) { // FREQUENCY IN MHz
  
  int multfact = frequency * 1000000 / 32768;
  
  *(int*)SOC_FLL_CONFIG_REG_1 = multfact;
  
}

void set_cluster_frequency(int frequency) { // FREQUENCY IN MHz
  
  int multfact = frequency * 1000000 / 32768;
  
  *(int*)CLUSTER_FLL_CONFIG_REG_1 = multfact;
  
}
