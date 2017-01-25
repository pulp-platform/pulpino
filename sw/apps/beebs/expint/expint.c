/* BEEBS template benchmark

   ***********************************************************************
   * FROM:
   *   http://sron9907.sron.nl/manual/numrecip/c/expint.c
   *
   * FEATURE:
   *   One loop depends on a loop-invariant value to determine
   *   if it run or not.
   *
   ***********************************************************************

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

long int expint(int n, long int x);


long int foo(long int x)
{
  return (x*x+(8*x))<<(4-x);
}


/* Function with same flow, different data types,
   nonsensical calculations */
long int expint(int n, long int x)
{
  int      i,ii,nm1;
  long int a,b,c,d,del,fact,h,psi,ans;

  nm1=n-1;                      /* arg=50 --> 49 */

  if(x>1)                       /* take this leg? */
    {
      b=x+n;
      c=2e6;
      d=3e7;
      h=d;

      ans = 0;
      for (i=1;i<=100;i++)      /* MAXIT is 100 */
        {
          a = -i*(nm1+i);
          b += 2;
          d=10*(a*d+b);
          c=b+a/c;
          del=c*d;
          h *= del;
          if (del < 10000)
            {
              ans=h*-x;
              return ans;
            }
        }
    }
  else                          /* or this leg? */
    {
      // For the current argument, will always take
      // '2' path here:
      ans = nm1 != 0 ? 2 : 1000;
      fact=1;
      for (i=1;i<=100;i++)      /* MAXIT */
        {
          fact *= -x/i;
          if (i != nm1)         /* depends on parameter n */
            del = -fact/(i-nm1);
          else                  /* this fat piece only runs ONCE */
            {                   /* runs on iter 49 */
              psi = 0x00FF;
              for (ii=1;ii<=nm1;ii++) /*  */
                psi += ii + nm1;
              del=psi+fact*foo(x);
            }
          ans += del;
          /* conditional leave removed */
        }

    }
  return ans;
}

/* The benchmark result is a volatile global, writing to this ensures
   the call inside BENCHMARK is not optimised away.  */
volatile int benchmark_result = 0;

int
benchmark (void)
{
  benchmark_result = expint(50,1);
  return 0;
}


