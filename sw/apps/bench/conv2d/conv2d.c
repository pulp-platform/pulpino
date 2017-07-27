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
#include "common.h"

void conv2d(int16_t *restrict mat, int16_t * restrict res,
            const int16_t * restrict coeffs,
            int W, int H, int K, uint16_t scf) {
  int R = K >> 1;
  int index = R * W;

  for(int j = R; j < H - R; j++) {
    for(int i = R; i < W - R; i++) {
      int index2 = index - R * W;
      int c = 0;
      int val = 0;
      for(int y = -R; y <= R; ++y) {
        val += coeffs[c++] * mat[index2 + i - 2];
        val += coeffs[c++] * mat[index2 + i -1];
        val += coeffs[c++] * mat[index2 + i];
        val += coeffs[c++] * mat[index2 + i + 1];
        val += coeffs[c++] * mat[index2 + i + 2];
        index2 += W;
      }
      res[index+i] = val >> scf;
    }
    index += W;
  }
}
