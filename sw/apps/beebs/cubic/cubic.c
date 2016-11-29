/* BEEBS cubic benchmark

   Contributor: James Pallister <james.pallister@bristol.ac.uk>
   From http://www.snippets.org/.

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

/* +++Date last modified: 05-Jul-1997 */

/*
 **  CUBIC.C - Solve a cubic polynomial
 **  public domain by Ross Cottrell
 */

#include <math.h>
#include "snipmath.h"

void SolveCubic(double  a,
      double  b,
      double  c,
      double  d,
      int    *solutions,
      double *x)
{
   long double    a1 = b/a, a2 = c/a, a3 = d/a;
   long double    Q = (a1*a1 - 3.0*a2)/9.0;
   long double R = (2.0*a1*a1*a1 - 9.0*a1*a2 + 27.0*a3)/54.0;
   double    R2_Q3 = R*R - Q*Q*Q;

   double    theta;

   if (R2_Q3 <= 0)
   {
      *solutions = 3;
      theta = acos(R/sqrt(Q*Q*Q));
      x[0] = -2.0*sqrt(Q)*cos(theta/3.0) - a1/3.0;
      x[1] = -2.0*sqrt(Q)*cos((theta+2.0*PI)/3.0) - a1/3.0;
      x[2] = -2.0*sqrt(Q)*cos((theta+4.0*PI)/3.0) - a1/3.0;
   }
   else
   {
      *solutions = 1;
      x[0] = pow(sqrt(R2_Q3)+fabs(R), 1/3.0);
      x[0] += Q/x[0];
      x[0] *= (R < 0.0) ? 1 : -1;
      x[0] -= a1/3.0;
   }
}

#ifdef TEST


#endif /* TEST */

/* vim: set ts=3 sw=3 et: */
