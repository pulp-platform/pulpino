
#include "Wire.h"
#include <main.cpp>


volatile int x,y,i;
unsigned char trData[18]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int recData[16];
void setup()
{

	Wire.begin();
	printf("Init \n");

	Wire.beginTransmission(0x50);
	Wire.write(0);
	Wire.write(0);
	Wire.write(trData, 16);
	y=Wire.endTransmission();
	printf("Error= %d \n",y);

	Wire.beginTransmission(0x50);
	Wire.write(0);
	Wire.write(0);
	y=Wire.endTransmission();
	printf("Error= %d \n",y);

	y=Wire.requestFrom(0x50, 16);
	printf("Recieved Data Length= %d \n",y);
	
	for(i=0; i<16; i++)	
		if(Wire.available())	
			recData[i]=Wire.read();
		


	for(i=0; i<16; i++)
		printf("Received %d expecting %d\n", recData[i], i);
}

void loop()
{
x++;
}



