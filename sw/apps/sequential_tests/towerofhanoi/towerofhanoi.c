#include "utils.h"
#include "string_lib.h"
#include "bar.h"

#define NR_PLATES 12

void ToH(int, char*, char*, char*);

void computeGold(int, char*, char*, char*);

__attribute__ ((section(".heapscm"))) char A[NR_PLATES];
__attribute__ ((section(".heapscm"))) char B[NR_PLATES];
__attribute__ ((section(".heapscm"))) char C[NR_PLATES];


int main()
{
  int i,j;
  int error = 0;


  printf("Start towerofhanoi\n",0,0,0,0);

  for (j=0; j<2; j++) {
    for (i=0;i<NR_PLATES;i++){
      A[i] = i;
      B[i] = 0;
      C[i] = 0;
    }

    computeGold(NR_PLATES, A, B, C);
  }
  
  for (i=0;i<NR_PLATES;i++) {
    if (B[i] != i) {
      printf("expected: %d, actual: %d\n",i,B[i],0,0);
      error = error + 1;
    }
  }
  

  print_summary(error);
  

  eoc(0);
}

void computeGold(int n, char* A, char* B, char* C)
{
  ToH(n-1,A,B,C);
}

void ToH(int n, char* A, char* B, char* C)
{
  if(n==0)
    {
      B[n] = A[n];
      A[n] = 0;
    }
  else {
    ToH(n-1,A,C,B);
    B[n] = A[n];
    A[n] = 0;
    ToH(n-1,C,B,A);
  }
 
}
