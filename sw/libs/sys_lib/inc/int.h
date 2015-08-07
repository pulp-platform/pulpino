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

/* Enable/Disable interrupts */
void int_enable(void);
void int_disable(void);

/* Actual interrup handler function */
void int_main();
#endif // _INT_H_
