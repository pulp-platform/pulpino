// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
// IPM: Inflexion Point Method
// see http://ch.mathworks.com/help/images/morphology-fundamentals-dilation-and-erosion.html

#include "ipm.h"

#define max(x,y) (x > y ? x : y)
#define min(x,y) (x < y ? x : y)

void dilation(int A[N + 1][N + 1], int D[N + 1][N + 1], int B[M][M]) {
  int i, j, k, h;
  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      D[i][j] = 0;

  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      for (h = 0; h < M; h++)
        for (k = 0; k < M; k++)
          D[i][j] = max(A[i + h][j + k] * B[h][k], D[i][j]);
}

void erosion(int A[N + 1][N + 1], int E[N + 1][N + 1], int B[M][M]) {
  int i, j, k, h;
  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      E[i][j] = 0;

  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      for (h = 0; h < M; h++)
        for (k = 0; k < M; k++)
          E[i][j] = min(A[i + h][j + k] * B[h][k], E[i][j]);
}

void laplacian_filter(int A[N + 1][N + 1], int R[N + 1][N + 1], int B[M][M]) {
  int D[N][N] = {0};
  int E[N][N] = {0};
  int i, j, k, h;

  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      R[i][j] = 0;

  for (i = 1; i < N - 1; i++)
    for (j = 1; j < N - 1; j++)
      for (h = 0; h < M; h++)
        for (k = 0; k < M; k++) {
          D[i][j] = max(A[i + h][j + k] * B[h][k], D[i][j]);
          E[i][j] = min(A[i + h][j + k] * B[h][k], E[i][j]);
          R[i][j] = ((D[i][j] + E[i][j]) / 2) - A[i][j];
        }

}
