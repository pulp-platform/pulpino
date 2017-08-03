// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.


// This testbench checks the basic functionality of:
//


#include <stdio.h>
#include "utils.h"
#include "bench.h"
#ifdef __riscv__
#include "testDotMul_stimuli_riscv.h"
#else
#include "testDotMul_stimuli.h"
#endif

#ifdef __riscv__
#define DOTP_H "pv.dotsp.h"
#define DOTP_H_SC "pv.dotsp.sc.h"
#define DOTP_H_SCI "pv.dotsp.sci.h"
#define DOTUP_H "pv.dotup.h"
#define DOTUP_H_SC "pv.dotup.sc.h"
#define DOTUP_H_SCI "pv.dotup.sci.h"
#define DOTUSP_H "pv.dotusp.h"
#define DOTUSP_H_SC "pv.dotusp.sc.h"
#define DOTUSP_H_SCI "pv.dotusp.sci.h"

#define SDOTP_H "pv.sdotsp.h"
#define SDOTP_H_SC "pv.sdotsp.sc.h"
#define SDOTP_H_SCI "pv.sdotsp.sci.h"
#define SDOTUP_H "pv.sdotup.h"
#define SDOTUP_H_SC "pv.sdotup.sc.h"
#define SDOTUP_H_SCI "pv.sdotup.sci.h"
#define SDOTUSP_H "pv.sdotusp.h"
#define SDOTUSP_H_SC "pv.sdotusp.sc.h"
#define SDOTUSP_H_SCI "pv.sdotusp.sci.h"

#define DOTP_B "pv.dotsp.b"
#define DOTP_B_SC "pv.dotsp.sc.b"
#define DOTP_B_SCI "pv.dotsp.sci.b"
#define DOTUP_B "pv.dotup.b"
#define DOTUP_B_SC "pv.dotup.sc.b"
#define DOTUP_B_SCI "pv.dotup.sci.b"
#define DOTUSP_B "pv.dotusp.b"
#define DOTUSP_B_SC "pv.dotusp.sc.b"
#define DOTUSP_B_SCI "pv.dotusp.sci.b"

#define SDOTP_B "pv.sdotsp.b"
#define SDOTP_B_SC "pv.sdotsp.sc.b"
#define SDOTP_B_SCI "pv.sdotsp.sci.b"
#define SDOTUP_B "pv.sdotup.b"
#define SDOTUP_B_SC "pv.sdotup.sc.b"
#define SDOTUP_B_SCI "pv.sdotup.sci.b"
#define SDOTUSP_B "pv.sdotusp.b"
#define SDOTUSP_B_SC "pv.sdotusp.sc.b"
#define SDOTUSP_B_SCI "pv.sdotusp.sci.b"
#else
#define DOTP_H "lv32.dotp.h"
#define DOTP_H_SC "lv32.dotp.h.sc"
#define DOTP_H_SCI "lv32.dotp.h.sci"
#define DOTUP_H "lv32.dotup.h"
#define DOTUP_H_SC "lv32.dotup.h.sc"
#define DOTUP_H_SCI "lv32.dotup.h.sci"
#define DOTUSP_H "lv32.dotusp.h"
#define DOTUSP_H_SC "lv32.dotusp.h.sc"
#define DOTUSP_H_SCI "lv32.dotusp.h.sci"

#define SDOTP_H "lv32.sdotp.h"
#define SDOTP_H_SC "lv32.sdotp.h.sc"
#define SDOTP_H_SCI "lv32.sdotp.h.sci"
#define SDOTUP_H "lv32.sdotup.h"
#define SDOTUP_H_SC "lv32.sdotup.h.sc"
#define SDOTUP_H_SCI "lv32.sdotup.h.sci"
#define SDOTUSP_H "lv32.sdotusp.h"
#define SDOTUSP_H_SC "lv32.sdotusp.h.sc"
#define SDOTUSP_H_SCI "lv32.sdotusp.h.sci"

#define DOTP_B "lv32.dotp.b"
#define DOTP_B_SC "lv32.dotp.b.sc"
#define DOTP_B_SCI "lv32.dotp.b.sci"
#define DOTUP_B "lv32.dotup.b"
#define DOTUP_B_SC "lv32.dotup.b.sc"
#define DOTUP_B_SCI "lv32.dotup.b.sci"
#define DOTUSP_B "lv32.dotusp.b"
#define DOTUSP_B_SC "lv32.dotusp.b.sc"
#define DOTUSP_B_SCI "lv32.dotusp.b.sci"

#define SDOTP_B "lv32.sdotp.b"
#define SDOTP_B_SC "lv32.sdotp.b.sc"
#define SDOTP_B_SCI "lv32.sdotp.b.sci"
#define SDOTUP_B "lv32.sdotup.b"
#define SDOTUP_B_SC "lv32.sdotup.b.sc"
#define SDOTUP_B_SCI "lv32.sdotup.b.sci"
#define SDOTUSP_B "lv32.sdotusp.b"
#define SDOTUSP_B_SC "lv32.sdotusp.b.sc"
#define SDOTUSP_B_SCI "lv32.sdotusp.b.sci"
#endif

void check_dotp_h        (testresult_t *result, void (*start)(), void (*stop) () );
void check_dotup_h       (testresult_t *result, void (*start)(), void (*stop) () );
void check_dotusp_h      (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotp_h       (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotup_h      (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotusp_h     (testresult_t *result, void (*start)(), void (*stop) () );

void check_dotp_b        (testresult_t *result, void (*start)(), void (*stop) () );
void check_dotup_b       (testresult_t *result, void (*start)(), void (*stop) () );
void check_dotusp_b      (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotp_b       (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotup_b      (testresult_t *result, void (*start)(), void (*stop) () );
void check_sdotusp_b     (testresult_t *result, void (*start)(), void (*stop) () );

testcase_t testcases[] = {

{ .name = "dotp_h"         , .test = check_dotp_h    },
{ .name = "dotup_h"        , .test = check_dotup_h   },
{ .name = "dotusp_h"       , .test = check_dotusp_h  },
{ .name = "sdotp_h"        , .test = check_sdotp_h   },
{ .name = "sdotup_h"       , .test = check_sdotup_h  },
{ .name = "sdotusp_h"      , .test = check_sdotusp_h },
{ .name = "dotp_b"         , .test = check_dotp_b    },
{ .name = "dotup_b"        , .test = check_dotup_b   },
{ .name = "dotusp_b"       , .test = check_dotusp_b  },
{ .name = "sdotp_b"        , .test = check_sdotp_b   },
{ .name = "sdotup_b"       , .test = check_sdotup_b  },
{ .name = "sdotusp_b"      , .test = check_sdotusp_b },
{0, 0}
};

int main()
{
  int retval = 0;
  if(get_core_id() == 0) {
    retval = run_suite(testcases);
  }

  synch_barrier();

  return retval;
}


//################################################################################
//#  test lv32.dotp{.sc, .sci}.h
//################################################################################

void check_dotp_h        (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotp_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotp_h", res, g_dotp_h_exp[i]);

    asm volatile (DOTP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotp_h", res, g_dotp_h_sc_exp[i]);

 }


    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotp_h_imm_0));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[0]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotp_h_imm_1));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[1]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotp_h_imm_2));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[2]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotp_h_imm_3));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[3]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotp_h_imm_4));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[4]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotp_h_imm_5));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[5]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotp_h_imm_6));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[6]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotp_h_imm_7));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[7]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotp_h_imm_8));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[8]);

    asm volatile (DOTP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotp_h_imm_9));

    check_uint32(result, "dotp_h", res, g_dotp_h_sci_exp[9]);



}
void check_dotup_h      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotup_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTUP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotup_h", res, g_dotup_h_exp[i]);

    asm volatile (DOTUP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotup_h", res, g_dotup_h_sc_exp[i]);

 }


    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotup_h_imm_0));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[0]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotup_h_imm_1));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[1]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotup_h_imm_2));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[2]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotup_h_imm_3));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[3]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotup_h_imm_4));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[4]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotup_h_imm_5));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[5]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotup_h_imm_6));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[6]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotup_h_imm_7));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[7]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotup_h_imm_8));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[8]);

    asm volatile (DOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotup_h_imm_9));

    check_uint32(result, "dotup_h", res, g_dotup_h_sci_exp[9]);



}
void check_dotusp_h      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotusp_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTUSP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_exp[i]);

    asm volatile (DOTUSP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sc_exp[i]);

 }


    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotp_h_imm_0));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[0]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotp_h_imm_1));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[1]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotp_h_imm_2));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[2]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotp_h_imm_3));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[3]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotp_h_imm_4));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[4]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotp_h_imm_5));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[5]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotp_h_imm_6));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[6]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotp_h_imm_7));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[7]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotp_h_imm_8));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[8]);

    asm volatile (DOTUSP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotp_h_imm_9));

    check_uint32(result, "dotusp_h", res, g_dotusp_h_sci_exp[9]);

}


//################################################################################
//#  test lv32.dotp{.sc, .sci}
//################################################################################

void check_sdotp_h        (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotp_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    res = g_dotp_h_c[i];
    asm volatile (SDOTP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_exp[i]);

    res = g_dotp_h_c[i];
    asm volatile (SDOTP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sc_exp[i]);

 }

    res = g_dotp_h_c[0];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotp_h_imm_0));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[0]);

    res = g_dotp_h_c[1];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotp_h_imm_1));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[1]);

    res = g_dotp_h_c[2];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotp_h_imm_2));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[2]);

    res = g_dotp_h_c[3];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotp_h_imm_3));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[3]);

    res = g_dotp_h_c[4];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotp_h_imm_4));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[4]);

    res = g_dotp_h_c[5];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotp_h_imm_5));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[5]);

    res = g_dotp_h_c[6];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotp_h_imm_6));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[6]);

    res = g_dotp_h_c[7];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotp_h_imm_7));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[7]);

    res = g_dotp_h_c[8];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotp_h_imm_8));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[8]);

   res = g_dotp_h_c[9];
    asm volatile (SDOTP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotp_h_imm_9));

    check_uint32(result, "sdotp_h", res, g_sdotp_h_sci_exp[9]);



}
void check_sdotup_h      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotup_h
  //-----------------------------------------------------------------
 for(i = 0; i < 1; i++) {

    res = g_dotp_h_c[i];

    asm volatile (SDOTUP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_exp[i]);

    res = g_dotp_h_c[i];
    asm volatile (SDOTUP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sc_exp[i]);

 }

    res = g_dotp_h_c[0];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotup_h_imm_0));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[0]);

    res = g_dotp_h_c[1];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotup_h_imm_1));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[1]);

    res = g_dotp_h_c[2];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotup_h_imm_2));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[2]);

    res = g_dotp_h_c[3];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotup_h_imm_3));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[3]);

    res = g_dotp_h_c[4];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotup_h_imm_4));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[4]);

    res = g_dotp_h_c[5];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotup_h_imm_5));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[5]);

    res = g_dotp_h_c[6];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotup_h_imm_6));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[6]);

    res = g_dotp_h_c[7];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotup_h_imm_7));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[7]);

    res = g_dotp_h_c[8];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotup_h_imm_8));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[8]);

   res = g_dotp_h_c[9];
    asm volatile (SDOTUP_H_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotup_h_imm_9));

    check_uint32(result, "sdotup_h", res, g_sdotup_h_sci_exp[9]);



}
void check_sdotusp_h      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotusp_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {
    res = g_dotp_h_c[i];
    asm volatile (SDOTUSP_H " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_exp[i]);

    res = g_dotp_h_c[i];
    asm volatile (SDOTUSP_H_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[i]), [b] "r" (g_dotp_h_b[i]));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sc_exp[i]);

 }

    res = g_dotp_h_c[0];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[0]), [imm] "i" (g_dotp_h_imm_0));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[0]);

    res = g_dotp_h_c[1];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[1]), [imm] "i" (g_dotp_h_imm_1));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[1]);

    res = g_dotp_h_c[2];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[2]), [imm] "i" (g_dotp_h_imm_2));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[2]);

    res = g_dotp_h_c[3];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[3]), [imm] "i" (g_dotp_h_imm_3));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[3]);

    res = g_dotp_h_c[4];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[4]), [imm] "i" (g_dotp_h_imm_4));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[4]);

    res = g_dotp_h_c[5];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[5]), [imm] "i" (g_dotp_h_imm_5));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[5]);

    res = g_dotp_h_c[6];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[6]), [imm] "i" (g_dotp_h_imm_6));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[6]);

    res = g_dotp_h_c[7];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[7]), [imm] "i" (g_dotp_h_imm_7));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[7]);

    res = g_dotp_h_c[8];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[8]), [imm] "i" (g_dotp_h_imm_8));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[8]);

   res = g_dotp_h_c[9];
    asm volatile (SDOTUSP_H_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_h_a[9]), [imm] "i" (g_dotp_h_imm_9));

    check_uint32(result, "sdotusp_h", res, g_sdotusp_h_sci_exp[9]);



}


/*###################################################################
##
##      8 bit case
##
##
 ####################################################################*/

//################################################################################
//#  test lv32.dotp{.sc, .sci}.b
//################################################################################

void check_dotp_b        (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotp_b
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotp_b", res, g_dotp_b_exp[i]);

    asm volatile (DOTP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotp_b", res, g_dotp_b_sc_exp[i]);

 }


    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotp_b_imm_0));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[0]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotp_b_imm_1));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[1]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotp_b_imm_2));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[2]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotp_b_imm_3));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[3]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotp_b_imm_4));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[4]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotp_b_imm_5));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[5]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotp_b_imm_6));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[6]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotp_b_imm_7));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[7]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotp_b_imm_8));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[8]);

    asm volatile (DOTP_B_SCI " %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotp_b_imm_9));

    check_uint32(result, "dotp_b", res, g_dotp_b_sci_exp[9]);

}

void check_dotup_b      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotup_h
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTUP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotup_b", res, g_dotup_b_exp[i]);

    asm volatile (DOTUP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotup_b", res, g_dotup_b_sc_exp[i]);

 }


    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotup_b_imm_0));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[0]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotup_b_imm_1));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[1]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotup_b_imm_2));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[2]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotup_b_imm_3));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[3]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotup_b_imm_4));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[4]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotup_b_imm_5));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[5]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotup_b_imm_6));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[6]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotup_b_imm_7));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[7]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotup_b_imm_8));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[8]);

    asm volatile (DOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotup_b_imm_9));

    check_uint32(result, "dotup_b", res, g_dotup_b_sci_exp[9]);



}
void check_dotusp_b      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.dotusp_b
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    asm volatile (DOTUSP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_exp[i]);

    asm volatile (DOTUSP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sc_exp[i]);

 }


    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotp_b_imm_0));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[0]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotp_b_imm_1));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[1]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotp_b_imm_2));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[2]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotp_b_imm_3));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[3]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotp_b_imm_4));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[4]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotp_b_imm_5));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[5]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotp_b_imm_6));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[6]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotp_b_imm_7));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[7]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotp_b_imm_8));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[8]);

    asm volatile (DOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotp_b_imm_9));

    check_uint32(result, "dotusp_b", res, g_dotusp_b_sci_exp[9]);

}


//################################################################################
//#  test lv32.dotp{.sc, .sci}
//################################################################################

void check_sdotp_b        (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotp_b
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {

    res = g_dotp_b_c[i];
    asm volatile (SDOTP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_exp[i]);

    res = g_dotp_b_c[i];
    asm volatile (SDOTP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sc_exp[i]);

 }

    res = g_dotp_b_c[0];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotp_b_imm_0));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[0]);

    res = g_dotp_b_c[1];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotp_b_imm_1));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[1]);

    res = g_dotp_b_c[2];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotp_b_imm_2));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[2]);

    res = g_dotp_b_c[3];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotp_b_imm_3));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[3]);

    res = g_dotp_b_c[4];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotp_b_imm_4));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[4]);

    res = g_dotp_b_c[5];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotp_b_imm_5));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[5]);

    res = g_dotp_b_c[6];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotp_b_imm_6));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[6]);

    res = g_dotp_b_c[7];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotp_b_imm_7));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[7]);

    res = g_dotp_b_c[8];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotp_b_imm_8));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[8]);

   res = g_dotp_b_c[9];
    asm volatile (SDOTP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotp_b_imm_9));

    check_uint32(result, "sdotp_b", res, g_sdotp_b_sci_exp[9]);



}
void check_sdotup_b      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotup_h
  //-----------------------------------------------------------------
 for(i = 0; i < 1; i++) {

    res = g_dotp_b_c[i];
    asm volatile (SDOTUP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_exp[i]);

    res = g_dotp_b_c[i];
    asm volatile (SDOTUP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sc_exp[i]);

 }

    res = g_dotp_b_c[0];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotup_b_imm_0));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[0]);

    res = g_dotp_b_c[1];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotup_b_imm_1));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[1]);

    res = g_dotp_b_c[2];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotup_b_imm_2));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[2]);

    res = g_dotp_b_c[3];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotup_b_imm_3));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[3]);

    res = g_dotp_b_c[4];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotup_b_imm_4));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[4]);

    res = g_dotp_b_c[5];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotup_b_imm_5));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[5]);

    res = g_dotp_b_c[6];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotup_b_imm_6));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[6]);

    res = g_dotp_b_c[7];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotup_b_imm_7));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[7]);

    res = g_dotp_b_c[8];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotup_b_imm_8));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[8]);

   res = g_dotp_b_c[9];
    asm volatile (SDOTUP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotup_b_imm_9));

    check_uint32(result, "sdotup_b", res, g_sdotup_b_sci_exp[9]);



}
void check_sdotusp_b      (testresult_t *result, void (*start)(), void (*stop) () ){
  unsigned int res,i;

  //-----------------------------------------------------------------
  // Check lv32.sdotusp_b
  //-----------------------------------------------------------------
 for(i = 0; i < NumberOfStimuli; i++) {
    res = g_dotp_b_c[i];
    asm volatile (SDOTUSP_B " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_exp[i]);

    res = g_dotp_b_c[i];
    asm volatile (SDOTUSP_B_SC " %[c], %[a], %[b]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[i]), [b] "r" (g_dotp_b_b[i]));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sc_exp[i]);

 }

    res = g_dotp_b_c[0];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[0]), [imm] "i" (g_dotp_b_imm_0));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[0]);

    res = g_dotp_b_c[1];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[1]), [imm] "i" (g_dotp_b_imm_1));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[1]);

    res = g_dotp_b_c[2];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[2]), [imm] "i" (g_dotp_b_imm_2));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[2]);

    res = g_dotp_b_c[3];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[3]), [imm] "i" (g_dotp_b_imm_3));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[3]);

    res = g_dotp_b_c[4];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[4]), [imm] "i" (g_dotp_b_imm_4));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[4]);

    res = g_dotp_b_c[5];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[5]), [imm] "i" (g_dotp_b_imm_5));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[5]);

    res = g_dotp_b_c[6];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[6]), [imm] "i" (g_dotp_b_imm_6));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[6]);

    res = g_dotp_b_c[7];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[7]), [imm] "i" (g_dotp_b_imm_7));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[7]);

    res = g_dotp_b_c[8];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[8]), [imm] "i" (g_dotp_b_imm_8));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[8]);

   res = g_dotp_b_c[9];
    asm volatile (SDOTUSP_B_SCI "  %[c], %[a], %[imm]\n"
                  : [c] "+r" (res)
                  : [a] "r"  (g_dotp_b_a[9]), [imm] "i" (g_dotp_b_imm_9));

    check_uint32(result, "sdotusp_b", res, g_sdotusp_b_sci_exp[9]);



}
