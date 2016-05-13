#ifndef _CONF_HEADER_
#define _CONF_HEADER_

#define LINE 32
#define COL  32

typedef signed short  Filts;
typedef signed char   Filtc;
typedef signed short  FiltsV __attribute__((vector_size (4)));
typedef signed char   FiltcV __attribute__((vector_size (4)));

void __attribute__ ((noinline))  Conv5x5_Byte           (Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, FiltcV * __restrict__ Coeff);
void __attribute__ ((noinline))  Conv3x3_Byte           (Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, FiltcV * __restrict__ Coeff);
void __attribute__ ((noinline))  Conv5x5_Byte_Scalar    (Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, Filtc  * __restrict__ Coeff);
void __attribute__ ((noinline))  Conv3x3_Byte_Scalar    (Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M, Filtc  * __restrict__ Coeff);

void __attribute__ ((noinline))  perf_enable_id         (int eventid);
void __attribute__ ((noinline))  InitData               (Filtc * __restrict__ In, Filtc * __restrict__ Out, int N, int M);
int  __attribute__ ((noinline))  checkRes               (int is5x5, Filtc *Out, int N, int M);

void  __attribute__ ((noinline)) InitKernelVect   (FiltcV * __restrict__ Out, int size);
void  __attribute__ ((noinline)) InitKernelVect2  (FiltcV * __restrict__ Out, int size);
void  __attribute__ ((noinline)) InitKernelScalar (Filtc *  __restrict__ Out, int size);


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



// kernels
static Filtc __attribute__ ((section(".heapsram"))) Out[LINE*COL*4];

/*
	The kernel we want to use:

	1 2 3 2 1
	2 3 4 3 2
	3 4 5 4 3
	2 3 4 3 2
	1 2 3 2 1
*/
static FiltcV Kernel5x5_init[7] =
  {
	{1, 2, 3, 2},
	{1, 2, 3, 4},
	{3, 2, 3, 4},
	{5, 4, 3, 2},
	{3, 4, 3, 2},
	{1, 2, 3, 2},
	{1, 0, 0, 0},
	/* Last 3 elmts are 0 since they are outside the Kernel */
  };
static FiltcV Kernel5x5_init2[7] =
  {
	{1, 2, 3, 2},
	{2, 3, 4, 3},
	{3, 4, 5, 4},
	{2, 3, 4, 3},
	{1, 2, 3, 2},
	{1, 2, 3, 2},
	{1, 0, 0, 0},
	/* Last 3 elmts are 0 since they are outside the Kernel */
  };

static Filtc Kernel5x5_Scalar_init[25] =
  {
	1, 2, 3, 2, 1,
	2, 3, 4, 3, 2,
	3, 4, 5, 4, 3,
	2, 3, 4, 3, 2,
	1, 2, 3, 2, 1,
  };

/*
	10 20 10
	20 30 20
	10 20 10
*/

static FiltcV Kernel3x3_init[3] =
  {
	{10, 20, 10, 20},
	{30, 20, 10, 20},
	{10,  0,  0,  0},
  };

static FiltcV Kernel3x3_init2[3] =
  {
	{10, 20, 10, 0},
	{20, 30, 20, 0},
	{10, 20, 10, 0},
  };

static Filtc Kernel3x3_Scalar_init[9] =
  {
	10, 20, 10,
	20, 30, 20,
	10, 20, 10,
  };



 #endif

