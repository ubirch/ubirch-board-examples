/**
 * wolfSSL test code.
 *
 * Generates a key pair on the board and loads a public key from elsewhere.
 * Then encrypts and signs a message and prints the resulting ciphers in
 * DER encoding.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-01
 *
 * Copyright 2016 ubirch GmbH (https://ubirch.com)
 *
 * == LICENSE ==
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdbool.h>
#include <board.h>
#include <stdio.h>
#include <drivers/fsl_trng.h>
#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/signature.h>
#include "public_key.h"

static const char plaintext[] = "We love things.\n0a1b2c3d4e5f6g7h8i9j-UBIRCH\n";

void print_buffer_hex(const byte *out, int len) {
  for (int i = 0; i < len; i++) {
    PRINTF("%02x", out[i]);
  }
  PRINTF("\r\n");
}

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

void print_public_key(RsaKey *key) {
  byte encoded_key[294];
  wc_RsaKeyToPublicDer(key, encoded_key, 2048);
  print_buffer_hex(encoded_key, sizeof(encoded_key));
}

void error(char *message) {
  PRINTF("] ERROR: %s\r\n", message);
  while (true);
}

WC_RNG rng;
RsaKey board_rsa_key;
RsaKey recipient_public_key;

int init_trng() {
  PRINTF("- initializing random number generator\r\n");
  trng_config_t trngConfig;
  TRNG_GetDefaultConfig(&trngConfig);
  trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
  int r = TRNG_Init(TRNG0, &trngConfig);
  return r == kStatus_Success ? wc_InitRng(&rng) : r;
}

int init_board_key(unsigned int size) {
  PRINTF("- generating board private key (please wait)\r\n");
  wc_InitRsaKey(&board_rsa_key, NULL);
  int r = wc_MakeRsaKey(&board_rsa_key, size, 65537, &rng);
  if(r != 0) return r;
  PRINTF("-- BOARD PUBLIC KEY\r\n");
  print_public_key(&board_rsa_key);
  return 0;
}


int init_recipient_public_key(byte *key, size_t length) {
  PRINTF("- loading recipient public key\r\n");
  word32 idx = 0;
  wc_InitRsaKey(&recipient_public_key, NULL); // not using heap hint. No custom memory
  return wc_RsaPublicKeyDecode(key, &idx, &recipient_public_key, length);
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(SystemCoreClock / 100 - 1);

  PRINTF("ubirch #1 r0.2 RSA encryption/signature test\r\n");
  if (init_trng() != 0) error("failed to initialize TRNG");
  if (init_board_key(2048) != 0) error("failed to generate key pair");
  if (init_recipient_public_key(test_der, test_der_len)) error("failed to load recipient public key");

  byte cipher[256]; // 256 bytes is large enough to store 2048 bit RSA ciphertext
  word32 plaintextLength = strlen(plaintext);
  word32 cipherLength = sizeof(cipher);

  PRINTF("- signing message with board private key\r\n");
  int signatureLength = wc_SignatureGetSize(WC_SIGNATURE_TYPE_RSA, &board_rsa_key, sizeof(board_rsa_key));
  byte *signature = malloc((size_t) signatureLength);

  if (wc_SignatureGenerate(
    WC_HASH_TYPE_SHA256, WC_SIGNATURE_TYPE_RSA,
    (const byte *) plaintext, plaintextLength,
    signature, (word32 *) &signatureLength,
    &board_rsa_key, sizeof(board_rsa_key),
    &rng) != 0)
    error("failed to sign plain text message");
  PRINTF("-- SIGNATURE\r\n");
  print_buffer_hex(signature, signatureLength);

  PRINTF("- encrypting message\r\n");
  int r = wc_RsaPublicEncrypt((const byte *) plaintext, plaintextLength, cipher, cipherLength, &recipient_public_key, &rng);
  if(r < 0) error("failed to encrypt message");

  PRINTF("-- CIPHER (%d bytes)\r\n", r);
  print_buffer_hex(cipher, r);

  wc_FreeRsaKey(&board_rsa_key);
  wc_FreeRsaKey(&recipient_public_key);

  PRINTF("THE END\r\n");
  while (true) {
    uint8_t ch = (uint8_t) GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}

