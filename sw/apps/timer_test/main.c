
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "int.h"


int main()
{
	int reg, val, tmp;
	int_enable();

	// 0. mstatus -- interrupts enabled
	asm volatile ("csrr %0, mstatus" : "=r" (reg));
	if (reg % 2 != 1) 
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 0: Passed\n");

	// 1. testing read to csr
	asm volatile ("csrr %0, 0x321" : "=r" (reg));
	if (reg != 0x00)
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 1: Passed\n");
	
	// 2. testing write to csr
	val = 0x55;
	asm volatile ("csrw 0x321, %1\n"
				  "csrr %0, 0x321\n" 
				  : "=r" (reg) 
				  : "r" (val)
				 );
	
	if (reg != val)
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 2: Passed\n");

	// 3. testing timer count

	asm volatile ("csrr %0, 0x701\n" : "=r" (reg));
	asm volatile ("csrr %0, 0x701" : "=r" (val));
	
	if (val == reg)
		printf("[ERROR] Timer doesn't count. Timer value %d and %d\n", reg, val);
	else printf("Test 3: Passed\n");
	
	val = 0x00;
	// 4. write to timer register
	int_disable(); // we want to stay atomic
	asm volatile ("csrw 0x701, %1\n"
				  "csrr %0, 0x701\n" 
				  : "=r" (reg) 
				  : "r" (val)
				 );
	int_enable();

	if (val != reg - 1)
		printf("[ERROR] Timer is %d\n", reg);
	else printf("Test 4: Passed\n");
	
	
	// 5. test interrupt calling

	//write to mtimecmp
	val = 0x100;
	asm volatile ("csrw mtimecmp, %0" : /* no output */ : "r" (val));
	val = 0x40;
	asm volatile ("csrw mtime, %0" : /* no output */ : "r" (val)); 

	// wait a few cycles
	for (int i = 0; i < 100; i++)
		asm volatile ("nop\n"
					  "nop\n"
					  );
	
	printf("Finished timer testing.\n");

  return 0;
}
