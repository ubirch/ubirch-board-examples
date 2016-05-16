/**
 * wolfSSL test code (ECC).
 *
 * Generates a key pair on the board and loads a public key from elsewhere.
 * Then signs a message and prints the signature hex encoded.
 *
 * @author Matthias L. Jugel
 * @date 2016-05-04
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
#include <wolfssl/wolfcrypt/signature.h>
#include <wolfssl/wolfcrypt/ed25519.h>
#include <ubirch/dbgutil.h>

#include "ecc_keys.h"

const char *plaintext = "We love things.\n0a1b2c3d4e5f6g7h8i9j-UBIRCH\n";

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

void print_public_key(ed25519_key *key) {
  byte encoded_key[ED25519_PUB_KEY_SIZE + 17];
  memset(encoded_key, 0x00, ED25519_PUB_KEY_SIZE + 17);
  // copy the ASN.1 (PKCS#8) header into the encoded key array
  memcpy(encoded_key, (const byte[]) {
    0x30, 13 + ED25519_PUB_KEY_SIZE, 0x30, 0x08, 0x06, 0x03, 0x2b, 0x65, 0x64, 0x0a, 0x01, 0x01,
    0x03, 1 + ED25519_PUB_KEY_SIZE, 0x00
  }, 15);

  word32 len = ED25519_PUB_KEY_SIZE;
  wc_ed25519_export_public(key, encoded_key + 15, &len);
  encoded_key[sizeof(encoded_key) - 1] = 0x00;
  dbg_dump("ECCPUB", encoded_key, sizeof(encoded_key));
  dbg_xxd("ECCPUB", encoded_key, sizeof(encoded_key));
}

void print_private_key(ed25519_key *key) {
  byte encoded_key[ED25519_PRV_KEY_SIZE];
  memset(encoded_key, 0x00, ED25519_PRV_KEY_SIZE);

  word32 len = ED25519_PRV_KEY_SIZE;
  wc_ed25519_export_private(key, encoded_key, &len);
  dbg_dump("ECCPRI", encoded_key, len);
  dbg_xxd("ECCPRI", encoded_key, sizeof(encoded_key));
}

void error(char *message) {
  PRINTF("] ERROR: %s\r\n", message);
  while (true);
}

WC_RNG rng;
ed25519_key board_ecc_key;
ed25519_key recipient_public_key;

int init_trng() {
  PRINTF("- initializing random number generator\r\n");
  trng_config_t trngConfig;
  TRNG_GetDefaultConfig(&trngConfig);
  trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
  int r = TRNG_Init(TRNG0, &trngConfig);
  return r == kStatus_Success ? wc_InitRng(&rng) : r;
}

int init_board_key(unsigned int size) {
/*
  PRINTF("- generating board private key (please wait)\r\n");
  wc_ed25519_init(&board_ecc_key);
  int r = wc_ed25519_make_key(&rng, size, &board_ecc_key);
  */
  wc_ed25519_init(&board_ecc_key);
  int r = wc_ed25519_import_private_key(device_ecc_key, ED25519_KEY_SIZE,
                                        device_ecc_key + 32, ED25519_PUB_KEY_SIZE,
                                        &board_ecc_key);
  if (r != 0) return r;
  PRINTF("-- BOARD KEY\r\n");
  print_private_key(&board_ecc_key);

  PRINTF("-- BOARD PUBLIC KEY\r\n");
  print_public_key(&board_ecc_key);
  return 0;
}


int init_recipient_public_key(const byte *key, size_t length) {
  PRINTF("- loading recipient public key\r\n");
  wc_ed25519_init(&recipient_public_key); // not using heap hint. No custom memory
  return wc_ed25519_import_public(key, length, &recipient_public_key);
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  PRINTF("ubirch #1 ECC encryption/signature test\r\n");
  if (init_trng() != 0) error("failed to initialize TRNG");
  if (init_board_key(ED25519_KEY_SIZE) != 0) error("failed to generate key pair");
  if (init_recipient_public_key(recipient_key, recpient_key_len)) error("failed to load recipient public key");

  word32 plaintextLength = strlen(plaintext);

  PRINTF("- signing message with board private key\r\n");
  word32 signatureLength;
  byte signature[ED25519_SIG_SIZE];

  if (wc_ed25519_sign_msg((const byte *) plaintext, plaintextLength, signature, &signatureLength, &board_ecc_key) !=
      0) {
    error("failed to sign plain text message");
  }

  PRINTF("-- SIGNATURE\r\n");
  dbg_dump("ECCSIG", signature, signatureLength);
  dbg_xxd("ECCSIG", signature, signatureLength);

/* TODO encryption...
  PRINTF("- encrypting message\r\n");

  byte cipher[256]; // 256 bytes is large enough to store 2048 bit RSA ciphertext
  word32 cipherLength = sizeof(cipher);

  int r = wc_curve25519_shared_secret()
  int r = wc_ecc_encrypt(&board_ecc_key, &recipient_public_key, plaintext, plaintextLength, cipher, cipherLength,  &rng);
  if (r < 0) error("failed to encrypt message");

  PRINTF("-- CIPHER (%d bytes)\r\n", r);
  print_buffer_hex(cipher, r);
*/

  wc_ed25519_free(&board_ecc_key);
  wc_ed25519_free(&recipient_public_key);

  PRINTF("THE END\r\n");
  while (true) {
    int ch = GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}

