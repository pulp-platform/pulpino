/* BEEBS prime benchmark

   MDH WCET BENCHMARK SUITE.

   Copyright (C) 2014 Embecosm Limited and University of Bristol

   Contributor Pierre Langlois <pierre.langlois@embecosm.com>

   This file is part of the Bristol/Embecosm Embedded Benchmark Suite.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

typedef  unsigned char  bool;
typedef  unsigned long  ulong;

bool divides (ulong n, ulong m);
bool even (ulong n);
bool prime (ulong n);
void swap (ulong* a, ulong* b);

bool divides (ulong n, ulong m) {
  return (m % n == 0);
}

bool even (ulong n) {
  return (divides (2, n));
}

bool prime (ulong n) {
  ulong i;
  if (even (n))
      return (n == 2);
  for (i = 3; i * i <= n; i += 2) {
      if (divides (i, n)) /* ai: loop here min 0 max 357 end; */
          return 0;
  }
  return (n > 1);
}

void swap (ulong* a, ulong* b) {
  ulong tmp = *a;
  *a = *b;
  *b = tmp;
}

/* Write to this so call in BENCHMARK is not optimised away.  */
volatile int result = 0;

int
benchmark (void)
{
  ulong x =  21649L;
  ulong y = 513239L;
  swap (&x, &y);
  result = (!(prime(x) && prime(y)));
  return 0;
}


