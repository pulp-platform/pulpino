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

#include "SPI.h"

#include <main.cpp>


int y;

void setup()
{
  Serial.begin(781250);
  SPI.begin();
}

void loop()
{
  SPISettings mySettings(4000000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySettings);
  y=SPI.transfer(0xA1);
  Serial.print("Received=");
  Serial.print(y);
  Serial.print("\tExpected=");
  Serial.println("28");
  SPI.endTransaction();
  delay(1);

  if (y == 28)
    exit(0);
  else
    exit(1);
}
