// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

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
