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


#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "gpio.h"

int main()
{
  set_pin_function(PIN_MSPI_CSN1, FUNC_EXT2);
  if (get_pin_function(PIN_MSPI_CSN1) == FUNC_EXT2) {
     printf("Successfully enabled func 2 on PIN_MSPI_CSN1\n");
  } else {
     printf("ERROR on enabling func 2 on PIN_MSPI_CSN1\n");
  }

  set_pin_function(PIN_MSPI_CSN2, FUNC_GPIO);
  set_gpio_pin_direction(PIN_MSPI_CSN2, DIR_OUT);
  if (get_gpio_pin_direction(PIN_MSPI_CSN2) == DIR_OUT) {
     printf("Successfully set out dir on PIN_MSPI_CSN2\n");
  } else {
     printf("ERROR on setting out dir on PIN_MSPI_CSN2\n");
  }

  printf("Done!!!\n");

  return 0;
}
