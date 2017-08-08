// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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