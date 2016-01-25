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