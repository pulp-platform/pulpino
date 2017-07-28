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



