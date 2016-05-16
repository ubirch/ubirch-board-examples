/**
 * Ubirch crypto layer test code.
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
#include <ubirch/crypto.h>

const char *plaintext = "We love things.\n0a1b2c3d4e5f6g7h8i9j-UBIRCH\n";

void SysTick_Handler() {
  static uint32_t counter = 0;
  counter++;
  BOARD_LED0((counter % 100) < 10);
}

void error(char *message) {
  PRINTF("] ERROR: %s\r\n", message);
  while (true);
}

int main(void) {
  board_init();
  board_console_init(BOARD_DEBUG_BAUD);

  SysTick_Config(BOARD_SYSTICK_100MS);

  PRINTF("ubirch #1 ECC crypto test\r\n");

  PRINTF("initialize random\r\n");
  uc_random_init();

  PRINTF("create ED25519 key\r\n");
  uc_ed25519_key key;
  uc_ecc_create_key(&key);

  PRINTF("export public key as PKCS#8 structure\r\n");
  uc_ed25519_pkcs8 pkcs8;
  uc_ecc_export_pub(&key, &pkcs8);

  PRINTF("create signature from plaintext\r\n");
  char *signature = uc_ecc_sign_encoded(&key, (const unsigned char *) plaintext, strlen(plaintext));
  PRINTF("SIGNATURE: %s\r\n", signature);
  free(signature);

  PRINTF("THE END\r\n");
  while (true) {
    int ch = GETCHAR();
    if (ch == '\r') PUTCHAR('\n');
    PUTCHAR(ch);
  }
}

