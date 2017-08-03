// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <stdio.h>
#include <stdlib.h>

#define N 2000
#define T 1000

double a[N][N];
double b[N][N];

void init_array()
{
    int i, j;

    for (i=0; i<N; i++) {
        for (j=0; j<N; j++) {
            a[i][j] = ((double)j)/N;
        }
    }
}


int main()
{
    int t, i, j;
    double t_start, t_end;

    init_array();

    for (t=0; t<T; t++) {
        for (i=2; i<N-1; i++) {
            for (j=2; j<N-1; j++) {
                b[i][j]= 0.2*(a[i][j]+a[i][j-1]+a[i][1+j]+a[1+i][j]+a[i-1][j]);
            }
        }
        for (i=2; i<N-1; i++) {
            for (j=2; j<N-1; j++)   {
                a[i][j]=b[i][j];
            }
        }
    }

    return 0;
}
