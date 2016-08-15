
#include "SPI.h"

#include <main.cpp>


volatile int x,y;

void setup()
{

	delay(5);
	SPISettings mySettings(4000000, MSBFIRST, SPI_MODE0);
	SPI.beginTransaction(mySettings);
	printf("Init \n");
	SPI.begin();
	y=SPI.transfer(0xAA);
	printf("Received= %d \n",y);
	SPI.endTransaction();
	printf("End \n");
}

void loop()
{
x++;
}



