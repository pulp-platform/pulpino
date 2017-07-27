/* Copyright (C) 2017 ETH Zurich, University of Bologna
 * All rights reserved.
 *
 * This code is under development and not yet released to the public.
 * Until it is released, the code is under the copyright of ETH Zurich and
 * the University of Bologna, and may contain confidential and/or unpublished
 * work. Any reuse/redistribution is strictly forbidden without written
 * permission from ETH Zurich.
 *
 * Bug fixes and contributions will eventually be released under the
 * SolderPad open hardware license in the context of the PULP platform
 * (http://www.pulp-platform.org), under the copyright of ETH Zurich and the
 * University of Bologna.
 */
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
