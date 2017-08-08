// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include <utils.h>
#include <string_lib.h>
#include <bench.h>

#include "sudokusolver.h"

int grid_init[SUDOKUSIZE * SUDOKUSIZE];
int grid_solved[SUDOKUSIZE * SUDOKUSIZE];
int solved[1];

int safe(int*, int, int, int);
void solve(int*, int, int, int*);
void sudokusolver(int*, int*);

void check(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "sudokusolver",   .test = check       },
  {0, 0}
};

int main() {
  return run_suite(testcases);
}

void check(testresult_t *result, void (*start)(), void (*stop)()) {
  printf("Start sudokusolver\n");
  start();
  // store sudoku to solve in grid_init
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      grid_init[i * SUDOKUSIZE + j] = 0;
    }
  }

  grid_init[0 * SUDOKUSIZE + 1] = 9; grid_init[0 * SUDOKUSIZE + 2] = 3; grid_init[0 * SUDOKUSIZE + 3] = 1; grid_init[0 * SUDOKUSIZE + 5] = 5; grid_init[0 * SUDOKUSIZE + 6] = 6; grid_init[0 * SUDOKUSIZE + 7] = 4;
  grid_init[1 * SUDOKUSIZE + 0] = 7; grid_init[1 * SUDOKUSIZE + 8] = 5;
  grid_init[2 * SUDOKUSIZE + 0] = 5; grid_init[2 * SUDOKUSIZE + 2] = 1; grid_init[2 * SUDOKUSIZE + 3] = 2; grid_init[2 * SUDOKUSIZE + 5] = 9; grid_init[2 * SUDOKUSIZE + 6] = 3; grid_init[2 * SUDOKUSIZE + 8] = 7;
  grid_init[3 * SUDOKUSIZE + 0] = 2; grid_init[3 * SUDOKUSIZE + 8] = 3;
  grid_init[4 * SUDOKUSIZE + 1] = 3; grid_init[4 * SUDOKUSIZE + 2] = 6; grid_init[4 * SUDOKUSIZE + 3] = 9; grid_init[4 * SUDOKUSIZE + 5] = 7; grid_init[4 * SUDOKUSIZE + 6] = 5; grid_init[4 * SUDOKUSIZE + 7] = 2;
  grid_init[5 * SUDOKUSIZE + 0] = 9; grid_init[5 * SUDOKUSIZE + 8] = 1;
  grid_init[6 * SUDOKUSIZE + 0] = 3; grid_init[6 * SUDOKUSIZE + 2] = 2; grid_init[6 * SUDOKUSIZE + 3] = 4; grid_init[6 * SUDOKUSIZE + 5] = 8; grid_init[6 * SUDOKUSIZE + 6] = 1; grid_init[6 * SUDOKUSIZE + 8] = 9;
  grid_init[7 * SUDOKUSIZE + 0] = 6; grid_init[7 * SUDOKUSIZE + 8] = 4;
  grid_init[8 * SUDOKUSIZE + 1] = 4; grid_init[8 * SUDOKUSIZE + 2] = 7; grid_init[8 * SUDOKUSIZE + 3] = 3; grid_init[8 * SUDOKUSIZE + 5] = 2; grid_init[8 * SUDOKUSIZE + 6] = 8; grid_init[8 * SUDOKUSIZE + 7] = 5;

  // loop two times. (cold/hot IC)
  for (int k = 0; k < 2; k++) {

    // reset result grid_solved and solved
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        grid_solved[i * SUDOKUSIZE + j] = grid_init[i * SUDOKUSIZE + j];
      }
    }
    solved[0] = 0;

    // start solver
    sudokusolver(grid_solved, solved);
    //printf("Iteration %d completed\n",k);
  }
  stop();
  // print solution
  printf("Solution:\n");

  for (int i = 0; i < 9; i++) {
    printf("\n");
    for (int j = 0; j < 9; j++) {
      printf("%d ", grid_solved[i * SUDOKUSIZE + j]);
    }
  }
  printf("\n");

  // check result!
  for (int i = 0; i < SUDOKUSIZE; i++) {
    for (int k = 0; k < SUDOKUSIZE; k++) {
      if (SUDOKUSOLVED[i * SUDOKUSIZE + k] != grid_solved[i * SUDOKUSIZE + k]) {
        result->errors++;
        printf("Error occurred at i=%d k=%d; Computed result %d does not match expected result %d\n", i, k, grid_solved[i * SUDOKUSIZE + k], SUDOKUSOLVED[i * SUDOKUSIZE + k]);
      }
    }
  }
}


// start solver. (this function is used for profiling)
void sudokusolver(int* grid_solved, int* solved)
{
  solve(grid_solved, 0, 0, solved);
}

// check if current index doesn't violate any sudoku constraints
int safe(int* grid_solved, int row, int col, int n)
{
  int r, c, br, bc;

  if (grid_solved[row * SUDOKUSIZE + col] == n) return 1;

  if (grid_solved[row * SUDOKUSIZE + col] != 0) return 0;

  for (c = 0; c < 9; c++)
    if (grid_solved[row * SUDOKUSIZE + c] == n) return 0;

  for (r = 0; r < 9; r++)
    if (grid_solved[r * SUDOKUSIZE + col] == n) return 0;

  /* br = row / 3; */
  /* bc = col / 3; */

  if (row > 5)
    br = 2;
  else if (row > 2)
    br = 1;
  else
    br = 0;

  if (col > 5)
    bc = 2;
  else if (col > 2)
    bc = 1;
  else
    bc = 0;

  for (r = br * 3; r < (br + 1) * 3; r++)
    for (c = bc * 3; c < (bc + 1) * 3; c++)
      if (grid_solved[r * SUDOKUSIZE + c] == n) return 0;

  return 1;
}

// solve sudoku! brute force
void solve(int* grid_solved, int row, int col, int* solved)
{
  int n, t;

  if (row == 9)
    solved[0] = 1;
  else
    for (n = 1; n <= 9; n++)
      if (safe(grid_solved, row, col, n)) {
        t = grid_solved[row * SUDOKUSIZE + col];
        grid_solved[row * SUDOKUSIZE + col] = n;

        if (col == 8)
          solve(grid_solved, row + 1, 0, solved);
        else
          solve(grid_solved, row, col + 1, solved);

        if (solved[0] == 0)
          grid_solved[row * SUDOKUSIZE + col] = t;
        else
          return;

      }
}
