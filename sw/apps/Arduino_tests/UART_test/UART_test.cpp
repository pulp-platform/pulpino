// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "main.cpp"

byte thisByte=0;


void setup() {
  pinMode(0,OUTPUT);
  Serial.begin(781250);
}

void loop(){
  digitalWrite(0,HIGH);

  delayMicroseconds(100);

  if (Serial.available())
    thisByte=Serial.read();

  Serial.write(thisByte);
  Serial.print(", dec: ");
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:
  // Serial.print(thisByte, DEC);

  Serial.print(", hex: ");
  // prints value as string in hexadecimal (base 16):
  Serial.print(thisByte, HEX);

  Serial.print(", oct: ");
  // prints value as string in octal (base 8);
  Serial.print(thisByte, OCT);

  Serial.print(", bin: ");
  // prints value as string in binary (base 2)
  // also prints ending line break:
  Serial.println(thisByte, BIN);

  if(thisByte == 0x65){
    Serial.print("TEST Success\n");
    delay(1);
    exit(0);
  } else{
    Serial.print("TEST Fail\n");
    delay(1);
    exit(-1);
  }
}
