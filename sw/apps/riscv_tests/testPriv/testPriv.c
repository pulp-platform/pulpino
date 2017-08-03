// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/*
  Beta version. Not included in Tests yet
*/

#include <stdio.h>
#include "utils.h"
#include "bench.h"

#define PRIV_LVL_M 3
#define PRIV_LVL_U 0

void check_switch_mode();
void user_taks();

volatile unsigned int g_illegal_insn_counter;
volatile unsigned int g_ecall_insn_counter;
volatile unsigned int mepc_endill;
volatile unsigned int num_errors;

int main()
{
  num_errors = 0;
  check_switch_mode();
  while(1);
}

void check_switch_mode() {
  unsigned int mstatus;
  unsigned int priv_lvl;
  unsigned int mepc;
  unsigned int mpp;
  unsigned int boot_add;

  //-----------------------------------------------------------------
  // Check switch_mode
  //-----------------------------------------------------------------

  //First we check we are in machine mode
  mstatus  = __builtin_pulp_spr_read(0x300);
  priv_lvl = __builtin_pulp_spr_read(0xC10);
  boot_add = __builtin_pulp_spr_read(0x305);

  if(mstatus!=0x1800){
    num_errors++;
    printf("1) mstatus is %x. Expected %x\n",mstatus,0x1800);
  }

  if(priv_lvl!=PRIV_LVL_M){
    num_errors++;
    printf("2) priv_lvl is %x. Expected %x\n",priv_lvl,PRIV_LVL_M);
  }

  if(boot_add!=0){
    num_errors++;
    printf("3) boot_add is %x. Expected %x\n",boot_add,0);
  }

  //Here we prepare contex for USER task

  //First we set MPP to USER so that the MRET will go to USER MODE
  __builtin_pulp_spr_write(0x300, mstatus & ~(3 << 11) );
  asm volatile ("csrr %[d], mstatus" : [d] "=r" (mstatus) );
  mpp  = ( mstatus & (3 << 11) ) >> 11;
  if(mpp!=PRIV_LVL_U){
    num_errors++;
    printf("4) mpp is %x. Expected %x\n",mpp,PRIV_LVL_U);
  }

  //the ECALL handler will return here
  //the MRET will jump to user_task
  asm volatile ("csrrw %[d], mepc, %[s];"
      "mret;"
       : [d] "+r" (mepc)
       : [s] "r"  ((int)&user_taks)
       : "x16" );

}

void user_taks()
{

  unsigned int ustatus, mstatus_tmp;
  unsigned int priv_lvl;

  //First we check we are in user mode
  ustatus  = __builtin_pulp_spr_read(0x000);
  priv_lvl = __builtin_pulp_spr_read(0xC10);

  if(ustatus!=0){
    num_errors++;
    printf("5) ustatus is %x. Expected %x\n",ustatus,0);
  }

  if(priv_lvl!=PRIV_LVL_U){
    num_errors++;
    printf("6) priv_lvl is %x. Expected %x\n",priv_lvl,PRIV_LVL_U);
  }

  //Here we try to access mstatus, this will raise an illegal instruction
  asm volatile ("la x16, add_lbl;"
      "sw x16, 0(%[newmepc]);"
      "csrr %[d], mstatus;"
      "add_lbl: addi %[d], x0, 0xFF;"
       : [d] "=r" (mstatus_tmp)
       : [newmepc] "r"  (&mepc_endill)
       : "x16" );

  if(g_illegal_insn_counter!=1){
    num_errors++;
    printf("9) g_illegal_insn_counter is %x. Expected %x\n",g_illegal_insn_counter,1);
  }

  //Here we call the ECALL, we go back to machine mode
  asm volatile ("ecall;");

}

// overwrite illegal instruction handler
void illegal_insn_handler_c()
{

  unsigned int mstatus, priv_lvl, mpp;

  //Here we are in M mode
  asm volatile("csrr %0, mstatus" : "=r" (mstatus) );
  priv_lvl = __builtin_pulp_spr_read(0xC10);
  mpp  = ( mstatus & (3 << 11) ) >> 11;

  if(mpp!=PRIV_LVL_U){
    num_errors++;
    printf("7) mpp is %x. Expected %x\n",mstatus,PRIV_LVL_U);
  }

  if(priv_lvl!=PRIV_LVL_M){
    num_errors++;
    printf("8) priv_lvl is %x. Expected %x\n",priv_lvl,PRIV_LVL_M);
  }

  g_illegal_insn_counter += 1;


  // jump over illegal instruction
  __builtin_pulp_spr_write(0x341, mepc_endill);

  // go back in U mode, mpp already in PRIV_LVL_U
}

// overwrite ecall instruction handler
void ecall_insn_handler_c()
{

  unsigned int mstatus, priv_lvl, mpp;

  //Here we are in M mode
  asm volatile("csrr %0, mstatus" : "=r" (mstatus) );
  priv_lvl = __builtin_pulp_spr_read(0xC10);
  mpp  = ( mstatus & (3 << 11) ) >> 11;

  if(mpp!=PRIV_LVL_U){
    num_errors++;
    printf("10) mpp is %x. Expected %x\n",mpp,PRIV_LVL_U);
  }

  if(priv_lvl!=PRIV_LVL_M){
    num_errors++;
    printf("11) priv_lvl is %x. Expected %x\n",priv_lvl,PRIV_LVL_M);
  }

  g_ecall_insn_counter += 1;

  // go back in M mode by setting MPP
  __builtin_pulp_spr_write(0x300, mstatus | (3 << 11));

  if(num_errors == 0)
    printf("TEST SUCCESS\n");
  else
    printf("TEST FAIL\n");

  uart_wait_tx_done();

  exit(num_errors);

}
