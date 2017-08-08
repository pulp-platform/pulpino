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


void setup(){
pinMode(0,OUTPUT);
pinMode(1,OUTPUT);
pinMode(2,OUTPUT);
}


void loop(){
digitalWrite(0,HIGH);
analogWrite(1,50);
analogWrite(2,200);
delay(20);
analogWrite(0,125);
analogWrite(1,300);
digitalWrite(2,LOW);
delay(20);	
exit(0);	//to end simulation properly
}

