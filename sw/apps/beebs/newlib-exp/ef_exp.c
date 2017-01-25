/* BEEBS newlib ef_exp implementation

   ====================================================
   Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.

   Developed at SunPro, a Sun Microsystems, Inc. business.
   Permission to use, copy, modify, and distribute this
   software is freely granted, provided that this notice
   is preserved.
   ====================================================

   Copyright (C) 2014 Embecosm Limited and University of Bristol

   Contributor Pierre Langlois <pierre.langlois@embecosm.com>

   This file is part of the Bristol/Embecosm Embedded Benchmark Suite.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include "support.h"
#include <stdint.h>

#define FLT_UWORD_IS_NAN(x) ((x)>0x7f800000L)
#define FLT_UWORD_IS_INFINITE(x) ((x)==0x7f800000L)
#define FLT_UWORD_LOG_MAX 0x42b17217
#define FLT_UWORD_LOG_MIN 0x42cff1b5

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;

static const float
one	= 1.0,
halF[2]	= {0.5,-0.5,},
huge	= 1.0e+30,
twom100 = 7.8886090522e-31,      /* 2**-100=0x0d800000 */
ln2HI[2]   ={ 6.9313812256e-01,		/* 0x3f317180 */
	     -6.9313812256e-01,},	/* 0xbf317180 */
ln2LO[2]   ={ 9.0580006145e-06,  	/* 0x3717f7d1 */
	     -9.0580006145e-06,},	/* 0xb717f7d1 */
invln2 =  1.4426950216e+00, 		/* 0x3fb8aa3b */
P1   =  1.6666667163e-01, /* 0x3e2aaaab */
P2   = -2.7777778450e-03, /* 0xbb360b61 */
P3   =  6.6137559770e-05, /* 0x388ab355 */
P4   = -1.6533901999e-06, /* 0xb5ddea0e */
P5   =  4.1381369442e-08; /* 0x3331bb4c */

	float __ieee754_expf(float x)	/* default IEEE double exp */
{
	float y,hi,lo,c,t;
	int32_t k = 0,xsb,sx;
	uint32_t hx;

	GET_FLOAT_WORD(sx,x);
	xsb = (sx>>31)&1;		/* sign bit of x */
	hx = sx & 0x7fffffff;		/* high word of |x| */

    /* filter out non-finite argument */
        if(FLT_UWORD_IS_NAN(hx))
            return x+x;	 	/* NaN */
        if(FLT_UWORD_IS_INFINITE(hx))
	    return (xsb==0)? x:0.0;		/* exp(+-inf)={inf,0} */
	if(sx > FLT_UWORD_LOG_MAX)
	    return huge*huge; /* overflow */
	if(sx < 0 && hx > FLT_UWORD_LOG_MIN)
	    return twom100*twom100; /* underflow */

    /* argument reduction */
	if(hx > 0x3eb17218) {		/* if  |x| > 0.5 ln2 */
	    if(hx < 0x3F851592) {	/* and |x| < 1.5 ln2 */
		hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = 1-xsb-xsb;
	    } else {
		k  = invln2*x+halF[xsb];
		t  = k;
		hi = x - t*ln2HI[0];	/* t*ln2HI is exact here */
		lo = t*ln2LO[0];
	    }
	    x  = hi - lo;
	}
	else if(hx < 0x31800000)  {	/* when |x|<2**-28 */
	    if(huge+x>one) return one+x;/* trigger inexact */
	}

    /* x is now in primary range */
	t  = x*x;
	c  = x - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	if(k==0) 	return one-((x*c)/(c-(float)2.0)-x);
	else 		y = one-((lo-(x*c)/((float)2.0-c))-hi);
	if(k >= -125) {
	    uint32_t hy;
	    GET_FLOAT_WORD(hy,y);
	    SET_FLOAT_WORD(y,hy+(k<<23));	/* add k to y's exponent */
	    return y;
	} else {
	    uint32_t hy;
	    GET_FLOAT_WORD(hy,y);
	    SET_FLOAT_WORD(y,hy+((k+100)<<23));	/* add k to y's exponent */
	    return y*twom100;
	}
}

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

/* Tell the compiler not to optimize out calls in BENCHMARK. */
volatile float result = 0;

int
benchmark (void)
{
  result = __ieee754_expf(1);
  result = __ieee754_expf(2);
  result = __ieee754_expf(3);
  result = __ieee754_expf(4);
  result = __ieee754_expf(5);
  return 0;
}


