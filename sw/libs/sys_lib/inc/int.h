/**
 * \brief Interrupt library for PULPino.
 * 
 * Contains various interrupt manipulating functions.
 *
 * \author Name
 *
 * \version 1.0
 *
 * \date 11/9/2015
 *
 */

#ifndef _INT_H_
#define _INT_H_

/* Number of interrupt handlers - really depends on PIC width in OR1200*/
#define MAX_INT_HANDLERS	32

/* Handler entry */
struct ihnd {
	void 	(*handler)(void *);
	void	*arg;
};

/* Add interrupt handler */ 
int int_add(unsigned long vect, void (* handler)(void *), void *arg);

/* Add exception vector handler */
void add_handler(unsigned long vector, void (* handler) (void));

/* Initialize routine */
void int_init();

/** 
 * \brief Disables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit 
 * interrupts are globally disable. 
 */
static inline void int_disable(void) {
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus &= 0xFFFFFFFE;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
}

/** 
 * \brief Enables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit 
 * interrupts are globally enabled. 
 */
static inline void int_enable(void) {
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus |= 0x01;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
}

/* Actual interrup handler function */
void int_main();
#endif // _INT_H_
