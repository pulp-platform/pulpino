#include <stdio.h>
#include <stdlib.h>


#define N 2000000
#define T 1000

double a[N];
double b[N];

void init_array()
{
    int j;

    for (j=0; j<N; j++) {
        a[j] = ((double)j)/N;
    }
}

int main()
{
    int t, i, j;

    init_array();

    for (t = 0; t < T; t++) {
        for (i = 2; i < N - 1; i++) {
            b[i] = 0.33333 * (a[i-1] + a[i] + a[i + 1]);
        }
        for (j = 2; j < N - 1; j++) {
            a[j] = b[j];
        }
    }


    return 0;
}
