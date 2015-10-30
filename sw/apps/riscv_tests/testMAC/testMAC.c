// This testbench checks the basic functionality of the following instructions:
//
// mul
//
// p.mac
//
// p.mac.zl.zl
// p.mac.zl.zh
// p.mac.zh.zl
// p.mac.zh.zh
//
// p.mac.zl.sl
// p.mac.zl.sh
// p.mac.zh.sl
// p.mac.zh.sh
//
// p.mac.sl.zl
// p.mac.sl.zh
// p.mac.sh.zl
// p.mac.sh.zh
//
// p.mac.sl.sl
// p.mac.sl.sh
// p.mac.sh.sl
// p.mac.sh.sh

#include <stdio.h>
#include "utils.h"
#include "bench.h"

#include "testMAC_stimuli.h"


void check_mac(testresult_t *result, void (*start)(), void (*stop)());
void check_mac_subword(testresult_t *result, void (*start)(), void (*stop)());
void check_mul(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "mac",         .test = check_mac         },
  { .name = "mac_subword", .test = check_mac_subword },
  { .name = "mul",         .test = check_mul         },
  {0, 0},
};


int main()
{
  if(get_core_id() == 0) {
    run_suite(testcases);
  }

  synch_barrier();

  return 0;
}


#define util_check_mac(asm_str, i, act, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[d], %[a], %[b], %[c]\n" \
                    : [d] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r"  (prefix ## _b[i]), \
                      [c] "r"  (prefix ## _c[i])); \
      check_uint32(result, asm_str, act, prefix ## _exp[i]); \
    }


void check_mul(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int tmp = 0;

  //-----------------------------------------------------------------
  // Check mul
  //-----------------------------------------------------------------
  for(i = 0; i < (sizeof(g_mul_a)/4); i++) {
    asm volatile ("mul %[d], %[a], %[b]\n"
                  : [d] "+r" (tmp)
                  : [a] "r"  (g_mul_a[i]),
                    [b] "r"  (g_mul_b[i]));
    check_uint32(result, "mul", tmp, g_mul_exp[i]);
  }
}


void check_mac(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int tmp = 0;

  //-----------------------------------------------------------------
  // Check p.mac
  //-----------------------------------------------------------------
  util_check_mac("p.mac", i, tmp, g_mac);
}

void check_mac_subword(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int tmp = 0;

  //-----------------------------------------------------------------
  // Check p.mac with subword selection
  //-----------------------------------------------------------------
  util_check_mac("p.mac.zl.zl", i, tmp, g_mac_zl_zl);
  util_check_mac("p.mac.zl.zh", i, tmp, g_mac_zl_zh);
  util_check_mac("p.mac.zh.zl", i, tmp, g_mac_zh_zl);
  util_check_mac("p.mac.zh.zh", i, tmp, g_mac_zh_zh);

  util_check_mac("p.mac.zl.sl", i, tmp, g_mac_zl_sl);
  util_check_mac("p.mac.zl.sh", i, tmp, g_mac_zl_sh);
  util_check_mac("p.mac.zh.sl", i, tmp, g_mac_zh_sl);
  util_check_mac("p.mac.zh.sh", i, tmp, g_mac_zh_sh);

  util_check_mac("p.mac.sl.zl", i, tmp, g_mac_sl_zl);
  util_check_mac("p.mac.sl.zh", i, tmp, g_mac_sl_zh);
  util_check_mac("p.mac.sh.zl", i, tmp, g_mac_sh_zl);
  util_check_mac("p.mac.sh.zh", i, tmp, g_mac_sh_zh);

  util_check_mac("p.mac.sl.sl", i, tmp, g_mac_sl_sl);
  util_check_mac("p.mac.sl.sh", i, tmp, g_mac_sl_sh);
  util_check_mac("p.mac.sh.sl", i, tmp, g_mac_sh_sl);
  util_check_mac("p.mac.sh.sh", i, tmp, g_mac_sh_sh);
}
