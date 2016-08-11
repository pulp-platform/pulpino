extern "C" {
#include "twi.h"
}
#include <main.cpp>


volatile int x,y;
unsigned char trData[18]={0x00, 0x00, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
void setup()
{

        twi_init();
        printf("Init \n");
	y= twi_writeTo(0x50, trData, 18, 1, 1);	
	printf("Error= %d \n",y);
}

void loop()
{
x++;
}



