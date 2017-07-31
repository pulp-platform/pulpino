/*
    FreeRTOS V8.2.2 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/


#include <stdlib.h>
#include <int.h>
#include <utils.h>
#include <timer.h>
#include <event.h>

#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RI5CY port.
 *----------------------------------------------------------*/

/*
 * Macro to save the general purpose registers and the exception program counter (mepc),
 * and than save the stack pointer into the TCB.
 *
 * The first thing we do is disable interrupts.  This is to guard our stack against
 * having a context switch interrupt after we have already pushed the registers onto the stack -
 * causing the 31 (32 register + mepc - x0 - x2) registers to be on the stack twice.
 *
 * Then we load the stack pointer of the current task and push all registers to the stack.
 * The last thing we do is to adjust the stack poitner (sp) to accommodate for the memory
 * we used.
 *
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer.
 */

#define portSAVE_CONTEXT()                                            \
     int_disable();                                                   \
     asm volatile ( "addi sp,   sp, -0x6c                       \n\t" \
                    "sw   x1,   0x04(sp)                        \n\t" \
                    "sw   x3,   0x08(sp)                        \n\t" \
                    "sw   x4,   0x0c(sp)                        \n\t" \
                    "sw   x5,   0x10(sp)                        \n\t" \
                    "sw   x6,   0x14(sp)                        \n\t" \
                    "sw   x7,   0x18(sp)                        \n\t" \
                    "sw   x8,   0x1C(sp)                        \n\t" \
                    "sw   x9,   0x20(sp)                        \n\t" \
                    "sw   x10,  0x24(sp)                        \n\t" \
                    "sw   x11,  0x28(sp)                        \n\t" \
                    "sw   x12,  0x2c(sp)                        \n\t" \
                    "sw   x13,  0x30(sp)                        \n\t" \
                    "sw   x14,  0x34(sp)                        \n\t" \
                    "sw   x15,  0x38(sp)                        \n\t" \
                    "sw   x16,  0x3C(sp)                        \n\t" \
                    "sw   x17,  0x40(sp)                        \n\t" \
                    "sw   x18,  0x44(sp)                        \n\t" \
                    "sw   x19,  0x48(sp)                        \n\t" \
                    "sw   x20,  0x4C(sp)                        \n\t" \
                    "sw   x21,  0x50(sp)                        \n\t" \
                    "sw   x22,  0x54(sp)                        \n\t" \
                    "sw   x23,  0x58(sp)                        \n\t" \
                    "sw   x24,  0x5c(sp)                        \n\t" \
                    "sw   x25,  0x60(sp)                        \n\t" \
                    "sw   x26,  0x64(sp)                        \n\t" \
                    "sw   x27,  0x68(sp)                        \n\t" \
                    "sw   x28,  0x6c(sp)                        \n\t" \
                    "sw   x29,  0x70(sp)                        \n\t" \
                    "sw   x30,  0x74(sp)                        \n\t" \
                    "sw   x31,  0x78(sp)                        \n\t" \
                    "csrr t0,   mepc                            \n\t" \
                    "sw   t0,   0x00(sp)                        \n\t" \
                    "lw   t0,   pxCurrentTCB                    \n\t" \
                    "sw   sp,   (t0)                            \n\t" \
                );                                                    \


/*
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer.
 */
#define portRESTORE_CONTEXT()                                         \
    asm volatile (  "lw   sp,   pxCurrentTCB                    \n\t" \
                    "lw   sp,   (sp)                            \n\t" \
                    "lw   t0,   0x00(sp)                        \n\t" \
                    "csrw mepc, t0                              \n\t" \
                    "lw   x1,   0x04(sp)                        \n\t" \
                    "lw   x3,   0x08(sp)                        \n\t" \
                    "lw   x4,   0x0c(sp)                        \n\t" \
                    "lw   x5,   0x10(sp)                        \n\t" \
                    "lw   x6,   0x14(sp)                        \n\t" \
                    "lw   x7,   0x18(sp)                        \n\t" \
                    "lw   x8,   0x1c(sp)                        \n\t" \
                    "lw   x9,   0x20(sp)                        \n\t" \
                    "lw   x10,  0x24(sp)                        \n\t" \
                    "lw   x11,  0x28(sp)                        \n\t" \
                    "lw   x12,  0x2c(sp)                        \n\t" \
                    "lw   x13,  0x30(sp)                        \n\t" \
                    "lw   x14,  0x34(sp)                        \n\t" \
                    "lw   x15,  0x38(sp)                        \n\t" \
                    "lw   x16,  0x3c(sp)                        \n\t" \
                    "lw   x17,  0x40(sp)                        \n\t" \
                    "lw   x18,  0x44(sp)                        \n\t" \
                    "lw   x19,  0x48(sp)                        \n\t" \
                    "lw   x20,  0x4C(sp)                        \n\t" \
                    "lw   x21,  0x50(sp)                        \n\t" \
                    "lw   x22,  0x54(sp)                        \n\t" \
                    "lw   x23,  0x58(sp)                        \n\t" \
                    "lw   x24,  0x5C(sp)                        \n\t" \
                    "lw   x25,  0x60(sp)                        \n\t" \
                    "lw   x26,  0x64(sp)                        \n\t" \
                    "lw   x27,  0x68(sp)                        \n\t" \
                    "lw   x28,  0x6C(sp)                        \n\t" \
                    "lw   x29,  0x70(sp)                        \n\t" \
                    "lw   x30,  0x74(sp)                        \n\t" \
                    "lw   x31,  0x78(sp)                        \n\t" \
                    "addi sp,   sp,             0x7c            \n\t" \
                );                                                    \

/*-----------------------------------------------------------*/

/*
 * Setup mtimcmp and mtime regisiter to periodically generate tick interrupts.
 */
static void prvSetupTimerInterrupt( void );


/*
 * See header file for description.
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
    int usAddress = ( int ) pxCode;

    /* End of stack marker. Useful for debugging - unncecessary for deployment */
    *pxTopOfStack = ( StackType_t ) 0xdeadbeef;
    pxTopOfStack--;

    /* reg x31 -> x2 */
    pxTopOfStack -= 30;

    /* return address: x1*/
    *pxTopOfStack = ( StackType_t ) pxCode;
    pxTopOfStack--;

    /* zero register not pushed to stack - zero anyway */

    /* mepc */
    /* put the start of the code in the mepc register in order to account for a context switch where the task gets
       switch in and has never run before */
    *pxTopOfStack = ( StackType_t ) pxCode;

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
    // Configure ISRs
    //int_init();
    //int_add(TIMER_A_OUTPUT_CMP, (void *) int_time_cmp, 0);

    // enable timer interrupt
    IER = 0xF0000000;

    /* Setup the hardware to generate the tick. */
    prvSetupTimerInterrupt();

    /* Restore the context of the first task that is going to run. */
    portRESTORE_CONTEXT();
    int_enable();
    /* Simulate a function call end as generated by the compiler.  We will now
    jump to the start of the task the context of which we have just restored. */
    asm volatile ( "ret" );

    /* Should not get here. */
    return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    int_disable();
}
/*-----------------------------------------------------------*/

/*
 * Manual context switch.
 */
void vPortYield( void );
void vPortYield( void )
{
    portSAVE_CONTEXT();
    vTaskSwitchContext();
    portRESTORE_CONTEXT();
    int_enable();
    /* called in a non interrupt context only - this function
     accounts for manual context switches */
    // asm volatile ("addi sp,   sp, 0x10                       \n\t");
    asm volatile ( "ret" );
    /* restore from restored context return register
                              we need this manual return since GCC would return to the task
                              it thinks it just left (where it knows where it stored the return address). */
}
/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR - this function is called inside a ISR only
 * and needs to return with the MRET instruction.
 */
void vPortYieldFromTick( void );
void vPortYieldFromTick( void )
{
    // next time the stored task will resume from here
    portSAVE_CONTEXT();
    if ( xTaskIncrementTick() != pdFALSE ) {
        vTaskSwitchContext();
    }
    portRESTORE_CONTEXT();
    int mcause;
    csrr(mcause, mcause);
    // clear pending register
    ICP = (1 << mcause);

    // this is hacky but needed since we would run directly into
    // another interrupt if we didn't wait long enough.
    for (int i = 0; i < 6; i++)
        asm volatile ("nop");
    int_enable();
    /* we do need this since the return from interrupt is not handled
       in the crt0 runtime - because the call to the timer ISR is naked */
    /* return address is restored from stack to the empc register */
    // asm volatile ("addi sp,   sp, 0x10                       \n\t");
    asm volatile ( "mret" );
}
/*-----------------------------------------------------------*/

/*
 * Setup mtime and mtimecmp registers accordingly.
 */
static void prvSetupTimerInterrupt( void )
{
    unsigned int CompareMatch;

    CompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

    /* Setup Timer A */
    TOCRA = CompareMatch;
    TPRA  = 0x27; /* Timer A - enable interrupts, start timer */

}
/*-----------------------------------------------------------*/

#if configUSE_PREEMPTION == 1

/*
 * Tick ISR for preemptive scheduler.  We can use a naked attribute as
 * the context is saved at the start of vPortYieldFromTick().  The tick
 * count is incremented after the context is saved.
 */

void ISR_TA_CMP(void)
{
    /* interrupts are disabled until eret */
    vPortYieldFromTick();

}
#else
/*
 * Tick ISR for the cooperative scheduler.  All this does is increment the
 * tick count.  We don't need to switch context, this can only be done by
 * manual calls to taskYIELD();
 */

void ISR_TA_CMP(void)
{
    int mcause;
    csrr(mcause, mcause);
    // clear pending register
    ICP = (1 << mcause);
    xTaskIncrementTick();

    /* return address is restored from stack to the empc register */
    asm volatile ( "mret" );
}
#endif



