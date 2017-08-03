// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


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
