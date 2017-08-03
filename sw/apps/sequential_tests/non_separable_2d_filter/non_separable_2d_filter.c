// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "utils.h"
#include "bar.h"
#include "string_lib.h"
#include "timer.h"


void START_TIMER() {
  reset_timer();
  start_timer();
}

void STOP_TIMER() {
  stop_timer();
  printf("Nr. cycles: %d\n", get_time(), 0, 0, 0);
}

typedef unsigned char Pixu;
typedef unsigned char PixuV __attribute__((vector_size (4)));
typedef signed char   PixsV __attribute__((vector_size (4)));



typedef signed char   Filts;
typedef unsigned char FiltuV __attribute__((vector_size (4)));
typedef signed char   FiltsV __attribute__((vector_size (4)));

typedef signed short  WeightT;

#define MAX_LINE  350


static __inline void __attribute__ ((__always_inline__)) non_separable_2d_filter_NbyN(
		  Pixu *In,
		  Pixu *Out,
		  const int LineSize,
		  const int LineCount,
		  Filts * __restrict__ Ker,
		  const int KerDim,
		  const WeightT Weight)

{
	int i, ki, j, kj;

	for (i = KerDim/2; i < LineCount-KerDim/2; i++) {
		for (j = KerDim/2; j < LineSize-KerDim/2; j++) {
			int Val = 0;
			for (ki = -KerDim/2; ki < (KerDim/2 +1); ki++) {
				for (kj = -KerDim/2; kj < (KerDim/2 +1); kj++) {
					Val += In[(i+ki)*LineSize + j + kj] * Ker[(ki+KerDim/2)*KerDim + kj + KerDim/2];
				}
			}
			Out[i*LineSize + j] = (Val*Weight)>>15;
		}
	}
}
static __inline void __attribute__ ((__always_inline__)) vectorial_non_separable_2d_filter_NbyN(
		  Pixu *In,
		  Pixu *Out,
		  const int LineSize,
		  const int LineCount,
		  Filts * __restrict__ Ker,
		  const int KerDim,
		  const WeightT Weight)

{
	int i, ki, j, kj;

	for (i = KerDim/2; i < LineCount-KerDim/2; i++) {
		for (j = KerDim/2; j < LineSize-KerDim/2; j++) {
			FiltsV Val = {0};
			PixuV *InV = (PixuV *) &(In[(i-KerDim/2)*LineSize + (j-KerDim/2)]);

			for (ki = 0; ki < KerDim; ki++) {
				for (kj = 0; kj < (KerDim/4+1); kj++) {
					Val += InV[ki*(KerDim/4)+kj] * Ker[ki*(KerDim/4)+kj];
				}
				InV++;
			}
			Out[i*LineSize + j] = ((Val[0]+Val[1]+Val[2]+Val[3])*Weight)>>15;
		}
	}
}

void non_separable_2d_filter_3by3(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 3, Weight);
	STOP_TIMER();
}

void vectorial_non_separable_2d_filter_3by3(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	vectorial_non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 3, Weight);
	STOP_TIMER();
}

void non_separable_2d_filter_5by5(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 5, Weight);
	STOP_TIMER();
}

void vectorial_non_separable_2d_filter_5by5(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	vectorial_non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 5, Weight);
	STOP_TIMER();
}

void non_separable_2d_filter_7by7(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 7, Weight);
	STOP_TIMER();
}

void vectorial_non_separable_2d_filter_7by7(Pixu *In, Pixu *Out, const int LineSize, const int LineCount, Filts * __restrict__ Ker, const WeightT Weight)

{
	START_TIMER();
	vectorial_non_separable_2d_filter_NbyN(In, Out, LineSize, LineCount, Ker, 7, Weight);
	STOP_TIMER();
}

#define LINE 80
#define COL 60

__attribute__ ((section(".heapsram"))) Pixu ImageIn[LINE][COL];
__attribute__ ((section(".heapsram"))) Pixu ImageOut[LINE][COL];


void non_separable_2d_filter() {
	printf("\n------------ Non Separable 2D filters ----------------------------------\n", 0, 0, 0, 0);

	{
		Filts Ker[3][3] = {
			{1,1,1},
			{1,1,1},
			{1,1,1},
			};
		Filts KerV[3][4] = {
			{1,1,1,0},
			{1,1,1,0},
			{1,1,1,0},
			};
		WeightT	Weight = (32767/9); // 1/9 in fractionnal

		printf("\nScalar Non Separable 2d Filter Image [%1dx%1d], Kernel [3x3]\n", LINE, COL, 0, 0);
		non_separable_2d_filter_3by3((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) Ker, Weight);

		printf("\nVectorial Non Separable 2d Filter Image [%1dx%1d], Kernel [3x3]\n", LINE, COL, 0, 0);
		vectorial_non_separable_2d_filter_3by3((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) KerV, Weight);
	}
	{
		Filts Ker[5][5] = {
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			{1,1,1,1,1},
			};
		Filts KerV[5][8] = {
			{1,1,1,1,1,0,0,0},
			{1,1,1,1,1,0,0,0},
			{1,1,1,1,1,0,0,0},
			{1,1,1,1,1,0,0,0},
			{1,1,1,1,1,0,0,0},
			};
		WeightT	Weight = (32767/25); // 1/25 in fractionnal

		printf("\nScalar Non Separable 2d Filter Image [%1dx%1d], Kernel [5x5]\n", LINE, COL, 0, 0);
		non_separable_2d_filter_5by5((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) Ker, Weight);

		printf("\nVectorial Non Separable 2d Filter Image [%1dx%1d], Kernel [5x5]\n", LINE, COL, 0, 0);
		vectorial_non_separable_2d_filter_5by5((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) KerV, Weight);
	}
	{
		Filts Ker[7][7] = {
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1},
			};
		Filts KerV[7][8] = {
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			{1,1,1,1,1,1,1,0},
			};
		WeightT	Weight = (32767/49); // 1/49 in fractionnal

		printf("\nScalar Non Separable 2d Filter Image [%1dx%1d], Kernel [7x7]\n", LINE, COL, 0, 0);
		non_separable_2d_filter_7by7((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) Ker, Weight);

		printf("\nVectorial Non Separable 2d Filter Image [%1dx%1d], Kernel [7x7]\n", LINE, COL, 0, 0);
		vectorial_non_separable_2d_filter_7by7((Pixu *) ImageIn, (Pixu *) ImageOut, LINE, COL, (Filts * __restrict__) KerV, Weight);
	}

	printf("\n------------------------------------------------------------------------\n", 0, 0, 0, 0);
	
}

int main() {
  if(get_core_id() == 0) {
    non_separable_2d_filter();
    
    eoc(0);
  }

  synch_barrier();

  return 0;
}
