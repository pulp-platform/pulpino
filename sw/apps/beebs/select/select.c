/* BEEBS select benchmark

   SOURCE : Numerical Recipes in C - The Second Edition

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

#define SWAP(a,b) temp=(a);(a)=(b);(b)=temp;

float arr[20] = {
  5, 4, 10.3, 1.1, 5.7, 100, 231, 111, 49.5, 99,
  10, 150, 222.22, 101, 77, 44, 35, 20.54, 99.99, 888.88
};


float select(unsigned long k, unsigned long n)
{
	unsigned long i,ir,j,l,mid;
	float a,temp;
	int flag, flag2;

	l=1;
	ir=n;
	flag = flag2 = 0;
	while (!flag) {
		if (ir <= l+1) {
			if (ir == l+1)
			  if (arr[ir] < arr[l]) {
			    SWAP(arr[l],arr[ir])
			      }
			flag = 1;
		} else if (!flag) {
			mid=(l+ir) >> 1;
			SWAP(arr[mid],arr[l+1])
			if (arr[l+1] > arr[ir]) {
				SWAP(arr[l+1],arr[ir])
			}
			if (arr[l] > arr[ir]) {
				SWAP(arr[l],arr[ir])
			}
			if (arr[l+1]> arr[l]) {
				SWAP(arr[l+1],arr[l])
			}
			i=l+1;
			j=ir;
			a=arr[l];
			while (!flag2) {
				i++;
				while (arr[i] < a) i++;
				j--;
				while (arr[j] > a) j--;
				if (j < i) flag2 = 1;
				if (!flag2) SWAP(arr[i],arr[j]);

			}
			arr[l]=arr[j];
			arr[j]=a;
			if (j >= k) ir=j-1;
			if (j <= k) l=i;
		}

	}
	return arr[k];
}

int
benchmark (void)
{
  select(10, 20);
  return 0;
}


