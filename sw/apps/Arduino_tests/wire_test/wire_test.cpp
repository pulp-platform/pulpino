// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
    int err = 0;
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
        err = err + (recData[i] != i);
    }

    if(err == 0){
        Serial.println("Test Success");
        delay(1);
        exit(0);
    } else {
        Serial.println("Test Fail");
        delay(1);
        exit(-1);
    }

}



