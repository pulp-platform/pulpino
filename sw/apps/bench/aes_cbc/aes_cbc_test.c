#include "common.h"

#define AES_MAXROUNDS 14
#define AES_BLOCKSIZE 16
#define AES_IV_SIZE   16
#define KEYSIZE       16
#define MSG_SIZE      KEYSIZE * 4

static const uint8_t key[KEYSIZE] = {
  0xff, 0xde, 0x00, 0xad,
  0xff, 0xb0, 0x00, 0x0b,
  0xff, 0xde, 0x00, 0xad,
  0xff, 0xb0, 0x00, 0x0b
};

static const uint8_t iv[KEYSIZE] = {
  0xff, 0xc0, 0x00, 0xfe,
  0xff, 0xc0, 0x00, 0x7d,
  0xff, 0xc0, 0x00, 0xfe,
  0xff, 0xc0, 0x00, 0x7d
};

static const uint8_t original_msg[MSG_SIZE] = {
  0xf4, 0xfc, 0xa2, 0xf4,
  0x93, 0xfa, 0x64, 0xeb,
  0x87, 0xca, 0xeb, 0x62,
  0x90, 0x89, 0x34, 0xdb,
  0x34, 0xe3, 0xf8, 0xc6,
  0xd7, 0xf6, 0x89, 0xe7,
  0xc0, 0x37, 0x43, 0xcd,
  0x32, 0x69, 0xcd, 0xbd,
  0x05, 0xec, 0x97, 0xbf,
  0x05, 0x92, 0xc9, 0xf7,
  0x8d, 0x4b, 0xb0, 0x88,
  0x1a, 0xc2, 0x15, 0x2e,
  0xba, 0x46, 0x58, 0xf9,
  0x4d, 0x1f, 0xe9, 0xef,
  0xa8, 0x6b, 0x5a, 0x6f,
  0x8b, 0xe6, 0x7d, 0x51
};

static uint8_t out[MSG_SIZE] __sram;
static uint8_t msg[MSG_SIZE] __sram;


typedef struct aes_key_st {
  uint16_t rounds;
  uint16_t key_size;
  uint32_t ks[(AES_MAXROUNDS+1)*8];
  uint8_t iv[AES_IV_SIZE];
} AES_CTX;

typedef enum {
  AES_MODE_128,
  AES_MODE_256
} AES_MODE;

static AES_CTX context __sram;


extern void AES_set_key(AES_CTX *ctx, const uint8_t *key,
                        const uint8_t *iv, AES_MODE mode);
extern void AES_cbc_encrypt(AES_CTX *ctx, const uint8_t *in, uint8_t *out,
                            int size);
extern void AES_convert_key(AES_CTX *ctx);
extern void AES_cbc_decrypt(AES_CTX *ctx, const uint8_t *in, uint8_t *out,
                            int size);

void test_setup() {
}

void test_clear() {
  memcpy(msg, original_msg, MSG_SIZE);
  memset(out, 0, MSG_SIZE);
  memset(&context, 0, sizeof(context));
}

void test_run() {
  AES_set_key(&context, key, iv, AES_MODE_128);
  AES_cbc_encrypt(&context, msg, out, MSG_SIZE);

  memset(msg, 0, MSG_SIZE);
  memset(&context, 0, sizeof(context));
  AES_set_key(&context, key, iv, AES_MODE_128);

  AES_convert_key(&context);
  AES_cbc_decrypt(&context, out, msg, MSG_SIZE);
}

int test_check() {
  return 0 == memcmp(msg, original_msg, MSG_SIZE);
}
