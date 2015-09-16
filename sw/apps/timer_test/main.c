
#include "string_lib.h"
#include "utils.h"
#include "uart.h"
#include "int.h"

volatile char status = 0x00;

void int_time_cmp(void) {
	status += 1;
}

int main()
{
	int reg, val, tmp;
	int_enable();

	// 0. mstatus -- interrupts enabled
	csrr(mstatus, reg);
	if (reg % 2 != 1) 
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 0: Passed\n");

	// 1. testing read to csr
	csrr(mtimecmp, reg);
	if (reg != 0x00)
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 1: Passed\n");
	
	// 2. testing write to csr
	val = 0x55;
	csrw(mtimecmp, val);
	csrr(mtimecmp, reg);

	if (reg != val)
		printf("[ERROR] Register content is %d\n", reg);
	else printf("Test 2: Passed\n");

	// 3. testing timer count
	csrr(mtime, reg);
	csrr(mtime, val);

	if (val == reg)
		printf("[ERROR] Timer doesn't count. Timer value %d and %d\n", reg, val);
	else printf("Test 3: Passed\n");
	
	val = 0x00;
	// 4. write to timer register
	int_disable(); // we want to stay atomic
	csrw(mtime, val);
	csrr(mtime, reg);
	int_enable();

	if (val != reg - 1)
		printf("[ERROR] Timer is %d\n", reg);
	else printf("Test 4: Passed\n");
	
	
	// 5. test interrupt calling

	//write to mtimecmp
	val = 0x100;
	csrw(mtimecmp, val);
	val = 0x40;
	csrw(mtime, val);
	// wait a few cycles
	status  = 0;
	while (status < 5);
	int_disable();
	printf("Test 5: Passed\n");

  return 0;
}
