/* BEEBS st benchmark

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
#define SCALE_FACTOR    (REPEAT_FACTOR >> 8)

/* stats.c */

/* 2012/09/28, Jan Gustafsson <jan.gustafsson@mdh.se>
 * Changes:
 *  - time is only enabled if the POUT flag is set
 *  - st.c:30:1:  main () warning: type specifier missing, defaults to 'int': fixed
 */


/* 2011/10/18, Benedikt Huber <benedikt@vmars.tuwien.ac.at>
 * Changes:
 *  - Measurement and Printing the Results is only enabled if the POUT flag is set
 *  - Added Prototypes for InitSeed and RandomInteger
 *  - Changed return type of InitSeed from 'missing (default int)' to 'void'
 */

#include <math.h>


#define MAX 100

void InitSeed(void);
int RandomInteger();
void Initialize(double[]);
void Calc_Sum_Mean(double[], double *, double *);
void Calc_Var_Stddev(double[], double, double *, double *);
void Calc_LinCorrCoef(double[], double[], double, double);


/* Statistics Program:
 * This program computes for two arrays of numbers the sum, the
 * mean, the variance, and standard deviation.  It then determines the
 * correlation coefficient between the two arrays.
 */

int Seed;
double ArrayA[MAX], ArrayB[MAX];
double SumA, SumB;
double Coef;

int
benchmark()
{
#ifdef POUT
   long StartTime, StopTime;
   float TotalTime;
#endif

   double MeanA, MeanB, VarA, VarB, StddevA, StddevB /*, Coef*/;

   InitSeed ();
#ifdef POUT
   printf ("\n   *** Statictics TEST ***\n\n");
   StartTime = ttime();
#endif

   Initialize(ArrayA);
   Calc_Sum_Mean(ArrayA, &SumA, &MeanA);
   Calc_Var_Stddev(ArrayA, MeanA, &VarA, &StddevA);

   Initialize(ArrayB);
   Calc_Sum_Mean(ArrayB, &SumB, &MeanB);
   Calc_Var_Stddev(ArrayB, MeanB, &VarB, &StddevB);

   /* Coef will have to be used globally in Calc_LinCorrCoef since it would
      be beyond the 6 registers used for passing parameters
   */
   Calc_LinCorrCoef(ArrayA, ArrayB, MeanA, MeanB /*, &Coef*/);

#ifdef POUT
   StopTime = ttime();
   TotalTime = (StopTime - StartTime) / 1000.0;
   printf("     Sum A = %12.4f,      Sum B = %12.4f\n", SumA, SumB);
   printf("    Mean A = %12.4f,     Mean B = %12.4f\n", MeanA, MeanB);
   printf("Variance A = %12.4f, Variance B = %12.4f\n", VarA, VarB);
   printf(" Std Dev A = %12.4f, Variance B = %12.4f\n", StddevA, StddevB);
   printf("\nLinear Correlation Coefficient = %f\n", Coef);
#endif
   return 0;
}


void InitSeed ()
/*
 * Initializes the seed used in the random number generator.
 */
{
   Seed = 0;
}


void Calc_Sum_Mean(double Array[], double *Sum, double *Mean)
{
   int i;

   *Sum = 0;
   for (i = 0; i < MAX; i++)
      *Sum += Array[i];
   *Mean = *Sum / MAX;
}


double Square(double x)
{
   return x*x;
}


void Calc_Var_Stddev(double Array[], double Mean, double *Var, double *Stddev)
{
   int i;
   double diffs;

   diffs = 0.0;
   for (i = 0; i < MAX; i++)
      diffs += Square(Array[i] - Mean);
   *Var = diffs/MAX;
   *Stddev = sqrt(*Var);
}


void Calc_LinCorrCoef(double ArrayA[], double ArrayB[], double MeanA, double MeanB /*, Coef*/)
{
   int i;
   double numerator, Aterm, Bterm;

   numerator = 0.0;
   Aterm = Bterm = 0.0;
   for (i = 0; i < MAX; i++) {
      numerator += (ArrayA[i] - MeanA) * (ArrayB[i] - MeanB);
      Aterm += Square(ArrayA[i] - MeanA);
      Bterm += Square(ArrayB[i] - MeanB);
      }

   /* Coef used globally */
   Coef = numerator / (sqrt(Aterm) * sqrt(Bterm));
}



void Initialize(double Array[])
/*
 * Intializes the given array with random integers.
 */
{
  register int i;

for (i=0; i < MAX; i++)
   Array [i] = i + RandomInteger ()/8095.0;
}


int RandomInteger()
/*
 * Generates random integers between 0 and 8095
 */
{
   Seed = ((Seed * 133) + 81) % 8095;
   return (Seed);
}

