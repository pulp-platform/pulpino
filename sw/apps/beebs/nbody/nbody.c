/* BEEBS nbody benchmark

   Copyright (C) 2014 Embecosm Limited and University of Bristol

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

/* The original source code for this benchmark can be found here:

     http://benchmarksgame.alioth.debian.org/

   and was released under the following licence, disclaimers, and
   copyright:

   Revised BSD license

   This is a specific instance of the Open Source Initiative (OSI) BSD
   license template http://www.opensource.org/licenses/bsd-license.php

   Copyright 2004-2009 Brent Fulgham
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the
   distribution.

   Neither the name of "The Computer Language Benchmarks Game" nor the
   name of "The Computer Language Shootout Benchmarks" nor the names
   of its contributors may be used to endorse or promote products
   derived from this software without specific prior written
   permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.  */

#include <math.h>
#include <stdlib.h>

#include "support.h"

#define SCALE_FACTOR (REPEAT_FACTOR >> 8)

#define PI 3.141592653589793
#define SOLAR_MASS ( 4 * PI * PI )
#define DAYS_PER_YEAR 365.24

struct body {
   double x[3], fill, v[3], mass;
};

static struct body solar_bodies[] = {
   /* sun */
   {
      .x = { 0., 0., 0. },
      .v = { 0., 0., 0. },
      .mass = SOLAR_MASS
   },
   /* jupiter */
   {
      .x = { 4.84143144246472090e+00,
         -1.16032004402742839e+00,
         -1.03622044471123109e-01 },
      .v = { 1.66007664274403694e-03 * DAYS_PER_YEAR,
         7.69901118419740425e-03 * DAYS_PER_YEAR,
         -6.90460016972063023e-05 * DAYS_PER_YEAR },
      .mass = 9.54791938424326609e-04 * SOLAR_MASS
   },
   /* saturn */
   {
      .x = { 8.34336671824457987e+00,
         4.12479856412430479e+00,
         -4.03523417114321381e-01 },
      .v = { -2.76742510726862411e-03 * DAYS_PER_YEAR,
         4.99852801234917238e-03 * DAYS_PER_YEAR,
         2.30417297573763929e-05 * DAYS_PER_YEAR },
      .mass = 2.85885980666130812e-04 * SOLAR_MASS
   },
   /* uranus */
   {
      .x = { 1.28943695621391310e+01,
         -1.51111514016986312e+01,
         -2.23307578892655734e-01 },
      .v = { 2.96460137564761618e-03 * DAYS_PER_YEAR,
         2.37847173959480950e-03 * DAYS_PER_YEAR,
         -2.96589568540237556e-05 * DAYS_PER_YEAR },
      .mass = 4.36624404335156298e-05 * SOLAR_MASS
   },
   /* neptune */
   {
      .x = { 1.53796971148509165e+01,
         -2.59193146099879641e+01,
         1.79258772950371181e-01 },
      .v = { 2.68067772490389322e-03 * DAYS_PER_YEAR,
         1.62824170038242295e-03 * DAYS_PER_YEAR,
         -9.51592254519715870e-05 * DAYS_PER_YEAR },
      .mass = 5.15138902046611451e-05 * SOLAR_MASS
   }
};

static const int BODIES_SIZE = sizeof(solar_bodies) / sizeof(solar_bodies[0]);

void offset_momentum(struct body *bodies, unsigned int nbodies)
{
   unsigned int i, k;
   for (i = 0; i < nbodies; ++i)
      for (k = 0; k < 3; ++k)
         bodies[0].v[k] -= bodies[i].v[k] * bodies[i].mass
            / SOLAR_MASS;
}


double bodies_energy(struct body *bodies, unsigned int nbodies) {
   double dx[3], distance, e = 0.0;
   unsigned int i, j, k;

   for (i=0; i < nbodies; ++i) {
      e += bodies[i].mass * ( bodies[i].v[0] * bodies[i].v[0]
         + bodies[i].v[1] * bodies[i].v[1]
         + bodies[i].v[2] * bodies[i].v[2] ) / 2.;

      for (j=i+1; j < nbodies; ++j) {
         for (k = 0; k < 3; ++k)
            dx[k] = bodies[i].x[k] - bodies[j].x[k];

         distance = sqrt(dx[0] * dx[0] + dx[1] * dx[1]
            + dx[2] * dx[2]);
         e -= (bodies[i].mass * bodies[j].mass) / distance;
      }
   }
   return e;
}

int benchmark()
{
   int i;
   offset_momentum(solar_bodies, BODIES_SIZE);
   /*printf("%.9f\n", bodies_energy(solar_bodies, BODIES_SIZE));*/
   for (i = 0; i < 100; ++i)
       bodies_energy(solar_bodies, BODIES_SIZE);
   /*printf("%.9f\n", bodies_energy(solar_bodies, BODIES_SIZE));*/
   return 0;
}

