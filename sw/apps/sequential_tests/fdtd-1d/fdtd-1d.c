#define N 1000000
#define T 10000
double h[N];
double e[N+1];
#define coeff1 0.5
#define coeff2 0.7

void init_array()
{
    int i, j;

        for (j=0; j<N; j++) {
            h[j] = ((double)j)/N;
            e[j] = ((double)j)/N;
        }
}

int main()
{
    int t, i, j, k, l;


    init_array();

    for (t=1; t<=T; t++){
	    for (i=1; i<=N-1; i++)
		    e[i] = e[i] - coeff1*(h[i]-h[i-1]);
	    for (i=0; i<=N-1; i++)
		    h[i] = h[i] - coeff2*(e[i+1]-e[i]);
    }

    return 0;
}
