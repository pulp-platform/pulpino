/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

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
