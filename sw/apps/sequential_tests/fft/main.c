// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#include "kiss_fft.h"
#include "kiss_fftr.h"
#include "_kiss_fft_guts.h"
#include <stdint.h>
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "timer.h"

#include "stimuli.h"
#include "expresp.h"

extern struct kiss_fftr_state g_fftr_state;
extern struct kiss_fftr_state g_ifftr_state;

__attribute__ ((section(".heapsram"))) kiss_fft_cpx freq[257];
__attribute__ ((section(".heapsram"))) int16_t buffer_out[516];

//--------------------------------------------------------------------------
// main function
//--------------------------------------------------------------------------
int main() {
  unsigned int i;
  kiss_fftr_cfg fftr_cfg = &g_fftr_state;
  kiss_fftr_cfg ifftr_cfg = &g_ifftr_state;

  int core_id = get_core_id();

  unsigned int errors = 0;

  synch_barrier();

  // Work!
  if(core_id == 0) {
    reset_timer();
    start_timer();

    // perform FFT
    kiss_fftr(fftr_cfg, g_stimuli, freq);

    // perform IFFT
    kiss_fftri(ifftr_cfg, freq, buffer_out);
    stop_timer();
  }

  synch_barrier();

  if(core_id == 0) {
    // now check the responses
    for(i = 0; i < 512; i++) {
      if((buffer_out[i] & 0xFFFF) != (g_expresp[i] & 0xFFFF)) {
        errors++;
        printf("Error: Is %X, expected %X, at %X\n", buffer_out[i], g_expresp[i], &g_expresp[i], 0);
      }
    }

    printf("#Cycles %d, %d\n", get_time_hi(), get_time(), 0, 0);

    if(errors) {
      printf("Core %d, %d errors found\n", core_id, errors, 0, 0);
      printf("NOT OK!!!!!\n");
    } else {
      printf("Core %d, OK\n", core_id, 0, 0, 0);
      printf ("OOOOOOK!!!!!!\n",0,0,0,0);
    }

  }

  synch_barrier();
  eoc(0);

  return 0;
}
