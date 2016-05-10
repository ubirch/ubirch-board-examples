#include <wolfssl/wolfcrypt/sha512.h>
#include <wolfssl/wolfcrypt/coding.h>
#include <fsl_debug_console.h>
#include <dbgutil.h>
#include "crypto.h"

#ifndef NDEBUG
#  define BUFDEBUG(p, b,s) dbg_dump((p), (b), (s))
#endif

int crypto_sha512(const byte *message, size_t size, byte *hash) {
  Sha512 sha512;

  wc_InitSha512(&sha512);
  if(wc_Sha512Update(&sha512, message, size)) return cStatus_Failed;
  if(wc_Sha512Final(&sha512, hash)) return cStatus_Failed;
  BUFDEBUG("SHA512", hash, SHA512_DIGEST_SIZE);

  return cStatus_Success;
}

char *crypto_base64_encode(const byte *in, size_t inlen) {
  word32 outlen;

  Base64_Encode_NoNl(in, inlen, NULL, &outlen);
  // allocate len + 1 to cover for string ending zero
  char *out = malloc(++outlen * sizeof(byte) + 1);
  Base64_Encode_NoNl(in, inlen, (byte *) out, &outlen);
  out[outlen] = 0;

  return out;
}



