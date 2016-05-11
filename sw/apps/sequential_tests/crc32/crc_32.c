/* Crc - 32 BIT ANSI X3.66 CRC checksum files */

#include "crc.h"


/**********************************************************************\
  |* Demonstration program to compute the 32-bit CRC used as the frame  *|
  |* check sequence in ADCCP (ANSI X3.66, also known as FIPS PUB 71     *|
  |* and FED-STD-1003, the U.S. versions of CCITT's X.25 link-level     *|
  |* protocol).  The 32-bit FCS was added via the Federal Register,     *|
  |* 1 June 1982, p.23798.  I presume but don't know for certain that   *|
  |* this polynomial is or will be included in CCITT V.41, which        *|
  |* defines the 16-bit CRC (often called CRC-CCITT) polynomial.  FIPS  *|
  |* PUB 78 says that the 32-bit FCS reduces otherwise undetected       *|
  |* errors by a factor of 10^-5 over 16-bit FCS.                       *|
  \**********************************************************************/

/* Need an unsigned type capable of holding 32 bits; */

/* Copyright (C) 1986 Gary S. Brown.  You may use this program, or
   code or tables extracted from it, as desired without restriction.*/

/* First, the polynomial itself and its table of feedback terms.  The  */
/* polynomial is                                                       */
/* X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0 */
/* Note that we take it "backwards" and put the highest-order term in  */
/* the lowest-order bit.  The X^32 term is "implied"; the LSB is the   */
/* X^31 term, etc.  The X^0 term (usually shown as "+1") results in    */
/* the MSB being 1.                                                    */

/* Note that the usual hardware shift register implementation, which   */
/* is what we're using (we're merely optimizing it by doing eight-bit  */
/* chunks at a time) shifts bits into the lowest-order term.  In our   */
/* implementation, that means shifting towards the right.  Why do we   */
/* do it this way?  Because the calculated CRC must be transmitted in  */
/* order from highest-order term to lowest-order term.  UARTs transmit */
/* characters in order from LSB to MSB.  By storing the CRC this way,  */
/* we hand it to the UART in the order low-byte to high-byte; the UART */
/* sends each low-bit to hight-bit; and the result is transmission bit */
/* by bit from highest- to lowest-order term without requiring any bit */
/* shuffling on our part.  Reception works similarly.                  */

/* The feedback terms table consists of 256, 32-bit entries.  Notes:   */
/*                                                                     */
/*  1. The table can be generated at runtime if desired; code to do so */
/*     is shown later.  It might not be obvious, but the feedback      */
/*     terms simply represent the results of eight shift/xor opera-    */
/*     tions for all combinations of data and CRC register values.     */
/*                                                                     */
/*  2. The CRC accumulation logic is the same for all CRC polynomials, */
/*     be they sixteen or thirty-two bits wide.  You simply choose the */
/*     appropriate table.  Alternatively, because the table can be     */
/*     generated at runtime, you can start by generating the table for */
/*     the polynomial in question and use exactly the same "updcrc",   */
/*     if your application needn't simultaneously handle two CRC       */
/*     polynomials.  (Note, however, that XMODEM is strange.)          */
/*                                                                     */
/*  3. For 16-bit CRCs, the table entries need be only 16 bits wide;   */
/*     of course, 32-bit entries work OK if the high 16 bits are zero. */
/*                                                                     */
/*  4. The values must be right-shifted by eight bits by the "updcrc"  */
/*     logic; the shift must be unsigned (bring in zeroes).  On some   */
/*     hardware you could probably optimize the shift in assembler by  */
/*     using byte-swap instructions.                                   */

#include "bench.h"
#include "utils.h"
#include "string_lib.h"
#include "bar.h"
#include "jrand.c"

__attribute__ ((section(".heapsram"))) UNS_32_BITS crc_tab[CRC_TAB_SIZE];


void initialize_crc_tab();
DWORD crc32pseudo();

void initialize_crc_tab(){
  int i;

  for (i=0;i<CRC_TAB_SIZE;i++){
    crc_tab[i] = crc_32_tab[i];
  }
}

DWORD crc32pseudo()
{
   int i;
   register DWORD oldcrc32;

   oldcrc32 = 0xFFFFFFFF;

   for (i = 0 ; i < 1024; ++i)
   {
      oldcrc32 = UPDC32(jrand(), oldcrc32);
   }

   return ~oldcrc32;
}

void check_crc32(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "crc32",          .test = check_crc32        },
  {0, 0}
};

int main()
{
  return run_suite(testcases);
}

void check_crc32(testresult_t *result, void (*start)(), void (*stop)()) {
  // initialize jrand
  next = 1;

  int n;
  DWORD output;

  // initialize crc tab
  initialize_crc_tab();

  DWORD check_output = 469871797;

  start();

  for(n = 0; n < REPEAT_FACTOR>>5; ++n) {
    output = crc32pseudo();
  }

  stop();

  if (output != check_output){
    result->errors++;
    printf("Error:\nExpected result: %d does not match actual results: %d\n",check_output,output);
  }
}
