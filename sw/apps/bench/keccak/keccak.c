// keccak.c
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>
// A baseline Keccak (3rd round) implementation.

#include "common.h"

#define KECCAK_ROUNDS 24

#define ROTL64(x, y) (((x) << (y)) | ((x) >> (64 - (y))))

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define __bswap_64(x)                     \
  ( (((x) & 0xff00000000000000ull) >> 56) \
  | (((x) & 0x00ff000000000000ull) >> 40) \
  | (((x) & 0x0000ff0000000000ull) >> 24) \
  | (((x) & 0x000000ff00000000ull) >>  8) \
  | (((x) & 0x00000000ff000000ull) <<  8) \
  | (((x) & 0x0000000000ff0000ull) << 24) \
  | (((x) & 0x000000000000ff00ull) << 40) \
  | (((x) & 0x00000000000000ffull) << 56))
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __bswap_64(x) (x)
#else
#error Unsupported endianness
#endif

const uint64_t keccakf_rndc[24] =
{
    0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
    0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
    0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
    0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
    0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
    0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
    0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

const int keccakf_rotc[24] =
{
    1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
    27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44
};

const int keccakf_piln[24] =
{
    10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
    15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1
};

static inline int mod5(int a) {
  while (a > 9) {
    int s = 0; /* accumulator for the sum of the digits */
    while (a != 0) {
      s = s + (a & 7);
      a = (a >> 3) * 3;
    }
    a = s;
  }
  /* note, at this point: a < 10 */
  if (a > 4) a = a - 5;
  return a;
}
// update the state with given number of rounds

void keccakf(uint64_t st[25], int rounds)
{
    int i, j, round;
    uint64_t t, bc[5];

    for (round = 0; round < rounds; round++) {

        // Theta
        for (i = 0; i < 5; i++)
            bc[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];

        for (i = 0; i < 5; i++) {
            t = bc[mod5(i + 4)] ^ ROTL64(bc[mod5(i + 1)], 1);
            for (j = 0; j < 25; j += 5)
                st[j + i] ^= t;
        }

        // Rho Pi
        t = st[1];
        for (i = 0; i < 24; i++) {
            j = keccakf_piln[i];
            bc[0] = st[j];
            st[j] = ROTL64(t, keccakf_rotc[i]);
            t = bc[0];
        }

        //  Chi
        for (j = 0; j < 25; j += 5) {
            for (i = 0; i < 5; i++)
                bc[i] = st[j + i];
            for (i = 0; i < 5; i++)
                st[j + i] ^= (~bc[mod5(i + 1)]) & bc[mod5(i + 2)];
        }

        //  Iota
        st[0] ^= keccakf_rndc[round];
    }
}

// compute a keccak hash (md) of given byte length from "in"

int do_keccak(const uint8_t *in, int inlen, uint8_t *md, int mdlen)
{
    uint64_t st[25];
    uint8_t temp[144];
    int i, rsiz, rsizw;

    rsiz = 200 - 2 * mdlen;
    rsizw = rsiz / 8;

    memset(st, 0, sizeof(st));

    for ( ; inlen >= rsiz; inlen -= rsiz, in += rsiz) {
        for (i = 0; i < rsizw; i++)
          st[i] ^= __bswap_64(((uint64_t *) in)[i]);
        keccakf(st, KECCAK_ROUNDS);
    }

    // last block and padding
    memcpy(temp, in, inlen);
    temp[inlen++] = 1;
    memset(temp + inlen, 0, rsiz - inlen);
    temp[rsiz - 1] |= 0x80;

    for (i = 0; i < rsizw; i++)
      st[i] ^= __bswap_64(((uint64_t *) temp)[i]);

    keccakf(st, KECCAK_ROUNDS);

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

    for (i = 0; i < mdlen / 8; i++)
      ((uint64_t *) md)[i] = __bswap_64(((uint64_t *) st)[i]);

    int remaining = mdlen % 8;
    for (i = 0; i < remaining; i++)
      ((uint8_t *) md)[mdlen - remaining + i] = ((uint8_t *) st)[mdlen + remaining - i - 1];
#else
    memcpy(md, st, mdlen);
#endif

    return 0;
}
