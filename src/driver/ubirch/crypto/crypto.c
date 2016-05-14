#include <wolfssl/wolfcrypt/sha512.h>
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/random.h>
#include <wolfssl/wolfcrypt/ed25519.h>
#include <ubirch/dbgutil/dbgutil.h>
#include <fsl_debug_console.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include "crypto.h"

#ifndef NDEBUG
#  define BUFDEBUG(p, b, s) dbg_dump((p), (b), (s))
#else
#  define BUFDEBUG(...)
#endif

WC_RNG uc_random;

static bool initialized = false;

bool uc_random_init() {
  if (initialized) return true;
  trng_config_t trngConfig;
  TRNG_GetDefaultConfig(&trngConfig);
  trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
  if(TRNG_Init(TRNG0, &trngConfig) != kStatus_Success) return false;
  if(wc_InitRng(&uc_random)) return false;

  initialized = true;
  return true;
}

// === BASE64 ===

char *uc_base64_encode(const unsigned char *in, size_t inlen) {
  // estimate the length of the encoded digest
  word32 base64_len;
  if (Base64_Encode_NoNl(in, inlen, NULL, &base64_len) != LENGTH_ONLY_E) return NULL;

  // allocate memory for the encoded string + 1
  char *encoded_digest = malloc(base64_len * sizeof(char) + 1);
  if (Base64_Encode_NoNl(in, inlen, (byte *) encoded_digest, &base64_len)) return NULL;
  encoded_digest[base64_len] = '\0';

  return encoded_digest;
}

bool uc_base64_decode(const char *in, size_t inlen, unsigned char *out, size_t *outlen) {
  if (Base64_Decode((const byte *) in, strlen(in), out, outlen)) return false;
  return true;
}

// === SHA512 ===

bool uc_sha512(const unsigned char *in, size_t inlen, unsigned char *hash) {
  Sha512 sha512;

  wc_InitSha512(&sha512);
  if (wc_Sha512Update(&sha512, in, inlen)) return false;
  if (wc_Sha512Final(&sha512, hash)) return false;
  BUFDEBUG("SHA512", hash, SHA512_DIGEST_SIZE);

  return true;
}

char *uc_sha512_encoded(const unsigned char *in, size_t inlen) {
  unsigned char digest[SHA512_DIGEST_SIZE];
  if (!uc_sha512(in, inlen, digest)) return NULL;

  return uc_base64_encode(digest, SHA512_DIGEST_SIZE);
}

// === ED25519 ===

bool uc_ecc_create_key(uc_ed25519_key *key) {
  if(uc_random_init());

  wc_ed25519_init(key);
  if(wc_ed25519_make_key(&uc_random, ED25519_KEY_SIZE, key))
    return false;

  BUFDEBUG("ECCPRV", key->k, ED25519_PRV_KEY_SIZE);
  BUFDEBUG("ECCPUB", key->p, ED25519_PUB_KEY_SIZE);

  return false;
}

bool uc_import_ecc_key(uc_ed25519_key *key, const unsigned char *in, size_t inlen) {
  if(inlen < ED25519_PRV_KEY_SIZE) return false;
  uc_random_init();

  wc_ed25519_init(key);
  if(wc_ed25519_import_private_key(in, ED25519_KEY_SIZE, in + 32, ED25519_PUB_KEY_SIZE, key))
    return false;

  BUFDEBUG("ECCPRV", key->k, ED25519_PRV_KEY_SIZE);
  BUFDEBUG("ECCPUB", key->p, ED25519_PUB_KEY_SIZE);

  return true;
}

bool uc_ecc_export_pub(ed25519_key *key, uc_ed25519_pkcs8 *pkcs8) {
  // copy the ASN.1 (PKCS#8) header into the encoded key array
  memcpy(pkcs8->header, (const unsigned char[]) {
    0x30, 13 + ED25519_PUB_KEY_SIZE, 0x30, 0x08, 0x06, 0x03, 0x2b, 0x65, 0x64, 0x0a, 0x01, 0x01,
    0x03, 1 + ED25519_PUB_KEY_SIZE, 0x00
  }, 15);

  word32 len = ED25519_PUB_KEY_SIZE;
  const int status = wc_ed25519_export_public((ed25519_key *) key, pkcs8->data, &len);
  if (status < 0 || len != ED25519_PUB_KEY_SIZE) {
    PRINTF("ecc pub key export failed: [%d] %d\r\n", status, len);
    return false;
  }
  memcpy(pkcs8->footer, (const unsigned char[]) {0x00, 0x00}, 2);
  BUFDEBUG("ECCPUB", (const unsigned char *) pkcs8, sizeof(uc_ed25519_pkcs8));

  return true;
}

char *uc_ecc_export_pub_encoded(ed25519_key *key) {
  uc_ed25519_pkcs8 pkcs8;
  uc_ecc_export_pub(key, &pkcs8);

  return uc_base64_encode((const unsigned char *) &pkcs8, sizeof(pkcs8));
}


bool uc_ecc_sign(uc_ed25519_key *key, const unsigned char *in, size_t inlen, unsigned char *signature) {
  word32 len = ED25519_SIG_SIZE;

  const int status = wc_ed25519_sign_msg(in, inlen, signature, &len, key);
  if (status < 0 || len != ED25519_SIG_SIZE) {
    PRINTF("ecc signature failed: [%d] %d\r\n", status, len);
    return false;
  }
  BUFDEBUG("ECCSIG", signature, len);

  return true;
}

char *uc_ecc_sign_encoded(uc_ed25519_key *key, const unsigned char *in, size_t inlen) {
  unsigned char signature[ED25519_SIG_SIZE];
  if(!uc_ecc_sign(key, in, inlen, signature)) return NULL;

  return uc_base64_encode(signature, ED25519_SIG_SIZE);
}





