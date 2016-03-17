
// This testbench checks the basic functionality of:
//

#include <stdio.h>
#include "utils.h"
#include "bench.h"
#include "testClip_stimuli.h"

#ifdef RISCV
#define CLIP "p.clip"
#define CLIPU "p.clipu"
#else
#define CLIP "l.clip"
#define CLIPU "l.clipu"
#endif

void check_clip    (testresult_t *result, void (*start)(), void (*stop)()); 
void check_clipu   (testresult_t *result, void (*start)(), void (*stop)()); 


testcase_t testcases[] = {

{ .name = "clip"        , .test = check_clip       },
{ .name = "clipu"       , .test = check_clipu      },
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
//# T E S T    BSET 
//################################################################################

void check_clip(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.clip
  //-----------------------------------------------------------------
    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[0]), [I] "i" (CLIP_IMM_0));
    
    check_uint32(result, "clip", res,  res_clip[0]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[1]), [I] "i" (CLIP_IMM_1));
    
    check_uint32(result, "clip", res,  res_clip[1]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[2]), [I] "i" (CLIP_IMM_2));
    
    check_uint32(result, "clip", res,  res_clip[2]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[3]), [I] "i" (CLIP_IMM_3));
    
    check_uint32(result, "clip", res,  res_clip[3]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[4]), [I] "i" (CLIP_IMM_4));
    
    check_uint32(result, "clip", res,  res_clip[4]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[5]), [I] "i" (CLIP_IMM_5));
    
    check_uint32(result, "clip", res,  res_clip[5]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[6]), [I] "i" (CLIP_IMM_6));
    
    check_uint32(result, "clip", res,  res_clip[6]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[7]), [I] "i" (CLIP_IMM_7));
    
    check_uint32(result, "clip", res,  res_clip[7]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[8]), [I] "i" (CLIP_IMM_8));
    
    check_uint32(result, "clip", res,  res_clip[8]);

    asm volatile (CLIP " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clip[9]), [I] "i" (CLIP_IMM_9));
    
    check_uint32(result, "clip", res,  res_clip[9]);
}

void check_clipu(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int res;

  //-----------------------------------------------------------------
  // Check l.clip
  //-----------------------------------------------------------------
    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[0]), [I] "i" (CLIP_IMM_0));
    
    check_uint32(result, "clipu", res,  res_clipu[0]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[1]), [I] "i" (CLIP_IMM_1));
    
    check_uint32(result, "clipu", res,  res_clipu[1]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[2]), [I] "i" (CLIP_IMM_2));
    
    check_uint32(result, "clipu", res,  res_clipu[2]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[3]), [I] "i" (CLIP_IMM_3));
    
    check_uint32(result, "clipu", res,  res_clipu[3]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[4]), [I] "i" (CLIP_IMM_4));
    
    check_uint32(result, "clipu", res,  res_clipu[4]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[5]), [I] "i" (CLIP_IMM_5));
    
    check_uint32(result, "clipu", res,  res_clipu[5]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[6]), [I] "i" (CLIP_IMM_6));
    
    check_uint32(result, "clipu", res,  res_clipu[6]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[7]), [I] "i" (CLIP_IMM_7));
    
    check_uint32(result, "clipu", res,  res_clipu[7]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[8]), [I] "i" (CLIP_IMM_8));
    
    check_uint32(result, "clipu", res,  res_clipu[8]);

    asm volatile (CLIPU " %[c], %[a],%[I]\n"
                  : [c] "=r" (res)
                  : [a] "r"  (op_a_clipu[9]), [I] "i" (CLIP_IMM_9));
    
    check_uint32(result, "clipu", res,  res_clipu[9]);
}

