extern "C" {
#include "twi.h"
}
#include <main.cpp>


volatile int x,y,i;
unsigned char trData[18]={0x00, 0x00, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
unsigned char recData[16];
void setup()
{

        twi_init();
        printf("Init \n");
	y= twi_writeTo(0x50, trData, 18, 1, 1);	
	printf("Error= %d \n",y);
	y= twi_writeTo(0x50, trData, 2, 1, 1);	
        printf("Error= %d \n",y);
	y= twi_readFrom(0x50, recData, 16, 1);
        printf("Recieved Data Length= %d \n",y);
	for(i=0; i<16; i++)
		printf("Received %d expecting %d\n", recData[i], i);
}

void loop()
{
x++;
}



