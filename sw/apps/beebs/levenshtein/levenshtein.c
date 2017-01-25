/* BEEBS levenshtein benchmark

   c: levenhstein.c
   Copyright (C) 2011 Miguel Serrano

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.


   Copyright (C) 2014 Embecosm Limited and University of Bristol

   Contributor James Pallister <james.pallister@bristol.ac.uk>

   This file is part of the Bristol/Embecosm Embedded Benchmark Suite.
   Originally from https://github.com/miguelvps/c/

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


#include <string.h>

static int min(int x, int y) {
    return x < y ? x : y;
}

int levenshtein_distance(const char *s, const char *t) {
    int i, j;
    int sl = strlen(s);
    int tl = strlen(t);
    int d[sl + 1][tl + 1];

    for (i = 0; i <= sl; i++)
        d[i][0] = i;

    for (j = 0; j <= tl; j++)
        d[0][j] = j;

    for (j = 1; j <= tl; j++) {
        for (i = 1; i <= sl; i++) {
            if (s[i - 1] == t[j - 1]) {
                d[i][j] = d[i - 1][j - 1];
            }
            else {
                d[i][j] = min(d[i - 1][j] + 1,  /* deletion */
                              min(d[i][j - 1] + 1,  /* insertion */
                                  d[i - 1][j - 1] + 1));    /* substitution */
            }
        }
    }
    return d[sl][tl];
}

const char *strings[] = {"srrjngre", "asfcjnsdkj", "string", "msd",
    "strings"};

int benchmark()
{
  int i, j;
  volatile unsigned sum = 0;

  for(i = 0; i < 5; ++i)
    for(j = 0; j < 5; ++j)
      sum += levenshtein_distance(strings[i], strings[j]);

  return sum;

}

