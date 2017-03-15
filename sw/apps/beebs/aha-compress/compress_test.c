/* -*- mode: C++; c-file-style: "gnu-mode" -*- */
/* BEEBS aha-compress benchmark

   Copyright (C) 2013 Embecosm Limited and University of Bristol

   Contributor James Pallister <james.pallister@bristol.ac.uk>

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
#include <stdio.h>
#include <stdlib.h>

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

/* Code from GLS.  Nine insns in the loop, giving 9*32 + 3 = 291 insns
worst case (mask = all 1's, not counting subroutine linkage). */

unsigned compress1(unsigned x, unsigned mask) {
  unsigned result = 0, bit = 1;
  while (mask != 0) {
    if ((mask & 1) != 0) {
      if (x & 1) result |= bit;
      bit <<= 1;
    }
    mask >>= 1;
    x >>= 1;
  }
  return result;
}

/* A version with no branches in the loop.  Eight insns in the loop,
giving 8*32 + 2 = 258 insns worst case (however, the code above might
be faster if the mask is sparse). */

// ------------------------------ cut ----------------------------------
unsigned compress2(unsigned x, unsigned m) {
   unsigned r, s, b;    // Result, shift, mask bit.

   r = 0;
   s = 0;
   do {
      b = m & 1;
      r = r | ((x & b) << s);
      s = s + b;
      x = x >> 1;
      m = m >> 1;
   } while (m != 0);
   return r;
}
// ---------------------------- end cut --------------------------------

/* Code from GLS.  Runs on a basic RISC in 159 ops total, incl.
subroutine overhead (just 3 ops).  Makes it clear that the five
masks can be precomputed if the mask is known (the five masks are
independent of x).  But this costs 5 stores and 5 loads because
"masks" is an array.  */

unsigned compress3(unsigned x, unsigned mask) {
  unsigned masks[5];
  unsigned long q, m, zm;
  int i;
  m = ~mask;
  zm = mask;
  for (i = 0; i < 5; i++) {
      q = m;
      m ^= m << 1;
      m ^= m << 2;
      m ^= m << 4;
      m ^= m << 8;
      m ^= m << 16;
      masks[i] = (m << 1) & zm;
      m = q & ~m;
      q = zm & masks[i]; zm = zm ^ q ^ (q >> (1 << i));
  }
  x = x & mask;
  q = x & masks[0];  x = x ^ q ^ (q >> 1);
  q = x & masks[1];  x = x ^ q ^ (q >> 2);
  q = x & masks[2];  x = x ^ q ^ (q >> 4);
  q = x & masks[3];  x = x ^ q ^ (q >> 8);
  q = x & masks[4];  x = x ^ q ^ (q >> 16);
  return x;
}

/* Modification of GLS's code in which last 5 lines are
merged into the loop, to avoid the stores and loads of
array "masks."  Num. insns. = 5*24 + 7 = 127 total
(compiled to Cyclops and adding 1 for the "andc" op).
   Michael Dalton has observed that the first shift left
can be omitted. */

// ------------------------------ cut ----------------------------------
unsigned compress4(unsigned x, unsigned m) {
   unsigned long mk, mp, mv, t;
   int i;

   x = x & m;           // Clear irrelevant bits.
   mk = ~m << 1;        // We will count 0's to right.

   for (i = 0; i < 5; i++) {
      mp = mk ^ (mk << 1);              // Parallel suffix.
      mp = mp ^ (mp << 2);
      mp = mp ^ (mp << 4);
      mp = mp ^ (mp << 8);
      mp = mp ^ (mp << 16);
      mv = mp & m;                      // Bits to move.
      m = (m ^ mv) | (mv >> (1 << i));    // Compress m.
      t = x & mv;
      x = (x ^ t) | (t >> (1 << i));      // Compress x.
      mk = mk & ~mp;
   }
   return x;
}



int
benchmark (void)
{
  static const unsigned long test[] = {
//       Data        Mask       Result
      0xFFFFFFFF, 0x80000000, 0x00000001,
      0xFFFFFFFF, 0x0010084A, 0x0000001F,
      0xFFFFFFFF, 0x55555555, 0x0000FFFF,
      0xFFFFFFFF, 0x88E00F55, 0x00001FFF,
      0x01234567, 0x0000FFFF, 0x00004567,
      0x01234567, 0xFFFF0000, 0x00000123,
      0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
      0,          0,          0,
      0,          0xFFFFFFFF, 0,
      0xFFFFFFFF, 0,          0,
      0x80000000, 0x80000000, 1,
      0x55555555, 0x55555555, 0x0000FFFF,
      0x55555555, 0xAAAAAAAA, 0,
      0x789ABCDE, 0x0F0F0F0F, 0x00008ACE,
      0x789ABCDE, 0xF0F0F0F0, 0x000079BD,
      0x92345678, 0x80000000, 0x00000001,
      0x12345678, 0xF0035555, 0x000004ec,
      0x80000000, 0xF0035555, 0x00002000,
   };
   int errors = 0,  n, r, i;

   n = sizeof(test)/sizeof(test[0]);

   for (i = 0; i < n; i += 3) {
      r = compress1(test[i], test[i+1]);
      if (r != test[i+2])
         errors = 1;
   }

   for (i = 0; i < n; i += 3) {
      r = compress2(test[i], test[i+1]);
      if (r != test[i+2])
         errors = 1;
   }

   for (i = 0; i < n; i += 3) {
      r = compress3(test[i], test[i+1]);
      if (r != test[i+2])
         errors = 1;
   }

   for (i = 0; i < n; i += 3) {
      r = compress4(test[i], test[i+1]);
      if (r != test[i+2])
         errors = 1;
   }

   return errors;
}


