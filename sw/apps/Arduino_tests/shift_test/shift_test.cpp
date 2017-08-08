// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <main.cpp>

volatile uint8_t x;

void setup()
{
  Serial.begin(781250);
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,INPUT);
  pinMode(4,OUTPUT);
}

void loop()
{
  shiftOut(1, 2, 0, 5);
  digitalWrite(0,HIGH);
  x=shiftIn(3, 4, 0);
  Serial.print("Received\t");
  Serial.print(x);
  Serial.print("\texpected\t");
  Serial.println("147");

  delay(1);

  exit(0);	//to end simulation properly

}
