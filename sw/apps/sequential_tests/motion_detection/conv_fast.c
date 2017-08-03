// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

// SumDotp and Dotp are now defined on shorts!

#ifdef DOTP
  #define SumDotp(a, b, c) __builtin_pulp_sdotsp2(a, b, c)
  #define Dotp(a, b)       __builtin_pulp_dotsp2(a, b)
#endif
typedef short pixel;
typedef short pixelV __attribute__((vector_size (4)));

void  __attribute__ ((noinline)) Conv3x3_fast(pixel * __restrict__ In, pixel * __restrict__ Out, pixel * __restrict__ Coeff)
{
#ifdef DOTP

  int i, j;
  pixelV V0, V1, V2, V3, V4;
  /* Move all Kernel coeffs into registers */
  // initialize constants C0-C4
  pixelV C0 = *(pixelV *)Coeff;
  pixelV C1 = *(pixelV *)(Coeff+3);
  pixelV C2 = *(pixelV *)(Coeff+6);
  //  pixelV C3 = {Coeff[2], Coeff[5]};
  pixelV C4 = {Coeff[8],       0 };
    pixelV C3 = {Coeff[5], Coeff[2]};
  //pixelV C4 = {0,        Coeff[8]};

  // define mask for shuffling
    pixelV Mask  = {2,0};
    //pixelV Mask  = {3,0};

  /* For fresh data */
  pixelV *VectIn;
  // pointer to output
  pixel *O;
  // accumulator result
  int S;
  
  /* in a 3x3 image we index the pixels with numbers 1-9 */
  /* 1 2 3 */
  /* 4 5 6 */
  /* 7 8 9 */

  /* Half kernel size has to be removed left, right, top and bottom for proper and safe operations */
  for (i=0; i<(IMG_WIDTH-2); i++) {
	  
    // initialize first 3x3 image
    /* VectIn points, as a vector to the top left corner of the current vertical strip */
    VectIn = (pixelV *) (In + i);
    // read pixels 1,2
    V1 = *VectIn;
    VectIn+=(IMG_WIDTH>>1);
    // read pixels 4,5
    V2 = *VectIn;
    VectIn+=(IMG_WIDTH>>1);
    // read pixel 3
    //    V3 = (pixelV) { In[IMG_HEIGHT*(0)+2+i], 0             };
    // read pixel 6
    //    V4 = (pixelV) { 0            , In[IMG_HEIGHT*(1)+2+i] };
    // read pixel 3
    //        V3 = (pixelV) { 0, In[IMG_HEIGHT*(0)+2+i]             };
        V3 = (pixelV) { In[IMG_HEIGHT*(0)+2+i], 0             };
    // read pixel 6
        V4 = (pixelV) { In[IMG_HEIGHT*(1)+2+i], 0 };
	//V4 = (pixelV) { 0, In[IMG_HEIGHT*(1)+2+i] };

    /* O points to the center of the first kernel to be evaluated at the top of current vertical strip */
    O = Out + IMG_WIDTH + i + 1;
    /* Process one full vertical strip */
    for (j=0; j<(IMG_HEIGHT-2); j++) {
      V0 = V1;
      V1 = V2;
      // read pixels 7,8
      V2 = *VectIn; VectIn++;
      V3 = __builtin_shuffle(V3, V4, Mask);
      // read pixel 9
      V4 = *VectIn;
      /* Goes to next line of In, ready to parse 3 fresh pixels */
      VectIn += ((IMG_WIDTH>>1)-1);
      /* The convolution itself, first a dot product producing an accumulator that is used in subsequent
	 sum of products */
      S = Dotp(V0, C0);
      S = SumDotp(V1, C1, S);
      S = SumDotp(V2, C2, S);
      S = SumDotp(V3, C3, S);
      S = SumDotp(V4, C4, S);
      // assign result to O and adjust pointer
      *O = S;
      O+=IMG_WIDTH;
    }
  }

  // set borders to inputs
  for (unsigned k = 0; k < IMG_WIDTH; k++) {
    Out[k]                              = In[k];
    Out[(IMG_HEIGHT-1) * IMG_WIDTH + k] = In[(IMG_HEIGHT-1) * IMG_WIDTH + k];
  }

  for (unsigned k = 0; k < IMG_HEIGHT; k++) {
    Out[k*IMG_WIDTH]                   = In[k*IMG_WIDTH];
    Out[k*IMG_WIDTH + IMG_WIDTH - 1]   = In[k*IMG_WIDTH + IMG_WIDTH - 1];
  }
#endif
}
