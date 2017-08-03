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

#define M 28
#define N 16
#define T 28 //To translate k

int g_tmp_image[N][M];
int g_acc1[N][M][4];
int g_acc2[N][M][4];
int in_image[N][M]; //input
int gauss_image[N][M]; //output


//2D gauss blur filter for image processing
int main (int argc, char* argv[])
{
    int x,y,k;
    int tot[4];
    int Gauss[4]; //input

    tot[0]=0;

    for (k=T-1; k<=1+T; k++)
    {
        tot[k+2 - T] = tot[k+1 - T] + Gauss[k - T];
    }

    for (k=T-1; k<=1+T; k++)
    {
        tot[k+2 - T] = tot[k+1 - T] + Gauss[k - T];
    }

    for( x=1; x<N-1; x++)
    {
        for( y=0; y<M; y++)
        {
            g_acc1[x][y][0]=0;
            for( k=T-1; k<=1+T; k++)
            {
                g_acc1[x][y][k+2-T] = g_acc1[x][y][k+1-T] + 
                    in_image[x+k][y] * Gauss[k-T];
            }
            g_tmp_image[x][y] = g_acc1[x][y][3]/tot[3];
        }
    }
    for( x=1; x<N-1; x++)
    {
        for( y=1; y<M-1; y++)
        {
            g_acc2[x][y][0]=0;
            for( k=T-1; k<=1+T; k++)
            {
                g_acc2[x][y][k+2-T] = 
                    g_acc2[x][y][k+1-T] + g_tmp_image[x][y+k-T] * Gauss[k-T];
            }
            gauss_image[x][y] = g_acc2[x][y][3]/tot[3];
        }
    }

    eoc(0);

}
