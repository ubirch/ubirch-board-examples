/*! Blink - simplistic app. Blinks LED, reverses blink pattern on button press.
 * Written by Matthias Krauss
 * Copyright 2016 Press Every Key UG
 */


#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <drivers/fsl_trng.h>
#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/signature.h>
#include "board_key.h"
#include "public_key.h"

const byte message[] = "We love things.\n0a1b2c3d4e5f6g7h8i9j-UBIRCH\n";

void to_hex(const byte *out, int len) {
  for (int i = 0; i < len; i++) {
    PRINTF("%02x", out[i]);
  }
  PRINTF("\r\n");
}

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  LED_Write((counter % 100) < 10);
}

int main(void) {
  BOARD_Init();
  SysTick_Config(SystemCoreClock / 100 - 1);

  trng_config_t trngConfig;
  TRNG_GetDefaultConfig(&trngConfig);
  trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
  if (TRNG_Init(TRNG0, &trngConfig) == kStatus_Success) {
    WC_RNG rng;

    PRINTF("ubirch #2 RSA encryption/signature test\r\n");

    if (wc_InitRng(&rng) == 0) {
      PRINTF("- loading board private key\r\n");

      RsaKey board_priv_key;
      word32 idx = 0;
      wc_InitRsaKey(&board_priv_key, NULL); // not using heap hint. No custom memory
      int r;
      if ((r = wc_RsaPrivateKeyDecode(board_der, &idx, &board_priv_key, board_der_len)) == 0) {
        PRINTF("- loading recipient public key\r\n");

        idx = 0;
        RsaKey ext_pub_key;
        wc_InitRsaKey(&ext_pub_key, NULL); // not using heap hint. No custom memory
        if ((r = wc_RsaPublicKeyDecode(test_der, &idx, &ext_pub_key, test_der_len)) == 0) {
          PRINTF("- encrypting message\r\n");

          byte encr[256]; // 256 bytes is large enough to store 2048 bit RSA ciphertext
          word32 inLen = sizeof(message);
          word32 outLen = sizeof(encr);

          int encRet = wc_RsaPublicEncrypt(message, inLen, encr, outLen, &ext_pub_key, &rng);

          PRINTF("- signing message with board private key\r\n");

          int sigLen = wc_SignatureGetSize(WC_SIGNATURE_TYPE_RSA, &board_priv_key, sizeof(board_priv_key));
          byte *sigBuf = malloc((size_t) sigLen);

          int sigRet = wc_SignatureGenerate(
            WC_HASH_TYPE_SHA256, WC_SIGNATURE_TYPE_RSA,
            message, (word32) inLen,
            sigBuf, (word32 *) &sigLen,
            &board_priv_key, sizeof(board_priv_key),
            &rng);

          PRINTF("-- CIPHER (%d)\r\n", encRet);
          to_hex(encr, encRet);
          PRINTF("-- SIGNATURE (%d)\r\n", sigRet);
          to_hex(sigBuf, sigLen);

        } else {
          PRINTF("RSA public key loading failed: %d\r\n", r);
        }
        wc_FreeRsaKey(&ext_pub_key);
      } else {
        PRINTF("RSA private key loading failed: %d\r\n", r);
      }
      wc_FreeRsaKey(&board_priv_key);
    } else {
      PRINTF("RNG init failed\r\n");
    }
  } else {
    PRINTF("TRNG init failed\r\n");
  }

  PRINTF("THE END\r\n");
  while (true) {
    uint8_t ch = GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}

