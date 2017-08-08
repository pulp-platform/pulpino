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

int x;

void setup()
{
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
  pinMode(7,INPUT);	//used to terminate test bench (for simulation only)
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);
  Serial.begin(781250);
}

void loop()
{
  x=digitalRead(4);
  digitalWrite(1,HIGH);
  digitalWrite(2,HIGH);
  digitalWrite(3,x);
  if (digitalRead(7)) {
    Serial.print("TEST GPIO Success\n");
    delay(3);
    exit(0);	//to end simulation properly
  }
  else {
    Serial.print("TEST Failed Success\n");
    delay(3);
    exit(-1);  //to end simulation properly
  }
}



