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

unsigned int x;

void setup()
{
  Serial.begin(781250);

  
  pinMode(4,INPUT);
 
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);
  
}

void loop()
{
  x= pulseIn(4,1);
  Serial.print("Calculated\t");
  Serial.print(x);
  Serial.print("\texpected\t");
  Serial.println("500");

 
  x= pulseInLong(4,1);
  Serial.print("Calculated\t");
  Serial.print(x);
  Serial.print("\texpected\t");
  Serial.println("500");

  delay(1);

  exit(0);	//to end simulation properly
}



