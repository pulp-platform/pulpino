/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */

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
