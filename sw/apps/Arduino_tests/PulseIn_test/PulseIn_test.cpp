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



