// Copyright 2015 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "bar.h"
#include "string_lib.h"
#include "utils.h"

volatile unsigned int setme = 1;
volatile unsigned int setme2 = 1;

int main() {

  asm volatile ("EBREAK");
  asm volatile ("EBREAK");

  while(setme);

  printf("Hello!\n");

  while(setme2);

  return 0;
}
