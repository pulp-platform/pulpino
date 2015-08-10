#include "spr-defs.h"
#include "pulp.h"

/* ======================================================= [ macros ] === */
#define CLABEL(label) _CLABEL(__USER_LABEL_PREFIX__, label)
#define __CLABEL(prefix, label) prefix ## label
#define _CLABEL(prefix, label) __CLABEL(prefix, label)

#define REDZONE 8
#define EXCEPTION_STACK_SIZE (72 + REDZONE)


/* =================================================== [ exceptions ] === */
  .section .vectors, "ax"

  // external interrupt handler
  .org 0x00
  jal x0, interrupt_handler

  // non maskable interrupts
  .org 0x04
  jal x0, nm_interrupt_handler

  // illegal instruction exception
  .org 0x08
  jal x0, illegal_insn_handler

  // unused slot
  .org 0x0c
  .byte 0, 0, 0, 0

  // reset vector
  .org 0x10
  jal x0, reset_handler


/* ========================================================= [ entry ] === */
  .section .text

reset_handler:
  /* set all registers to zero (use mv for reduced code size) */
  mv  x1, x0
  mv  x2, x0
  mv  x3, x0
  mv  x4, x0
  mv  x5, x0
  mv  x6, x0
  mv  x7, x0
  mv  x8, x0
  mv  x9, x0
  mv x10, x0
  mv x11, x0
  mv x12, x0
  mv x13, x0
  mv x14, x0
  mv x15, x0
  mv x16, x0
  mv x17, x0
  mv x18, x0
  mv x19, x0
  mv x20, x0
  mv x21, x0
  mv x22, x0
  mv x23, x0
  mv x24, x0
  mv x25, x0
  mv x26, x0
  mv x27, x0
  mv x28, x0
  mv x29, x0
  mv x30, x0
  mv x31, x0

  /* stack initilization */
  li   x2, 0x10002000 // stack base addr


_start:
  .global _start

  /* clear BSS */
  la x26, _bss_start
  la x27, _bss_end

zero_loop:
  sw x0, 0(x26)
  addi x26, x26, 4
  ble x26, x27, zero_loop
zero_loop_end:



main_entry:
  addi   x10, x0, 0
  addi   x11, x0, 0
  jal  uart_set_cfg;

  /* jump to main program entry point (argc = argv = 0) */
  addi x10, x0, 0
  addi x11, x0, 0
  jal x1, main

  jal  uart_wait_tx_done;

  addi   x10, x0, 0
  jal  eoc;

  /* if program exits call exit routine from library */
  jal  x1, exit


/* ========================================== [ interrupt handler ] === */
interrupt_handler:
  addi x2, x2, -EXCEPTION_STACK_SIZE
  sw x1, 0x44(x2)
  jal x1, store_regs
  la x1, end_except
  jal x0, int_main

/* ======================================= [ nm_interrupt handler ] === */
nm_interrupt_handler:
  addi x2, x2, -EXCEPTION_STACK_SIZE
  sw x1, 0x44(x2)
  jal x1, store_regs
  la x1, end_except
  jal x0, irq_emergency_handler_c

/* ================================= [ illegal instruction handler] === */
illegal_insn_handler:
  addi x2, x2, -EXCEPTION_STACK_SIZE
  sw x1, 0x44(x2)
  jal x1, store_regs
  la x1, end_except
  jal x0, illegal_insn_handler_c


// saves all caller-saved registers (except return address)
store_regs:
  sw  x3, 0x00(x2)  // gp
  sw  x4, 0x04(x2)  // tp
  sw  x5, 0x08(x2)  // t0
  sw  x6, 0x0c(x2)  // t1
  sw  x7, 0x10(x2)  // t2
  sw x10, 0x14(x2)  // a0
  sw x11, 0x18(x2)  // a1
  sw x12, 0x1c(x2)  // a2
  sw x13, 0x20(x2)  // a3
  sw x14, 0x24(x2)  // a4
  sw x15, 0x28(x2)  // a5
  sw x16, 0x2c(x2)  // a6
  sw x17, 0x30(x2)  // a7
  sw x28, 0x34(x2)  // t3
  sw x29, 0x38(x2)  // t4
  sw x30, 0x3c(x2)  // t5
  sw x31, 0x40(x2)  // t6
  jalr x0, x1

// load back registers from stack
end_except:
  lw  x3, 0x00(x2)
  lw  x4, 0x04(x2)
  lw  x5, 0x08(x2)
  lw  x6, 0x0c(x2)
  lw  x7, 0x10(x2)
  lw x10, 0x14(x2)
  lw x11, 0x18(x2)
  lw x12, 0x1c(x2)
  lw x13, 0x20(x2)
  lw x14, 0x24(x2)
  lw x15, 0x28(x2)
  lw x16, 0x2c(x2)
  lw x17, 0x30(x2)
  lw x28, 0x34(x2)
  lw x29, 0x38(x2)
  lw x30, 0x3c(x2)
  lw x31, 0x40(x2)
  lw  x1, 0x44(x2)
  addi x2, x2, EXCEPTION_STACK_SIZE
  eret
