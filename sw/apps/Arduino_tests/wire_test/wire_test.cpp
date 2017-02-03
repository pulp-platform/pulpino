
#include "Wire.h"
#include <main.cpp>


int y,i;
unsigned char trData[18]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
int recData[16];
void setup()
{
   	Serial.begin(781250);
	Wire.begin();
	Serial.println("Init");
}

void loop()
{
	Wire.beginTransmission(0x50);
	Wire.write(0);
	Wire.write(0);
	Wire.write(trData, 16);
	y=Wire.endTransmission();
	Serial.print("Error=");
   	Serial.println(y);

	Wire.beginTransmission(0x50);
	Wire.write(0);
	Wire.write(0);
	y=Wire.endTransmission();
	Serial.print("Error=");
   	Serial.println(y);

	y=Wire.requestFrom(0x50, 16);
	Serial.print("Received data length=");
   	Serial.println(y);
	
	for(i=0; i<16; i++)	
		if(Wire.available())	
			recData[i]=Wire.read();
		


	for(i=0; i<16; i++){
		Serial.print("Received=");
   	   	Serial.print(recData[i]);
  		Serial.print("\tExpected=");
   	   	Serial.println(i);
	}

        delay(1);
        exit(0);

}



