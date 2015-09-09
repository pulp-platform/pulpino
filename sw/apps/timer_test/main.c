
#include "string_lib.h"
#include "utils.h"
#include "uart.h"

int main()
{
	uint32_t reg, val;
 	//start_timer();
	asm volatile ("csrr %0, 0x321" : "=r" (reg));
	printf("Register content is %d\n", reg);
	val = 55;
	asm volatile ("csrw 0x321, %1\n"
				  "csrr %0, 0x321\n" 
				  : "=r" (reg) 
				  : "r" (val)
				 );
	
	printf("Register content is now %d\n", reg);
	printf("Hello World!!!!!\n");

  return 0;
}
