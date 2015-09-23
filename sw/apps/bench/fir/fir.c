#include "../common.h"

void fir(const int16_t *arr, const int16_t *coeff, int16_t *output,
         unsigned arr_length, unsigned coeff_len) {

  for (unsigned i = 0; i < arr_length - coeff_len; ++i) {
    uint16_t sum = 0;
    for (unsigned j = 0; j < coeff_len; ++j) {
      sum += arr[i+j] * coeff[j];
    }
    
    output[i] = sum;
  }
}
