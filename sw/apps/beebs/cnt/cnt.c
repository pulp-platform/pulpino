/* BEEBS cnt benchmark

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

/* Original code from: WCET Benchmarks,
       http://www.mrtc.mdh.se/projects/wcet/benchmarks.html
   Permission to license under GPL obtained by email from Björn Lisper
 */

#include "support.h"

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)


/* $Id: cnt.c,v 1.3 2005/04/04 11:34:58 csg Exp $ */



/* sumcntmatrix.c */



//#include <sys/types.h>

//#include <sys/times.h>



// #define WORSTCASE 1

// #define MAXSIZE 100 Changed JG/Ebbe

#define MAXSIZE 10



// Typedefs

typedef int matrix [MAXSIZE][MAXSIZE];



// Forwards declarations

int main(void);

int Test(matrix);

int Initialize(matrix);

int InitSeed(void);

void Sum(matrix);

int RandomInteger(void);



// Globals

int Seed;

matrix Array;

int Postotal, Negtotal, Poscnt, Negcnt;



// The main function

int benchmark (void)

{

   InitSeed();

   //printf("\n   *** MATRIX SUM AND COUNT BENCHMARK TEST ***\n\n");

   //printf("RESULTS OF THE TEST:\n");

   Test(Array);

   return 0;
}


int Test(matrix Array)

{
   Initialize(Array);

   Sum(Array);

   return 0;
}





// Intializes the given array with random integers.

int Initialize(matrix Array)

{

   register int OuterIndex, InnerIndex;



   for (OuterIndex = 0; OuterIndex < MAXSIZE; OuterIndex++) //100 + 1

      for (InnerIndex = 0; InnerIndex < MAXSIZE; InnerIndex++) //100 + 1

         Array[OuterIndex][InnerIndex] = RandomInteger();



   return 0;

}





// Initializes the seed used in the random number generator.

int InitSeed (void)

{

   Seed = 0;

   return 0;

}



void Sum(matrix Array)

{

  register int Outer, Inner;



  int Ptotal = 0; /* changed these to locals in order to drive worst case */

  int Ntotal = 0;

  int Pcnt = 0;

  int Ncnt = 0;



  for (Outer = 0; Outer < MAXSIZE; Outer++) //Maxsize = 100

    for (Inner = 0; Inner < MAXSIZE; Inner++)

#ifdef WORSTCASE

      if (Array[Outer][Inner] >= 0) {

#else

	if (Array[Outer][Inner] < 0) {

#endif

	  Ptotal += Array[Outer][Inner];

	  Pcnt++;

	}

	else {

	  Ntotal += Array[Outer][Inner];

	  Ncnt++;

	}



  Postotal = Ptotal;

  Poscnt = Pcnt;

  Negtotal = Ntotal;

  Negcnt = Ncnt;

}





// This function returns in milliseconds the amount of compiler time

//int ttime()

//{

//  struct tms buffer;

//int utime;



//times(&buffer);

//utime = (buffer.tms_utime / 60.0) * 1000.0;

//return (utime);

//}





// Generates random integers between 0 and 8095

int RandomInteger(void)

{

   Seed = ((Seed * 133) + 81) % 8095;

   return Seed;

}











