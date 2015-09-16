/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

int main( void )
{
	/* Configure the hardware ready to run the demo. */
	//prvSetupHardware();

	/* The mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is described at the top
	of this file. */
	//#if( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 1 )
	//{
	//	main_blinky();
	//}
	//#else
	//{
	//	main_full();
	//}
	//#endif
	vTaskStartScheduler();
	
	return 0;
}