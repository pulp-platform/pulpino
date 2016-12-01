/* BEEBS recursion benchmark

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

/* $Id: recursion.c,v 1.2 2005/04/04 11:34:58 csg Exp $ */



/* Generate an example of recursive code, to see  *

 * how it can be modeled in the scope graph.      */



/* self-recursion  */

int fib(int i)

{

  if(i==0)

    return 1;

  if(i==1)

    return 1;

  return fib(i-1) + fib(i-2);

}



/* mutual recursion */

int anka(int);



int kalle(int i)

{

  if(i<=0)

    return 0;

  else

    return anka(i-1);

}



int anka(int i)

{

  if(i<=0)

    return 1;

  else

    return kalle(i-1);

}



volatile int In;



int
benchmark(void)
{
  In = fib(10);
  return 0;
}


