/* BEEBS ludcmp benchmark

   MDH WCET BENCHMARK SUITE.

   *************************************************************************
   *                                                                       *
   *   SNU-RT Benchmark Suite for Worst Case Timing Analysis               *
   *   =====================================================               *
   *                              Collected and Modified by S.-S. Lim      *
   *                                           sslim@archi.snu.ac.kr       *
   *                                         Real-Time Research Group      *
   *                                        Seoul National University      *
   *                                                                       *
   *                                                                       *
   *        < Features > - restrictions for our experimental environment   *
   *                                                                       *
   *          1. Completely structured.                                    *
   *               - There are no unconditional jumps.                     *
   *               - There are no exit from loop bodies.                   *
   *                 (There are no 'break' or 'return' in loop bodies)     *
   *          2. No 'switch' statements.                                   *
   *          3. No 'do..while' statements.                                *
   *          4. Expressions are restricted.                               *
   *               - There are no multiple expressions joined by 'or',     *
   *                'and' operations.                                      *
   *          5. No library calls.                                         *
   *               - All the functions needed are implemented in the       *
   *                 source file.                                          *
   *                                                                       *
   *                                                                       *
   *************************************************************************
   *                                                                       *
   *  FILE: ludcmp.c                                                       *
   *  SOURCE : Turbo C Programming for Engineering                         *
   *                                                                       *
   *  DESCRIPTION :                                                        *
   *                                                                       *
   *     Simultaneous linear equations by LU decomposition.                *
   *     The arrays a[][] and b[] are input and the array x[] is output    *
   *     row vector.                                                       *
   *     The variable n is the number of equations.                        *
   *     The input arrays are initialized in function main.                *
   *                                                                       *
   *                                                                       *
   *  REMARK :                                                             *
   *                                                                       *
   *  EXECUTION TIME :                                                     *
   *                                                                       *
   *                                                                       *
   *************************************************************************

   Benchmark Suite for Real-Time Applications, by Sung-Soo Lim

      III-4. ludcmp.c : Simultaneous Linear Equations by LU Decomposition
                   (from the book C Programming for EEs by Hyun Soon Ahn)

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

/* NOTE: These arrays were all originally size 5 in every dimension.
   However, the code below is either broken, or very smart in how is
   accesses the data.  The compiler is warnings that there might be
   accesses beyond the bounds of the arrays, I've increased the array sizes
   to remove the warnings, but we should investigate the code below to
   ensure that the algorithm is doing the right thing.  */
float          a[8][9], b[6], x[6];

int             ludcmp( /* int nmax, */ int n, float eps);


static float
ludcmp_fabs(float n)
{
	float          f;

	if (n >= 0)
		f = n;
	else
		f = -n;
	return f;
}

int
ludcmp( /* int nmax, */ int n, float eps)
{

	int             i, j, k;
	float          w, y[10];

	if (n > 99 || eps <= 0.0)
		return (999);
	for (i = 0; i < n; i++) {
		if (ludcmp_fabs(a[i][i]) <= eps)
			return (1);
		for (j = i + 1; j <= n; j++) {
			w = a[j][i];
			if (i != 0)
				for (k = 0; k < i; k++)
					w -= a[j][k] * a[k][i];
			a[j][i] = w / a[i][i];
		}
		for (j = i + 1; j <= n; j++) {
			w = a[i + 1][j];
			for (k = 0; k <= i; k++)
				w -= a[i + 1][k] * a[k][j];
			a[i + 1][j] = w;
		}
	}
	y[0] = b[0];
	for (i = 1; i <= n; i++) {
		w = b[i];
		for (j = 0; j < i; j++)
			w -= a[i][j] * y[j];
		y[i] = w;
	}
	x[n] = y[n] / a[n][n];
	for (i = n - 1; i >= 0; i--) {
		w = y[i];
		for (j = i + 1; j <= n; j++)
			w -= a[i][j] * x[j];
		x[i] = w / a[i][i];
	}
	return (0);

}

/* Write to CHKERR from BENCHMARK to ensure that the core call within
   BENCHMARK is not optimised away.  */
volatile int chkerr;

int
benchmark (void)
{
  int             i, j/*, nmax = 50*/, n = 5;
  float          eps, w;

  eps = 1.0e-6;

  for (i = 0; i <= n; i++) {
          w = 0.0;
          for (j = 0; j <= n; j++) {
                  a[i][j] = (i + 1) + (j + 1);
                  if (i == j)
                          a[i][j] *= 10.0;
                  w += a[i][j];
          }
          b[i] = w;
  }

  chkerr = ludcmp( /* nmax, */ n, eps);
  return 0;
}


