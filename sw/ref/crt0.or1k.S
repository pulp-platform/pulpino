// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "spr-defs.h"
#include "pulpino.h"

/* ======================================================= [ macros ] === */
#define CLABEL(label) _CLABEL(__USER_LABEL_PREFIX__, label)
#define __CLABEL(prefix, label) prefix ## label
#define _CLABEL(prefix, label) __CLABEL(prefix, label)

#define REDZONE 128
#define EXCEPTION_STACK_SIZE (100 + REDZONE)

#define CLEAR_GPR(gpr) \
    l.or    gpr, r0, r0

#define ENTRY(symbol)    \
    .global symbol ; \
symbol:

#define LOAD_SYMBOL_2_GPR(gpr,symbol)  \
    .global symbol ;               \
    l.movhi gpr, hi(symbol) ;      \
    l.ori   gpr, gpr, lo(symbol)

#define LOAD_CONST_2_GPR(gpr,symbol)  \
    l.movhi gpr, hi(symbol) ;      \
    l.ori   gpr, gpr, lo(symbol)

/* =================================================== [ exceptions ] === */
    .section .vectors, "ax"

/* ---[ 0x000: RESET exception ]----------------------------------------- */
    .org 0x00
    l.j   reset_handler
    l.nop
/* ---[ 0x008: BUS exception ]------------------------------------------- */
    .org 0x08
    l.j   default_exception_handler
    l.nop
/* ---[ 0x010: Data Page Fault exception ]------------------------------- */
    .org 0x10
    l.j   default_exception_handler
    l.nop
/* ---[ 0x018: Insn Page Fault exception ]------------------------------- */
    .org 0x18
    l.j   default_exception_handler
    l.nop
/* ---[ 0x020: Timer exception ]----------------------------------------- */
    .org 0x20
    l.j   default_exception_handler
    l.nop
/* ---[ 0x028: Aligment exception ]-------------------------------------- */
    .org 0x28
    l.j   default_exception_handler
    l.nop
/* ---[ 0x030: Illegal insn exception ]---------------------------------- */
    .org 0x30
    l.j   illegal_insn_handler
    l.nop
/* ---[ 0x038: External interrupt exception ]---------------------------- */
    .org 0x38
    l.j   interrupt_handler
    l.nop
/* ---[ 0x040: DTLB miss exception ]------------------------------------- */
    .org 0x40
    l.j   default_exception_handler
    l.nop
/* ---[ 0x048: ITLB miss exception ]------------------------------------- */
    .org 0x48
    l.j   default_exception_handler
    l.nop
/* ---[ 0x050: Range exception ]----------------------------------------- */
    .org 0x50
    l.j   default_exception_handler
    l.nop
/* ---[ 0x058: Syscall exception ]--------------------------------------- */
    .org 0x58
    l.j   default_exception_handler
    l.nop
/* ---[ 0x060: FPU exception ]------------------------------------------- */
    .org 0x60
    l.j   default_exception_handler
    l.nop
/* ---[ 0x068: Trap exception ]------------------------------------------ */
    .org 0x68
    l.j   default_exception_handler
    l.nop
/* ---[ 0x070: Emergency IRQ exception ]--------------------------------- */
    .org 0x70
    l.j   default_exception_handler
    l.nop

/* ========================================================= [ entry ] === */
    .section .text

ENTRY(_start)

    /* Clear BSS */
    LOAD_SYMBOL_2_GPR(r5, _bss_start)
    LOAD_SYMBOL_2_GPR(r6, _bss_end)

    l.sfleu r6, r5
    l.bf    zero_loop_end
    l.nop   0
zero_loop:
    l.sw    -4(r6), r0
    l.addi  r6, r6, -4
    l.sfgtu r6, r5
    l.bf    zero_loop
    l.nop   0
zero_loop_end:


main_entry:
    l.ori   r3, r0, 0x0
    l.ori   r4, r0, 0x1
    l.jal  CLABEL(uart_set_cfg);
    l.nop

    /* Jump to main program entry point (argc = argv = 0) */
    CLEAR_GPR(r3)
    CLEAR_GPR(r4)
    l.jal   main
    l.nop

    l.jal  CLABEL(uart_wait_tx_done);
    l.nop

    l.ori   r3, r0, 0x0
    l.jal  CLABEL(eoc);
    l.nop


    /* If program exits, call exit routine */
    l.addi  r3, r11, 0
    l.jal   exit
    l.nop

/* ====================================== [ default reset handler ] === */
reset_handler:
    l.movhi r0, 0
    l.movhi r1, 0
    l.movhi r2, 0
    l.movhi r3, 0
    l.movhi r4, 0
    l.movhi r5, 0
    l.movhi r6, 0
    l.movhi r7, 0
    l.movhi r8, 0
    l.movhi r9, 0
    l.movhi r10, 0
    l.movhi r11, 0
    l.movhi r12, 0
    l.movhi r13, 0
    l.movhi r14, 0
    l.movhi r15, 0
    l.movhi r16, 0
    l.movhi r17, 0
    l.movhi r18, 0
    l.movhi r19, 0
    l.movhi r20, 0
    l.movhi r21, 0
    l.movhi r22, 0
    l.movhi r23, 0
    l.movhi r24, 0
    l.movhi r25, 0
    l.movhi r26, 0
    l.movhi r27, 0
    l.movhi r28, 0
    l.movhi r29, 0
    l.movhi r30, 0
    l.movhi r31, 0

    /* Clear status register, set supervisor mode */
    l.ori r1, r0, SPR_SR_SM
    l.mtspr r0, r1, SPR_SR

    /* Early Stack initilization */
    l.movhi r1, 0x0012       // stack starts from high address
    l.ori   r1, r1, 0x0000

    /* LOAD program start address into r4 */
    LOAD_SYMBOL_2_GPR(r4, _start)

    /* LOAD_SYMBOL_2_GPR(r1, _stack) */
    l.addi  r2, r0, -3
    l.and   r1, r1, r2

    /* Jump to program start */
    l.jr    r4
    l.nop

/* ====================================== [ interrupt handler ] === */
interrupt_handler:
    l.addi  r1,r1,-EXCEPTION_STACK_SIZE;
    l.sw    0x18(r1),r9;
    l.jal   store_regs;
    l.nop;
    l.movhi r9,hi(end_except);
    l.ori   r9,r9,lo(end_except);
    l.j    CLABEL(int_main);
    l.nop

/* ====================================== [ default exception handler ] === */
default_exception_handler:
    l.addi  r1, r1, -EXCEPTION_STACK_SIZE;
    l.sw    4(r1), r3;
    l.sw    8(r1), r4;
    l.mfspr r3,r0,SPR_NPC;
    l.mfspr r4,r0,SPR_EPCR_BASE;

    l.sw    0x18(r1),r9;
    l.jal   store_regs;
    l.nop

    l.jal   default_exception_handler_c
    l.nop

    l.jal   end_except ;
    l.nop

/* ====================================== [ illegal instruction handler] === */
illegal_insn_handler:
    l.addi  r1,r1,-EXCEPTION_STACK_SIZE ;
    l.sw    0x18(r1),r9 ;
    l.jal   store_regs ;
    l.nop ;
    l.movhi r9,hi(end_except) ;
    l.ori   r9,r9,lo(end_except) ;
    l.j    CLABEL(illegal_insn_handler_c) ;
    l.nop


// we only have to save the following registers: r31, r29, r27, r25, r23, r21, r19, r17, r15, r13, r12, r11, r8, r7, r6, r5, r4, r3
// all other registers are saved by c function call conventions
// r9 must be saved before calling store_regs, as otherwise its content is already overwritten by l.jal
// we also save the hwloop registers here (currently 6)
store_regs:
    l.sw    0x00(r1),  r3
    l.sw    0x04(r1),  r4
    l.sw    0x08(r1),  r5
    l.sw    0x0c(r1),  r6
    l.sw    0x10(r1),  r7
    l.sw    0x14(r1),  r8
    l.sw    0x1c(r1), r11
    l.sw    0x20(r1), r12
    l.sw    0x24(r1), r13
    l.sw    0x28(r1), r15
    l.sw    0x2c(r1), r17
    l.sw    0x30(r1), r19
    l.sw    0x34(r1), r21
    l.sw    0x38(r1), r23
    l.sw    0x3c(r1), r25
    l.sw    0x40(r1), r27
    l.sw    0x44(r1), r29
    l.sw    0x48(r1), r31
    // hwloop registers
    l.mfspr r3, r0, 0x6000
    l.mfspr r4, r0, 0x6010
    l.mfspr r5, r0, 0x6020
    l.mfspr r6, r0, 0x6001
    l.mfspr r7, r0, 0x6011
    l.mfspr r8, r0, 0x6021
    l.sw    0x4C(r1), r3
    l.sw    0x50(r1), r4
    l.sw    0x54(r1), r5
    l.sw    0x58(r1), r6
    l.sw    0x5C(r1), r7
    l.jr    r9
    l.sw    0x60(r1), r8

end_except:   // load back registers from stack
    // hwloop registers
    l.lwz   r3, 0x4C(r1)
    l.lwz   r4, 0x50(r1)
    l.lwz   r5, 0x54(r1)
    l.lwz   r6, 0x58(r1)
    l.lwz   r7, 0x5C(r1)
    l.lwz   r8, 0x60(r1)
    l.mtspr r0, r3, 0x6000
    l.mtspr r0, r4, 0x6010
    l.mtspr r0, r5, 0x6020
    l.mtspr r0, r6, 0x6001
    l.mtspr r0, r7, 0x6011
    l.mtspr r0, r8, 0x6021

    l.lwz   r3,  0x00(r1)
    l.lwz   r4,  0x04(r1)
    l.lwz   r5,  0x08(r1)
    l.lwz   r6,  0x0c(r1)
    l.lwz   r7,  0x10(r1)
    l.lwz   r8,  0x14(r1)
    l.lwz   r9,  0x18(r1)
    l.lwz   r11, 0x1c(r1)
    l.lwz   r12, 0x20(r1)
    l.lwz   r13, 0x24(r1)
    l.lwz   r15, 0x28(r1)
    l.lwz   r17, 0x2c(r1)
    l.lwz   r19, 0x30(r1)
    l.lwz   r21, 0x34(r1)
    l.lwz   r23, 0x38(r1)
    l.lwz   r25, 0x3c(r1)
    l.lwz   r27, 0x40(r1)
    l.lwz   r29, 0x44(r1)
    l.lwz   r31, 0x48(r1)
    l.rfe                                // recover SR register and prior PC (jumps back to program)
    l.addi  r1, r1, EXCEPTION_STACK_SIZE // free stack places
