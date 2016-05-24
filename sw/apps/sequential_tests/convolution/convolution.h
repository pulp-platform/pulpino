#ifndef _CONF_HEADER_
#define _CONF_HEADER_

typedef signed char     Filtc;
typedef signed char     FiltcV __attribute__((vector_size (4)));

typedef unsigned char   Pixel;
typedef unsigned char   PixelV __attribute__((vector_size (4)));

#include "data_image.h" //generate by matlab

void __attribute__ ((noinline))  Conv3x3_Byte_Scalar    (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv3x3_Byte_Vector    (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv5x5_Byte_Scalar    (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);
void __attribute__ ((noinline))  Conv5x5_Byte_Vector    (Pixel * In, Pixel * Out, int R, int C, Filtc  * Kernel);

void __attribute__ ((noinline))  perf_enable_id         (int eventid);

void __attribute__ ((noinline))  InitData               (Pixel * __restrict__ Img,    int size);
void __attribute__ ((noinline))  InitZero               (Pixel * __restrict__ Img,    int size);
void __attribute__ ((noinline))  InitKernel             (Filtc * __restrict__ Kernel, int size);

int  __attribute__ ((noinline))  checkresult            (Pixel * __restrict__ Out, Pixel * __restrict__ OutGold, int N);

/*  Profiling */
/* #define SPR_PCER_CYCLES      0 */
/* #define SPR_PCER_INSTR       1 */
/* #define SPR_PCER_LD_STALL    2 */
/* #define SPR_PCER_JMP_STALL   3 */
/* #define SPR_PCER_IMISS       4 */
/* #define SPR_PCER_WBRANCH     5 */
/* #define SPR_PCER_WBRANCH_CYC 6 */
/* #define SPR_PCER_LD          7 */
/* #define SPR_PCER_ST          8 */
/* #define SPR_PCER_JUMP        9 */
/* #define SPR_PCER_BRANCH      10 */
/* #define SPR_PCER_DELAY_NOP   11 */
/* #define SPR_PCER_LD_EXT      12 */
/* #define SPR_PCER_ST_EXT      13 */
/* #define SPR_PCER_LD_EXT_CYC  14 */
/* #define SPR_PCER_ST_EXT_CYC  15 */
/* #define SPR_PCER_TCDM_CONT   16 */

// define an event you want to profile
#define EVENT_ID 1


#endif