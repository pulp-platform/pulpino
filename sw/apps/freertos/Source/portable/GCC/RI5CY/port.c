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
#include "int.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the AVR port.
 *----------------------------------------------------------*/

/* Start tasks with interrupts enables. */
#define portFLAGS_INT_ENABLED					( ( StackType_t ) 0x80 )

/* Hardware constants for timer 1. */
#define portCLEAR_COUNTER_ON_MATCH				( ( uint8_t ) 0x08 )
#define portPRESCALE_64							( ( uint8_t ) 0x03 )
#define portCLOCK_PRESCALER						( ( uint32_t ) 64 )
#define portCOMPARE_MATCH_A_INTERRUPT_ENABLE	( ( uint8_t ) 0x10 )

/*-----------------------------------------------------------*/

/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/*-----------------------------------------------------------*/

/* 
 * Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.  
 * 
 * The first thing we do is save the flags then disable interrupts.  This is to 
 * guard our stack against having a context switch interrupt after we have already 
 * pushed the registers onto the stack - causing the 32 registers to be on the 
 * stack twice. 
 * 
 * r1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1. 
 * 
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer. 
 */

  // gp
    // tp
    // t0
    // t1
    // t2
    // a0
    // a1
    // a2
    // a3
    // a4
    // a5
    // a6
    // a7
    // t3
    // t4
    // t5
    // t6

#define portSAVE_CONTEXT() /*											\
	 asm volatile (	"sw  x3, 0x00(x2)						\n\t"	\
					"sw  x4, 0x04(x2)						\n\t"	\
					"sw  x5, 0x08(x2)						\n\t"	\
					"sw  x6, 0x0c(x2)						\n\t"	\
					"sw  x7, 0x10(x2)						\n\t"	\
					"sw x10, 0x14(x2)						\n\t"	\
					"sw x11, 0x18(x2)						\n\t"	\
					"sw x12, 0x1c(x2)						\n\t"	\
					"sw x13, 0x20(x2)						\n\t"	\
					"sw x14, 0x24(x2)						\n\t"	\
					"sw x15, 0x28(x2)						\n\t"	\
					"sw x16, 0x2c(x2)						\n\t"	\
					"sw x17, 0x30(x2)						\n\t"	\
					"sw x28, 0x34(x2)						\n\t"	\
					"sw x29, 0x38(x2)						\n\t"	\
					"sw x30, 0x3c(x2)						\n\t"	\
					"sw x31, 0x40(x2)						\n\t"	\
				);													\
	*/
/* 
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer. 
 */

#define portRESTORE_CONTEXT()		int_enable();								/*\
	asm volatile (	"lw  x3, 0x00(x2)						\n\t"	\
					"lw  x4, 0x04(x2)						\n\t"	\
					"lw  x5, 0x08(x2)						\n\t"	\
					"lw  x6, 0x0c(x2)						\n\t"	\
					"lw  x7, 0x10(x2)						\n\t"	\
					"lw x10, 0x14(x2)						\n\t"	\
					"lw x11, 0x18(x2)						\n\t"	\
					"lw x12, 0x1c(x2)						\n\t"	\
					"lw x13, 0x20(x2)						\n\t"	\
					"lw x14, 0x24(x2)						\n\t"	\
					"lw x15, 0x28(x2)						\n\t"	\
					"lw x16, 0x2c(x2)						\n\t"	\
					"lw x17, 0x30(x2)						\n\t"	\
					"lw x28, 0x34(x2)						\n\t"	\
					"lw x29, 0x38(x2)						\n\t"	\
					"lw x30, 0x3c(x2)						\n\t"	\
					"lw x31, 0x40(x2)						\n\t"	\
					"lw  x1, 0x44(x2)						\n\t"	\
				);										    		\
				int_enable(); 										\
*/
/*-----------------------------------------------------------*/

/*
 * Perform hardware setup to enable ticks from timer 1, compare match A.
 */
static void prvSetupTimerInterrupt( void );

/*
 * Read global pointer
 */
static void prvReadGp( int *ulValue )
{
	asm volatile( "mv  x3, %0" :: "r"(ulValue) );
}

/* 
 * See header file for description. 
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	int usAddress = ( int ) pxCode;
	StackType_t xGlobalPointer;

	/* read global pointer */
    prvReadGp( &xGlobalPointer );
	
	/* End of stack marker. */
    *pxTopOfStack = 0xdeadbeef;
    pxTopOfStack--;

    /* zero */
    *pxTopOfStack = ( StackType_t ) 0x00;	/* R1 */
	pxTopOfStack--;

    /* return address*/
    *pxTopOfStack = ( StackType_t ) pxCode; 
    pxTopOfStack--;
    /* stack pointer */
	StackType_t *pxFramePointer = pxTopOfStack - 1;
	*pxTopOfStack = ( StackType_t ) pxFramePointer; 
    pxTopOfStack--;

    /* global pointer */
    *pxTopOfStack = xGlobalPointer;

    /* reg x4 -> x18 */
    pxTopOfStack -= 28;



	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick. */
	prvSetupTimerInterrupt();

	/* Restore the context of the first task that is going to run. */
	portRESTORE_CONTEXT();

	/* Simulate a function call end as generated by the compiler.  We will now
	jump to the start of the task the context of which we have just restored. */
	asm volatile ( "ret" );

	/* Should not get here. */
	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the AVR port will get stopped.  If required simply
	disable the tick interrupt here. */
}
/*-----------------------------------------------------------*/

/*
 * Manual context switch.  The first thing we do is save the registers so we
 * can use a naked attribute.
 */
void vPortYield( void );
void vPortYield( void )
{
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();

	//asm volatile ( "eret" );
}
/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to 
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void vPortYieldFromTick( void );
void vPortYieldFromTick( void )
{
	portSAVE_CONTEXT();
	if( xTaskIncrementTick() != pdFALSE )
	{
		vTaskSwitchContext();
	}
	portRESTORE_CONTEXT();

	//asm volatile ( "ret" );
}
/*-----------------------------------------------------------*/

/*
 * Setup timer 1 compare match A to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt( void )
{
	
	unsigned int CompareMatch, time = 0x00;
	
	CompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;
	/* timer interrupts are automatically enabled if mtimecmp is different to 0x00
	- this is okay as interrupt are currently globally
	disabled. */
	csrw(mtimecmp, CompareMatch);
	csrw(mtime, time);

}
/*-----------------------------------------------------------*/

#if configUSE_PREEMPTION == 1

	/*
	 * Tick ISR for preemptive scheduler.  We can use a naked attribute as
	 * the context is saved at the start of vPortYieldFromTick().  The tick
	 * count is incremented after the context is saved.
	 */
	void int_time_cmp(void)
	{
		vPortYieldFromTick();
		// we do not need this since the return from interrupt is handled
		// in the cr0 runtime
		asm volatile ( "eret" );
	}
#else

	/*
	 * Tick ISR for the cooperative scheduler.  All this does is increment the
	 * tick count.  We don't need to switch context, this can only be done by
	 * manual calls to taskYIELD();
	 */
	void int_time_cmp(void)
	{
		xTaskIncrementTick();
		asm volatile ( "eret" );
	}
#endif


	
