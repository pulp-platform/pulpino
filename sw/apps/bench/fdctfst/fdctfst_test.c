#include "common.h"

static const int16_t in_block[64] = {
  99 ,104 ,109 ,113 ,115 ,115 , 55 , 55,
  104 ,111 ,113 ,116 ,119 , 56 , 56 , 56,
  110 ,115 ,120 ,119 ,118 , 56 , 56 , 56,
  119 ,121 ,122 ,120 ,120 , 59 , 59 , 59,
  119 ,120 ,121 ,122 ,122 , 55 , 55 , 55,
  121 ,121 ,121 ,121 , 60 , 57 , 57 , 57,
  122 ,122 , 61 , 63 , 62 , 57 , 57 , 57,
  62 , 62 , 61 , 61 , 63 , 58 , 58 , 58,
};

static const int16_t check_block[64] = {
  5593, 1826, -529, -146, 253, -92, -81, 24,
  797, 216, -874, -10, 127, -98, 44, 38,
  -606, -785, 180, 352, -200, -49, 80, -51,
  281, 291, 39, 37, -102, 53, 57, -57,
  -1, -312, -281, 102, 3, -40, 51, 14,
  49, 53, 171, 157, -58, -37, 9, -23,
  6, -41, -82, -76, -40, 53, 38, -11,
  1, 0, 0, 40, 9, -22, -6, 2
};

static int16_t data[64] __sram;

extern void ff_fdct_ifast(int16_t * data);

void test_setup() {}

void test_clear() {
  for (int i = 0; i != 64; ++i)
    data[i] = in_block[i];
}

void test_run() {
  ff_fdct_ifast(data);
}

int test_check() {
  for (int i = 0; i != 64; ++i)
    if (data[i] != check_block[i])
      return 0;

  return 1;
}


void porco(int16_t *data, int16_t *filter, int16_t *output, unsigned L, unsigned K) {
  K = K &~1;
  for (unsigned i = 0; i != L + K - 1; ++i) {
    int16_t val = 0;
    for (unsigned j = 0; j != K; ++j) {
      val += filter[j] * data[i + j];
    }
    output[i] = val;
  }
}
