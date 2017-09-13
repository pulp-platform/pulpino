// This testbench checks the basic functionality of:
//
// fadd.s
// fsub.s
// fmul.s
// fdiv.s
// fsqrt.s
// fmadd.s
// fnmadd.s
// fmsub.s
// fnmsub.s
// fLog2
// fExp2
// fSin
// fCos
// fAtan2


/////////////////////////////////////////////////////////
// includes
/////////////////////////////////////////////////////////
#include "testFPU.h"
#include "math_fns.h"
#include "bar.h"
#include "utils.h"
#include "bench.h"

/////////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////////
void check_basic   (testresult_t *result, void (*start)(), void (*stop)());
void check_trig    (testresult_t *result, void (*start)(), void (*stop)());
void check_fma     (testresult_t *result, void (*start)(), void (*stop)());
void check_explog  (testresult_t *result, void (*start)(), void (*stop)());
void check_special (testresult_t *result, void (*start)(), void (*stop)());
void check_basic_special   (testresult_t *result, void (*start)(), void (*stop)());
void check_fma_special (testresult_t *result, void (*start)(), void (*stop)());


/////////////////////////////////////////////////////////
// Test case selection
/////////////////////////////////////////////////////////
testcase_t testcases[] = {
  {  .name = "basic",          .test = check_basic          },
  {  .name = "trig",           .test = check_trig           },
  {  .name = "fma",            .test = check_fma            },
  {  .name = "explog",         .test = check_explog         },
  {  .name = "special",        .test = check_special        },
  {  .name = "basic_special",  .test = check_basic_special  },
  {  .name = "fma_special",    .test = check_fma_special    },
  {0, 0}
};

/////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////

void check_float(testresult_t* result, const char* fail_msg, float actual, float min, float max)
{
  int pass = 0;
  pass = (actual <= max);
  pass = pass && (actual >= min);

  if(!pass) {
    result->errors += 1;
    printf("Error!:\n");
    printf("%s: \t", fail_msg);
    printFloat(min);
    printf("\t !< ");
    printFloat(actual);
    printf("\t !< ");
    printFloat(max);
    printf("\n");
    result->errors += 1;
  }
}

void check_float_value(testresult_t* result, const char* fail_msg, float actual, float exp)
{
  if((!(fIsNan(actual)&&(fIsNan(exp)))) && (actual != exp)) {
    printf("Error!:\n");
    printf("%s: \t", fail_msg);
    printFloat(actual);
    printf("\t != ");
    printFloat(exp);
    printf("\n");
    result->errors += 1;
  }
}

/////////////////////////////////////////////////////////
// Main
/////////////////////////////////////////////////////////
int main()
{
  int errors = 1;
  int pass = 0;

  int coreid = get_core_id();

  //if(coreid == 0) {
    testresult_t result;
    
    return run_suite(testcases);
    
}

/////////////////////////////////////////////////////////
// Function definitions
/////////////////////////////////////////////////////////
void check_basic(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  float g_add_act[sizeof(g_in_b)/4];
  float g_sub_act[sizeof(g_in_b)/4];
  float g_mul_act[sizeof(g_in_b)/4];
  float g_div_act[sizeof(g_in_b)/4];
  float g_div_act_ds[sizeof(g_inbr_b)/4];
  float g_sqrt_act_ss[sizeof(g_inbr_a)/4];
  int   g_branch_act[sizeof(g_inbr_b)/4];

  start();

  //-----------------------------------------------------------------
  // Check fadd.s (floating point addition)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    asm volatile ("fadd.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_add_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]));

    check_float(result, "fadd.s", g_add_act[i],  g_add_min[i], g_add_max[i]);
  }

  //-----------------------------------------------------------------
  // Check fsub.s (floating point subtraction)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sub_act)/4); i++) {
    asm volatile ("fsub.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_sub_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]));

    check_float(result, "fsub.s", g_sub_act[i],  g_sub_min[i], g_sub_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fmul.s (floating point multiplication)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_mul_act)/4); i++) {
    asm volatile ("fmul.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_mul_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]));

    check_float(result, "fmul.s", g_mul_act[i],  g_mul_min[i], g_mul_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fdiv.s (floating point division)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_div_act)/4); i++) {
    asm volatile ("fdiv.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_div_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]));

    check_float(result, "fdiv.s", g_div_act[i],  g_div_min[i], g_div_max[i]);
  }


  //-----------------------------------------------------------------
  // Check fsqrt.s (floating point square root)
  //-----------------------------------------------------------------

  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    asm volatile ("fsqrt.s %[c], %[a]\n"
                  : [c] "=f" (g_add_act[i])
                  : [a] "f"  (g_in_a[i]));

    check_float(result, "fsqrt.s", g_add_act[i],  g_sqrt_min[i], g_sqrt_max[i]);
  }

  //-----------------------------------------------------------------
  // Check f{eq,lt,le}.s (floating point compare instructions)
  //-----------------------------------------------------------------

  for(i = 0; i < (sizeof(g_branch_act)/4); i++) {
    asm volatile ("feq.s %[c], %[a], %[b]\n"
                  : [c] "=r" (g_branch_act[i])
                  : [a] "f"  ( *(float*)&g_inbr_a[i] ), [b] "f" ( *(float*)&g_inbr_b[i] ));

    check_uint32(result, "feq.s", g_branch_act[i], g_feq[i]);

  }

  for(i = 0; i < (sizeof(g_branch_act)/4); i++) {
    asm volatile ("fle.s %[c], %[a], %[b]\n"
                  : [c] "=r" (g_branch_act[i])
                  : [a] "f"  ( *(float*)&g_inbr_a[i] ), [b] "f" ( *(float*)&g_inbr_b[i] ));

    check_uint32(result, "fle.s", g_branch_act[i], g_fle[i]);

  }

  for(i = 0; i < (sizeof(g_branch_act)/4); i++) {
    asm volatile ("flt.s %[c], %[a], %[b]\n"
                  : [c] "=r" (g_branch_act[i])
                  : [a] "f"  ( *(float*)&g_inbr_a[i] ), [b] "f" ( *(float*)&g_inbr_b[i] ));

    check_uint32(result, "flt.s", g_branch_act[i], g_flt[i]);

  }

  stop();
}


void check_basic_special(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
    float g_add_act_s[sizeof(g_in_b_ass)/4];
    float g_sub_act_s[sizeof(g_in_b_ass)/4];
    float g_mul_act_s[sizeof(g_in_b_ass)/4];
    float g_div_act_s[sizeof(g_in_b_ass)/4];
    float g_sqrt_act_ss[sizeof(g_sqrt_a_ss)/4];
  int   g_branch_act[sizeof(g_inbr_b)/4];

  start();

  //-----------------------------------------------------------------
  // Check fadd.s (floating point addition) special cases
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_add_act_s)/4); i++) {
    asm volatile ("fadd.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_add_act_s[i])
                  : [a] "f"  ( *(float*)&g_in_a_ass[i]), [b] "f" ( *(float*)&g_in_b_ass[i]));

    check_float_value(result, "fadd.s", g_add_act_s[i],  ( *(float*)&g_output_as[i]) );
  }

  //-----------------------------------------------------------------
  // Check fsub.s (floating point subtraction)  special cases
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sub_act_s)/4); i++) {
    asm volatile ("fsub.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_sub_act_s[i])
                  : [a] "f"  ( *(float*)&g_in_a_ass[i]), [b] "f" ( *(float*)&g_in_b_ass[i]));

    check_float_value(result, "fsub.s", g_sub_act_s[i], ( *(float*)&g_output_ss[i]) );
  }


  //-----------------------------------------------------------------
  // Check fmul.s (floating point multiplication)  special cases
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_mul_act_s)/4); i++) {
    asm volatile ("fmul.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_mul_act_s[i])
                  : [a] "f"  ( *(float*)&g_in_a_ass[i]), [b] "f" ( *(float*)&g_in_b_ass[i]));

    check_float_value(result, "fmul.s", g_mul_act_s[i], ( *(float*)&g_output_ms[i]) );
  }

 //-----------------------------------------------------------------
  // Check fdiv.s (floating point division) full special cases
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_div_act_s)/4); i++) {
    asm volatile ("fdiv.s %[c], %[a], %[b]\n"
                  : [c] "=f" (g_div_act_s[i])
                  : [a] "f"  ( *(float*)&g_in_a_ass[i]), [b] "f" ( *(float*)&g_in_b_ass[i]));

    check_float_value(result, "fdiv.s", g_div_act_s[i],  ( *(float*)&g_output_ds[i]));
  }

  //-----------------------------------------------------------------
  // Check fsqrt.s (floating point square root) special cases
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_sqrt_act_ss)/4); i++) {
    asm volatile ("fsqrt.s %[c], %[a]\n"
                  : [c] "=f" (g_sqrt_act_ss[i])
                  : [a] "f"  ( *(float*)&g_sqrt_a_ss[i] ));

    check_float_value(result, "fsqrt.s", g_sqrt_act_ss[i], ( *(float*)&g_sqrt_output_ss[i]) );
  }
  stop();
}



void check_explog(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  float g_add_act[sizeof(g_in_a)/4];
  //-----------------------------------------------------------------
  // Check fExp2 (floating point exponentiation 2^x)
  //-----------------------------------------------------------------
  start();
  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    float g_exp_in = g_in_a[i]/100.0F;
    g_add_act[i] = fExp2(g_exp_in);

    check_float(result, "fExp2", g_add_act[i],  g_exp_min[i], g_exp_max[i]);
  }

  //-----------------------------------------------------------------
  // Check fExp2 (floating point exponentiation 2^(-x))
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    float g_exp_in = g_in_a[i]/1000.0F;
    g_add_act[i] = fExp2(-g_exp_in);

    check_float(result, "fExp2", g_add_act[i],  g_mexp_min[i], g_mexp_max[i]);
  }

  //-----------------------------------------------------------------
  // Check fLog2 (floating point logarithm (base 2))
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_add_act)/4); i++) {
    g_add_act[i] = fLog2(g_in_a[i]);
    check_float(result, "gLog2", g_add_act[i],  g_log_min[i], g_log_max[i]);
  }
  stop();
}

void check_trig(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  float g_trig_act[sizeof(g_trig_in)/4];
  start();
  float pi2 = MATH_0_5PI;
  //-----------------------------------------------------------------
  // Check fSin (floating point sin)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_trig_act)/4); i++) {
    g_trig_act[i] = fSin(g_trig_in[i]);
    check_float(result, "fSin", g_trig_act[i],  g_sin_min[i], g_sin_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fCos (floating point cos)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_trig_act)/4); i++) {
     g_trig_act[i] = fCos(g_trig_in[i]);
     check_float(result, "fCos", g_trig_act[i],  g_cos_min[i], g_cos_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fAtan2 (floating point atan) // atan2(y,x); y=a, x=b
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_trig_act)/4); i++) {
    float x,y;
    x = g_in_b[i];
    y = g_in_a[i];
    g_trig_act[i] = fAtan2(y,x);
    check_float(result, "fAtan2", g_trig_act[i],  g_atan_min[i], g_atan_max[i]);
  }
  stop();
}

void check_fma(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  start();
  float g_fma_act[(sizeof(g_fma_init)/4)];

  //-----------------------------------------------------------------
  // Check fmadd.s (floating point multiply-add)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_fma_init)/4); i++) {
    g_fma_act[i] = g_fma_init[i];
    asm volatile ("fmadd.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]), [d] "f" (g_fma_act[i]));

    check_float(result, "fmadd.s", g_fma_act[i],  g_fma_min[i], g_fma_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fnmadd.s (floating point negative multiply-add)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_fma_init)/4); i++) {
    g_fma_act[i] = g_fma_init[i];
    asm volatile ("fnmadd.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (g_in_b[i]), [d] "f" (g_fma_act[i]));

    check_float(result, "fnmadd.s", g_fma_act[i],  -g_fma_max[i], -g_fma_min[i]);
  }

  //-----------------------------------------------------------------
  // Check fmsub.s (floating point multiply-subtract)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_fma_init)/4); i++) {
    g_fma_act[i] = g_fma_init[i];
    asm volatile ("fmsub.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (-g_in_b[i]), [d] "f" (-g_fma_act[i]));

    check_float(result, "fmsub.s", g_fma_act[i],  g_fms_min[i], g_fms_max[i]);
  }
  //-----------------------------------------------------------------
  // Check fnmsub.s (floating point negative multiply-subtract)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_fma_init)/4); i++) {
    g_fma_act[i] = g_fma_init[i];
    asm volatile("fnmsub.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act[i])
                  : [a] "f"  (g_in_a[i]), [b] "f" (-g_in_b[i]), [d] "f" (-g_fma_act[i]));

    check_float(result, "fnmsub.s", g_fma_act[i], -g_fms_max[i], -g_fms_min[i]);
  }

  
  stop();
}


void check_fma_special(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  start();
  float g_fma_act_s[(sizeof(g_fma_init_s)/4)];

  //-----------------------------------------------------------------
  // Check fmadd.s (floating point multiply-add)
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_fma_init_s)/4); i++) {
    asm volatile ("fmadd.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act_s[i])
                  : [a] "f"  ( *(float*)&g_fma_a_s[i]), [b] "f" ( *(float*)&g_fma_b_s[i]), [d] "f" ( *(float*)&g_fma_init_s[i]) );

    check_float_value(result, "fmadd.s", g_fma_act_s[i],  ( *(float*)&g_fma_output_sma[i]) );
  }

  for(i = 0; i < (sizeof(g_fma_init_s)/4); i++) {
    asm volatile ("fnmadd.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act_s[i])
                  : [a] "f"  ( *(float*)&g_fma_a_s[i]), [b] "f" ( *(float*)&g_fma_b_s[i]), [d] "f" ( *(float*)&g_fma_init_s[i]) );

    check_float_value(result, "fnmadd.s", g_fma_act_s[i],  ( *(float*)&g_fma_output_snma[i]) );
  }


  for(i = 0; i < (sizeof(g_fma_init_s)/4); i++) {
    asm volatile ("fmsub.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act_s[i])
                  : [a] "f"  ( *(float*)&g_fma_a_s[i]), [b] "f" ( *(float*)&g_fma_b_s[i]), [d] "f" ( *(float*)&g_fma_init_s[i]) );

    check_float_value(result, "fmsub.s", g_fma_act_s[i],  ( *(float*)&g_fma_output_sms[i]) );
  }

  for(i = 0; i < (sizeof(g_fma_init_s)/4); i++) {
    asm volatile ("fnmsub.s %[c], %[a], %[b], %[d]\n"
                  : [c] "=f" (g_fma_act_s[i])
                  : [a] "f"  ( *(float*)&g_fma_a_s[i]), [b] "f" ( *(float*)&g_fma_b_s[i]), [d] "f" ( *(float*)&g_fma_init_s[i]) );

    check_float_value(result, "fnmsub.s", g_fma_act_s[i],  ( *(float*)&g_fma_output_snms[i]) );
  }
  stop();
}

void check_special(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  float in_a = 3.1F;
  float in_zero = 0.0F;
  float in_inf = 0.0F;
  float res;
  int flag_zero = 0;
  int flag_init;
  int flag_test;
  int flags;
  int flag_swap;
  
  start();
  //-----------------------------------------------------------------
  // Check read/write flags
  //-----------------------------------------------------------------
  asm volatile ("fsflags %[c]" :: [c] "r"  (flag_zero));
  flag_test = 31;
  flag_init = flag_test+32+64; // high bits should be ignored!
  asm volatile ("fsflags %[c]" :: [c] "r"  (flag_init));
  asm volatile ("frflags %[c]" :  [c] "=r" (flags));
  if (flags != flag_test)
    result->errors +=1;
  asm volatile ("fsflags %[c]" :: [c] "r"  (flag_zero));

  //-----------------------------------------------------------------
  // Check read/write frm
  //-----------------------------------------------------------------
  asm volatile ("fsrm %[c]" :: [c] "r"  (flag_zero));
  flag_test = 7;
  flag_init = flag_test+8+16; // high bits should be ignored!
  asm volatile ("fsrm %[c]" :: [c] "r"  (flag_init));
  asm volatile ("frrm %[c]" :  [c] "=r" (flags));
  if (flags != flag_test)
    result->errors +=1;
  asm volatile ("fsrm %[c]" :: [c] "r"  (flag_zero));

  //-----------------------------------------------------------------
  // Check read/write fcsr
  //-----------------------------------------------------------------
  asm volatile ("fscsr %[c]" :: [c] "r"  (flag_zero));
  flag_test = 255;
  flag_init = flag_test+256+512; // high bits should be ignored!
  asm volatile ("fscsr %[c]" :: [c] "r"  (flag_init));
  asm volatile ("frcsr %[c]" :  [c] "=r" (flags));
  if (flags != flag_test)
    result->errors +=1;
  asm volatile ("fscsr %[c]" :: [c] "r"  (flag_zero));

  //-----------------------------------------------------------------
  // Check read/write fprec
  //-----------------------------------------------------------------
  asm volatile ("csrrw %[d], 0x6, %[c] " : [d] "=r"(flag_zero) : [c] "r"  (flag_zero));
  flag_test = 31;
  flag_init = flag_test+32+64; // high bits should be ignored!
  asm volatile ("csrrw %[d], 0x6, %[c] " : [d] "=r"(flag_swap) : [c] "r"  (flag_init));
  asm volatile ("csrrs %[d], 0x6, x0 " : [d] "=r"(flags));
  if (flags != flag_test)
    result->errors +=1;
  if (flag_swap != 0)
    result->errors +=1;

  // swap zero flag in and compare swap
  asm volatile ("csrrw %[d], 0x6, %[c] " : [d] "=r"(flag_swap) : [c] "r"  (flag_zero));
  if (flag_swap != flag_test)
    result->errors +=1;

  /* //----------------------------------------------------------------- */
  /* // Check fdiv.s divide zero by zero -> NaN */
  /* //----------------------------------------------------------------- */
  /* asm volatile ("fsflags %[c]" :: [c] "r" (flag_zero)); */
  /* asm volatile ("fdiv.s %[c], %[a], %[b]\n" */
  /* 		: [c] "=f" (res) */
  /* 		: [a] "f"  (in_zero), [b] "f" (in_zero)); */
  /* asm volatile ("frflags %[c]" : [c] "=r" (flags)); */

  /* // flag should be 24 (divide by 0 + invalid operation) */
  /* if (flags != 24) { */
  /*   result->errors +=1; */
  /*   printf("flags: is: %d; should: 24\n",flags); */
  /* } */
  /* asm volatile ("fsflags %[c]" :: [c] "r" (flag_zero)); */

  /* // res should be NaN -> comparison should fail! */
  /* if (res==res) { */
  /*   result->errors +=1; */
    /* printf("result is: ");printFloat(res); */
    /* printf("; should be NaN\n"); */
  /*   printFloat(res); */
  /* } */

  //-----------------------------------------------------------------
  // check fdiv.s divide a by 0 -> Inf
  //-----------------------------------------------------------------
  asm volatile ("fsflags %[c]" :: [c] "r" (flag_zero));
  asm volatile ("fdiv.s %[c], %[a], %[b]\n"
  		: [c] "=f" (res)
  		: [a] "f"  (in_a), [b] "f" (in_zero));
  asm volatile ("frflags %[c]" : [c] "=r" (flags));

  // flag should be 8 (divide by 0)
  if (flags != 8) {
    result->errors +=1;
    printf("flags: is: %d; should: 8\n",flags);
  }

  // res should be Inf
  if (!fIsInf(res)) { 
    result->errors +=1;
    printf("result is: ");
    printFloat(res);
    printf("; should be Inf\n");
  }

  //-----------------------------------------------------------------
  // check fadd.s add large numbers to cause overflow
  //-----------------------------------------------------------------
  float in_max = 3.4028235E38F;
  float in_big = 1.0E38F;
  asm volatile ("fadd.s %[c], %[a], %[b]\n"
  		: [c] "=f" (res)
  		: [a] "f"  (in_big), [b] "f" (in_max));
  asm volatile ("frflags %[c]" : [c] "=r" (flags));

  // flag should be 13 (divide by 0, + overflow)
  if (flags != 13) {
    result->errors +=1;
    printf("flags: is: %d; should: 13\n",flags);
  }

  // res should be Inf
  if (!fIsInf(res)) {
    result->errors +=1;
    printf("result is: ");
    printFloat(res);
    printf("; should be Inf\n");
  }

  //-----------------------------------------------------------------
  // check fsub.s subtract small numbers to cause underflow
  //-----------------------------------------------------------------
  /* float in_min = 1.4E-45F; */
  /* float in_tiny = 10.0F; */
  /* asm volatile ("fdiv.s %[c], %[a], %[b]\n" */
  /* 		: [c] "=f" (res) */
  /* 		: [a] "f"  (in_min), [b] "f" (in_tiny)); */
  /* asm volatile ("frflags %[c]" : [c] "=r" (flags)); */

  /* // flag should be 15 (divide by 0, + overflow, + underflow) */
  /* if (flags != 15) { */
  /*   result->errors +=1; */
  /*   printf("flags: is: %d; should: 15\n",flags); */
  /* } */

  /* // underflow: res should be 0 */
  /* if (res!=0.0F) { */
  /*   result->errors +=1; */
  /*   printf("result is: "); */
  /*   printFloat(res); */
  /*   printf("; should be 0.0F\n"); */
  /* } */
  /* // reset flags */
  /* asm volatile ("fsflags %[c]" :: [c] "r" (flag_zero)); */
  
  //-----------------------------------------------------------------
  // TP division
  //-----------------------------------------------------------------
  /* float in_min = 1.4E-45F; */
  /* float in_tiny = 10.0F; */
  /* asm volatile ("fdiv.s %[c], %[a], %[b]\n" */
  /* 		: [c] "=f" (res) */
  /* 		: [a] "f"  (in_min), [b] "f" (in_tiny)); */
  /* asm volatile ("frflags %[c]" : [c] "=r" (flags)); */

  /* // flag should be 15 (divide by 0, + overflow, + underflow) */
  /* if (flags != 15) { */
  /*   result->errors +=1; */
  /*   printf("flags: is: %d; should: 15\n",flags); */
  /* } */

  /* // underflow: res should be 0 */
  /* if (res!=0.0F) { */
  /*   result->errors +=1; */
  /*   printf("result is: "); */
  /*   printFloat(res); */
  /*   printf("; should be 0.0F\n"); */
  /* } */
  /* // reset flags */
  /* asm volatile ("fsflags %[c]" :: [c] "r" (flag_zero)); */
  

  stop();
}
