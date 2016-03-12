/*! Blink - simplistic app. Blinks LED, reverses blink pattern on button press.
 * Written by Matthias Krauss
 * Copyright 2016 Press Every Key UG
 */


#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <drivers/fsl_pit.h>
#include <drivers/fsl_trng.h>
#include <wolfssl/wolfcrypt/rsa.h>

const byte public_key[] = {
  0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00,
  0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xbd, 0x77, 0x49,
  0x26, 0xd1, 0x6b, 0x54, 0x2b, 0xf7, 0x5e, 0x7c, 0x3a, 0x6c, 0xc3, 0xf3,
  0x0d, 0x88, 0x22, 0xf6, 0x0b, 0x60, 0x5d, 0x5f, 0xa7, 0x51, 0x91, 0x4d,
  0x40, 0x1c, 0x3f, 0xaf, 0x3d, 0x99, 0x2e, 0x43, 0x8a, 0xc3, 0xcf, 0xcd,
  0xe0, 0xdb, 0x64, 0x60, 0x68, 0xc2, 0x41, 0x5f, 0x59, 0x43, 0xde, 0xfd,
  0xb0, 0x86, 0x18, 0x4d, 0xae, 0xbb, 0xb2, 0x7b, 0x22, 0xe7, 0x14, 0xb1,
  0xb7, 0x61, 0x17, 0x2f, 0x9c, 0x2e, 0x5e, 0x01, 0xa8, 0x9c, 0xb6, 0x04,
  0xdc, 0xd0, 0x20, 0x76, 0xe1, 0x26, 0xf5, 0x6f, 0xa9, 0x48, 0x66, 0x81,
  0xd8, 0x34, 0x8b, 0x21, 0x3b, 0xc0, 0x0b, 0x5f, 0x15, 0xc9, 0xf4, 0x0f,
  0x08, 0xe2, 0x4b, 0x0b, 0x42, 0x16, 0xa3, 0x68, 0x52, 0x3e, 0x5f, 0xce,
  0x0b, 0x56, 0x79, 0x35, 0xf5, 0xc9, 0xa7, 0xcd, 0x75, 0x43, 0x6e, 0x63,
  0x11, 0xfd, 0xa0, 0x0d, 0xb4, 0xfb, 0x58, 0x4f, 0x20, 0xf8, 0xcc, 0x23,
  0x3b, 0x1c, 0xe9, 0x8e, 0xed, 0x2c, 0x15, 0xac, 0x4e, 0xac, 0x4a, 0x65,
  0xdf, 0xa4, 0x14, 0xdd, 0x56, 0x9b, 0xd1, 0xb8, 0xbe, 0xf8, 0x08, 0x19,
  0x60, 0xbd, 0xbf, 0xe6, 0x1e, 0x23, 0x43, 0x5b, 0x2f, 0xb6, 0x1b, 0x02,
  0x14, 0x3c, 0x35, 0x30, 0x83, 0x72, 0xe9, 0x9b, 0x32, 0xf7, 0xce, 0xba,
  0xb3, 0x96, 0xc3, 0xbc, 0x37, 0xca, 0x08, 0xbe, 0x8e, 0x59, 0xa9, 0xfa,
  0xc2, 0xb4, 0x2c, 0xbb, 0xff, 0xfa, 0x68, 0x48, 0x1c, 0xfb, 0x7b, 0xe8,
  0xd7, 0xe7, 0xcf, 0xd9, 0x48, 0xe0, 0x8f, 0x61, 0x73, 0x78, 0x61, 0x74,
  0xf3, 0xe7, 0x86, 0x03, 0xf6, 0x82, 0xc9, 0x1e, 0xc3, 0x04, 0x91, 0x99,
  0x73, 0x28, 0xfd, 0x58, 0xc2, 0xa2, 0x10, 0xe4, 0x8f, 0xbd, 0xa8, 0x57,
  0xf2, 0x9b, 0x56, 0x78, 0x2e, 0xe8, 0xfb, 0x28, 0x2e, 0x1a, 0x9a, 0x0b,
  0x3d, 0x02, 0x03, 0x01, 0x00, 0x01
};
const unsigned int public_key_len = 294;

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
    PRINTF("wolfSSL test\r\n");

    WC_RNG rng;
    if (wc_InitRng(&rng) == 0) {
      word32 idx = 0;
      RsaKey rsa_key;
      wc_InitRsaKey(&rsa_key, NULL); // not using heap hint. No custom memory
      if (wc_RsaPublicKeyDecode(public_key, &idx, &rsa_key, public_key_len) == 0) {
        byte msg[] = "0123456789";
        byte out[2048] = {0, 0, 0, 0, 0, 0}; // 256 bytes is large enough to store 2048 bit RSA ciphertext
        word32 inLen = sizeof(msg);
        word32 outLen = sizeof(out);

        int ret = wc_RsaPublicEncrypt(msg, inLen, out, outLen, &rsa_key, &rng);
        PRINTF("ENCRYPT: %d\r\n", ret);

        for (int i = 0; i < ret; i++) {
          PRINTF("%02x", out[i]);
        }
        PRINTF("\r\n");
      } else {
        PRINTF("RSA public key loading failed");
      }
      wc_FreeRsaKey(&rsa_key);
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
