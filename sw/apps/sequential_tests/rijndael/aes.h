
/*
   -----------------------------------------------------------------------
   Copyright (c) 2001 Dr Brian Gladman <brg@gladman.uk.net>, Worcester, UK

   TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   This software is provided 'as is' with no guarantees of correctness or
   fitness for purpose.
   -----------------------------------------------------------------------

   1. FUNCTION

   The AES algorithm Rijndael implemented for block and key sizes of 128,
   192 and 256 bits (16, 24 and 32 bytes) by Brian Gladman.

   This is an implementation of the AES encryption algorithm (Rijndael)
   designed by Joan Daemen and Vincent Rijmen. This version is designed
   to provide both fixed and dynamic block and key lengths and can also
   run with either big or little endian internal byte order (see aes.h).
   It inputs block and key lengths in bytes with the legal values being
   16, 24 and 32.

   2. THE CIPHER INTERFACE

   byte                    (an unsigned 8-bit type)
   word                    (an unsigned 32-bit type)
aes_ret:                (a signed 16 bit type for function return values)
aes_good            (value != 0, a good return)
aes_bad             (value == 0, an error return)
enum aes_key:           (encryption direction)
enc                 (set key for encryption)
dec                 (set key for decryption)
both                (set key for both)
class or struct aes     (structure for context)

C subroutine calls:

aes_ret set_blk(const word block_length, aes *cx)  (variable block size)
aes_ret set_key(const byte key[], const word key_length,
const enum aes_key direction, aes *cx)
aes_ret encrypt(const byte input_blk[], byte output_blk[], const aes *cx)
aes_ret decrypt(const byte input_blk[], byte output_blk[], const aes *cx)

IMPORTANT NOTE: If you are using this C interface and your compiler does
not set the memory used for objects to zero before use, you will need to
ensure that cx.mode is set to zero before using the C subroutine calls.

C++ aes class subroutines:

aes_ret set_blk(const word block_length)  (variable block size)
aes_ret set_key(const byte key[], const word key_length,
const aes_key direction)
aes_ret encrypt(const byte input_blk[], byte output_blk[]) const
aes_ret decrypt(const byte input_blk[], byte output_blk[]) const

The block length inputs to set_block and set_key are in numbers of
BYTES, not bits.  The calls to subroutines must be made in the above
order but multiple calls can be made without repeating earlier calls
if their parameters have not changed. If the cipher block length is
variable but set_blk has not been called before cipher operations a
value of 16 is assumed (that is, the AES block size). In contrast to
earlier versions the block and key length parameters are now checked
for correctness and the encryption and decryption routines check to
ensure that an appropriate key has been set before they are called.

3. BYTE ORDER WITHIN 32 BIT WORDS

The fundamental data processing units in Rijndael are 8-bit bytes. The
input, the output and the key input are all enumerated arrays of bytes
in which bytes are numbered starting at zero and increasing to one less
than the number of bytes in the array in question.  When these inputs
and outputs are considered as bit sequences, the n'th byte contains
bits 8n to 8n+7 of the sequence with the lower numbered bit mapped to
the most significant bit within the  byte (i.e. that having a numeric
      value of 128).  However, Rijndael can be implemented more efficiently
using 32-bit words to process 4 bytes at a time provided that the order
of bytes within words is known.  This order is called big-endian if the
lowest numbered bytes in words have the highest numeric significance
and little-endian if the opposite applies. This code can work in either
order irrespective of the native order of the machine on which it runs.
The byte order used internally is set by defining INTERNAL_BYTE_ORDER
whereas the order for all inputs and outputs is specified by defining
EXTERNAL_BYTE_ORDER, the only purpose of the latter being to determine
if a byte order change is needed immediately after input and immediately
before output to account for the use of a different internal byte order.
In almost all situations both of these defines will be set to the native
order of the processor on which the code is to run but other settings
may sometimes be useful in special circumstances.

#define INTERNAL_BYTE_ORDER AES_LITTLE_ENDIAN
#define EXTERNAL_BYTE_ORDER AES_LITTLE_ENDIAN

4. COMPILATION

To compile AES (Rijndael) for use in C code
a. Exclude the AES_DLL define in aes.h
b. Exclude the AES_IN_CPP define in aes.h

To compile AES (Rijndael) for use in in C++ code
a. Exclude the AES_DLL define in aes.h
b. Include the AES_IN_CPP define in aes.h

To compile AES (Rijndael) in C as a Dynamic Link Library
a. Include the AES_DLL define in aes.h
b. Compile the DLL.  If using the test files, exclude aes.c from
   the test build project and compile it with the same defines
as used for the DLL (ensure that the DLL path is correct)

6. CONFIGURATION OPTIONS (see also aes.c)

   a. define BLOCK_SIZE to set the cipher block size (16, 24 or 32) or
   leave this undefined for dynamically variable block size (this will
         result in much slower code).
   b. set AES_IN_CPP to use the code from C++ rather than C
   c. set AES_DLL if AES (Rijndael) is to be compiled to a DLL
   d. set INTERNAL_BYTE_ORDER to one of the above constants to set the
internal byte order (the order used within the algorithm code)
   e. set EXTERNAL_BYTE_ORDER to one of the above constants to set the byte
   order used at the external interfaces for the input, output and key
   byte arrays.

   IMPORTANT NOTE: BLOCK_SIZE is in BYTES: 16, 24, 32 or undefined for aes.c
   and 16, 20, 24, 28, 32 or undefined for aespp.c.  If left undefined a
   slower version providing variable block length is compiled

#define BLOCK_SIZE  16

   Define AES_IN_CPP if you intend to use the AES C++ class rather than the
   C code directly.

#define AES_IN_CPP

   Define AES_DLL if you wish to compile the code to produce a Windows DLL

#define AES_DLL

   */

#ifndef _AES_H
#define _AES_H

#define BLOCK_SIZE  16

#define AES_BIG_ENDIAN      1   /* do not change */
#define AES_LITTLE_ENDIAN   2   /* do not change */

#define INTERNAL_BYTE_ORDER AES_LITTLE_ENDIAN
#define EXTERNAL_BYTE_ORDER AES_LITTLE_ENDIAN

#if defined(BLOCK_SIZE) && ((BLOCK_SIZE & 3) || BLOCK_SIZE < 16 || BLOCK_SIZE > 32)
#error an illegal block size has been specified
#endif

#if defined(AES_DLL)
#undef AES_IN_CPP
#endif

   /* Adjust the C subroutine names if using the C++ interface */

#if defined(AES_IN_CPP)
#define c_name(x)   c_##x
#else
#define c_name(x)   x
#endif

   /*
      The number of key schedule words for different block and key lengths
      (allowing for the method of computation which requires the length to
      be a multiple of the key length):

      Key Schedule    key length (bytes)
      Length          16  20  24  28  32
      ---------------------
      block     16 |  44  60  54  56  64
      length    20 |  60  60  66  70  80
      (bytes)   24 |  80  80  78  84  96
      28 | 100 100 102  98 112
      32 | 120 120 120 126 120

      Rcon Table      key length (bytes)
      Length          16  20  24  28  32
      ---------------------
      block     16 |  10   9   8   7   7
      length    20 |  14  11  10   9   9
      (bytes)   24 |  19  15  12  11  11
      28 |  24  19  16  13  13
      32 |  29  23  19  17  14

      The following values assume that the key length will be variable and may
      be of maximum length (32 bytes).

      Nk = number_of_key_bytes / 4
      Nc = number_of_columns_in_state / 4
      Nr = number of encryption/decryption rounds
      Rc = number of elements in rcon table
      Ks = number of 32-bit words in key schedule
      */

#define Nr(Nk,Nc)   ((Nk > Nc ? Nk : Nc) + 6)
#define Rc(Nk,Nc)   ((Nb * (Nr(Nk,Nc) + 1) - 1) / Nk)
#define Ks(Nk,Nc)   (Nk * (Rc(Nk,Nc) + 1))

#if !defined(BLOCK_SIZE)
#define RC_LENGTH    29
#define KS_LENGTH   128
#else
#define RC_LENGTH   5 * BLOCK_SIZE / 4 - (BLOCK_SIZE == 16 ? 10 : 11)
#define KS_LENGTH   4 * BLOCK_SIZE
#endif

   /* End of configuration options, but see also aes.c */

   typedef unsigned char   byte;           /* must be an 8-bit storage unit */
   typedef unsigned long   word;           /* must be a 32-bit storage unit */
   typedef short           aes_ret;        /* function return value         */

#define aes_bad     0
#define aes_good    1

   /*
      upr(x,n): rotates bytes within words by n positions, moving bytes
      to higher index positions with wrap around into low positions
      ups(x,n): moves bytes by n positions to higher index positions in
      words but without wrap around
      bval(x,n): extracts a byte from a word
      */

#if(INTERNAL_BYTE_ORDER == AES_LITTLE_ENDIAN)

#define upr(x,n)        (((x) << 8 * (n)) | ((x) >> (32 - 8 * (n))))
#define ups(x,n)        ((x) << 8 * (n))
#define bval(x,n)       ((byte)((x) >> 8 * (n)))
#define byte_swap(x)    (upr(x,1) & 0x00ff00ff | upr(x,3) & 0xff00ff00)
#define bytes2word(b0, b1, b2, b3) ((word)(b3) << 24 | (word)(b2) << 16 | (word)(b1) << 8 | (b0))
#else

#define upr(x,n)        (((x) >> 8 * (n)) | ((x) << (32 - 8 * (n))))
#define ups(x,n)        ((x) >> 8 * (n)))
#define bval(x,n)       ((byte)((x) >> 24 - 8 * (n)))
#define byte_swap(x)    (upr(x,3) & 0x00ff00ff | upr(x,1) & 0xff00ff00)
#define bytes2word(b0, b1, b2, b3) ((word)(b0) << 24 | (word)(b1) << 16 | (word)(b2) << 8 | (b3))
#endif

#if(INTERNAL_BYTE_ORDER == EXTERNAL_BYTE_ORDER)

#define word_in(x)      *(word*)(x)
#define word_out(x,v)   *(word*)(x) = (v)

#else

#define word_in(x)      byte_swap(*(word*)(x))
#define word_out(x,v)   *(word*)(x) = byte_swap(v)

#endif

   enum aes_const  {   Nrow =  4,  /* the number of rows in the cipher state       */
      Mcol =  8,  /* maximum number of columns in the state       */
#if defined(BLOCK_SIZE)         /* set up a statically defined block size       */
      Ncol =  BLOCK_SIZE / 4,
      Shr0 =  0,  /* the cyclic shift values for rows 0, 1, 2 & 3 */
      Shr1 =  1,
      Shr2 =  BLOCK_SIZE == 32 ? 3 : 2,
      Shr3 =  BLOCK_SIZE == 32 ? 4 : 3
#endif
   };

enum aes_key    {   enc  =  1,  /* set if encryption is needed */
   dec  =  2,  /* set if decryption is needed */
   both =  3   /* set if both are needed      */
};

#if defined(__cplusplus)
extern "C"
{
#endif

   /* _stdcall is needed for Visual Basic DLLs but is not necessary for C/C++ */

#if defined(AES_DLL)
#define cf_dec  aes_ret __declspec(dllexport) _stdcall
#else
#define cf_dec  aes_ret
#endif

   typedef struct
   {
      word    Nkey;               /* the number of words in the key input block */
      word    Nrnd;               /* the number of cipher rounds                */
      word    e_key[KS_LENGTH];   /* the encryption key schedule                */
      word    d_key[KS_LENGTH];   /* the decryption key schedule                */
#if !defined(BLOCK_SIZE)
      word    Ncol;               /* the number of columns in the cipher state  */
#endif
      byte    mode;               /* encrypt, decrypt or both                   */
   } c_name(aes);

   void initialize_aes();
   cf_dec c_name(set_key)(byte key[], word n_bytes, enum aes_key f, c_name(aes) *cx);
   cf_dec c_name(encrypt)(byte in_blk[], byte out_blk[], c_name(aes) *cx);
   cf_dec c_name(decrypt)(byte in_blk[], byte out_blk[], c_name(aes) *cx);
   cf_dec c_name(set_blk)(word n_bytes, c_name(aes) *cx);

#if defined(__cplusplus)
}

#if defined(AES_IN_CPP)

class aes
{
   c_name(aes) cx;
   public:
#if defined(BLOCK_SIZE)
   aes()   { cx.mode = 0; }
#else
   aes(word n_bytes = 16)
   { cx.mode = 0; c_name(set_blk)(n_bytes, &cx); }
   aes_ret set_blk(const word n_bytes)
   { return c_name(set_blk)(n_bytes, &cx); }
#endif
   aes_ret set_key(const byte key[], const word n_bytes, const aes_key f)
   { return c_name(set_key)(key, n_bytes, f, &cx); }
   aes_ret encrypt(const byte in_blk[], byte out_blk[]) const
   { return c_name(encrypt)(in_blk, out_blk, &cx); }
   aes_ret decrypt(const byte in_blk[], byte out_blk[]) const
   { return c_name(decrypt)(in_blk, out_blk, &cx); }
};

#endif
#endif
#endif


/* vim: set ts=3 sw=3 et: */
