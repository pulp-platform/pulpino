// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

#define __sram __attribute__((section(".heapsram")))

#ifdef __cplusplus
extern "C" {
#endif

uint32_t crc32(const void *src, size_t len);

#ifndef __USE_LIBC__
static inline
void *memcpy(void *__restrict__ dst, const void * __restrict__ src, size_t N) {
  char * __restrict__ dst_ = (char * __restrict__ )dst;
  const char * __restrict__ src_ = (char * __restrict__ )src;
  for (size_t i = 0; i != N; ++i)
    dst_[i] = src_[i];
  return dst;
}

static inline
int memcmp(const void *x, const void *y, size_t N) {
  if (x == y)
    return 0;

  const char *x_ = (const char *)x;
  const char *y_ = (const char *)y;
  for (size_t i = 0; i != N; ++i) {
    int delta = x_[i] - y_[i];
    if (delta)
      return delta;
  }
  return 0;
}

static inline
void *memset(void *dst, int V, size_t N) {
  char *dst_ = (char *) dst;
  for (size_t i = 0; i != N; ++i)
    dst_[i] = V;
  return dst;
}

static inline
size_t strlen(const char *str) {
  const char *base = str;
  while (*str++ != 0);
  return str - base - 1;
}
#else
#include "string.h"
#endif

#ifdef __cplusplus
}
#endif

#endif
