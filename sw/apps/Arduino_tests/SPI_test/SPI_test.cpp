// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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

  if (y == 28) {
    Serial.print("Test Success\n");
    delay(1);
    exit(0);
  }
  else {
    Serial.print("Test Fail\n");
    delay(1);
    exit(1);
  }
}
