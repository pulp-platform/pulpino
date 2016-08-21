
#include "SPI.h"

#include <main.cpp>


int y;

void setup()
{
  Serial.begin(781250);
  SPI.begin();
}

void loop()
{
  SPISettings mySettings(4000000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySettings);
  y=SPI.transfer(0xA1);
  Serial.print("Received=");
  Serial.print(y);
  Serial.print("\tExpected=");
  Serial.println("28");
  SPI.endTransaction();
  delay(1);

  if (y == 28)
    exit(0);
  else
    exit(1);
}
