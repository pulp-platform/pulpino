/* BEEBS nettle-arcfour benchmark

   Copyright (C) 2001, 2004 Niels Möller
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

/* This scale factor will be changed to equalise the runtime of the
   benchmarks. */
#define SCALE_FACTOR    (REPEAT_FACTOR >> 0)

static const uint8_t key[16] = {
  0xf,0xe,0xd,0xc,0xb,0xa,0x9,0x8,0x7,0x6,0x5,0x4,0x3,0x2,0x1,0x0
};

static const uint8_t data[16] = {
  0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf
};

static uint8_t result[16];

struct arcfour_ctx
{
  uint8_t S[256];
  uint8_t i;
  uint8_t j;
} arcfour_ctx;

#define SWAP(a,b) do { int _t = a; a = b; b = _t; } while(0)

void
arcfour_set_key(struct arcfour_ctx *ctx,
		int length, const uint8_t *key)
{
  unsigned i, j, k;

  /* Initialize context */
  for (i = 0; i<256; i++)
    ctx->S[i] = i;

  for (i = j = k = 0; i<256; i++)
    {
      j += ctx->S[i] + key[k]; j &= 0xff;
      SWAP(ctx->S[i], ctx->S[j]);
      /* Repeat key as needed */
      k = (k + 1) % length;
    }
  ctx->i = ctx->j = 0;
}

void
arcfour_crypt(struct arcfour_ctx *ctx,
	      int length, uint8_t *dst,
	      const uint8_t *src)
{
  register uint8_t i, j;
  register int si, sj;

  i = ctx->i; j = ctx->j;
  while(length--)
    {
      i++; i &= 0xff;
      si = ctx->S[i];
      j += si; j &= 0xff;
      sj = ctx->S[i] = ctx->S[j];
      ctx->S[j] = si;
      *dst++ = *src++ ^ ctx->S[ (si + sj) & 0xff ];
    }
  ctx->i = i; ctx->j = j;
}


int
benchmark (void)
{
  /* Encryption */
  arcfour_set_key(&arcfour_ctx, 16, key);
  arcfour_crypt(&arcfour_ctx, 16, result, data);
  /* Decryption */
  arcfour_set_key(&arcfour_ctx, 16, key);
  arcfour_crypt(&arcfour_ctx, 16, result, result);
  return 0;
}


