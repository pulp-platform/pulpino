// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

/*----------------------------------------------------------------------------
| special FP math functions for ETHZ openrisc cores with FPU or LNU.
| Michael Gautschi, Michael Schaffner 2015
*----------------------------------------------------------------------------*/

#include "math_fns.h"

#include "stdio.h"


/*----------------------------------------------------------------------------
| FP function implementations. 
*----------------------------------------------------------------------------*/

  // uses 3 iterations newton raphson 
  // initial estimate is generated using
  // a fast linear approximation
  // exact to around 6 digits
  // TODO: special case handling!
#if (HWDIV==0)
  float fDiv(float a, float b)
  {
    //    return a/b;

    int ex, tmp;
    float x;

    // propagate nan, or set to nan
    if (fIsNan(a) || fIsNan(b) || b == 0.0F)
    {
      tmp = MATH_NAN;
      return *((float*)&tmp);
    }

    // calculate output sign
    tmp = ((*((int*)&a)) ^ (*((int*)&b))) & MATH_SGN_MASK;
    
    // get exponent of b and correct exponent of a
    // (with FP bias!)
    ex  = (((*((int*)&a))>>23) & 0xFF)  -
          (((*((int*)&b))>>23) & 0xFF)  + 127;

    if(ex < 1)
    {
      //return +/- 0.0
      return *((float*)&tmp);
    }
    else if(ex > 254)
    {
      // return +/- infty
      tmp |= MATH_INF;
      return *((float*)&tmp);
    }
    
    // reassemble using adjusted exponent and new sign
    tmp |= ((0xFF & ex) << 23) | (MATH_MNT_MASK & (*((int*)&a)));
    a   = *((float*)&tmp);

    // get normalized abs value
    tmp = (0x3F800000 | (MATH_MNT_MASK & (*((int*)&b))));
    b   = *((float*)&tmp);
    
    // calculate initial estimate using linear approximation
    // note: b is [1 ... 2) which maps to (0.5 ... 1)
    x =    1.5F - 0.5F*b;
    x = x*(2.0F - b*x);
    x = x*(2.0F - b*x);
    x = x*(2.0F - b*x);
    
    return a*x;

  }
#endif

#if (HWSQRT==0)
  float fSqrt(float x)
  {

    // uses 3 iterations newton raphson 
    // initial estimate is generated using
    // a fast inverse square root 
    unsigned int i;
    float ahalf, a;
    
    if(x < 0.0F || fIsNan(x))
    {
      // return nan
      i = MATH_NAN;
      return *((float*)&i);
    }
    else if (x == 0.0F)
    {
      // return zero
      return 0.0F;
    }

    a     = fAbs(x);
    ahalf = 0.5F*a;

    i = *(unsigned int*)&a;
    i = 0x5f3759df - (i>>1);
    a = *(float*) &i;
    // 3 newton raphson iterations
    a = a*(1.5F - ahalf*a*a);
    a = a*(1.5F - ahalf*a*a);
    a = a*(1.5F - ahalf*a*a);

    return x*a;
  }
#endif
  



  // this is the actual implementation of 2^x
  // uses a polynomial approximation
  // which is precise up to about 6.5 digits over the range [0 1]
  // from "Computer Approximations", Hart and Cheney et. al
  // ISBN 0-88275-642-7

  static float fExp2_65(float x)
  {  
    const float c1 = 0.9999999250635449F;
    const float c2 = 0.6931530732007278F;
    const float c3 = 0.240153617040129F; 
    const float c4 = 0.0558263180623292F;
    const float c5 = 0.0089893400833312F;
    const float c6 = 0.0018775766770276F;

    return (c1 + x*(c2 + x*(c3 + x*(c4 + x*(c5 + x * c6)))));
  }  



  // wrapper function which performs a range reduction first 
  // and then it calls the actual implementation
  // precise to about 6 digits
  float fExp2(float x)
  {
    int tmp;
    
    // range check
    if(x > 127.0F)
    {
      // return infinity
      tmp = MATH_INF;
      return *((float*)&tmp);
    }
    else if (x < -100.0F)
    {
      // return zero
      return 0.0F;
    }
    else if (x == 0.0F)
    {
      // return one
      return 1.0F;
    }

    // perform a range reduction first
    tmp = (int)x;
    
    if (x < 0.0F)
    {
      tmp--;
    }
    
    x  -= (float)tmp;
    x   = fExp2_65(x);

    // adjust the exponent of the floating point format
    tmp  = (*((int*) &x)) + (tmp << 23);
    return (*((float*) &tmp));
  }


  // this is the actual implementation of log2(x)
  // uses a polynomial approximation
  // which is precise up to about 6.5 digits over the range [0.5,1]
  // from "Computer Approximations", Hart and Cheney et. al
  // ISBN 0-88275-642-7
  static float fLog2_65(float x)
  {  
    const float c1 = - 4.2455636761F;
    const float c2 =  14.2681945918F;
    const float c3 = -29.9775899371F;
    const float c4 =  46.2539752349F;
    const float c5 = -47.7637022658F;
    const float c6 =  31.3017639486F;
    const float c7 = -11.7733536664F;
    const float c8 =   1.9362760474F;

    return (c1 + x*(c2 + x*(c3 + x*(c4 + x*(c5 + x* (c6 + x*(c7 + x*c8)))))));

  }  


  // wrapper function which performs a range reduction first 
  // and then it calls the actual implementation
  // precise to about 6 digits
  float fLog2(float x)
  {
    int tmp1, tmp2;

    // range check
    if(fIsInf(x))
    {
      // return infinity
      tmp1 = MATH_INF;
      return *((float*)&tmp1);
    }
    else if (x == 0.0F)
    {
      // return -inf
      tmp1 = (MATH_INF | MATH_SGN_MASK);
      return *((float*)&tmp1);    
    }
    else if (x <= 0.0F || fIsNan(x))
    {
      // return nan
      tmp1 = MATH_NAN;
      return *((float*)&tmp1);
    }

    // perform a range reduction first -> [0.5,1]
    tmp1 = *((int*)&x);
    tmp2 = (tmp1>>23) - 126;
    // set exponent to -1
    tmp1 = (tmp1 & MATH_MNT_MASK) | 0x3F000000;
    
    return (fLog2_65((*((float*) &tmp1))) + (float)tmp2);
}


/*----------------------------------------------------------------------------
| shared function implementations. 
*----------------------------------------------------------------------------*/

// this is the actual implementation of atan(x)
// uses a polynomial approximation (nr. 4991)
// which is precise up to about 7 digits over the range [0,1]
// from "Computer Approximations", Hart and Cheney et. al
// ISBN 0-88275-642-7
static float fatan_70(float x)
{
  static const float p[7] = { 0.0242524017F, 
                             -0.0594863929F,
                              0.0991429314F, 
                             -0.140194803F, 
                              0.199697241F, 
                             -0.333319902F, 
                              0.999999881F };

  int k;    
  float v = -0.00469327578F;
  float x2 = x * x;
  
  for (k = 0; k < 7; k++) {
    v = x2 * v + p[k];
  }
  return x * v;

}


float fAtan2(float y, float x)
{
  float v;
  float a;
  int tst = ((int)(fAbs(x) < fAbs(y)) + ((int)(x < 0.0F) << 1)) + ((int)(y < 0.0F) << 2);
  
  switch (tst) {
   case 0:
    /*  octant 1 */
    v = fDiv(y,x);
    a = 0.0F;
    break;

   case 1:
    /*  octant 2 */
    v = -fDiv(x,y);
    a = MATH_0_5PI;
    break;

   case 2:
    /*  octant 4 */
    v = fDiv(y,x);
    a = MATH_PI;
    break;

   case 3:
    /*  octant 3 */
    v = -fDiv(x,y);
    a = MATH_0_5PI;
    break;

   case 4:
    /*  octant 8 */
    v = fDiv(y,x);
    a = 0.0F;
    break;

   case 5:
    /*  octant 7 */
    v = -fDiv(x,y);
    a = -MATH_0_5PI;
    break;

   case 6:
    /*  octant 5 */
    v = fDiv(y,x);
    a = -MATH_PI;
    break;

   case 7:
    /*  octant 6 */
    v = -fDiv(x,y);
    a = -MATH_0_5PI;
    break;

   default:
    v = 0.0F;
    a = 0.0F;
    break;
  }

  return a + fatan_70(v);
}



// legacy, use the more precise fCos_73 for single precision!
// this is the actual implementation
// uses a polynomial approximation
// which is precise up to about 5.2 digits
// from "Computer Approximations", Hart and Cheney et. al
// ISBN 0-88275-642-7
// static float fCos_52(float x)
// {
//   const float c1= 0.9999932946F;
//   const float c2=-0.4999124376F;
//   const float c3= 0.0414877472F;
//   const float c4=-0.00127120948F;
//   float x2;
//   // The input argument squared
//   x2 = x*x;
//   return (c1 + x2*(c2 + x2*(c3 + c4*x2)));
// }

// this is the actual implementation
// uses a polynomial approximation
// which is precise up to about 7.33 digits
// from "Computer Approximations", Hart and Cheney et. al
// ISBN 0-88275-642-7$
// note: when evaluated with floats, the precision will saturate around 6.7 digits
static float fCos_73(float x)
{
  const float c1= 0.999999953464F;
  const float c2=-0.499999053455F;
  const float c3= 0.0416635846769F;
  const float c4=-0.0013853704264F;
  const float c5= 0.00002315393167F;
  float x2;
  // The input argument squared
  x2 = x*x;
  return (c1 + x2*(c2 + x2*(c3 + x2*(c4 + c5*x2))));
}



// wrapper function which determines 
// the quadrant and calls the actual implementation
float fCos(float x)
{
  
  // nan and inf result in nan
  if (fIsNan(x) || fIsInf(x))
  {
    int tmp = MATH_NAN;
    return (*((float*) &tmp));
  }  
    
  // mod 2pi
  // only divide if necessary
  if(fAbs(x) > MATH_2PI)
  {
    x -= ((float)((int)fDiv(x, MATH_2PI))) * MATH_2PI;
  }

  // cos(-x) = cos(x)
  x = fAbs(x); 

  if(x < MATH_0_5PI)
  {
    return fCos_73(x);
  }
  else if (x < MATH_PI)
  {
    return -fCos_73(MATH_PI-x);
  }
  else if (x < MATH_1_5PI)
  {
    return -fCos_73(x-MATH_PI);
  }
  else
  {
    return fCos_73(MATH_2PI-x);
  }

}
