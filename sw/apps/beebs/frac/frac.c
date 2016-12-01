/* BEEBS frac benchmark

   Copyright (C) 2014 Embecosm Limited and University of Bristol

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
#include <math.h>


/* The following values are used to test for too small
   and too large values of v for an integer fraction to
   represent.

   These values are currently set for the amdal 5890.
*/

#define MAX 4.0e+10
#define MIN 3.0e-10

double
frac(double v, int *n, int *d, double error)
{
    /*
      given a number, v, this function outputs two integers,
      d and n, such that

        v = n / d

      to accuracy
        epsilon = | (v - n/d) / v | <= error

      input:  v = decimal number you want replaced by fraction.
          error = accuracy to which the fraction should
              represent the number v.

      output:  n = numerator of representing fraction.
           d = denominator of representing fraction.

      return value:  -1.0 if (v < MIN || v > MAX || error < 0.0)
             | (v - n/d) / v | otherwise.

      Note:  This program only works for positive numbers, v.

      reference:  Jerome Spanier and Keith B. Oldham, "An Atlas
        of Functions," Springer-Verlag, 1987, pp. 665-7.
    */

    int D, N, t;
    double epsilon, r, m;


    if (v < MIN || v > MAX || error < 0.0)
        return(-1.0);
    *d = D = 1;
    *n = (int)v;
    N = (*n) + 1;
    goto three;

one:    if (r > 1.0)
        goto two;
    r = 1.0/r;
two:    N += (*n)*(int)r;
    D += (*d)*(int)r;
    (*n) += N;
    (*d) += D;
three:  r = 0.0;
    if (v*(*d) == (double)(*n))
        goto four;
    r = (N - v*D)/(v*(*d) - (*n));
    if (r > 1.0)
        goto four;
    t = N;
    N = (*n);
    *n = t;
    t = D;
    D = (*d);
    *d = t;
four:
    epsilon = fabs(1.0 - (*n)/(v*(*d)));
    if (epsilon <= error)
        goto six;
    m = 1.0;
    do {
        m *= 10.0;
    } while (m*epsilon < 1.0);
    epsilon = 1.0/m * ((int)(0.5 + m*epsilon));
six:
    if (epsilon <= error)
        return(epsilon);
    if (r != 0.0)
        goto one;

    /* Fall through case, not sure what should be returned here, but
       must return something.  */
    return 0.0;
}

double nums[10] = {0.0, 1.12312, 10000.2, 1200, 3.1415, 3.1415926, 9.9999999, 12.1212, 0.0000012, 1.000001};

int benchmark()
{
  double error = 1.0e-10;
  int n,d,i;

  n = 0;
  for(i = 0; i < 10; ++i)
    frac(nums[i], &n, &d, error);

  return n;
}

