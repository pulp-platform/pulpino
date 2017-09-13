#include "mlShared.h"

union float_int {
  const uint32_t i;
  const float    f;
};




#define F_PZERO 0x00000000
#define F_MZERO 0x80000000
#define F_QNAN  0x7FC00000
#define F_SNAN  0x7FA00000
#define F_PINF  0x7F800000
#define F_MINF  0xFF800000

__attribute__ ((section(".heapsram"))) float g_in_a[] = {
  792.2073F,
  959.4924F,
  655.7407F,
   35.7117F,
  849.1293F,
  933.9932F,
  678.7352F,
  757.7401F,
  743.1325F,
  392.2270F
};

__attribute__ ((section(".heapsram"))) float g_in_b[] = {
  655.4779F,
  171.1867F,
  706.0461F,
   31.8328F,
  276.9230F,
   46.1714F,
   97.1318F,
  823.4578F,
  694.8286F,
  317.0995F
};
////////////////////////////////////////////////////////////////////
//         The added testcases                                    //
//         Begin                                                 //
///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//         ADD, SUB, Mult, DIV                                    //
///////////////////////////////////////////////////////////////////
__attribute__ ((section(".heapsram"))) int g_in_a_ass[] = {
// For test special cases index 0-19
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_SNAN,
  F_SNAN,
  F_SNAN,
  F_SNAN
};

__attribute__ ((section(".heapsram"))) int g_in_b_ass[] = {
// For test special cases index 0-19
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN
};

////////////////////////////////////////////////////////////////////
//         Output for ADD                                        //
///////////////////////////////////////////////////////////////////
__attribute__ ((section(".heapsram"))) int g_output_as[] = {
// For test special cases index 0-19
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF, 
  F_QNAN,
  0x007fffff,   //1.17549e-38 
  0x00fffffe,   //2.35099e-38 
  F_PINF,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};
////////////////////////////////////////////////////////////////////
//         Output for SUB                                        //
///////////////////////////////////////////////////////////////////
__attribute__ ((section(".heapsram"))) int g_output_ss[] = {
// For test special cases index 0-19
  F_PZERO,
  0x807fffff,   //-1.17549e-38
  F_MINF,
  F_QNAN,
  0x007fffff,   //1.17549e-38
  F_PZERO,
  F_MINF,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


////////////////////////////////////////////////////////////////////
//         Output for MUL                                        //
///////////////////////////////////////////////////////////////////
__attribute__ ((section(".heapsram"))) int g_output_ms[] = {
// For test special cases index 0-19
  F_PZERO,
  F_PZERO,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};

////////////////////////////////////////////////////////////////////
//         Output for DIV                                        //
///////////////////////////////////////////////////////////////////
__attribute__ ((section(".heapsram"))) int g_output_ds[] = {
// For test special cases index 0-19
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_QNAN,
  F_PINF,
  0x3f800000,
  F_PZERO,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};




////////////////////////////////////////////////////////////////////
//         SQRT                                                   //
///////////////////////////////////////////////////////////////////

__attribute__ ((section(".heapsram"))) int g_sqrt_a_ss[] = {
// For test SQRT special cases index 0-5
  F_QNAN,
  F_SNAN,
  F_PINF,
  F_PZERO,
  F_MZERO,
  0xd0600000  //-1.50324e+10 
};




__attribute__ ((section(".heapsram"))) int g_sqrt_output_ss[] = {
// For test SQRT special cases index 0-5
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PZERO,
  F_MZERO,
  F_QNAN
};

////////////////////////////////////////////////////////////////////
//         FMAC                                                   //
///////////////////////////////////////////////////////////////////

__attribute__ ((section(".heapsram"))) int g_fma_init_s[] = {
  // For test special cases index 0-63
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,

};

__attribute__ ((section(".heapsram"))) int g_fma_a_s[] = {
  // For test special cases index 0-63
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  F_PZERO,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_PINF,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


__attribute__ ((section(".heapsram"))) int g_fma_b_s[] = {
  // For test special cases index 0-63
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN,
  F_PZERO,
  0x007fffff,   //1.17549e-38 
  F_PINF,
  F_QNAN
};


__attribute__ ((section(".heapsram"))) int g_fma_output_sma[] = {
  // For test special cases index 0-63
  F_PZERO,
  F_PZERO,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  0x007fffff,   //1.17549e-38 
  0x007fffff,   //1.17549e-38 
  F_QNAN,
  F_QNAN,
  0x007fffff,   //1.17549e-38 
  0x007fffff,   //1.17549e-38    
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


__attribute__ ((section(".heapsram"))) int g_fma_output_snma[] = {
  // For test special cases index 0-63
  F_MZERO,
  F_MZERO,
  F_QNAN,
  F_QNAN,
  F_MZERO,
  F_MZERO,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  0x807fffff,   //1.17549e-38
  0x807fffff,   //1.17549e-38
  F_QNAN,
  F_QNAN,
  0x807fffff,   //1.17549e-38
  0x807fffff,   //1.17549e-38
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


__attribute__ ((section(".heapsram"))) int g_fma_output_sms[] = {
  // For test special cases index 0-63
  F_MZERO,
  F_MZERO,
  F_QNAN,
  F_QNAN,
  F_MZERO,
  F_MZERO,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  0x807fffff,   //1.17549e-38
  0x807fffff,   //1.17549e-38
  F_QNAN,
  F_QNAN,
  0x807fffff,   //1.17549e-38
  0x807fffff,   //1.17549e-38
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_QNAN, 
  F_QNAN, 
  F_QNAN, 
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


__attribute__ ((section(".heapsram"))) int g_fma_output_snms[] = {
  // For test special cases index 0-63
  F_PZERO,
  F_PZERO,
  F_QNAN,
  F_QNAN,
  F_PZERO,
  F_PZERO,
  F_MINF,
  F_QNAN,
  F_QNAN, 
  F_MINF, 
  F_MINF, 
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_QNAN,
  F_QNAN,
  0x007fffff,   //1.17549e-38
  0x007fffff,   //1.17549e-38
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_MINF,
  F_MINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_PINF,
  F_PINF,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN,
  F_QNAN
};


////////////////////////////////////////////////////////////////////
//         The added testcases                                    //
//         End                                                   //
///////////////////////////////////////////////////////////////////


__attribute__ ((section(".heapsram"))) int g_inbr_a[] = {
  0x123123,       //1.67067e-39
  F_QNAN,
  0xABBAABBA,     //-1.32638e-12
  0x4423ef68,     //655.741
  0x420ed810,     //1.20994e-19
  F_SNAN,
  F_PINF,
  0x420bff10,    //34.9991
  0x520b7833,    //1.49754e+11
  0x4c0b8722     //3.65764e+07
};

__attribute__ ((section(".heapsram"))) int g_inbr_b[] = {
  F_QNAN,
  F_QNAN,
  0xABBAABBA,     //-1.32638e-12
  0xABBAABBA,     //-1.32638e-12
  0xc20ed810,     //-35.711
  F_SNAN,
  F_PINF,
  0x420eff10,     //35.7491
  0x520e7833,     //1.52975e+11
  F_PINF
};

__attribute__ ((section(".heapsram"))) int g_feq[] = {
  0,
  0,
  1,
  0,
  0,
  0,
  1,
  0,
  0,
  0
};

__attribute__ ((section(".heapsram"))) int g_fle[] = {
  0,
  0,
  1,
  0,
  0,
  0,
  1,
  1,
  1,
  1
};

__attribute__ ((section(".heapsram"))) int g_flt[] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1
};


float g_add_min[] = {
    1446.7F,
    1129.7F,
    1360.8F,
    0066.5F,
    1125.1F,
    0979.2F,
    0774.9F,
    1580.2F,
    1437.0F,
    0708.3F 
};
float g_add_max[] = {
    1448.7F,
    1131.7F,
    1362.8F,
    0068.5F,
    1127.1F,
    0981.2F,
    0776.9F,
    1582.2F,
    1439.0F,
    0710.3F
};


float g_sub_min[] = {
  135.7294F,
  787.3057F,
  -51.3054F,
    2.8788F,
  571.2063F,
  886.8219F,
  580.6034F,
  -66.7177F,
   47.3038F,
   74.1275F
};

float g_sub_max[] = {
  137.7294F,
  789.3057F,
  -49.3054F,
    4.8788F,
  573.2063F,
  888.8219F,
  582.6034F,
  -64.7177F,
   49.3038F,
   76.1275F
};

float g_mul_min[] = {
   519273.3889628930F,
   164251.3304544798F,
   462982.1553947512F,
   001135.8043779320F,
   235142.4219989503F,
   043122.7670890740F,
   065925.7545787244F,
   623966.0423624737F,
   516348.7095158557F,
   124373.9839601057F
};

float g_mul_max[] = {
   519275.3889628930F,
   164253.3304544798F,
   462984.1553947513F,
   001137.8043779320F,
   235144.4219989503F,
   043124.7670890740F,
   065927.7545787244F,
   623968.0423624737F,
   516350.7095158558F,
   124375.9839601057F
};


float g_div_min[] = {
   0.208595044060083F,
   4.604947666544547F,
  -0.071249440676208F,
   0.121849995780464F,
   2.066301289467540F,
  19.228830775725925F,
   5.987776258418691F,
  -0.079806998595730F,
   0.069519077873077F,
   0.236921042755694F
};

float g_div_max[] = {
   2.208595044060083F,
   6.604947666544547F,
   1.928750559323792F,
   2.121849995780464F,
   4.066301289467541F,
  21.228830775725925F,
   7.987776258418691F,
   1.920193001404270F,
   2.069519077873077F,
   2.236921042755695F
};

__attribute__ ((section(".heapsram"))) float g_fma_init[] = {
   950.2220488383550F,
   034.4460805029088F,
   438.7443596563982F,
   381.5584570930084F,
   765.5167881490025F,
   795.1999011370632F,
   186.8726045543786F,
   489.7643957882311F,
   445.5862007108995F,
   646.3130101112646F
};


float g_fma_min[] = {
   520223.6110117314F,
   164285.7765349827F,
   463420.8997544077F,
   001517.3628350250F,
   235907.9387870993F,
   043917.9669902111F,
   066112.6271832787F,
   624455.8067582619F,
   516794.2957165667F,
   125020.2969702170F
};

float g_fma_max[] = {
   520225.6110117314F,
   164287.7765349827F,
   463422.8997544077F,
   001519.3628350250F,
   235909.9387870993F,
   043919.9669902111F,
   066114.6271832787F,
   624457.8067582618F,
   516796.2957165667F,
   125022.2969702170F
};


float g_fms_min[] = {
  -518325.1669140547F,
  -164218.8843739769F,
  -462545.4110350948F,
  -000756.2459208390F,
  -234378.9052108013F,
  -042329.5671879369F,
  -065740.8819741700F,
  -623478.2779666855F,
  -515905.1233151448F,
  -123729.6709499944F
};

float g_fms_max[] = {
  -518323.1669140548F,
  -164216.8843739769F,
  -462543.4110350949F,
  -000754.2459208390F,
  -234376.9052108013F,
  -042327.5671879369F,
  -065738.8819741700F,
  -623476.2779666855F,
  -515903.1233151449F,
  -123727.6709499944F
};


__attribute__ ((section(".heapsram"))) float g_trig_in[] = {
   0.709364830858073F,
   0.754686681982361F,
   0.276025076998578F,
   0.679702676853675F,
   0.655098003973841F,
   0.162611735194631F,
   0.118997681558377F,
   0.498364051982143F,
   0.959743958516081F,
   0.340385726666133F
};


float g_sin_min[] = {
   0.551351951504031F,
   0.585060454428205F,
   0.172533354047857F,
   0.528561805861656F,
   0.509236962252271F,
   0.061896036556847F,
   0.018717036916856F,
   0.377989218242609F,
   0.719044696542342F,
   0.233850712936964F
};

float g_sin_max[] = {
   0.751351951504031F,
   0.785060454428205F,
   0.372533354047857F,
   0.728561805861656F,
   0.709236962252271F,
   0.261896036556847F,
   0.218717036916856F,
   0.577989218242609F,
   0.919044696542342F,
   0.433850712936964F
};

float g_cos_min[] = {
   0.658775747682997F,
   0.628486220719803F,
   0.862146335507975F,
   0.677759639099339F,
   0.692988224266682F,
   0.886807819865238F,
   0.892928126878115F,
   0.778365702451900F,
   0.473729714295733F,
   0.842625960533382F
};

float g_cos_max[] = {
   0.858775747682997F,
   0.828486220719803F,
   1.062146335507975F,
   0.877759639099339F,
   0.892988224266682F,
   1.086807819865238F,
   1.092928126878115F,
   0.978365702451900F,
   0.673729714295733F,
   1.042625960533382F
};

float g_atan_min[] = {
  0.7796F,
  1.2942F,
  0.6485F,
  0.7428F,
  1.1555F,
  1.4214F,
  1.3287F,
  0.6439F,
  0.7190F,
  0.7909F
};

float g_atan_max[] = {
  0.9796F,
  1.4942F,
  0.8485F,
  0.9428F,
  1.3555F,
  1.6214F,
  1.5287F,
  0.8439F,
  0.9190F,
  0.9909F
};


float g_exp_min[] = {
   241.5390598689143F,
   772.3213662753867F,
   093.1837977129578F,
   000.2808635426626F,
   358.8602792562511F,
   647.0370453393218F,
   109.4578046174749F,
   189.9963563331674F,
   171.6043102182665F,
   014.1607602488125F
};

float g_exp_max[] = {
   243.5390598689143F,
   774.3213662753867F,
   095.1837977129578F,
   002.2808635426626F,
   360.8602792562511F,
   649.0370453393218F,
   111.4578046174749F,
   191.9963563331675F,
   173.6043102182665F,
   016.1607602488125F
};

float g_mexp_min[] = {
    0.4775F,
    0.4142F,
    0.5347F,
    0.8756F,
    0.4551F,
    0.4234F,
    0.5247F,
    0.4914F,
    0.4974F,
    0.6620F
};

float g_mexp_max[] = {
    0.6775F,
    0.6142F,
    0.7347F,
    1.0756F,
    0.6551F,
    0.6234F,
    0.7247F,
    0.6914F,
    0.6974F,
    0.8620F
    };


float g_log_min[] = {
   9.529734238997950F,
   9.806127608547103F,
   9.256981629625521F,
   5.058324042133790F,
   9.629840454661442F,
   9.767268309878924F,
   9.306704928736242F,
   9.465559346077075F,
   9.437475593099149F,
   8.515545112365650F
};

float g_log_max[] = {
   9.729734238997949F,
  10.006127608547102F,
   9.456981629625520F,
   5.258324042133789F,
   9.829840454661442F,
   9.967268309878923F,
   9.506704928736241F,
   9.665559346077075F,
   9.637475593099149F,
   8.715545112365650F
};

float g_pow_min[] = {
   627591.4530078805F,
   920624.7163053405F,
   429994.8645303694F,
   001274.3239865862F,
   721019.5780851912F,
   872342.3868566972F,
   460680.4104398059F,
   574169.1054888699F,
   552244.8651814296F,
   153841.0348526567F
};

float g_pow_max[] = {
   627593.4530078805F,
   920626.7163053404F,
   429996.8645303694F,
   001276.3239865862F,
   721021.5780851912F,
   872344.3868566971F,
   460682.4104398059F,
   574171.1054888699F,
   552246.8651814296F,
   153843.0348526567F
};

float g_sqrt_min[] = {
  28.046177885452838F,
  30.875674752826658F,
  25.507434450889196F,
   5.875924913700771F,
  29.039823367151303F,
  30.461303109611514F,
  25.952546034078384F,
  27.427079950084305F,
  27.160456124667395F,
  19.704722152410220F
};

float g_sqrt_max[] = {
  28.246177885452841F,
  31.075674752826661F,
  25.707434450889199F,
   6.075924913700770F,
  29.239823367151306F,
  30.661303109611517F,
  26.152546034078387F,
  27.627079950084308F,
  27.360456124667397F,
  19.904722152410223F
};

float g_mex_min[] = {
   241.7787544470711F,
   004.6815779381627F,
   132.8582555172626F,
   000.0120960829352F,
   011.0253241298157F,
   000.5779116904330F,
   001.0082828032969F,
   733.6847311402638F,
   234.3838127467586F,
   002.7264751650322F
};

float g_mex_max[] = {
   243.7787544470711F,
   006.6815779381627F,
   134.8582555172626F,
   002.0120960829352F,
   013.0253241298157F,
   002.5779116904330F,
   003.0082828032969F,
   735.6847311402638F,
   236.3838127467586F,
   004.7264751650322F
};


float g_vect_add_max[] = {
  1449.135277857514F,
  1131.900360398583F,
  1362.565034256958F,
  0068.528528927304F,
  1127.312572540585F,
  0981.249744150237F,
  0776.887829093486F,
  1582.434080850508F,
  1439.359183389252F,
  0710.482965656847F 
};

float g_mul_small_min[] = {
   8109.662327545204F,
   2562.442663351247F,
   7230.111803042989F,
   0013.762568405188F,
   3670.115968733598F,
   0669.808860766781F,
   1026.105540292568F,
   9745.485036913651F,
   8063.964211185246F,
   1939.359124376652F 
};

float g_mul_small_max[] = {
   8117.662327545204F,
   2570.442663351247F,
   7238.111803042989F,
   0021.762568405188F,
   3678.115968733598F,
   0677.808860766781F,
   1034.105540292568F,
   9753.485036913650F,
   8071.964211185247F,
   1947.359124376652F
};


float g_pow_small_min[] = {
   09801.13207824813F,
   14379.77681727094F,
   06713.68538328702F,
  -00014.92693729041F,
   11260.44653258111F,
   13625.36541963589F,
   07193.14703812197F,
   08966.40789826359F,
   08623.84164345984F,
   02398.78179457276F
};

float g_pow_small_max[] = {
   09811.13207824813F,
   14389.77681727094F,
   06723.68538328702F,
   00024.92693729041F,
   11270.94653258111F,
   13635.36541963589F,
   07203.14703812197F,
   08976.40789826359F,
   08633.84164345984F,
   02408.78179457276F
};