
#include <stdio.h>
#include "utils.h"
#include "bar.h"
#include "bench.h"
#include "events.h"
#include "events_ids.h"

#define NUM_INTERRUPTED  10

void check_illegal(testresult_t *result, void (*start)(), void (*stop)());
void check_irq(testresult_t *result, void (*start)(), void (*stop)());
void check_irq_lfsr_running(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "illegal",          .test = check_illegal          },
  { .name = "irq",              .test = check_irq              },
  { .name = "irq_lfsr",         .test = check_irq_lfsr_running },
  {0, 0}
};

int main()
{
  run_suite(testcases);

  synch_barrier();

  return 0;
}


unsigned int g_illegal_insn_counter;

void check_illegal(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int core_id = get_core_id();
  unsigned int i;

  //////////////////////////////////////////////////////////////////////////////
  // Check if we can correctly cause an illegal instruction exception and jump
  // over the illegal instruction
  //////////////////////////////////////////////////////////////////////////////

  if(core_id == 0) {
    g_illegal_insn_counter = 0;

    for(i = 0; i < NUM_INTERRUPTED; i++) {
      unsigned int act = 0;
      unsigned int exp = 1+2;
      asm volatile ("addi %[act], %[act], 1\n"
                    ".word 0xF0F0F0F0\n"
                    "addi %[act], %[act], 2\n"
                    : [act] "+r" (act));

      check_uint32(result, "Checking illegal instruction", act, exp);
    }

    check_uint32(result, "Comparing number of illegal instructions found", g_illegal_insn_counter, NUM_INTERRUPTED);
  }

  synch_barrier();
}


unsigned int g_irq_counter;

void check_irq(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int core_id = get_core_id();
  unsigned int i, j;


  //////////////////////////////////////////////////////////////////////////////
  // Trigger an IRQ and count the number of IRQs received
  //////////////////////////////////////////////////////////////////////////////

  if(core_id == 0) {
    set_irq_mask_low(core_id, 1 << GP0);
    int_enable();

    for(i = 0; i < NUM_INTERRUPTED; i++) {
      trigg_GP(0x1, GP0);

      for(j = 0; j < 100; j++) asm volatile ("nop");
    }

    int_disable();

    check_uint32(result, "Comparing number of IRQs received", g_irq_counter, NUM_INTERRUPTED);
  }

  synch_barrier();
}


unsigned int lfsr_step(unsigned int reg) {
  // From https://www.schneier.com/paper-pseudorandom-sequence.html, example 1
  // This LFSR has a period of 2^32-1

  /*Register should be initialized with some random value.*/
  reg = ((((reg >> 31)  /*Shift the 32nd bit to the first
                                    bit*/
             ^ (reg >> 6)    /*XOR it with the seventh bit*/
             ^ (reg >> 4)    /*XOR it with the fifth bit*/
             ^ (reg >> 2)    /*XOR it with the third bit*/
             ^ (reg >> 1)    /*XOR it with the second bit*/
             ^ reg)          /*and XOR it with the first bit.*/
             & 0x0000001)         /*Strip all the other bits off and*/
             <<31)                /*move it back to the 32nd bit.*/
             | (reg >> 1);   /*Or with the register shifted
                                    right.*/
  return reg;
}

unsigned int test_lfsr(void) {
  unsigned int i;
  unsigned int j;
  unsigned int lfsr_exp = 0;
  unsigned int lfsr;
  unsigned int errors = 0;

  for(j = 0; j < 10; j++) {
    lfsr = 0xDEADBEEF;

    for(i = 0; i < 200; i++) {
      lfsr = lfsr_step(lfsr);
    }

    if(j == 0)
      lfsr_exp = lfsr;
    else {
      if (lfsr != lfsr_exp)
        errors++;
    }
  }

  return errors;
}

////////////////////////////////////////////////////////////////////////////////
// In this test we run an LFSR in a loop and trigger some interrupts on core 0
// from core 1
////////////////////////////////////////////////////////////////////////////////
void check_irq_lfsr_running(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int core_id = get_core_id();
  unsigned int i;
  unsigned int j;

  //////////////////////////////////////////////////////////////////////////////
  // Check if we can correctly interrupt core 1 when it runs an LFSR
  //////////////////////////////////////////////////////////////////////////////

  if(core_id == 0) {
    clear_evnt_buff_low(1 << GP0);
    set_evnt_mask_low(core_id, (1 << GP0));

    for(i = 0; i < NUM_INTERRUPTED; i++) {
      for(j = 0; j < 500; j++) asm volatile ("nop");

      trigg_GP(1 << 1, GP0);
      wait_event();
      clear_evnt_buff_low(1 << GP0);
    }

    set_evnt_mask_low(core_id, (1 << BARRIER));

  } else if(core_id == 1) {
    g_irq_counter = 0;

    set_irq_mask_low(core_id, 1 << GP0);
    int_enable();

    if(test_lfsr() != 0) {
      result->errors++;
      printf("LFSR has wrong value\n");
    }

    int_disable();
  }

  synch_barrier();

  check_uint32(result, "lfsr_running: Comparing number of interrupts received", g_irq_counter, NUM_INTERRUPTED);

  synch_barrier();
}

// overwrite illegal instruction handler
void illegal_insn_handler_c()
{
  uint32_t exception_address;

  g_illegal_insn_counter += 1;

  // jump over illegal instruction
  asm("csrr %0, 0x341" : "=r" (exception_address) );
  asm("csrw 0x341, %0" : : "r" (exception_address + 4) );
}

void int_main() {
  g_irq_counter += 1;

  trigg_GP(1 << 0, GP0);
  clear_irq_buff_low(1 << GP0);
}
