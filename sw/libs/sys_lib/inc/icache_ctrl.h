#ifndef __ICACHE_CTRL_H__
#define __ICACHE_CTRL_H__

#include "pulp.h"

void flush_L0_buffers (int conf);

void enable_all_icache_banks();
void disable_all_icache_banks();
void flush_all_icache_banks();
void set_icache_banks_conf(int conf);
void flush_some_icache_banks ( int conf );

int  read_enable_icache_reg();
int  read_status_icache_reg();

void reset_all_icache_stat_regs();
void start_all_icache_stat_regs();
void stop_all_icache_stat_regs();

int read_hit_icache_reg    ( int bank );
int read_refill_icache_reg ( int bank );
int read_trans_icache_reg  ( int bank );

int read_L0_hit_reg( int core_id );
int read_L0_trans_reg( int core_id );

int read_intc_stall_reg( int core_id );
int read_intc_mcast_reg( int core_id );
int read_intc_trans_reg( int core_id );
#endif
