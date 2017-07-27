/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define DELAY_LOOP 10000
#define TRUE 1

void task1 (void *pvParameters) {

	while(TRUE) {

		printf("Task 1\n");

		taskYIELD();

		for(int i = 0; i < DELAY_LOOP; i++)
			portNOP();
	}

	vTaskDelete(NULL);

}

void task2 (void *pvParameters) {

	while(TRUE) {

		printf("Task 2\n");

		taskYIELD();

		for(int i = 0; i < DELAY_LOOP; i++)
			portNOP();
	}

	vTaskDelete(NULL);
}


int main( void )
{

	printf("Starting FreeRTOS\n");
	xTaskCreate(task1, "Task 1", 100, NULL, 1, NULL);
 	xTaskCreate(task2, "Task 2", 100, NULL, 1, NULL);
	vTaskStartScheduler();

	return 0;
}