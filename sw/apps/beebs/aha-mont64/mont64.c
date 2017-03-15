/* -*- mode: C++; c-file-style: "gnu-mode" -*- */
/* BEEBS aha-mont64 benchmark

   Some of this code is referenced by the book the Hacker's Delight
   (which placed it in the public domain).
   See http://www.hackersdelight.org/

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

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

/* Computes a*b mod m using Montgomery multiplication (MM). a, b, and m
are unsigned numbers with a, b < m < 2**64, and m odd. The code does
some 128-bit arithmetic.
   The variable r is fixed at 2**64, and its log base 2 at 64.
   Works with gcc on Windows and Linux. */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint64_t uint64;
typedef int64_t int64;

/* ---------------------------- mulul64 ----------------------------- */

/* Multiply unsigned long 64-bit routine, i.e., 64 * 64 ==> 128.
Parameters u and v are multiplied and the 128-bit product is placed in
(*whi, *wlo). It is Knuth's Algorithm M from [Knu2] section 4.3.1.
Derived from muldwu.c in the Hacker's Delight collection. */

void mulul64(uint64 u, uint64 v, uint64 *whi, uint64 *wlo)
   {
   uint64 u0, u1, v0, v1, k, t;
   uint64 w0, w1, w2;

   u1 = u >> 32; u0 = u & 0xFFFFFFFF;
   v1 = v >> 32; v0 = v & 0xFFFFFFFF;

   t = u0*v0;
   w0 = t & 0xFFFFFFFF;
   k = t >> 32;

   t = u1*v0 + k;
   w1 = t & 0xFFFFFFFF;
   w2 = t >> 32;

   t = u0*v1 + w1;
   k = t >> 32;

   *wlo = (t << 32) + w0;
   *whi = u1*v1 + w2 + k;

   return;
}

/* ---------------------------- modul64 ----------------------------- */

uint64 modul64(uint64 x, uint64 y, uint64 z) {

   /* Divides (x || y) by z, for 64-bit integers x, y,
   and z, giving the remainder (modulus) as the result.
   Must have x < z (to get a 64-bit result). This is
   checked for. */

   int64 i, t;

   for (i = 1; i <= 64; i++) {  // Do 64 times.
      t = (int64)x >> 63;       // All 1's if x(63) = 1.
      x = (x << 1) | (y >> 63); // Shift x || y left
      y = y << 1;               // one bit.
      if ((x | t) >= z) {
         x = x - z;
         y = y + 1;
      }
   }
   return x;                    // Quotient is y.
}

/* ---------------------------- montmul ----------------------------- */

uint64 montmul(uint64 abar, uint64 bbar, uint64 m,
               uint64 mprime) {

   uint64 thi, tlo, tm, tmmhi, tmmlo, uhi, ulo, ov;

   /* t = abar*bbar. */

   mulul64(abar, bbar, &thi, &tlo);  // t = abar*bbar.

   /* Now compute u = (t + ((t*mprime) & mask)*m) >> 64.
   The mask is fixed at 2**64-1. Because it is a 64-bit
   quantity, it suffices to compute the low-order 64
   bits of t*mprime, which means we can ignore thi. */

   tm = tlo*mprime;

   mulul64(tm, m, &tmmhi, &tmmlo);   // tmm = tm*m.

   ulo = tlo + tmmlo;                // Add t to tmm
   uhi = thi + tmmhi;                // (128-bit add).
   if (ulo < tlo) uhi = uhi + 1;     // Allow for a carry.

   // The above addition can overflow. Detect that here.

   ov = (uhi < thi) | ((uhi == thi) & (ulo < tlo));

   ulo = uhi;                   // Shift u right
   uhi = 0;                     // 64 bit positions.

// if (ov > 0 || ulo >= m)      // If u >= m,
//    ulo = ulo - m;            // subtract m from u.
   ulo = ulo - (m & -(ov | (ulo >= m))); // Alternative
                                // with no branching.

   return ulo;
}

/* ---------------------------- xbinGCD ----------------------------- */

/* C program implementing the extended binary GCD algorithm. C.f.
http://www.ucl.ac.uk/~ucahcjm/combopt/ext_gcd_python_programs.pdf. This
is a modification of that routine in that we find s and t s.t.
    gcd(a, b) = s*a - t*b,
rather than the same expression except with a + sign.
   This routine has been greatly simplified to take advantage of the
facts that in the MM use, argument a is a power of 2, and b is odd. Thus
there are no common powers of 2 to eliminate in the beginning. The
parent routine has two loops. The first drives down argument a until it
is 1, modifying u and v in the process. The second loop modifies s and
t, but because a = 1 on entry to the second loop, it can be easily seen
that the second loop doesn't alter u or v. Hence the result we want is u
and v from the end of the first loop, and we can delete the second loop.
   The intermediate and final results are always > 0, so there is no
trouble with negative quantities. Must have a either 0 or a power of 2
<= 2**63. A value of 0 for a is treated as 2**64. b can be any 64-bit
value.
   Parameter a is half what it "should" be. In other words, this function
does not find u and v st. u*a - v*b = 1, but rather u*(2a) - v*b = 1. */

void xbinGCD(uint64 a, uint64 b, volatile uint64 *pu, volatile uint64 *pv)
   {
   uint64 alpha, beta, u, v;

   u = 1; v = 0;
   alpha = a; beta = b;         // Note that alpha is
                                // even and beta is odd.

   /* The invariant maintained from here on is:
   a = u*2*alpha - v*beta. */

// printf("Before, a u v = %016llx %016llx %016llx\n", a, u, v);
   while (a > 0) {
      a = a >> 1;
      if ((u & 1) == 0) {             // Delete a common
         u = u >> 1; v = v >> 1;      // factor of 2 in
      }                               // u and v.
      else {
         /* We want to set u = (u + beta) >> 1, but
         that can overflow, so we use Dietz's method. */
         u = ((u ^ beta) >> 1) + (u & beta);
         v = (v >> 1) + alpha;
      }
//    printf("After,  a u v = %016llx %016llx %016llx\n", a, u, v);
   }

// printf("At end,    a u v = %016llx %016llx %016llx\n", a, u, v);
   *pu = u;
   *pv = v;
   return;
}

/* ------------------------------ main ------------------------------ */

int benchmark() {
   uint64 a, b, m, hr, p1hi, p1lo, p1, p, abar, bbar;
   uint64 phi, plo;
   volatile uint64 rinv, mprime;
   int errors = 0;

   m = 0xfae849273928f89fLL;             // Must be odd.
   b = 0x14736defb9330573LL;             // Must be smaller than m.
   a = 0x0549372187237fefLL;             // Must be smaller than m.

   /* The simple calculation: This computes (a*b)**4 (mod m) correctly for all a,
   b, m < 2**64. */

   mulul64(a, b, &p1hi, &p1lo);         // Compute a*b (mod m).
   p1 = modul64(p1hi, p1lo, m);
   mulul64(p1, p1, &p1hi, &p1lo);       // Compute (a*b)**2 (mod m).
   p1 = modul64(p1hi, p1lo, m);
   mulul64(p1, p1, &p1hi, &p1lo);       // Compute (a*b)**4 (mod m).
   p1 = modul64(p1hi, p1lo, m);

   /* The MM method uses a quantity r that is the smallest power of 2
   that is larger than m, and hence also larger than a and b. Here we
   deal with a variable hr that is just half of r. This is because r can
   be as large as 2**64, which doesn't fit in one 64-bit word. So we
   deal with hr, where 2**63 <= hr <= 1, and make the appropriate
   adjustments wherever it is used.
      We fix r at 2**64, and its log base 2 at 64. It doesn't hurt if
   they are too big, it's just that some quantities (e.g., mprime) come
   out larger than they would otherwise be. */

   hr = 0x8000000000000000LL;

   /* Now, for the MM method, first compute the quantities that are
   functions of only r and m, and hence are relatively constant. These
   quantities can be used repeatedly, without change, when raising a
   number to a large power modulo m.
      First use the extended GCD algorithm to compute two numbers rinv
   and mprime, such that

                         r*rinv - m*mprime = 1

   Reading this nodulo m, clearly r*rinv = 1 (mod m), i.e., rinv is the
   multiplicative inverse of r modulo m. It is needed to convert the
   result of MM back to a normal number. The other calculated number,
   mprime, is used in the MM algorithm. */

   xbinGCD(hr, m, &rinv, &mprime);      // xbinGCD, in effect, doubles hr.

   /* Do a partial check of the results. It is partial because the
   multiplications here give only the low-order half (64 bits) of the
   products. */

   if (2*hr*rinv - m*mprime != 1) {
      errors = 1;
   }

   /* Compute abar = a*r(mod m) and bbar = b*r(mod m). That is, abar =
   (a << 64)%m, and bbar = (b << 64)%m. */

   abar = modul64(a, 0, m);
   bbar = modul64(b, 0, m);

   p = montmul(abar, bbar, m, mprime); /* Compute a*b (mod m). */
   p = montmul(p, p, m, mprime);       /* Compute (a*b)**2 (mod m). */
   p = montmul(p, p, m, mprime);       /* Compute (a*b)**4 (mod m). */

   /* Convert p back to a normal number by p = (p*rinv)%m. */

   mulul64(p, rinv, &phi, &plo);
   p = modul64(phi, plo, m);
   if (p != p1) 
	   errors = 1;

   return errors;
}

