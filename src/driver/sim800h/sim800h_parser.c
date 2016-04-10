/**
 * ubirch#1 SIM800H AT command parser.
 *
 * @author Matthias L. Jugel
 * @date 2016-04-09
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
#include <string.h>
#include "sim800h_parser.h"
#include "sim800h.h"

#ifndef NDEBUG
#include <utilities/fsl_debug_console.h>
#else
# undef PRINTF
# define PRINTF(...)
#endif


int check_urc(const char *line) {
  size_t len = strlen(line);
  for (int i = 0; SIM800H_URC[i] != NULL; i++) {
    const char *urc = SIM800H_URC[i];
    size_t urc_len = strlen(SIM800H_URC[i]);
    if (len >= urc_len && !strncmp(urc, line, urc_len)) {
      PRINTF("GSM INFO !! [%02d] %s\r\n", i, line);
      return i;
    }
  }
  return -1;
}

void sim800h_send(const char *cmd) {
  PRINTF("GSM (%02d) <- %s\r\n", strlen(cmd), cmd);
  sim800h_writeline(cmd);
}

void sim800h_expect_urc(int n) {
  char response[128] = {0};
  do {
    sim800h_readline(response, 127);
    PRINTF("GSM .... ?? %s\r\n", response);
  } while (check_urc(response) != n);
}

bool sim800h_expect(const char *expected) {
  char response[255] = {0};
  size_t len, expected_len = strlen(expected);
  while (true) {
    len = sim800h_readline(response, 127);
    if (check_urc(response) >= 0) continue;
    PRINTF("GSM (%02d) -> %s\r\n", len, response);
    return strncmp(expected, (const char *) response, MIN(len, expected_len)) == 0;
  }
}
