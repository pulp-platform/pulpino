// Copyright 2017 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the “License”); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an “AS IS” BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
#ifndef SHA_H
#define SHA_H

#include <stdint.h>

/* NIST Secure Hash Algorithm */
/* heavily modified from Peter C. Gutmann's implementation */

/* Useful defines & typedefs */

#define SHA_BLOCKSIZE   64
#define SHA_DIGESTSIZE    20

typedef struct {
   uint32_t digest[5];    /* message digest */
   uint32_t count_lo, count_hi; /* 64-bit bit count */
   uint32_t data[16];   /* SHA data buffer */
} SHA_INFO;

void sha_init(SHA_INFO *);
void sha_update(SHA_INFO *, uint8_t *, int);
void sha_final(SHA_INFO *);

void sha_stream(SHA_INFO *);
void sha_print(SHA_INFO *);

#endif /* SHA_H */
