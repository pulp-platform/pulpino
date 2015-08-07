#include "icache_ctrl.h"

// Flush the L0 buffers, each bit of conf identifies a L0 Buffer Block
void flush_L0_buffers ( int conf)
{
    *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x0C) = conf;
}


// Enable All Icache Banks api 
void enable_all_icache_banks ( )
{
  *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = 0xFFFFFFFF;
}


// Disable all icache banks routine
void disable_all_icache_banks ( )
{
  *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = 0x00000000;
}


// Reset the statistic counters in each icache banks
void reset_all_icache_stat_regs()
{
    *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x10) = 0xFFFFFFFF;
}




// Enable all the statisctic counters in all icache banks availabe
void start_all_icache_stat_regs()
{
    *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x14) = 0xFFFFFFFF;
}

// Disable all the statisctic counters in all icache banks availabe
void stop_all_icache_stat_regs()
{
    *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x14) = 0x00000000;
}


// Read Hit counter reg in a specified icache bank
int read_hit_icache_reg( int bank )
{
  volatile int reg_status;
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x18+bank*4);
  return reg_status;
}

// Read refill counter reg in a specified icache bank
int read_refill_icache_reg( int bank )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x28+ bank*4);
  return reg_status;
}


// Read Total transaction counter reg in a specified icache bank
int read_trans_icache_reg( int bank )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x38+ bank*4);
  return reg_status;
}

// Read Total L0 Hit reg in a specified CORE
int read_L0_hit_reg( int core_id )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x48+ core_id*4);
  return reg_status;
}

// Read Total L0 tranzactions reg in a specified CORE
int read_L0_trans_reg( int core_id )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x58+ core_id*4);
  return reg_status;
}



// Read Total L0 tranzactions reg in a specified CORE
int read_intc_stall_reg( int core_id )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x68+ core_id*4);
  return reg_status;
}

// Read Total multicast hit tranzactions reg in a specified CORE
int read_intc_mcast_reg( int core_id )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x78+ core_id*4);
  return reg_status;
}

// Read Total icache interconnect transactions reg in a specified CORE
int read_intc_trans_reg( int core_id )
{
  volatile int reg_status;
  //FIXME we need to know how many banks are used
  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR +0x88+ core_id*4);
  return reg_status;
}
















void flush_all_icache_banks ( )
{
  *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = 0x0;
  *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = 0xF;
}


void flush_some_icache_banks ( int conf )
{
   volatile int current_conf = read_enable_icache_reg();
   *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = conf ^ current_conf;
   *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = current_conf;
}



void set_icache_banks_conf ( int conf )
{
  *(volatile int*) (ICACHE_CTRL_BASE_ADDR) = conf;
}



int read_enable_icache_reg(  )
{
  volatile int reg_status;

  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR);

  return reg_status;
}


int read_status_icache_reg(  )
{
  volatile int reg_status;

  reg_status = *(volatile int*) (ICACHE_CTRL_BASE_ADDR+0x08);

  return reg_status;
}