
#include "SPI.h"

#include <main.cpp>


volatile int x,y;

void setup()
{

	delay(1);
	SPI.begin();
	SPISettings mySettings(4000000, MSBFIRST, SPI_MODE0);
	SPI.beginTransaction(mySettings);
	y=SPI.transfer(0xA1);
	printf("Received= %d \n",y);
	SPI.endTransaction();
	printf("End \n");
}

void loop()
{
x++;
}



