#include "utils.h"
#include "string_lib.h"
#include "bar.h"

#include "sudokusolver.h"

__attribute__ ((section(".heapsram"))) int grid_init[SUDOKUSIZE*SUDOKUSIZE];
__attribute__ ((section(".heapsram"))) int grid_solved[SUDOKUSIZE*SUDOKUSIZE];
__attribute__ ((section(".heapsram"))) int solved[1];

int safe(int*, int, int, int);
void solve(int*, int, int, int*);
void sudokusolver(int*, int*);


int main()
{

  int i,j,k;
  int error = 0;



    printf("Start sudokusolver\n",0,0,0,0);

    // store sudoku to solve in grid_init
    for (i=0;i<9;i++){
      for (j=0;j<9;j++){
        grid_init[i*SUDOKUSIZE+j] = 0;
      }
    }
    /* grid_init[0*SUDOKUSIZE+7] = 1; grid_init[0*SUDOKUSIZE+8] = 2; grid_init[1*SUDOKUSIZE+0] = 4; grid_init[1*SUDOKUSIZE+4] = 9; grid_init[2*SUDOKUSIZE+7] = 5; */
    /* grid_init[3*SUDOKUSIZE+1] = 7; grid_init[3*SUDOKUSIZE+3] = 2; grid_init[4*SUDOKUSIZE+0] = 6; grid_init[4*SUDOKUSIZE+6] = 4; grid_init[5*SUDOKUSIZE+3] = 1; */
    /* grid_init[5*SUDOKUSIZE+5] = 8; grid_init[6*SUDOKUSIZE+1] = 1; grid_init[6*SUDOKUSIZE+2] = 8; grid_init[7*SUDOKUSIZE+4] = 3; grid_init[7*SUDOKUSIZE+6] = 7; */
    /* grid_init[8*SUDOKUSIZE+0] = 5; grid_init[8*SUDOKUSIZE+2] = 2; */

    grid_init[0*SUDOKUSIZE+1] = 9; grid_init[0*SUDOKUSIZE+2] = 3; grid_init[0*SUDOKUSIZE+3] = 1; grid_init[0*SUDOKUSIZE+5] = 5; grid_init[0*SUDOKUSIZE+6] = 6; grid_init[0*SUDOKUSIZE+7] = 4;
    grid_init[1*SUDOKUSIZE+0] = 7; grid_init[1*SUDOKUSIZE+8] = 5;
    grid_init[2*SUDOKUSIZE+0] = 5; grid_init[2*SUDOKUSIZE+2] = 1; grid_init[2*SUDOKUSIZE+3] = 2; grid_init[2*SUDOKUSIZE+5] = 9; grid_init[2*SUDOKUSIZE+6] = 3; grid_init[2*SUDOKUSIZE+8] = 7;
    grid_init[3*SUDOKUSIZE+0] = 2; grid_init[3*SUDOKUSIZE+8] = 3;
    grid_init[4*SUDOKUSIZE+1] = 3; grid_init[4*SUDOKUSIZE+2] = 6; grid_init[4*SUDOKUSIZE+3] = 9; grid_init[4*SUDOKUSIZE+5] = 7; grid_init[4*SUDOKUSIZE+6] = 5; grid_init[4*SUDOKUSIZE+7] = 2;
    grid_init[5*SUDOKUSIZE+0] = 9; grid_init[5*SUDOKUSIZE+8] = 1;
    grid_init[6*SUDOKUSIZE+0] = 3; grid_init[6*SUDOKUSIZE+2] = 2; grid_init[6*SUDOKUSIZE+3] = 4; grid_init[6*SUDOKUSIZE+5] = 8; grid_init[6*SUDOKUSIZE+6] = 1; grid_init[6*SUDOKUSIZE+8] = 9;
    grid_init[7*SUDOKUSIZE+0] = 6; grid_init[7*SUDOKUSIZE+8] = 4;
    grid_init[8*SUDOKUSIZE+1] = 4; grid_init[8*SUDOKUSIZE+2] = 7; grid_init[8*SUDOKUSIZE+3] = 3; grid_init[8*SUDOKUSIZE+5] = 2; grid_init[8*SUDOKUSIZE+6] = 8; grid_init[8*SUDOKUSIZE+7] = 5;

    // loop two times. (cold/hot IC)
    for (k=0;k<2;k++) {

      // reset result grid_solved and solved
      for (i=0;i<9;i++){
        for (j=0;j<9;j++){
          grid_solved[i*SUDOKUSIZE+j] = grid_init[i*SUDOKUSIZE+j];
        }
      }
      solved[0] = 0;

      // start solver
      sudokusolver(grid_solved, solved);
      //printf("Iteration %d completed\n",k,0,0,0);
    }

    // print solution
    printf("Solution:\n",0,0,0,0);

    for (i=0;i<9;i++){
      printf("\n",0,0,0,0);
      for (j=0;j<9;j++){
        printf("%d ",grid_solved[i*SUDOKUSIZE+j],0,0,0);
      }
    }
    printf("\n",0,0,0,0);

    // check result!
    for (i=0;i<SUDOKUSIZE;i++) { 
      for (k=0;k<SUDOKUSIZE;k++) {
        if (SUDOKUSOLVED[i*SUDOKUSIZE+k] != grid_solved[i*SUDOKUSIZE+k]) {
          error = error + 1;
          printf("Error occurred at i=%d k=%d; Computed result %d does not match expected result %d\n",i,k,grid_solved[i*SUDOKUSIZE+k],SUDOKUSOLVED[i*SUDOKUSIZE+k]);
        }
      }
    }
  
  print_summary(error);
  eoc(0);
}

// start solver. (this function is used for profiling)
void sudokusolver(int* grid_solved, int* solved)
{
  solve(grid_solved, 0, 0, solved);
}

// check if current index doesnt violate any sudoku constraints
int safe(int* grid_solved, int row, int col, int n)
{
  int r, c, br, bc;

  if (grid_solved[row*SUDOKUSIZE+col] == n) return 1;

  if (grid_solved[row*SUDOKUSIZE+col] != 0) return 0;

  for (c = 0; c < 9; c++)
    if (grid_solved[row*SUDOKUSIZE+c] == n) return 0;

  for (r = 0; r < 9; r++)
    if (grid_solved[r*SUDOKUSIZE+col] == n) return 0;

  /* br = row / 3; */
  /* bc = col / 3; */

  if (row>5)
    br = 2;
  else if (row>2)
    br = 1;
  else
    br = 0;

  if (col>5)
    bc = 2;
  else if (col>2)
    bc = 1;
  else
    bc = 0;

  for (r = br * 3; r < (br + 1) * 3; r++)
    for (c = bc * 3; c < (bc + 1) * 3; c++)
      if (grid_solved[r*SUDOKUSIZE+c] == n) return 0;

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
        t = grid_solved[row*SUDOKUSIZE+col];
        grid_solved[row*SUDOKUSIZE+col] = n;

        if (col == 8)
          solve(grid_solved, row + 1, 0, solved);
        else
          solve(grid_solved, row, col + 1, solved);

        if (solved[0] == 0)
          grid_solved[row*SUDOKUSIZE+col] = t;
        else
          return;

      }
}
