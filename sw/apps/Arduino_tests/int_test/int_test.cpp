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

// self test variables
int irqPinToggles = 0;

const byte ledPin = 13;
const byte interruptPin1 = 1;
const byte interruptPin2 = 2;
byte state = LOW;

void blink() {
  state = !state;

  irqPinToggles++;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin1, INPUT);
  pinMode(interruptPin2, INPUT);
  attachInterrupt(interruptPin1, blink, RISING);
  attachInterrupt(interruptPin2, blink, RISING);
  pinMode(0,OUTPUT);	//used to intiate test bench stimulus (for simulation only)
  digitalWrite(0,HIGH);
  Serial.begin(781250);
}

void loop() {
  digitalWrite(ledPin, state);

  // check for test condition
  if (irqPinToggles == 3){
    Serial.print("TEST Success\n");
    delay(1);
    exit(0);	//last signal won't be shown in modelsim as simulation will end
  }
}
